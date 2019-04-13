
# CShell下的stderr重定向 - Augusdi的专栏 - CSDN博客


2015年05月04日 21:02:13[Augusdi](https://me.csdn.net/Augusdi)阅读数：1073


﻿﻿
在C Shell下面不能直接使用
2>&1
将stderr的内容重定向到stdout，需要用下面的方法来实现：
( rm dir >out.txt ) >& err.txt
原因是cshell不能单独重定向stderr，只能重定向标准输出stdout，或>&同时重定向stdout和stderr，故只有先在子shell先将stdout重定向到某处，相当于先把stdout过滤掉。

|更多

|[http://www.itokit.com/2011/0914/68456.html](http://www.itokit.com/2011/0914/68456.html)
|


