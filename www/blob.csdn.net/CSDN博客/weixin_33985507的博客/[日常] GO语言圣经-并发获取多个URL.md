# [日常] GO语言圣经-并发获取多个URL - weixin_33985507的博客 - CSDN博客
2018年04月04日 16:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
go语言圣经-并发获取多个URL
1.GO最新奇的特性就是对并发编程的支持,goroutine和channel
2.goroutine是一种函数的并发执行方式，而channel是用来在goroutine之间进行参数传递
  go function则表示创建一个新的goroutine，并在这个新的goroutine中执行这个函数。
3.make函数创建了一个传递string类型参数的channel
4.io.Copy把响应的Body内容拷贝到ioutil.Discard输出流中,扔到一个垃圾桶
5.goroutine尝试在一个channel上做send或者receive操作时，这个goroutine会阻塞在调用处,直到另一个goroutine往这个channel里写入、或者接收值
6.用main函数来接收所有fetch函数传回的字符串，可以避免在goroutine异步执行还没有完成时main函数提前退出。
练习 1.10： 找一个数据量比较大的网站，用本小节中的程序调研网站的缓存策略，对每个URL执行两遍请求，查看两次时间是否有较大的差别，并且每次获取到的响应内容是否一致，修改本节中的程序，将响应结果输出，以便于进行对比。
练习 1.11： 在fetchall中尝试使用长一些的参数列表，比如使用在alexa.com的上百万网站里排名靠前的。如果一个网站没有回应，程序将采取怎样的行为？（Section8.9 描述了在这种情况下的应对机制）。
fetchall.go
package main
//导入io/ioutil和net/http包
import (
        "fmt"
        "io"
        "os"
        "io/ioutil"
        "net/http"
        "strings"
        "time"
)
func main() {
        //当前时间
        start := time.Now()
        //make函数创建了一个传递string类型参数的channel
        ch := make(chan string)
        //for循环命令行参数
        for _, url := range os.Args[1:] {
                //开启一个goroutine
                go fetch(url,ch)
        }   
        //接收并打印channel,for循环不需要key value
        for range os.Args[1:]{
                fmt.Printf(<-ch)//注意箭头方向,接收channel
        }   
        //main函数的时间
        fmt.Printf("main: %.2fs \n",time.Since(start).Seconds())
}
//参数类型:string , chan<- string
func fetch(url string,ch chan<- string){
        start := time.Now()
                //加入前缀
                if !strings.HasPrefix(url, "http://") {
                        url = "http://" + url 
                }   
                res, err := http.Get(url)
                //判断错误
                if err != nil {
                        //向标准错误流打印信息
                        fmt.Fprintf(os.Stderr, "fetch:%v \n", err)
                        //终止进程
                        os.Exit(1)
                }   
                //输出http code
                //fmt.Printf("\nhttp status code :%s\n", res.Status)
                //把内容扔掉,只获取字节数
                nBytes , err:=io.Copy(ioutil.Discard, res.Body)
                //判断错误
                if err != nil {
                        //向标准错误流打印信息
                        fmt.Fprintf(os.Stderr, "fetch:%v \n", err)
                        //终止进程
                        os.Exit(1)
                }   
                res.Body.Close()
                //记录执行的秒数
                sec := time.Since(start).Seconds()
                //发送给channel
                ch<- fmt.Sprintf("%.2fs %7d %s \n",sec,nBytes,url)      
}
对比fetch.go
package main
//导入io/ioutil和net/http包
import (
        "fmt"
        "io"
        "os"
        "io/ioutil"
        "net/http"
        "strings"
        "time"
)
func main() {
        //开始时间
        start := time.Now()
        //for循环命令行参数
        for _, url := range os.Args[1:] {
                //加入前缀
                if !strings.HasPrefix(url, "http://") {
                        url = "http://" + url 
                }   
                res, err := http.Get(url)
                //判断错误
                if err != nil {
                        //向标准错误流打印信息
                        fmt.Fprintf(os.Stderr, "fetch:%v \n", err)
                        //终止进程
                        os.Exit(1)
                }   
                //body,err := ioutil.ReadAll(res.Body)
                //避免申请一个缓冲区,直接到标准输出流
                nBytes,err:=io.Copy(ioutil.Discard, res.Body)
                //判断错误
                if err != nil {
                        //向标准错误流打印信息
                        fmt.Fprintf(os.Stderr, "fetch:%v \n", err)
                        //终止进程
                        os.Exit(1)
                }   
                res.Body.Close()
                //输出http code
                fmt.Printf("\nhttp status code :%s %7d %s\n", res.Status,nBytes,url)
                //fmt.Printf("%s",body)
        }   
        //结束时间
        sec:=time.Since(start).Seconds()
        fmt.Printf("main:%.2fs\n",sec)
}
执行效果对比:
![](https://images2018.cnblogs.com/blog/726254/201804/726254-20180404162241485-1984214529.jpg)
