# Go语言入门



#  【一】：安装与环境配置



本篇主要介绍go的生态。包括安装，ide，go命令三部分。随着作者的持续学习，内容可能会有所更新。

## 安装

截止到本文撰写，go的版本更新到了1.7.3.通常来说，安装go只需要到[https://golang.org/dl/](https://link.jianshu.com/?t=https://golang.org/dl/) 官网去下载对应的包安装即可。mac用户也可以通过homebrew，至于ubuntu用户也可以通过apt-get方式，这一点与寻常软件一致。

安装完成后，在控制台输入`go version`，控制台返回类似
`go version go1.7.1 darwin/amd64` 即表示成功。

验证完毕之后，还需要开发者配置几个环境变量：
对于macOS，作者配置在~/.zshrc中（通过官网下载的安装包安装）：

```
export GOROOT=/usr/local/go
export GOPATH=$HOME/goworkspace
export PATH=$PATH:$GOROOT/bin:$GOPATH/bin
```

**GOROOT**：类似于Java中的JAVA_HOME，配置到go的安装目录，取决于安装方式和操作系统的不同，位置也不尽相同。
**GOPATH**：这个变量在其他语言中不常见，相当于是workspace，要存放代码，打包后的模块，可执行文件。配置之后配合后面讲到的go命令十分方便，可以看出go语言是一门工程化至上的语言。这里只要自定义一个目录即可。
PATH中加入这两个路径含义是可以直接在任何地方执行go生成的可执行文件。

至此，安装工作就全部结束了。

## 开发工具

总的来说go的开发工具相对Java较弱，主要有：

- LiteIDE
- Sublime
- Vim
- Eclipse
- IntelliJ IDEA

5种选择，只有用LiteIDE可以比较方便的进行调试，其他工具都需要配置GDB，并且不是专门为go写的ide。而LiteIDE目前的版本还比较粗糙，作者选择的是Sublime。



![img](https://upload-images.jianshu.io/upload_images/1689841-3465ecf71408c390.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/404/format/webp)

通过packageControl添加gosublime

安装GoSublime插件后可以执行代码跳转，代码提示。调试基本靠print，当做锻炼自己的语言熟悉程度了。详细的ide安装步骤建议另行谷歌。

## go命令

go命令是go中十分有特色的一个部分，通过go命令，可以很方便的实现其他语言需要第三方工具来做到的一些功能（如包管理）。
先来个汇总：

```
Go is a tool for managing Go source code.

Usage:

        go command [arguments]

The commands are:

        build       compile packages and dependencies
        clean       remove object files
        doc         show documentation for package or symbol
        env         print Go environment information
        fix         run go tool fix on packages
        fmt         run gofmt on package sources
        generate    generate Go files by processing source
        get         download and install packages and dependencies
        install     compile and install packages and dependencies
        list        list packages
        run         compile and run Go program
        test        test packages
        tool        run specified go tool
        version     print Go version
        vet         run go tool vet on packages
```

这是所有支持的命令。常用解释如下：
**build**: 将一个.go格式的源文件编译成可执行文件或模块，这取决于代码中的package，如果是package main，一般编译为可执行文件并生成到当前的目录。

**install**: build的同时，安装到（即拷贝到）GOPATH/bin目录下，这样在任何地方都可以执行了。

**get**: 相当于Java中的maven, gradle 做的事情了，如代码中使用到了一个第三方包`github.com/go-sql-driver/mysql` , 我们需要在build之前执行 `go get github.com/go-sql-driver/mysql` 。系统将会把代码下载到`GOPATH/src/github.com/go-sql-driver/mysql�`目录结构之下，这个目录结构是一个约定。之后，通过build命令就可以将这个包包含的代码一并编译。从这里可以看出go高度自由的包管理，同时对开源十分友好，工程化便捷没有多余操作。

**fmt**：执行go fmt xxx.go之后，将对目标文件的代码风格统一化，如大括号{}不另起一行，删除多余的空行等。在协作中统一所有人的编码风格是很好的。

**run**： 直接运行一个.go文件，不生成可执行文件。如 `go run test.go`

**doc**：doc命令现在被godoc命令所替代，需要安装godoc：

```
go get -v  golang.org/x/tools/cmd/godoc
```

这里也许需要翻墙，安装好之后，成为一个查看代码文档的简便工具。比如查询fmt.Println函数用法：

```
[~]$ godoc fmt Println
use 'godoc cmd/fmt' for documentation on the fmt command

func Println(a ...interface{}) (n int, err error)
    Println formats using the default formats for its operands and writes to
    standard output. Spaces are always added between operands and a newline
    is appended. It returns the number of bytes written and any write error
    encountered.
```

另外，通过命令在命令行执行 godoc -http=:端口号 比如godoc -http=:8080。然后在浏览器中打开127.0.0.1:8080，你将会看到一个golang.org的本地copy版本，真是欲罢不能。

另外的一些命令：
go version 查看go当前的版本
go env 查看当前go的环境变量
go test 执行这个命令，会自动读取源码目录下面名为*_test.go的文件，生成并运行测试用的可执行文件。
go list 列出当前全部安装的package





# 【二】：为什么要使用Go语言



Go语言是google推出的新兴后端开发语言，我认识的人里，用了都说好。经过前段时间的学习，做个总结�。

## Go语言的主要优势

##### 工程化

- 强制代码风格，通过gofmt工具，方便团队协作。
- 依赖管理的语言级别支持（GOPATH），类似于在Java中使用了maven，Gradle工具。
- 容器技术docker用go编写，go语言在容器应用上有很大的亲和力，譬如通过交叉编译可以只打包可执行程序，极大减小容器的体积。由于这一点很大程度上属于docker的优势，算半条。
- 交叉编译为目标OS的可执行代码十分方便，对制作命令行小工提高效率。
- 开源生态足够丰富，涌现大量的优秀项目和第三方组件。参考：[https://github.com/avelino/awesome-go](https://link.jianshu.com/?t=https://github.com/avelino/awesome-go)

##### 语言本身

- goroutine给并发编程带来了性能和开发速度的极大提升，不再需要复杂的多线程代码。
- 接口的非侵入性。go的接口重于功能而非血统，取消了implements关键字，贯彻了邓小平理论『只要能抓老鼠就是好猫』。
- 强制使用utf-8编码，乱码问题减少
- 关键字少，取消分号，文法简洁。
- go自带的包满足了大部分后端开发的需要，这属于『后发优势』。举例如log自带包就很足够，而Java中不引入log4j则很难灵活日志。又比如发送http请求，只需要使用net/http包就够用，Java中需要引入apache-httpclient。

当前Go语言的一些问题：

- IDE生态不够丰富，目前使用下来体验最好的是IntelliJ IDEA安装golang-plugin配合delve做debug。然而debug体验略有欠缺，比如不能逐行跟踪。参考[https://github.com/go-lang-plugin-org/go-lang-idea-plugin](https://link.jianshu.com/?t=https://github.com/go-lang-plugin-org/go-lang-idea-plugin)
- 版本还在激烈的迭代，未来可能会有重大更新，不够稳定。



# 【三】：通过一段代码学Go语法





### 写在前面

Golang的语法足够简单，但变化很多，刚接触只需要**掌握典型的用法**即可，不需要知道所有的语法糖，所以代码中并不会包含所有可能的语法形式。

并发是Golang的一大主题，后续有专门的一节，并不出现在这一节。

还有一些『约定』：

- 可见性。Java中有private，protect，public。Golang默认采用首字母大小写来判断：首字母大写的变量、方法为public，所有范围内可见；首字母小写的为private，本文件内可见。
- 简洁性。import引入的包以及定义的变量必须被使用，否则程序编译不通过。因此，代码`var p ClassA`不是一个声明定义，而是包含了**初始化**，实际上内存已经被分配了，这一点很容易忽视。

下面上代码：

```go
//一段普通的Go语言代码,用于go语言学习，作者知米无忌于2016-11-29


//声明包，包名和目录的路径一致，以$GOPATH/src为根，如src/math/add.go的包为math。
// 此外，如果为入口（即包含main函数）则包名为main，此时可以被go build编译为可执行文件。
package main

//块状import注意：
//import必须被使用，不能import了但不用他
import (
    "fmt"
    "strconv"
    "reflect"
)

//块状定义变量
var (
    // int 类型包括了rune, int8, int16, int32, int64和byte, uint8, uint16, uint32, uint64。其中rune是int32的别称，byte是uint8的别称。
    myInt int
    //float 包括了float32,float64
    myFloat float32
    myBool bool
    myString string
)

//块状声明常量
const (
    constString = "hello world,世界"
)

// 全局单独定义的变量
var globalDefinedVar interface{}

// 定义一个结构体（也是类，因为golang没有类的概念），面向对象
type person struct {
    name string
    age  int
}

//person 的成员函数,显式的指定p，即Java中的this指针，python中的函数第一个参数self
func (p *person)  introduceMyself() {
    fmt.Println("my name is " + p.name)
    fmt.Println(p.age)
}
func (p *person) shoot() {
    fmt.Println("I can shoot, so I can play football.")
}

//定义一个接口
type footballPlayer interface {
    shoot()
}

func main() {

    ////////////
    // 1.变量定义
    ////////////

    var localString string
    localString = `字符串是
    一对双引号（表示单行）或反引号（表示多行）
    括起来定义`


    ////////////
    // 2.rune, byte数组,string的关系
    ////////////

    myRune := '我'
    fmt.Println(myRune)
    //rune转为string
    myRuneString:= string(myRune)
    fmt.Println(myRuneString)
    //rune转为字节数组
    bytes:=[]byte(myRuneString)
    fmt.Println(bytes)


    // interface{} 类型可赋值任意类型，类似Java的Object类型
    globalDefinedVar = localString

    newVar := ":=是声明并初始化的简写形式，由系统自动推测类型"
    fmt.Println(newVar)

    ////////////
    // 3.数组定义并初始化
    ////////////

    myArray := [3]int{1, 2, 3}
    //数组定义,默认初始化为了5个0
    var myArray2 [5]int
    fmt.Println(myArray2)
    fmt.Println(myArray)

    ////////////
    // 4.slice
    ////////////

    mySlice := []int{1, 2, 3, 4}
    //从数组创建slice，注意1:的使用方式，从第一个元素取到末尾，还可以[1:4]从第一个取到第四个（不包含）。来自于python（作者注）
    mySlice2 := myArray[1:]
    mySlice=append(mySlice,5,6,7)

    ////////////
    // 5.if,else块没什么特殊，myBool未定义默认为false,执行else
    ////////////

    if myBool {
        fmt.Println("if")
    } else {
        fmt.Println("else")
    }

    ////////////
    // 6.for 循环
    ////////////
    for index, value := range mySlice {
        fmt.Printf("index: %d,value:%d \n", index, value)
    }
    for (true) {
        fmt.Println("还可以这样使用，相当于while(true)")
        //避免无限循环，直接break掉
        break
    }

    ////////////
    // 7.map创建，删除和使用，range关键字使用
    ////////////

    myMap := make(map[string]int)
    myMap["a"] = 1
    myMap["b"] = 2
    myMap["c"] = 3
    delete(myMap, "a")
    for key, value := range myMap {
        fmt.Printf("key: %s,value:%d", key, value)
    }

    ////////////
    // 8.面向对象
    ////////////

    // 对象初始化的两种方式，第一种返回的是指针，第二种返回的是对象。
    //注意new永远返回指针
    jack := new(person)
    jack.name = "jack"
    jack.age = 17
    amy := person{"amy", 16}
    jack.introduceMyself()
    //证明是指针
    (*jack).introduceMyself()
    amy.introduceMyself()
    //抛异常 invalid indirect of amy (type person)
    //(*amy).introduceMyself()
    fmt.Println(reflect.TypeOf(jack))
    fmt.Println(reflect.TypeOf(amy))



    ////////////
    // 9.接口使用
    ////////////

    //接口的赋值，可以看出来不需要jack的person类实现该接口就可以实现，只需要拥有接口所需的功能即可。
    var footballPlayer1 = jack
    footballPlayer1.shoot()

    //函数调用，可以多返回值
    myString = "golang is awesome"
    myRet1, myRet2 := receive2Return2(myString, myInt)
    fmt.Println(myRet1, myRet2)



    ////////////
    // 10.异常处理
    ////////////

    //错误处理，调用了一个抛异常的方法
    malfunction()
    //两个知识点：
    //1.recover会收集到由调用栈中的panic抛出的异常信息
    //2.defer的执行时机是执行defer的函数退出的时候，类似于java中的finally，但是不需要把一大段代码用大括号包围，这样代码层次减少了。
    defer func() {
        if err := recover(); err != nil {
            fmt.Println(err)
        }
    }()

    //每个定义过的变量和常量都要被使用，不允许垃圾存在，这里做个打印
    fmt.Println(myBool, myFloat,constString,mySlice2)

}

func receive2Return2(param1 string, param2 int) (ret1 string, ret2 int) {
    fmt.Println("param1:" + param1)
    fmt.Println("param2:" + strconv.Itoa(param2))
    return "returned " + param1, param2 + 1
}

func malfunction() {
    panic("some thing went wrong!")
}
```







# 【四】：源码学习-io



## 介绍

package io主要包含以下文件:

- io.go 基础的io操作，如读写，较底层
- multi.go 多个Reader封装成一个
- pipe.go
- ioutil 常用操作
  - ioutil.go 封装了ioutil
  - tempfile.go 有关创建临时文件

## Reader & Writer

这两个接口是用途最广的接口之一，所以重点学习一下，后续会用在os，net等各种常用包。

```
io.Reader

type Reader interface {
    Read(p []byte) (n int, err error)
}

io.Writer

type Writer interface {
    Write(p []byte) (n int, err error)
}
```

具体例子，以strings.Reader为例，代码：

```
package strings
...

type Reader struct {
    s        string
    i        int64 // current reading index
    prevRune int   // index of previous rune; or < 0
}

func (r *Reader) Read(b []byte) (n int, err error) {
    if r.i >= int64(len(r.s)) {
        return 0, io.EOF
    }
    r.prevRune = -1
    n = copy(b, r.s[r.i:])
    r.i += int64(n)
    return
}
```

可见该类满足io.Reader契约，所以使用时可以这样（清晰起见，以显式类型编码）：

```
var strReader io.Reader = strings.NewReader("hello")
if _, err := io.Copy(os.Stdout, r); err != nil {
    log.Fatal(err)
}
```

从源码来看，
第一，实际上Reader就是封装了序列化数据的一个容器。这一点从命名上来讲可能不是那么明显，从名字上看，可能Reader只是一个调用者，需要从别的地方去『读取』数据，实际上**大部分**Reader本身包括了数据。
第二，Reader类似我们现在的优酷土豆播放器，能够记住上一次播放的位置，每次去观看都从上次看的接着看。

## Copy

这个方法从前面也能看到十分常用
`func Copy(dst Writer, src Reader) (written int64, err error)`
作用是将src的内容写入到dst之中，

> 注意，在buildin中有一个copy方法与之类似，
> func copy(dst, src []Type) int
> 只不过是处理[]byte类型。

查看io.Copy方法的源码，我们发现其中调用了
`func copyBuffer(dst Writer, src Reader, buf []byte) (written int64, err error)`
并默认创建了一个buf，代码如下

```
if buf == nil {
    buf = make([]byte, 32*1024)
}
```

然后，内部调用了src.Read(buf)，并在返回不为EOF的情况下将buf内容逐渐写入dst。加上一些错误处理。

Copy方法还有一个兄弟：io.CopyN:
`func CopyN(dst Writer, src Reader, n int64) (written int64, err error)`
最多拷贝n个字节。
与CopyN相反，还有一个方法：
`func ReadAtLeast(r Reader, buf []byte, min int) (n int, err error)`
该方法至少读取min个字节直到全部读完，否则将抛异常。
这个方法主要用途是保证byte的内容被完整的读取，见ReadFull：

```
func ReadFull(r Reader, buf []byte) (n int, err error) {
    return ReadAtLeast(r, buf, len(buf))
}
```

## io/ioutil

相比之下io中的偏底层实现，ioutil的方法更加实用，常用方法有：

`func ReadAll(r io.Reader) ([]byte, error)`
将reader中的所有内容读取出来，返回字节数组，不需要自己提供字节数组

`func ReadFile(filename string) ([]byte, error)`
将一个文件名传入，返回内容字节数组

`func WriteFile(filename string, data []byte, perm os.FileMode)`error
将data写入文件，如果文件不存在，则以perm的模式创建文件

`func ReadDir(dirname string) ([]os.FileInfo, error)`
返回目录下的所有文件信息（个人认为这个方法出现得有点奇怪，应该在os包更合理）







# 【五】：源码学习-bufio

![96](https://upload.jianshu.io/users/upload_avatars/1689841/c0cd745bbc7d.jpg?imageMogr2/auto-orient/strip|imageView2/1/w/96/h/96)

 

[纳达丶无忌](https://www.jianshu.com/u/957e5f43e612)

 

关注

 0.1 2016.12.01 10:46* 字数 754 阅读 811评论 1喜欢 4

## 介绍

package bufio也是io的一部分，但在不同包中，因此独立一节。
其中包含`bufio.go`,`scan.go`两部分。

## bufio

bufio的作用是为一个已有的Reader或者Writer提供缓冲，我们知道操作系统的io是资源瓶颈，应该尽可能少的调用io操作，所以把大批量的数据一起读取或写入是更好的选择。使用方法：

```
    w := bufio.NewWriter(os.Stdout)
    fmt.Fprint(w, "Hello, ")
    fmt.Fprint(w, "world!")
    w.Flush() // Don't forget to flush!
    // Output: Hello, world!
```

源码中对Reader和Writer做了一个简单封装，bufio.Reader为例：

```
// Reader implements buffering for an io.Reader object.
type Reader struct {
    buf          []byte
    rd           io.Reader // reader provided by the client
    r, w         int       // buf read and write positions
    err          error
    lastByte     int
    lastRuneSize int
}
```

除了包括原始的reader，还有一个[]byte结构，过程以Read方法为例：

```
// Read reads data into p.
// It returns the number of bytes read into p.
// The bytes are taken from at most one Read on the underlying Reader,
// hence n may be less than len(p).
// At EOF, the count will be zero and err will be io.EOF.
func (b *Reader) Read(p []byte) (n int, err error) {
    n = len(p)
    if n == 0 {
        return 0, b.readErr()
    }
    if b.r == b.w {
        if b.err != nil {
            return 0, b.readErr()
        }
        if len(p) >= len(b.buf) {
            // Large read, empty buffer.
            // Read directly into p to avoid copy.
            n, b.err = b.rd.Read(p)
            if n < 0 {
                panic(errNegativeRead)
            }
            if n > 0 {
                b.lastByte = int(p[n-1])
                b.lastRuneSize = -1
            }
            return n, b.readErr()
        }
        b.fill() // buffer is empty
        if b.r == b.w {
            return 0, b.readErr()
        }
    }

    // copy as much as we can
    n = copy(p, b.buf[b.r:b.w])
    b.r += n
    b.lastByte = int(b.buf[b.r-1])
    b.lastRuneSize = -1
    return n, nil
}
```

> **解释**:每次读取只调用内部reader的一次操作，
> 如果内部的buf小于提供的p，那么直接读取到p里，不经过buf。
> 如果buf更大，做一次fill操作：1.清理buf中的遗留数据到buf头部，2.读取内部reader到buf，并向后移动w，w+=n
> 最后做了一次copy操作，将buf的内容copy到p中。

再以Writer.Write为例：

```
// Write writes the contents of p into the buffer.
// It returns the number of bytes written.
// If nn < len(p), it also returns an error explaining
// why the write is short.
func (b *Writer) Write(p []byte) (nn int, err error) {
    for len(p) > b.Available() && b.err == nil {
        var n int
        if b.Buffered() == 0 {
            // Large write, empty buffer.
            // Write directly from p to avoid copy.
            n, b.err = b.wr.Write(p)
        } else {
            n = copy(b.buf[b.n:], p)
            b.n += n
            b.flush()
        }
        nn += n
        p = p[n:]
    }
    if b.err != nil {
        return nn, b.err
    }
    n := copy(b.buf[b.n:], p)
    b.n += n
    nn += n
    return nn, nil
}
```

> **解释**:
> n指buf中已经写了多少字节
> b.Available()指buf中还剩多少字节可写，等于len(buf)-n
> b.Buffered()就是n
> **来看过程：**
> 首先如果n=0，那么直接把p写入到内部writer
> 如果buf中有东西，那么把p的内容先copy到buf中，并做一次flush（即buf写入writer）
> 只要buf中没有足够的空间（小于len(p))，都会持续的写入writer。
> 最后一点点尾巴，只能暂时留在buf里，等待下一次flush操作了。

在使用的场景中来看bufio.Writer的用途：
小buf，大量的写入数据：这样就类似于不加这个buf，只留一点点尾巴。
大buf，小数据写入：这样就有可能不写入，只是把数据先放到buf里。

## Scanner

Scanner的作用是对一个Reader进行迭代，使用方式如下：

```
scanner := bufio.NewScanner(reader)
    for scanner.Scan() {
        fmt.Println(scanner.Text()) // Println will add back the final '\n'
    }
```

默认按照一行一行进行读取，每次scan，scanner.Text()都会返回下一行的数据，直到EOF，Scan()返回false。
我们来看源码，主要是Scanner结构：

```
type Scanner struct {
    r            io.Reader // The reader provided by the client.
    split        SplitFunc // The function to split the tokens.
    maxTokenSize int       // Maximum size of a token; modified by tests.
    token        []byte    // Last token returned by split.
    buf          []byte    // Buffer used as argument to split.
    start        int       // First non-processed byte in buf.
    end          int       // End of data in buf.
    err          error     // Sticky error.
    empties      int       // Count of successive empty tokens.
    scanCalled   bool      // Scan has been called; buffer is in use.
    done         bool      // Scan has finished.
}
```

每次返回的『一行』（其实未必是一行，暂且这么叫）称为token，移动到下一个token称为一次advance，通过split函数做tokenize。其他都是一些比较明显的辅助字段。

这里主要是这个split函数，默认的`bufio.NewScanner()`代码如下：

```
func NewScanner(r io.Reader) *Scanner {
    return &Scanner{
        r:            r,
        split:        ScanLines,
        maxTokenSize: MaxScanTokenSize,
    }
}
```

以分行函数作为split，同时看到MaxScanTokenSize = 64 * 1024,也就是说一行不能太长。否则会抛错，除非使用scanner.Buffer()方法自己提供缓冲区和最大容量。

除了默认的ScanLines，系统还提供了`ScanRunes`,`ScanWords`,`ScanBytes`三个split函数，用户也可以自定义split函数。



# 【六】：源码学习-net/http

![96](https://upload.jianshu.io/users/upload_avatars/1689841/c0cd745bbc7d.jpg?imageMogr2/auto-orient/strip|imageView2/1/w/96/h/96)

 

[纳达丶无忌](https://www.jianshu.com/u/957e5f43e612)

 

关注

 0.2 2016.12.07 16:42* 字数 2358 阅读 1512评论 2喜欢 8

`package net/http`是Go语言的主要应用场景之一**web应用**的基础，从中可以学习到大量前文提到的io,以及没有提到的sync包等一系列基础包的知识，代码量也相对较多，是一个源码学习的宝库。本文主要从一个http server开始，讲解Go是如何实现一个http协议服务器的。

**主要涉及以下源码文件：**
net/net.go
net/server.go
net/http.go
net/transfer.go
sync/pool.go
sync/mutex.go

## 0.引子：从最简单的http server说起

```
func main() {
    http.HandleFunc("/hi", hi)
    http.ListenAndServe(":9999", nil)
    fmt.Printf("hello, world\n")
}

func hi(res http.ResponseWriter, req *http.Request) {
    fmt.Fprintf(res, "hi")
}
```

以上就是最简单的服务器代码，运行后监听本机的9999端口，在浏览器中打开`http://localhost:9999`可以看到返回的hi，接下来就从此入手，开始分析net/http模块。

## 1.Handler： 从路由开始上路

先来分析`http.HandleFunc("/hi", hi)` 这一句，查看源码发现：

```
func HandleFunc(pattern string, handler func(ResponseWriter, *Request)) {
    DefaultServeMux.HandleFunc(pattern, handler)
}
```

首先我们了解到handler的定义是这样的`func(ResponseWriter, *Request)`。这个定义很关键，先提一下。
然后看到了`DefaultServeMux`，这个类是来自于`ServeMux`结构的一个实例，而后者是一个『路由器』的角色，在后面讲到的请求处理过程中，`ServeMux`用来匹配请求的地址，分配适合的handler来完成业务逻辑。
完整的来讲，我们应该先定义一个自己的`ServeMux`，并向他分配路由，像这样：

```
mux := http.NewServeMux()
mux.HandleFunc("/", func(w http.ResponseWriter, req *http.Request) {
    fmt.Fprintf(w, "Welcome to the home page!")
})
http.ListenAndServe(":9999", mux)
```

1.生成一个路由器
2.向路由器注册路由
3.由路由器以及服务地址建立底层连接并提供服务

而之前的简写方式只是省略了建立路由的过程，实际上用了系统自带的DefaultServeMux作为路由器而已。

## 2.向net包匆匆一瞥：一切的基础在net.Conn

接下来看到`http.ListenAndServe(":9999", nil)`这句代码的源码。

```
func ListenAndServe(addr string, handler Handler) error {
    server := &Server{Addr: addr, Handler: handler}
    return server.ListenAndServe()
}
```

首先生成了一个server对象，并调用了它的ListenAndServe方法。Server对象顾名思义，封装了有关提供web服务相关的所有信息，是一个比较重要的类。

```
// A Server defines parameters for running an HTTP server.
// The zero value for Server is a valid configuration.
type Server struct {
    Addr         string        // TCP address to listen on, ":http" if empty
    Handler      Handler       // handler to invoke, http.DefaultServeMux if nil
    ReadTimeout  time.Duration // maximum duration before timing out read of the request
    WriteTimeout time.Duration // maximum duration before timing out write of the response
    TLSConfig    *tls.Config   // optional TLS config, used by ListenAndServeTLS

    MaxHeaderBytes int

    TLSNextProto map[string]func(*Server, *tls.Conn, Handler)

    ConnState func(net.Conn, ConnState)

    ErrorLog *log.Logger

    disableKeepAlives int32     // accessed atomically.
    nextProtoOnce     sync.Once // guards setupHTTP2_* init
    nextProtoErr      error     // result of http2.ConfigureServer if used
}
```

1.handler即路由器（实际上路由器本身作为handler，其中有注册了很多handler），见Handler定义：

```
type Handler interface {
    ServeHTTP(ResponseWriter, *Request)
}
```

和之前注册的函数几乎一样。
2.`ErrorLog`默认以`stdErr`作为输出，也可以提供其他的logger形式。
3.其他的是一些配置以及https,http2的相关支持，暂搁一边。

初始化一个Server必须要的是地址（端口）以及路由，其他都可以按照默认值。生成好Server之后，进入ListenAndServe,源码主要有：

```
ln, err := net.Listen("tcp", addr)
return srv.Serve(tcpKeepAliveListener{ln.(*net.TCPListener)})
```

重要的有两句，首先调用底层的net模块对地址实现监听，返回的ln是一个Listener类型，这个类型有三个方法：

- Accept() (Conn, error)
- Close() error
- Addr() Addr

我们先不碰net模块，只要知道ln可以通过`accept()`返回一个`net.Conn`就够了，获取一个连接的上下文意味着和客户端建立了通道，可以获取数据，并把处理的结果返回给客户端了。接下来`srv.Serve()`方法接受了ln，在这里程序被分为了两层：ln负责连接的底层建立，读写，关闭；Server负责数据的处理。

补充说明一下net.Conn，这个Conn区别于后文要讲的server.conn，是比较底层的，有

- Read(b []byte) (n int, err error)
- Write(b []byte) (n int, err error)

两个方法，也意味着实现了io.Reader, io.Writer接口。

## 3.回到server：建立一个服务器，用goroutine 优雅处理并发

接着前面说，建立好ln之后，用tcpKeepAliveListener类型简单包装，作为参数传给srv.Serve()方法，该方法十分重要，值得放出全部代码：

```
// Serve accepts incoming connections on the Listener l, creating a
// new service goroutine for each. The service goroutines read requests and
// then call srv.Handler to reply to them.
//
// For HTTP/2 support, srv.TLSConfig should be initialized to the
// provided listener's TLS Config before calling Serve. If
// srv.TLSConfig is non-nil and doesn't include the string "h2" in
// Config.NextProtos, HTTP/2 support is not enabled.
//
// Serve always returns a non-nil error.
func (srv *Server) Serve(l net.Listener) error {
    defer l.Close()
    if fn := testHookServerServe; fn != nil {
        fn(srv, l)
    }
    var tempDelay time.Duration // how long to sleep on accept failure

    if err := srv.setupHTTP2_Serve(); err != nil {
        return err
    }

    // TODO: allow changing base context? can't imagine concrete
    // use cases yet.
    baseCtx := context.Background()
    ctx := context.WithValue(baseCtx, ServerContextKey, srv)
    ctx = context.WithValue(ctx, LocalAddrContextKey, l.Addr())
    for {
        rw, e := l.Accept()
        if e != nil {
            if ne, ok := e.(net.Error); ok && ne.Temporary() {
                if tempDelay == 0 {
                    tempDelay = 5 * time.Millisecond
                } else {
                    tempDelay *= 2
                }
                if max := 1 * time.Second; tempDelay > max {
                    tempDelay = max
                }
                srv.logf("http: Accept error: %v; retrying in %v", e, tempDelay)
                time.Sleep(tempDelay)
                continue
            }
            return e
        }
        tempDelay = 0
        c := srv.newConn(rw)
        c.setState(c.rwc, StateNew) // before Serve can return
        go c.serve(ctx)
    }
}
```

分析一下：

##### a) 首先是context这个类型

这个类型比较奇葩，其作用就是一个map，以key,value的形式设置一些背景变量,使用方法是`context.WithValue(parentCtx,key,value)`

##### b) 然后进入一个for无限循环，

l.Accept()阻塞直到获取到一个`net.Conn`，之后通过`srv.newConn(rw)`建立一个`server.conn`(属于私有变量，不对外暴露)，并设置状态为StateNew

##### c) 启动一个goroutine来处理这个连接

调用`go c.serve(ctx)`。从这里可以看出，go语言的并发模型不同于nodejs的单线程回调模型，也不同于Java的多线程方案，采用原生的goroutine来处理既有隔离性，又兼顾了性能。因为这样不会发生nodejs中因为异常处理问题经常让服务器挂掉的现象。同时，goroutine的创建代价远远低于创建线程，当然能在同一台机器比Java服务器达到更大的并发量了。

## 4. 从server到conn：一次请求所有的精华都在conn

前面提到了server.conn，来看一下源码：

```
// A conn represents the server side of an HTTP connection.
type conn struct {
    // server is the server on which the connection arrived.
    // Immutable; never nil.
    server *Server

    // rwc is the underlying network connection.
    // This is never wrapped by other types and is the value given out
    // to CloseNotifier callers. It is usually of type *net.TCPConn or
    // *tls.Conn.
    rwc net.Conn

    // remoteAddr is rwc.RemoteAddr().String(). It is not populated synchronously
    // inside the Listener's Accept goroutine, as some implementations block.
    // It is populated immediately inside the (*conn).serve goroutine.
    // This is the value of a Handler's (*Request).RemoteAddr.
    remoteAddr string

    // tlsState is the TLS connection state when using TLS.
    // nil means not TLS.
    tlsState *tls.ConnectionState

    // werr is set to the first write error to rwc.
    // It is set via checkConnErrorWriter{w}, where bufw writes.
    werr error

    // r is bufr's read source. It's a wrapper around rwc that provides
    // io.LimitedReader-style limiting (while reading request headers)
    // and functionality to support CloseNotifier. See *connReader docs.
    r *connReader

    // bufr reads from r.
    // Users of bufr must hold mu.
    bufr *bufio.Reader

    // bufw writes to checkConnErrorWriter{c}, which populates werr on error.
    bufw *bufio.Writer

    // lastMethod is the method of the most recent request
    // on this connection, if any.
    lastMethod string

    // mu guards hijackedv, use of bufr, (*response).closeNotifyCh.
    mu sync.Mutex

    // hijackedv is whether this connection has been hijacked
    // by a Handler with the Hijacker interface.
    // It is guarded by mu.
    hijackedv bool
}
```

> 解释一下：
> 首先，持有server的引用；持有对原始`net.Conn`引用；持有一个reader，封装自底层读取接口，可以从连接中读取数据，以及一个bufr（还是前面的reader，加了缓冲）。以及一个对应的同步锁，锁定对本身的参数修改，防止同步更新出错。
> 然后，这里的mu类型是`sync.Mutex`这个类型的作用有点像Java中的`synchronized`块（有关于Java的Synchronized，可以参考本人另一篇拙作《Java多线程你只需要看着一篇就够了》），mu就是持有对象锁的那个实例。我们可以看到conn的hijackedv属性就是通过mu来进行维护的，目的是防止同步更新问题。参考`conn.hijackLocked()`，不再展开。

继续看`serv.Serve()`方法，接着前面的3点：

##### d) setState(state)

实际上state被维护在Server里，只不过通过conn来调用了。一共有`StateNew, StateActive, StateIdle, StateHijacked, StateClosed`五个状态。从new开始，当读取了一个字节之后进入active，读取完了并发送response之后，进入idle。终结有两种，主动终结closed以及被接管： Hijack让调用者接管连接,在调用Hijack()后,http server库将不再对该连接进行处理,对于该连接的管理和关闭责任将由调用者接管。参考`interface Hijacker`

##### e) c.serve(ctx)

让我们先来看`conn.serve()`源码：

```
// Serve a new connection.
func (c *conn) serve(ctx context.Context) {
    c.remoteAddr = c.rwc.RemoteAddr().String()
    defer func() {
        if err := recover(); err != nil {
            const size = 64 << 10
            buf := make([]byte, size)
            buf = buf[:runtime.Stack(buf, false)]
            c.server.logf("http: panic serving %v: %v\n%s", c.remoteAddr, err, buf)
        }
        if !c.hijacked() {
            c.close()
            c.setState(c.rwc, StateClosed)
        }
    }()

    if tlsConn, ok := c.rwc.(*tls.Conn); ok {
        if d := c.server.ReadTimeout; d != 0 {
            c.rwc.SetReadDeadline(time.Now().Add(d))
        }
        if d := c.server.WriteTimeout; d != 0 {
            c.rwc.SetWriteDeadline(time.Now().Add(d))
        }
        if err := tlsConn.Handshake(); err != nil {
            c.server.logf("http: TLS handshake error from %s: %v", c.rwc.RemoteAddr(), err)
            return
        }
        c.tlsState = new(tls.ConnectionState)
        *c.tlsState = tlsConn.ConnectionState()
        if proto := c.tlsState.NegotiatedProtocol; validNPN(proto) {
            if fn := c.server.TLSNextProto[proto]; fn != nil {
                h := initNPNRequest{tlsConn, serverHandler{c.server}}
                fn(c.server, tlsConn, h)
            }
            return
        }
    }

    // HTTP/1.x from here on.

    c.r = &connReader{r: c.rwc}
    c.bufr = newBufioReader(c.r)
    c.bufw = newBufioWriterSize(checkConnErrorWriter{c}, 4<<10)

    ctx, cancelCtx := context.WithCancel(ctx)
    defer cancelCtx()

    for {
        w, err := c.readRequest(ctx)
        if c.r.remain != c.server.initialReadLimitSize() {
            // If we read any bytes off the wire, we're active.
            c.setState(c.rwc, StateActive)
        }
        if err != nil {
            if err == errTooLarge {
                // Their HTTP client may or may not be
                // able to read this if we're
                // responding to them and hanging up
                // while they're still writing their
                // request. Undefined behavior.
                io.WriteString(c.rwc, "HTTP/1.1 431 Request Header Fields Too Large\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n431 Request Header Fields Too Large")
                c.closeWriteAndWait()
                return
            }
            if err == io.EOF {
                return // don't reply
            }
            if neterr, ok := err.(net.Error); ok && neterr.Timeout() {
                return // don't reply
            }
            var publicErr string
            if v, ok := err.(badRequestError); ok {
                publicErr = ": " + string(v)
            }
            io.WriteString(c.rwc, "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n400 Bad Request"+publicErr)
            return
        }

        // Expect 100 Continue support
        req := w.req
        if req.expectsContinue() {
            if req.ProtoAtLeast(1, 1) && req.ContentLength != 0 {
                // Wrap the Body reader with one that replies on the connection
                req.Body = &expectContinueReader{readCloser: req.Body, resp: w}
            }
        } else if req.Header.get("Expect") != "" {
            w.sendExpectationFailed()
            return
        }

        // HTTP cannot have multiple simultaneous active requests.[*]
        // Until the server replies to this request, it can't read another,
        // so we might as well run the handler in this goroutine.
        // [*] Not strictly true: HTTP pipelining. We could let them all process
        // in parallel even if their responses need to be serialized.
        serverHandler{c.server}.ServeHTTP(w, w.req)
        w.cancelCtx()
        if c.hijacked() {
            return
        }
        w.finishRequest()
        if !w.shouldReuseConnection() {
            if w.requestBodyLimitHit || w.closedRequestBodyEarly() {
                c.closeWriteAndWait()
            }
            return
        }
        c.setState(c.rwc, StateIdle)
    }
}
```

## 5.从conn到conn.Serve：http协议的处理实现之处，conn变成Request和Response

上文的conn.Serve(),我们只关注主要逻辑：

##### 1.初始化bufr和bufw。

```
...
c.bufr = newBufioReader(c.r)
c.bufw = newBufioWriterSize(checkConnErrorWriter{c}, 4<<10)
...
```

这两个是读写的切入点，从效率考虑，是加了一层缓冲的。值得注意的是bufw和bufr还加了一层sync.Pool的封装，这是来源于sync包的对象池，目的是为了重用，不需要每次都执行new分配内存。

##### 2.接下来重要的是，从底层读取客户端发送的数据：

```
...
w, err := c.readRequest(ctx)
...
```

我们看到readRequest定义：

`func readRequest(b *bufio.Reader, deleteHostHeader bool) (req *Request, err error)`
返回的是 (w *response, err error)，而response又是server.go中的一个重要对象，它是conn的更高一层封装，包括了req，conn，以及一个writer，当然这个write操作实际上还是由conn，进而由更底层的`net.Conn`来执行的。对于开发者而言，面对的基本上就是这个response，可以说是一个设计模式中的门面模式。

另外，注意到readRequest执行的时候也调用了mu.Lock()

##### 3.最重要的，调用用户的handler

```
...
serverHandler{c.server}.ServeHTTP(w, w.req)
```

首先serverHandler只是一个包装，这句实际上调用的是`c.server.Handler.ServeHTTP()`。而在前面讲到的server的初始化中，`Handler`就是`DefaultServeMux`或者用户指定的`ServeMux`，我们称之为路由器。在路由器中，根据用户定义路由规则，来具体调用用户的业务逻辑方法。

路由器可以看做一个Map，以路由规则(string)作为key，以业务方法(func类型)作为value。

ServeHttp传入了最重要的两个高层封装response对象和Request对象（严格来讲这里response是私有类型，暴露在外的是`ResponseWriter`，但从http的本质来理解，还是称之为response）。

从层次来看，这两个封装对象中间封装的是底层的`conn`,客户端发送来的数据（req.body)，以及读写的接口reader,writer。

然后，用户的业务逻辑就接受数据，进行处理，进而返回数据。返回数据一般直接写入到这个w，即`ResponseWriter`中。这样，一个http请求的完整流程就完成了。

##### 4.最后做一些处理工作

主要包括：异常处理，资源回收，状态更新。我们了解即可，重点还是放在主要流程上。







