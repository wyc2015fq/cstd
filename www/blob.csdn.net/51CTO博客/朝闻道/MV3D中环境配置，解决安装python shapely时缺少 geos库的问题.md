# MV3D中环境配置，解决安装python shapely时缺少 geos库的问题-朝闻道-51CTO博客
MV3D 环境配置
![MV3D中环境配置，解决安装python shapely时缺少 geos库的问题](https://s1.51cto.com/images/blog/201810/08/2c426ea0907f9a700263cf542c97cd25.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
需要安装 shapely库
在pip install shapely 的时候报错 发现没有geos库
先安装VS 2013 
然后出现
![MV3D中环境配置，解决安装python shapely时缺少 geos库的问题](https://s1.51cto.com/images/blog/201810/08/208a7c549df8546a20f63a87140e702c.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
打开CMD
![MV3D中环境配置，解决安装python shapely时缺少 geos库的问题](https://s1.51cto.com/images/blog/201810/08/b1fe0b4d574bac8e5fe0489ce8fbbdb4.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
下载 geos库
下载geos-3.5.0，放在d:\geos350中
下载网站：[http://trac.osgeo.org/geos/](http://trac.osgeo.org/geos/)
atuogen.bat
nmake /f makefile.vc
![MV3D中环境配置，解决安装python shapely时缺少 geos库的问题](https://s1.51cto.com/images/blog/201810/08/7dd626ae345ab06b897b5627a36b8dbb.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
之后在src目录下生成
![MV3D中环境配置，解决安装python shapely时缺少 geos库的问题](https://s1.51cto.com/images/blog/201810/08/1a92c5fdf1d860cacdb207e1598f4a53.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![MV3D中环境配置，解决安装python shapely时缺少 geos库的问题](https://s1.51cto.com/images/blog/201810/08/3d9ffce93bd6d8333dda26616d3b3f05.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
将文件拷贝到system32目录
然后安装 pip install shapely 
请注意 你的系统是64位的 就要用 vcvars64.bat
