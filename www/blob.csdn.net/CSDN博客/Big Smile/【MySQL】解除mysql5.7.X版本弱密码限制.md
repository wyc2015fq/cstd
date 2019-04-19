# 【MySQL】解除mysql5.7.X版本弱密码限制 - Big Smile - CSDN博客
2018年07月31日 11:44:22[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1196
所属专栏：[mysql系列](https://blog.csdn.net/column/details/26482.html)
        mysql数据库从5.7的版本开始对密码进行了严格的限制，因为他默认安装了一个validate_password的插件，我们在命令行中输入
`mysql> show plugins;`
即可看到，一般在最下方：
![](https://img-blog.csdn.net/20180731113650311?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
状态为AVTIVE，说明为启用状态。
我们可以看一下他的验证规则，在mysql命令中中输入：
`mysql> SHOW VARIABLES LIKE 'vali%';`
![](https://img-blog.csdn.net/20180731113838353?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- validate_password_length 8                             # 密码的最小长度，此处为8。
- validate_password_mixed_case_count 1         # 至少要包含小写或大写字母的个数，此处为1。
- validate_password_number_count 1                # 至少要包含的数字的个数，此处为1。
- validate_password_policy MEDIUM                 # 强度等级，其中其值可设置为0、1、2。分别对应：
	- 【0/LOW】：只检查长度。
- 【1/MEDIUM】：在0等级的基础上多检查数字、大小写、特殊字符。
- 【2/STRONG】：在1等级的基础上多检查特殊字符字典文件，此处为1。
- validate_password_special_char_count 1         # 至少要包含的个数字符的个数，此处为1。
想要关闭这个插件，我们在我们的my.cnf文件中加入一行即可：
```
[mysqld]
validate_password=off
```
然后我么可以再次查看这个插件的状态【show plugins;】：
![](https://img-blog.csdn.net/20180731114259446?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
变为DISABLED了。
这样就解除了弱密码的限制了，建议在内网或者本地这样使用，真正发布在外网的时候还是复杂密码比较安全。
