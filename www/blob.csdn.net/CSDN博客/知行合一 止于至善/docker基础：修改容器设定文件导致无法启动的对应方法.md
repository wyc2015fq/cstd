
# docker基础：修改容器设定文件导致无法启动的对应方法 - 知行合一 止于至善 - CSDN博客

2019年03月08日 06:15:43[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：411


当修改了某个容器的设定文件，导致此容器无法启动时，如果此时设定文件未被挂出，则会导致无法改回此文件而重新启动容器，可以在宿主机上找到此文件，进行修改即可。
# 场景示例
Mysql设定binlog时，版本5.7之后，需要同时设定server-id，如果只是设定了log_bin=ON而未设定server-id，则会导致容器无法启动。而由于/etc/mysql/mysql.conf.d/mysqld.cnf文件未被挂出, 如何改回此文件重新启动mysql容器则是一个问题。
# 文件定位
此问题的关键在于如何定位修改后的设定文件，可以采用如下方式。
# 方法1
直接在/下搜索mysqld.cnf中包含修改内容的文件存在的路径
# 方法2
步骤1: 使用docker inspect 镜像名称 来取得如下的信息
`"Data": {
                "LowerDir": "/opt/docker/overlay/c4c7916c7f092ae360f546ef1f15557160f5320e7749a12227e74c240729c432/root",
                "MergedDir": "/opt/docker/overlay/df4245a7bd277141523f0d3773da0f6bfcf2f7b7ecfec4a68ee7161df59f7f09/merged",
                "UpperDir": "/opt/docker/overlay/df4245a7bd277141523f0d3773da0f6bfcf2f7b7ecfec4a68ee7161df59f7f09/upper",
                "WorkDir": "/opt/docker/overlay/df4245a7bd277141523f0d3773da0f6bfcf2f7b7ecfec4a68ee7161df59f7f09/work"
            }`可以看到此机器是使用overlay方式进行的docker 安装，接下来在/opt/docker/overlay下进行搜索即可。如果熟悉的话会知道这里其实是在upper下进行的layer是修改的内容
`[root@platform ~]# cd /opt/docker/overlay/df4245a7bd277141523f0d3773da0f6bfcf2f7b7ecfec4a68ee7161df59f7f09/upper
[root@platform upper]# find . -name mysqld.cnf |xargs grep log_bin
log_bin=ON
[root@platform upper]#`
# 修改并重启
删除此行信息，并重启容器即可解决问题。

