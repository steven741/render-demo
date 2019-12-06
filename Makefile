all: src/main.c
	$(CC) `pkg-config --cflags --libs sdl2 glew` $^ -o main

clean:
	$(RM) main
