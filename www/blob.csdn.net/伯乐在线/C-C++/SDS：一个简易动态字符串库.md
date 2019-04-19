# SDS：一个简易动态字符串库 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [cjpan](http://www.jobbole.com/members/cjpan) 翻译。未经许可，禁止转载！
英文出处：[SDS](https://github.com/antirez/sds/blob/master/README.md)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
SDS（Simple Dynamic Strings）是一个C语言字符串库，设计中增加了从堆上分配内存的字符串，来扩充有限的libc字符处理的功能，使得：
- 使用更简便
- 二进制安全
- 计算更有效率
- 而且仍旧…兼容一般的C字符串功能
它使用另一种设计来实现，不用C结构体来表现一个字符串，而是使用一个二进制的前缀（prefix），保存在实际的指向字符串的指针之前，SDS将其返回给用户。

C
```
+--------+-------------------------------+-----------+
| Header | Binary safe C alike string... | Null term |
+--------+-------------------------------+-----------+
         |
         Pointer returned to the user.
```
因为元数据作为一个前缀被储存于实际的返回指针之前，还因为不论字符串的实际内容，每个SDS字符串都隐含地在字符串的末尾追加一个空项（null term）。SDS字符串能够和C字符串一起使用，用户能够使用以只读方式访问字符串的函数，自由地交替使用它们。
SDS以前是C字符串的库，是我为自己每天的C编程的需要而开发的，后来它被迁移到Redis，那里它得到了扩展使用，并且为了适应高性能的操作而修改。现在它被从Redis分离出来，成了一个独立的项目。
因为它在Redis里存在了好几年，SDS不仅提供了能够简单操作C字符串的上层函数，还有一系列的底层函数，使得避免因使用上层字符串库造成的损失而写出高效能的代码变为可能。
**SDS的优缺点**
通常动态C字符串库使用一个定义字符串的结构体来实现。此结构体有一个指针域，由字符串函数管理，看起来像这样：

C
```
struct yourAverageStringLibrary {
    char *buf;
    size_t len;
    ... possibly more fields here ...};
```
SDS字符串，已经提过了，不遵从这样的模式，而是对在实际返回字符串地址之前的前缀给予一个单独分配（single allocation）的空间。
相比传统的方式，这种方法也有其优缺点：
**缺点#1：**
很多函数以值的形式返回新字符串，由于有时SDS要求创建一个占用更多空间的新字符串，所以大多数SDS的API调用像这样：

C
```
s = sdscat(s,"Some more data");
```
你可以看到*s*被用来作为*sdscat*的输入，但也被设为SDS API调用返回的值，因为我们不知道此调用是否会改变了我们传递的SDS字符串，还是会重新分配一个新的字符串。忘记将*sdscat*或者类似函数的返回值赋回到存有SDS字符串的变量的话，就会引起bug。
**缺点#2：**
如果一个SDS字符串在你的程序中多个地方共享，当你修改字符串的时候，你必须修改所有的引用。但是，大多数时候，当你需要共享SDS字符串时，将字符串封装成一个结构体，并使用一个引用计数会更好，否则很容易导致内存泄露。
**优点#1：**
你无需访问结构体成员或者调用一个函数就可以把SDS字符串传递给C函数，就像这样：

C
```
printf("%s\n", sds_string);
```
而在大多数其它库中，这将是像这样的：

C
```
printf("%s\n", string->buf);
```
或者：

C
```
printf("%s\n", getStringPointer(string));
```
**优点#2：**
直接访问单个字符。C是个底层的语言，所以在很多程序中这是一个重要的操作。 用SDS字符串来访问单个字符是很轻松的：

C
```
printf("%c %c\n", s[0], s[1]);
```
用其他库的话，你最有可能分配*string->buf*（或者调用函数来取得字符串指针）到一个字符指针，并操作此指针。然而，每次你调用一个函数，可能修改了字符串，其它的库可能隐式地重新分配缓存，你必须再次取得一个内存区的引用。
**优点#3：**
单次分配有更好的缓存局部性。通常当你访问一个由使用结构体的字符串库所创建字符串时，你会有两块不同的内存分配：用结构体来表现字符串的分配，和实际的内存区里储存着字符串。过了一段时间后，内存重新分配，很可能终结在一个与结构体本身地址完全不同的内存部分。因为现代的程序性能经常由缓存未命中次数所决定的，SDS可以在大工作量下表现得更好。
### [](https://github.com/antirez/sds/raw/master/#sds-basics)**SDS基础**
SDS字符串的类型只是字符指针*char **。 然而，SDS在头文件里定义了一个sds类型作为char*的别名：你应该用sds类型，来保证你能记住你程序里的一个变量保存了一个SDS字符串而不是C字符串，当然这不是硬性规定的。
这是你可以写的能做些事情的最简单的SDS程序

C
```
sds mystring = sdsnew("Hello World!");
printf("%s\n", mystring);
sdsfree(mystring);
output> Hello World!
```
上面的小程序已经展示了一些关于SDS的重要内容：
- SDS字符串由*sdsnew()*函数或者其它类似的函数创建、从堆上分配内存，稍后你将看到。
- SDS字符串可以被传递到*printf()*像任何其它的C字符串。
- SDS字符串要求由*sdsfree()*释放，因为它们是堆上分配的。
### **创建SDS字符串**

C
```
sds sdsnewlen(const void *init, size_t initlen);
sds sdsnew(const char *init);
sds sdsempty(void);sds sdsdup(const sds s);
```
创建SDS字符串有很多方法：
- *sdsnew()*函数创建一个以C的空字符结尾的SDS字符串?。我们已经在上述例子中看到它如何工作的。
- *sdsnewlen()*函数类似于*sdsnew()*，但不同于创建一个输入是以空字符结尾的字符串，它取另一个长度的参数。用这个方法，你可以创建一个二进制数据的字符串：

C
```
char buf[3];sds mystring;
buf[0] = 'A';buf[1] = 'B';buf[2] = 'C';mystring = sdsnewlen(buf,3);printf("%s of len %d\n", mystring, (int) sdslen(mystring));
output> ABC of len 3
```
注意：*sdslen*的返回值被转换成*int*，因为它返回一个*size_t*类型。你可以使用正确的*printf*标识符而不是类型转换。
- *sdsempty()*函数创建一个空的零长度的字符串：

C
```
sds mystring = sdsempty();printf("%d\n", (int) sdslen(mystring));
output> 0
```
- *sdsup()*函数复制一个已存在的SDS字符串：

C
```
sds s1, s2;
s1 = sdsnew("Hello");s2 = sdsdup(s1);printf("%s %s\n", s1, s2);
output> Hello Hello
```
### **获得字符串长度**

C
```
size_t sdslen(const sds s);
```
在上述例子中，我们已经用了*sdslen()*函数来获得字符串长度。这个函数的运作方式类似于*libc*中的*strlen*，不同点在于：
- 能以常数时间运行，因为长度被存在SDS字符串的前缀，所以即使是非常大的字符串，调用*sdslen*的花费不昂贵。
- 这个函数是二进制安全的，就像其它的SDS字符串函数一样，所以长度是真正的字符串长度，而不用考虑字符串的内容，字符串中间包含空字符也没有问题。
我们可以运行下面的代码，来看一个SDS字符串二进制安全的例子：

C
```
sds s = sdsnewlen("AB",4);printf("%d\n", (int) sdslen(s));
output> 4
```
注意，SDS字符串在末尾总是以空字符终结，所以哪怕在样例中*s[4]*也会有一个空字符终结，然而用*printf*打印字符串的话，最终只有“A”被打印出来，因为*libc*会把SDS字符串当作一般的C字符串那样处理。
销毁字符串

C
```
void sdsfree(sds s);
```
要销毁一个SDS字符串，只需要调用*sdsfree()*函数，并将字符串指针作为参数。但需要注意的是，由*sdsempty()*创建的空字符串也需要被销毁，否则它们会造成内存泄漏。
如果不是SDS字符串指针，而是NULL指针被传递过来，*sdsfree()*函数将不执行任何操作。因此在调用它之前你不需要显式地检查NULL指针：

C
```
if (string) sdsfree(string); /* Not needed. */sdsfree(string); /* Same effect but simpler. */
```
### **连接字符串**
把字符串和另外的字符连接起来，也许会是你最可能放弃动态C字符串库的操作了。SDS提供了不同的函数来把字符串和已存在的字符串连接起来。

C
```
sds sdscatlen(sds s, const void *t, size_t len);sds sdscat(sds s, const char *t);
```
主要的字符串连接函数是*sdscatlen()*和*sdscat()*，它们基本是一样的，唯一的区别是*sdscat()*没有一个显式的长度参数，因为它要求一个以空字符结尾的字符串。

C
```
sds s = sdsempty();s = sdscat(s, "Hello ");s = sdscat(s, "World!");printf("%s\n", s);
output> Hello World!
```
有时，你需要连接一个SDS字符串到另一个SDS字符串，你不需要指定长度，但同时字符串不需要以空字符结尾，但可以包含任何二进制数据。为此有个特别的函数：

C
```
sds sdscatsds(sds s, const sds t);
```
用法很直接：

C
```
sds s1 = sdsnew("aaa");sds s2 = sdsnew("bbb");s1 = sdscatsds(s1,s2);sdsfree(s2);printf("%s\n", s1);
output> aaabbb
```
有时你不想给字符串添加任何特殊数据，但你想确定整个字符串至少包含了给定数量的字节。

C
```
sds sdsgrowzero(sds s, size_t len);
```
如果现在的字符串长度已经是*len*字节了的话，*sdsgrowzero()*函数不做任何事情；如果不是，它需要用0字节补齐，把字符串增长到*len*。

C
```
sds s = sdsnew("Hello");s = sdsgrowzero(s,6);s[5] = '!'; /* We are sure this is safe because of sdsgrowzero() */printf("%s\n', s);
output> Hello!
```
### **字符串的格式**
有个特殊的字符串连接函数，它接收类似*printf*格式标识符，并且将格式化字符串连接到指定的字符串。

C
```
sds sdscatprintf(sds s, const char *fmt, ...) {
```
样例：

C
```
sds s;int a = 10, b = 20;s = sdsnew("The sum is: ");s = sdscatprintf(s,"%d+%d = %d",a,b,a+b);
```
经常地，你需要直接从*printf*的格式标识符中创建SDS字符串。因为*sdscatprintf()*实际上是一个连接字符串的函数，你需要做的只是将你的字符串连接到一个空字符串：

C
```
char *name = "Anna";int loc = 2500;sds s;s = sdscatprintf(sdsempty(), "%s wrote %d lines of LISP\n", name, loc);
```
你可以用*sdscatprintf()*来把数字转换成SDS字符串：

C
```
int some_integer = 100;sds num = sdscatprintf(sdsempty(),"%d\n", some_integer);
```
但是这很慢，然而我们有个特殊函数来提高效率。
### [](https://github.com/antirez/sds/raw/master/#fast-number-to-string-operations)**数字到字符串快速转换操作**
从一个整数创建一个SDS字符串在特定类型的程序中可能是一个普通的操作，当你能用*sdscatprintf()*来完成的时候，会有很大的性能下降，所以SDS提供了一个专用的函数。

C
```
sds sdsfromlonglong(long long value);
```
用起来像这样:

C
```
sds s = sdsfromlonglong(10000);printf("%d\n", (int) sdslen(s));
output> 5
```
### **裁剪字符串和取得区间**
字符串裁剪是一个通常的操作，一系列字符被从字符串的左边或右边去除。另一个有用的对字符操作是从一个大字符串中只取出一个区间。

C
```
void sdstrim(sds s, const char *cset);void sdsrange(sds s, int start, int end);
```
SDS提供*dstrim()*和*sdsrange()*函数来完成这两个操作。但是，留意，两个函数工作方式都不同于大多数修改SDS字符串的函数，因为它们的返回值为空：基本上那些函数总是破坏性地修改了传递过来的SDS字符串，从来不分配一个新的。因为裁剪和取得区间从不需要更多的空间：所以这两个操作可以只从原来的字符串中去除字符。
因为这个行为，这两个函数速度很快，并且不涉及到内存的重新分配。
这是一个字符串裁剪的例子，里面换行和空格从SDS字符串中被去除了。


```
sds s = sdsnew("         my string\n\n  ");sdstrim(s," \n");printf("-%s-\n",s);
output> -my string-
```
基本上，*sdstrim()*把要裁剪的SDS字符串作为第一个参数，并且带有一个以空字符终结的字符集， 它们会被从字符串的左边或右边去除。字符只要不被裁剪字符列表以外的字符隔开，就会被去除：这是为什么“my”和“string”中间的空格在上面的例子中被保留。
取得区间也类似，但不是取得一组字符，而是在字符串内取得表示开始和结束的索引，由0起始，来取得将被保留的区间。

C
```
sds s = sdsnew("Hello World!");sdsrange(s,1,4);printf("-%s-\n");
output> -ello-
```
索引可以为负，来指定一个起始于字符串末尾的位置，因此-1表示最后一个字符，-2表示倒数第二的，以此类推。

C
```
sds s = sdsnew("Hello World!");sdsrange(s,6,-1);printf("-%s-\n");sdsrange(s,0,-2);printf("-%s-\n");
output> -World!-output> -World-
```
当实现网络服务器处理一个协议或者发送消息时，*sdsrange()*会非常有用。例如，下面的代码用来实现节点间的Redis Cluster消息总线的写处理：

C
```
void clusterWriteHandler(..., int fd, void *privdata, ...) {
    clusterLink *link = (clusterLink*) privdata;
    ssize_t nwritten = write(fd, link->sndbuf, sdslen(link->sndbuf));
    if (nwritten <= 0) {
        /* Error handling... */
    }
    sdsrange(link->sndbuf,nwritten,-1);
    ... more code here ...}
```
每当我们需要发送消息的目标节点的socket是可写的时候，我们尝试写入尽可能多的字节，我们用?*`sdsrange`()*从缓冲中移除已经发送的部分。
给发送到某个集群节点的新消息排队的函数就只是用*sdscatlen()*来把更多的数据放到发送缓冲中去。
注意，Redis Cluster总线实现了一个二进制的协议，因为SDS是二进制安全，所以这不会造成问题。所以SDS的目标不仅是为C程序员提供一个高层字符串API，还提供了易于管理的动态分配缓冲。
### **字符串复制**
最危险、最恶名的C标准库函数可能就是*strcpy*了。所以可能有些有趣的是，在更好设计的动态字符串库的环境中，复制字符串的概念几乎是无关紧要的。通常你做的就是创建一个字符串，内容由你定，或者根据需要连接更多内容。
然而，SDS以一个有利于高效重要的代码段的字符串复制函数为特性。但是我猜它的实用性是有限的，因为这个函数从来没在50千行代码所组成的Redis代码库中被调用。

C
```
sds sdscpylen(sds s, const char *t, size_t len);sds sdscpy(sds s, const char *t);
```
SDS字符串复制函数叫*sdscpylen*，像这样调用：

C
```
s = sdsnew("Hello World!");s = sdscpylen(s,"Hello Superman!",15);
```
正如你能看到的，这个函数接收SDS字符串*s*作为输入，但也返回一个SDS字符串。这对很多修改字符串的SDS函数来说很普遍，用这个方法，返回的SDS字符串可能是基于原来的那个修改的，或者一个新分配的（比如旧的SDS字符串没有足够空间时）。
sdscpylen只是用由指针和长度参数传递的新数据，替换掉在旧SDS字符串里的内容。还有一个类似的函数叫*sdscpy*，不需要长度参数，但是要求带有空字符终结的字符串。
你可能会想，为什么SDS库需要一个字符串复制函数，你可以简单地从零开始创建一个新的SDS字符串，用新的值，而非复制一个存在的SDS字符串的值。理由是效率：*sdsnewlen()*总是分配一个新的字符串，而*sdscplylen()*会尽量重用已存在的字符串，如果有足够的空间，就用用户指定的新内容，只在必要时分配新的。
### **引用字符串（Quoted String）**
为了给程序用户提供的输出，或者为了调试的目的，将一个可能包含二进制数据或者特殊字符的字符串转换成引用的字符串通常是很重要的。这里的引用字符串，意思是程序代码里的字符串文字上的一般形式。然而今天，这个形式也是著名的串行化格式的一部分，如JSON和CSV，所以它显然偏离了在程序的源代码中表现文字上的字符串这一简单的目标。
下面是一个引用字符串文面的例子：

C
```
"\x00Hello World\n"
```
第一个字节是一个0字节，当最后一个字节是一个换行，所以共有两个非字母的字符在这个字符串里。
SDS使用一个连接函数，把表示输入字符串的引用字符串，连接到一个已存在的字符串，来达到这个目的。

C
```
sds sdscatrepr(sds s, const char *p, size_t len);
```
*scscatrepr()*（*repr*表示*representation*）遵从通常的SDS字符串函数规则，接收一个字符指针和一个长度参数，所以你可以用它来处理SDS字符串，或者一般的使用*strlen()*作为*len*参数的C字符串，或者二进制数据。下面是一个使用例子：

C
```
sds s1 = sdsnew("abcd");sds s2 = sdsempty();s[1] = 1;s[2] = 2;s[3] = '\n';s2 = sdscatrepr(s2,s1,sdslen(s1));printf("%s\n", s2);
output> "a\x01\x02\n"
```
这是*sdscatrepr()*使用的规则：
- \和“用backslash引用。
- 能引用特殊字符’\n’, ‘\r’, ‘\t’, ‘\a’以及’\b’
- 所有其他不能通过isprint测试的不可打印字符在\x..格式里被引用，就是backslash后跟x，后跟2位十六进制数字表示字符串的字节数值。
- 这个函数总是加上初始的和最后的双引号字符。
有一个SDS函数能处理逆转换，在下面的语汇单元化（*Tokenization）*的篇幅里有记述*。*
### **语汇单元化（Tokenization）**
语汇单元化是一个把大字符串分割成小字符串的过程。在这个特定的例子中，指定另一个字符串作为分隔符来执行分割。例如，在下面的字符串，有两个子字符串被|-|分割符分割：

C
```
foo|-|bar|-|zap
```
一个更常用的由一个字符组成的分割符是逗号：

C
```
foo,bar,zap
```
处理一行内容来获得组成它的子字符串在许多程序中是很有用的，所以SDS提供了一个函数，给定一个字符串和一个分割符，返回一个SDS字符串的数组。

C
```
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);void sdsfreesplitres(sds *tokens, int count);
```
和往常一样，这个函数可以处理SDS字符串和普通的C字符串。头两个参数*s*和*len*指定了要单元化的字符串，另两个字符串*sep*和*seplen*是在单元化过程中用到的分割符。最后的参数*count*是一个整数指针，会被设为返回的单元（子字符串）的数目。
返回值是一个在堆上分配的SDS字符串数组。

C
```
sds *tokens;int count, j;
sds line = sdsnew("Hello World!");tokens = sdssplitlen(line,sdslen(line)," ",1,&count);
for (j = 0; j < count; j++)
    printf("%s\n", tokens[j]);sdsfreesplitres(tokens,count);
output> Hellooutput> World!
```
返回的数组是在堆上分配的，并且数组的单个元素是普通的SDS字符串。在例子中，你可以通过调用*sdsfreesplitres()*释放所有资源。你也可以选择用free函数自行释放数组，或者像通常那样释放单独的SDS字符串。
合理的方法是用某种方式将你会重用的数组元素设置为NULL，并且用*sdsfreesplitres()*来释放其余所有的数组。
### **面向命令行的单元化**
用分割符分割字符串是很有用的操作，但是对于执行最常见的涉及到重要的字符串操作，即为程序实现命令行接口来说，通常还是不够的。
这是为什么SDS也提供一个额外的函数，允许你将用户由键盘交互式输入，或者通过一个文件、网络或者其他任何方式的参数，分割成单元。

C
```
sds *sdssplitargs(const char *line, int *argc);
```
sdssplitargs函数返回一个SDS字符串数组，就像*sdssplitlen()*一样。释放结构的函数*sdsfreesplitres()*，也是一样的。不同在于执行单元化的方式。
例如，如果输入下面一行：

C
```
call "Sabrina"    and "Mark Smith\n"
```
函数会返回下面的标记（token）：
- “call”
- “Sabrina”
- “and”
- “Mark Smith\n”
基本上，不同的标记要被一个或多个空格分割，每一个标记也可以是一个*sdscatrepr()*可以发出的相同格式的引用字符串。
### **字符串结合（Joining）**
有两个函数做与单元化相反的工作，将字符串结合成一个。

C
```
sds sdsjoin(char **argv, int argc, char *sep, size_t seplen);sds sdsjoinsds(sds *argv, int argc, const char *sep, size_t seplen);
```
这两个函数取一个长度为*argc*的字符串数组，一个分割符及其长度作为输入，产生一个由所有被输入分割符分割的输入字符串所组成的SDS字符串。
*sdsjoin()*和*sdsjoinsds()*不同点在于前者接收C空字符终结的字符串作为输入，而后者要求所有数组里的字符串须为SDS字符串。但是也因为这个原因，只有*sdsjoinsds()*能够处理二进制数据。

C
```
char *tokens[3] = {"foo","bar","zap"};sds s = sdsjoin(tokens,3,"|",1);printf("%s\n", s);
output> foo|bar|zap
```
### [](https://github.com/antirez/sds/raw/master/#error-handling)**错误处理**
所有返回SDS指针的SDS函数，在内存不足的情况下，也有可能返回NULL，基本上这是唯一需要你进行检查的地方。
但是许多现代的C程序处理内存不足时，只会简单地中止程序，所以可能你也会需要通过包装*malloc*，直接调用其他相关的内存分配函数来处理这种情况。
### [](https://github.com/antirez/sds/raw/master/#sds-internals-and-advanced-usage)**SDS本质和进阶用法**
在本篇开始时，解释了SDS字符串是如何被分配的，但是只涉及到保存在返回用户的指针之前的前缀，被当作一个字符串头（header）而已，没有更深入的细节。为了了解进阶的用法，最好挖掘更多SDS的本质，看看实现它所用到的结构体：

C
```
struct sdshdr {
    int len;
    int free;
    char buf[];};
```
如你所见，这个结构体可能与某个传统的字符串库类似，但是结构体的*buf*域是不同的，因为它不是一个指针，而是一个没有声明任何长度的数组，所以*buf*实际上指向了紧跟叫*free*的整数后的第一个字节。所以为了创建一个SDS字符串，我们只要分配一片内存，其大小为*sdshdr*结构体加上我们的字符串长度，外加一个额外的字节，这是为了所有SDS字符串硬性需要的空字符。
结构体的len域显而易见，就是当前的SDS字符串的长度，每当字符串被通过SDS函数调用修改时，总是会被重新计算。而*free*域表示了在当前分配空间中的空闲内存的数量，可以被用来存储更多的字符。
所以实际的SDS内存分布是这个：

C
```
+------------+------------------------+-----------+---------------\
| Len | Free | H E L L O W O R L D \n | Null term |  Free space   \
+------------+------------------------+-----------+---------------\
             |
             Pointer returned to the user.
```
你可能要问，为什么在字符串末尾会有一些空闲空间，这看上去是浪费。实际上，在一个新的SDS字符串创建后，之后是没有任何空闲空间的：分配空间小到只需要保存字符串头、字符串和空终结符。然而，其他的访问模式会在末尾创建一些额外的空闲空间，如下面的程序：

C
```
s = sdsempty();s = sdscat(s,"foo");s = sdscat(s,"bar");s = sdscat(s,"123");
```
因为SDS致力于高效，它负担不起在每次添加新数据时，重新分配字符串，因为这会非常的低效，所以会使用每次你扩大字符串时，**预分配一些空闲空间**。
所使用的预分配算法如下：每次字符串为了保存更多的字节而被重新分配时，实际进行分配的大小是最小需求的两倍。例如，如果字符串现在保存了30个字节，我们多连接2个字节，SDS总共会分配64个字节，而非32个。
然而，可进行分配的空间有一个硬性限制，被定义为*SDS_MAX_PREALLOC*。SDS绝不会分配超过1MB的额外空间（默认的，你可以修改这个默认值）。
### [](https://github.com/antirez/sds/raw/master/#shrinking-strings)**缩减字符串**

C
```
sds sdsRemoveFreeSpace(sds s);size_t sdsAllocSize(sds s);
```
有时，有一类程序要求使用非常少的内存。字符串连接、裁剪、取得区间后，字符串可能最终会有非常巨大的额外空间。
可以用函数*sdsRemoveFreeSpace()*改变字符串大小，回到可以保存现在内容的最小尺寸。

C
```
s = sdsRemoveFreeSpace(s);
```
也可以用另一个函数，来取得给定字符串的总的分配空间大小，叫做*sdsAllocSize()*。

C
```
sds s = sdsnew("Ladies and gentlemen");s = sdscat(s,"... welcome to the C language.");printf("%d\n", (int) sdsAllocSize(s));s = sdsRemoveFreeSpace(s);printf("%d\n", (int) sdsAllocSize(s));
output> 109output> 59
```
注意：SDS底层API使用cammelCase，这可以警告你，你在玩火。
### **手动修改SDS字符串**

C
```
void sdsupdatelen(sds s);
```
有时你会想手动修改一个SDS字符串，而不是用SDS函数。在下面的例子中，我们隐式地修改字符串的长度，当然，我们还想要逻辑上的长度来表示以空字符终结的C字符串。
函数*sdsupdatelen()*正好做了那些工作，把指定字符串的内部长度信息更新成通过*strlen*得到的长度。

C
```
sds s = sdsnew("foobar");s[2] = '';printf("%d\n", sdslen(s));sdsupdatelen(s);printf("%d\n", sdslen(s));
output> 6output> 2
```
### [](https://github.com/antirez/sds/raw/master/#sharing-sds-strings)**共享SDS字符串**
如果你在写一个程序，在其中，不同的数据结构间分享同一个SDS字符串会有好处的话，强烈建议，把SDS字符串封装到一个结构体中，结构体中包含记录引用字符串的数目，还有增减引用数目的函数。
这个方法是一个内存管理技术，叫做引用计数，在SDS的情景下有两个优点：
- 降低了因没有释放SDS字符串或者释放已被释放了的字符串，而造成内存泄露或者bug的可能性。
- 当你修改SDS字符串时，你不需要更新每一个它的引用。（因为新的SDS字符串可以指向不同的内存位置）
尽管这显然已经是一项很常用的编程技术了，我还是把它的基本思路概述一下。像这样，你创建一个结构体：

C
```
struct mySharedStrings {
    int refcount;
    sds string;}
```
当新的字符串被创建出来，分配了这个结构体并且返回refcount设成1。然后你有两个函数修改共享字符串的引用数目：
- *incrementStringRefCount*会简单地将结构体里的为1的refcount增加。每当你在新的数据结构、变量或者随便什么中加了一个字符串的引用，它就会被调用。
- *decrementStringRefCount*被用于删减一个引用。然而这个函数有点特殊，因为当refcount减到0时，它会自动释放SDS字符串，以及mySharedString结构体。
### [](https://github.com/antirez/sds/raw/master/#interactions-with-heap-checkers)**对堆检查工具（Heap Checker）的影响**
因为SDS返回一个指向由malloc分配的内存块的中间，堆检查工具可能会有些问题，但是：
- 常用的Valgrind程序会发现SDS字符串是可能丢失的内存，但并不是确定丢失，所以还是可以容易知道是否有泄露。我用Valgrind和Redis许多年，每一个真的泄露都会被检测为“确定丢失”。
- OSX工具不会把SDS字符串检测为泄露，并能够正确操作指向内存块中间的指针。
### [](https://github.com/antirez/sds/raw/master/#zero-copy-append-from-syscalls)**系统调用的零复制**
此时，通过阅读代码，你应该已经拥有所有挖掘更多SDS库内幕的工具了。然而，还有一个有趣的模式，你可以应用导出的底层API，它在Redis内部使用过，用来改进网络代码性能。
用*sdsIncrLen()*和*sdsMakeRoomFor()*，可以应用下面的模式，来把从内核而来的字节连接到一个sds字符串的末尾，而无需复制到一个中间的内存缓冲区域。

C
```
oldlen = sdslen(s);s = sdsMakeRoomFor(s, BUFFER_SIZE);nread = read(fd, s+oldlen, BUFFER_SIZE);... check for nread <= 0 and handle it ...sdsIncrLen(s, nread);
```
*sdsIncrLen*记述于sds.c的代码中。
### [](https://github.com/antirez/sds/raw/master/#embedding-sds-into-your-project)**在你的项目中嵌入SDS**
这和在你的项目中拷贝sds.c和sds.h文件一样简单。代码很小，每个C99编译器应该都不带任何问题地搞定。
### [](https://github.com/antirez/sds/raw/master/#credits-and-license)**开发人员和许可**
SDS由Salvatore Sanfilippo开发，在BDS的两个条款许可下发布。详情参见软件发布中的LICENSE文件。
