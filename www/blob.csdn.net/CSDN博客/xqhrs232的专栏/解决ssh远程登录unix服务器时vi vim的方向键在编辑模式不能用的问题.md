# 解决ssh远程登录unix服务器时vi/vim的方向键在编辑模式不能用的问题 - xqhrs232的专栏 - CSDN博客
2017年03月19日 11:20:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1036
原文地址::[http://blog.csdn.net/ablo_zhou/article/details/1562822](http://blog.csdn.net/ablo_zhou/article/details/1562822)
相关文章
1、vi/vim使用入门: vimrc在哪儿?----[http://easwy.com/blog/archives/where-is-vimrc/](http://easwy.com/blog/archives/where-is-vimrc/)
2、
## 配置文件vimrc存放位置
 ----[http://blog.sina.com.cn/s/blog_4ba5b45e0102e1jm.html](http://blog.sina.com.cn/s/blog_4ba5b45e0102e1jm.html)
作者：周海汉 Email:ablo_zhou#163.com #换为@
远程用ssh登录unix或[Linux](http://lib.csdn.net/base/linux)服务器时，有时会遇到方向键失灵的问题。看到很多人都遇到相似的问题，但没有找到好的解决办法。例如我通过ssh2登录solaris
 10时，使用vi或vim编辑器，在编辑模式下移动方向键，会输入A B C D。而且删除也不能用。ubutun 使用vi时好像也有此问题，但使用vim则无此问题。 查阅了一些资料，找到一种解决办法：
$ vim ./vimrc
输入
set t_ku=^[OA 
set t_kd=^[OB 
set t_kr=^[OC 
set t_kl=^[OD 
保存退出。
其中^[OA等几个键是用[ctrl+v][方向键]输入的，u d r l分别代表上下右左。
这样，在solaris 10 远程ssh登录后使用vim，编辑模式下方向键不再输入乱码。也可以删除刚刚输入的内容了。 
[](http://blog.csdn.net/ablo_zhou/article/details/1562822#)[](http://blog.csdn.net/ablo_zhou/article/details/1562822#)[](http://blog.csdn.net/ablo_zhou/article/details/1562822#)[](http://blog.csdn.net/ablo_zhou/article/details/1562822#)[](http://blog.csdn.net/ablo_zhou/article/details/1562822#)[](http://blog.csdn.net/ablo_zhou/article/details/1562822#)
- 
顶
0- 
踩
