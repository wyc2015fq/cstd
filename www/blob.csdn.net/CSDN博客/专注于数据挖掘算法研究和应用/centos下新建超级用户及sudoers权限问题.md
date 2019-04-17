# centos下新建超级用户及sudoers权限问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月03日 14:34:14[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2471








                
1、新建用户：

   #useradd hdfs



2、修改密码：

   #passwd hdfs



3、设置超级用户权限

   可先sudo chmod 777 /etc/sudoers 读写权限

   修改/etc/sudoers文件，加入最后一行加入(免密切换到sudo)：

   hdfs    ALL=(ALL)    NOPASSWD: ALL

   设置/etc/sudoers 只读权限chmod 440 /etc/sudoers



4、查看用户权限：cat /etc/passwd



5、错误：上面要用root用户执行，用超级用户执行，会提示下面错误，sudo无法正常使用。

```
sudo：/etc/sudoers 可被任何人写
sudo：没有找到有效的 sudoers 资源，退出
sudo：无法初始化策略插件
```



   解决方案：

    sudoers的权限被改，要改回来

   pkexec chmod 0440 /etc/sudoers



   正常下/etc/sudoers的权限是：
`-r--r-----   1 root root     4050 5月   3 14:01 sudoers`



