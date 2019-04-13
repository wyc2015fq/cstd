
# tcl channel i-o - Augusdi的专栏 - CSDN博客


2015年08月22日 20:48:19[Augusdi](https://me.csdn.net/Augusdi)阅读数：1382


﻿﻿
Input and Output
输入和输出
open - Open a file-based or command pipeline channel
打开基于文件的或者是命令管道的通道
open fileName ;\#以只读方式打开文件
open fileName access ;\#以指定access方式打开文件，access可以是r,r+,w,w+,a,a+分别表示读写追加，+代表可同时读写，跟w和a在一起时，如果文件不存在就创建。多跟个b，代表读取二进制数。缺省权限是0666；另外access还有第二种标准 POSIX 含义的表达，RDONLY  WRONLY  RDWR  APPEND  BINARY  CREAT EXCL NOCTTY NONBLOCK TRUNC[文件系统权限可参考](http://en.wikipedia.org/wiki/Filesystem_permissions)
open fileName access permissions ;\#可以给新创建的文件赋予权限，0666为缺省值。权限值参考[下表](http://www.comptechdoc.org/os/linux/usersguide/linux_ugfilesp.html)
|1|2|3|4|5|6|7|8|9|10|
|File|User Permissions|Group Permissions|Other Permissions|
|Type|Read|Write|Execute|Read|Write|Execute|Read|Write|Execute|
|d|r|w|e|r|w|e|r|w|e|
二进制文件（[binary file](http://en.wikipedia.org/wiki/Binary_file)）计算机中保存的文件要按照一定格式，譬如gif文件，bmp文件，word文件等等，都叫做formatted text，这些文件是对某些内容的格式化，需要借助其他程序开能读懂其中内容，还有一类文件，只包含内容，不包含格式，叫做plain text，通常plain text文件使用ASCII或者ISO/IEC 646 或者EBCDIC进行编码，现在[Unicode](http://en.wikipedia.org/wiki/Unicode)逐渐替代了老的ASCII编码。
二进制文件怎么读写操作？？？
文件被计算机保存是按照二进制的格式，
譬如11111110就是8个bit的1，如果换算成16进制就是FE,
|下面的代码新建一个二进制文件，并在其中写入十六进制的ff 和 fe
|下面代码将前面写入的二进制文件读出
|
|set fid [open test.txt w+b]
|fconfigure $fid -encoding binary
|puts -nonewline $fid \xfe\xfe
|close $fid
|set fid [open test.txt r+b]
|fconfigure $fid -encoding binary
|set value [gets $fid]
|binary scan $value B8 _value;\#按照从高到低的顺序读入8个bit,转换为2进制字符串;此时_value的值是11111110
|binary scan $value H2 _value;\#按照从高到低的顺序读入2个byte,转换为216进制字符串;此时_value的值是fe
|binary format H2 42;\#H2说明后面42是2个字节的16进制，顺序从高到低;42的10进制是66，对应ASCII是B，所以上面命令返回B
|binary format H4 4242;\#返回BB
|
同样一串二进制代码，编码方式的不同代表不同的含义，上面提到ASCII或者[Unicode](http://en.wikipedia.org/wiki/Unicode)就是编码方式
Unicode又细分为多种方式，
encoding names命令返回Tcl所知道的所有编码方式，
encoding dirs 返回当前编码文件的目录
encoding system 返回当前系统的编码方式
更多编码相关知识参考[http://en.wikipedia.org/wiki/Character_encoding](http://en.wikipedia.org/wiki/Character_encoding)
如果filename是一个串口的名字，那可以用它实现串口通信
set fid [open COM1 w+];
fconfigure $fid -mode 115200,n,8,1 -buffering none -blocking 0 -encoding binary -translation {binary binary} -handshake none;
puts -nonewline $fid "SPEED1[binary format c 0x0A]SPEED2"
set wait 50
while {$wait} {
after 100
catch {append input [read $fid];puts $input}
if {[regexp SPEED $input]} break;
incr wait -1
}
**socket - Open a TCP network connection ;参考Socket编程知识**
**close - Close an open channel**
关闭一个通道，打开的管道都需要关闭的。
chan - Read, write and manipulate channels
fconfigure - Set and get options on a channel
eof - Check for end of file condition on channel
fblocked - Test whether the last input operation exhausted all available input
fcopy - Copy data from one channel to another
file - Manipulate file names and attributes
fileevent - Execute a script when a channel becomes readable or writable
flush - Flush buffered output for a channel
gets - Read a line from a channel
read - Read from a channel
puts - Write to a channel
refchan - Command handler API of reflected channels, version 1
seek - Change the access position for an open channel
tell - Return current access position for an open channel

