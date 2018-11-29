#include "client.h"
<<<<<<< HEAD
#include "server.h"
#include "doctest.h"

TEST_CASE("Test Client Constructor"){
	TCPserver(5555);
=======
#include "doctest.h"

TEST_CASE("Test Client Constructor"){
>>>>>>> 76b57a7f37f2132d7bf7620c7678c70c3d6b3480
	CHECK_NOTHROW(Client c("127.0.0.1", 5555, "client1", "channel_name"));
}