# 对象复制语意（C++） - youbingchen的博客 - CSDN博客





2016年05月24日 22:16:14[youbingchen](https://me.csdn.net/youbingchen)阅读数：306
个人分类：[C++ 模型对象](https://blog.csdn.net/youbingchen/article/category/6243383)









设计一个class，并以一个类对象指定给另一个类对象时，有三种选择 

+ 1.什么都不做，实施默认行为 

+ 2.提供一个explicit copy assignment operator 

+ 3.显示拒绝把一个类对象指定给另一个
如果要实现第三点，不准将一个class object指定给另外一个类对象，那么只要将copy assignment operator声明为private，并且不提供定义即可。

```cpp
class Pointy
{
  public :
   Point(float x = 0.0,float y = 0.0);
   //... 没有virtual 函数
   protected：
   float _x, _y;
}
```

*如果默认的memberwise copy行为已经满足我们的需求，我们就不必去重写拷贝赋值操作符，反而这样，程序反倒执行得更慢*

一个类对于默认的拷贝赋值符，下面这些情况是不会表现出bitwise copy语意 

+ 1.当class 内含一个成员对象，而其类有一个拷贝赋值符 

+ 2.一个类的基类有一个拷贝赋值操作符 

+ 3.一个类声明任何的虚函数 

+ 4.类继承自一个虚基类（不论这个基类有没有拷贝操作符）
```cpp
Point a,b;
...
a = b;
```

说明：由于bitwise copy完成，把Point b拷贝到Point a，其间并没有拷贝赋值操作符被调用，从语意上或效率上考虑，这些都是我们所需要的。

假如现在要导入一个拷贝赋值操作符，说明该操作符在继承下的行为：

```cpp
inline Point & Point::operator=( const Point &p)
{
  _x = p._x;
  _y = p._y;

  return *this;
}
```

现在派生一个Point3d 类

```cpp
class Point3d ： vitual public point 
{
   public :
    Point3d(float x = 0.0,float y = 0.0,float z = 0.0);
    // ...
    protected:
    float _z;
}
```

如果没有Point3d定义一个拷贝赋值操作符，编译器就必须合成一个，合成得到的东西可能看起来是这样的：

```cpp
// c++ 伪代码
inline Point3d & Point3d::operator = (Point3d * const this, const Point3d &p)
{
  // 调用base class的函数实例
  this->Point::operator=(p);
  //memberwise copy the derived class members
  _z = p.z;
  return *this;
}
```

拷贝赋值操作符有一个非正交性情况，它缺乏一个member assignment list(和成员初始化列表的东西）,因此下列写法是错误的：

```cpp
//C++ 伪代码
inline Point3d & Point3d:: operator = (const  Point3d &p3d):Point( p3d ),z(p3d._z)
{}
```

# 析构语意学

如果类没有定义析构函数，那么只有在类内含的成员对象（或是类的基类）拥有析构函数的情况下，编译器才会自动合成一个，否则，析构被视为不需要，也不需要合成，例如下面的Point，默认情况下并没有被编译器合成出一个析构函数-甚至虽然它拥有一个虚函数

```cpp
class Point
{
  public:
   Point(float x = 0.0,float y = 0.0);
   Point(const Point &);

   virtual float z();
   private；
   float _x,_y;
};
// 类似的道理如果把两个Point对象合成一个Line 类：
class Line{
public:
Line( const Point &, const Point &);
// ...
virtual draw();
//...
protected:
Point _begin,_end;
};
```

Line也不会拥有一个被合成出来的析构函数，因为Point并没有析构函数

#### 析构函数调用顺序
- 1.析构的函数本体首先被执行
- 2.如果class拥有成员类对象，而后者拥有析构函数，那么他们将会以声明顺序的相反顺序被调用
- 3.如果对象内含一个vptr，现在被重新设定，指向适当只base class的virtual table
- 4.如有任何直接的（上一层）nonvirtual 基类拥有析构函数，它会议其声明顺序的相反顺序被调用
- 5.如果有任何虚基类拥有的析构，那么他们会其原来的构造顺序的相反顺序被调用。





