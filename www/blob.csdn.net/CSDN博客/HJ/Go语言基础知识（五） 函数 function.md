# Go语言基础知识（五） 函数 function - HJ - CSDN博客
2018年11月18日 22:34:43[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：44
```
package main
import (
	"fmt"
)
//单返回值
func plus(a, b int) int {
	return a + b
}
//多返回值
func multi() (string, int) {
	return "age is: ", 18
}
//定义变量多返回值
func nameReturnValue() (name string, height int) {
	name = "This value is:"
	height = 10
	return
}
//不定参数传参
func Sum(nums ...int) (res int) {
	fmt.Println("len of nums is : ", len(nums))
	res = 0
	for _, v := range nums {
		res += v
	}
	return
}
func sayHello(name string) {
	fmt.Println("Hello", name)
}
//函数作为形参
func logger(f func(string), name string) {
	fmt.Println("start calling method sayHello")
	f(name)
	fmt.Println("end calling method sayHello")
}
//函数传值
func sendValue(name string) {
	name = "上海"
}
//函数传引用
func sendAddres(name *string) {
	*name = "上海"
}
//闭包
func addInt(n int) func() int {
	i := 0
	return func() int {
		i += n
		return i
	}
}
func main() {
	res := plus(1, 2)
	fmt.Print(res)
	str, age := multi()
	fmt.Println(str)
	fmt.Println(age)
	name, height := nameReturnValue()
	fmt.Println(name, " ", height)
	fmt.Println(Sum(1))
	fmt.Println(Sum(1, 2))
	fmt.Println(Sum(1, 2, 3))
	func(name string) {
		fmt.Println(name)
	}("杭州")
	logger(sayHello, "杭州")
	str1 := "杭州"
	fmt.Println("before calling sendValue, str: ", str1)
	sendValue(str1)
	fmt.Println("after calling sendValue, str: ", str1)
	str2 := "杭州"
	fmt.Println("before calling sendAddress, str: ", str2)
	sendAddres(&str2)
	fmt.Println("after calling sendAddress, str: ", str2)
	fmt.Println()
	addOne := addInt(1)
	fmt.Println(addOne())
	fmt.Println(addOne())
	fmt.Println(addOne())
}
```
