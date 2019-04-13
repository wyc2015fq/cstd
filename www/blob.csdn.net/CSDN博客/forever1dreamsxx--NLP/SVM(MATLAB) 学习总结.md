
# SVM(MATLAB) 学习总结 - forever1dreamsxx--NLP - CSDN博客


2012年12月07日 16:54:21[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：913个人分类：[SVM																](https://blog.csdn.net/forever1dreamsxx/article/category/1295874)



## 转载地址：
## [http://blog.csdn.net/liweifire/article/details/6922960](http://blog.csdn.net/liweifire/article/details/6922960)

## 1 SVM通用工具包
SVM通用工具包下载地址：http://www.isis.ecs.soton.ac.uk/isystems/kernel/svm.zip
详见：[http://see.xidian.edu.cn/faculty/chzheng/bishe/indexfiles/indexl.htm](http://see.xidian.edu.cn/faculty/chzheng/bishe/indexfiles/indexl.htm)。
解压到toolbox目录下，然后用addpath或者pathtool把该工具箱的路径添加到matlab的搜索路径中，最后用which newtoolbox_command.m来检验是否可以访问。
例如：
|>> addpath D:\'Program Files'\MATLAB\R2007a\toolbox\svm
|>> which svcinfo.m
|D:\Program Files\MATLAB\R2007a\toolbox\svm\svcinfo.m
|>>|
补充：这样有一个问题，每次打开还要重新addpath。通过：打开matlab->File->Set Path中添加SVM工具箱的文件夹，这样可以直接添加了，save。这样不用每次重新打开matlab重新addpath了。
下载了steve gunn 's svm toolbox，才知道这个包即为steve
 gunn 所写的。

如果出现错误：
??? Invalid MEX-file ‘××××\svm\qp.dll': The specified procedure could not be found.
??? Invalid MEX-file 'D:\Program Files\MATLAB\R2007a\toolbox\svm\qp.dll': 找不到指定的程序。
Error in ==> svc at 60
[alpha lambda how] = qp(H, c, A, b, vlb, vub, x0, neqcstr);
参见：[http://frysoo.blog.163.com/blog/static/575172392009293150887/](http://frysoo.blog.163.com/blog/static/575172392009293150887/)
这里方便看，把内容贴出来了。
这个问题是由于版本问题引起的，我使用的是Matlab R2007版。我正在努力找解决方案，看看能否重新编译一下qp.dll。
查阅一晚上资料，终于找到如何解决这个简单的问题了。关键是一开始没有仔细看。
|问题：??? Invalid MEX-file 'D:\Program Files\MATLAB\R2007a\toolbox\svm\qp.dll':找不到指定的程序。
|问题描述：mex在不同windows OS下编译的结果，所以我们需要重新编译一下qp.dll
|方法：steve gunn 的包下面有一个optimiser 文件夹，把current Diretory目录改为optimiser目录，例如D:\Program Files\MATLAB\R2007a\toolbox\svm\Optimiser，然后运行命令
|>> mex -v qp.c pr_loqo.c
|命令运行完毕后，你会发现原先的qp.dll变为qp.dll.old，还出现了qp.mexw32，我们把该文件改为qp.dll 复制到工具箱文件夹下。原先的工具箱文件qp.dll可以先改一下名字....(|Frysoo@hotmail.com|)
|
这个问题解决了...
我运行下面的文件通过(该文件从网所搜索测试，作者没有求证，感谢原作者)。
|N = 50;    n=2*N;randn('state',6); x1 = randn(2,N); y1 = ones(1,N); x2 = 5+randn(2,N);y2 = -ones(1,N);
|figure;plot(x1(1,:),x1(2,:),'bx',x2(1,:),x2(2,:),'k.');axis([-3 8 -3 8]);title('C-SVC')
|hold on;X1 = [x1,x2];Y1 = [y1,y2];  X=X1';Y=Y1';    C=Inf; ker='linear';
|global p1 p2|p1=3; p2=1;
|[nsv alpha bias] = svc(X,Y,ker,C)
|

注：有的读者通过google或者百度搜索，??? Invalid MEX-file  找不到指定的程序，找到这里来了，呵呵。
其实该文章是对svm的matlab工具包编写的。
解决：???Invalid MEX-file  的一个共性就是，版本问题，您可以尝试更换matlab版本。例如您的mex dll在6.5版本中编写的，请不要使用7.4版本去调用，matlab就这一点很讨厌人...Frysoo

## 2 其他工具箱
SVM OSU_3.00 http://www.ece.osu.edu/~maj/osu_svm/osu_svm3.00.zip


参考资料：
1[http://frysoo.blog.163.com/blog/static/575172392009293150887/](http://frysoo.blog.163.com/blog/static/575172392009293150887/)
2[http://see.xidian.edu.cn/faculty/chzheng/bishe/index.htm](http://see.xidian.edu.cn/faculty/chzheng/bishe/index.htm)
3[
http://www.isis.ecs.soton.ac.uk/resources/svminfo/](http://www.isis.ecs.soton.ac.uk/resources/svminfo/)

