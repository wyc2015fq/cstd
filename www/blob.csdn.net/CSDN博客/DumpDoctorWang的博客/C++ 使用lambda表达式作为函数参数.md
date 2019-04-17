# C++ 使用lambda表达式作为函数参数 - DumpDoctorWang的博客 - CSDN博客





2018年08月21日 16:30:16[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1391








# lambda表达式参考
- [Ｃ++11 学习笔记 lambda表达式](https://blog.csdn.net/fjzpdkf/article/details/50249287)
- [C++中的Lambda表达式详解](https://blog.csdn.net/u010984552/article/details/53634513)
- [Lambda expressions](https://en.cppreference.com/w/cpp/language/lambda)

# 作为函数参数传递

由于时间有限，我直接贴代码了
- 模板参数

```cpp
#include <iostream>
#include <string>

template <typename F>
void print(F const &f){
    std::cout<<f()<<std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    int num = 101;
    auto a = [&]//以引用的方式捕获本函数中的变量
             () //无参数
             ->std::string {//返回值的类型为std::string
        return std::to_string(num);
    };
    print(a);
    num++;
    print(a);

    return 0;
}
```

调用print时，C++编译器会自动推断F的类型。运行上述程序，得到如下输出

```
Hello, World!
101
102
```

编译器需要支持c++11标准，有的编译器可能需要添加参数-std=c++11。
- 使用std::function

```cpp
#include <iostream>
#include <functional>
#include <string>

void print(std::function<std::string ()> const &f){
    std::cout<<f()<<std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    int num = 101;
    auto a = [&]//以引用的方式捕获本函数中的变量
             () //无参数
             ->std::string {//返回值的类型为std::string
        return std::to_string(num);
    };
    print(a);
    num++;
    print(a);

    return 0;
}
```

stdL::function是个模板类；模板参数：返回值类型+函数参数类型列表；例如std::function<int  (int, int)> f，f可以指向任意一个返回指为int，参数为两个int类型的函数。

观察上面的程序，lambda表达式无参数，返回值类型为string；故在print函数中声明一个类型为function<std::string ()>类型的变量f，就可以用来接收a。注意：f前面必须加上const，否则编译过不了。至于加不加&，需要视程序而定，一般加上。

### 把lambda表达式作为子线程的参数
- 示例

由于lambda表达式可以方便的捕获作用域中的变量，故可以作为子线程的参数

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>

int main() {

    std::atomic_uint num(0);//atomic_uint时原子操作类，在不同的线程中访问时，是线程安全的。
    // 无参数lambda表达式
    auto a = [&]() -> void {
        std::this_thread::sleep_for(std::chrono::milliseconds(num*20));//休眠i*2ms
        num += 100;
    };
    // 新建子线程
    std::thread t(a);
    t.join();
    int value = num;
    printf("%d\n",value);

    // 有参数lambda表达式
    auto b = [&](int i) -> void {
        std::this_thread::sleep_for(std::chrono::milliseconds(i));//休眠i*2ms
        num += i;
    };
    // 新建子线程
    std::thread tt(b,1000);
    tt.join();

    value = num;
    printf("%d\n",value);


    return 0;
}
```
- 陷阱

```cpp
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>

int main() {
    std::vector<std::thread> _threads;

    // 新建20个线程，每个线程打印i
    for(int i=0;i<20;i++){
        //定义一个lambda表达式a
        auto a = [=]() -> std::string {
            std::this_thread::sleep_for(std::chrono::milliseconds(i*20));//休眠i*2ms
            return std::to_string(i);
        };

        // 再定义一个wrapper包裹a
        auto wrapper = [&]() {
            //这里有其他操作
            std::string str = a();
            //这里有其他操作
            std::cout<<str<<"\t";
        };

        // 新建子线程，并把线程放到vector里面
        _threads.emplace_back(std::thread(wrapper));
    }

    //对所有线程调用join函数，避免主线程先于子线程退出。
    for(auto& t : _threads)
    {
        if(t.joinable()){
            t.join();
        }
    }
    std::cout<<std::endl;

    return 0;
}
```

观察上面的代码，理论上应该输出0～19。运行上面的程序，实际输出为

```
19	19	19	19	19	19	19	19	19	19	19	19	19	19	19	19	19	19	19	19	

Process finished with exit code 0
```

并不是预想的输出。直接调用wrapper()，可以得到正确的输出，但是把wrapper作为线程参数参数传进去时，结果却不正确。经过一番尝试，把定义wrapper的代码更改为如下代码

```cpp
auto wrapper = [=]() {//把&改为=，把捕获变量的引用改为捕获变量的副本
            //这里有其他操作
            std::string str = a();
            //这里有其他操作
            std::cout<<str<<"\t";
        };
```

再运行上面的程序，得到的结果为

```cpp
0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	

Process finished with exit code 0
```

初步的原因分析为，定义wrapper时，以引用的方式捕获a。然而在for循环中，虽然在更新a的定义，但是a的存储地址是固定的，故以引用的方式捕获a时，所有wapper里面的a都是最后一次更新的a。在子线程中调用wrapper时，自然都调用到在for循环中最后一次更新的a。定义wrapper时，以值的方式捕获a时，wrapper中保存的是a的副本，即使for循环中在更新a，也不影响之前定义的wapper。**所以，在wrapper中，一定要以值的方式捕获用到的变量。**



