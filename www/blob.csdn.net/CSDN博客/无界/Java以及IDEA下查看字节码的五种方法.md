
# Java以及IDEA下查看字节码的五种方法 - 无界 - CSDN博客

2019年03月08日 15:26:11[21aspnet](https://me.csdn.net/21aspnet)阅读数：211个人分类：[Java																](https://blog.csdn.net/21aspnet/article/category/1877583)



## 1.最本质的是cmd下
\#javap -v 类名
说明：这是最原始的方法，效率低下，强烈不推荐使用。

## 2.查看字节码的方法idea可以集成命令行使用javap
![](https://img-blog.csdnimg.cn/20190308144058837.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)
注意需要在class文件目录下，不要在java文件目录下。
\#java -p  class文件名即可
这种方式需要每次敲命令还是不方便

## 3.在IDEA下集成javap -v
上面那种方法效率只能说还是不高，其实可以在idea里建扩展工具建快捷方式
![](https://img-blog.csdnimg.cn/20190308151638940.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)
在tool setting 的Program输入工具的路径，这中间可以插入宏，比如`$JDK_PATH$`，不需要自己再手动输入jdk的路径了。
在参数栏中输入`-p $FileClass$`，**$FileClass$**代表要解析的 class文件。
在Workding Directory中输入**$OutputPath$**代表项目的输出路径
![](https://img-blog.csdnimg.cn/20190308151652284.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)

## 4.查看字节码使用idea自带的插件
![](https://img-blog.csdnimg.cn/20190308152024659.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190308152035982.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)
这种方法比较简单，直接view菜单下就有，不过信息不全。

## 5.idea也可以安装jclasslib插件
![](https://img-blog.csdnimg.cn/20190306231928385.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)
jclasslib插件还是很方便的，setting里安装插件就行。
```python
public class TestApp {
    public static void main(String[] args) {
        int a=0;
        a=a++;
        System.out.println(a);
        a=++a;
        System.out.println(a);
        a+=2;
        System.out.println(a);
    }
}
```
输出：
![](https://img-blog.csdnimg.cn/20190307013502526.png)
字节码：
![](https://img-blog.csdnimg.cn/20190307013612432.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)
本地变量表：
![](https://img-blog.csdnimg.cn/20190307013623147.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)

