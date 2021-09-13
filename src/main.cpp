#include <iostream>
#include "kitchen.h"

int main()
{
    Kitchen kithen;

    for(int i = 0; i < 2; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(std::rand()%10+5+1));
        kithen.setOrder(static_cast<Kitchen::Food>(rand()%5+1));
    }
    
        
    std::cout<<"yes"<<std::endl;
}