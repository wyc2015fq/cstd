# jupyter安装与卸载 - Keith - CSDN博客





2017年01月21日 09:15:51[ke1th](https://me.csdn.net/u012436149)阅读数：11354
个人分类：[python](https://blog.csdn.net/u012436149/article/category/6462204)









# jupyter使用笔记

## 安装与卸载

**虚拟环境安装 virtualenvwrapper**

```python
~/ workon virenv
(virenv)~/ pip install jupyter
(virenv)~/ jupyter notebook #打开
```

**卸载**

使用`pip uninstall jupyter`是卸不掉`jupyter`的

```python
(virenv)~/ pip install pip-autoremove
(virenv)~/ pip-autoremove jupyter -y
```

**pip-autoremove会卸载掉package和无用的依赖**



