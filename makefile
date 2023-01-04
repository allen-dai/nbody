CC = gcc
WCC = clang
SOURCE = src/sdl2.c\
		 src/body.c\
		 src/body.h\
		 src/system.c\
		 src/system.h\
		 src/geo.c\
		 src/geo.h

TARGET = tangerine
OBJS = src/body.o src/system.o src/geo.o

WASM_OBJS = src/body.o src/geo.o src/wasm.o
WASM_FLAGS = -Wall --target=wasm32 -D WASM -nostdlib
WASM_EXPORTS = --export-all

sdl2: $(SOURCE)
	$(CC) $(FLAGS) -c src/body.c -o src/body.o
	$(CC) $(FLAGS) -c src/system.c -o src/system.o
	$(CC) $(FLAGS) -c src/geo.c -o src/geo.o
	$(CC) $(FLAGS) $(OBJS) src/sdl2.c -o $(TARGET) -lSDL2 -lm

wasm: $(SOURCE)
	$(WCC) $(WASM_FLAGS) -c src/body.c -o src/body.o
	$(WCC) $(WASM_FLAGS) -c src/geo.c -o src/geo.o
	$(WCC) $(WASM_FLAGS) -c src/wasm.c -o src/wasm.o
	wasm-ld -m wasm32 --no-entry $(WASM_EXPORTS) --allow-undefined $(WASM_OBJS) -o index.wasm

run: $(TARGET)
	./$(TARGET)
