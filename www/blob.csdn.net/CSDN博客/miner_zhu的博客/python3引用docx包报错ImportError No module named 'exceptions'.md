# python3引用docx包报错ImportError: No module named 'exceptions' - miner_zhu的博客 - CSDN博客





2018年08月24日 13:47:16[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：314








**使用环境**：

ubuntu anaconda python3.6

**原因**：

docx包中引用了该模块，而python3.x版本移除了exceptions模块。即docx包尚未适配python3。

**解决方法**：

1.命令行 卸载docx，`pip uninstall docx`

2.下载 `python_docx-0.8.7-py2.py3-none-any.whl` 网址：[http://www.lfd.uci.edu/~gohlke/pythonlibs/](http://www.lfd.uci.edu/~gohlke/pythonlibs/)

3. 命令行输入`pip install python_docx-0.8.7-py2.py3-none-any.whl`使用新的docx包



