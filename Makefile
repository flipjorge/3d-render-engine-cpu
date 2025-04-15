CC = gcc
SOURCE = ./src/*.c
INCLUDE = -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lSDL2
OUTPUT_FOLDER = ./dist
OUTPUT = $(OUTPUT_FOLDER)/main

build:
	mkdir -p $(OUTPUT_FOLDER)
	$(CC) $(SOURCE) $(INCLUDE) $(LIBS) -o $(OUTPUT)

run: build
	$(OUTPUT)

clean:
	rm -f OUTPUT_FOLDER/*