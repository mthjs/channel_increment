#define CATCH_CONFIG_MAIN

#include <channel.hpp>

#include <catch2/catch.hpp>

#include <chrono>
#include <string>
#include <thread>


SCENARIO("A new channel", "[channel]") {
   Channel<std::string> channel;

   GIVEN("That no message has been sent over this channel yet") {

      WHEN("Checking if it is empty") {
         THEN("``true`` is returned") {
            REQUIRE(channel.empty());
         }
      }

      WHEN("Trying to receive a message") {
         THEN("Noting is received and ``false`` is returned") {
            std::string received = "Waiting for a message :)";
            REQUIRE(!channel.try_receive(received));
            REQUIRE(received == "Waiting for a message :)");
         }
      }

      WHEN("A message is send") {
         THEN("The channel is no longer empty") {
            channel.send("Hello there!");
            REQUIRE(!channel.empty());
         }
         THEN("The message can be received") {
            std::string received;
            channel.send("Hello there!");
            channel.try_receive(received);
            REQUIRE(received == "Hello there!");
         }
      }
   }

   GIVEN("Several messages have been send in order") {
      channel.send("one");
      channel.send("two");
      channel.send("three");

      THEN("The order should be te same") {
         std::string received;
         channel.blocking_receive(received);
         REQUIRE(received == "one");
         channel.blocking_receive(received);
         REQUIRE(received == "two");
         channel.blocking_receive(received);
         REQUIRE(received == "three");
      }
   }
}

void ping(Channel<std::string>& channel) {
   for(;;) {
      channel << "ping!";
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
   }
}

SCENARIO("Somebody is sending messages", "[channel]") {
   Channel<std::string> channel;
   std::thread pinger(&ping, std::ref(channel));
   pinger.detach();

   WHEN("Waiting for a message") {
      THEN("A message is received") {
         std::string received;
         channel >> received;
         REQUIRE(received == "ping!");
      }
   }
}

void correspond(Channel<std::string>& inbox, Channel<std::string>& outbox) {
   for(;;) {
      std::string message;
      inbox >> message;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      outbox << "Thank you!";
   }
}

SCENARIO("Somebody is awaiting messages", "[channel]") {
   Channel<std::string> outbox;
   Channel<std::string> inbox;
   std::thread pen_pal(&correspond, std::ref(outbox), std::ref(inbox));
   pen_pal.detach();

   WHEN("A message through the awaited channel") {
      THEN("The message is taken from the channel") {
         outbox << "Hello there!";
         std::string reply;
         inbox >> reply;
         REQUIRE(outbox.empty());
      }
   }
}

void block(Channel<std::string>& inbox, Channel<std::string>& outbox) {
   std::string message;
   inbox.blocking_receive(message);
   outbox.send(message);
}

SCENARIO("Somebody is blocking receiving", "[channel]") {
   Channel<std::string> inbox;
   Channel<std::string> outbox;
   std::thread blocker(&block, std::ref(outbox), std::ref(inbox));
   blocker.detach();

   THEN("Everything still works") {
      outbox.send("Hello there!");
      std::string message;
      inbox.blocking_receive(message);
      REQUIRE(message == "Hello there!");
   }
}
