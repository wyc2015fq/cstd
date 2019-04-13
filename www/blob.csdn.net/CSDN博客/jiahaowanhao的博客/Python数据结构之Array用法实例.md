
# Python数据结构之Array用法实例 - jiahaowanhao的博客 - CSDN博客


2018年07月15日 22:16:12[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：127


[Python数据结构之Array用法实例](http://cda.pinggu.org/view/26100.html)
这篇文章主要介绍了Python数据结构之Array用法实例,较为详细的讲述了Array的常见用法,具有很好的参考借鉴价值,需要的朋友可以参考下
import ctypes
class Array:
def __init__(self, size):
assert size > 0, "Array size must be > 0 "
self._size = size
pyArrayType = ctypes.py_object * size
self._elements = pyArrayType()
self.clear(None)
def clear(self, value):
for index in range(len(self)):
self._elements[index] = value
def __len__(self):
return self._size
def __getitem__(self, index):
assert index >= 0 and index < len(self), "index must >=0 and <= size"
return self._elements[index]
def __setitem__(self, index, value):
assert index >= 0 and index < len(self), "index must >=0 and <= size"
self._elements[index] = value
def __iter__(self):
return _ArrayIterator(self._elements)
class _ArrayIterator:
def __init__(self, theArray):
self._arrayRef = theArray
self._curNdr = 0
def __next__(self):
if self._curNdr < len(theArray):
entry = self._arrayRef[self._curNdr]
sllf._curNdr += 1
return entry
else:
raise StopIteration
def __iter__(self):
return self
class Array2D :
def __init__(self, numRows, numCols):
self._theRows = Array(numCols)
for i in range(numCols):
self._theRows[i] = Array(numCols)
def numRows(self):
return len(self._theRows)
def numCols(self):
return len(self._theRows[0])
def clear(self, value):
for row in range(self.numRows):
self._theRows[row].clear(value)
def __getitem__(self, ndxTuple):
assert len(ndxTuple) == 2, "the tuple must 2"
row = ndxTuple[0]
col = ndxTuple[1]
assert row>=0 and row <len(self.numRows()) \
and col>=0 and col<len(self.numCols), \
"array subscrpt out of range"
theArray = self._theRows[row]
return theArray[col]
def __setitem__(self, ndxTuple, value):
assert len(ndxTuple)==2, "the tuple must 2"
row = ndxTuple[0]
col = ndxTuple[1]
assert row >= 0 and row < len(self.numRows) \
and col >= 0 and col < len(self.numCols), \
"row and col is invalidate"
theArray = self._theRows[row];
theArray[col] = value
希望本文所述对大家的Python程序设计有所帮助。

