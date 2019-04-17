# C#系列 --4  array中的Rectangular arrays 和Jagged arrays - 墨流觞的博客 - CSDN博客





2018年10月31日 22:26:03[墨氲](https://me.csdn.net/dss_dssssd)阅读数：47








### 数组 Arrays

`char [] vowels = new char[5]`

和C++的 new 动态申请内存一样。。。
- for 循环遍历
```
for (int i = 0; i < vowels.Length; i++)
    Console.Write (vowels[i]);
```


数组的Length属性返回数组中的元素个数也就是数组的长度，数组一旦被创建，长度（length）不可改变。`System.Collection`中提供了长度可变的数组和字典等更高级的数据结构
- 
数组初始化：

`char[] vowels = new char[] {'a','e','i','o','u'};`

`char[] vowels = {'a','e','i','o','u'};`


所有的数组都继承于`System.Array`类，

### 默认的元素初始化

创建数组时，总是先使用默认值初始化，默认值是在内存中按比特位清0后的值。比如，创建一个整型数组，`int`是值类型，将会在一块连续的内存中创建1000个整数，每个元素的值为0

```
int[] a = new int[1000];
Console.Write (a[123]); // 0
```

#### 值类型和引用类型

当数组的元素类型是值类型还是引用类型对性能有重要的影响
- 值类型： 每个元素都将直接分配一个内存值

```
public struct Point { public int X, Y; }
...
Point[] a = new Point[1000];
int x = a[500].X; // 0
```
- 引用类型：每个元素分配一个null的引用，和C++一样，使用前要先完成初始化，否则出现`NullReferenceException`的错误

```
public class Point { public int X, Y; }
...
Point[] a = new Point[1000];
int x = a[500].X; // Runtime error, NullReferenceException
```

使用前先完成初始化：

```
Point[] a = new Point[1000];
for (int i = 0; i < a.Length; i++) // Iterate i from 0 to 999
a[i] = new Point(); // Set array element i with new point
```

### 多维数组 multidimensional arrays

分为rectangular and jagged（矩形数组和锯齿状的），
- Rectangular arrays：代表的是n维的内存块
- jagged arrays: 数组的数组

#### Rectangular arrays

在定义时使用**逗号**，将每一维度分开

GetLength(dim)方法得到指定维度上的长度

```
int[,] matrix = new int[3,3];
for (int i = 0; i < matrix.GetLength(0); i++)
    for (int j = 0; j < matrix.GetLength(1); j++)
        matrix[i,j] = i * 3 + j;

int[,] matrix = new int[,]
        {
        {0,1,2},
        {3,4,5},
        {6,7,8}
        };
```

#### Jagged arrays

使用连续的方括号代表每一个维度， 这种方式在c++中你们应该学过的

例子： 创建一个二维锯齿数组，最外侧维度的长度是3
`int[][] matrix = new int[3][];`

初始化：

```
for (int i = 0; i < matrix.Length; i++)
{
    matrix[i] = new int[3]; // Create inner array
    for (int j = 0; j < matrix[i].Length; j++)
        matrix[i][j] = i * 3 + j;
}

int[][] matrix = new int[][]
{
new int[] {0,1,2},
new int[] {3,4,5},
new int[] {6,7,8,9}
};
// 两者混合
int[][,] jaggedArray4 = new int[3][,] 
{
    new int[,] { {1,3}, {5,7} },
    new int[,] { {0,2}, {4,6}, {8,10} },
    new int[,] { {11,22}, {99,88}, {0,9} } 
};
```

**利用c++指针的观点理解jagged数组，用c++中普通的二维数组理解Rectangular数组**

可以使用var类型初始化，类型交给编译器去推断。

```
var i = 3; // i is implicitly of type int
var s = "sausage"; // s is implicitly of type string
// Therefore:
var rectMatrix = new int[,] // rectMatrix is implicitly of type int[,]
{
{0,1,2},
{3,4,5},
{6,7,8}
};
var jaggedMat = new int[][] // jaggedMat is implicitly of type int[][]
{
new int[] {0,1,2},
new int[] {3,4,5},
new int[] {6,7,8}
};

//更隐式的类型转换
var x = new[] {1,10000000000}; // all convertible to long
```

### Bounds Checking 便捷检查

所有的数组索引都会进行边界检查，无效的索引会抛出`IndexOutOfRangeException`异常错误

```
int[] arr = new int[3];
arr[3] = 1; // IndexOutOfRangeException thrown
```



