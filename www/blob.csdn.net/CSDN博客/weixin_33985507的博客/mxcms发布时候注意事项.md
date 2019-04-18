# mxcms发布时候注意事项 - weixin_33985507的博客 - CSDN博客
2012年10月12日 09:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
network service 角色 完全控制的权限 （xml目录里的所有文件都要赋这个权限）
xml/sys/base.config
xml/sys/base.config
baidu.xml  
index.html
 ejuportal部署：
1 权限：在iis6里设置文件夹 files,xml，templets,logs 添加角色network service，并且给完全权限。
2 iis里web目录添加aspnet角色，并且给最大权限。
今天发布的时候，由于在本地有测试数据，有2个存放图片的地方没有发布出来。
1个是Kindeditor图片存放的路径；2是发布新闻的其他图片存放路径；
解决方法：将本地的2个图片方法目录全部上传到虚拟空间里即可。
安全性：
1 删除文件夹 Install目录，包括下面的所有文件。
2 将不是该站的模版目录删除。
