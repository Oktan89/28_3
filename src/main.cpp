#include <iostream>
#include "kitchen.h"

int main()
{
    Kitchen kithen;

   kithen.orderStart();
   int delivery = 0;
   while(delivery < 10)
   {
       std::this_thread::sleep_for(std::chrono::seconds(30));
       if(kithen.getCookOrder())
            ++delivery;
   }

   kithen.endThread();
   std::cout<<"["<<delivery<< "] order delivered."<<std::endl; 
         
}