#ifndef CHANNEL_INCREMENT_CHANNEL
#define CHANNEL_INCREMENT_CHANNEL

#include <condition_variable>
#include <mutex>
#include <queue>


template<typename ValueType>
struct Channel
{
   struct Receiver
   {
      Receiver(Channel<ValueType>* incomming, ValueType& recipient)
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
         Channel<ValueType>* receiving = incomming;
         incomming = nullptr;
         return receiving && receiving->try_receive(recipient);
      }

      ~Receiver()
      {
         if (incomming)
            incomming->receive(recipient);
      }

   private:
      Channel<ValueType>* incomming;
      ValueType& recipient;
   };

public:
   bool empty() {
      return messages.empty();
   }

   void send(const ValueType& message)
   {
      std::lock_guard<std::mutex> guard(key);
      messages.push(message);
      received.notify_all();
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
      std::unique_lock<std::mutex> guard(key, std::try_to_lock);
      if(guard.owns_lock()){
         if (!empty()) {
            recipient = messages.front();
            messages.pop();
            succeded = true;
         }
      }
      return succeded;
   }

   void operator<<(const ValueType& message) { send(message); }
   Receiver operator>>(ValueType& recipient)
   {
      return Receiver(this, recipient);
   }

private:
   std::mutex key;
   std::condition_variable_any received;
   std::queue<ValueType> messages;
};

#endif
