# _tkinter.TclError: no display name and no $DISPLAY environment variable的问题 - zj360202的专栏 - CSDN博客





2018年01月18日 12:26:33[zj360202](https://me.csdn.net/zj360202)阅读数：891








升级了matplotlib ,但发现绘图脚本不能用了。




报错：

>>> plt.figure(figsize=(30,20))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "/usr/local/lib/python2.7/site-packages/matplotlib-1.5.0-py2.7-linux-x86_64.egg/matplotlib/pyplot.py", line 527, in figure
    **kwargs)
  File "/usr/local/lib/python2.7/site-packages/matplotlib-1.5.0-py2.7-linux-x86_64.egg/matplotlib/backends/backend_tkagg.py", line 84, in new_figure_manager
    return new_figure_manager_given_figure(num, figure)
  File "/usr/local/lib/python2.7/site-packages/matplotlib-1.5.0-py2.7-linux-x86_64.egg/matplotlib/backends/backend_tkagg.py", line 92, in new_figure_manager_given_figure
    window = Tk.Tk()
  File "/usr/local/lib/python2.7/lib-tk/Tkinter.py", line 1810, in __init__
    self.tk = _tkinter.create(screenName, baseName, className, interacti


_tkinter.TclError: no display name and no $DISPLAY environment variable





解决：在脚本导入任何库之前，运行：



```
import matplotlib
# Force matplotlib to not use any Xwindows backend.
matplotlib.use('Agg')
```


```

```

举例：




```
import matplotlib
# Force matplotlib to not use any Xwindows backend.
matplotlib.use('Agg')
```





```
import matplotlib.pyplot as plt
import numpy as np

x = np.random.randn(60)
y = np.random.randn(60)

plt.scatter(x, y, s=20)

out_png = 'path/to/store/out_file.png'
plt.savefig(out_png, dpi=150)
```






