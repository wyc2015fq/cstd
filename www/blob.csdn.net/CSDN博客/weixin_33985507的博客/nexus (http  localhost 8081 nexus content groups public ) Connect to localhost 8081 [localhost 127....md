# nexus (http://localhost:8081/nexus/content/groups/public/): Connect to localhost:8081 [localhost/127... - weixin_33985507的博客 - CSDN博客
2017年11月09日 14:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：97
原版打包错误：
**[ERROR] Failed to execute goal on project admin: Could not resolve dependencies for project com.sfm:admin:jar:1.0-SNAPSHOT: Failed to collect dependencies at com.sfm:model:jar:1.0-SNAPSHOT: Failed to read artifact descriptor for com.sfm:model:jar:1.0-SNAPSHOT: Could not transfer artifact com.sfm:model:pom:1.0-SNAPSHOT from/to nexus (****http://localhost:8081/nexus/content/groups/public/****): Connect to localhost:8081 [localhost/****127.0.0.1****, localhost/0:0:0:0:0:0:0:1] failed: Connection refused: connect -> [Help 1]**
这个错误其实是有两个错误：
先描述标题错误是： nexus 指向maven私服  也就是：
![5949949-986fbe26749a35a6.png](https://upload-images.jianshu.io/upload_images/5949949-986fbe26749a35a6.png)
这个我没有用到我很郁闷，所以我就想把这个配置注解了：
一般两个地方有：
一个是 pom.xml
一个是maven的配置文件Setting里面
这两个你没用到的nexus注解或者删除掉就行啦就ok啦如图（maven的Setting文件）：
![5949949-be46238ef351b710.png](https://upload-images.jianshu.io/upload_images/5949949-be46238ef351b710.png)
在说说第二个错误是我项目模块化不能被识别报错，不能成功打包，需要在父模块加源代码编译版本；
![5949949-419ed24cef81908a.png](https://upload-images.jianshu.io/upload_images/5949949-419ed24cef81908a.png)
其中一个子模块就ok拉
![5949949-c03b81d1abfbf89f.png](https://upload-images.jianshu.io/upload_images/5949949-c03b81d1abfbf89f.png)
