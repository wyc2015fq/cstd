
# Eclipse工程导入和删除 - 李昆鹏的博客 - CSDN博客


2018年03月08日 10:03:39[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：534


-------------------------------------**Eclipse工程导入和删除**-----------------------------------
包的管理
1.   打jar包
选中项目—右键—Export—java—jar—next—自己写jar包的名字和选择路径—finish
![](https://img-blog.csdn.net/20180308100126902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180308100133323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.   Jar包的使用
导入jar包
1.   单独添加jar、
把jar复制，然后粘贴到项目路径下
![](https://img-blog.csdn.net/20180308100150507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一定要把jar添加至classpath路径中。
怎么添加呢？
直接把依赖包拷贝到我们的项目的src下，然后右键jar包Add BuildPath
![](https://img-blog.csdn.net/20180308100159317?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
导入成功
![](https://img-blog.csdn.net/201803081002055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果删除，首先Remove BuildPath再删除src下的包
![](https://img-blog.csdn.net/201803081002299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后再删除源文件包
![](https://img-blog.csdn.net/20180308100233343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**注意：**
![](https://img-blog.csdn.net/20180308100243241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
----------------------------------**导入已有的项目**--------------------------------
1.   空白区域—右键—import—General—ExistingProject into Workspace—选中你的项目目录
--finish—ok
**在删除项目的时候注意下面的情况**
![](https://img-blog.csdn.net/20180308100249928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
引入项目
第一步
![](https://img-blog.csdn.net/2018030810025687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第二步
![](https://img-blog.csdn.net/20180308100259720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第三步
![](https://img-blog.csdn.net/2018030810030535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180308100312506?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


