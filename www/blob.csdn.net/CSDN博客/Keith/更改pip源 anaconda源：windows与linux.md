# 更改pip源/anaconda源：windows与linux - Keith - CSDN博客





2017年03月27日 18:21:02[ke1th](https://me.csdn.net/u012436149)阅读数：12061








# 更改pip源

## windows

在 `c:\user\username\pip\pip.ini`中加入

```python
[global]
index-url=https://pypi.tuna.tsinghua.edu.cn/simple 
[install]  
trusted-host=pypi.tuna.tsinghua.edu.cn
disable-pip-version-check = true  
timeout = 6000
```

需要创建`pip文件夹与pip.ini文件`。

## linux（ubuntu）

```python
cd $HOME  
mkdir .pip  
cd .pip
sudo vim pip.conf  

在里面添加  
[global]  
index-url=https://pypi.tuna.tsinghua.edu.cn/simple
[install]  
trusted-host=pypi.tuna.tsinghua.edu.cn 
disable-pip-version-check = true  
timeout = 6000
```

# 更改anaconda源

```python
conda config --add channels 'https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/'
conda config --set show_channel_urls yes
#好了，这次可以开心的下载东西了
# 如何删除添加的源呢？
# conda config --remove channels 'https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/' 
# 看看当前的 cofig 是什么样的
conda config --show
```



