# 【Python】Jupyter Notebook - zj360202的专栏 - CSDN博客





2017年11月29日 13:48:20[zj360202](https://me.csdn.net/zj360202)阅读数：243标签：[python																[jupyter](https://so.csdn.net/so/search/s.do?q=jupyter&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/zj360202/article/category/1298884)








1.安装

```python
pip install jupyter
```

2. 切换工作空间



2.1 找到配置文件路径

jupyter
 notebook --generate-config

2.2 修改配置文件

c.NotebookApp.notebook_dir
 = u'' --> c.NotebookApp.notebook_dir = 'd:\workspace\jupyter'

3. 运行jupyter

双击：{pythonhome}\python27\Scripts\jupyter-notebook.exe

4. 新建一个新的脚本运行环境



```python
#添加python3的支持可以使用
pip3 install ipykernel
ipython3 kernelspec install-self
```



![](https://img-blog.csdn.net/20171129134729665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5. 脚本快捷键


- 

执行当前cell，并自动跳到下一个cell：`Shift Enter`

- 

执行当前cell，执行后不自动调转到下一个cell：`Ctrl-Enter`

- 

是当前的cell进入编辑模式：`Enter`

- 

退出当前cell的编辑模式：`Esc`

- 

删除当前的cell：双D

- 

进入下一个cell：双A （前面）或 Alt+Enter（后面）

- 

为当前的cell加入line number：单L

- 

将当前的cell转化为具有一级标题的maskdown：单1

- 

将当前的cell转化为具有二级标题的maskdown：单2

- 

将当前的cell转化为具有三级标题的maskdown：单3

- 

为一行或者多行添加/取消注释：`Crtl /`

- 

撤销对某个cell的删除：`z`

- 

浏览器的各个Tab之间切换：`Crtl PgUp`和
```
Crtl
 PgDn
```


- 

快速跳转到首个cell：`Crtl Home`

- 

快速跳转到最后一个cell：`Crtl End`

- 
`home 一行的开始`

- 
`end 一行的结束`

- 
`ctrl + ?  注释`

- 
`shift + tab 查询函数的注解`












