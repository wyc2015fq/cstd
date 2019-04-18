# Not allowed to load local resource - 苝花向暖丨楠枝向寒 - CSDN博客

2018年11月01日 23:37:12[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：949


访问tomcat外的硬盘中的文件夹出现错误，img标签中的src的值 形如 file:///C:\Users\Administrator\Desktop\eloamPhoto\picture.jpg。

结果图片就显示不出来。

console 提示

`Not allowed to load local resource`

这个是由于浏览器出于安全问题考虑，不允许以这种形式来访问资源。

**解决方式**

在tomcat目录下的conf目录中找到server.xml 文件，并在

HOST 标签内 添加如下内容

```xml
<Context  path="/Img"  docBase="C:\Users\Administrator\Desktop\eloamPhoto" reloadable="true"/>
```

path 代表虚拟路径，docBase代表真实路径

如果你用的idea，需要更改tomcat配置如下

勾选圈上的内容![把圈上的勾选](https://img-blog.csdnimg.cn/20181101233422228.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

将原路径 file:///C:\Users\Administrator\Desktop\eloamPhoto

替换为 /Img/picture.jpg

然后重启服务器，即可。

