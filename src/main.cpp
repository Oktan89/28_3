#include <iostream>
#include "kitchen.h"

int main()
{
    Kitchen kithen;

   kithen.orderStart();
  std::this_thread::sleep_for(std::chrono::seconds(10));
   kithen.endThread();
   // std::this_thread::sleep_for(std::chrono::seconds(15));
      
}