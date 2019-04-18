# go---weichart个人对Golang中并发理解 - weixin_33985507的博客 - CSDN博客
2018年01月10日 15:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
```
个人觉得goroutine是Go并行设计的核心，goroutine是协程，但比线程占用更少。golang对并发的处理采用了协程的技术。golang的goroutine就是协程的实现。
十几个goroutine可能体现在底层就是五六个线程，Go语言内部帮你实现了这些goroutine之间的内存共享。执行goroutine只需极少的栈内存(大概是4~5KB)，当然会根据相应的数据伸缩也就是说，当传输数据多的情况下占用量可能会高但是使用过后会自动进行缩小。也正因为如此，可同时运行成千上万个并发任务。goroutine比thread更易用、更高效、更轻便。
不要通过共享内存来通信,而应该通过通信来共享内存。golang解决方案是消息传递机制，消息的传递就是通过channel来实现的。
采用别人的说法:
发送者角度：对于同一个通道，发送操作（协程或者函数中的），在接收者准备好之前是阻塞的。如果chan中的数据无人接收，就无法再给通道传入其他数据。因为新的输入无法在通道非空的情况下传入。所以发送操作会等待 chan 再次变为可用状态：就是通道值被接收时（可以传入变量）。
接收者角度：对于同一个通道，接收操作是阻塞的（协程或函数中的），直到发送者可用：如果通道中没有数据，接收者就阻塞了。
结果就是：如果Channel满了，就阻塞写，如果Channel空了，就阻塞读
```
如果Channel满了，就阻塞写，如果Channel空了，就阻塞读
```
package main
 
 import (
     "fmt"
 )
 
 //此方法是对管道进行读取
 func c(i chan int) {
     fmt.Println(<-i)
 }
 
 func main() {
     out := make(chan int)
     //对管道进行发送操作
     out <- 2
     go c(out)
 }
这里会报死锁的错误
package main
 
 import (
     "fmt"
 )
 
 //此方法是对管道进行读取
 func c(i chan int) {
     fmt.Println(<-i)
 }
 
 func main() {
     out := make(chan int)
     //在发送操作之前进行管道读取操作
     //注意的地方是：作为发送方，在准备读取之前管道是堵塞的。
     go c(out)
     //对管道进行发送操作
     out <- 2
 }
 
本文来自：开源中国博客
感谢作者：iderek718
查看原文：个人对Golang中并发理解
```
