#include "server.h"
#include "doctest.h"

TEST_CASE("Test Server Constructor"){
	CHECK_NOTHROW(Server s(5555));
}