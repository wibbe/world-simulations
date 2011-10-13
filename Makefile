
all: compile

configure:
	mkdir -p build
	(cd build && cmake -DCMAKE_BUILD_TYPE=Release ..)

configure-debug:
	mkdir -p build
	(cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..)

compile:
	make -C build

gfx:
	./scripts/obj2opengl.pl --noMove --noScale gfx/environment_walls.obj
	./scripts/obj2opengl.pl --noMove --noScale gfx/environment_celling.obj

run:
	(cd build/bin && ./world-test)

clean:
	make -C build clean

distclean:
	(rm -rf build)
