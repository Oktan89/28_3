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
    _kitchen_process = std::thread(kitchen_process, std::ref(*this));
    _kitchen_process.detach();
}

Kitchen::~Kitchen()
{   
    delete _order;
    std::shared_lock sl(s_order);
    std::cout<<"Kitchen close"<<std::endl;
}

void Kitchen::kitchen_process(Kitchen &kitchen)
{
    std::srand(std::time(nullptr)); //Новое зерно для RAND, в потоках нужно вызывать заново
    std::unique_lock<std::mutex> ul(kitchen.m_order);
    std::cout << "Kitchen start...\n";

    while (true)
    {
        if (kitchen.end_order) //Проверка на выход из потока
            break;
        //Ожадние события первого заказа или завершения заказов
        kitchen.cv.wait(ul, [&]() { return kitchen.status_order || kitchen.end_order; });
        while (kitchen._order->size() > 0)
        {
            if (kitchen.end_order) //Проверка на выход из потока
                break;
            Food cook_food = kitchen._order->front();
            std::cout << "Cook " << kitchen.getNameFood(cook_food) << " ...\n";
            kitchen._order->pop();
            kitchen.status_order = false;
            ul.unlock();
            //Эмуляция приготовления блюда
            std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 15 + 5 + 1));
            kitchen.m_extradition.lock();
            kitchen._extradition->push(cook_food);
            kitchen.m_extradition.unlock();
            ul.lock();
            std::cout << "End cook " << kitchen.getNameFood(cook_food) << ".\n";
        }
    }
    //Выход из потока с увидомлением родительского потока
    std::cout << "Thread kitchen close.\n";
    kitchen.end_cook = true;
    std::notify_all_at_thread_exit(kitchen.cv, std::move(ul));
}

//Создаем поток онлайн-заказ 
void Kitchen::orderStart()
{
    _order_process = std::thread(order_process, std::ref(*this));
    _order_process.detach();
}

//Поток онлайн-заказ который каждые 5–10 секунд случайно 
//генерирует блюдо из пяти доступных: пицца, суп, стейк, салат, суши.
void Kitchen::order_process(Kitchen& kitchen)
{
    std::srand(std::time(nullptr)); //Новое зерно для RAND, в потоках нужно вызывать заново
    std::unique_lock<std::mutex> sh(kitchen.m_order); //Блокировка для проверки выхода из потока
    std::cout<<"Thread order start...\n";
    while(true)
    {
        
        if(kitchen.end_order) //Проверка на выход из потока
            break;
        sh.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(std::rand()%10+5+1));
        kitchen.setOrder(static_cast<Kitchen::Food>(rand()%5+1));
        
        sh.lock();  
    }
    //Выход из потока с увидомлением родительского потока
    std::cout<<"Thread order close.\n";
    kitchen.end_ord = true;
    std::notify_all_at_thread_exit(kitchen.cv, std::move(sh)); 
    
}

void Kitchen::endThread()
{
    std::unique_lock<std::mutex> ul(m_order);
    end_order = true; //Запускаем выход из дочерних потоков
    cv.wait(ul, [=](){return end_cook && end_ord;}); //Ждем завершения всех потоков
}

void Kitchen::setOrder(const Food &food)
{
    std::string text = "An order for a " + getNameFood(food) + " dish has been accepted.\n";
    std::unique_lock sl(m_order);
    if (end_order) //Проверка на выход из потока
        return;
    _order->push(food);
    status_order = true;
    std::cout << text;
    cv.notify_all();
}

bool Kitchen::getCookOrder()
{
    std::unique_lock<std::mutex> ul(m_extradition);
    if(_extradition->size()==0)
    {
        ul.unlock();
        m_order.lock();
        std::cout<<"[The order is not ready for delivery.]\n";
        m_order.unlock();
        return false;
    }
    else
    {
        Food food = _extradition->front();
        _extradition->pop();
        ul.unlock();
        m_order.lock();
        std::cout<<"|----->> The order "<<getNameFood(food)<<" is being delivered.\n";
        m_order.unlock();
        return true;
    }
}

std::size_t Kitchen::getSizeOrder()
{
    std::shared_lock sl(s_order);
    return _order->size();
}

std::string Kitchen::getNameFood(const Food &food)
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