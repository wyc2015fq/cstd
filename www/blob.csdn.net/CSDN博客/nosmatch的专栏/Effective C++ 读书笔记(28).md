# Effective C++ 读书笔记(28) - nosmatch的专栏 - CSDN博客
2013年07月20日 11:49:37[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：795
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款28：避免返回handle指向对象内部成分
> 
首先handle是指reference、指针和迭代器等都可以称之为handle，因为她们都是用来取得一个对象的，返回一个handle指
向对象的内部成分主要会造成下面的两种情况：
> 
1.增大了成员的访问权限，如下例子：
```cpp
class Point{
public:
	Point(int x, int y);
	...
	void setX(int x);
	void setY(int y);
};
struct RectData{
	Point ulhc;
	Point lrhc;
};
class Rectangle{
public:
	Point& upperLefter() const{return pData->ulhc;}
	Point& lowerRight() const{return pData->lrhc;}
	...
private:
	std::str::shared_ptr<RectData> pData;
};
```
上面定义了三个类，我们在看下面的应用：
```cpp
Point coord1(0,0);
Point coord2(1,1);
const Rectangle rect(coord1,coord2);
rect.upperLefter().setX(10);
```
> 
在上面的例子中，rect的成员函数是一个const的成员函数，正常情况下是不能修改对象的任何成员的，但是在这里他却
修改了对象的成员变量！之所以如此是因为函数upperLefter返回了一个reference的对象,这样通过该reference就可以获得对象内部
的成员变量，从而可以对成员进行修改！因此需要注意：成员变量的封装最多只能等于"返回其reference"的函数的访问级别，在
例子中；lrhc是private的权限，当时函数upperLefter却返回了该成员的reference并且他是public的，因此该成员函数的权限是public
的。第二，如果const成员函数传出一个reference，后者所指向的数据与该对象相关联，而且该reference的对象是在对象之外，那么
这个函数的调用者就可以修改那笔数据！例子防止setX被调用的一个办法就是返回一个const reference！
> 
2.
```cpp
Point coord1(0,0);
Point coord2(1,1);
const Rectangle rect(coord1,coord2);
rect.upperLefter().setX(10);
class GUIObject{...}
const Rectangle boundingBox(const GUIObject& obj);
GUIObject* pgo;
const Point* pUpperLeft = &(boundingBox(*pgo).upperLefr());
```
在上面的例子中，pUpperLeft指向了右边表达式产生的临时对象的内部成员point，但是在该条语句执行结束后，后边的
的语句产生的临时对象会自动的销毁，此时pUpperLeft就会指向一个无意义的空间！
请记住:
- 避免返回handle指向对象的内部成员，这样不仅可以增加封装性并且可以防止出现"悬吊号码牌"的情况
