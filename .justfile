# "just" scripts

initialized_marker := ".initialized"
initialized := path_exists(initialized_marker)

# Show all recipes
@default:
	just --list


# Clean directory forcing a new build
@clean:
	rm -rf build/cmake {{initialized_marker}}


# Initialize repository for use
@init:
	if ! {{initialized}}; then \
		mkdir -p build/cmake/debug; \
		cd build/cmake/debug; \
		cmake -DCMAKE_BUILD_TYPE=Debug ../../../src; \
		cd ../../..; \
		touch {{initialized_marker}}; \
	fi


# Build the project in Debug mode
@build: init
	cmake --build build/cmake/debug

@run: build
	./build/cmake/debug/elf2pgz elf-main pgz-main

# Build and install the project in Release mode, defaulting to $HOME/.local
@install directory="$HOME/.local" sudo="":
	rm -rf build/cmake/release
	mkdir -p build/cmake/release
	cd build/cmake/release; cmake -DCMAKE_INSTALL_PREFIX={{directory}} -DCMAKE_BUILD_TYPE=Release ../../../src; cd ../../..
	cmake --build build/cmake/release -- -j
	{{sudo}} cmake --install build/cmake/release
