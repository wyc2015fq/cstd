
# Lightgbm-GPU运行出错（2017.11）boost::filesystem::create_directory...Abort (core dumped) - OraYang的博客 - CSDN博客

2017年11月24日 10:23:56[OraYang](https://me.csdn.net/u010665216)阅读数：884所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



## 问题描述
在编译lightgbm GPU版本时并没有出错，但是在运行demo时发生了错误，错误如下：
![这里写图片描述](https://img-blog.csdn.net/20171124100021627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171124100021627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
部分logo：
[
](https://img-blog.csdn.net/20171124100021627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [LightGBM] [Warning] boost::filesystem::create_directory: No such file or direct                                      ory

> [LightGBM] [Warning] boost::filesystem::create_directory: No such file or direct                                      ory

> [LightGBM] [Warning] boost::filesystem::create_directory: No such file or direct                                      ory

> [LightGBM] [Warning] boost::filesystem::create_directory: No such file or direct                                      ory

> [LightGBM] [Warning] boost::filesystem::create_directory: No such file or direct                                      ory

> [LightGBM] [Warning] boost::filesystem::create_directory: No such file or direct                                      ory

> [LightGBM] [Warning] boost::filesystem::create_directory: No such file or direct                                      ory

> [LightGBM] [Warning] boost::filesystem::create_directory: No such file or direct                                      ory

> terminate called without an active exception

> Aborted (core dumped)

> 解决方法

> 后来发现，源文件目录：src/treelearner/gpu_tree_learner.h的第26行代码：

> \#
> define
> BOOST_COMPUTE_USE_OFFLINE_CACHE
> 如果包含这个这个宏 BOOST_COMPUTE_USE_OFFLINE_CACHE，就会导致要寻找缓存目录，因此将其注释，并clean然后重新编译源文件，然后就能安装使用了。为了让大家能直接使用lightgbm，特将编译好的gpu版本的lightgbm上传。安装方法。

> $
> cd python-package
> $
> python setup.py install
> 下载地址：
> [传送门](http://download.csdn.net/download/u010665216/10130925)


