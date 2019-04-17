# 使用boost的deadline_timer实现一个异步定时器 - DoubleLi - 博客园






## 概述

最近在工作上需要用到定时器，然后看到boost里面的deadline_timer可以实现一个定时器，所以就直接将其封装成了ATimer类，方便使用，ATimer有以下优点：
- 可以支持纳秒、毫秒、秒、分、小时定时。
- 可以随时停止定时器。
- 支持单次调用。
- 因为使用了deadline_timer，所以定时比较准确。

ATimer和Qt的QTimer使用方法类似，若没有类似的Timer类，使用最原始的方法，我们的代码可能会是这样的：


```cpp
m_timerThread = std::thread([this]
{
    while (!m_bThreadStoped)
    {
        ++m_sleepCount;
        Sleep(SLEEP_DURATION_TIME);

        if (m_sleepCount == m_sleepAllCount)
        {
            m_sleepCount = 0;
            doSomeThing();
        }
    }
});
```


若使用QTimer的话，书写是这样的：


```cpp
QTimer *timer = new QTimer(this);
connect(timer, SIGNAL(timeout()), this, SLOT(update()));
timer->start(1000);
```


再来看看ATimer的使用：


```cpp
ATimer<> t;
t.bind([]{ std::cout << "Hello C++" << std::endl; });
t.start(1000);
```


从上面的例子可以看到，QTimer和ATimer的使用都非常方便，接下来看看ATimer的具体实现：


```cpp
// ATimer.hpp
#ifndef _ATIMER_H
#define _ATIMER_H

#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <boost/timer.hpp>
#include <boost/asio.hpp>

template<typename Duration = boost::posix_time::milliseconds>
class ATimer
{
public:
    ATimer() : m_timer(m_ios, Duration(0)), m_isSingleShot(false) {}
    ~ATimer()
    {
        stop();
    }

    void start(unsigned int duration)
    {
        if (m_ios.stopped())
        {
            return;
        }

        m_isActive = true;
        m_duration = duration;
        m_timer.expires_at(m_timer.expires_at() + Duration(m_duration));
        m_func = [this]
        {
            m_timer.async_wait([this](const boost::system::error_code&)
            {
                for (auto& func : m_funcVec)
                {
                    func();
                }

                if (!m_isSingleShot)
                {
                    m_timer.expires_at(m_timer.expires_at() + Duration(m_duration));
                    m_func();
                }
            });
        };

        m_func();
        m_thread = std::thread([this]{ m_ios.run(); });
    }

    void stop()
    {
        m_ios.stop();
        if (m_thread.joinable())
        {
            m_thread.join();
        }
        m_isActive = false;
    }

    void bind(const std::function<void()>& func)
    {
        m_funcVec.emplace_back(func);
    }

    void setSingleShot(bool isSingleShot)
    {
        m_isSingleShot = isSingleShot; 
    }

    bool isSingleShot() const
    {
        return m_isSingleShot;
    }

    bool isActive() const
    {
        return m_isActive;
    }

private:
    boost::asio::io_service m_ios;
    boost::asio::deadline_timer m_timer;
    std::function<void()> m_func = nullptr;
    std::vector<std::function<void()>> m_funcVec;
    std::thread m_thread;
    unsigned int m_duration = 0;
    std::atomic<bool> m_isSingleShot;
    bool m_isActive = false;
};

#endif
```


下面是ATimer的具体使用例子：


```cpp
// main.cpp
#include <iostream>
#include "ATimer.hpp"

void test()
{
    std::cout << "Timer thread id: " << std::this_thread::get_id() << std::endl;
}

int main()
{
    std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

    ATimer<boost::posix_time::minutes> t0;
    t0.setSingleShot(true);// 单次调用
    t0.bind(test);
    t0.start(1);// 一分钟之后调用

    ATimer<> t;//默认使用毫秒定时器
    t.bind(test);
    t.bind([]{ std::cout << "Hello C++" << std::endl; });
    t.start(1000);//每1000ms调用一次

    std::cin.get();
    t0.stop();
    t.stop();
    std::cout << "Tiemr stop" << std::endl;

    std::cin.get();
    std::cout << "Process end" << std::endl;

    return 0;
}
```


from:http://www.cnblogs.com/highway-9/p/5737421.html









