#include <iostream>
#include "kitchen.h"

int main()
{
    Kitchen kithen;

    std::cout<<kithen.getNameFood(Kitchen::Food::pizza);
    std::cout<<kithen.setOrder(Kitchen::Food::pizza);
   
}