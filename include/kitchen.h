#pragma once
#include <queue>
#include <string>
#include <thread>
#include <shared_mutex>
#include <condition_variable>


class Kitchen
{
public:
    enum class Food
    {
        no_order, pizza, soup, steak, salad, sushi
    };
private:
    std::queue<Food>* _order;
    std::mutex m_order;
    std::shared_mutex s_order;
    std::condition_variable cv;
 
   // std::queue<Food>* _extradition;
   // std::mutex m_extradition;
    std::thread _kitchen_process;
    static void kitchen_process(Kitchen& kitchen);
public:
    Kitchen();
    ~Kitchen();
    std::string getNameFood(const Food& food);
    void setOrder(const Food& food);
    std::size_t getSizeOrder();
};