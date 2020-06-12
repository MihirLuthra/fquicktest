all:
	@( cd utils/src && make ) || exit 1

clean:
	@( cd utils/src && make clean ) || exit 1
