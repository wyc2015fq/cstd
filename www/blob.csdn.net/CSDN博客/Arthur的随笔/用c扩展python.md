# 用c扩展python - Arthur的随笔 - CSDN博客
2011年12月14日 00:06:33[largetalk](https://me.csdn.net/largetalk)阅读数：1055
搞了一晚上终于做了一个demo，也会用distutils做一个包了，不过现在太晚了，先睡觉，明天好好做下笔记;
写一个简单得python c扩展一共就那么几步，
1。 编写c函数
```cpp
int math_fib(int n)
{
    if (n<=2)
        return 1;
    return math_fib(n-1) + math_fib(n-2);
}
```
2. 编写一个包装函数
```cpp
static PyObject*
spam_fib(PyObject *self, PyObject *args)
{
    int n, result;
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;
    result = math_fib(n);
    return Py_BuildValue("i", result);
}
```
3. 模块函数数组
```cpp
static PyMethodDef SpamMethods[] = {
    {"system", spam_system, METH_VARARGS, "exec a shell command"},
    {"fib",    spam_fib,    METH_VARARGS, "cal fibonacci number"},
    {NULL, NULL, 0, NULL}
};
```
4. 模块初始化函数
```cpp
PyMODINIT_FUNC
initspam(void)
{
    PyObject *m;
    m = Py_InitModule("spam", SpamMethods);
    if (m==NULL)
        return;
    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    Py_INCREF(SpamError);
    PyModule_AddObject(m, "error", SpamError);
}
```
这样c扩展就写好了，现在是将其编译成动态连接库，python提供了一个很好得工具distutils, 就不需要自己写makefile 手动编译了
5.编写setup.py
```python
from distutils.core import setup, Extension
module1 = Extension('spam',
                    sources = ['spammodule.c'])
setup (name = 'spam',
       version = '1.0',
       description = 'this is a demo package',
       ext_modules = [module1],
       author = 'Arthur')
```
6. python setup.py build #编译
    python setup.py install #安装
    python setup.py sdist #将源码打包
