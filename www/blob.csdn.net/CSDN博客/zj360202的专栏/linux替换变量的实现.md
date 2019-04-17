# linux替换变量的实现 - zj360202的专栏 - CSDN博客





2014年08月07日 19:15:04[zj360202](https://me.csdn.net/zj360202)阅读数：487标签：[脚本																[linux																[for																[sed](https://so.csdn.net/so/search/s.do?q=sed&t=blog)
个人分类：[linux](https://blog.csdn.net/zj360202/article/category/2463627)





一、单个参数的修改



```java
1.sh
     ls #val
```

```java
2.sh
     sed 's/#val/./g' 1.sh > 3.sh
     sh 3.sh
```
执行脚本
```java
sh 2.sh
```


二、多个参数的修改
```java
1.sh
    ls #val1 > #val2
```

```java
2.sh---
#!/bin/bash

i=1
for args in $*;
	do if [ $i -eq 1 ]; then
		sed 's/#val'$i'/'$args'/g' 1.sh > 3.sh
		i=$(expr $i \+ 1)
	else
		sed -i 's/#val'$i'/'$args'/g' 3.sh 
		i=$(expr $i \+ 1) 
	fi
done
sh 3.sh
```

```java
执行脚本
sh 2.sh
```](https://so.csdn.net/so/search/s.do?q=for&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)




