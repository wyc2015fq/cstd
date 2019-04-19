# Github Git彻底删除历史提交记录的方法 - xcyl的口袋 - CSDN博客
2017年01月08日 23:39:49[xcyl](https://me.csdn.net/cai6811376)阅读数：23307标签：[github																[git](https://so.csdn.net/so/search/s.do?q=git&t=blog)](https://so.csdn.net/so/search/s.do?q=github&t=blog)
个人分类：[Git](https://blog.csdn.net/cai6811376/article/category/6671208)
有时候我们可能会遇到git提交错误的情况，比如提交了敏感的信息或者提交了错误的版本。这个时候我们想将提交到代码库的记录删除，我们要怎么做呢？ 
首先，我们需要找到我们需要回滚到的提交点的hash，可以使用git log命令获取提交的历史找到需要回滚到的提交点 
![这里写图片描述](https://img-blog.csdn.net/20170108230821630?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
复制hash值，使用git reset –hard commit_hash 
![这里写图片描述](https://img-blog.csdn.net/20170108231322294?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再使用git push origin HEAD –force即可 
![这里写图片描述](https://img-blog.csdn.net/20170108231430165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：[288214068](http://jq.qq.com/?_wv=1027&k=2Dlyhr7) / [496258327](http://jq.qq.com/?_wv=1027&k=2Hyz2ea)
Copyright © www.EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
