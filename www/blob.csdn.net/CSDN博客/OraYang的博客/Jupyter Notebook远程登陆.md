
# Jupyter Notebook远程登陆 - OraYang的博客 - CSDN博客

2018年01月04日 18:51:04[OraYang](https://me.csdn.net/u010665216)阅读数：506所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



# 引言
最近买了台性能比较好的主机，准备在上面跑跑算法之类的，本来想做个利用tensorflow实现word2vec的demo，但是我一直在笔记本上办公，笔记本上有没有安装tensorflow环境，于是我配置了主机jupyter notebook环境，从而实现了远程访问jupyter-notebook并编码的功能。现总结如下
# 登陆远程服务器
这里我设置了SSH登陆
# 生成配置文件
```python
$
```
```python
jupyter
```
```python
notebook
```
```python
-
```
```python
-
```
```python
generate
```
```python
-
```
```python
config
```
这句命令会在用户目录下面生成’.jupyter’文件夹，里面试jupyter相关的配置文件
![这里写图片描述](https://img-blog.csdn.net/20180104184110174?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 生成密码
这里我们直接打开ipython
在里面生成密码：
```python
In
```
```python
[
```
```python
1
```
```python
]: from notebook.auth import passwd
```
```python
In
```
```python
[
```
```python
2
```
```python
]: passwd()
```
```python
Enter
```
```python
password: 
Verify password:
```
```python
Out
```
```python
[
```
```python
2
```
```python
]:
```
```python
'XXX'
```
并复制out[2]中的内容
![这里写图片描述](https://img-blog.csdn.net/20180104184349935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 修改默认配置文件
```python
$
```
```python
vim ~
```
```python
/.jupyter/jupyter
```
```python
_notebook_config.py
```
进行如下修改：
> c.NotebookApp.ip=’*’

> c.NotebookApp.password = ‘xxx’\#粘贴[out2]中的内容

> c.NotebookApp.open_browser = False

> c.NotebookApp.port =8888 \#随便指定一个端口
![这里写图片描述](https://img-blog.csdn.net/20180104184642618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 启动jupyter notebook

> $
> jupyter notebook
> 直接访问

> 此时应该可以直接从本地笔记本的浏览器直接访问
> [http://address_of_remote:8888](http://address_of_remote:8888)
> 就可以看到jupyter的登陆界面。
![这里写图片描述](https://img-blog.csdn.net/20180104184939471?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20180104184939471?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 输入设置的密码然后登陆：
![这里写图片描述](https://img-blog.csdn.net/20180104185026386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> [
](https://img-blog.csdn.net/20180104184939471?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
