
# 极简代码 —— list 最小最大索引（argmax-argmin）的实现 - Zhang's Wikipedia - CSDN博客


2019年02月11日 22:59:47[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：179


`def argmin(lst):
	return min(range(len(lst)), key=lst.__getitem__)
def argmax(lst):
	return max(range(len(lst)), key=lst.__getitem__)`

