# linux相关命令（grep tr  alias ）配合案例讲解-老男孩之永不止步-51CTO博客
1、alias 设置别名--网卡，单引号和双引号都可以，这是临时配置，重启后不生效
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/2558ea22bad4a11c397492d277f62787.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/4cfc16c80d4886ed0b5b407a7651cf5b.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
永久生效 vim /etc/profile 在末尾添加内容
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/bd85cb73c13b32c13a1dfe43f6d7e54a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/308c8d8f12a1496b7e25c512a9fceb53.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/57768f3c0646d27f0f886370d0ce5699.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
使用命令source  /etc/profile 使配置生效
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/0b9657447b00c7698c4d8a0d810368b2.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2、grep 过滤文件内容
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/c281f45a1f86389fdd02423fdccacfc4.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
-n 显示行号和内容
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/a35d489e5ace453c201f7f8ed93d6257.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
-w 按照单词进行查找
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/fa06857d97db07fd6f2c62f4b6421fbe.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
-i忽略大小写
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/4b9a12dfd0c6539efc916f13560b646c.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
-v 取反或排除，过滤出不包含#的内容
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/77abe8021f7db5c47e9dbe64d9141afd.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
以上的参数可以组合使用如：-ni   -nwiv
3、tr 替换 搭配 < 输入重定向来使用
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/65785dcb1faf33960666c93cb4c37ec3.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
将所有小写字母更改为大写字母
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/aa925e9bb28d064f010645a7f3998fb3.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
将a都替换为1，b都替换为2，c都替换为3，一一对应
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/08d771339333702d18d672a0e5ebfae9.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
命令行使用，一般还没有生效
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/d174b7ad2613d6504f45d551284923d3.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
加上绝对路径或者\取消别名，直接复制不提示覆盖
![linux相关命令（grep tr  alias ）配合案例讲解](https://s1.51cto.com/images/blog/201904/08/950beb5642bbd6640b0848882ee63acd.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
