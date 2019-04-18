# getResource中文或有空格路径处理 - z69183787的专栏 - CSDN博客
2012年12月26日 11:27:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1300
今天遇到文件路径中有中文，读取文件就找不到，查了下得到以下解决方法，记录下来。
在使用类似这样：
Java代码  ![收藏代码](http://justmiaotou.iteye.com/images/icon_star.png)
- this.getClass().getClassLoader().getResource("").getPath()  
来获取文件路径时，里面的路径空格会被“%20”代替，这时候如果你用这个获取到的包含“%20”的路径来new一个File时，会出现找不到路径的错误。
于是有以下官方解决方法：
Java代码  ![收藏代码](http://justmiaotou.iteye.com/images/icon_star.png)
- URI uri = new URI(url.toString());  
- FileInputStream fis = new FileInputStream(uri.getPath())  
但有另一种解决方法：
Java代码  ![收藏代码](http://justmiaotou.iteye.com/images/icon_star.png)
- configPath = java.net.URLDecoder.decode(configPath,"utf-8");  
于是乎，问题解决了……
