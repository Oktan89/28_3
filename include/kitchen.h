#pragma once
#include <queue>
#include <string>
#include <thread>
#include <shared_mutex>


class Kitchen
{
public:
    enum class Food
    {
        no_order, pizza, soup, steak, salad, sushi
    };
private:
    std::queue<Food>* _order;
    std::shared_mutex m_oreder;
    //std::queue<Food>* _extradition;
    //std::mutex m_extradition;
public:
    
    Kitchen();
    ~Kitchen();
    std::string getNameFood(const Food& food);
    std::size_t setOrder(const Food& food);
};