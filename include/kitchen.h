#pragma once
#include <queue>
#include <string>
#include <thread>
#include <condition_variable>
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
    std::mutex m_oreder;
    
   // std::queue<Food>* _extradition;
   // std::mutex m_extradition;
    static bool _work;
    std::condition_variable cv_kitchen;
    std::thread _kitchen_process;
    static void kitchen_process(Kitchen& kitchen);
public:
    
    Kitchen();
    ~Kitchen();
    std::string getNameFood(const Food& food);
    void setOrder(const Food& food);
    std::size_t getSizeOrder();
};