#include <iostream>
#include "kitchen.h"

int main()
{
    Kitchen kithen;

    kithen.setOrder(Kitchen::Food::salad);
    kithen.setOrder(Kitchen::Food::soup);
    std::this_thread::sleep_for(std::chrono::seconds(20));
}