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
    std::thread _start;
    bool _work{true};
    std::queue<Food>* _extradition;
    std::mutex m_extradition;
    static void kitchenStart(Kitchen& kitchen);
public:
    
    Kitchen();
    ~Kitchen();
    std::string getNameFood(const Food& food);
    void setOrder(const Food& food);
    std::size_t getSizeOrder();
};