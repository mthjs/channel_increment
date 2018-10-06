#define CATCH_CONFIG_MAIN

#include <channel.hpp>

#include <catch2/catch.hpp>

#include <string>
#include <thread>


SCENARIO("a new channel", "[channel]") {
   GIVEN("that no message has been sent over this channel yet") {
      Channel<std::string> channel;

      WHEN("checking if it is empty") {
         THEN("``true`` is returned") {
            REQUIRE(channel.empty() == true);
         }
      }

      WHEN("trying to receive a message") {
         THEN("noting is received and ``false`` is returned") {
            std::string received = "Waiting for a message :)";
            REQUIRE(channel.try_receive(received) == false);
            REQUIRE(received == "Waiting for a message :)");
         }
      }

      WHEN("a message is send") {
         THEN("the channel is no longer empty") {
            channel.send("Hello there!");
            REQUIRE(channel.empty() == false);
         }
         THEN("the message can be received") {
            std::string received;
            channel.send("Hello there!");
            channel.try_receive(received);
            REQUIRE(received == "Hello there!");
         }
      }
   }
}
