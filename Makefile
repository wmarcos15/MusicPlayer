COMPILER = clang
SOURCE_LIBS = -Ilib/
OSX_OPT = -L lib/ -framework CoreVideo -framework GLUT -framework Cocoa -framework IOKit -framework OpenGL lib/libraylib.a lib/libfutilities.a
OSX_OUT = -o "bin/build_osx"
MAIN = src/main.c
O_OUT = "bin/libfutilities.o"
A_OUT = "lib/libfutilities.a"
FUTILS = src/futilities.c

build.o:
	$(COMPILER) $(SOURCE_LIBS) -c $(FUTILS) -o $(O_OUT)

build.a:
	ar rcs $(A_OUT) $(O_OUT)

build_osx:
	-@mkdir ./bin
	@make build.o
	@make build.a
	# rm $(O_OUT)
	$(COMPILER) $(MAIN) $(SOURCE_LIBS) $(OSX_OPT) $(OSX_OUT)

run:
	bin/build_osx

build_n_run:
	make build_osx && clear && make run

