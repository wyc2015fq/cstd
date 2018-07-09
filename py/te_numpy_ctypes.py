import ctypes as ct
import numpy as np

libc = ct.cdll.msvcrt
qsort = libc.qsort
# 定义比较函数类：
CMPFUNC = ct.CFUNCTYPE(ct.c_int, ct.POINTER(ct.c_int), ct.POINTER(ct.c_int))
# 设置一下qsort的参数和返回值：
qsort.restype = None

qsort.argtypes = [ct.c_void_p, ct.c_size_t, ct.c_size_t, CMPFUNC]
# 定义比较函数：
def py_cmp_func(a, b):
    print('py_cmp_func', a[0], b[0])
    return a[0]-b[0]

# 生成可以被C调用的函数对象：
cmp_func = CMPFUNC(py_cmp_func)
# 创建一个Numpy的array对象：
arr = np.array([5, 1, 7, 33, 99], dtype=int)
# 用来自MSVCRT的qsort对Numpy array进行排序：
qsort(arr.ctypes.data, len(arr), ct.sizeof(ct.c_int), cmp_func)
print(arr)

