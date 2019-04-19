# python 一行代码定义树_用户5706034127_新浪博客
|||
def tree(): return defaultdict(tree)
注意返回的是一个字典。
mytree = tree()
mytree['val'] = 111
mytree['left']['val'] = 222
mytree['right']['val'] =
333
mytree['right']['right']['val'] = 444
