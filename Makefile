all:
	@( cd src/utils/src && make ) || exit 1

clean:
	@( cd src/utils/src && make clean ) || exit 1
