# 接口（Interface）的用途 - 左直拳的马桶_日用桶 - CSDN博客
2008年01月07日 11:57:00[左直拳](https://me.csdn.net/leftfist)阅读数：1490
接口（Interface）的用途
左直拳
Interface我之前很少用，不知道它有什么用。纯粹是制订一些标准，让继承的类拥有指定的功能和属性吗？
现在我发现，接口更大的好处也许是**能够将不同类的实体看作同一种对象，只要这些类都继承了同一个接口。**
比如说，
Interface ILife
{
void 吃();
void 繁殖();
}
Class Human : ILife{} 
Class Dog : ILife{}
ILife objLife;
Human human = new Human();
Dog dog = new Dog();
objLife = (上帝高兴) ? (ILife)human : (ILife)dog;
objLife.吃();
objLife.繁殖();
这样在某些时候，只由objLife对外界服务，可以屏蔽大量的细节，省去不少判断。从这个意义来说，接口可以实现类级的统一调用，而委托则是实现函数级的统一调用。
