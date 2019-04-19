# go的包 - 左直拳的马桶_日用桶 - CSDN博客
2018年11月22日 15:59:20[左直拳](https://me.csdn.net/leftfist)阅读数：135
go这种语言搞搞新意思，有些新奇的地方。比如函数多返回值，而且提取多返回值时可以指定提取哪个参数值；还可以对现有的类型进行扩展，类似C#里的this语法糖。我只是初学，新玩意应该还有不要。这说明go是一门相对新潮的语言，有一些新思想。
其中我认为最大的特点是go是一门函数式编程语言，里面好像没有类这些东东，不是面向对象的。当然里面有个type，据说已经涵括了类而大于类。代码结构，基本就是包，包里面是函数。而且函数都是public的，没有什么私有的概念。同一包名，函数直接使用；跨包的话，就`包名.函数`这样。
go的包有些约定，或者说可能是硬性规定。所谓约定大于配置。这种约定，也包括了代码文件的存放结构。我记录一下我的理解：
**0、函数名首字母大写是公有的，首字母小写是私有的**
**1、如果想可执行，必须有main包**
```
package main
import "fmt"
func main() {
    fmt.Println("Hello World!");
}
```
**2、包名应该与存放目录一致**
准确来说，除了main之外，包名应该与所在目录名字保持一致。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181122152711162.png)
**3、相应地，一个目录只能存放一种包的文件**
而且文件应该是平铺的，尽量避免嵌套，即使有嵌套，也不要太深，1、2层足矣。
以上是一些基本的条条框框。说容易，写起来磕磕绊绊。如上图，a.go与b.go同一包名main，
在a.go里使用了b.go的函数，同时还使用了子目录pc的c.go里的函数。代码如下：
a.go
```
package main
import (
	"fmt"
	"ref/pc"
)
func main(){
	_, reB := B()
	_, reC := pc.C()
	fmt.Println("Hello World!","\n\r" + reB,"\n\r" + reC)
}
```
b.go
```
package main
func B()  (s1 string,s2 string){
	return "yes","I am a duck!"
}
```
c.go
```
package pc
func C() (s1 string,s2 string){
	return "emm","you confirm your mistake now!"
}
```
这几个文件存放在ref文件夹下，ref又位于`%GOPATH%/src/`里，在GoLand里做相应配置，运行是没有问题的：
**一、GoLand里运行**
**1）代码要在GOPATH范围内**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181122154607922.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**2、按包运行**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181122154901800.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**二、命令行里直接运行**
如果只是运行文件，a.go里的写法会报错，说找不到包"ref/pc"。要改写为“./pc”
```
package main
import (
	"fmt"
	"./pc"
)
func main(){
	_, reB := B()
	_, reC := pc.C()
	fmt.Println("Hello World!","\n\r" + reB,"\n\r" + reC)
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181122155842718.png)
