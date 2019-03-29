# GO语言总结



# (1)——基本知识



**1、注释（与C++一样）**

　　行注释：//　　块注释：/* 　　...　　*/

 

**2、标识符**

　　可以这么说，除了数字开头的不允许，符号开头的不允许，关键字不允许，其他的Unicode字符组合都可以。“_33”也可以是标识符、“我们”也可以是标识符。标识符也区分大小写。

　　（1）、以大写字母开头的标识符是公开的。（这个很有意思）

　　（2）、其他任何标识符都是私有的。　

　　（3）、空标识符“_”是一个占位符，用于赋值操作的时候，丢弃、忽略某个值。通常这样用：

go的方法一般会返回2个值，一个通常的返回、一个错误标识。如 fmt.Println(x)会返回2个值，一个是打印的字节数，一个是相应的error值，那么 count,_ = fmt.Println(x) 这行代码就忽略了相应的error值。

 

**3、常量——用const关键字声明**

　　（1）可以自动推断类型，

　　　　如：const a = 5

 

　　（2）可以显式指定类型，

　　　　如: const a int16 = 6　　// int16是一个整形类型

 

　　（PS：go语言常量、变量定义的格式均为：

　　　　关键字　　常量(变量)名　　类型　　=　　值）

 

　　（3）可以一次声明多个常量，

　　　　如：const ( a = 0 ; b = 2)，这叫分组声明。此时，第一个常量被默认设置为0，后续的常量默认设置为前面常量的表达式。

　　（PS: 但是希望用const (a b)表示a=0,b=0这样的是不行的，常量分组定义，第一个常量必须赋值、或为iota）

　　（PPS：注意，go语言中一般不需要分号来表示分隔，编译器会自动在每行的末尾添加分号，当然，添加分号也没有错，不过像liteide这样的IDE的自动格式化一般会帮你把分号删除，然后帮你换行）

 

　　（4）**关键字iota**表示连续的无类型的整数常量,iota默认为0，并逐步递增。

　　　　即，const(a = iota; b ; c )表示b和c此时都为iota，所以a为0，b为1，c为2。

 

　　（5）一行多个赋值中，iota不相互影响。**每次iota出现，它的值为0**，如：

```
const (
        i, j, k = 2 * iota, iota, iota + 2
        m, n, l
    )
```

 　　此时，i、j的值均为0，k的值为2，m、n、l 的值分别为2 、1 、3

　　（PS：go语言支持一行赋多个值）

 

**4、变量**

　　变量有2种定义方式：

　　（1）一种是用关键字var ,

　　例如： var i int 　　// 　　这里会自动设置默认值0,如果是字符串，默认为空

　　或者：var i = 8　　// 　　声明同时赋值，自动推导其类型

　　或者：var k int = 16 　　// 　　指明类型，声明并赋值

　　或者：var ( a int; b int; c int)　　// 　　分组声明，跟常量类似。

 

　　（2）另一种是用快速变量声明，就是用 := 操作符，它会声明并初始化一个变量，可以自动推导类型。不过这种声明有一个限制，它只能在函数内部使用，在函数外部会报错。

如： name := "chandler qian" // 自动推导类型为string　

　　值得注意到是，**:=操作符是声明并初始化**，就是说，在同一个作用域内，这个变量必须未声明过，不然是错误。如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
k, b := 7, 8
    fmt.Printf("before k=%d,b=%d\n", k, b)
    if k := 1; k != -1 {
        b := 3
        fmt.Printf("inner k=%d,b=%d\n", k, b)
    }
    fmt.Printf("after k=%d,b=%d\n", k, b)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

在下面的 if 作用于内，k、b重新用:=声明赋值，但没问题，因为出了 if 作用域，它们就消失了。

结果为：

```
before k=7,b=8
inner k=1,b=3
after k=7,b=8
```

而 if语句改为这样：

```
if k = 1; k != -1 {
    b := 3
    fmt.Printf("inner k=%d,b=%d\n", k, b)
}
```

那么最后的输出就是：

```
before k=7,b=8
inner k=1,b=3
after k=1,b=8
```

可见，“=” 是全局作用的赋值，而，“:=” 则会在自己的作用域内创建变量并赋值。

　　（3）整形字面量的类型会自动推断为int，浮点字面量会自动推断为float64,复数字面量会自动推断为complex128

 

