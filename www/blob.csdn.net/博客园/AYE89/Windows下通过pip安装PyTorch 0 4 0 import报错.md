
# Windows下通过pip安装PyTorch 0.4.0 import报错 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Windows下通过pip安装PyTorch 0.4.0 import报错](https://www.cnblogs.com/eniac1946/p/9210142.html)
|Posted on|2018-06-21 17:28|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9210142)|[收藏](#)
|问题：通过pip安装PyTorch 0.4.0成功，但是import时报错。
|import torch
|File "D:\Python\Python36\lib\site-packages\torch\__init__.py", line 78, in <module>
|from torch._C import *
|ImportError: DLL load failed: 找不到指定的模块。
|解决：
|Download|[win-64/intel-openmp-2018.0.0-8.tar.bz2](https://anaconda.org/anaconda/intel-openmp/files)
|put the dll files in Library\bin into a dir
|append the path of it to the environment variable PATH.
|参考：
|[from torch._C import * (ImportError: DLL load failed: The specified module could not be found.](https://github.com/pytorch/pytorch/issues/4518)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
