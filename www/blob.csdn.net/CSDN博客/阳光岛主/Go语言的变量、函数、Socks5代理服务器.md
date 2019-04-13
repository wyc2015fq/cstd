
# Go语言的变量、函数、Socks5代理服务器 - 阳光岛主 - CSDN博客

2017年09月28日 11:10:35[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3747个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)



Go语言中变量的声明和JavaScript很像，使用var关键字，变量的声明、定义有好几种形式
**1. 变量和常量**
// 声明并初始化一个变量
**var**m int = 10
// 声明初始化多个变量
**var**i, j, k = 1, 2, 3
// 多个变量的声明(注意小括号的使用)
**var**(
no  int
name string
)
// 声明时不指明类型，通过初始化值来推导
**var**b = true    // bool型
// := 隐含声明变量并赋值
str := "mimvp.com"      // 等价于var str string = "mimvp.com"
**Go 语言中 = 和 := 有什么区别？**
= 是赋值， := 是声明变量并赋值
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|// = 使用必须使用先var声明例如：
|var|a
|a = 100
|// 或
|var|b = 100
|// 或
|var|c int = 100
|// := 是声明并赋值，并且系统自动推断类型，不需要var关键字
|d := 100
|

// Go中有一个特殊的变量下划线"_"表示任何赋给它的值将被丢弃
_, Ret:= 2, 3     // 2赋值被丢弃
Go语言的编译器，对声明却未使用的变量在报错，因此变量声明了就必须使用，如果要使用变量也必须先声明
Go语言和C语言一样，Go语言中也是使用分号来终结语句。但是和C语言不同的是，Go语言的词法分析器在扫描源代码的过程中使用简单的规则自动插入分号，因此在编写源代码的多数时候就不需要加分号了
Go语言词法分析器插入分号的规则：如果在一个新行前方的最后一个标记是一个标识符（包括像int和float64这样的单词）、一个基本的如数值这样的文字、或以下标记中的一个时，会自动插入分号
Go语言通常仅在for语句中使用分号，以此来分开初始化器、添加和增量。还有一种情况就是当你在一行中写了多个语句，也需要使用分号来分开
由于Go语言词法分析器添加分号的特殊性，所以在有些情况下需要注意：
你都不应该将一个控制结构(if、for、switch或select）的左大括号放在下一行。
如果这样做，将会在大括号的前方插入一个分号，这可能导致出现不想要的结果。
常量：在程序不能被改变的值，一般都定义为数值，布尔值，字符串等
格式：const constName [type] = val
1). var num = 3 // 实际上 3 也称为常量
2). 格式中 val 可以是表达式，但不能为运行时才能知道结果的表达式
3). 预定义常量: true / false / iota
4). 定义多个常量时，也可用下面方式
|1
|2
|3
|4
|const (
|constName1 [type] = val1
|constName2 [type] = val2
|)
|

**示例代码：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|/**
|* mimvp.com
|* 2017.1.20
|*/
|// 声明当前文件所属的包名，main是一个可独立运行的包，编译后会生成可执行文件
|package main
|import|"fmt"|// 导入包
|var|id = 123456
|/*
|id2 := 654321
|// 在函数外用 := ，编译时会出现错误，局部变量声明应在函数内部
|// non-declaration statement outside function body
|*/
|const PI = 3.14|// 常量声明
|// 每个可独立运行的程序，都包含入口函数 main ，与其他语言相同，但无参数和返回值
|func main() {
|var|num int
|num = 100
|fmt.Println(num)|// 输出 100
|var|num1, num2 int
|num1, num2 = 1, 2
|fmt.Println(num1, num2)|// 输出 1 2
|var|no1, no2 = 3, 4
|fmt.Println(no1, no2)|// 输出 3 4
|n1, n2 := 5, 6
|fmt.Println(n1, n2)|// 输出 5 6
|_, n := 7, 8
|fmt.Println(n)|// 输出 8
|var|(
|key1 string
|key2 string
|)
|key1, key2 =|"k1"|,|"k2"
|fmt.Println(key1, key2)|// 输出 k1 k2
|var|(
|a = 9
|b = 10
|)
|fmt.Println(a, b)|// 输出 9 10
|fmt.Println(id)|// 输出 123456
|fmt.Println(PI)|// 输出 3.14
|/*
|PI = 3.1415
|// 改变常量的值，编译会出现错误
|// cannot assign to PI
|// cannot use 3.1415 (type float64) as type ideal in assignment
|*/
|}
|

