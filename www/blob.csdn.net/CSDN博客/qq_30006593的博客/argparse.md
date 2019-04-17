# argparse - qq_30006593的博客 - CSDN博客





2017年09月07日 18:05:26[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：145标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/qq_30006593/article/category/7099907)








- import argparse
- parser = argparse.ArgumentParser(description=”your script description”)
- parser.add_argument(‘–lr’, default=0.1) 

这里lr前面如果不加-或者–，则必须输入。 

default=必须有- args = parser.parse_args()
- print args.lr   




