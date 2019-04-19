# 误导人的接口（interface） - 左直拳的马桶_日用桶 - CSDN博客
2013年08月20日 16:28:09[左直拳](https://me.csdn.net/leftfist)阅读数：1760
接口，interface，这个词语有误导之嫌。窃以为，这也是其名称与实际开发不符，造成难于直观理解和使用过程中产生困惑的根源。所谓名不正则言不顺；不怕生错命，最怕改坏名。
在现实生活中，接口通常是指将两样东西接合在一起的部位，比如水龙头与水管的接口，插头与插座的接口。但在编码里面，我还没见过接口是在起这种作用：定义了一个接口，然后某个类实现了这个接口，竟然是为了便于与其他的什么类接合在一起的？好像没有吧。
当然你可以讲，不管是什么水龙头，你只要是这种接口，你都可以接到这个水管里来；不管是什么牌子的插头，只要是这种双脚，也都可以插到这个插座里来，这就是接口的好处……，一大堆。问题是，很明显，现实生活中的接口，就是为了将不同的东西接合在一起，接口一词很准确，也很容易理解，但在我们编码里面，接口是为了接合的吗？
所以开发语言里的interface，直接套用“接口”这个名称，真的是很让人困惑。
我觉得**将接口理解成身份**更加符合实际。
不是吗？一个类可以实现多个接口，正如一个人可以拥有多个身份一样。
好比说，张三在家里是父亲，丈夫，在公司里是开发主管，那么他就拥有了3个身份，也就是实现了3个接口：父亲、丈夫、开发主管。
于是，同是张三这个人，外界不同的人就可以将他分别对待，分别打交道（调用）了：
对孩子来说，张三是父亲，他会 教育()、呵护()、讲故事()；
对妻子来说，张三是丈夫，他拥有 养家()、敦伦() 等功能；
对公司来说，张三是开发主管，可以 制订工作计划()、开发()、指导下属() 等等。
如果用代码来表示，是这样： 
```
class WhatIsInterface
{
    abstract class Person
    {//人类
        string _name;
        public Person(string name)
        {
            this._name = name;
        }
        public string Name
        {
            get
            {
                return _name;
            }
        }
    }
    interface IHusband
    {//丈夫接口
        void GetMoney();
        void MakeLove();
    }
    interface IFather
    {//父亲接口
        void Teach();
        void Care();
        void SayStory();
    }
    interface IManager
    {//开发主管接口
        void Planning();
        void Coding();
        void Guide();
    }
    class ItOldMan : Person,IFather,IHusband,IManager
    {//IT老男人类
        public ItOldMan(string name) : base(name) { }
        //实现父亲接口
        public void Teach()
        {
            Console.WriteLine(String.Format("{0}细心教育孩子",Name));
        }
        public void Care()
        {
            Console.WriteLine(String.Format("{0}很疼爱自己的孩子",Name));
        }
        public void SayStory()
        {
            Console.WriteLine(String.Format("{0}睡觉前给孩子讲故事",Name));
        }
        //实现丈夫接口
        public void GetMoney()
        {
            Console.WriteLine(String.Format("{0}要努力赚钱养家",Name));
        }
        public void MakeLove()
        {
            Console.WriteLine(String.Format("{0}：雅蠛蝶。。。",Name));
        }
        //实现开发主管接口
        public void Planning()
        {
            Console.WriteLine(String.Format("{0}需要制订工作计划",Name));
        }
        public void Coding()
        {
            Console.WriteLine(String.Format("{0}是开发主力",Name));
        }
        public void Guide()
        {
            Console.WriteLine(String.Format("{0}常常指导下属各种问题",Name));
        }
    }
    //应用部分
    class Wife
    {//妻子类
        private IHusband _husband;
        public Wife(IHusband husband)
        {
            this._husband = husband;
        }
        public void MyHusband()
        {
            split("丈夫");
            _husband.GetMoney();
            _husband.MakeLove();
        }
    }
    class Daughter
    {//女儿类
        private IFather _father;
        public Daughter(IFather father)
        {
            this._father = father;
        }
        public void MyFather()
        {
            split("父亲");
            _father.Care();
            _father.Teach();
            _father.SayStory();
        }
    }
    class Department
    {//部门类
        private IManager _manager;
        public Department(IManager manager)
        {
            this._manager = manager;
        }
        public void MyManager()
        {
            split("开发主管");
            _manager.Planning();
            _manager.Coding();
            _manager.Guide();
        }
    }
    static void split(string subject)
    {
        Console.WriteLine();
        Console.WriteLine(String.Format("作为 {0} 的角色",subject));
        Console.WriteLine("---------------------------");
    }
    static void Main(string[] args)
    {
        //张三
        ItOldMan zhang3 = new ItOldMan("张三");
        //张三的爱人
        Wife wife = new Wife(zhang3);
        wife.MyHusband();
        //张三的小孩
        Daughter daughter = new Daughter(zhang3);
        daughter.MyFather();
        //张三的部门
        Department department = new Department(zhang3);
        department.MyManager();
        Console.ReadLine();
    }
}
```
运行结果
![](https://img-blog.csdn.net/20130820161948281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
同一个ItOldMan对象 zhang3，传给不同的类，这些类分别应用了它不同的身份。
其中，对妻子类而言，一个 ItOldMan 对象传给她，赋给了她的 IHusband 对象，那么在这里，这个 ItOldMan 就只是一个丈夫对象，她访问的也只是ItOldMan里面有关丈夫的方法。其他也类似。也就是说，ItOldMan实现了许多接口，拥有多种身份，用哪种身份（接口）来访问它，它就表现出什么身份。接口在这里的作用，有点类似语文中，以部分代替整体的“借代”修辞手法。
```
//张三的爱人
        Wife wife = new Wife(zhang3);
        wife.MyHusband();
```
```
class Wife
    {//妻子类
        private IHusband _husband;
        public Wife(IHusband husband)
        {
            this._husband = husband;
        }
        public void MyHusband()
        {
            split("丈夫");
            _husband.GetMoney();
            _husband.MakeLove();
        }
    }
```
看到没？zhang3本身是一个完整的ItOldMan对象，继承了Person的基因，又实现了IHusband、IFather、IManager等多种接口，当Wife用IHusband调用它以后，Wife只会使用它IHusband的特性。
这让我想起以前学过的课文，说同是一棵树，在植物学家眼里，它是一种什么科什么属的研究对象；在木匠眼里，它可以做成漂亮的家具；而在樵夫眼里，它只是一堆柴。就看你用什么眼光，站在什么角度去看它，又或者说，你用什么接口去调用它。
接口有强制性，即里面的方法继承类需要全部实现，因此说它是一种强制性契约，一种规范，从这个角度看，接口跟现实生活中所说的“接口”是相等的，接口不对，就接不进去，就套不进去。但是，接口的这种强制性，本意不在于规范编码，而是为了外界用它来调用，正如妻子类用IHusband、女儿类用IFather、部门类用IManager来调用zhang3一样。不然你无法解释为何接口里的方法铁定是public。如果只为了强制性，实现就是了，管得了人家是公有私有呢，全部公有，不就是为了让外界调用嘛。
