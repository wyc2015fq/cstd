# 算法：如何使用C++实现一个简单的集合类 - 文章 - 伯乐在线
原文出处： [kirineko](https://segmentfault.com/a/1190000005053016)
来自于C++程序设计的一个题目。实现一个集合类，要求实现以下4个操作。
- 向集合中添加元素，如果集合中已存在元素则不添加
- 从集合中移除元素，移除之前需要先判断集合中元素是否存在
- 重载`+`运算符，用以实现集合的求并集运算
- 重载`*`运算符，用以实现集合的求交集运算
# 1.类的整体设计
该问题需要模拟实现集合类，我们可以使用数组来模拟集合，于是使用`int items[100]`用来存放集合中的数据。为了实现数组的遍历，这就需要一个整数用来表示数组中元素的个数，于是使用`int number`来表示数组中元素的个数；此外，为了实现题目的需求，设计以下四个函数：
- 使用`add_item(int item)`成员函数向数组中添加元素
- 使用`remove_item(int item)`成员函数向数组中移除元素
- 重载`operator+`表示集合的求并集运算
- 重载`operator*`表示集合的求交集运算
由于向集合添加元素之前，必须确保集合中不存在该元素；在从集合中移除元素之前，必须确保集合中存在该元素，因此添加`is_exist(int item)`方法用以判断集合中是否存在这个元素；此外为了显示集合，添加`display()`方法, 基本设计如下:

C++
```
class Set
{
public:
    int items[100]; //定义一个数组作为容器存放100个集合元素
    int number; //定义数字i表示集合中元素的个数
    //构造函数和析构函数
    Set() {
        this->number = 0;
        memset(this->items,0,sizeof(items));
    }
    //初始化方法
    int init(int items[], int num);
    //添加元素
    bool add_item(int item);
    //删除元素
    bool remove_item(int item);
    //求集合的并集
    Set operator+ (Set set2);
    //求集合的交集
    Set operator* (Set set2);
    //显示集合元素
    int display();
    //判断集合当中是否存在item,返回元素在集合中的位置，不存在返回-1
    int is_exist(int item);
};
```
# 2.构造函数

C++
```
Set() {
    this->number = 0;
    memset(this->items,0,sizeof(items));
}
```
在构造函数中，我们对数组进行初始化，声明完数组之后，如果不进行初始化，数组元素是随机值，在C语言中，变量不进行初始化都会被分配随机值。为了避免这种情况，我们使用memset函数对数组items所有元素全部赋值为0；同时，由于此时数组中没有元素，即元素个数为0，我们的number也应当赋值为0.
# 3.判断数组中是否包含元素 item

C++
```
int Set::is_exist(int item)
{
    for(int i=0; i< this->number; i++) {
        if(this->items[i] == item) {
            return i;
        }
    }
    return -1;
}
```
该函数用于判断数组中是否存在item元素，如果存在就返回item元素的位置，如果不存在就返回-1. 判断方法非常简单，写一个for循环从items[0]-items[number-1]一个一个进行遍历。如果相等，直接返回i，此时i就是数组中item元素的位置；如果遍历完整个数组之后，都没有发现与item相等的数组元素，说明数组中不存在item这个元素，于是返回-1.
# 4.向数组中添加元素

C++
```
bool Set::add_item(int item)
{
    if(is_exist(item) >= 0 || this->number >= 100) {
        return false;
    }
    this->items[this->number] = item;
    this->number++;
    return true;
}
```
首先判断数组中是否存在该元素，如果存在则不能再向集合中添加元素，直接返回false，如果不存在，则向数组中的number所指向的那个位置添加该元素，然后number作为数组元素个数的指示器+1，这样就完成了添加元素。
# 5.保护数组元素不被修改
写到这里，我们发现，数组元素个数指示器`this->number`，对于该问题的几个算法都起到了核心的作用，首先，我们依赖于数组元素个数指示器遍历数组，如果number值遭到修改，会导致无法遍历数组。举个例子来说，当我们调用下列语句以后:

C++
```
Set set1;
set1.add_item(1);
set1.add_item(2);
set1.add_item(3);
```
集合set1中的数组`items`变为`[1,2,3]`，数组元素个数指示器`number=3`，此时，如果我们还想向集合`set1`中添加元素`20`，我们需要利用`number=3`这个指示器，让`set1.items[number]`=20,并且让`number+1`以指向下一个位置,即`number=4`。但是如果用户手动修改`number`值，比如`set1.number=50`;此时，我们的`number`就不再能指示数组元素的正确位置，从而导致以上所有算法所依赖的`number`失效，因此，我们需要对数组本身，以及数组元素个数指示器`number`进行私有化，以避免用户随意篡改。于是:

C++
```
class Set
{
public:
    //构造函数和析构函数
    Set() {
        this->number = 0;
        memset(this->items,0,sizeof(items));
    }
    //初始化方法
    int init(int items[], int num);
    //添加元素
    bool add_item(int item);
    //删除元素
    int remove_item(int item);
    //求集合的并集
    Set operator+ (Set set2);
    //求集合的交集
    Set operator* (Set set2);
    //显示集合元素
    int display();
    //判断集合当中是否存在item,返回元素在集合中的位置，不存在返回-1
    int is_exist(int item);
private:
    int items[100]; //定义一个数组作为容器存放100个集合元素
    int number; //定义数字i表示集合中元素的个数
};
```
# 6. 从集合中移除元素

C++
```
bool Set::remove_item(int item)
{
    int pos = is_exist(item);
    if(pos == -1) return false;
    for(int i=pos; i< this->number-1; i++) {
        this->items[i] = this->items[i+1];
    }
    this->number--;
    return true;
}
```
首先检查要移除的元素在结合中是否存在，如果不存在，则直接返回`false`;其次，定位到集合中元素的位置，然后从这个位置开始将集合中剩余的元素逐个前移，最后集合元素指示器-1，并返回`true`.
# 7. 求两个集合的交集

C++
```
Set Set::operator* (Set set2)
{
    Set result;
    for(int i=0; i< this->number; i++) {
        if(set2.is_exist(this->items[i]) >= 0) {
            result.items[result.number] = this->items[i];
            result.number++;
        }
    }
    return result;
}
```
算法很简单，遍历集合A中的元素，对于A中的每一个元素判断在集合B中是否存在，如果存在就加入到集合C当中，最后返回集合C
# 8. 求两个集合的并集

C++
```
Set Set::operator+ (Set set2)
{
    Set result;
    for(int i=0; i<this->number; i++) {
        result.items[result.number] = this->items[i];
        result.number++;
    }
    for(int j=0; j<set2.number; j++) {
        if(result.is_exist(set2.items[j]) == -1) {
            result.items[result.number] = set2.items[j];
            result.number++;
        }
    }
    return result;
}
```
首先遍历集合A，将集合A中的元素全部加到集合C当中，然后遍历集合B，对于B中的每一个元素，首先判断是否在A中存在，如果不存在则将其加入到集合C中，最终返回集合C
