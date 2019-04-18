# Docker拉取镜像报错no matching manifest for unknown in the manifest list entries - z69183787的专栏 - CSDN博客
2019年02月01日 15:32:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：319
在Docker拉取centos镜像时报错
> 
C:\WINDOWS\system32>docker pull centos
Using default tag: latest
latest: Pulling from library/centos
no matching manifest for unknown in the manifest list entries
鼠标点击docker图标，进入设置（settings）=> Daemon ，开启advance，把json 里面的experimental设置为true，保存重启docker即可。
![](http://oldchen.iwulai.com/wp-content/uploads/2018/11/56c07cf495e26993e9ce49aa8fc155e4.png)
查看镜像：
> 
C:\WINDOWS\system32>docker images
REPOSITORY TAG IMAGE ID CREATED SIZE
**centos**      latest 75835a67d134 3 weeks ago 297MB
hello-world   latest e3c960ee07af 7 weeks ago 334MB
