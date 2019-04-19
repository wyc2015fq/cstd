# Go语言基础知识（四） Map - HJ - CSDN博客
2018年11月18日 22:32:24[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：55
```
package main
import (
	"fmt"
	"sync"
)
/*
  Map实现的注意点
    1、必须制定key, value的类型，插入的记录类型必须匹配；
	2、key具有唯一性，插入记录的key不能重复；
	3、KeyType可以为基础数据类型（例如bool,数字类型,字符串），不能为数组、切片、map，它的取值必须是能够使用==进行比较；
	4、ValueType可以为任意类型；
	5、无序性，即每次遍历Map输出的顺序都有可能不一样；
	6、线程不安全，一个goroutine在对map进行写的时候，另外的goroutine不能进行读和写操作，Go1.6版本以后会抛出runtime错误信息；
 */
func main() {
	cMap := make(map[string]int)
	cMap["北京"] = 1
	fmt.Println(cMap)
	code := cMap["北京"]
	fmt.Println(code)
	code, ok := cMap["上海"]
	if ok {
		fmt.Println(code)
	} else {
		fmt.Println("上海 is not exitst")
	}
	delete(cMap, "北京")
	fmt.Println(cMap)
	fmt.Println()
	//无序性
	cMap2 := map[string]int{
		"北京": 1,
		"上海": 2,
		"广州": 3,
		"深圳": 4,
		"杭州": 5,
	}
	for k, v := range cMap2 {
		fmt.Printf("%s  is %d\n", k, v)
	}
	fmt.Println()
	//线程不安全
	cMap3 := make(map[string]int)
	var wg sync.WaitGroup
	wg.Add(2)
	go func() {
		cMap3["北京"] = 1
		wg.Done()
	}()
	go func() {
		cMap3["上海"] = 2
		wg.Done()
	}()
	wg.Wait()
	fmt.Println(cMap3)
}
```
