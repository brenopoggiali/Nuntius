.PHONY: clean
all:
	# Building server
	cd server; \
	mkdir build;
	cd server/src; \
	g++ -std=c++11 -I ../include *.cpp -o ../build/server -lpthread -fprofile-arcs -ftest-coverage -g -O0
	
	# Building client
	cd client; \
	mkdir build;
	cd client/src; \
	g++ -std=c++11 -I ../include *.cpp -o ../build/client -lpthread -fprofile-arcs -ftest-coverage -g -O0