**2. 函数使用**
**1）Go语言函数格式**
|1
|2
|3
|4
|5
|func GetMsg(i int) (str string) {
|fmt.Println(i)
|str =|"hello mimvp.com"
|return|str
|}
|

**解释说明：**
func 说明这是个函数
GetMsg是函数名
(i int) 函数接收一个int参数，是传入参数
(str string) 函数返回一个string类型返回值，是返回参数
**2）Go语言函数可返回多个值**
函数返回多个值，跟Java、PHP、C等主流语言都不一样，但和Python、lua等脚本语言是一样的
|1
|<span style=|"color:\#0000FF;"|>vim mimvp_func.go</span>
|

|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|func GetMsg(i int) (str string, err string) {
|fmt.Println(i)
|str =|"hello mimvp.com"
|err =|"no err"
|return|str, err
|}
|func main() {
|fmt.Println(GetMsg(100))
|}
|

编译执行：
$ go build mimvp_func.go
$ ./mimvp_func
100
hello mimvp.com no err
**3）defer的使用**
defer的意思是"在函数退出时调用"，特别用于对文件进行读写操作的时候，需要在open之后要调用close操作，将close操作使用defer
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|func ReadFile(filePath string)(){
|file.Open(filePath)
|defer file.Close()
|if|true|{
|file.Read()
|}|else|{
|return|false
|}
|}
|

上述代码含义是在file.Open后不立即调用close，当return false的时候调用file.Close()，这样就有效避免了C语言中的内存泄露问题。
**4）理解 panic，recover**
上面讲述了很多变量和函数，还没介绍 throw - try - catch 的用法
Go语言里，Panic和Recover就是其他语言中的throw和catch
示例代码：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|package main
|import|"fmt"
|func main() {
|f()
|fmt.Println(|"Returned normally from f."|)
|}
|func f() {
|defer func() {
|if|r := recover(); r != nil {
|fmt.Println(|"Recovered in f"|, r)
|}
|}()
|fmt.Println(|"Calling g."|)
|g(0)
|fmt.Println(|"Returned normally from g."|)
|}
|func g(i int) {
|if|i > 3 {
|fmt.Println(|"Panicking!"|)
|panic(fmt.Sprintf(|"%v"|, i))
|}
|defer fmt.Println(|"Defer in g"|, i)
|fmt.Println(|"Printing in g"|, i)
|g(i + 1)
|}
|

