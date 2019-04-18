# 解决xfire和ws冲突问题 - z69183787的专栏 - CSDN博客
2014年05月07日 17:13:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2313
同时用到了JAX-WS和xfire，直接引用myclipse自带的xfire 1.2 Core Libraries后，发生冲突。本该调用JAX-WS的相关包，却调用了xfire的。
于是对xfire 1.2 Core Libraries进行精简，保留的jar包如图所示。
![解决xfire和ws冲突问题 - belinda - belindas](http://img.bimg.126.net/photo/iX9sDFx8f53VXtq5Jw6Gdw==/4845591724090761772.jpg)
现在二者可以共存了。
本人使用的是 xfire-all-1.2.6.jar 包，故进入将jaxws目录整个删除，最后运行成功。
