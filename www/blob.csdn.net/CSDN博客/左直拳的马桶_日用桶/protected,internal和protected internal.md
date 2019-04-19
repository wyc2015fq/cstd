# protected,internal和protected internal - 左直拳的马桶_日用桶 - CSDN博客
2010年04月08日 12:33:00[左直拳](https://me.csdn.net/leftfist)阅读数：5241
我03年开始搞C#，搞了好多年了，楞是没搞出什么名堂。其中是有原因的，水平太烂。
好比面向对象里非常基本的protected可访问级别，一直都没搞清楚。
我记忆中，好像在哪读过，基类中的protected，在子类中会变成private，所以只能被儿子访问，孙子就不行了。但事实上，protected不管在基类还是子类，永远都是protected。
另外，internal这个访问级别原本不知道有什么用，现在有点明白了。它的作用是只能在本程序集里才是可访问的。比如，你写了个公用类库，编译成一个独立的DLL被其他程序调用，这时候，internal可以保证有些东西只能在这个公用类库才能访问，那些调用程序即使继承了里面的类，也无法访问这些东西。
C#里protected还可以与internal结合:protected internal，只能在本程序集中，由本类或继承类访问。
==================================
我又发现一种现象
属性里边，访问器get和set如果成对出现，则它们其中一个（仅一个）可以设置访问修饰符，并且这个访问限制应该比属性的更为严格。例如
publicstring
 Name
{
get
    {
return
 name;
    }
protectedset
    {
        name = value;
    }
}
这个当然是有用的，通常我们都希望SET有更严格的限制。
对于 protected 这个东东，属性和方法的访问级别似乎也有区别。比如，基类中有：
class Base
{
protected int Property{get;}
protected int Method();
}
它们都是protected，子类中访问当然没有问题
class Son : Base
{
 void Test()
{
int i = Property;
Method();
}
}
但如果象这样
