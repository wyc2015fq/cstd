# CentOS7 安装完成后ifconfig命令不能用 - 菜鸟路上的小白 - CSDN博客





2016年09月08日 15:15:54[somnus_小凯](https://me.csdn.net/u012486840)阅读数：878








今天用VMWare安装了CentOS7，选择了最小安装包模式，安装完毕之后想查看一下本机的ip地址，发现报错

```bash
# ifconfig
```
`-bash: ifconfig: command not found`
谷歌了一下，整理了一下解决思路
- 
 查看ifconfig命令是否存在

 查看 /sbin/ifconfig是否存在



- 
如果ifconfig命令存在，查看环境变量设置    


```bash
#echo $PATH
```

       如果环境变量中没有包含ifconfig命令的路径

临时修改环境变量：在shell中输入
`$export PATH = $PATH:/sbin`
       然后再输入ifconfig命令即可，但是这只是临时更改了shell中的PATH，如果关闭shell，则修改消失，下次还需要重复如上操作


永久修改PATH变量使之包含/sbin路径：

        打开/etc/profile文件，在其中输入export PATH=$PATH:/sbin，保存并重启即可，这样一来，PATH路径永久修改成功，以后任何时候只输入ifconfig命令即可

  3. 如果ifconfig命令不存在
`yum upgrade``yum install net-tools`


