# vs2015 c++调用python - zj360202的专栏 - CSDN博客





2018年01月30日 16:57:47[zj360202](https://me.csdn.net/zj360202)阅读数：403








hello_world.py



```python
# -*- encoding:utf-8 -*-

def hello():
    print("hello world")

def add(a, b):
    return a + b
```
c++



```cpp
#include<Python.h>

int main() {
	Py_Initialize();
	PyObject *pModule = NULL;
	PyObject *pFunc = NULL;

	pModule = PyImport_ImportModule("hello_world");//调用的文件名称
	pFunc = PyObject_GetAttrString(pModule, "hello");//调用方法名称
	
	PyEval_CallObject(pFunc, NULL);
	Py_Finalize();
	system("pause");
	return 0;
}
```
这里python用的是3.5

需要在vs中项目属性里面配置

![](https://img-blog.csdn.net/20180130165539790?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后直接生成项目

生成完了exe，将py文件复制到exe目录下面，点击exe就可以运行了

![](https://img-blog.csdn.net/20180130165716323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





