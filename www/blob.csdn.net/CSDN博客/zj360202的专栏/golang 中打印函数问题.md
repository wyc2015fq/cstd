# golang 中打印函数问题 - zj360202的专栏 - CSDN博客





2013年05月24日 10:13:54[zj360202](https://me.csdn.net/zj360202)阅读数：931








```java
/**
 * Created with IntelliJ IDEA.
 * User: zhangjian
 * Date: 13-5-13
 * Time: 下午1:51
 * To change this template use File | Settings | File Templates.
 */
package main

import "fmt"
import "unsafe"

func main() {
	a_int := []int{1,2,3,4,5}
	ai:=reverse(a_int)
	for aa := range ai{
		//1.这个地方"%d "中%d后面一定要加空格，不然golang会认为是字符串%d，直接输出
		//fmt.Printf这个函数要求()括号里面的必须是字符串，也就是直接fmt.Printf(ai[aa])有语法错误，如果是fmt.Println(ai[aa])是可以的
		fmt.Printf("%d ",ai[aa])
	}

}

func reverse  (a []int) []int {
	for i,j := 0,len(a)-1;i<=j;i,j=i+1,j-1{
		a[i],a[j]=a[j],a[i]
	}
	return a
}
```




