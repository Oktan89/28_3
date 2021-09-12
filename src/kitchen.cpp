#include "kitchen.h"
#include <ctime>
#include <iostream>
#include <string>
//#include <cassert>

Kitchen::Kitchen()
{
    std::srand(std::time(nullptr));
    _order = new std::queue<Food>;
    _extradition = new std::queue<Food>;
    _start = std::thread(kitchenStart, std::ref(*this));
    _start.detach();
}

Kitchen::~Kitchen()
{
    _work = false; 
     delete _order;
}

void Kitchen::kitchenStart(Kitchen& kitchen)
{
    std::shared_lock sl(kitchen.m_oreder);
    bool work = kitchen._work;
    sl.unlock();
    while(work)
    {
        sl.lock();
        bool emptyOrder = kitchen._order->empty();
        sl.unlock();
        if(emptyOrder)
        {
            //Снимаем нагрузку на поцессор
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        else
        {
            Food order;
            std::string text;
            std::unique_lock ul(kitchen.m_oreder);
            order = kitchen._order->front();
            text  = "The " + kitchen.getNameFood(order) +" order arrived in the kitchen\n";
            kitchen._order->pop();
            std::cout<<text;
            ul.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(rand()%15+5));
            text = "The " + kitchen.getNameFood(order) + " order is ready\n";
            std::unique_lock eul(kitchen.m_extradition);
            kitchen._extradition->push(order);
            std::cout<<text;
        }
    }
}

void Kitchen::setOrder(const Food& food)
{
    std::string text = "An order for a " + getNameFood(food) + " dish has been accepted\n";
     std::unique_lock ul(m_oreder);
    _order->push(food);
    std::cout<<text;
}

std::size_t Kitchen::getSizeOrder()
{
    std::shared_lock sl(m_oreder);
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