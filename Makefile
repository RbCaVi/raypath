
#.SILENT:

flags := -Wall -Wpedantic -pedantic-errors -Wextra
compileflags := -std=c++2a -I. -isystem/home/rvail/glad/output/include -isystem/home/rvail/glm
linkflags := -Wl,-rpath=/opt/gcc-12.2.0/lib64

compilecmd = g++ $(flags) $(compileflags) $(CXXFLAGS)
compilecmdc = gcc $(flags) $(compileflags) $(CFLAGS)
linkcmd = g++ $(flags) $(linkflags) $(LDFLAGS)

.PHONY: all clean

all: main

%.o: %.cpp
	 $(compilecmd) -c $< -o $@

%.o: %.c
	 $(compilecmdc) -Wno-pedantic -c $< -o $@

main.o:
shader.o:
glad.o:

main: main.o shader.o glad.o
	$(linkcmd) $^ \
	-lSDL2 -lSDL2_image \
	-lGL -lGLEW \
	-ldl \
	$(opencv-libs) \
	-o $@

clean:
	rm -f *.o
	rm -f main
