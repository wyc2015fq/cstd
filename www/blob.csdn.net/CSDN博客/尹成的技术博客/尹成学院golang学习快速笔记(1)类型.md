
# 尹成学院golang学习快速笔记(1)类型 - 尹成的技术博客 - CSDN博客

置顶2018年05月26日 16:58:28[尹成](https://me.csdn.net/yincheng01)阅读数：920



**1.1****变量**
Go是静态类型语⾔，不能在运⾏期改变变量类型。
使⽤关键字var定义变量，⾃动初始化为零值。如果提供初始化值，可省略变量类型，由
编译器⾃动推断。
```python
var x int
var f float32 = 1.6
var s = "abc"
```
在函数内部，可⽤更简略的":="⽅式定义变量。
```python
func main() {
x := 123
// 注意检查，是定义新局部变量，还是修改全局变量。该⽅式容易造成错误。
}
```
可⼀次定义多个变量。
```python
var x, y, z int
var s, n = "abc", 123
var (
a int
b float32
)
func main() {
n, s := 0x1234, "Hello, World!"
println(x, s, n)
}
```

多变量赋值时，先计算所有相关值，然后再从左到右依次赋值。
```python
data, i := [3]int{0, 1, 2}, 0
i, data[i] = 2, 100+l
// (i = 0) -> (i = 2), (data[0] = 100)
```
特殊只写变量"_"，⽤于忽略值占位。

```python
func test() (int, string) {
return 1, "abc"
}
func main() {
_, s := test()
println(s)
}
```
编译器会将未使⽤的局部变量当做错误。


```python
var s string
func main() {
i := 0
// 全局变量没问题。
// Error: i declared and not used。(可使⽤ "_ = i" 规避)
}
```
注意重新赋值与定义新同名变量的区别。

```python
s := "abc"
println(&s)
s, y := "hello", 20
println(&s, y)
{
s, z := 1000, 30
println(&s, z)

// 重新赋值: 与前 s 在同⼀层次的代码块中，且有新的变量被定义。
// 通常函数多返回值 err 会被重复使⽤。
// 定义新同名变量: 不在同⼀层次代码块。
}
```

输出：
0x2210230f30
0x2210230f3020
0x2210230f1830
**1.2****常量**
常量值必须是编译期可确定的数字、字符串、布尔值。

```python
const x, y int = 1, 2
const s = "Hello, World!"
const (
// 多常量初始化
// 类型推断
// 常量组
a, b
c= 10, 100
bool = false
)
func main() {
const x = "xxx"
// 未使⽤局部常量不会引发编译错误。
}
```

不⽀持1UL、2LL这样的类型后缀。
在常量组中，如不提供类型和初始化值，那么视作与上⼀常量相同。
```python
const (
s= "abc"
x// x = "abc"
)
```

常量值还可以是len、cap、unsafe.Sizeof等编译期可确定结果的函数返回值。

```python
const (
a= "abc"
b= len(a)
c= unsafe.Sizeof(b)
)
```

如果常量类型⾜以存储初始化值，那么不会引发溢出错误。

```python
const (
a byte = 100
// int to byte
b int= 1e20
// float64 to int, overflows
)
```


**枚举**
关键字iota定义常量组中从0开始按⾏计数的⾃增枚举值。

```python
const (
Sunday = iota  // 0
Monday  // 1，通常省略后续⾏表达式。
Tuesday  // 2
Wednesday  // 3
Thursday  // 4
Friday  // 5
Saturday // 6
)
```

```python
const (
_
KB= iota
MB
GB
TB
int64 = 1 << (10 * iota)
// iota = 0
// iota = 1
// 与 KB 表达式相同，但 iota = 2
)
```
在同⼀常量组中，可以提供多个iota，它们各⾃增⻓。

```python
const (
A, B = iota, iota << 10
C, D
// 0, 0 << 10
// 1, 1 << 10
)
```

如果iota⾃增被打断，须显式恢复。

```python
const (
A= iota// 0
B= "c" // 1
C= iota// c
D // c，与上⼀⾏相同。
E // 4，显式恢复。注意计数包含了 C、D 两⾏。
F// 5
)
```

可通过⾃定义类型来实现枚举类型限制。
type Color int
const (
Black Color = iota
Red
Blue
)
 
func test(c Color) {}
func main() {
c := Black
test(c)
x := 1
test(x) // Error: cannot use x (type int) as type Color in function argument

test(1) // 常量会被编译器⾃动转换。

}


|类型
|⻓度
|默认值
|说明
|
|bool
|1
|false
|
|byte
|1
|0
|uint8
|
|rune
|4
|0
|Unicode|Code|Point,|int32
|
|int,|uint
|4|或|8
|0
|32|或|64|位
|
|int8,|uint8
|1
|0
|-128|~|127,|0|~|255
|
|int16,|uint16
|2
|0
|-32768|~|32767,|0|~|65535
|
|int32,|uint32
|4
|0
|-21|亿|~|21|亿|,|0|~|42|亿
|
|int64,|uint64
|8
|0
|
|float32
|4
|0.0
|
|float64
|8
|0.0
|
|complex64
|8

|
|complex128
|16

|
|uintptr
|4|或|8
|⾜以存储指针的|uint32|或|uint64|整数
|
|array

|值类型
|
|struct

|值类型
|
|string
|""
|UTF-8|字符串
|
|slice
|nil
|引⽤类型
|
|map
|nil
|引⽤类型
|
|channel
|nil
|引⽤类型
|
|interface
|nil
|接⼝
|
|function
|nil
|函数
|




**1.3****基本类型**
明确字类型命名，⽀持Unicode，⽀持常⽤数据结构。
⽀持⼋进制、⼗六进制，以及科学记数法。标准库math定义了各数字类型取值范围。
```python
a, b, c, d := 071, 0x1F, 1e9, math.MinInt16
```
空指针值nil，⽽⾮C/C++NULL。

**1.4****引⽤类型**
引⽤类型包括slice、map和channel。它们有复杂的内部结构，除了申请内存外，还需
要初始化相关属性。
内置函数new计算类型⼤⼩，为其分配零值内存，返回指针。⽽make会被编译器翻译
成具体的创建函数，由其分配内存和初始化成员结构，返回对象⽽⾮指针。


```python
a := []int{0, 0, 0}
a[1] = 10
b := make([]int, 3)
b[1] = 10
c := new([]int)
c[1] = 10
// 提供初始化表达式。
// makeslice
// Error: invalid operation: c[1] (index of type *[]int)
```


**1.5****类型转换**
不⽀持隐式类型转换，即便是从窄向宽转换也不⾏。
```python
var b byte = 100
// var n int = b
var n int = int(b)
// Error: cannot use b (type byte) as type int in assignment
// 显式转换
```

使⽤括号避免优先级错误。

```python
*Point(p)
(*Point)(p)
<-chan int(c)
(<-chan int)(c)
// 相当于 *(Point(p))
// 相当于 <-(chan int(c))
```

同样不能将其他类型当bool值使⽤。

```python
a := 100
if a {
println("true")
// Error: non-bool a (type int) used as if condition
}
```


**1.6****字符串**
字符串是不可变值类型，内部⽤指针指向UTF-8字节数组。

•默认值是空字符串""。
•⽤索引号访问某字节，如s[i]。
•不能⽤序号获取字节元素指针，&s[i]⾮法。
•不可变类型，⽆法修改字节数组。
•字节数组尾部不包含NULL。

```python
//runtime.h
struct String
{

byte*
intgo
str;
len;
};
```

使⽤索引号问字符(byte)。
```python
s := "abc"
println(s[0] == '\x61', s[1] == 'b', s[2] == 0x63)
```
输出：
truetruetrue

使⽤"`"定义不做转义处理的原始字符串，⽀持跨⾏。

```python
s := `a
b\r\n\x00
c`
 
println(s)
```
输出:
a
b\r\n\x00
c

连接跨⾏字符串时，"+"必须在上⼀⾏末尾，否则导致编译错误。

```python
s := "Hello, " +
"World!"
s2 := "Hello, "
+ "World!"
// Error: invalid operation: + untyped string
```


⽀持⽤两个索引号返回⼦串。⼦串依然指向原字节数组，仅修改了指针和⻓度属性。

```python
s := "Hello, World!"
s1 := s[:5]
// Hello
s2 := s[7:]
s3 := s[1:5]
// Hello
// World!
// ello
```



单引号字符常量表⽰UnicodeCodePoint，⽀持\uFFFF、\U7FFFFFFF、\xFF格式。
对应rune类型，UCS-4。

```python
func main() {
fmt.Printf("%T\n", 'a')
 
var c1, c2 rune = '\u6211', '们'
println(c1 == '我', string(c2) == "\xe4\xbb\xac")
}
```
输出：
int32
//rune是int32的别名
truetrue

要修改字符串，可先将其转换成[]rune或[]byte，完成后再转换为string。⽆论哪种转
换，都会重新分配内存，并复制字节数组。

```python
func main() {
s := "abcd"
bs := []byte(s)
bs[1] = 'B'
println(string(bs))
u := "电脑"
us := []rune(u)
us[1] = '话'
println(string(us))
}
```

输出:
aBcd
电话

⽤for循环遍历字符串时，也有byte和rune两种⽅式。
```python
func main() {
s := "abc汉字"
for i := 0; i < len(s); i++ {
fmt.Printf("%c,", s[i])
}
fmt.Println()
for _, r := range s {
fmt.Printf("%c,", r)
}
// byte
// rune
}
```

输出：
a,b,c,æ,±,,å,­,,
a,b,c,汉,字,

**1.7****指针**
⽀持指针类型*T，指针的指针**T，以及包含包名前缀的*<package>.T。

•默认值nil，没有NULL常量。
•操作符"&"取变量地址，"*"透过指针访问⺫标对象。
•不⽀持指针运算，不⽀持"->"运算符，直接⽤"."访问⺫标成员。

```python
func main() {
type data struct{ a int }
var d = data{1234}
var p *data
p = &d
fmt.Printf("%p, %v\n", p, p.a)
}
```

输出：
0x2101ef018,1234

不能对指针做加减法等运算。

```python
x := 1234
p := &x
```

```python
// 直接⽤指针访问⺫标对象成员，⽆须转换。

 

p++

// Error: invalid operation: p += 1 (mismatched types *int and int)
```

可以在unsafe.Pointer和任意类型指针间进⾏转换。

```python
func main() {
x := 0x12345678
p := unsafe.Pointer(&x)
n := (*[4]byte)(p)
for i := 0; i < len(n); i++ {
fmt.Printf("%X ", n[i])
}
// *int -> Pointer
// Pointer -> *[4]byte
}
```

输出：
78563412

返回局部变量指针是安全的，编译器会根据需要将其分配在GCHeap上。

```python
func test() *int {
x := 100
return &x

// 在堆上分配 x 内存。但在内联时，也可能直接分配在⺫标栈。

}
```

将Pointer转换成uintptr，可变相实现指针运算。

```python
func main() {
d := struct {
s
x

string
int
}{"abc", 100}
p := uintptr(unsafe.Pointer(&d))
p += unsafe.Offsetof(d.x)
// *struct -> Pointer -> uintptr
// uintptr + offset
p2 := unsafe.Pointer(p)
px := (*int)(p2)
*px = 200
fmt.Printf("%#v\n", d)
// uintptr -> Pointer
// Pointer -> *int
// d.x = 200
}
```

输出：
struct{sstring;xint}{s:"abc",x:200}
注意：GC把uintptr当成普通整数对象，它⽆法阻⽌"关联"对象被回收。

**1.8****⾃定义类型**

可将类型分为命名和未命名两⼤类。命名类型包括bool、int、string等，⽽array、
slice、map等和具体元素类型、⻓度等有关，属于未命名类型。
具有相同声明的未命名类型被视为同⼀类型。
•具有相同基类型的指针。
•具有相同元素类型和⻓度的array。
•具有相同元素类型的slice。
•具有相同键值类型的map。
•具有相同元素类型和传送⽅向的channel。
•具有相同字段序列(字段名、类型、标签、顺序)的匿名struct。
•签名相同(参数和返回值，不包括参数名称)的function。
•⽅法集相同(⽅法名、⽅法签名相同，和次序⽆关)的interface。

```python
var a struct { x int `a` }
var b struct { x int `ab` }
 
// cannot use a (type struct { x int "a" }) as type struct { x int "ab" } in assignment
b = a
```

可⽤type在全局或函数内定义新类型。

```python
func main() {
type bigint int64
 
var x bigint = 100
println(x)
}
```

新类型不是原类型的别名，除拥有相同数据存储结构外，它们之间没有任何关系，不会持
有原类型任何信息。除⾮⺫标类型是未命名类型，否则必须显式转换。

```python
x := 1234
var b bigint = bigint(x)
var b2 int64 = int64(b)
var s myslice = []int{1, 2, 3}
var s2 []int = s
// 必须显式转换，除⾮是常量。
// 未命名类型，隐式转换。
```




test(1)//常量会被编译器⾃动转换。


![](https://img-blog.csdn.net/20180430224554793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180425001235188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180425001144107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
网址：http://www.qukuailianxueyuan.io/
![](https://img-blog.csdn.net/20180426145827720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018042614570887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
欲领取造币技术与全套虚拟机资料
区块链技术交流QQ群：756146052备注：CSDN
尹成学院微信：备注：CSDN
![](https://img-blog.csdn.net/20180425000635656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



test(1)//常量会被编译器⾃动转换。

![](https://img-blog.csdn.net/20180430224554793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180425001235188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180425001144107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
网址：http://www.qukuailianxueyuan.io/
![](https://img-blog.csdn.net/20180426145827720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018042614570887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
欲领取造币技术与全套虚拟机资料
区块链技术交流QQ群：756146052备注：CSDN
尹成学院微信：备注：CSDN
![](https://img-blog.csdn.net/20180425000635656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


