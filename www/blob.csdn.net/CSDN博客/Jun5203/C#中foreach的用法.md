# C#中foreach的用法 - Jun5203 - CSDN博客
2019年02月16日 16:17:58[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：47
个人分类：[【C#】](https://blog.csdn.net/Ellen5203/article/category/8309127)
所属专栏：[C#](https://blog.csdn.net/column/details/29824.html)
foreach循环用于列举出集合中所有的元素，使用foreach可以迭代数组或者一个集合对象。
<组成>
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216155101226.png)
<循环过程>
每一次循环时，从集合中取出一个新的元素值，放到**只读变量**中去，如果括号中的整个表达式返回值为true，foreach块中的语句就能执行。一旦集合中的元素都应经被访问到，整个表达式的值为false，控制流程就转入到foreach块后面的执行语句。
<遍历数组>
```
class Program
    {
        static void Main(string[] args)
        {
            int[] array = new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            foreach (int i in array)
            {
                Console.WriteLine(i);
            }            
        }
    }
    它实现了对数组元素的遍历，遍历之前需要指定元素的类型
```
```
//结果
1
2
3
4
5
6
7
8
9
```
<遍历字符串>
```
class Program
    {
        static void Main(string[] args)
        {
            string test = "Hello,world!";
            foreach (char i in test)
            {
                Console.WriteLine(i);
            }
        }
    }
```
```
//结果
H
e
l
l
o
,
w
o
r
l
d
!
```
将i的类型char换成int
```
//结果
72
101
108
108
111
44
119
111
114
108
100
33
输出的结果是字符所对应的ASCII码值，说明这里进行了数据类型的隐式转换
```
<注释>
- 变量名用来存放该集合中的每个元素
- ArrayList或List是一个类，它可以让foreach去遍历
<举例>
```
class Program
    {
        static void Main(string[] args)
        {
            int count;
            Console.WriteLine("输入要登记的学生数");
            count = int.Parse(Console.ReadLine());
            string[] names = new string[count];//声明一个存放姓名的字符串数组，其长度等于提供的学生人数
            for (int i = 0; i < names.Length; i++)//接受姓名
            {
                Console.WriteLine("请输入第{0}个学生的姓名", i + 1);
                names[i] = Console.ReadLine();
            }
            Console.WriteLine("已登记的学生如下");
            foreach (string name in names)//显示姓名
            {
                Console.WriteLine("{0}", name);
            }
            Console.ReadKey();
        }
    }
```
<注意>：
- foreach循环是只读的，不能遍历修改数据
- foreach循环是只进的，并且是一条一条循环的
