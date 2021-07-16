CC=gcc
CFLAGS=-Wall \
	-std=c99 \
	-I"C:\libsdl\include" \
	-L"C:\libsdl\lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-lm

build_and_run:
	$(CC) ./src/*.c $(CFLAGS) -o raycast.exe
	raycast.exe
clean:
	del raycast.exe