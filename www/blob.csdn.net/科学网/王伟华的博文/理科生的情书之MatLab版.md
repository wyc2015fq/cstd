# 科学网—理科生的情书之MatLab版 - 王伟华的博文




# 理科生的情书之MatLab版                           

已有 8688 次阅读2011-6-25 22:16|个人分类:[图像处理](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=11843&view=me)|系统分类:[图片百科](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=6)|关键词:MatLab,,情书|[情书](misc.php?mod=tag&id=34515)



用matlab所写，情书内容如下： 

clc 
clear all 
close all 
[x,y,z]=meshgrid(linspace(-1.3,1.3)); 
val=(x.^2 + (9/4)*y.^2 + z.^2 - 1).^3 - x.^2.*z.^3 - (9/80)*y.^2.*z.^3; 
isosurface(x,y,z,val,0) 
axis equal 
view(-10,24)  
colormap flag 
camlight 
lighting phong  



情书实际效果如下：






![](http://bbs.whu.edu.cn/wForum/bbscon.php?bid=64&id=402685&ap=616)



![](http://image.sciencenet.cn/album/201106/25/2227346bgbhhlg6gg64bg4.jpg)
由MatLab代码产生的心图



转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-459279.html](http://blog.sciencenet.cn/blog-81613-459279.html)

上一篇：[[转载]教育部《全国教育人才发展中长期规划（2010-2020年）》](blog-81613-459157.html)
下一篇：[[转载]华中科技大学一成(852名)2011届本科生未如期毕业](blog-81613-459719.html)




