CC = gcc
WCC = clang
SOURCE = src/particle.c\
		 src/geo.c\

TARGET = tangerine
OBJS = particle.o system.o geo.o

WASM_OBJS = particle.o geo.o wasm.o
WASM_FLAGS = -Wall --target=wasm32 -D WASM -nostdlib
WASM_EXPORTS = --export-all

sdl2: $(SOURCE)
	$(CC) -c $(SOURCE) src/sdl2.c src/system.c
	$(CC) $(FLAGS) $(OBJS) src/sdl2.c -o $(TARGET) -lSDL2 -lm

wasm: $(SOURCE)
	$(WCC) $(WASM_FLAGS) -c $(SOURCE) src/wasm.c
	wasm-ld -m wasm32 --no-entry $(WASM_EXPORTS) --allow-undefined $(WASM_OBJS) -o index.wasm

clean:
	rm *.o

run: $(TARGET)
	./$(TARGET)
