
# python函数返回多个值的示例方法 - jiahaowanhao的博客 - CSDN博客


2018年01月24日 21:13:01[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1928


[python函数返回多个值的示例方法](http://cda.pinggu.org/view/24552.html)
python可以返回多个值，确实挺方便
函数里的return只能返回一个值，但是返回类型是没是限制的
因此，我们可以“返回一个 tuple类型，来间接达到返回多个值”。
例子是我在robot framework source code 时的例子:
复制代码代码如下:
def __init__(self, cells):
self.cells, self.comments = self._parse(cells)def _parse(self, row):
data = []
comments = []
for cell in row:
cell = self._collapse_whitespace(cell)
if cell.startswith('\#') and not comments:
comments.append(cell[1:])
elif comments:
comments.append(cell)
else:
data.append(cell)
return self._purge_empty_cells(data), self._purge_empty_cells(comments)
当时__init__是类的构造函数，他将得到有_parse解析回来的多个返回值，self._purge_empty_cells(data) 赋给self.cells, self._purge_empty_cells(comments)赋给self.comments
就这么简单 ：）


