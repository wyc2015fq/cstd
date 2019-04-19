# inode和block，硬链接和软件链接-老男孩之永不止步-51CTO博客
1、inode   block 两者的关系
![inode和block，硬链接和软件链接](https://s1.51cto.com/images/blog/201904/10/139bdcd93d62b8ab38265741979a0c5a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![inode和block，硬链接和软件链接](https://s1.51cto.com/images/blog/201904/10/9f47de2439452559668faa694965b29a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
两者的特点
![inode和block，硬链接和软件链接](https://s1.51cto.com/images/blog/201904/10/0a1f328dad866185bf38894d34ae81d7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
df -h 查看block使用情况（磁盘使用情况）
![inode和block，硬链接和软件链接](https://s1.51cto.com/images/blog/201904/10/313495feb4915f803c032f6cc2958042.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
df -i 显示inode
![inode和block，硬链接和软件链接](https://s1.51cto.com/images/blog/201904/10/78df131ae3a87f9d17ff45695c60d7c4.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
软链接与硬链接的区别
![inode和block，硬链接和软件链接](https://s1.51cto.com/images/blog/201904/10/ea4be61f5eac743d9816fb39d0910f10.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
