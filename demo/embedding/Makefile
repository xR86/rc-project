
all:
	echo "Set param"

high:
	gcc embed-high.c -o embed-high.bin -lpython2.7 -lm -L/usr/lib/python2.7/config

pure:
	gcc embed-pure.c -o embed-pure.bin -lpython2.7 -lm -L/usr/lib/python2.7/config

py:
	python setup.py build

clean:
	rm -rf build/ embed-pure.bin embed-high.bin