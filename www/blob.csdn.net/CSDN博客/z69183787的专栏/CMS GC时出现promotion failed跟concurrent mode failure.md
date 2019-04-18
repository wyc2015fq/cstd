# CMS GC时出现promotion failed跟concurrent mode failure - z69183787的专栏 - CSDN博客
2019年04月08日 16:46:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：26
个人分类：[Jvm-Gc&垃圾回收](https://blog.csdn.net/z69183787/article/category/2917447)
CMS GC时出现promotion failed和concurrent mode failure
对于采用CMS进行旧生代GC的程序而言，尤其要注意GC日志中是否有promotion failed和concurrent mode failure两种状况，当这两种状况出现时可能会触发Full GC。
promotion failed是在进行Minor GC时，survivor space放不下、对象只能放入旧生代，而此时旧生代也放不下造成的；concurrent mode failure是在执行CMS GC的过程中同时有对象要放入旧生代，而此时旧生代空间不足造成的。
所以在别一次性占用太多的内存，如果是读文件，可以采用拆分的方法。或者把GC内存调大点
