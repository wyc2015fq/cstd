
# 轻松掌握python设计模式之策略模式 - jiahaowanhao的博客 - CSDN博客


2018年07月11日 13:49:06[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：35标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)


[轻松掌握python设计模式之策略模式](http://cda.pinggu.org/view/26062.html)
这篇文章主要帮助大家轻松掌握python设计模式之策略模式,具有一定的参考价值，感兴趣的小伙伴们可以参考一下
本文实例为大家分享了python策略模式代码，供大家参考，具体内容如下
"""
策略模式
"""
import types
class StrategyExample:
def __init__(self, func=None):
self.name = '策略例子0'
if func is not None:
"""给实例绑定方法用的，不会影响到其他实例"""
self.execute = types.MethodType(func, self)
def execute(self):
print(self.name)
def execute_replacement1(self):
print(self.name + ' 从执行1')

def execute_replacement2(self):
print(self.name + ' 从执行2')

if __name__ == '__main__':
strat0 = StrategyExample()
strat1 = StrategyExample(execute_replacement1)
strat1.name = '策略例子1'
strat2 = StrategyExample(execute_replacement2)
strat2.name = '策略例子2'
strat0.execute()
strat1.execute()
strat2.execute()
运行结果如图:
![](http://www.cda.cn/uploadfile/image/20180711/20180711060118_20805.png)
以上就是本文的全部内容，希望对大家的学习有所帮助

