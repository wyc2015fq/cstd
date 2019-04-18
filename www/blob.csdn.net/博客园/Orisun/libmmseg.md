# libmmseg - Orisun - 博客园







# [libmmseg](https://www.cnblogs.com/zhangchaoyang/articles/2040078.html)






libmmseg主要被作者用来实现Sphinx全文检索软件的中文分词功能，因此作者给Sphinx提供了一个补丁文件，可以让Sphinx集成libmmseg，从而支持对于中文文章的全文检索功能。

安装mmseg同样是./configure      make        sudo make install

但是注意把改一下src/css/UnigramCorpusReader.cpp文件，加一行#include <string.h>，否则make时会报错：error: 'strncmp' was not declared in this scope












