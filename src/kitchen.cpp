#include "kitchen.h"
#include <ctime>
#include <iostream>
#include <string>
//#include <cassert>

Kitchen::Kitchen()
{
    std::srand(std::time(nullptr));
    _order = new std::queue<Food>;
    //_extradition = new std::queue<Food>;
    _kitchen_process = std::thread(kitchen_process, std::ref(*this));
    _kitchen_process.detach();
}

Kitchen::~Kitchen()
{   
    delete _order;
    std::shared_lock sl(s_order);
    std::cout<<"Kitchen delete"<<std::endl;
}



void Kitchen::kitchen_process(Kitchen& kitchen)
{
//??????????????????????????????????/
    
}

void Kitchen::setOrder(const Food& food)
{
    std::string text = "An order for a " + getNameFood(food) + " dish has been accepted\n";
     std::unique_lock sl(m_order);
    _order->push(food);
     std::cout<<text;
}

std::size_t Kitchen::getSizeOrder()
{
    std::shared_lock sl(s_order);
    return _order->size();
}

std::string Kitchen::getNameFood(const Food& food)
{
    std::string name_food;
    switch (food)
    {
    case Food::pizza:
        name_food = "pizza";
        break;
    case Food::salad:
        name_food = "salad";
        break;
    case Food::soup:
        name_food = "soup";
        break;
    case Food::steak:
        name_food = "steak";
        break;
    case Food::sushi:
        name_food = "sushi";
        break;
    
    default:
        name_food = "no order";
        break;
    }
    return name_food;
}