# Windows和Ubuntu环境下修改Jupyter Notebook默认文件路径 - 技术分享 - CSDN博客

2018年11月15日 11:40:34[丿一叶秋丶](https://me.csdn.net/zhuqiang9607)阅读数：1114


本文有两部分，分别介绍在Windows和Ubuntu环境下，如何修改Jupyter Notebook默认文件路径。1）Windows环境中有三个步骤，而网上多数答案只提供两步，结果保存的文件还在原来的路径，根本不起作用；2）Ubuntu比较简单，两步OK。回归正题，具体修改方法如下：

# Windows环境

### 1、添加jupyter_notebook_config.py配置文件

打开Windows运行框，方式有：1）windows+R快捷键；2）“开始”菜单中搜索“运行”；3） “开始”菜单，“附件”中点击“运行”；

然后输入cmd指令（或“开始”菜单，“附件”中点击“命令提示符”）

命令行中输入 **jupyter notebook  --generate-config**
![jupyter notebook  --generate-config](https://img-blog.csdnimg.cn/20181115105055803.)

由于已经配置过了，会弹出Overwrite，否则会直接在**C:\Users\Administrator.jupyter** 下产生一个配置文件**jupyter_notebook_config.py**

### 2、修改jupyter_notebook_config.py配置文件

在对应目录找到jupyter_notebook_config.py文件，用记事本打开文件，直接点击“ctrl+F”去查找**c.NotebookApp.notebook_dir**，原来是以下这种样式：

```
## The directory to use for notebooks and kernels.
#c.NotebookApp.notebook_dir = ' '
```

在’ ‘中添加需要保存的路径，用双斜杠\分割路径（单斜杠\也可以，这是在Windows环境下），且删除最前面的#，重要的一点要在’ '前面加个u，示例如下：

```
## The directory to use for notebooks and kernels.
c.NotebookApp.notebook_dir = u'E:\\Jupyter_NoteBook'
```

一般就说到前面两个步骤，结果一试和原来一样，因为还有下一步

### 3、 修改Jupyter Notebook属性中的目标

“开始”菜单的Anaconda文件中找到Jupyter Notebook，右键，点击“属性”，删除目标中的%USERPROFILE%，不行的话再添加要存储的位置“E:\Jupyter_NoteBook”（干脆直接加了），示例如下：
![修改Jupyter Notebook属性中的目标](https://img-blog.csdnimg.cn/20181115113155625.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXFpYW5nOTYwNw==,size_16,color_FFFFFF,t_70)

操作完上面三个步骤，就搞定了

# Ubuntu环境

### 1、添加jupyter_notebook_config.py配置文件

打开Ubuntu终端，方式：Alt+Ctrl+T快捷键，或者其它进入终端的方式，命令行中输入 **jupyter notebook  --generate-config**，然后会生成类似/home/user/.jupyter/jupyter_notebook_config.py的文件

### 2、修改jupyter_notebook_config.py配置文件

终端中输入：**sudo gedit /home/user/.jupyter/jupyter_notebook_config.py**，找到：

```
## 用于笔记本和内核的目录。
#c.NotebookApp.notebook_dir = ''
```

在’ '中添加需要保存的路径，并去掉#，例如：

```
## 用于笔记本和内核的目录。
c.NotebookApp.notebook_dir = '/home/user/JupyterNotebook'
```

需要在对应的目录下添加JupyterNotebook文件夹，否则报错

参考：[How to change the Jupyter start-up folder](https://stackoverflow.com/questions/35254852/how-to-change-the-jupyter-start-up-folder)

