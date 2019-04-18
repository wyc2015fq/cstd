# Ubuntu 配置pip国内镜像源加速安装 - YZXnuaa的博客 - CSDN博客
2018年03月27日 11:36:46[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2164
- [问题描述](https://blog.csdn.net/yucicheung/article/details/79095742#%E9%97%AE%E9%A2%98%E6%8F%8F%E8%BF%B0)
- [国内镜像源列表](https://blog.csdn.net/yucicheung/article/details/79095742#%E5%9B%BD%E5%86%85%E9%95%9C%E5%83%8F%E6%BA%90%E5%88%97%E8%A1%A8)
- [永久置换pip镜像源](https://blog.csdn.net/yucicheung/article/details/79095742#%E6%B0%B8%E4%B9%85%E7%BD%AE%E6%8D%A2pip%E9%95%9C%E5%83%8F%E6%BA%90)- [1. 创建pip.conf文件](https://blog.csdn.net/yucicheung/article/details/79095742#1-%E5%88%9B%E5%BB%BApipconf%E6%96%87%E4%BB%B6)
- [2. 编辑pip.conf文件](https://blog.csdn.net/yucicheung/article/details/79095742#2-%E7%BC%96%E8%BE%91pipconf%E6%96%87%E4%BB%B6)
- [3. 测试速度](https://blog.csdn.net/yucicheung/article/details/79095742#3-%E6%B5%8B%E8%AF%95%E9%80%9F%E5%BA%A6)
- [暂时置换镜像源](https://blog.csdn.net/yucicheung/article/details/79095742#%E6%9A%82%E6%97%B6%E7%BD%AE%E6%8D%A2%E9%95%9C%E5%83%8F%E6%BA%90)
# 问题描述
基于国内网速的问题,我们直接pip安装包通常速度非常慢,而且经常会出现装到一半失败了的问题,既然这样,我们就要充分利用国内镜像的力量,节省时间,明显提高pip安装的效率.以下是我的操作分享,一共分为永久置换和暂时置换两种方法.
# 国内镜像源列表
**豆瓣(douban) [http://pypi.douban.com/simple/](http://pypi.douban.com/simple/) (推荐)**
清华大学 [https://pypi.tuna.tsinghua.edu.cn/simple/](https://pypi.tuna.tsinghua.edu.cn/simple/)
阿里云 [http://mirrors.aliyun.com/pypi/simple/](http://mirrors.aliyun.com/pypi/simple/)
中国科技大学 [https://pypi.mirrors.ustc.edu.cn/simple/](https://pypi.mirrors.ustc.edu.cn/simple/)
中国科学技术大学 [http://pypi.mirrors.ustc.edu.cn/simple/](http://pypi.mirrors.ustc.edu.cn/simple/)
**我们推荐豆瓣/douban镜像源,以下的所有示例也都会以豆瓣镜像源为例.**
# 永久置换pip镜像源
## 1. 创建pip.conf文件
运行以下命令:
```bash
cd ~/.pip
```
- 1
如果提示目录不存在的话,我们要自行创建一个,再进入目录
```
mkdir ~/.pip
cd ~/.pip
```
- 1
- 2
在.pip目录下创建一个pip.conf文件
`touch pip.conf`- 1
文件就创建好了(当然如果你已经有这个文件了这步可以跳过).
## 2. 编辑pip.conf文件
`sudo gedit ~/.pip/pip.conf`- 1
打开pip.conf文件窗口,将以下内容复制到文件中:
> 
[global] 
  index-url = [http://pypi.douban.com/simple](http://pypi.douban.com/simple)
  [install] 
  trusted-host=pypi.douban.com
这样就替换成功了,当然你也可以选择其他镜像源.
## 3. 测试速度
尝试安装django库,运行
`pip install django`- 1
终端反馈内容为
> 
Collecting django 
    Downloading [http://pypi.doubanio.com/packages/87/9f/4ec8b197d83666fddd2398842024c5341ee7d40bbec6aee9705d1ad22f13/Django-2.0.tar.gz](http://pypi.doubanio.com/packages/87/9f/4ec8b197d83666fddd2398842024c5341ee7d40bbec6aee9705d1ad22f13/Django-2.0.tar.gz) (8.0MB) 
      100% |████████████████████████████████| 8.0MB 4.3MB/s 
可以看到下载速度是非常快的,这就是镜像的力量.
# 暂时置换镜像源
同样以豆瓣镜像源为例,接下来我们安装pygame包,你可以替换成你想安装的其他包的名字.
`pip install pygame -i http://pypi.douban.com/simple`- 1
这步如果出错,请将命令变换为
`pip install pygame -i http://pypi.douban.com/simple --trusted-host pypi.douban.com`- 1
同样可以超迅速成功安装:
> 
Collecting pygame 
    Downloading [http://pypi.doubanio.com/packages/dc/76/fdfb5cee3432192a89df6aad8a33eed96ba6a3a16e0dca951db9fef03688/pygame-1.9.3-cp27-cp27mu-manylinux1_x86_64.whl](http://pypi.doubanio.com/packages/dc/76/fdfb5cee3432192a89df6aad8a33eed96ba6a3a16e0dca951db9fef03688/pygame-1.9.3-cp27-cp27mu-manylinux1_x86_64.whl) (9.4MB) 
      100% |████████████████████████████████| 9.4MB 3.8MB/s  
  Installing collected packages: pygame 
  Successfully installed pygame-1.9.3
