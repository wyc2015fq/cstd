# C#学习笔记6——C#中枚举、结构、数组 - 深之JohnChen的专栏 - CSDN博客

2011年08月31日 00:42:02[byxdaz](https://me.csdn.net/byxdaz)阅读数：1639标签：[c#																[java																[struct																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)
个人分类：[.Net & C#](https://blog.csdn.net/byxdaz/article/category/628880)


一、数组。

数组：是具有相同数据类型的一块连续的内存空间。（一组相同数据类型的值的集合。）C#中数组属于引用类型。它的分类有：一维数组和二维数组。

1) 一维数组。

定义：

格式： dataType[] 数组名；

JAVA中我们定义数组时这个[ ]符号，放在数组名之前和之后都是可以的，而在C#中这个符号只能放于数组名之前。

数组定义之后并不可能直接使用，我们必须为它在内存中开辟空间，即实例化。如下：

int[ ] array;

array=new int[10];

以上两句也可以写成：int[] array=new int[10]; 这和JAVA中都是一样的。

2) 二维数组。

二维数组又可分为：矩阵型和钜齿型。

C#中矩阵型数组的定义：

dataType [ , ] 数组名；

矩阵型数组的实例化为：数组名=newdataType[2,3];

同样可写成:dataType[,] 数组名=newdataType[2,3]；

在JAVA中矩阵型数组的定义完全和C#中不同，这里我们必须特别注意。

C#中钜齿型数组的定义：

dataType [ ][ ] 数组名;

实例化为: 数组名=new dataType[2][ ];

这个同样可以写成dataTye[][ ] 数组名=newdataType[3][ ];

这个的定义和我们JAVA中是一样的。

还有就是我们在定义引用类型数组时，它的每一项元素都要进行实例化。如下：

假如有一个Student类。

Student [ ] sarray=new Student[2];

sarray[0]=new Student();

sarray[1]=new Student();

在C#中对数组为我们提供了Length属性，它可以得到一个数组的元数总个数。还有一个GetLength(inttemp)方法，它是得到一个数组中某一维元素的个数。（也可以说是有几行，或者几列）。如下：

1: int[,] a=new int[2,3]; 

2: Console.WriteLine(a.Length); 

3: Console.WriteLine(a.GetLength(0)); 

4: Console.WriteLine(a.GetLength(1)); 

以上代码第二行将输出6（即6个元素），第三行将输出2（即2行），第四行将输出3（即3列）。

二、枚举

枚举：是一组已命名的数值常量。

枚举类型：就是用户自定义的一组数据类型。它出现的目的就是为了提高我们程序的可读性。

枚举类型 的定义：

在C#中我们用关键字：enum

如下：

enum Week{

Monday,

Tuesday,

Wednesday,

Thursday,

Friday,

Saturday,

Sunday

}

以上代码就是定义了一个枚举类型，应注意它元素后面的逗号，最后一个元素不用逗号。枚举类型是属于值类型。

三、结构

结构：是用户自定义的一种新的数据类型。

C#中结构体和类是非常的相似。都具有方法和数据。它们的区别在于：

1）结构体用struct 关键字定义，而类用class定义。

2）结构体它属于值类型，而类是属于引用类型。

3）结构体如果有自定义构造函数，此构造函数必须要有参数。

4）结构体不可以被继承，而类是可以被继承的。结构总是派生于System.ValueType，它们还可以派生于任意多个接口。（结构不支持实现继承）；类总是派生于用户选择的另一个类，它们还可以派生于任意多个接口。


