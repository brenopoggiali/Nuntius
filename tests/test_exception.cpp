#include "server.h"
#include "client.h"
#include "exception.h"
#include "doctest.h"

TEST_CASE("Test Exceptions in Server"){
	CHECK_NOTHROW(Server s(5555));
	try{
		Server s2(5555);
	}
	catch(std::exception &e){
		CHECK_EQ(e.what() == "Port already in use");
	}
}
TEST_CASE("Test Exceptions in Client"){
	Server s(5555);
	CHECK_NOTHROW(Client c("127.0.0.1", 5555, "client1", "channel_name"));
	try{
	 Client c1("127.0.0.1", 5555, "client1", "channel_name");
	}
	catch(std::exception &e){
		CHECK_EQ(e.what() == "Nickname already in use");
	}
}