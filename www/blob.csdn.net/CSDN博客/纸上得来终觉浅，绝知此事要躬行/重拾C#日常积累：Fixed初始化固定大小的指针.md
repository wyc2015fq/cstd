# 重拾C#日常积累：Fixed初始化固定大小的指针 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 16:03:59[boonya](https://me.csdn.net/boonya)阅读数：479








Fixed的说明


- Fixed 语句可防止垃圾回收器重新定位可移动的变量。 
- Fixed 语句仅允许存在于不安全的上下文中。 
- Fixed 可用于创建固定大小的缓冲区。
fixed 语句将为托管变量设置一个指针，并在该语句的执行过程中“单边锁定”该变量。 如果没有 fixed，指向可移动的托管变量的指针将几乎没有什么用处，因为垃圾回收可能会不可预见地重新定位变量。 C# 编译器只允许将指针分配给 fixed 语句中的托管变量。

```
unsafe static void TestMethod()
{
    
    // Assume that the following class exists.
    //class Point 
    //{ 
    //    public int x;
    //    public int y; 
    //}

    // Variable pt is a managed variable, subject to garbage collection.
    Point pt = new Point();

    // Using fixed allows the address of pt members to be taken,
    // and "pins" pt so that it is not relocated.
    
    fixed (int* p = &pt.x)
    {
        *p = 1;
    }        
   
}
```

可以通过使用一个数组、字符串、固定大小的缓冲区或变量的地址来初始化指针。下面的示例说明了变量地址、数组和字符串的使用。有关固定大小的缓冲区的详细信息，请参阅[固定大小的缓冲区](https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/unsafe-code-pointers/fixed-size-buffers)。

```
static unsafe void Test2()
{
    Point point = new Point();
    double[] arr = { 0, 1.5, 2.3, 3.4, 4.0, 5.9 };
    string str = "Hello World";

    // The following two assignments are equivalent. Each assigns the address
    // of the first element in array arr to pointer p.

    // You can initialize a pointer by using an array.
    fixed (double* p = arr) { /*...*/ }

    // You can initialize a pointer by using the address of a variable. 
    fixed (double* p = &arr[0]) { /*...*/ }

    // The following assignment initializes p by using a string.
    fixed (char* p = str) { /*...*/ }

    // The following assignment is not valid, because str[0] is a char, 
    // which is a value, not a variable.
    //fixed (char* p = &str[0]) { /*...*/ } 


    // You can initialize a pointer by using the address of a variable, such
    // as point.x or arr[5].
    fixed (int* p1 = &point.x)
    {
        fixed (double* p2 = &arr[5])
        {
            // Do something with p1 and p2.
        }
    }
}
```

可以初始化多个指针，只要它们的类型相同。
`fixed (byte* ps = srcarray, pd = dstarray) {...}  `
若要初始化不同类型的指针，只需嵌套 fixed 语句，如下面的示例中所示。

```
fixed (int* p1 = &point.x)
{
    fixed (double* p2 = &arr[5])
    {
        // Do something with p1 and p2.
    }
}
```

执行该语句中的代码之后，任何固定的变量都将被解锁并受垃圾回收的约束。 因此，请勿指向 fixed 语句之外的那些变量。

注：不能修改在固定语句中初始化的指针。

在不安全模式中，可以在堆栈上分配内存，在这种情况下，内存不受垃圾回收的约束，因此不需要固定。有关详细信息，请参阅 [stackalloc](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/stackalloc)。


```
class Point
{ 
    public int x, y; 
}

class FixedTest2 
{
    // Unsafe method: takes a pointer to an int.
    unsafe static void SquarePtrParam (int* p) 
    {
        *p *= *p;
    }

    unsafe static void Main() 
    {
        Point pt = new Point();
        pt.x = 5;
        pt.y = 6;
        // Pin pt in place:
        fixed (int* p = &pt.x) 
        {
            SquarePtrParam (p);
        }
        // pt now unpinned.
        Console.WriteLine ("{0} {1}", pt.x, pt.y);
    }
}
/*
Output:
25 6
 */
```
参考地址：[https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/fixed-statement](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/fixed-statement)




