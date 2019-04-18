# pd.read_csv()报'utf-8' codec can't decode bytes in position 20-21: invalid continuation byte异常的处理 - 技术分享 - CSDN博客

2018年12月09日 17:11:46[丿一叶秋丶](https://me.csdn.net/zhuqiang9607)阅读数：1161


### 原因

csv中的编码格式是‘ANSI’，而Python 3默认读取格式为‘utf-8’，导致decode异常，不过编码格式也有可能是其它格式，反正不是‘utf-8’，所以要对症下药。本文提供两种解决方式，均已尝试过。

### 方案1：

**pd.read_csv(‘test.csv’,encoding=‘ANSI’)**，尝试过pd.read_csv(‘test.csv’,encoding=‘utf-8’)，还是会报异常’utf-8’ codec can’t decode bytes in position 20-21: invalid continuation byte

### 方案2：

**pd.read_csv(‘test.csv’)** ，不过需要先找到csv文件$\rightarrow$右键记事本打开$\rightarrow$找到菜单“文件”$\rightarrow$另存为$\rightarrow$修改编码格式为UTF-8才行
![修改编码格式为UTF-8](https://img-blog.csdnimg.cn/20181209165951831.)

网上还有一种方案是打开csv文件，另存为，选择其他格式，右下角工具中选择web选项，编码中的格式选择Unicode（UTF-8），**然而多次尝试之后并没有效果**，有兴趣的都可以试一下

