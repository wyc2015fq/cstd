# 【python】Python + C/C++ 嵌入式编程（1）：多维数组Numpy.Array()在Python和C/C++文件间的传递问题 - zj360202的专栏 - CSDN博客





2018年01月31日 10:48:47[zj360202](https://me.csdn.net/zj360202)阅读数：1205









Python 提供了丰富的 C API 函数，我们使用这些 C API 函数可以实现将 Python 文件中的函数、类等在 C/C++ 文件中进行调用，从而使得我们可以方便地使用 Python 代码来帮助我们实现一些额外的需求（如：嵌入神经网络模型）。


网上已经有很多介绍如何将 Python 嵌入到 C/C++ 的博客，这里不再累述。这里主要叙述一下如何实现多维数组在 Python 文件和 C/C++文件间互传，即如何从 Python 文件中返回 Numpy 数组，已经如何从 C/C++文件中传递一个多维数组到Python文件中。在处理这个的过程中，遇到了许多的困难，查阅了许多网站，发现中文博客中对这部分的相信介绍不是很多，故特此写这篇论文（第一篇博文，不详细的地方望评论中指出，有问题也欢迎在评论中咨询）。

#### **博文目录**：
- **从 Python 文件中的函数返回 List 数组**
- **从 Python 文件中的函数返回包含 Numpy Array 的 List 数组**
- **向 Python 文件中的函数传递 List 数组**
- **向 Python 文件中的函数传递 Numpy Array 数组**

#### **从 Python 文件中的函数返回 List 数组:**


如果我们在 C/C++ 文件中调用的 Python 函数返回的是 List 数组，那么我们这里主要用到的是 Python C API 中的 List Object 来处理返回的数据，主要用到 List Object 里面的这些函数：
- int PyList_Check（PyObject *list）函数判断一个 PyObject 指针对象是不是一个 List 对象；
- Py_ssize_t PyList_Size (PyObject *list) 函数计算一个 List 对象的大小；
- PyObject* PyList_GetItem(PyObject *list, Py_ssize_t index) 函数返回 List对象中第 index 个元素的 PyObject 指针。


示例：假设我们有这么一个Python函数：



```python
def IntegerListReturn():

    IntegerList = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

    return IntegerList
```


我们在 C/C++ 文件中调用这个 Python 函数时，将返回 List 对象，那么我们将在 C/C++文件中进行如下的接收操作：



```python
// some code omitted...
 cout<<"Integer List Show:"<<endl;
 PyObject *pFuncTwo     = PyDict_GetItemString(pDict, "IntegerListReturn");
 PyObject *FuncTwoBack  = PyObject_CallObject (pFuncTwo, nullptr);//返回List对象

 if(PyList_Check(FuncTwoBack)){ //检查是否为List对象

     int SizeOfList = PyList_Size(FuncTwoBack);//List对象的大小，这里SizeOfList = 3
     for(Index_i = 0; Index_i < SizeOfList; Index_i++){

         PyObject *ListItem = PyList_GetItem(FuncTwoBack, Index_i);//获取List对象中的每一个元素

         int NumOfItems = PyList_Size(ListItem);//List对象子元素的大小，这里NumOfItems = 3

         for(Index_k = 0; Index_k < NumOfItems; Index_k++){

             PyObject *Item = PyList_GetItem(ListItem, Index_k);//遍历List对象中子元素中的每个元素

             cout << PyInt_AsLong(Item) <<" "; //输出元素

             Py_DECREF(Item); //释放空间
         }

         Py_DECREF(ListItem); //释放空间
     }
     cout<<endl;

 }else{

     cout<<"Not a List"<<endl;
 }
 // some code omitted...
```

#### **从 Python 文件中的函数返回包含 Numpy Array 的 List 数组**


带有 Numpy Array 的 List 数组，除了上述提到的 List Object 操作函数，这里还需要用到 PyArrayObject 这个对象来处理返回的 Numpy Array。先介绍一下PyArrayObject, 这个 C API 模块来自 Numpy Module 中，所以使用这个 C API 模块前需要进行一些初始化操作：



```python
// some code omitted...
#include <numpy/arrayobject.h> //包含 numpy 中的头文件arrayobject.h
using namespace std;

void init_numpy(){//初始化 numpy 执行环境，主要是导入包，python2.7用void返回类型，python3.0以上用int返回类型

    import_array();
}
int main()
{

    Py_Initialize();
    init_numpy();
    // some code omitted...
```


做完初始化后，我们就可以使用 PyArrayObject 对象。先对PyArrayObject 对象做一个简单的介绍。PyArrayObject 实际上是一个结构体，结构体内包含四个元素，用来访问 Numpy Array 中的数据：
- int nd：Numpy Array数组的维度。
- int *dimensions ：Numpy Array 数组每一维度数据的个数。
- int *strides：Numpy Array 数组每一维度的步长。
- char *data： Numpy Array 中指向数据的头指针。


所以当我们要访问 PyArrayObject 对象中的数据时，有:



```python
//对于二维 Numpy Array 数组，我们访问[i, j]位置处的元素的值
 PyArrayObject *array
 array->data + i*array->strides[0] + j*array->strides[1]
```


知道如何访问PyArrayObject对象中的数据后，这里给出简单的示例:



```python
#假设我们有这么一段 python 代码：
def ArrayListReturn():

    ArrList = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

    Array_A  = np.asarray(ArrList, dtype='float' )
    Array_B  = np.asarray(ArrList, dtype='double')

    return [Array_A, Array_B]
```


那么我们在 C/C++中做如下的访问：



```python
/*Return the List which contains Numpy Array*/
PyObject *pFuncOne    = PyDict_GetItemString(pDict, "ArrayListReturn");

PyObject *FuncOneBack = PyObject_CallObject(pFuncOne, nullptr);

int Index_i = 0, Index_k = 0, Index_m = 0, Index_n = 0;
if(PyList_Check(FuncOneBack)){

    int SizeOfList = PyList_Size(FuncOneBack);

    for(Index_i = 0; Index_i < SizeOfList; Index_i++){

        PyArrayObject *ListItem = (PyArrayObject *)PyList_GetItem(FuncOneBack, Index_i);//读取List中的PyArrayObject对象，这里需要进行强制转换。

        int Rows = ListItem->dimensions[0], columns = ListItem->dimensions[1];
        cout<<"The "<<Index_i<<"th Array is:"<<endl;
        for(Index_m = 0; Index_m < Rows; Index_m++){

            for(Index_n = 0; Index_n < columns; Index_n++){

                cout<<*(double *)(ListItem->data + Index_m * ListItem->strides[0] + Index_n * ListItem->strides[1])<<" ";//访问数据，Index_m 和 Index_n 分别是数组元素的坐标，乘上相应维度的步长，即可以访问数组元素
            }
            cout<<endl;
        }

        Py_DECREF(ListItem);
    }
}else{

    cout<<"Not a List"<<endl;
}
```

#### **向 Python 文件中的函数传递 List 数组**


现在我们的需求是我们要将 C/C++文件中的数组传递给 Python 文件的某个函数，那么我们将借助 List Object 和 Tuple Object 来封装我们的数据，从而传递给 Python 文件中的函数。



```python
#假设现在我们有这样一个Python函数，其功能是接受一个由 C/C++ 文件传递的List数组，并打印出来
def PassListFromCToPython(List):

    PyList = List
    print (PyList)
```


那么在 C/C++ 文件端，我们需要做如下处理：



```python
/*Pass by List: Transform an C Array to Python List*/

    double CArray[] = {1.2, 4.5, 6.7, 8.9, 1.5, 0.5};

    PyObject *PyList  = PyList_New(6);//定义一个与数组等长的PyList对象数组
    PyObject *ArgList = PyTuple_New(1);//定义一个Tuple对象，Tuple对象的长度与Python函数参数个数一直，上面Python参数个数为1，所以这里给的长度为1
    for(Index_i = 0; Index_i < PyList_Size(PyList); Index_i++){

        PyList_SetItem(PyList, Index_i, PyFloat_FromDouble(CArray[Index_i]));//给PyList对象的每个元素赋值
    }

    PyObject *pFuncFour = PyDict_GetItemString(pDict, "PassListFromCToPython");
    cout<<"C Array Pass Into The Python List:"<<endl;
    PyTuple_SetItem(ArgList, 0, PyList);//将PyList对象放入PyTuple对象中
    PyObject_CallObject(pFuncFour, ArgList);//调用函数，完成传递
```

#### **向 Python 文件中的函数传递 Numpy Array 数组**


当我们需要向 Python 文件中传递一个多维数组时，这个时候我们借助PyArrayObject 和 PyTuple 会更加的合适。



```python
#假设现在我们的Python函数是接受一个 Numpy Array 数组进行处理
def PassArrayFromCToPython(Array):

    print "Shape Of Array:", Array.shape

    print Array
```


那么我们就需要在 C/C++ 文件中做如下的处理：



```python
double CArrays[3][3] = {{1.3, 2.4, 5.6}, {4.5, 7.8, 8.9}, {1.7, 0.4, 0.8}}; //定义一个3 X 3的数组

    npy_intp Dims[2] = {3, 3}; //给定维度信息

    PyObject *PyArray  = PyArray_SimpleNewFromData(2, Dims, NPY_DOUBLE, CArrays); //生成包含这个多维数组的PyObject对象，使用PyArray_SimpleNewFromData函数，第一个参数2表示维度，第二个为维度数组Dims,第三个参数指出数组的类型，第四个参数为数组
    PyObject *ArgArray = PyTuple_New(1);
    PyTuple_SetItem(ArgArray, 0, PyArray); //同样定义大小与Python函数参数个数一致的PyTuple对象

    PyObject *pFuncFive = PyDict_GetItemString(pDict, "PassArrayFromCToPython");
    PyObject_CallObject(pFuncFive, ArgArray);//调用函数，传入Numpy Array 对象。
```

#### **代码块**


下面给出完整的代码示例：



```python
#ModuleOne.py文件
def ArrayListReturn():

    ArrList = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

    Array_A  = np.asarray(ArrList, dtype='float' )
    Array_B  = np.asarray(ArrList, dtype='double')

    return [Array_A, Array_B]

def IntegerListReturn():

    IntegerList = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

    return IntegerList

def FloatListReturn():

    FloatList = [[1.2, 2.3, 3.5], [0.5, 5.2, 6.5], [7.2, 8.8, 9.3]]

    return FloatList

def PassListFromCToPython(List):

    PyList = List
    print (PyList)

def PassArrayFromCToPython(Array):

    print "Shape Of Array:", Array.shape

    print Array
```


C/C++文件：



```python
#include <iostream>
#include <Python.h>
#include <numpy/arrayobject.h>
using namespace std;

void init_numpy(){

    import_array();
}
int main()
{

    Py_Initialize();
    init_numpy();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/liaojian/Documents/Programming/PythonWorkSpace/CalledByCplusplus/')");

    PyObject *pModule  = nullptr;
    PyObject *pDict    = nullptr;

    pModule  = PyImport_ImportModule("ModuleOne");
    pDict    = PyModule_GetDict(pModule);

    /*Return the List which contains Numpy Array*/
    PyObject *pFuncOne    = PyDict_GetItemString(pDict, "ArrayListReturn");

    PyObject *FuncOneBack = PyObject_CallObject(pFuncOne, nullptr);

    int Index_i = 0, Index_k = 0, Index_m = 0, Index_n = 0;
    if(PyList_Check(FuncOneBack)){

        int SizeOfList = PyList_Size(FuncOneBack);

        for(Index_i = 0; Index_i < SizeOfList; Index_i++){

            PyArrayObject *ListItem = (PyArrayObject *)PyList_GetItem(FuncOneBack, Index_i);

            int Rows = ListItem->dimensions[0], columns = ListItem->dimensions[1];
            cout<<"The "<<Index_i<<"th Array is:"<<endl;
            for(Index_m = 0; Index_m < Rows; Index_m++){

                for(Index_n = 0; Index_n < columns; Index_n++){

                    cout<<*(double *)(ListItem->data + Index_m * ListItem->strides[0] + Index_n * ListItem->strides[1])<<" ";
                }
                cout<<endl;
            }

            Py_DECREF(ListItem);
        }
    }else{

        cout<<"Not a List"<<endl;
    }

    /*Return Integer List and Access to Each Items*/

    cout<<"Integer List Show:"<<endl;
    PyObject *pFuncTwo     = PyDict_GetItemString(pDict, "IntegerListReturn");
    PyObject *FuncTwoBack  = PyObject_CallObject (pFuncTwo, nullptr);

    if(PyList_Check(FuncTwoBack)){

        int SizeOfList = PyList_Size(FuncTwoBack);
        for(Index_i = 0; Index_i < SizeOfList; Index_i++){

            PyObject *ListItem = PyList_GetItem(FuncTwoBack, Index_i);

            int NumOfItems = PyList_Size(ListItem);

            for(Index_k = 0; Index_k < NumOfItems; Index_k++){

                PyObject *Item = PyList_GetItem(ListItem, Index_k);

                cout << PyInt_AsLong(Item) <<" ";

                Py_DECREF(Item);
            }

            Py_DECREF(ListItem);
        }
        cout<<endl;


    }else{

        cout<<"Not a List"<<endl;
    }

    /*Return Float List and Access to Each Items*/

    cout<<"Double List Show:"<<endl;
    PyObject *pFunThree     = PyDict_GetItemString(pDict, "FloatListReturn");
    PyObject *FuncThreeBack = PyObject_CallObject  (pFunThree, nullptr);

    if(PyList_Check(FuncThreeBack)){

        int SizeOfList = PyList_Size(FuncThreeBack);
        for(Index_i = 0; Index_i < SizeOfList; Index_i ++){

            PyObject *ListItem = PyList_GetItem(FuncThreeBack, Index_i);
            int NumOfItems = PyList_Size(ListItem);

            for(Index_k = 0; Index_k < NumOfItems; Index_k++){

                PyObject *Item  = PyList_GetItem(ListItem, Index_k);

                cout<< PyFloat_AsDouble(Item) << " ";

                Py_DECREF(Item);
            }

            Py_DECREF(ListItem);
        }
        cout<<endl;


    }else{

        cout<<"Not a List"<<endl;
    }

    /*Pass by List: Transform an C Array to Python List*/

    double CArray[] = {1.2, 4.5, 6.7, 8.9, 1.5, 0.5};

    PyObject *PyList  = PyList_New(6);
    PyObject *ArgList = PyTuple_New(1);
    for(Index_i = 0; Index_i < PyList_Size(PyList); Index_i++){

        PyList_SetItem(PyList, Index_i, PyFloat_FromDouble(CArray[Index_i]));
    }

    PyObject *pFuncFour = PyDict_GetItemString(pDict, "PassListFromCToPython");
    cout<<"C Array Pass Into The Python List:"<<endl;
    PyTuple_SetItem(ArgList, 0, PyList);
    PyObject_CallObject(pFuncFour, ArgList);

    /*Pass by Python Array: Transform an C Array to Python Array*/

    double CArrays[3][3] = {{1.3, 2.4, 5.6}, {4.5, 7.8, 8.9}, {1.7, 0.4, 0.8}};

    npy_intp Dims[2] = {3, 3};

    PyObject *PyArray  = PyArray_SimpleNewFromData(2, Dims, NPY_DOUBLE, CArrays);
    PyObject *ArgArray = PyTuple_New(1);
    PyTuple_SetItem(ArgArray, 0, PyArray);

    PyObject *pFuncFive = PyDict_GetItemString(pDict, "PassArrayFromCToPython");
    PyObject_CallObject(pFuncFive, ArgArray);



    //Release

    Py_DECREF(pModule);
    Py_DECREF(pDict);
    Py_DECREF(FuncOneBack);
    Py_DECREF(FuncTwoBack);
    Py_DECREF(FuncThreeBack);
    Py_DECREF(PyList);
    Py_DECREF(ArgList);
    Py_DECREF(PyArray);
    Py_DECREF(ArgArray);
    return 0;
}
```


执行结果： 
![这是执行C / C++文件后得到的结果](https://img-blog.csdn.net/20170408183637121?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3R1X2NzZG4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


参考网站：
- List Object 对象介绍：[https://docs.python.org/2.7/c-api/list.html?highlight=pylist_new#c.PyList_New](https://docs.python.org/2.7/c-api/list.html?highlight=pylist_new#c.PyList_New)
- Tuple Object 对象介绍：[https://docs.python.org/2.7/c-api/tuple.html?highlight=pytuple_new](https://docs.python.org/2.7/c-api/tuple.html?highlight=pytuple_new)
- PyArrayObject对象介绍：[https://docs.scipy.org/doc/numpy-1.10.0/reference/c-api.array.html#c.import_array](https://docs.scipy.org/doc/numpy-1.10.0/reference/c-api.array.html#c.import_array)
- PyArrayObject对象使用介绍：[http://folk.uio.no/hpl/scripting/doc/python/NumPy/Numeric/numpy-13.html](http://folk.uio.no/hpl/scripting/doc/python/NumPy/Numeric/numpy-13.html)
- Python与C /C++嵌入式编程：[https://www.codeproject.com/Articles/11805/Embedding-Python-in-C-C-Part-I](https://www.codeproject.com/Articles/11805/Embedding-Python-in-C-C-Part-I)



