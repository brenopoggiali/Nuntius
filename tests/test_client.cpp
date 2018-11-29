#include "client.h"
#include "server.h"
#include "doctest.h"

TEST_CASE("Test Client Constructor"){
	TCPserver(5555);
	CHECK_NOTHROW(Client c("127.0.0.1", 5555, "client1", "channel_name"));
}