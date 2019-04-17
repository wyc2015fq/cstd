# C++ 类成员的构造和析构顺序 - DoubleLi - 博客园






我想对面向对象有了解的童鞋应该不会对类和对象感到陌生吧 ！

对象并不是突然建立起来的，创建对象必须时必须同时创建父类以及包含于其中的对象。C++遵循如下的创建顺序：

(1)如果某个类具体基类，执行基类的默认构造函数。

(2)类的非静态数据成员，按照声明的顺序创建。

(3)执行该类的构造函数。

即构造类时，会先构造其父类，然后创建类成员，最后调用本身的构造函数。

下面看一个例子吧

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

class c
{
public:
    c(){ printf("c\n"); }
protected:
private:
};

class b 
{
public:
    b(){ printf("b\n");}
protected:
    c C;
private:
};

class a : public b
{
public:
    a(){ printf("a\n"); }
protected:
private:
};

int main()
{
    a A;
    getchar();
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

这个例子的结果是：![QQ截图20131118143652](https://images0.cnblogs.com/blog/557692/201311/18150412-c1f0bcbfaaa045569e43919c99e292d5.png)

分析一下吧  首先定义了3个类 a b c，其中a继承自b ，在main函数里面构造a ，因为a是继承b ，所以会先构造b，然后b类又有一个成员c类，所以c类是最先构造的，然后是b ，最后才是a。

在看一个例子，就是上面的该了一下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

class c
{
public:
    c(){ printf("c\n"); }
protected:
private:
};

class b 
{
public:
    b(){ printf("b\n");}
protected:
private:
};

class a : public b
{
public:
    a(){ printf("a\n"); }
protected:
    c C;
private:
};

int main()
{
    a A;
    getchar();
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

没有改变多少，只是在a添加了c成员，而b去掉了。

其结果是：![123](https://images0.cnblogs.com/blog/557692/201311/18150414-13dff2e886544563bc81d79962e8191d.png)

同样是在main里面构造a ，a继承自b，所以先构造b，然后构造a本身的数据成员c，最后才调用的a本身的构造函数。。

这里大家应该明白构造的细节了吧 。。

接下来看析构的顺序：

(1)调用类的析构函数。

(2)销毁数据成员，与创建的顺序相反。

(3)如果有父类，调用父类的析构函数。

也看一个例子吧：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

class c
{
public:
    c(){}
    ~c(){ printf("c\n"); }
protected:
private:
};

class b 
{
public:
    b(){}
    ~b(){ printf("b\n");}
protected:
private:
};

class a : public b
{
public:
    a(){}
    ~a(){ printf("a\n"); }
protected:
    c C;
private:
};

int main()
{
    a A;
    return 0;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

结果是：![image](https://images0.cnblogs.com/blog/557692/201311/18150415-7c2eb6d74605468e99089af350721353.png)

其过程是，在main函数结束时，会销毁a，就会先调用a的析构函数，先后销毁a的数据成员c，最后销毁a的父类b。其实就是跟创建时的顺序反了过来。









