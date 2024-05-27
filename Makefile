COMPILER = clang
SOURCE_LIBS = -Ilib/
OSX_OPT = -L lib/ -framework CoreVideo -framework GLUT -framework Cocoa -framework IOKit -framework OpenGL lib/libraylib.a
OSX_OUT = -o "bin/build_osx"
MAIN = src/main.c

build_osx:
	$(COMPILER) $(MAIN) $(SOURCE_LIBS) $(OSX_OPT) $(OSX_OUT)

run:
	bin/build_osx

build_n_run:
	make build_osx && clear && make run

