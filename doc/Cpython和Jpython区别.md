## Cpython和Jpython区别



当人们谈论起Python，他们不仅在说语言本身，还包括其CPython的实现。Python实际上是一个可以用许多不同的方式来实现的语言规范。

##### Cpython

```
CPython  :是python的参考实现， C编写。 
　　　　  :把python编译成中间态的字节码，然后有虚拟机解释。	
```

##### PyPy

```
PyPy : 是用RPython实现的解释器。
　　　：特点是即时编译（JIT Just In Time）
　　　：旨在提高性能，同时保持最大兼容性，目前比CPython的速度快超过5倍。
```

##### Jpython

```
Jython :是一个将python代码编译成java字节码的实现，运行在JVM上。
	   :它可以像使用Python模块一样，导入并使用任何Java类。
```

##### IronPython

```
 IronPython: 是一个针对.NET框架的Python实现
 		  ：可以用Python和.NET famework的库，也将Python暴漏给.NET框架中的其他语言。
```

##### PythonNet

```
PythonNet：是一个近乎无缝集成的，提供给本机已安装的Python.NET公共语言运行时（CLR）包.
         ：与IronPython互补
```

------

#### Cpython和Jpython的区别