运行结果：
$ ./mimvp-try-catch
Calling g.
Printing in g 0
Printing in g 1
Printing in g 2
Printing in g 3
Panicking!
Defer in g 3
Defer in g 2
Defer in g 1
Defer in g 0
Recovered in f 4
Returned normally from f.
Panic抛出了信息，并且跳出函数。Recover接受到了信息，并且继续处理。
这个例子理解了基本就掌握了Recover和Panic
**3. Socks5代理服务器**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|80
|81
|82
|83
|84
|85
|86
|87
|88
|89
|90
|91
|92
|93
|94
|95
|96
|97
|98
|99
|100
|101
|102
|103
|104
|105
|106
|107
|108
|109
|110
|111
|112
|113
|114
|115
|116
|117
|118
|119
|120
|121
|122
|123
|124
|125
|126
|127
|128
|129
|130
|131
|132
|133
|134
|135
|136
|137
|138
|139
|140
|141
|142
|143
|144
|145
|146
|147
|148
|149
|150
|151
|152
|153
|154
|155
|156
|157
|158
|159
|160
|161
|162
|163
|164
|165
|166
|167
|168
|169
|170
|171
|172
|173
|174
|175
|176
|177
|178
|179
|180
|181
|182
|183
|184
|185
|186
|187
|188
|189
|package main
|import (
|"net"
|"fmt"
|"io"
|"bytes"
|"encoding/binary"
|)
|type Methods struct{
|ver, nmethods uint8
|methods uint8
|}
|type sock5cmd struct{
|ver, cmd, rsv, atyp uint8
|dst [255]uint8
|}
|type proxyCoder struct {
|conn    net.Conn
|}
|func (c *proxyCoder) readMethods() Methods {
|var|m Methods
|b := make([]byte, 1024)
|n, err := c.conn.Read(b)
|if|err != nil && err != io.EOF { panic(err) }
|buf := bytes.NewBuffer(b[0:n])
|err = binary.Read(buf, binary.LittleEndian, &m.ver)
|if|err != nil {
|fmt.Println(|"binary.Read failed:"|, err)
|}
|err = binary.Read(buf, binary.LittleEndian, &m.nmethods)
|if|err != nil {
|fmt.Println(|"binary.Read failed:"|, err)
|}
|err = binary.Read(buf, binary.BigEndian, &m.methods)
|if|err != nil {
|fmt.Println(|"binary.Read failed:"|, err)
|}
|return|m
|}
|func (c *proxyCoder) returnMethod() {
|buf := make([]byte, 2)
|buf[0] = 5
|buf[1] = 0
|c.conn.Write(buf)
|fmt.Println(buf)
|}
|func (c *proxyCoder) serve() {
|buf := make([]byte, 128)
|n, err := c.conn.Read(buf)
|if|err != nil && err != io.EOF { panic(err) }
|fmt.Println(buf[:n])
|var|s string
|var|t string
|var|i int
|if|(buf[3] == 3){|//domail
|for|i = 4; i < n-2; i++ {
|s += fmt.Sprintf(|"%c"|, buf[i])
|}
|}|else|{|//ip4 or ip6
|s += fmt.Sprintf(|"%d"|, buf[4])
|for|i = 5; i < n-2; i++ {
|s += fmt.Sprintf(|".%d"|, buf[i])
|}
|}
|p := make([]byte, 2)
|var|port uint16
|p[1] = buf[n-1]
|p[0] = buf[n-2]
|b := bytes.NewBuffer(p)
|err = binary.Read(b, binary.BigEndian, &port)
|if|err != nil {
|fmt.Println(|"binary.Read failed:"|, err)
|}
|s += fmt.Sprintf(|":%d"|, port)

|switch|buf[1] {
|case|1:|//TCP
|t =|"tcp"
|case|2:|//BIND
|case|3:|//UDP
|t =|"udp"
|}
|conn, err := net.Dial(t, s)
|if|err != nil {
|fmt.Printf(|"%s connect error %s\n"|, t, s)
|buf[1] = 4
|c.conn.Write(buf[:n])
|c.conn.Close()
|return
|}
|buf[1] = 0
|c.conn.Write(buf[:n])
|fmt.Printf(|"%s connect success %s\n"|, t, s)
|go serv(conn, c.conn)
|go serv(c.conn, conn)
|}
|func serv(|in|net.Conn, out net.Conn){
|b := make([]byte, 10240)
|for|;;{
|n, err :=|in|.Read(b)
|if|( err != nil ){
|fmt.Printf(|"close\n"|)
|in|.Close()
|out.Close()
|return
|}
|fmt.Printf(|"serv %d\n"|, n)
|out.Write(b[:n]);
|}
|}
|type Proxy struct {
|}
|func NewProxy() *Proxy {
|return|&Proxy{}
|}
|var|DefaultProxy = NewProxy()
|func (p *Proxy) ProxyConn(conn net.Conn ){
|c := &proxyCoder{conn}
|m := c.readMethods()
|fmt.Println(m)
|c.returnMethod()
|c.serve()
|}
|func handleConnection(conn net.Conn){
|buf := make([]byte, 1024)
|n, err := conn.Read(buf)
|if|err != nil && err != io.EOF { panic(err) }
|fmt.Println(buf[:n])
|//answer
|buf[0] = 5
|buf[1] = 0
|conn.Write(buf[:2])
|fmt.Println(buf[:2])
|//serve
|n, err = conn.Read(buf)
|if|err != nil && err != io.EOF { panic(err) }
|fmt.Println(buf[:n])
|conn.Close()
|}
|func main() {
|ln, err := net.Listen(|"tcp"|,|":1080"|)
|if|err != nil {
|fmt.Printf(|"bind error\n"|)
|return
|}
|for|{
|conn, err := ln.Accept()
|if|err != nil {
|fmt.Printf(|"accept error\n"|)
|continue
|}
|go DefaultProxy.ProxyConn(conn)
|//go handleConnection(conn)
|}
|}
|




