all:
	@make -C build all

install:
	@make -C build install

docs:
	cd build; make docs
	@make VERBOSE=1 -C doc/latex all

gdbtest:
	@make -C build gdbtest

test:
	@make -C build rtest

mtest:
	@make -C build mtest

clean:
	@make -C build clean

dist:
	./scripts/make_dist_tarball.sh
