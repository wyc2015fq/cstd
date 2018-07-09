import ctypes as ct
import numpy as np
import platform
from ctypes import *



cstd = CDLL('./cstdlib.dll')

add_int = cstd.add_int
# 定义比较函数类：
# CMPFUNC = ct.CFUNCTYPE(ct.c_int, ct.POINTER(ct.c_int), ct.POINTER(ct.c_int))
# 设置一下qsort的参数和返回值：
dir(ct)
add_int.restype = ct.c_int
add_int.argtypes = [ct.c_int, ct.c_int]
# 定义比较函数：

a = add_int(1, 2)
print(a)


