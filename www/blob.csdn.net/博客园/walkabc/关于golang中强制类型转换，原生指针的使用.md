# 关于golang中强制类型转换，原生指针的使用 - walkabc - 博客园
# [关于golang中强制类型转换，原生指针的使用](https://www.cnblogs.com/hitfire/articles/6363696.html)
　　Golang中如何进行类型转换，也正好可以探究下golang的内存布局
```go;gutter
package main
import (
	"fmt"
)
type A struct {
	name string
	age  int8
}
type B struct {
	notname string
	notage  int8
	sex     int8
}
func main() {
	b := &B{notname: "我是B", notage: 90, sex: 1}
	a := (*A)(b)//这是编译不通过，可以接着看下面的代码
	fmt.Println(a.name)
}
```
　　上面的代码是通不过编译的
```go;gutter
./typecast.go:20: cannot convert b (type *B) to type *A
```
　　那么如何进行类型的强制类型转换呢？
```go;gutter
package main
import (
	"fmt"
	"unsafe"
)
type A struct {
	name string
	age  int8
}
type B struct {
	notname string
	notage  int8
	sex     int8
}
func main() {
	b := &B{notname: "我是B", notage: 90, sex: 1}
	a := (*A)(unsafe.Pointer(b))
	fmt.Println(a.name)
}
```
　　这样就能通过类型转换了。可以看到只要类型的内存布局一致就可以了，即使B类型多一个sex字段。
　　这样的转换方式是通过原生指针的方式进行转换的。　　

