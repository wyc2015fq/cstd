# golang []byte转string - walkabc - 博客园
# [golang []byte转string](https://www.cnblogs.com/hitfire/articles/6427078.html)
　　golang中，字符切片[]byte转换成string最简单的方式是
```go;gutter
package main
import (
	"fmt"
	_ "unsafe"
)
func main() {
	bytes := []byte("I am byte array !")
	str := string(bytes)
	bytes[0] = 'i'//注意这一行，bytes在这里修改了数据，但是str打印出来的依然没变化，
	fmt.Println(str)
}
```
　　打印信息:I am byte array !
　　上面的代码bytes在这里修改了数据，但是str打印出来的依然没变化，是因为[]byte的内存被拷贝了。接着看下面的代码
```go;gutter
package main
import (
	"fmt"
	"unsafe"
)
func main() {
	bytes := []byte("I am byte array !")
	str := (*string)(unsafe.Pointer(&bytes))
	bytes[0] = 'i'
	fmt.Println(*str)
}
```
　　打印信息:i am byte array !　
　　现在打印出来的信息已经是改变过的了，现在可以看出来str和bytes共用一片内存。
　　这样做的意义在于，在网络通信中，大多数的接受方式都是[]byte，如果[]byte的数据比较大，内存拷贝的话会影响系统的性能。

