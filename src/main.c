#include <stdio.h>
#include "elf.h"

#define PGZ_32BIT 'z'

static Elf32_Half
swaph(Elf32_Half h) {
	return ((h << 8) | (h >> 8)) & 0xFFFF;
}

static Elf32_Word
swapw(Elf32_Word h) {
	h = (h << 16) | (h >> 16);
	h = ((h & 0xFF00FF00) >> 8) | ((h & 0x00FF00FF) << 8);
	return h;
}


static void
fputll(uint32_t value, FILE* file) {
    fputc(value & 0xFFu, file);
    fputc((value >> 8u) & 0xFFu, file);
    fputc((value >> 16u) & 0xFFu, file);
    fputc((value >> 24u) & 0xFFu, file);
}


static void
printUsage(void) {
	printf("Usage: elf2pgz INFILE OUTFILE\n");
}


int
main(int argc, const char* argv[]) {
	if (argc < 3) {
		printUsage();
		return 0;
	}

	const char* in_name = argv[1];
	const char* out_name = argv[2];

	FILE* infile = fopen(in_name, "rb");
	if (infile == NULL) {
		fprintf(stderr, "Unable to open file %s for reading\n", in_name);
		return 1;
	}

	FILE* outfile = fopen(out_name, "wb");
	if (outfile == NULL) {
		fprintf(stderr, "Unable to open file %s for writing\n", out_name);
		return 1;
	}

	Elf32_Ehdr header;
	// Check if this is an ELF file we like
	if (fread(&header, sizeof(header), 1, infile) == 1) {
		if (header.e_ident[EI_MAG0] == ELFMAG0
		&& header.e_ident[EI_MAG1] == ELFMAG1
		&& header.e_ident[EI_MAG2] == ELFMAG2
		&& header.e_ident[EI_MAG3] == ELFMAG3
		&& header.e_ident[EI_CLASS] == ELFCLASS32
		&& header.e_ident[EI_DATA] == ELFDATA2MSB
		&& swaph(header.e_type) == ET_EXEC
		&& swaph(header.e_machine) == EM_68K) {

			// PGZ "header"
			fputc(PGZ_32BIT, outfile);

			// Copy sections with data
			Elf32_Off offset = swapw(header.e_phoff);
			for (Elf32_Half sectionIndex = 0; sectionIndex < swaph(header.e_phnum); ++sectionIndex, offset += swaph(header.e_phentsize)) {
				fseek(infile, offset, SEEK_SET);
				Elf32_Phdr pheader;
				if (fread(&pheader, sizeof(pheader), 1, infile) == 1) {
					if (swapw(pheader.p_type) == PT_LOAD) {
						// PGZ section information
						fputll(swapw(pheader.p_vaddr), outfile);
						fputll(swapw(pheader.p_memsz), outfile);

						// Copy bytes
						fseek(infile, swapw(pheader.p_offset), SEEK_SET);
						Elf32_Word i;
						for (i = 0; i < swapw(pheader.p_filesz); ++i) {
							fputc(fgetc(infile), outfile);
						}
						// Pad with zeroes if necessary
						while (i++ < swapw(pheader.p_memsz)) {
							fputc(0, outfile);
						}
					}
				}
			}

			// Entry address section
			fputll(swapw(header.e_entry), outfile);
			fputll(0, outfile);
		}
	}

	fclose(infile);
	fclose(outfile);
}