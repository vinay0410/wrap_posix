#process.so: process.cpp
#	g++ process.cpp -g -I/usr/include/python3.6 -lrt -lpthread -lpython3.6m -fpic -shared -o process.so

mymodule.so: mymodule.cpp
	g++ mymodule.cpp -g -I/usr/include/python3.6 -lrt -lpthread -lpython3.6m -fpic -shared -o mymodule.so

clean:
	rm -f environ.so

