# 读取文件，取出第一行出现\ufeff问题(编码问题) - sxf_123456的博客 - CSDN博客
2018年03月30日 20:55:29[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1791
                读取文件，取出第一行出现\ufeff问题(编码问题)
解决办法:
codecs.open(dict_file, "r", encoding="utf-8-sig")
