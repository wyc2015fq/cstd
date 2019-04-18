# 多线程获取随机数性能对比（C vs C++） - alw2009的博客 - CSDN博客
2018年04月22日 12:10:28[traveler_zero](https://me.csdn.net/alw2009)阅读数：450
测试结论：C 完胜 C++
测试代码：
```cpp
#include <iostream>
#include <random>
#include <functional>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace std::chrono;
class Rand {
public:
    Rand(int l, int h):dist{l, h}{}
    int get() {
        return dist(gen);
    }
private:
    mt19937 gen{random_device{}()};
    uniform_int_distribution<> dist;
};
class Crand {
public:
    Crand(int lo, int hi):l(lo),h(hi){srand48_r(time(nullptr), &randBuffer);}
    int get() const {
        long ret;
        lrand48_r((drand48_data *)(&randBuffer), &ret);
        return ret%(h-l) + l;
    }
private:
    drand48_data randBuffer;
    int l, h;
};
void randint(int l, int h) {
    int a = 0;
    Crand r{l, h};
    //Rand r{l, h};
    for (int i = 0; i < 1000000; ++i)
        a = r.get();
}
int main() {
    auto t1 = high_resolution_clock::now();
    thread th1{randint, 1, 1000000};
    thread th2{randint, 1, 1000000};
    thread th3{randint, 1, 1000000};
    thread th4{randint, 1, 1000000};
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    auto t2 = high_resolution_clock::now();
    cout << "time:" << duration_cast<milliseconds>(t2-t1).count() << " ms" << endl;
    return 0;
}
```
