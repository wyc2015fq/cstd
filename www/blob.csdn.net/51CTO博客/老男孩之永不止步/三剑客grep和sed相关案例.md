# 三剑客grep和sed相关案例-老男孩之永不止步-51CTO博客
1、grep（-i -n -w -A -B -C -l -R -E）
1.1grep -l 只找出来文件名，不显示内容
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/acab48baad68b499dbb4aa84c82c435a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
1.2、grep -R  递归进行过滤，使grep 后面可以加目录
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/5e832831bea2a4cc15cbe22b1c1f1446.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/461b195aedacd037599f2f57fa2a0f9d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
1.3、grep -A（after） 显示找出来的内容及接下来的几行
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/9ee3c76ed60333dbaaf7fe504a24f531.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
1.4、grep -B（before） 显示找出来的内容及上面的几行
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/26a1b3993ac8ecd46738d3e4a578fea0.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
1.5、grep -C（-C2==-A2 -B2） 显示找出来的内容及上下的几行
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/483922f53ca1ae9ef8e042d482d63b94.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/1d86d794be3644bfcb1eb08e34e0056d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2、sed 流编辑器（增删改查）
2.1 sed -n（p-print显示）取消默认输出，若不加-n，会将文件所有内容输出
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/37e5520961b19e2f9ddd74a1d2173828.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2.2 sed -n '1,3p' 范围----1至3行（查找）
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/28458b1c52820d8cc79e822311a2c3b9.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2.3、//---查找斜线里面有的内容的行
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/7d2da1adb43dc2d89902d59a841252b2.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2.4、从哪里来到哪里去,查找范围,只会查找到最近的内容就会结束,不会继续执行，从哪里来若相同-从第一个开始，从哪里去若相同从第一个结束，若后面没有这个内容或者写错了，一直到文件内容结尾。
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/2089d0c66a32b4566965c3f088f0386d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
没有qq，一直查找到结尾
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/238d752f61ec609d371c56f7dfc0e9b0.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2.5 3a --第三行后面增加内容
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/7b3fced184a6bf45c47bafc05de69a84.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2.6、 -i 确认
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/c4eb8a25912843ccdb053efb3c6db1d6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/2ea886a78090e604bf7abaae5c9a0347.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2.7、sed -i.bak先备份后再确认修改内容
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/b8f58b429f38eb5bfa485f56bfe848e5.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/2dc6fae4c721884558f3ed26559ed009.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2.8、 i 插入，如3i第三行上面写入内容
2.9、 c 把这一行先清空再写入
修改内容：
删除 d，将空行删除
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/ce024f5365b484baf5b8e260b29d046b.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
删除不显示空行，！非
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/03501cb1d182d9b0fb90dfd785f65bce.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/54ce8f350414c74823c0b515469503ee.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/332fe4e46275352f274571e2c7142897.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
案例实践：
#不显示文件/etc/ssh/sshd_config 的空行或以#号开头的行（5种方法）
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/bf2bd249e1bced678714b318928d8e51.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/c5099af4ae3685b357dfcb24867c3ad8.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/074da7ddc9f7ce9b59304d0bb6764abd.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/19072c75b176866f3b4862283e3e3b68.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/6b80539e7086f1b575a5fc76152ebbdc.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
sed 's###g'
![三剑客grep和sed相关案例](https://s1.51cto.com/images/blog/201904/18/1010f7c6909a5f526a48460a6007243d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
