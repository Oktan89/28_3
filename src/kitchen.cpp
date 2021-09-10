#include "kitchen.h"

Kitchen::Kitchen()
{
    _order = new std::queue<Food>;
}

Kitchen::~Kitchen()
{
    delete _order;
}

std::size_t Kitchen::setOrder(const Food food)
{
    {
         std::unique_lock ul(m_oreder);
        _order->push(food);
    }
   
    std::shared_lock sl(m_oreder);
    return _order->size();
}

std::string Kitchen::getNameFood(const Food food)
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