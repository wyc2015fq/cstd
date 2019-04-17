# pip和conda安装源更改 - 快来学习鸭～～～ - CSDN博客





2017年01月05日 11:55:14[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：17782








python模块安装，使用国内源可以提高下载速度。

## pip源更改：

pip源有好几个，我一直用的清华的pip源，它5分钟同步一次。 

临时使用： 

pip 后加参数 `-i https://pypi.tuna.tsinghua.edu.cn/simple`

例：pip install -i [https://pypi.tuna.tsinghua.edu.cn/simple](https://pypi.tuna.tsinghua.edu.cn/simple) pandas  

永久使用： 
**Linux下：**

    修改 ~/.pip/pip.conf (没有就创建一个)， 修改 index-url至tuna，内容如下：
```
[global]
    index-url = https://pypi.tuna.tsinghua.edu.cn/simple
```

**windows下:**

直接在user目录中创建一个pip目录，如：C:\Users\xxxx\pip，新建文件pip.ini，内容如下

```
[global]
 index-url = https://pypi.tuna.tsinghua.edu.cn/simple
```

## conda源更改：

conda源国内只有清华有， 

修改源只需输入如下两行命令：

```
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
    conda config --set show_channel_urls yes
```

就可以了。 

可以随便测试下速度，conda install pandas




