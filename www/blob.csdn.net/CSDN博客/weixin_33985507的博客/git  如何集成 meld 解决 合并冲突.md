# git : 如何集成 meld 解决 合并冲突 - weixin_33985507的博客 - CSDN博客
2017年01月12日 14:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
#### 1.meld工具
###### Meld 是一个可视化的文本差异比较工具，它可以跟多个版本控制系统集成
1.1: 下载meld
> 
下载地址: [http://meldmerge.org/](https://link.jianshu.com?t=http://meldmerge.org/)
1.2: 修改.gitconfig文件
> 
.gitconfig文件的位置在 C:\Users\你的用户名\ 下
例如我的.gitconfig在C:\Users\Lenovo下
修改值
```
[merge]
    tool = Meld
```
如果没有增加即可
在末尾加上,意思是不生成临时文件,在比较文本的时候会产生历史文件,这里写false 将不产生
```
[mergetool]
    keepBackup = false
```
1.3: 加入环境变量
> 
环境变量在
我的电脑 -> 属性 -> 高级系统设置 -> 环境变量
找到 PATH 大小写无所谓
在最后加上你的meld路径
我的meld安装在 E:\git_meld,所以我加上
> 
E:\git_meld;
1.4 打开git bash
键入命令
```
git mergetool
```
如图所示则配置完成
![4241895-890c0adde64c6bd6.png](https://upload-images.jianshu.io/upload_images/4241895-890c0adde64c6bd6.png)
git
当你的文件出现了冲突,可以用
```
git mergetool
```
来调出meld了
