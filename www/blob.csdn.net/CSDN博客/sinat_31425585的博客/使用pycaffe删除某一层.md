# 使用pycaffe删除某一层 - sinat_31425585的博客 - CSDN博客
2019年03月04日 22:44:29[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：58
    需要将mobilessdlite中silence层删除掉，方法很简单，首先，将deploy.prototxt文件中的silence层删除掉，然后使用caffe载入模型，最后将模型重新写成caffemodel
** 具体如下：**
    第一步：删除deploy.prototxt文件中的silence关键字字段    
    先用notepad++将deploy.prototxt文件打开，然后搜寻silence关键字，将包含silence关键字字段删除掉；
    第二步：使用pycaffe载入caffemodel
```python
import caffe
net = caffe.Net('deploy.prototxt', 'deploy.caffemodel', caffe.TEST)
```
   第三步：将caffemodel写出来
```python
net.save('ssd_remove.caffemodel')
```
    合起来三句代码，就是这么简单。
```python
import caffe
net = caffe.Net('deploy.prototxt', 'deploy.caffemodel', caffe.TEST)
net.save('ssd_remove.caffemodel')
```
参考资料：
[https://blog.csdn.net/a8039974/article/details/79105031](https://blog.csdn.net/a8039974/article/details/79105031)
[https://blog.csdn.net/woyaopojie1990/article/details/53462040](https://blog.csdn.net/woyaopojie1990/article/details/53462040)
[https://blog.csdn.net/wangkun1340378/article/details/76474900](https://blog.csdn.net/wangkun1340378/article/details/76474900)