**5、布尔**

　　Go语言会严格筛选使用比较操作符（<、<=、==、!=、>=、>）进行比较的值。这两个值必须是相同类型或实现了相同的接口。如：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test0() {
    var a int = 5
    var b float32 = 4.4
    if a > b {
        fmt.Println(">")
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 由于a和b类型不匹配，所以编译错误：invalid operation: a > b (mismatched types int and float32)，再看下面这个：

```
func test1() {
    var b float32 = 4.4
    if b > 7 {
        fmt.Println(">")
    }
}
```

 这个是可以的，虽然类型不匹配，但是b与无类型的整形数值常量7比较。无类型的数值常量可以用于任何比较。

 

 



# (2)——基本类型



　　上篇博文总结了Go语言的基础知识——[GO语言总结(1)——基本知识](http://www.cnblogs.com/zrtqsk/p/4121957.html)  ，本篇博文介绍Go语言的基本类型。

**一、整型**

　　go语言有13种整形，其中有2种只是名字不同，实质是一样的，所以，实质上go语言有11种整形。如下：

　　（1）int ：　　依赖不同平台下的实现，可以是int32或int64

　　（2）int8 :　　　　　　　　　　  (-128　　->　　127)

　　（3）int16: 　　　　　　　　　　(-32768　　->　　32767)

　　（4）int32:　　　　　　　　　　 (-2 147 483 648　　->　　2 147 483 647)

　　（5）int64 :　　　　　　　　　　(-9 223 372 036 854 775 808　　->　　9 223 372 036 854 775 807)

 

　　（6）unit :　　　　　　　　　　 依赖不同平台下的实现，可以是int32或int64

　　（7）**unit8 (又名byte)**:　　　　  (0　　->　　255)

　　（8）unit16 :　　　　　　　　   (0　　->　　65535)

　　（9）**unit32 (又名rune)**:　　　   (0　　->　　4 294 967 295)

　　（10）unit64 :　　　　　　　　 (0　　->　　18 446 744 073 709 551 615)

 

　　（11）**unitptr** : 　　　　　　　　恰好容纳指针值的类型，对32位平台是unit32，对64位平台是unit64

　　（PS :　值得注意的是，go语言中没有自动类型转换，这意味着，除了上篇博文分析的比较运算符之外，其他的操作符基本需要类型转换，才能进行操作。否则就是编译错误）

 

**二、浮点类型**

　　go语言有2种类型的浮点和两种类型的复数类型。

　　（1） float32　　　　　　　　(-3.402...×1038　　->　　3.402...×1038)

　　（2） float64　　　　　　　　(-1.797...×10308　　->　　+1.797...×10308)

　　（3） complex64　　　　　　( 实部、虚部都是一个float32 )

　　（4） complex128　　　　　 ( 实部、虚部都是一个float64 )

　　（PS：标准库math包中，包含了众多数学函数，其中所有函数均使用float64，

　　　　标准库math/cmplx包含众多复数相关的数学函数,其中所有函数均使用complex128）

　　（PPS : 跟数学中一样，Go语言使用后缀 i 表示复数,例如 5 + 5.1i ）

​      

**三、字符串**

　　Go语言的字符串是用UTF-8编码的变宽字符序列，每个字符都用一个或多个字节表示。这与Java不同，Java是采用UTF-16来表示，每个字符都对应2个字节。

　　（1）创建：一种是用双引号（"）括起来，它表示可解析的字符串，可以进行字符转义。另一种是用单引号（'）括起来，它表示原生的字符串，可以包含除反引号之外的任何字符，当然也可以换行。如下：　

```
func test1() {
    str1 := "\"it's me!\""
    str2 := `"it's me,too"`
    fmt.Println(str1)
    fmt.Println(str2)
}
```

输出为：

```
"it's me!"
"it's me,too"
```

　　（2）Go语言的字符串支持 “+=”操作，可以通过[n]获取索引处的原始字节、通过[n:m]、[n:]、[:m]来获取字节对应的字符串，若字符被截段，显示乱码。如：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test1() {
    str1 := "Go语言的字符串是用UTF-8编码的变宽字符序列，每个字符都用一个或多个字节表示。"
    fmt.Println(str1[4:15])
    fmt.Println(str1[5:15])
    fmt.Println(str1[5])
    fmt.Println(str1[:5])
    fmt.Println(len(str1))         //字节数
    fmt.Println(len([]rune(str1))) //字符数
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

输出为 ：

```
�言的字�
言的字�
232
Go语
109
41
```

　　（3）字符串支持常规的比较运算符操作，比较运算符在内存中一个字节一个字节的比较字符串。当然了，这个比较是按照utf-8编码的顺序进行比较的。

　　（4）上例的最后一个取字符数的操作：len([]rune(str1))，在Go语言中，一个字符串可以可以用一个rune（又名int32）数组来表示，每个rune都表示一个单一的字符。如：

```
func test1() {
    str1 := "Go语言的字符串是用UTF-8编码的变宽字符序列，每个字符都用一个或多个字节表示。"
    for _, char := range []rune(str1) {
        fmt.Println(char)
    }
}
```

这个操作会将str1的每个字符数字直接打印出来

 

**四、指针**

（1）指针的本质，用一个简单例子说明一下:

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test2() {
    a := "xyz"
    b := "opq"
    pa := &a  //pa为指向a的指针
    pp := &pa //pp为指向pa的指针
    fmt.Println(a, b, *pa, **pp)
    a += "zz" //a追加“zz”
    fmt.Println(a, b, *pa, **pp)
    *pa += "bb" //pp指向的值，追加"bb"
    fmt.Println(a, b, *pa, **pp)
    fmt.Println("打印a各种情况：", &a, a)
    fmt.Println("打印pa各种情况：", &pa, pa, *pa)
    fmt.Println("打印pp各种情况：", &pp, pp, *pp, **pp)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 输出如下:

```
xyz opq xyz xyz
xyzzz opq xyzzz xyzzz
xyzzzbb opq xyzzzbb xyzzzbb
打印a各种情况： 0xc0820001d0 xyzzzbb
打印pa各种情况： 0xc082038018 0xc0820001d0 xyzzzbb
打印pp各种情况： 0xc082038020 0xc082038018 0xc0820001d0 xyzzzbb
```

 可见，指针的本质就是一个存放逻辑地址的变量。

（2）有两种创建变量的语法，同时获得指向它们的指针：new(Type) 和 &Type{}, 如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
type point struct {
    x int
    y int
}

func test3() {
    a := point{1, 2}
    b := new(point)
    c := &point{}
    d := &point{3, 4}
    fmt.Println(a, b, c, d)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

输出为：

```
{1 2} &{0 0} &{0 0} &{3 4}
```

 Go语言打印指向结构体的指针时，会打印解引用后的结构体内容，同时会将&作为前缀，表示它是一个指针。

值得注意的是，如果Type不是一个可以用大括号初始化的类型，那就只能使用内置函数 new(Type)了。如下：

```
func test5() {
    //创建一个int变量，并获得它的指针
    var1 := new(int)
    //var1 := &int32{}
    fmt.Printf("%T->%v\n", var1, var1)
}
```

 

int不可以使用大括号初始化，所以第二种会报错，注释掉后，输出：

```
*int->0xc082006270
```

 

 



# (3)——数组和切片



　　上篇博文简单介绍了一下Go语言的基本类型——[GO语言总结(2)——基本类型](http://www.cnblogs.com/zrtqsk/p/4134008.html)，本篇博文开始介绍Go语言的数组和切片。

**一、数组**　

　　与其他大多数语言类似，Go语言的数组也是一个元素类型相同的定长的序列。

（1）数组的创建。

　　数组有3种创建方式：**[length]Type**　、**[N]Type{value1, value2, ... , valueN}**、**[...]Type{value1, value2, ... , valueN}**　如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test5() {
    var iarray1 [5]int32
    var iarray2 [5]int32 = [5]int32{1, 2, 3, 4, 5}
    iarray3 := [5]int32{1, 2, 3, 4, 5}
    iarray4 := [5]int32{6, 7, 8, 9, 10}
    iarray5 := [...]int32{11, 12, 13, 14, 15}
    iarray6 := [4][4]int32{{1}, {1, 2}, {1, 2, 3}}
    fmt.Println(iarray1)
    fmt.Println(iarray2)
    fmt.Println(iarray3)
    fmt.Println(iarray4)
    fmt.Println(iarray5)
    fmt.Println(iarray6)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　结果：

```
[0 0 0 0 0]
[1 2 3 4 5]
[1 2 3 4 5]
[6 7 8 9 10]
[11 12 13 14 15]
[[1 0 0 0] [1 2 0 0] [1 2 3 0] [0 0 0 0]]
```

我们看数组 iarray1，只声明，并未赋值，Go语言帮我们自动赋值为0。再看 iarray2 和 iarray3 ，我们可以看到，Go语言的声明，可以表明类型，也可以不表明类型，var iarray3 = [5]int32{1, 2, 3, 4, 5} 也是完全没问题的。

 

（2）数组的容量和长度是一样的。cap() 函数和 len() 函数均输出数组的容量（即长度）。如：

```
func test6() {
    iarray4 := [5]int32{6, 7, 8, 9, 10}
    fmt.Println(len(iarray4))
    fmt.Println(cap(iarray4))
}
```

输出都是5。

 

（3）使用：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test7() {
    iarray7 := [5]string{"aaa", `bb`, "可以啦", "叫我说什么好", "()"}
    fmt.Println(iarray7)
    for i := range iarray7 {
        fmt.Println(iarray7[i])
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

输出：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test7() {
    iarray7 := [5]string{"aaa", `bb`, "可以啦", "叫我说什么好", "()"}
    fmt.Println(iarray7)
    for i := range iarray7 {
        fmt.Println(iarray7[i])
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

 

**二、切片**

　　Go语言中，切片是长度可变、容量固定的相同的元素序列。Go语言的切片本质是一个数组。容量固定是因为数组的长度是固定的，切片的容量即隐藏数组的长度。长度可变指的是在数组长度的范围内可变。

 

（1）切片的创建。

　　切片的创建有4种方式：

　　1）make ( []Type ,length, capacity )

　　2)  make ( []Type, length)

　　3) []Type{}

　　4) []Type{value1 , value2 , ... , valueN } 

从3)、4)可见，创建切片跟创建数组唯一的区别在于 Type 前的“ [] ”中是否有数字，为空，则代表切片，否则则代表数组。因为切片是长度可变的。如下是创建切片的示例：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test8() {
    slice1 := make([]int32, 5, 8)
    slice2 := make([]int32, 9)
    slice3 := []int32{}
    slice4 := []int32{1, 2, 3, 4, 5}
    fmt.Println(slice1)
    fmt.Println(slice2)
    fmt.Println(slice3)
    fmt.Println(slice4)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

输出为：

```
[0 0 0 0 0]
[0 0 0 0 0 0 0 0 0]
[]
[1 2 3 4 5]
```

 如上，创造了4个切片，3个空切片，一个有值的切片。

 

（2）切片与隐藏数组：

一个切片是一个隐藏数组的引用，并且对于该切片的切片也引用同一个数组。如下示例，创建了一个切片 slice0，并根据这个切片创建了2个切片 slice1 和 slice2：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test9() {
    slice0 := []string{"a", "b", "c", "d", "e"}
    slice1 := slice0[2 : len(slice0)-1]
    slice2 := slice0[:3]
    fmt.Println(slice0, slice1, slice2)
    slice2[2] = "8"
    fmt.Println(slice0, slice1, slice2)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

输出为：

```
[a b c d e] [c d] [a b c]
[a b 8 d e] [8 d] [a b 8]
```

可见，切片slice0 、 slice1 和 slice2是同一个底层数组的引用，所以slice2改变了，其他两个都会变。

 

（3）遍历、修改切片：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test10() {
    slice0 := []string{"a", "b", "c", "d", "e"}
    fmt.Println("\n~~~~~~元素遍历~~~~~~")
    for _, ele := range slice0 {
        fmt.Print(ele, " ")
        ele = "7"
    }
    fmt.Println("\n~~~~~~索引遍历~~~~~~")
    for index := range slice0 {
        fmt.Print(slice0[index], " ")
    }
    fmt.Println("\n~~~~~~元素索引共同使用~~~~~~")
    for index, ele := range slice0 {
        fmt.Print(ele, slice0[index], " ")
    }
    fmt.Println("\n~~~~~~修改~~~~~~")
    for index := range slice0 {
        slice0[index] = "9"
    }
    fmt.Println(slice0)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

如上，前三种循环使用了不同的for range循环，当for后面，range前面有2个元素时，第一个元素代表索引，第二个元素代表元素值，使用 “_” 则表示忽略，因为go语言中，未使用的值会导致编译错误。

只有一个元素时，该元素代表索引。

只有用索引才能修改元素。如在第一个遍历中，赋值ele为7，结果没有作用。因为在元素遍历中，ele是值传递，ele是该切片元素的副本，修改它不会影响原本值，而在第四个遍历——索引遍历中，修改的是该切片元素引用的值，所以可以修改。

结果为：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
~~~~~~元素遍历~~~~~~
a b c d e 
~~~~~~索引遍历~~~~~~
a b c d e 
~~~~~~元素索引共同使用~~~~~~
aa bb cc dd ee 
~~~~~~修改~~~~~~
[9 9 9 9 9]
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

（4）、追加、复制切片：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test11() {
    slice := []int32{}
    fmt.Printf("slice的长度为：%d,slice为：%v\n", len(slice), slice)
    slice = append(slice, 12, 11, 10, 9)
    fmt.Printf("追加后，slice的长度为：%d,slice为：%v\n", len(slice), slice)
    slicecp := make([]int32, (len(slice)))
    fmt.Printf("slicecp的长度为：%d,slicecp为：%v\n", len(slicecp), slicecp)
    copy(slicecp, slice)
    fmt.Printf("复制赋值后，slicecp的长度为：%d,slicecp为：%v\n", len(slicecp), slicecp)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

追加、复制切片，用的是内置函数append和copy，copy函数返回的是最后所复制的元素的数量。

 

（5）、内置函数append

内置函数append可以向一个切片后追加一个或多个同类型的其他值。如果追加的元素数量超过了原切片容量，那么最后返回的是一个全新数组中的全新切片。如果没有超过，那么最后返回的是原数组中的全新切片。无论如何，append对原切片无任何影响。如下示例：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test12() {
    slice := []int32{1, 2, 3, 4, 5, 6}
    slice2 := slice[:2]
    _ = append(slice2, 50, 60, 70, 80, 90)
    fmt.Printf("slice为：%v\n", slice)
    fmt.Printf("操作的切片：%v\n", slice2)
    _ = append(slice2, 50, 60)
    fmt.Printf("slice为：%v\n", slice)
    fmt.Printf("操作的切片：%v\n", slice2)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

如上，append方法用了2次，结果返回的结果完全不同，原因是第二次append方法追加的元素数量没有超过 slice 的容量。而无论怎样，原切片slice2都无影响。结果：

```
slice为：[1 2 3 4 5 6]
操作的切片：[1 2]
slice为：[1 2 50 60 5 6]
操作的切片：[1 2]
```

 

 





# [(4)——映射（Map）](https://www.cnblogs.com/zrtqsk/p/4150129.html)



上一篇博客介绍了Go语言的数组和切片——[GO语言总结(3)——数组和切片](http://www.cnblogs.com/zrtqsk/p/4148495.html)，本篇博客介绍Go语言的映射（Map）

映射是一种内置的数据结构，用来保存键值对的无序集合。

**（1）映射的创建**

make ( map [KeyType] ValueType, initialCapacity )

make ( map [KeyType] ValueType )

map [KeyType ] ValueType {}

map [KeyType ] ValueType { key1 : value1, key2: value2, ... , keyN : valueN}

如下，用4种方式分别创建数组，其中第一种和第二种的区别在于，有没有指定初始容量，不过使用的时候则无需在意这些，因为map的本质决定了，一旦容量不够，它会自动扩容：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test1() {
    map1 := make(map[string]string, 5)
    map2 := make(map[string]string)
    map3 := map[string]string{}
    map4 := map[string]string{"a": "1", "b": "2", "c": "3"}
    fmt.Println(map1, map2, map3, map4)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

输出如下：

```
map[] map[] map[] map[c:3 a:1 b:2]
```

 

**（2）映射的填充和遍历**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test2() {
    map1 := make(map[string]string)
    map1["a"] = "1"
    map1["b"] = "2"
    map1["c"] = "3"
    for key, value := range map1 {
        fmt.Printf("%s->%-10s", key, value)
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

如上，数组的填充使用 map[key] = value 的方式，遍历映射的时候，每一项都返回2个值，键和值。结果如下：

```
a->1         b->2         c->3    
```

 

**（3）映射的查找、修改和删除**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test3() {
    map4 := map[string]string{"a": "1", "b": "2", "c": "3"}
    val, exist := map4["a"]
    val2, exist2 := map4["d"]
    fmt.Printf("%v,%v\n", exist, val)
    fmt.Printf("%v,%v\n", exist2, val2)

    map4["a"] = "8" //修改映射和添加映射没什么区别
    fmt.Printf("%v\n", map4)

    fmt.Println("删除b：")
    delete(map4, "b")
    fmt.Printf("%v", map4)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

map指定key取对应的value时，可以指定返回两个值，第一个是对应的value，第二个是一个bool，表示是否有值。如上，“a”肯定有值，"b"肯定没值。

修改映射和添加映射的操作没什么区别，若指定的键不存在则创建，否则，修改之。

删除则是使用go的内置函数delete,输出如下：

 

```
true,1
false,
map[a:8 b:2 c:3]
删除b：
map[a:8 c:3]
```

 

 



# [(5)——类型转换和类型断言](https://www.cnblogs.com/zrtqsk/p/4157350.html)



上一篇博客介绍了Go语言的数组和切片——[GO语言总结(4)——映射（Map）](http://www.cnblogs.com/zrtqsk/p/4150129.html)，本篇博客介绍Go语言的类型转换和类型断言

由于Go语言不允许隐式类型转换。而类型转换和类型断言的本质，就是把一个类型转换到另一个类型。

 

**一、类型转换**

（1）、**语法**：<结果类型> := <目标类型> ( <表达式> )

（2）、**类型转换是用来在不同但相互兼容的类型之间的相互转换的方式**，所以，当类型不兼容的时候，是无法转换的。如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test4() {
    var var1 int = 7
    fmt.Printf("%T->%v\n", var1, var1)
    var2 := float32(var1)
    var3 := int64(var1)
    //var4 := []int8(var1)
    //var5 := []string(var1)
    fmt.Printf("%T->%v\n", var2, var2)
    fmt.Printf("%T->%v\n", var3, var3)
    //fmt.Printf("%T->%d", var4, var4)
    //fmt.Printf("%T->%d", var5, var5)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

其中，var4和var5处运行会报错。因为类型不兼容。注释后，输出如下：

```
int->7
float32->7
int64->7
```

值得注意的是，如果某些类型可能引起误会，应该用括号括起来转换，如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test5() {
    //创建一个int变量，并获得它的指针
    var1 := new(int32)
    fmt.Printf("%T->%v\n", var1, var1)
    var2 := *int32(var1)
    fmt.Printf("%T->%v\n", var2, var2)
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

*int32(var1)相当于*(int32(var1))，一个指针，当然不能直接转换成一个int32类型，所以该表达式直接编译错误。将该表达式改为 (*int32)(var1)就可以正常输出了。

 

**二、类型断言**

（1）语法：

　　<目标类型的值>，<布尔参数> := <表达式>**.**( 目标类型 ) // 安全类型断言

　　<目标类型的值> := <表达式>**.**( 目标类型 )　　//非安全类型断言

（2）类型断言的本质，跟类型转换类似，都是类型之间进行转换，不同之处在于，类型断言实在接口之间进行，相当于Java中，对于一个对象，把一种接口的引用转换成另一种。

我们先来看一个最简单的错误的类型断言：

```
func test6() {
    var i interface{} = "kk"
    j := i.(int)
    fmt.Printf("%T->%d\n", j, j)
}
```

var i interface{} = "KK" 某种程度上相当于java中的，Object i = "KK";

现在把这个 i 转换成 int 类型，系统内部检测到这种不匹配，就会调用内置的panic()函数，抛出一个异常。

改一下，把 i 的定义改为：var i interface{} = 99，就没问题了。输出为：

```
int->99
```

以上是不安全的类型断言。我们来看一下安全的类型断言：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
func test6() {
    var i interface{} = "TT"
    j, b := i.(int)
    if b {
        fmt.Printf("%T->%d\n", j, j)
    } else {
        fmt.Println("类型不匹配")
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

输出“类型不匹配”。

 