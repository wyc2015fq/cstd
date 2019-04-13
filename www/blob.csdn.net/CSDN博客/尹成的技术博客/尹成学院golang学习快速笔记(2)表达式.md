
# 尹成学院golang学习快速笔记(2)表达式 - 尹成的技术博客 - CSDN博客

置顶2018年05月26日 17:04:04[尹成](https://me.csdn.net/yincheng01)阅读数：1230


2.1 保留字
语⾔言设计简练，保留字不多。
```python
break default func interface select
case defer go map struct
chan else goto package switch
const fallthrough if range type
continue for import return var
```

2.2 运算符
全部运算符、分隔符，以及其他符号。
```python
+ & += &= && == != ( )
- | -= |= || < <= [ ]
* ^ *= ^= <- > >= { }
/ << /= <<= ++ = := , ;
% >> %= >>= -- ! ... . :
&^ &^=
```

运算符结合律全部从左到右。
优先级 运算符 说明
```python
------------+---------------------------------------------+----------------------------
high * / & << >> & &^
+ - |? ^
== != < <= < >=
<- channel
&&
low ||
```

简单位运算演⽰示。
0110 & 1011 = 0010 AND 都为 1。
0110 | 1011 = 1111 OR ⾄至少⼀一个为 1。
0110 ^ 1011 = 1101 XOR 只能⼀一个为 1。
0110 &^ 1011 = 0100 AND NOT 清除标志位。
标志位操作。
```python
a := 0
a |= 1 << 2 // 0000100: 在 bit2 设置标志位。
a |= 1 << 6 // 1000100: 在 bit6 设置标志位
a = a &^ (1 << 6) // 0000100: 清除 bit6 标志位。
不⽀支持运算符重载。尤其需要注意，"++"、"--" 是语句⽽而⾮非表达式。
n := 0
p := &n
// b := n++ // syntax error
// if n++ == 1 {} // syntax error
// ++n // syntax error
n++
*p++ // (*p)++
没有 "~"，取反运算也⽤用 "^"。
x := 1
x, ^x // 0001, -0010
```

2.3 初始化
初始化复合对象，必须使⽤用类型标签，且左⼤大括号必须在类型尾部。
```python
// var a struct { x int } = { 100 } // syntax error
// var b []int = { 1, 2, 3 } // syntax error
// c := struct {x int; y string} // syntax error: unexpected semicolon or newline
// {
// }
var a = struct{ x int }{100}
var b = []int{1, 2, 3}
初始化值以 "," 分隔。可以分多⾏行，但最后⼀一⾏行必须以 "," 或 "}" 结尾。
a := []int{
1,
2 // Error: need trailing comma before newline in composite literal
}
a := []int{
1,
2, // ok
}
b := []int{
1,
2 } // ok
```
2.4 控制流
2.4.1 IF
很特别的写法：
•可省略条件表达式括号。
•⽀支持初始化语句，可定义代码块局部变量。
•代码块左⼤大括号必须在条件表达式尾部。
```python
x := 0
// if x > 10 // Error: missing condition in if statement
// {
// }
if n := "abc"; x > 0 { // 初始化语句未必就是定义变量，⽐比如 println("init") 也是可以的。
println(n[2])
} else if x < 0 { // 注意 else if 和 else 左⼤大括号位置。
println(n[1])
} else {
println(n[0])
}
```

不⽀支持三元操作符 "a > b ? a : b"。
2.4.2 For

⽀支持三种循环⽅方式，包括类 while 语法。
```python
s := "abc"
for i, n := 0, len(s); i < n; i++ { // 常⻅见的 for 循环，⽀支持初始化语句。
println(s[i])
}
n := len(s)
for n > 0 { // 替代 while (n > 0) {}
println(s[n]) // 替代 for (; n > 0;) {}
n--
}
for { // 替代 while (true) {}
println(s) // 替代 for (;;) {}
}
//不要期望编译器能理解你的想法，在初始化语句中计算出全部结果是个好主意。
func length(s string) int {
println("call length.")
return len(s)
}
func main() {
s := "abcd"
for i, n := 0, length(s); i < n; i++ { // 避免多次调⽤用 length 函数。
println(i, s[i])
}
}
```

输出：
call length.
0 97
1 98
2 99
3 100
2.4.3 Range
类似迭代器操作，返回 (索引, 值) 或 (键, 值)。
1st value 2nd value
------------------+-------------------+------------------+-------------------
string index s[index] unicode, rune
array/slice index s[index]
map key m[key]
channel element
可忽略不想要的返回值，或⽤用 "_" 这个特殊变量。
```python
s := "abc"
for i := range s { // 忽略 2nd value，⽀支持 string/array/slice/map。
println(s[i])
}
for _, c := range s { // 忽略 index。
println(c)
}
for range s { // 忽略全部返回值，仅迭代。
...
}
m := map[string]int{"a": 1, "b": 2}
for k, v := range m { // 返回 (key, value)。
println(k, v)
}
注意，range 会复制对象。
a := [3]int{0, 1, 2}
for i, v := range a { // index、value 都是从复制品中取出。
if i == 0 { // 在修改前，我们先修改原数组。
a[1], a[2] = 999, 999
fmt.Println(a) // 确认修改有效，输出 [0, 999, 999]。
}
a[i] = v + 100 // 使⽤用复制品中取出的 value 修改原数组。
}
fmt.Println(a) // 输出 [100, 101, 102]。
建议改⽤用引⽤用类型，其底层数据不会被复制。
s := []int{1, 2, 3, 4, 5}
for i, v := range s { // 复制 struct slice { pointer, len, cap }。
if i == 0 {
s = s[:3] // 对 slice 的修改，不会影响 range。
s[2] = 100 // 对底层数据的修改。
}
println(i, v)
}
```

输出：
0 1
1 2
2 100
3 4
4 5
另外两种引⽤用类型 map、channel 是指针包装，⽽而不像 slice 是 struct。
2.4.4 Switch
分⽀支表达式可以是任意类型，不限于常量。可省略 break，默认⾃自动终⽌止。
```python
x := []int{1, 2, 3}
i := 2
switch i {
case x[1]:
println("a")
case 1, 3:
println("b")
default:
println("c")
}
```

输出：
a
如需要继续下⼀一分⽀支，可使⽤用 fallthrough，但不再判断条件。
```python
x := 10
switch x {
case 10:
println("a")
fallthrough
case 0:
println("b")
}
```

输出：
a
b
省略条件表达式，可当 if...else if...else 使⽤用。
```python
switch {
case x[1] > 0:
println("a")
case x[1] < 0:
println("b")
default:
println("c")
}
switch i := x[2]; { // 带初始化语句
case i > 0:
println("a")
case i < 0:
println("b")
default:
println("c")
}
```

2.4.5 Goto, Break, Continue
⽀支持在函数内 goto 跳转。标签名区分⼤大⼩小写，未使⽤用标签引发错误。
```python
func main() {
var i int
for {
println(i)
i++
if i > 2 { goto BREAK }
}
BREAK:
println("break")
EXIT: // Error: label EXIT defined and not used
}
```

配合标签，break 和 continue 可在多级嵌套循环中跳出。
```python
func main() {
L1:
for x := 0; x < 3; x++ {
L2:
for y := 0; y < 5; y++ {
if y > 2 { continue L2 }
if x > 1 { break L1 }
print(x, ":", y, " ")
}
println()
}
}
```

输出：
0:0 0:1 0:2
1:0 1:1 1:2
附：break 可⽤用于 for、switch、select，⽽而 continue 仅能⽤用于 for 循环。
```python
x := 100
switch {
case x >= 0:
if x == 0 { break }
println(x)
}
```


