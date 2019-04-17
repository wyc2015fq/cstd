# 【python】argparse.add_argument中的action为‘store_true’使用说明 - zkq_1986的博客 - CSDN博客





2018年12月27日 16:09:39[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1215








a.py文件的代码如下：
import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--t', help=' ', action='store_true', default=False)

config = parser.parse_args()

print(config.t)
直接运行python a.py，输出结果False

运行python a.py --t，输出结果True

也就是说，action='store_true'，只要运行时该变量有传参就将该变量设为True。





