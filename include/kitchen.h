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
        no_order,
        pizza,
        soup,
        steak,
        salad,
        sushi
    };

private:
    std::queue<Food> *_order; //очередь заказов для кухни
    std::mutex m_order;
    std::shared_mutex s_order;
    std::condition_variable cv;
    bool end_order{false};          //завершение работы потока order
    bool status_order{false};       //есть ли заказ для кухни
    bool end_cook{false};           //статус завершени потока кухни
    bool end_ord{false};            //статус завершения потока заказов
    std::queue<Food> *_extradition; //очередь готовых заказов на доставку
    std::mutex m_extradition;
    std::thread _kitchen_process;
    std::thread _order_process;
    static void kitchen_process(Kitchen &kitchen);
    static void order_process(Kitchen &kitchen);

public:
    Kitchen();
    ~Kitchen();
    std::string getNameFood(const Food &food);
    void orderStart();
    void endThread();
    void setOrder(const Food &food);
    bool getCookOrder();
    std::size_t getSizeOrder();
};