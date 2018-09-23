#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <channel.hpp>


TEST_CASE("bla bla", "[channel]") {
   REQUIRE(true == false);
}

SCENARIO("asdf", "[channel]") {
   GIVEN("foo bar") {
      WHEN("42") {
         THEN("hello there!") {
            REQUIRE(true == false);
         }
      }
   }
}
