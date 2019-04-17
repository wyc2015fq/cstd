# tensorflow学习笔记（一）：命令行参数 - Keith - CSDN博客





2016年10月20日 10:54:05[ke1th](https://me.csdn.net/u012436149)阅读数：21425
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









全局环境下编写代码

```python
import tensorflow as tf
flags = tf.flags #flags是一个文件：flags.py，用于处理命令行参数的解析工作
logging = tf.logging

#调用flags内部的DEFINE_string函数来制定解析规则
flags.DEFINE_string("para_name_1","default_val", "description")
flags.DEFINE_bool("para_name_2","default_val", "description")

#FLAGS是一个对象，保存了解析后的命令行参数
FLAGS = flags.FLAGS

def main(_):
    FLAGS.para_name #调用命令行输入的参数

if __name__ == "__main__": #使用这种方式保证了，如果此文件被其它文件import的时候，不会执行main中的代码

    tf.app.run() #解析命令行参数，调用main函数 main(sys.argv)
```

调用方法：

```python
~/ python script.py --para_name_1=name --para_name_2=name2
# 不传的话，会使用默认值
```



