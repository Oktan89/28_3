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
    std::cout<<"Kitchen close"<<std::endl;
}



void Kitchen::kitchen_process(Kitchen& kitchen)
{
    std::unique_lock<std::mutex> ul(kitchen.m_order);
    std::cout<<"Kitchen start...\n";

    while(true)
    {
        if(kitchen.end_order) //Проверка на выход из потока
            break;
        kitchen.cv.wait(ul, [&](){return kitchen.status_order || kitchen.end_order;});
        for(int i = 0; i < kitchen._order->size(); ++i)
        {
            std::cout<<"kuc "<<kitchen.getNameFood(kitchen._order->front())<<"\n";
            ul.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(std::rand()%15+5+1));
            ul.lock();
        }
    }
    std::cout<<"Kitchen close.\n"; // не закрылась!!!

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
    std::notify_all_at_thread_exit(kitchen.cv, std::move(sh)); 
    std::cout<<"Thread order close.\n";
}

void Kitchen::endThread()
{
    std::unique_lock<std::mutex> ul(m_order);
    end_order = true; //Запускаем выход из дочерних потоков
    cv.wait(ul); //Ждем завершения
}

void Kitchen::setOrder(const Food& food)
{
    std::string text = "An order for a " + getNameFood(food) + " dish has been accepted\n";
     std::unique_lock sl(m_order);
    _order->push(food);
    status_order = true;
     std::cout<<text;
     cv.notify_all();
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