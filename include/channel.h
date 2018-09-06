#ifndef CHANNEL_INCREMENT_CHANNEL
#define CHANNEL_INCREMENT_CHANNEL

#include <mutex>
#include <queue>

template<typename ValueType>
struct Channel
{
   void send(const ValueType& message)
   {
      std::lock_guard<std::mutex> guard(key);
      messages.push(message);
   }

   void receive(ValueType& recipient)
   {
      std::lock_guard<std::mutex> guard(key);
      received.wait(key, [this]() { return !messages.empty(); });
      recipient = messages.front();
      messages.pop();
   }

   bool try_receive(ValueType& recipient)
   {
      bool succeded = false;
      if (key.try_lock()) {
         std::lock_guard<std::mutex> guard(key);
         if (!messages.empty()) {
            recipient = messages.front();
            messages.pop();
            succeded = true;
         }
      }
      return succeded;
   }

   void operator<<(const ValueType& message) { send(message); }
   Receiver operator>>(const ValueType& recipient)
   {
      return Receiver(this, recipient);
   }

private:
   std::mutex key;
   std::condition_variable_any received;
   std::queue<ValueType> messages;

   struct Receiver
   {
      Receiver(Channel* incomming, ValueType& recipient)
        : incomming(incomming)
        , recipient(recipient)
      {}
      Receiver(Receiver& other)
        : Receiver(other.incomming, other.recipient)
      {
         other.incomming = nullptr;
      }

      operator bool()
      {
         Channel* receiving = incomming;
         incomming = nullptr;
         return receiving && receiving->try_receive(recipient);
      }

      ~Receiver()
      {
         if (incomming)
            incomming->receive(recipient);
      }

   private:
      Channel* incomming;
      ValueType& recipient;
   };
};

#endif
