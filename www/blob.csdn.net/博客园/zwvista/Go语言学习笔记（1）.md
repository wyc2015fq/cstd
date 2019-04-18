# Go语言学习笔记（1） - zwvista - 博客园

## [Go语言学习笔记（1）](https://www.cnblogs.com/zwvista/p/9477061.html)

### 包

package 声明包，import 导入包，导入的包名要用""，包中导出的名字以大写字母打头。

```
package main
import "fmt"
import "math"
import (
    "fmt"
    "math"
)
func main() {
    fmt.Println(math.Pi)
}
```

### 函数

函数使用 func 声明，采用类型后置语法，多个相同类型参数的声明可合并，可以返回多值（类似元组，成员可命名）。

```
func add(x int, y int) int {
    return x + y
}
func add(x, y int) int {
    return x + y
}
func swap(x, y string) (string, string) {
    return y, x
}
func split(sum int) (x, y int) {
    x = sum * 4 / 9
    y = sum - x
    return
}
```

### 变量和常量

变量使用 var 声明，采用类型后置语法，多个相同类型参数的声明可合并。

变量声明时可初始化，初始化的变量可省略类型，由编译器自行推导。

多个变量可同时初始化，类似其他语言的解构声明。

未显式初始化的变量用零值（0, false, ""）初始化。

存在简写形式 :=，此时省略 var。

常量使用 const 声明。

```
var c, python, java bool
var i int
var i, j int = 1, 2
var c, python, java = true, false, "no!"
k := 3
c, python, java := true, false, "no!"
const Pi = 3.14
const World = "世界"
const Truth = true
```

### 基本类型
- bool
- string
- int int8 int16 int32 int64
- uint uint8 uint16 uint32 uint64 uintptr
- byte // uint8 的别名
- rune // int32 的别名，表示一个 Unicode 码点
- float32 float64
- complex64 complex128

```
// 类型转换
i := 42
f := float64(i)
u := uint(f)
```

### 语句

```
// 传统 for 循环
sum := 0
for i := 0; i < 10; i++ {
    sum += i
}
// for 循环三个部分都可以省略
sum := 1
for ; sum < 1000; {
    sum += sum
}
// while 循环也用 for 来表达
sum := 1
for sum < 1000 {
    sum += sum
}
// loop 循环也用 for 来表达
for {
}
// if 语句可以带初始化语句
if v := math.Pow(x, n); v < lim {
    return v
} else {
    fmt.Printf("%g >= %g\n", v, lim)
}
// 这里开始就不能使用 v 了
// switch 语句可以带初始化语句
// switch 语句没有 break，case 无需为常量
switch os := runtime.GOOS; os {
case "darwin":
    fmt.Println("OS X.")
case "linux":
    fmt.Println("Linux.")
default:
    fmt.Printf("%s.", os)
}
// 没有条件的 switch 语句相当于多路分支
switch {
case t.Hour() < 12:
    fmt.Println("Good morning!")
case t.Hour() < 17:
    fmt.Println("Good afternoon.")
default:
    fmt.Println("Good evening.")
}
// defer 语句会将函数推迟到外层函数返回之后执行。
func main() {
    fmt.Println("counting")
    for i := 0; i < 10; i++ {
        defer fmt.Println(i)
    }
    fmt.Println("done")
}
// counting
// done
// 9
// 8
// 7
// ...
// 2
// 1
```

### 指针

```
i, j := 42, 2701
// var p *int = &i
p := &i         // point to i
*p = 21         // set i through the pointer
p = &j         // point to j
*p = *p / 37   // divide j through the pointer
```

### 结构体

```
type Vertex struct {
    X int
    Y int
}
v := Vertex{1, 2}
v.X = 4
p := &v
p.X = 1e9
var (
    v1 = Vertex{1, 2}  // has type Vertex
    v2 = Vertex{X: 1}  // Y:0 is implicit
    v3 = Vertex{}      // X:0 and Y:0
    p  = &Vertex{1, 2} // has type *Vertex
)
```


