# tensorflow命令行参数 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:10:13[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：880

深度学习神经网络往往有过多的Hyperparameter需要调优，优化算法、学习率、卷积核尺寸等很多参数都需要不断调整，使用命令行参数是非常方便的。有两种实现方式，一是利用python的argparse包，二是调用tensorflow自带的app.flags实现。
## 利用python的argparse包
argparse介绍及基本使用：
[http://www.jianshu.com/p/b8b09084bd1a](http://www.jianshu.com/p/b8b09084bd1a)
下面代码用argparse实现了命令行参数的输入。
```
import argparse
import sys
parser = argparse.ArgumentParser()
parser.add_argument('--fake_data', nargs='?', const=True, type=bool,
                      default=False,
                      help='If true, uses fake data for unit testing.')
parser.add_argument('--max_steps', type=int, default=1000,
                      help='Number of steps to run trainer.')
parser.add_argument('--learning_rate', type=float, default=0.001,
                      help='Initial learning rate')
parser.add_argument('--dropout', type=float, default=0.9,
                      help='Keep probability for training dropout.')
parser.add_argument('--data_dir', type=str, default='/tmp/tensorflow/mnist/input_data',
                      help='Directory for storing input data')
parser.add_argument('--log_dir', type=str, default='/tmp/tensorflow/mnist/logs/mnist_with_summaries',
                      help='Summaries log directory')
FLAGS, unparsed = parser.parse_known_args()
tf.app.run(main=main, argv=[sys.argv[0]] + unparsed)
```
通过调用python的argparse包，调用函数parser.parse_known_args()解析命令行参数。代码运行后得到的FLAGS是一个结构体，内部参数分别为：
```
FLAGS.data_dir
Out[5]: '/tmp/tensorflow/mnist/input_data'
FLAGS.fake_data
Out[6]: False
FLAGS.max_steps
Out[7]: 1000
FLAGS.learning_rate
Out[8]: 0.001
FLAGS.dropout
Out[9]: 0.9
FLAGS.data_dir
Out[10]: '/tmp/tensorflow/mnist/input_data'
FLAGS.log_dir
Out[11]: '/tmp/tensorflow/mnist/logs/mnist_with_summaries'
```
# 利用tf.app.flags组件
首先需要定义一个tf.app.flags对象，调用自带的DEFINE_string, DEFINE_boolean, DEFINE_integer, DEFINE_float设置不同类型的命令行参数及其默认值。当然，也可以在终端用命令行参数修改这些默认值。
```
# Define hyperparameters
flags = tf.app.flags
FLAGS = flags.FLAGS
flags.DEFINE_boolean("enable_colored_log", False, "Enable colored log")
                    "The glob pattern of train TFRecords files")
flags.DEFINE_string("validate_tfrecords_file",
                    "./data/a8a/a8a_test.libsvm.tfrecords",
                    "The glob pattern of validate TFRecords files")
flags.DEFINE_integer("label_size", 2, "Number of label size")
flags.DEFINE_float("learning_rate", 0.01, "The learning rate")
def main():
    # Get hyperparameters
    if FLAGS.enable_colored_log:
        import coloredlogs
        coloredlogs.install()
    logging.basicConfig(level=logging.INFO)
    FEATURE_SIZE = FLAGS.feature_size
    LABEL_SIZE = FLAGS.label_size
      ...
  return 0
if __name__ == ‘__main__’:
    main()
```
这段代码采用的是tensorflow库中自带的tf.app.flags模块实现命令行参数的解析。如果用终端运行tf程序，用上述两种方式都可以，如果用spyder之类的工具，那么只有第一种方式有用，第二种方式会报错。
其中有个tf.app.flags组件，还有个tf.app.run()函数。官网帮助文件是这么说的：
flags module: Implementation of the flags interface.
run(...): Runs the program with an optional 'main' function and 'argv' list.
tf.app.run的源代码：
```
1."""Generic entry point script."""  
2.from __future__ import absolute_import  
3.from __future__ import division  
4.from __future__ import print_function  
5.  
6.import sys  
7.  
8.from tensorflow.python.platform import flags  
9.  
10.  
11.def run(main=None):  
12.  f = flags.FLAGS  
13.  f._parse_flags()  
14.  main = main or sys.modules['__main__'].main  
15.  sys.exit(main(sys.argv))
```
也就是处理flag解析，然后执行main函数。
# 用shell脚本实现训练代码的执行
在终端执行python代码，首先需要在代码文件开头写入shebang，告诉系统环境变量如何设置，用python2还是用python3来编译这段代码。然后修改代码权限为可执行，用
./python_code.py
就可以执行。同理，这段代码也可以用shell脚本来实现。创建.sh文件，运行python_code.py并设置参数max_steps=100
`python python_code.py --max_steps 100`
