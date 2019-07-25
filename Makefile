process.so: process.cpp
	g++ process.cpp -g -I/usr/include/python3.6 -lrt -lpthread -lpython3.6m -fpic -shared -o process.so

clean:
	rm -f environ.so

