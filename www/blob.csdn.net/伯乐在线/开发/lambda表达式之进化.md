# lambda表达式之进化 - 文章 - 伯乐在线
原文出处： [xpy0928](http://www.cnblogs.com/CreateMyself/p/4727220.html)
# 前言
在C#我们可以自定义委托，但是C#为什么还要内置泛型委托呢？因为我们常常要使用委托，如果系统内置了一些你可能会用到的委托，那么就省去了定义委托，然后实例化委托的步骤，这样一来既使代码看起来简洁而干净又能提高程序员的开发速度，何乐不为呢！通过本文可以让你复习扩展方法，同时可以循序渐进的了解系统内置泛型委托的实现以及委托逐步的演化过程。
#  Action
概念：封装一个方法，该方法具有五个参数并且不返回值。并且类型参数为逆变
下面我就自定义实现一个Action无返回值的委托。我们同样定义一个Person类，似乎我随笔中永远都离不开Person的话题，哈哈！请看如下代码


C#
```
public class Person
    {
        public string Name { get; set; }
        public int Age { get; set; }
        public bool Gender { get; set; }
    }
```
然后在控制台中通过 ForEach 方法模拟Action委托，先定义一个获得Person的列表GetList()


C#
```
static List GetList()
        {
            List list = new List() { 
            new Person(){ Name = "花千骨     （女娲后人及妖神）",   Age = 12, Gender = false},
            new Person(){ Name = "白子画     （长留尊上）",   Age = 13, Gender = true},
            new Person(){ Name = "东方彧卿   （异朽阁主及蜀国大学士）", Age = 14, Gender = true},
            new Person(){ Name = "轻水       （长留弟子）",     Age = 15, Gender = false},
            new Person(){ Name = "孟玄朗     （蜀国皇帝及长留弟子）",   Age = 16, Gender = true}
            };
            return list;
        }
```
因为我们知道在用委托时，有这样几个步骤：
（1）定义委托
（2）实例化委托
（3）将方法指针添加到实例化委托对象中
但是现在我们无需定义委托，已经有了内置委托，只需将其实例化即可，同时添加方法的指针一般是有明确的方法，如果我们只是临时的用方法，这时就可以派匿名方法上场了，所以上面三步就可以简化成两步。代码如下：


C#
```
var list = GetList();
            list.ForEach(new Action
                (
                  delegate(Person p)
                  {
                      Console.WriteLine(p.Name);
                  }
                ));
```
上述代码颇有点JQuery中Each的遍历方法的意味。结果打印出：
![](http://jbcdn2.b0.upaiyun.com/2016/06/feeae1f27ee18d04f01938abc4b963e4.png)
我们知道ForEach这个方法里面的的参数就是 Action action ，所以我们可以直接进行如下简写


C#
```
list.ForEach(delegate(Person p) { Console.WriteLine(p.Name); });
```
其打印结果和上面是一样的。其代码可以继续进行更加的精简，不着急，我们循序渐进的来。
# Predicate
概念：定义一组条件并确定指定对象是否符合这些条件的方法。返回值为bool类型，并且类型参数为逆变。
用到此泛型委托莫过于List中的 FindAll() 方法了，它就是从一个集合中根据条件筛选出一个新的集合出来。上节刚好学过扩展方法，我们可以自定义实现这个方法用扩展方法加在泛型集合List上根据  Predicate 委托的参数条件进行筛选。


C#
```
static List SelfDefineFindAll (this List list, Predicate pre)   /*注意：既然是添加的扩展方法，在此例中控制台的Program也要声明为静态类*/
{
      List preList = new List; /*根据条件筛选出的数据添加到该集合中*/
    
      foreach (T t in list)
     {
            if (pre(t)) /*根据条件进行筛选*/
            {
                preList.Add(t);
            }
     }
      return preList;
}
```
我们查询出年纪大于13岁的并且根据ForEach来遍历筛选出来的数据，代码如下：


C#
```
var list = GetList();
 var preList = list.SelfDefineFindAll(delegate(Person p) { return p.Age > 13; });
 preList.ForEach(delegate(Person p) { Console.WriteLine(p.Name); });
```
结果打印出正确的结果，如下：
![](http://jbcdn2.b0.upaiyun.com/2016/06/4929593e932ee817fb9acb4d58256c3e.png)
而通过C#中的FindAll，则只需如下做即可同样达到上述的效果，只是不是扩展方法罢了：


C#
```
list.FindAll(delegate(Person p) { return p.Name});
```
上述代码其实可以更加精简，不着急，我们一步一步循序渐进的来。
# Comparison
概念：表示比较同一类型的两个对象的方法。参数类型为逆变，返回值为int。


C#
```
list.Sort(new Comparison(delegate(Person p, Person p1) { return p.Age - p1.Age; }));  /*年龄按照从小到大顺序排列*/
```
同样可以简写为：


C#
```
list.Sort((delegate(Person p, Person p1) { return p.Age - p1.Age; }));
```
# Func
貌似在系统内置泛型委托中Func在实际项目开发中是使用的最多的。
概念：封装一个具有一个参数并返回 TResult 参数指定的类型值的方法。参数类型为逆变，返回值参数类型为协变。
用到此委托的最多的就是List泛型集合中的 Select 方法了，看看内置的Select方法是如何利用Func来实现返回一个新的集合的。
## （1）结合匿名方法实现
我们接下来在上面基础上再定义一个Animal类。属性和Person类一样，代码如下：


C#
```
public class Animal
    {
        public string Name { get; set; }
        public int Age { get; set; }
        public bool Gender { get; set; }
    }
```
接下来我们就通过Select方法通过Person的集合来返回一个新的集合即Animal集合。代码如下:


C#
```
var list = GetList();
            List animalList = list.Select(new Func(delegate(Person p)
            {
                return new Animal() { Name = p.Name, Age = p.Age, Gender = p.Gender };
            })).ToList();
            animalList.ForEach(delegate(Animal animal) { Console.WriteLine(animal.Name); });
```
同样打印出当遍历Person集合时的结果一样，看起来似乎很繁琐，我们将代码通过进行精简如下：


C#
```
var list = GetList();
            List animalList = list.Select((delegate(Person p)
            {
                return new Animal() { Name = p.Name, Age = p.Age, Gender = p.Gender };
            })).ToList();
            animalList.ForEach(delegate(Animal animal) { Console.WriteLine(animal.Name); });
```
下面我们通过扩展方法来自定义实现Select()方法，代码如下：


C#
```
publicstatic ListSelfDefineSelect(this List list, Func fun)/*T为传进来的泛型集合类型，TR为返回的泛型集合类型*/
        {
            List selList = new List();  /*实例化返回的泛型集合*/
            foreach (T t in list)
            {
                TR tr = fun(t);  /*获取传进来的集合数据*/
                selList.Add(tr);
            }
            return selList;  /*返回新的泛型集合*/
        }
```
依然是进行此调用，打印结果正确：

C#
```
List animalList = list.SelfDefineSelect((delegate(Person p)
            {
                return new Animal() { Name = p.Name, Age = p.Age, Gender = p.Gender };
            })).ToList();
```
##  （2）结合匿名类来实现
当我们使用Func根据条件转换成新的集合时可能只需要几个实例成员，这个时候如果还重新建立一个类来进行获取就显得有点小题大做了，这个时候只需将其需要的或的成员传给一个匿名类即可，这个就是Func需要用到匿名类的场景。所以鉴于此，我们将返回的新的集合为匿名集合而非Animal集合，代码改造如下：


C#
```
var anyousList = list.Select((delegate(Person p)
            {
                return new { Name = p.Name}; /*结合匿名类使用*/
            }));
```
通过上述所讲系统内置泛型委托的实现，似乎有点不太令人满意，关于委托的代码太过繁琐，是的微软大大也明确知道了这点，于是乎，一步一步走向了高级，那就下文的lambda表达式，这结构的简单简直了。。。让你爽到暴。
# lambda表达式
上述代码说过可以精简，如何精简呢？那就是lambda表达式，匿名方法已经够简洁的了，但是lambda表达式是比匿名方法更加简洁的一种语法！我们用lambda表达式来分别实现上述中的Action、Predicate以及Func委托。
## Action


C#
```
var list = GetList();
            list.ForEach(d => Console.WriteLine(d.Name);)
```
## Predicate


C#
```
var list = GetList();
            list.FindAll(d => d.Age > 13);
```
## Func


C#
```
list.Select(d => new Person() { Name = d.Name, Age = d.Age, Gender = d.Gender });
 
            list.Select(d => new { Name = d.Name});  /*lambda表达式中的匿名类*/
```
好了，一切都变得如此明朗。自从有了lambda表达式，敲代码的速度加快了，妈妈再也不用担心我熬夜到很晚了。
好了，问题来了，我们知道lambda表达式分为 语句lambda和表达式lambda  ，那么二者有何区别呢？从字面上理解语句lambda是不是就是用大括号括起来的呢？ok，给出代码来理解吧。


C#
```
(string str) => { return str.length; }  /*语句lambda（有大括号和return）*/
(string str) => str.length  /*表达式lambda（没有大括号和return，只有一个式子）*/
```
那问题又来了，lambda表达式到底是什么呢？我们依然用反编译来查看  list.ForEach(d => Console.WriteLine(d.Age)); 对应的C#代码如下：
![](http://jbcdn2.b0.upaiyun.com/2016/06/d7f5ab6ef2575e9fcd54726bb79d007f.png)
看ForEach()方法里面的参数意思大概是匿名方法委托，接着我们点击进去看看，代码如下：
![](http://jbcdn2.b0.upaiyun.com/2016/06/35a5bd1db0df8c070c381d2095f08e78.png)
我们接着点击Action看看，如下：
![](http://jbcdn2.b0.upaiyun.com/2016/06/dc1bbf0675ee91700d234bd1ff2cd795.png)
一下就豁然开朗了，这不正说明 lambda表达式的实质就是匿名方法 吗！所以现在想想，lambda表达式的本质是匿名方法，匿名方法的本质是通过委托实现的。应该就是这样了。
# lambda表达式进化过程
我们就一个扩展方法的实例来演示lambda表达式演变的过程是多么的惟妙惟肖。
假设如下场景：在花千骨电视中找出白子画出来，找对了你就赢了！我们获得给出一个花千骨众角色列表，再选出白子画即可。


C#
```
/*根据条件找出所需，返回true你就赢了，反之则输*/
             static bool SelDefine_Extension_IEnumerable(this IEnumerable source, Funcbool> func)
             {
                 foreach (var item in source)
                 {
                    if (func(item))
                    {
                        return true;
                    }
                 }
                 return false;
             }
```
下面给出集合列表：


C#
```
var list = new Liststring>() { "花千骨", "白子画", "东方彧卿", "霓漫天", "糖宝", "落十一", "轻水", "孟玄朗" };
```
然后在控制台执行扩展方法进行查询，在此列出 lambda表达式6部曲 ：


C#
```
list.SelDefine_Extension_IEnumerable(new Func(delegate(string item) { return item.Equals("白子画"); }));
            
            list.SelDefine_Extension_IEnumerable(delegate(string item) { return item.Equals("白子画"); });
            list.SelDefine_Extension_IEnumerable((string item) => { return item.Equals("白子画"); });
            list.SelDefine_Extension_IEnumerable((string item) => item.Equals("白子画"));
            list.SelDefine_Extension_IEnumerable((item) => item.Equals("白子画"));
            list.SelDefine_Extension_IEnumerable(item => item.Equals("白子画"));
```
从开始的繁琐，复杂到最终的简洁，每一个过程微软大大也是作出一定的努力，先点给赞先！就上述用一副图来看，估计会更加清晰明了吧
## lambda表达式演变六部曲
![](http://jbcdn2.b0.upaiyun.com/2016/06/0118324ce87278ded57231c3e5863e92.png)
