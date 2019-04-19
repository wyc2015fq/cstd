# Go语言基础知识（三） 数组Array 切片 Slice - HJ - CSDN博客
2018年11月18日 22:30:37[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：40
个人分类：[====编程语言基础===																[Golang](https://blog.csdn.net/feizaoSYUACM/article/category/8273988)](https://blog.csdn.net/feizaoSYUACM/article/category/7888043)
# 数组
```
package main
import "fmt"
/*
   数组的四种定义方法
     1、 先声明 再循环赋值
     2、 声明的同时赋值 并且指定数组大小
     3、 声明的同时赋值 但用...来表示数组大小
     4、 声明的同时只给某个位置赋值 其他部分值为初始值(即为0)
   注意: 数组的长度大小必须固定，因为数组的长度也是数组比较的一部分，若数组的长度不同，则不能进行传参和比较，否则会报错
 */
func main() {
	var arr1 [5]int
	for i := 0; i < 5; i ++ {
		arr1[i] = i
	}
	PrintHelper("arr1", arr1)
	arr2 := [5]int{1, 2, 3, 4, 5}
	PrintHelper("arr2", arr2)
	arr3 := [...]int{1, 2, 3, 4, 5}
	PrintHelper("arr3", arr3)
	arr4 := [...]int{4:-1}
	PrintHelper("arr4", arr4)
}
func PrintHelper(name string, arr [5]int) {
	fmt.Println(name, " ", arr)
	fmt.Printf("len of %v: %v\n", name, len(arr))
	fmt.Printf("cap of %v: %v\n", name, cap(arr))
	fmt.Println()
}
```
# 切片
```
package main
import (
	"fmt"
)
/*
 切片组成要素：
	指针： 指向底层数组
	长度： 切片中元素的长度，不能大于容量
	容量： 指针所指向的底层数组的总容量
 */
func main() {
	//数组
	arr := [3]int{1, 2, 3}
	fmt.Println(arr)
	//切片
	slice1 := []int{1, 2, 3}
	printHelper("slice1", slice1)
	slice2 := make([]int, 5)
	printHelper("slice2", slice2)
	slice3 := make([]int, 5, 10)
	printHelper("slice3", slice3)
	slice4 := arr[0:3] //左闭右开区间 包含了数组前三个元素
	printHelper("slice4", slice4)
	slice5 := slice4[:] //包含了slice4中的全部元素
	printHelper("slice5", slice5)
	//切片初始化的容量为len的大小， 若容量不够，则每次扩大一倍
	slice6 := []int{1, 2, 3}
	printHelper("slice6", slice6)
	for i := 0; i < 10; i++ {
		slice6 = append(slice6, i)
		printHelper("slice6", slice6)
	}
	//切片的实质是一个指针类型  指向的是一个底层数组 若有多个切片同时引用同一个数组 那么对其中一个切片进行操作 其他切片包括原数组都会被影响
	slice := []int{1, 2, 3}
	newSlice := slice[:]
	fmt.Println(" before modifying:")
	printHelper("slice", slice)
	printHelper("newSlice", newSlice)
	slice[0] = 5
	fmt.Println("after modifying:")
	printHelper("slice", slice)
	printHelper("newSlice", newSlice)
	//如果需要把切片独立出来， 那么可以使用拷贝 通过输出可以看出拷贝过后newSlice2重新指向了一个新的底层数组 而不是 slice
	newSlice2 := make([]int, len(slice))
	copy(newSlice2, slice)
	fmt.Printf("The slice's address about the first value is %d\n", &slice[0])
	fmt.Printf("The newSlice2's address about the first value is %d\n", &newSlice2[0])
}
func printHelper(name string, slice []int) {
	fmt.Println(name, " ", slice)
	fmt.Printf("len of %v: %v\n", name, len(slice))
	fmt.Printf("cap of %v: %v\n", name, cap(slice))
	fmt.Println()
}
```
