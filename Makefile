
all: compile

configure:
	mkdir -p build
	(cd build && cmake -DCMAKE_BUILD_TYPE=Release ..)

configure-debug:
	mkdir -p build
	(cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..)

compile:
	make -C build

run:
	(cd build/bin && ./world-test)

clean:
	make -C build clean

distclean:
	(rm -rf build)
