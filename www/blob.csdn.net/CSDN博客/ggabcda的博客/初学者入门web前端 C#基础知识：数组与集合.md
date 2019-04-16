# 初学者入门web前端 C#基础知识：数组与集合 - ggabcda的博客 - CSDN博客





2017年08月21日 20:17:12[ggabcda](https://me.csdn.net/ggabcda)阅读数：221








**对于初学者，想要入门web前端，要有足够的信念和坚持，不然只会越走越远，我现在就深深的体会到。**

**我本是一个很拒绝代码的人，以前想过UI设计，但是在这段学习时间里，发现其实只要认真，代码并不是很难**

**所以我整理了一套前期学C#的知识点，对于后期学习JavaScript有很大的帮助。**

**一、数组与集合**

数组：能存放任意多个同类型的数据

数据项：类型相同

①每一个数据型都有一个编号（索引或下标）

②数据的索引（下标）是一个int类型的数字

③从0开始，依次为数据中每一个数组项编号

数组的声明与赋值

声明：数据类型[ ] 变量名;

赋值：变量名 = new 数据类型[长度]（长度为int类型）

数据型的读取与修改：读取：变量名[索引] 
                                  修改 ：变量名[索引] = 值;

读取数组的长度 变量名.Length 返回类型为int

合并书写:数据类型[ ] 变量名 = new 数据类型[长度];

(数组的声明与赋值, 数据型的读取与修改, 读取数组的长度)

数组项的默认值：一个数组创建后，它每一个数组项的值为数组项类型的默认值。

常见默认值：数字类型 默认值为0
  bool类型 默认值为false
  char类型 默认值为\0
string类型 默认值为mull（表示没有任何数据）

示例

创建一个长度为3的字符串数组，然后依次为它的每一项赋值为a、b、c、，最后，输出该数组的长度

合并书写

string[ ] strs = new string[3];
strs[0] = "a";
strs[1] = "b";
strs[2] = "c"; sring[ ] strs = {"a","b","c"};
Console.Write(strs.Length); Console.Write(strs.Length);


int[ ] numbers = new int[5];
numbers[0] = 3;
numbers[1] = -2;
numbers[2] = numbers[0] * 2 + numbers[1];
Console.WriteLine(numbers[3]);
Console.WriteLine(numbers.Length); 控制台输出4,5

**二、数组的定长性**

一个数组创建后，它的长度固定不变

int[ ]nums = new int[5];
nums = new int [6];          不再使用上一个数组，重新建立一个长度为5的数组

数组适用于数据数量固定的场景

适合使用数组的场景：保存100以内的所有质数

保存太阳系中所有已知行星的质量

保存标准扑克中的所有牌面数据

保存一个星期的所有日期

其他定长的数据场景

不适合使用数组的场景：保存一个班学生信息

保存一年中的所有日期

保存斗地主游戏中某玩家的手牌数据

保存游戏某玩家的装备信息

其他不定长的数据场景

**三、数组的遍历 **

重复行为：输出一个数组项

代码书写格式：Console.WriteLine(arrays [?]);其中？为索引

int i = ？；
变量i的变化范围：0~arrays，Length-1 也可以写成：i < arrays.Length

遍历

定义：是指从数组的第一项开始，依次取完整数组所有项

①实现数组的遍历，可以使用循环

②循环变量从0开始，依次取到数组的最大下标（数组的长度-1）

③在循环体中，使用循环体变量作为下标，即可取出数组每一项的值

for (int i = 0; i < arrays.Length; i++)
{
Console.WriteLine(arrays[i]);
}

完整实现代码

```
Console.Write("亲输入数组的长度："）；
int len = int,Parse(Conesole,ReadLine());
//根据用户的输入创建数组，并遍历数组为其每一项赋值
int[ ] nums = 0; i < nums.Length; i++
{
Console,Write("请输入数组第"+ （i + 1） + "项的值：");
nums[i] = int.Parse(Console.ReadLine());
}
//遍历数组nums，依次输出数组中的值
Console.Write("数组的所有项是：");
for (int i = 0; i < nums.Length; i++)
{
Console.Write(nums[i]);
if (i < nums.Length - 1)//不是最后一项
Console.Write(",");//输出一个逗号分隔
```



**四、交换排序**

根据输入的数进行顺序排序：简化排序：如何把最小的数字，放置到数组索引未0的位置

实现方法：把第一个位置的数字拿出来依次和后面位置的数字进行比较，如果比后面位置数字大，则交换位置

全部代码实现

```
for (int i = 0; i < numbers.Count - 1; i++)
{
for (int j = i + 1; j < numbers.Count; j++)
{ 
if(numbers [i]>numbers [j])
{
int temp = numbers[i];
numbers[i] = numbers [j];
numbers[j] = temp;
}
}
}
```





**五、数组分析器**

代码示例：

```
Console.Write("请输入数组的长度：");
int n = int.Parse(Console.ReadLine());
int[] nums = new int[n];
{

for (int i = 0; i < nums.Length; i++)
{
Console.Write("请输入数组的第" + (i + 1) + "项:");
nums[i] = int.Parse(Console.ReadLine());
}
Console.Clear();
}
for (int i = 0; i < nums.Length - 1; i++)
{
for (int j = i + 1; j < nums.Length; j++)
if (nums[i] > nums[j])
{
int temp = nums[i];
nums[i] = nums[j];
nums[j] = temp;
}
}
Console.Write("你输入的数字从小到大的排序是：");
for (int i = 0; i < nums.Length; i++)
{
Console.Write(nums[i] + " ");
}
Console.ReadLine();
{
Console.Write("你输入的数字其中是奇数的是：");
for (int i = 0; i < nums.Length; i++)
if (nums[i] % 2 != 0)
{

{
Console.Write(nums[i] + " ");
}
}
Console.ReadLine();
}
Console.Write("你输入的数其中是质数的是：");
for (int i = 0; i < nums.Length; i++)
{

bool isFind = false;

 for (int j = 2; j < nums[i]; j++)

{
if (nums[i] % j == 0)
{
isFind = true;
break;
}

}

if (isFind)
{

}
else
{
Console.Write(nums[i] + " ");
} 
}
Console.ReadLine();
```



**六、集合**

数据：定长（用于保存固定数量的数据）占内存少 遍历速度快

集合：不定长（保存的数据数量，可以在程序的执行过程中，不断的发生变化） 占内存多 遍历速度慢

List集合

创建：定义：List<数据类型> 变量名;

           赋值：变量名 = new List<数据类型>(); //集合是不定长的所以赋值是无需指定长度，赋值后长度可以变化

           初始化器：变量名 = new List<数据类型>{元素1，元素2，···，元素n，}；



操作

1.添加元素：变量名.Add(要添加的数据）;

List<int> nums = new List<int> {3,5,7};
nums.Add(1);
nums.Add(3);

2.插入元素：向集合的指定位置插入一个新的元素

变量名.Insert(索引，要插入的数据）；

List<int> nums = neew List<int> {3,5.7};
nums.Insert(1,10);

3.删除元素：变量名.RemoveAT(索引）；删除指定索引位置的元素

变量名.Remove(数据);

删除集合中与填写的数据相同的第一个匹配项

List<int> nums = new List<int> {1,1,2,3,5};
nums.RemoveAt(2);
nums.Remove(1);

4.修改元素：修改元素中某一个值

变量名[索引] = 值；

代码实现

```
int maxIndex = numbers.Count - 1;
Console.Write("请您输入一个要删除的下标（0-"+maxIndex +")");
int index = int.Parse(Console.ReadLine());
if (index < 0 || index > maxIndex)
{
Console.WriteLine("呀！你输入有误，下标超出范围了，按回车键继续");
Console.ReadLine();
}
else
{
Console.WriteLine("请你重新输入一个新的数字");
int newNum = int.Parse(Console.ReadLine());
numbers[index] = newNum;
}
```






5.获取元素数量：集合的长度 变量名.Count

总结：在功能上，数组能实现的所有功能集合都能实现反之，集合能实现的某些功能，数据难以实现(数据, 集合, List集合)

c#的集合类型  

List Queue Stack LinkedList HashSet 其他

**七、foreach循环**

代码格式

foreach（数据类型） 变量 in 数组或集合）
{
循环体
}

从数组或集合中，依次取出每一项的数据没取出一项数据，酒吧数据赋值个循环变量每一次赋值后，运行一次循环体

代码示例

foreach循环现实：效率高

foreach (int item in numbers)
{
Console.WriteLine(item);
}

for循环现实：效率低

for ( int i = 1; i < numbers.Length; i++)
{
Console.WriteLine(numbers[i]);
}

foreach和for的对比

foreach：只用于遍历，不能更改循环目标，遍历速度快，执行效率高

for：可以用于任何形式的重复行为，在循环体重，可以进行任何操作，遍历速度慢，执行效率低

如需要遍历和组合，并且遍历的过程中只需要读取而不会更改，使用foreach循环最合适，反之，则根据需要，选择其他循环(foreach, for)

**八、集合管理器**

代码：

```
ite(item + "\t");
}
Console.WriteLine();

}

Console.WriteLine("*********************************************************");
Console.WriteLine("1.添加数据");
Console.WriteLine("2.删除数据");
Console.WriteLine("3.修改数据");
Console.WriteLine("4.升序排序");
Console.WriteLine("5.退出程序");
Console.WriteLine();
Console.Write("\t" + "请您老选择一个序号（1-5）：");
string input = Console.ReadLine();

if (input == "5")

{
break;
}
else if (input == "1")
{
Console.Write("请输入要添加的数字：");
int num = int.Parse(Console.ReadLine());
numbers.Add(num);
}
else if (input == "2")
{
Console.Write("请你输入要删除的数字：");
int num = int.Parse(Console.ReadLine());
numbers.Remove(num);
}
else if (input == "3")
{
if (numbers.Count == 0)
{
Console.Write("集合中没有要修改的元素哦？按回车键擦亮眼睛重新选择");
Console.ReadLine();

}
else
{
int maxIndex = numbers.Count - 1;
Console.Write("请您输入一个要删除的下标（0-"+maxIndex +")");
int index = int.Parse(Console.ReadLine());
if (index < 0 || index > maxIndex)
{
Console.WriteLine("呀！你输入有误，下标超出范围了，按回车键继续");
Console.ReadLine();
}
else
{
Console.WriteLine("请你重新输入一个新的数字");
int newNum = int.Parse(Console.ReadLine());
numbers[index] = newNum;
}
}
}
else if (input == "4")
{

for (int i = 0; i < numbers.Count - 1; i++)
{
for (int j = i + 1; j < numbers.Count; j++)
{ 
if(numbers [i]>numbers [j])
{
int temp = numbers[i];
numbers[i] = numbers [j];
numbers[j] = temp;
}
}
}
}
else
{ 
}
Console.Clear();
}
```



**时刻鼓励自己：**



**       埋头读书，抬头做人!**



**　　不登高山不知天之高也，不临深溪不知地之厚也**



**　　懦弱的人只会裹足不前，莽撞的人只能引为烧身，只有真正勇敢的人才能所向披靡**



**　　我们这个世界，从不会给一个伤心的落伍者颁发奖牌。**



**　　海浪的品格,就是无数次被礁石击碎又无数闪地扑向礁石、**



**　　鞋底磨穿了，不等于路走到了头。**



**　　松驰的琴弦，永远奏不出时代的强音。**



**　　躺在被窝里的人，并不感到太阳的温暖。**



**　　不安于现状，不甘于平庸，就可能在勇于进取的奋斗中奏响人生壮美的乐间。**



**　　不去耕耘，不去播种，再肥的沃土也长不出庄稼，不去奋斗，不去创造，再美的青春也结不出硕果。**





