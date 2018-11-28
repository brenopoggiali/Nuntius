#include "server.h"
#include "client.h"
#include "exception.h"
#include "doctest.h"

TEST_CASE("Test Exceptions in Client"){
	CHECK_NOTHROW(Client c("127.0.0.1", 5555, "client1", "channel_name"));
	CHECK_THROW(Client c1("127.0.0.1", 5555, "client1", "channel_name"));
}
TEST_CASE("Test Exceptions in Server"){
	CHECK_NOTHROW(Server s(5555));
	CHECK_THROW(Server s2(5555));
}