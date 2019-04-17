# Spyder 自动补全 加载模块 - Snoopy_Dream - CSDN博客





2018年10月19日 12:03:00[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：417标签：[spyder																[自动补齐](https://so.csdn.net/so/search/s.do?q=自动补齐&t=blog)](https://so.csdn.net/so/search/s.do?q=spyder&t=blog)
个人分类：[Python](https://blog.csdn.net/e01528/article/category/7430266)








1.删除环境中的enum包，pip uninstall enum     或 pip uninstall enum34

2.找到 module_completion.py 文件，位于目录usr/lib/python2.7/dist-packages/spyderlib/utils/introspection/。将cv和cv2添加到 module_completion.py 文件第274行的 mods 变量中，保存文件。

【强制修改只读文件】在终端下面vim module_completion.py，然后点击o开始插入修改文件，最后按Esc，输入:w !sudo tee %

![](https://img-blog.csdn.net/20181019115935866?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.修改后，删除同目录下的 module_completion.pyc 文件，使得修改后的 module_completion.py 文件生效。

4.进入用户目录下的Users/用户名/.spyder2(spyder3)/db，删除目录中的submodules文件。

5.终端下spyder reset



[主要参考：](https://blog.csdn.net/u014539580/article/details/79489001?utm_source=blogxgwz2)

[https://blog.csdn.net/u014539580/article/details/79489001?utm_source=blogxgwz2](https://blog.csdn.net/u014539580/article/details/79489001?utm_source=blogxgwz2)

[https://blog.csdn.net/slibra_L/article/details/78779717](https://blog.csdn.net/slibra_L/article/details/78779717)








