# argparse模块 - YZXnuaa的博客 - CSDN博客
2018年01月17日 16:02:51[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：114
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)
**一、简介：**
argparse是python用于解析命令行参数和选项的标准模块，用于代替已经过时的optparse模块。argparse模块的作用是用于解析命令行参数，例如python parseTest.py input.txt output.txt --user=name --port=8080。
**二、使用步骤：**
1：import argparse
2：parser = argparse.ArgumentParser()
3：parser.add_argument()
4：parser.parse_args()
解释：首先导入该模块；然后创建一个解析对象；然后向该对象中添加你要关注的命令行参数和选项，每一个add_argument方法对应一个你要关注的参数或选项；最后调用parse_args()方法进行解析；解析成功之后即可使用，下面简单说明一下步骤2和3。
**三、方法**
**ArgumentParser**(prog=None,usage=None,description=None,epilog=None,parents[],formatter_class=argparse.HelpFormatter, prefix_chars='-', fromfile_prefix_chars=None, argument_default=None,conflict_handler='error', add_help=True)
这些参数都有默认值，当调用parser.print_help()或者运行程序时由于参数不正确(此时python解释器其实也是调用了pring_help()方法)时，会打印这些描述信息，一般只需要传递description参数，如上。
**四、方法add_argument**(name or flags...[, action][, nargs][, const][, default][, type][, choices][, required][, help][, metavar][, dest])
其中：
name or flags：命令行参数名或者选项，如上面的address或者-p,--port.其中命令行参数如果没给定，且没有设置defualt，则出错。但是如果是选项的话，则设置为None
nargs：命令行参数的个数，一般使用通配符表示，其中，'?'表示只用一个，'*'表示0到多个，'+'表示至少一个
default：默认值
type：参数的类型，默认是字符串string类型，还有float、int等类型
help：和ArgumentParser方法中的参数作用相似，出现的场合也一致
```java
import
```
```java
argparse
```
```java
def parse_args():
```
```java
```
```java
description = usage: %prog [options] poetry-file
```
```java
This is the Slow Poetry Server, blocking edition.
```
```java
Run it like
```
```java
this
```
```java
:
```
```java
```
```java
python slowpoetry.py <path-to-poetry-file>
```
```java
If you are in the base directory of the twisted-intro
```
```java
package
```
```java
,
```
```java
you could run it like
```
```java
this
```
```java
:
```
```java
```
```java
python blocking-server/slowpoetry.py poetry/ecstasy.txt
```
```java
to serve up John Donne's Ecstasy, which I know you want to
```
```java
do
```
```java
.
```
```java
```
```java
parser = argparse.ArgumentParser(description = description)
```
```java
```
```java
```
```java
help = The addresses to connect.
```
```java
```
```java
parser.add_argument(
```
```java
'addresses'
```
```java
,nargs =
```
```java
'*'
```
```java
,help = help)
```
```java
```
```java
help = The filename to operate on.Default is poetry/ecstasy.txt
```
```java
```
```java
parser.add_argument(
```
```java
'filename'
```
```java
,help=help)
```
```java
```
```java
help = The port to listen on. Default to a random available port.
```
```java
```
```java
parser.add_argument(
```
```java
'-p'
```
```java
,--port', type=
```
```java
int
```
```java
,
 help=help)
```
```java
```
```java
help = The
```
```java
interface
```
```java
to listen on. Default is localhost.
```
```java
```
```java
parser.add_argument(
```
```java
'--iface'
```
```java
, help=help,
```
```java
default
```
```java
=
```
```java
'localhost'
```
```java
)
```
```java
```
```java
help = The number of seconds between sending bytes.
```
```java
```
```java
parser.add_argument(
```
```java
'--delay'
```
```java
, type=
```
```java
float
```
```java
,
 help=help,
```
```java
default
```
```java
=.
```
```java
7
```
```java
)
```
```java
```
```java
help = The number of bytes to send at a time.
```
```java
```
```java
parser.add_argument(
```
```java
'--bytes'
```
```java
, type=
```
```java
int
```
```java
,
 help=help,
```
```java
default
```
```java
=
```
```java
10
```
```java
)
```
```java
```
```java
args = parser.parse_args();
```
```java
```
```java
return
```
```java
args
```
```java
if
```
```java
__name__ ==
```
```java
'__main__'
```
```java
:
```
```java
```
```java
args = parse_args()
```
```java
```
```java
```
```java
for
```
```java
address in args.addresses:
```
```java
```
```java
print
```
```java
'The address is : %s .'
```
```java
% address
```
```java
```
```java
```
```java
print
```
```java
'The filename is : %s .'
```
```java
% args.filename
```
```java
```
```java
print
```
```java
'The port is : %d.'
```
```java
% args.port
```
```java
```
```java
print
```
```java
'The interface is : %s.'
```
```java
% args.iface
```
```java
```
```java
print
```
```java
'The number of seconds between sending bytes : %f'
```
```java
% args.delay
```
```java
```
```java
print
```
```java
'The number of bytes to send at a time : %d.'
```
```java
% args.bytes</path-to-poetry-file>
```
运行该脚本：python test.py --port 10000 --delay 1.2 127.0.0.1 172.16.55.67 poetry/ecstasy.txt
输出为：
The address is : 127.0.0.1 .
The address is : 172.16.55.67 .
The filename is : poetry/ecstasy.txt .
The port is : 10000.
The interface is : localhost.
The number of seconds between sending bytes : 1.200000
The number of bytes to send at a time : 10.
