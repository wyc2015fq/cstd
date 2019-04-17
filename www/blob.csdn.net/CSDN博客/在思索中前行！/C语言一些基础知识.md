# C语言一些基础知识 - 在思索中前行！ - CSDN博客





2014年11月16日 17:10:50[_Tham](https://me.csdn.net/txl16211)阅读数：881








## #include<bits/stdc++.h>

# #include<bits/stdc++.h>包含C++的所有头文件，看见别人用这个函数，然后就能直接用vector,set,string那些函数了，摸不着头脑，感觉特神奇就百度了一下，才发现这个是C++版本升级，然后文件自带的，方便了程序员吧。不然每次都得敲那模板老长……










```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int a,b;
    cin>>a>>b;
    cout<<a+b<<endl;
    return 0;
}
```
#include<bits/stdc++.h>这个头文件包含以下等等C++中包含的所有头文件：
```cpp
#include <iostream> 
#include <cstdio> 
#include <fstream> 
#include <algorithm> 
#include <cmath> 
#include <deque> 
#include <vector> 
#include <queue> 
#include <string> 
#include <cstring> 
#include <map> 
#include <stack> 
#include <set> 
等等……
不过在国内oj中，poj，hdu 不支持这个函数，这几个oj的编译器问题，其他国外的oj，还有台湾的oj都支持，CF，Topcoder也都支持。
```


## 输入输出(printf scanf)

### printf




printf()函数是格式化输出函数, 一般用于向标准输出设备按规定格式输出信息。

printf()函数的调用格式为: printf("<格式化字符串>", <参量表>)。

printf 函数的一般形式为：

int printf(const char *format,[argument]);

格式输出，它是c语言中产生格式化输出的函数（在 stdio.h 中定义）。用于向终端（显示器、控制台等）输出字符。格式控制由要输出的文字和数据格式说明组成。要输出的的文字除了可以使用字母、数字、空格和一些数字符号以外，还可以使用一些[转义字符](http://baike.baidu.com/view/73.htm)表示特殊的含义。


## 用法



```cpp
以下程序用于获得输入值i,j并将其输出（i,j之间以逗号隔开）
#include<stdio.h>
int main()
{
int i,j;
scanf("%d,%d",&i,&j);
printf("i=%d;j=%d",i,j);
return0;
}
```

```cpp
以下程序用于输出“百度百科”
#include<stdio.h>
int main()
{
printf("百度百科");
return0;
}
```

通常意义上format的格式如下：


%[flags][width][.prec][F|N|h|l]type



### Hap环境变量


下列环境变量影响 printf 命令的执行：


LANG 在 LC_ALL 和相应的环境变量（以 LC_ 开头）没有指定语言环境时，确定语言环境编目使用的语言环境。


LC_ALL 确定用于覆盖由 LANG 或其它任何 LC_环境变量设置的任何语言环境编目值的语言环境。


LC_CTYPE 确定把文本字节数据顺序解释为字符的语言环境；例如，单一字节对应多字节字符的参数。


LC_MESSAGES 确定写消息使用的语言。


LC_NUMERIC 确定数字格式编排的语言环境。此环境变量影响使用 e、E、f、g 和 G 转换字符编写的数字的格式。







### Unix命令行程序和内建指令(更多)


|文件系统| | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
||▪[cat](http://baike.baidu.com/view/250626.htm)|▪[cd](http://baike.baidu.com/view/22623.htm)|▪[chmod](http://baike.baidu.com/view/1229012.htm)|▪[chown](http://baike.baidu.com/view/1229019.htm)||----|----|----|----||▪[chgrp](http://baike.baidu.com/view/1188586.htm)|▪[cksum](http://baike.baidu.com/view/2761036.htm)|▪[cmp](http://baike.baidu.com/view/631858.htm)|▪[cp](http://baike.baidu.com/view/25561.htm)||▪[du](http://baike.baidu.com/view/43913.htm)|▪[df](http://baike.baidu.com/view/111942.htm)|▪[fsck](http://baike.baidu.com/view/1757895.htm)|▪[fuser](http://baike.baidu.com/view/3123051.htm)||▪[ln](http://baike.baidu.com/view/1156839.htm)|▪[ls](http://baike.baidu.com/view/370184.htm)|▪[lsattr](http://baike.baidu.com/view/4811972.htm)|▪[lsof](http://baike.baidu.com/searchword/?word=lsof&pic=1&sug=1&enc=utf8)||▪[mkdir](http://baike.baidu.com/view/1279338.htm)|▪[mount](http://baike.baidu.com/view/1374830.htm)|▪[mv](http://baike.baidu.com/view/17861.htm)|▪[pwd](http://baike.baidu.com/view/920712.htm)||▪[rm](http://baike.baidu.com/view/6779.htm)|▪[rmdir](http://baike.baidu.com/view/1081230.htm)|▪[split](http://baike.baidu.com/view/356808.htm)|▪[touch](http://baike.baidu.com/view/345328.htm)||▪[umask](http://baike.baidu.com/view/1867757.htm)|||||| | | ||▪[cat](http://baike.baidu.com/view/250626.htm)|▪[cd](http://baike.baidu.com/view/22623.htm)|▪[chmod](http://baike.baidu.com/view/1229012.htm)|▪[chown](http://baike.baidu.com/view/1229019.htm)|▪[chgrp](http://baike.baidu.com/view/1188586.htm)|▪[cksum](http://baike.baidu.com/view/2761036.htm)|▪[cmp](http://baike.baidu.com/view/631858.htm)|▪[cp](http://baike.baidu.com/view/25561.htm)|▪[du](http://baike.baidu.com/view/43913.htm)|▪[df](http://baike.baidu.com/view/111942.htm)|▪[fsck](http://baike.baidu.com/view/1757895.htm)|▪[fuser](http://baike.baidu.com/view/3123051.htm)|▪[ln](http://baike.baidu.com/view/1156839.htm)|▪[ls](http://baike.baidu.com/view/370184.htm)|▪[lsattr](http://baike.baidu.com/view/4811972.htm)|▪[lsof](http://baike.baidu.com/searchword/?word=lsof&pic=1&sug=1&enc=utf8)|▪[mkdir](http://baike.baidu.com/view/1279338.htm)|▪[mount](http://baike.baidu.com/view/1374830.htm)|▪[mv](http://baike.baidu.com/view/17861.htm)|▪[pwd](http://baike.baidu.com/view/920712.htm)|▪[rm](http://baike.baidu.com/view/6779.htm)|▪[rmdir](http://baike.baidu.com/view/1081230.htm)|▪[split](http://baike.baidu.com/view/356808.htm)|▪[touch](http://baike.baidu.com/view/345328.htm)|▪[umask](http://baike.baidu.com/view/1867757.htm)|||||
|▪[cat](http://baike.baidu.com/view/250626.htm)|▪[cd](http://baike.baidu.com/view/22623.htm)|▪[chmod](http://baike.baidu.com/view/1229012.htm)|▪[chown](http://baike.baidu.com/view/1229019.htm)| | | | | | | | | | | | | | | | | | | | | | | | | | |
|▪[chgrp](http://baike.baidu.com/view/1188586.htm)|▪[cksum](http://baike.baidu.com/view/2761036.htm)|▪[cmp](http://baike.baidu.com/view/631858.htm)|▪[cp](http://baike.baidu.com/view/25561.htm)| | | | | | | | | | | | | | | | | | | | | | | | | | |
|▪[du](http://baike.baidu.com/view/43913.htm)|▪[df](http://baike.baidu.com/view/111942.htm)|▪[fsck](http://baike.baidu.com/view/1757895.htm)|▪[fuser](http://baike.baidu.com/view/3123051.htm)| | | | | | | | | | | | | | | | | | | | | | | | | | |
|▪[ln](http://baike.baidu.com/view/1156839.htm)|▪[ls](http://baike.baidu.com/view/370184.htm)|▪[lsattr](http://baike.baidu.com/view/4811972.htm)|▪[lsof](http://baike.baidu.com/searchword/?word=lsof&pic=1&sug=1&enc=utf8)| | | | | | | | | | | | | | | | | | | | | | | | | | |
|▪[mkdir](http://baike.baidu.com/view/1279338.htm)|▪[mount](http://baike.baidu.com/view/1374830.htm)|▪[mv](http://baike.baidu.com/view/17861.htm)|▪[pwd](http://baike.baidu.com/view/920712.htm)| | | | | | | | | | | | | | | | | | | | | | | | | | |
|▪[rm](http://baike.baidu.com/view/6779.htm)|▪[rmdir](http://baike.baidu.com/view/1081230.htm)|▪[split](http://baike.baidu.com/view/356808.htm)|▪[touch](http://baike.baidu.com/view/345328.htm)| | | | | | | | | | | | | | | | | | | | | | | | | | |
|▪[umask](http://baike.baidu.com/view/1867757.htm)|||| | | | | | | | | | | | | | | | | | | | | | | | | | |
|| | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|程序| | | | | | | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
||▪[at](http://baike.baidu.com/view/27835.htm)|▪[bg](http://baike.baidu.com/view/194376.htm)|▪[chroot](http://baike.baidu.com/view/2977396.htm)|▪[cron](http://baike.baidu.com/view/660015.htm)||----|----|----|----||▪[exit](http://baike.baidu.com/view/619980.htm)|▪[fg](http://baike.baidu.com/view/228000.htm)|▪[jobs](http://baike.baidu.com/view/1374855.htm)|▪[kill](http://baike.baidu.com/view/22085.htm)||▪[killall](http://baike.baidu.com/searchword/?word=killall&pic=1&sug=1&enc=utf8)|▪[nice](http://baike.baidu.com/view/620110.htm)|▪[pgrep](http://baike.baidu.com/searchword/?word=pgrep&pic=1&sug=1&enc=utf8)|▪[pidof](http://baike.baidu.com/searchword/?word=pidof&pic=1&sug=1&enc=utf8)||▪[pkill](http://baike.baidu.com/view/6624478.htm)|▪[ps](http://baike.baidu.com/view/7556.htm)|▪[pstree](http://baike.baidu.com/view/1229127.htm)|▪[sleep](http://baike.baidu.com/view/66232.htm)||▪[time](http://baike.baidu.com/view/10368.htm)|▪[top](http://baike.baidu.com/view/184937.htm)|▪[wait](http://baike.baidu.com/view/11068.htm)|||| | | ||▪[at](http://baike.baidu.com/view/27835.htm)|▪[bg](http://baike.baidu.com/view/194376.htm)|▪[chroot](http://baike.baidu.com/view/2977396.htm)|▪[cron](http://baike.baidu.com/view/660015.htm)|▪[exit](http://baike.baidu.com/view/619980.htm)|▪[fg](http://baike.baidu.com/view/228000.htm)|▪[jobs](http://baike.baidu.com/view/1374855.htm)|▪[kill](http://baike.baidu.com/view/22085.htm)|▪[killall](http://baike.baidu.com/searchword/?word=killall&pic=1&sug=1&enc=utf8)|▪[nice](http://baike.baidu.com/view/620110.htm)|▪[pgrep](http://baike.baidu.com/searchword/?word=pgrep&pic=1&sug=1&enc=utf8)|▪[pidof](http://baike.baidu.com/searchword/?word=pidof&pic=1&sug=1&enc=utf8)|▪[pkill](http://baike.baidu.com/view/6624478.htm)|▪[ps](http://baike.baidu.com/view/7556.htm)|▪[pstree](http://baike.baidu.com/view/1229127.htm)|▪[sleep](http://baike.baidu.com/view/66232.htm)|▪[time](http://baike.baidu.com/view/10368.htm)|▪[top](http://baike.baidu.com/view/184937.htm)|▪[wait](http://baike.baidu.com/view/11068.htm)|||
|▪[at](http://baike.baidu.com/view/27835.htm)|▪[bg](http://baike.baidu.com/view/194376.htm)|▪[chroot](http://baike.baidu.com/view/2977396.htm)|▪[cron](http://baike.baidu.com/view/660015.htm)| | | | | | | | | | | | | | | | | | |
|▪[exit](http://baike.baidu.com/view/619980.htm)|▪[fg](http://baike.baidu.com/view/228000.htm)|▪[jobs](http://baike.baidu.com/view/1374855.htm)|▪[kill](http://baike.baidu.com/view/22085.htm)| | | | | | | | | | | | | | | | | | |
|▪[killall](http://baike.baidu.com/searchword/?word=killall&pic=1&sug=1&enc=utf8)|▪[nice](http://baike.baidu.com/view/620110.htm)|▪[pgrep](http://baike.baidu.com/searchword/?word=pgrep&pic=1&sug=1&enc=utf8)|▪[pidof](http://baike.baidu.com/searchword/?word=pidof&pic=1&sug=1&enc=utf8)| | | | | | | | | | | | | | | | | | |
|▪[pkill](http://baike.baidu.com/view/6624478.htm)|▪[ps](http://baike.baidu.com/view/7556.htm)|▪[pstree](http://baike.baidu.com/view/1229127.htm)|▪[sleep](http://baike.baidu.com/view/66232.htm)| | | | | | | | | | | | | | | | | | |
|▪[time](http://baike.baidu.com/view/10368.htm)|▪[top](http://baike.baidu.com/view/184937.htm)|▪[wait](http://baike.baidu.com/view/11068.htm)|| | | | | | | | | | | | | | | | | | |
|| | | | | | | | | | | | | | | | | | | | | |
|使用环境| | | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
||▪[env](http://baike.baidu.com/view/6397758.htm)|▪[finger](http://baike.baidu.com/view/137128.htm)|▪[id](http://baike.baidu.com/view/371.htm)|▪[logname](http://baike.baidu.com/searchword/?word=logname&pic=1&sug=1&enc=utf8)||----|----|----|----||▪[mesg](http://baike.baidu.com/view/1229115.htm)|▪[passwd](http://baike.baidu.com/view/1116984.htm)|▪[su](http://baike.baidu.com/view/843034.htm)|▪[sudo](http://baike.baidu.com/view/1138183.htm)||▪[uptime](http://baike.baidu.com/view/1228993.htm)|▪[w](http://baike.baidu.com/view/59877.htm)|▪[wall](http://baike.baidu.com/view/126810.htm)|▪[who](http://baike.baidu.com/view/52104.htm)||▪[whoami](http://baike.baidu.com/view/1116259.htm)|▪[write](http://baike.baidu.com/view/1081203.htm)||||| | | ||▪[env](http://baike.baidu.com/view/6397758.htm)|▪[finger](http://baike.baidu.com/view/137128.htm)|▪[id](http://baike.baidu.com/view/371.htm)|▪[logname](http://baike.baidu.com/searchword/?word=logname&pic=1&sug=1&enc=utf8)|▪[mesg](http://baike.baidu.com/view/1229115.htm)|▪[passwd](http://baike.baidu.com/view/1116984.htm)|▪[su](http://baike.baidu.com/view/843034.htm)|▪[sudo](http://baike.baidu.com/view/1138183.htm)|▪[uptime](http://baike.baidu.com/view/1228993.htm)|▪[w](http://baike.baidu.com/view/59877.htm)|▪[wall](http://baike.baidu.com/view/126810.htm)|▪[who](http://baike.baidu.com/view/52104.htm)|▪[whoami](http://baike.baidu.com/view/1116259.htm)|▪[write](http://baike.baidu.com/view/1081203.htm)||||
|▪[env](http://baike.baidu.com/view/6397758.htm)|▪[finger](http://baike.baidu.com/view/137128.htm)|▪[id](http://baike.baidu.com/view/371.htm)|▪[logname](http://baike.baidu.com/searchword/?word=logname&pic=1&sug=1&enc=utf8)| | | | | | | | | | | | | | |
|▪[mesg](http://baike.baidu.com/view/1229115.htm)|▪[passwd](http://baike.baidu.com/view/1116984.htm)|▪[su](http://baike.baidu.com/view/843034.htm)|▪[sudo](http://baike.baidu.com/view/1138183.htm)| | | | | | | | | | | | | | |
|▪[uptime](http://baike.baidu.com/view/1228993.htm)|▪[w](http://baike.baidu.com/view/59877.htm)|▪[wall](http://baike.baidu.com/view/126810.htm)|▪[who](http://baike.baidu.com/view/52104.htm)| | | | | | | | | | | | | | |
|▪[whoami](http://baike.baidu.com/view/1116259.htm)|▪[write](http://baike.baidu.com/view/1081203.htm)||| | | | | | | | | | | | | | |
|| | | | | | | | | | | | | | | | | |
|文字编辑| | | | | | | | | | | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
||▪[awk](http://baike.baidu.com/view/209681.htm)|▪[comm](http://baike.baidu.com/view/1374860.htm)|▪[cut](http://baike.baidu.com/view/443540.htm)|▪[ed](http://baike.baidu.com/view/16488.htm)||----|----|----|----||▪[ex](http://baike.baidu.com/view/345491.htm)|▪[fmt](http://baike.baidu.com/view/2131324.htm)|▪[head](http://baike.baidu.com/view/463665.htm)|▪[iconv](http://baike.baidu.com/view/1437030.htm)||▪[join](http://baike.baidu.com/view/598388.htm)|▪[less](http://baike.baidu.com/view/1229034.htm)|▪[more](http://baike.baidu.com/view/29762.htm)|▪[paste](http://baike.baidu.com/view/1374816.htm)||▪[sed](http://baike.baidu.com/view/432091.htm)|▪[sort](http://baike.baidu.com/view/1374853.htm)|▪[strings](http://baike.baidu.com/view/7923.htm)|▪[talk](http://baike.baidu.com/view/112883.htm)||▪[tac](http://baike.baidu.com/view/754049.htm)|▪[tail](http://baike.baidu.com/view/1053222.htm)|▪[tr](http://baike.baidu.com/view/316588.htm)|▪[uniq](http://baike.baidu.com/view/1374849.htm)||▪[vi](http://baike.baidu.com/view/27682.htm)|▪[wc](http://baike.baidu.com/view/24408.htm)|▪[xargs](http://baike.baidu.com/view/2285387.htm)|||| | | ||▪[awk](http://baike.baidu.com/view/209681.htm)|▪[comm](http://baike.baidu.com/view/1374860.htm)|▪[cut](http://baike.baidu.com/view/443540.htm)|▪[ed](http://baike.baidu.com/view/16488.htm)|▪[ex](http://baike.baidu.com/view/345491.htm)|▪[fmt](http://baike.baidu.com/view/2131324.htm)|▪[head](http://baike.baidu.com/view/463665.htm)|▪[iconv](http://baike.baidu.com/view/1437030.htm)|▪[join](http://baike.baidu.com/view/598388.htm)|▪[less](http://baike.baidu.com/view/1229034.htm)|▪[more](http://baike.baidu.com/view/29762.htm)|▪[paste](http://baike.baidu.com/view/1374816.htm)|▪[sed](http://baike.baidu.com/view/432091.htm)|▪[sort](http://baike.baidu.com/view/1374853.htm)|▪[strings](http://baike.baidu.com/view/7923.htm)|▪[talk](http://baike.baidu.com/view/112883.htm)|▪[tac](http://baike.baidu.com/view/754049.htm)|▪[tail](http://baike.baidu.com/view/1053222.htm)|▪[tr](http://baike.baidu.com/view/316588.htm)|▪[uniq](http://baike.baidu.com/view/1374849.htm)|▪[vi](http://baike.baidu.com/view/27682.htm)|▪[wc](http://baike.baidu.com/view/24408.htm)|▪[xargs](http://baike.baidu.com/view/2285387.htm)|||
|▪[awk](http://baike.baidu.com/view/209681.htm)|▪[comm](http://baike.baidu.com/view/1374860.htm)|▪[cut](http://baike.baidu.com/view/443540.htm)|▪[ed](http://baike.baidu.com/view/16488.htm)| | | | | | | | | | | | | | | | | | | | | | |
|▪[ex](http://baike.baidu.com/view/345491.htm)|▪[fmt](http://baike.baidu.com/view/2131324.htm)|▪[head](http://baike.baidu.com/view/463665.htm)|▪[iconv](http://baike.baidu.com/view/1437030.htm)| | | | | | | | | | | | | | | | | | | | | | |
|▪[join](http://baike.baidu.com/view/598388.htm)|▪[less](http://baike.baidu.com/view/1229034.htm)|▪[more](http://baike.baidu.com/view/29762.htm)|▪[paste](http://baike.baidu.com/view/1374816.htm)| | | | | | | | | | | | | | | | | | | | | | |
|▪[sed](http://baike.baidu.com/view/432091.htm)|▪[sort](http://baike.baidu.com/view/1374853.htm)|▪[strings](http://baike.baidu.com/view/7923.htm)|▪[talk](http://baike.baidu.com/view/112883.htm)| | | | | | | | | | | | | | | | | | | | | | |
|▪[tac](http://baike.baidu.com/view/754049.htm)|▪[tail](http://baike.baidu.com/view/1053222.htm)|▪[tr](http://baike.baidu.com/view/316588.htm)|▪[uniq](http://baike.baidu.com/view/1374849.htm)| | | | | | | | | | | | | | | | | | | | | | |
|▪[vi](http://baike.baidu.com/view/27682.htm)|▪[wc](http://baike.baidu.com/view/24408.htm)|▪[xargs](http://baike.baidu.com/view/2285387.htm)|| | | | | | | | | | | | | | | | | | | | | | |
|| | | | | | | | | | | | | | | | | | | | | | | | | |
|Shell 程序| | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
||▪[alias](http://baike.baidu.com/view/49616.htm)|▪[basename](http://baike.baidu.com/view/2978587.htm)|▪[dirname](http://baike.baidu.com/view/2978588.htm)|▪[echo](http://baike.baidu.com/view/32757.htm)||----|----|----|----||▪[expr](http://baike.baidu.com/view/1229144.htm)|▪[false](http://baike.baidu.com/view/2152638.htm)|▪**printf**|▪[test](http://baike.baidu.com/view/1234.htm)||▪[true](http://baike.baidu.com/view/743997.htm)|▪[unset](http://baike.baidu.com/view/4879915.htm)||||| | | ||▪[alias](http://baike.baidu.com/view/49616.htm)|▪[basename](http://baike.baidu.com/view/2978587.htm)|▪[dirname](http://baike.baidu.com/view/2978588.htm)|▪[echo](http://baike.baidu.com/view/32757.htm)|▪[expr](http://baike.baidu.com/view/1229144.htm)|▪[false](http://baike.baidu.com/view/2152638.htm)|▪**printf**|▪[test](http://baike.baidu.com/view/1234.htm)|▪[true](http://baike.baidu.com/view/743997.htm)|▪[unset](http://baike.baidu.com/view/4879915.htm)||||
|▪[alias](http://baike.baidu.com/view/49616.htm)|▪[basename](http://baike.baidu.com/view/2978587.htm)|▪[dirname](http://baike.baidu.com/view/2978588.htm)|▪[echo](http://baike.baidu.com/view/32757.htm)| | | | | | | | | | |
|▪[expr](http://baike.baidu.com/view/1229144.htm)|▪[false](http://baike.baidu.com/view/2152638.htm)|▪**printf**|▪[test](http://baike.baidu.com/view/1234.htm)| | | | | | | | | | |
|▪[true](http://baike.baidu.com/view/743997.htm)|▪[unset](http://baike.baidu.com/view/4879915.htm)||| | | | | | | | | | |
|| | | | | | | | | | | | | |
|网络| | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|
||▪[inetd](http://baike.baidu.com/view/500836.htm)|▪[netstat](http://baike.baidu.com/view/28008.htm)|▪[ping](http://baike.baidu.com/view/709.htm)|▪[rlogin](http://baike.baidu.com/view/611069.htm)||----|----|----|----||▪[netcat](http://baike.baidu.com/view/560266.htm)|▪[traceroute](http://baike.baidu.com/view/70478.htm)||||| | | ||▪[inetd](http://baike.baidu.com/view/500836.htm)|▪[netstat](http://baike.baidu.com/view/28008.htm)|▪[ping](http://baike.baidu.com/view/709.htm)|▪[rlogin](http://baike.baidu.com/view/611069.htm)|▪[netcat](http://baike.baidu.com/view/560266.htm)|▪[traceroute](http://baike.baidu.com/view/70478.htm)||||
|▪[inetd](http://baike.baidu.com/view/500836.htm)|▪[netstat](http://baike.baidu.com/view/28008.htm)|▪[ping](http://baike.baidu.com/view/709.htm)|▪[rlogin](http://baike.baidu.com/view/611069.htm)| | | | | | |
|▪[netcat](http://baike.baidu.com/view/560266.htm)|▪[traceroute](http://baike.baidu.com/view/70478.htm)||| | | | | | |
|| | | | | | | | | |
|搜索| | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|
||▪[find](http://baike.baidu.com/view/801452.htm)|▪[grep](http://baike.baidu.com/view/1057278.htm)|▪[locate](http://baike.baidu.com/view/1229038.htm)|▪[whereis](http://baike.baidu.com/view/1793607.htm)||----|----|----|----||▪[which](http://baike.baidu.com/view/1793602.htm)|||||| | | ||▪[find](http://baike.baidu.com/view/801452.htm)|▪[grep](http://baike.baidu.com/view/1057278.htm)|▪[locate](http://baike.baidu.com/view/1229038.htm)|▪[whereis](http://baike.baidu.com/view/1793607.htm)|▪[which](http://baike.baidu.com/view/1793602.htm)|||||
|▪[find](http://baike.baidu.com/view/801452.htm)|▪[grep](http://baike.baidu.com/view/1057278.htm)|▪[locate](http://baike.baidu.com/view/1229038.htm)|▪[whereis](http://baike.baidu.com/view/1793607.htm)| | | | | | |
|▪[which](http://baike.baidu.com/view/1793602.htm)|||| | | | | | |
|| | | | | | | | | |
|杂项| | | | | | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
||▪[apropos](http://baike.baidu.com/searchword/?word=apropos&pic=1&sug=1&enc=utf8)|▪[banner](http://baike.baidu.com/view/53292.htm)|▪[bc](http://baike.baidu.com/view/104163.htm)|▪[cal](http://baike.baidu.com/view/56183.htm)||----|----|----|----||▪[clear](http://baike.baidu.com/view/611255.htm)|▪[date](http://baike.baidu.com/view/555251.htm)|▪[dd](http://baike.baidu.com/view/21965.htm)|▪[file](http://baike.baidu.com/view/32758.htm)||▪[help](http://baike.baidu.com/view/310222.htm)|▪[info](http://baike.baidu.com/view/105555.htm)|▪[size](http://baike.baidu.com/view/126727.htm)|▪[lp](http://baike.baidu.com/view/136896.htm)||▪[man](http://baike.baidu.com/view/2671.htm)|▪[history](http://baike.baidu.com/view/325910.htm)|▪[tee](http://baike.baidu.com/view/912941.htm)|▪[tput](http://baike.baidu.com/searchword/?word=tput&pic=1&sug=1&enc=utf8)||▪[type](http://baike.baidu.com/view/360307.htm)|▪[yes](http://baike.baidu.com/view/74673.htm)|▪[uname](http://baike.baidu.com/view/1374878.htm)|▪[whatis](http://baike.baidu.com/view/2129683.htm)||▪[apropos](http://baike.baidu.com/searchword/?word=apropos&pic=1&sug=1&enc=utf8)|▪[banner](http://baike.baidu.com/view/53292.htm)|▪[bc](http://baike.baidu.com/view/104163.htm)|▪[cal](http://baike.baidu.com/view/56183.htm)|▪[clear](http://baike.baidu.com/view/611255.htm)|▪[date](http://baike.baidu.com/view/555251.htm)|▪[dd](http://baike.baidu.com/view/21965.htm)|▪[file](http://baike.baidu.com/view/32758.htm)|▪[help](http://baike.baidu.com/view/310222.htm)|▪[info](http://baike.baidu.com/view/105555.htm)|▪[size](http://baike.baidu.com/view/126727.htm)|▪[lp](http://baike.baidu.com/view/136896.htm)|▪[man](http://baike.baidu.com/view/2671.htm)|▪[history](http://baike.baidu.com/view/325910.htm)|▪[tee](http://baike.baidu.com/view/912941.htm)|▪[tput](http://baike.baidu.com/searchword/?word=tput&pic=1&sug=1&enc=utf8)|▪[type](http://baike.baidu.com/view/360307.htm)|▪[yes](http://baike.baidu.com/view/74673.htm)|▪[uname](http://baike.baidu.com/view/1374878.htm)|▪[whatis](http://baike.baidu.com/view/2129683.htm)|
|▪[apropos](http://baike.baidu.com/searchword/?word=apropos&pic=1&sug=1&enc=utf8)|▪[banner](http://baike.baidu.com/view/53292.htm)|▪[bc](http://baike.baidu.com/view/104163.htm)|▪[cal](http://baike.baidu.com/view/56183.htm)| | | | | | | | | | | | | | | | | |
|▪[clear](http://baike.baidu.com/view/611255.htm)|▪[date](http://baike.baidu.com/view/555251.htm)|▪[dd](http://baike.baidu.com/view/21965.htm)|▪[file](http://baike.baidu.com/view/32758.htm)| | | | | | | | | | | | | | | | | |
|▪[help](http://baike.baidu.com/view/310222.htm)|▪[info](http://baike.baidu.com/view/105555.htm)|▪[size](http://baike.baidu.com/view/126727.htm)|▪[lp](http://baike.baidu.com/view/136896.htm)| | | | | | | | | | | | | | | | | |
|▪[man](http://baike.baidu.com/view/2671.htm)|▪[history](http://baike.baidu.com/view/325910.htm)|▪[tee](http://baike.baidu.com/view/912941.htm)|▪[tput](http://baike.baidu.com/searchword/?word=tput&pic=1&sug=1&enc=utf8)| | | | | | | | | | | | | | | | | |
|▪[type](http://baike.baidu.com/view/360307.htm)|▪[yes](http://baike.baidu.com/view/74673.htm)|▪[uname](http://baike.baidu.com/view/1374878.htm)|▪[whatis](http://baike.baidu.com/view/2129683.htm)| | | | | | | | | | | | | | | | | |






# scanf




与[printf](http://baike.baidu.com/view/410546.htm)函数一样，都被定义在[头文件](http://baike.baidu.com/view/668911.htm)stdio.h里，因此在使用scanf函数时要加上#include
 <[stdio.h](http://baike.baidu.com/view/538727.htm)>。它是格式输入函数，

即按用户指定的格式从键盘上把数据输入到指定的[变量](http://baike.baidu.com/view/296689.htm)之中。



函数原型


int scanf(const char*format,...);

**scanf()**是从标准输入输出流stdio.h
 (标准输入设备)，一般是键盘中读内容的通用子程序，

可以说明的格式读入多个字符，保存在对应地址的变量中。

其调用形式为: scanf("<格式说明字符串>"，<变量地址>);变量地址要求有效，

并且与格式说明的次序一致。

**scanf()函数返回成功赋值的数据项数，读到文件末尾出错时则返回EOF。**

如：scanf("%d%d",&a,&b);


如果a和b都被成功读入，那么scanf的返回值就是2

如果只有a被成功读入，返回值为1

如果a和b都未被成功读入，返回值为0

如果遇到错误或遇到end of file，返回值为[EOF](http://baike.baidu.com/view/56074.htm)。

且返回值为[int](http://baike.baidu.com/subview/804413/9186076.htm)型.

注意问题


（1） 对于字符串数组或字符串指针变量，由于数组名和指针变量名本身就是地址，因此使用scanf()函数时，不需要在它们前面加上"&"操作符。

（2） 可以在格式化字符串中的"%"各格式化规定符之间加入一个整数，表示任何读操作中的最大位数。

（3） scanf()函数中没有精度控制。

         如： scanf("%5.2f",&a); 是非法的。不能企图用此语句输入小数为2位的实数。

（4） scanf中要求给出[变量](http://baike.baidu.com/view/296689.htm)地址，如给出变量名则会出错

          如 scanf("%d",a);是非法的，应改为scanf("%d",&a);才是合法的。

（5） 在输入多个数值数据时，若格式控制串中没有非[格式字符](http://baike.baidu.com/view/2194593.htm)作输入数据之间的间隔,则可用空格，TAB或回车作间隔。

C编译在碰到空格，TAB，回车或非法数据(如对“%d”输入“12A”时，A即为非法数据)时即认为该数据结束。

（6） 在输入[字符](http://baike.baidu.com/view/263416.htm)数据(%c)时，若格式控制串中无非[格式字符](http://baike.baidu.com/view/2194593.htm)，则认为所有输入的字符均为有效字符。

例如：scanf("%c%c%c",&a,&b,&c);

输入为： d e f

 则把'd'赋予a, ' (空格)'赋予b,'e'赋予c。因为%c 只要求读入一个[字符](http://baike.baidu.com/view/263416.htm)，后面不需要用空格作为两个字符的间隔，因此把'
 '作为下一个字符送给b。

只有当输入为：def (中间无空格)时，才能把'd'赋于a,'e'赋予b,'f'赋予c。 如果在格式控制中加入空格作为间隔，

如：scanf("%c %c %c",&a,&b,&c);

则输入时各数据之间可加空格。

scanf标准输入的一些问题：

  1. 如何让scanf()函数正确接受有空格的字符串？如:
 I love you!


  2. 键盘[**缓冲区**](http://baike.baidu.com/view/266782.htm)残余信息问题

 
 3. 输入类型与格式化字符串不匹配导致stdin流的阻塞。

 
 4. 如何处理scanf()函数误输入造成程序死锁或出错

**补充**

[fflush(stdin)](http://baike.baidu.com/view/1913351.htm)这个方法在GCC下不可用。（在VC6.0下可以）













