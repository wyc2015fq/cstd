# DLL load failed: 找不到指定模块\Failed to load the native TensorFlow runtime解决方法 - Machine Learning with Peppa - CSDN博客





2018年10月13日 22:04:48[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：6650








完整报错信息如下：

```python
Traceback (most recent call last):
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\pywrap_tensorflow_internal.py", line 18, in swig_import_helper
return importlib.import_module(mname)
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\importlib\__init__.py", line 126, in import_module
return _bootstrap._gcd_import(name[level:], package, level)
File "<frozen importlib._bootstrap>", line 986, in _gcd_import
File "<frozen importlib._bootstrap>", line 969, in _find_and_load
File "<frozen importlib._bootstrap>", line 958, in _find_and_load_unlocked
File "<frozen importlib._bootstrap>", line 666, in _load_unlocked
File "<frozen importlib._bootstrap>", line 577, in module_from_spec
File "<frozen importlib._bootstrap_external>", line 906, in create_module
File "<frozen importlib._bootstrap>", line 222, in _call_with_frames_removed
ImportError: DLL load failed: 找不到指定的模块。


During handling of the above exception, another exception occurred:


Traceback (most recent call last):
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\pywrap_tensorflow.py", line 41, in <module>
from tensorflow.python.pywrap_tensorflow_internal import *
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\pywrap_tensorflow_internal.py", line 21, in <module>
_pywrap_tensorflow_internal = swig_import_helper()
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\pywrap_tensorflow_internal.py", line 20, in swig_import_helper
return importlib.import_module('_pywrap_tensorflow_internal')
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\importlib\__init__.py", line 126, in import_module
return _bootstrap._gcd_import(name[level:], package, level)
ImportError: No module named '_pywrap_tensorflow_internal'


During handling of the above exception, another exception occurred:


Traceback (most recent call last):
File "<stdin>", line 1, in <module>
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\__init__.py", line 24, in <module>
from tensorflow.python import *
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\__init__.py", line 51, in <module>
from tensorflow.python import pywrap_tensorflow
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\pywrap_tensorflow.py", line 52, in <module>
raise ImportError(msg)
ImportError: Traceback (most recent call last):
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\pywrap_tensorflow_internal.py", line 18, in swig_import_helper
return importlib.import_module(mname)
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\importlib\__init__.py", line 126, in import_module
return _bootstrap._gcd_import(name[level:], package, level)
File "<frozen importlib._bootstrap>", line 986, in _gcd_import
File "<frozen importlib._bootstrap>", line 969, in _find_and_load
File "<frozen importlib._bootstrap>", line 958, in _find_and_load_unlocked
File "<frozen importlib._bootstrap>", line 666, in _load_unlocked
File "<frozen importlib._bootstrap>", line 577, in module_from_spec
File "<frozen importlib._bootstrap_external>", line 906, in create_module
File "<frozen importlib._bootstrap>", line 222, in _call_with_frames_removed
ImportError: DLL load failed: 找不到指定的模块。


During handling of the above exception, another exception occurred:


Traceback (most recent call last):
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\pywrap_tensorflow.py", line 41, in <module>
from tensorflow.python.pywrap_tensorflow_internal import *
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\pywrap_tensorflow_internal.py", line 21, in <module>
_pywrap_tensorflow_internal = swig_import_helper()
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\site-packages\tensorflow\python\pywrap_tensorflow_internal.py", line 20, in swig_import_helper
return importlib.import_module('_pywrap_tensorflow_internal')
File "C:\Users\toy\AppData\Local\Programs\Python\Python35\lib\importlib\__init__.py", line 126, in import_module
return _bootstrap._gcd_import(name[level:], package, level)
ImportError: No module named '_pywrap_tensorflow_internal'




Failed to load the native TensorFlow runtime.


See https://www.tensorflow.org/install/install_sources#common_installation_problems


for some common reasons and solutions. Include the entire stack trace
above this error message when asking for help.
```



错误原因如下：

## 1.版本问题

我自己的电脑上是，cuddn更换成6.0，之前TensorFlow1.3+python3.6+cuda8.0+cudnn5.0也是这个错误，换成cudnn6.0就成功了，亲测可用可以试试。以下是其他版本。

tensorflow-gpu v1.9.0 | cuda9.0 |  cuDNN7.1.4可行  | 备注：7.0.4/ 7.0.5/ 7.1.2不明确

tensorflow-gpu v1.8.0 | cuda9.0 |  cuDNN  不明确 | 备注：7.0.4/ 7.0.5/ 7.1.2/ 7.1.4

tensorflow-gpu v1.7.0 | cuda9.0 |  cuDNN  不明确 | 备注：7.0.4/ 7.0.5/ 7.1.2/ 7.1.4

tensorflow-gpu v1.6.0 | cuda9.0 |  cuDNN  不明确 | 备注：7.0.4/ 7.0.5/ 7.1.2/ 7.1.4

tensorflow-gpu v1.5.0 | cuda9.0 |  cuDNN  不明确 | 备注：7.0.4/ 7.0.5/ 7.1.2/ 7.1.4

tensorflow-gpu v1.4.0 | cuda8.0 |  cuDNN 6.0 | 备注：6.0正常使用, 7.0.5不能用，5.1未知 

tensorflow-gpu v1.3.0 | cuda8.0 |  cuDNN 6.0 | 备注：6.0正常使用, 7.0.5不能用，5.1未知 

tensorflow-gpu v1.2.0 | cuda8.0 |  cuDNN 5.1 | 备注：5.1正常使用, 6.0/ 7.0.5 未知

tensorflow-gpu v1.1.0 | cuda8.0 |  cuDNN 5.1 | 备注：5.1正常使用, 6.0/ 7.0.5 未知




如果是版本不兼容，假如你之前安装的是cudnn5.1的话，若你的TensorFlow为1.3版本以上，则只要将cudnn64_5改成cudnn64_6就可以了



## 2. 环境问题
- python的版本太高了，换成3.6的就好。
- 安装Visual C++ Redistributable for Visual Studio 2015 vc_redist.x64就可以解决。
- 确定你的系统中有MSVCP140.DLL，如果没有，可以在[这里](https://www.microsoft.com/en-us/download/details.aspx?id=53587)下载 
- 确认你的系统PATH环境变量设置正确，把CUDA目录下的bin,lib/x64等路径包含进去 
- 确认你的电脑正确安装了[DXSDK_Jun10](http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=6812)

最后记得安装之前一定要把tensorflow卸载干净：

```python
pip uninstall tensorflow
 pip uninstall tensorflow-gpu
```





