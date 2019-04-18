# 趣味编程：CPS风格代码（C++11, C++14版） - zwvista - 博客园

## [趣味编程：CPS风格代码（C++11, C++14版）](https://www.cnblogs.com/zwvista/p/7696485.html)

### CPS风格代码（C++11版）

```
#include <iostream>
using namespace std;

int add(int x, int y){return x + y;}
int square(int x) {return x * x;}
int pythagoras(int x, int y) {return add(square(x), square(y));}
template<typename F>
void add_cps(int x, int y, F f) {f(add(x, y));}
template<typename F>
void square_cps(int x, F f) {f(square(x));}
template<typename F>
void pythagoras_cps(int x, int y, F f) {
    square_cps(x, [=](int x_squared){
        square_cps(y, [=](int y_squared){
            add_cps(x_squared, y_squared, f);
        });
    });
}

int main()
{
    cout << pythagoras(3, 4) << endl; // 25
    pythagoras_cps(3, 4, [](int x){cout << x << endl;}); // 25
}
```

### CPS风格代码（C++14版）

```
#include <iostream>
using namespace std;
 
int add(int x, int y){return x + y;}
int square(int x) {return x * x;}
int pythagoras(int x, int y) {return add(square(x), square(y));}
auto add_cps = [](int x, int y, auto f) {f(add(x, y));};
auto square_cps = [](int x, auto f) {f(square(x));};
auto pythagoras_cps = [](int x, int y, auto f) {
    square_cps(x, [=](int x_squared){
        square_cps(y, [=](int y_squared){
            add_cps(x_squared, y_squared, f);
        });
    });
};
 
int main()
{
    cout << pythagoras(3, 4) << endl; // 25
    pythagoras_cps(3, 4, [](int x){cout << x << endl;}); // 25
}
```


