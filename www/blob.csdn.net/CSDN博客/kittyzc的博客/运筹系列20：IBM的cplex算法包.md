# 运筹系列20：IBM的cplex算法包 - kittyzc的博客 - CSDN博客
2019年01月03日 19:41:24[IE06](https://me.csdn.net/kittyzc)阅读数：466所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)
# 1. cplex介绍

Cplex是IBM出的一款科学计算软件，从IBM官网可以下到最新版。最近惊奇的发现python竟然能直接安装使用cplex了！

安装方法如下：

```
pip install cplex
```

# 2. 示例代码和说明

用一个简单案例尝试一下：
$max⁡x1+2x2+3x3+x4\max x_1+2x_2+3x_3+x_4maxx1​+2x2​+3x3​+x4​$

s.t. $−x1+x2+x3+10x4≤20-x_1+x_2+x_3+10x_4\le20−x1​+x2​+x3​+10x4​≤20$
$x1−3x2+x3≤30x_1-3x_2+x_3\le30x1​−3x2​+x3​≤30$
$x2−3.5x4=0x_2-3.5x_4=0x2​−3.5x4​=0$
$x1≤40x_1\le40x1​≤40$
$2≤x4≤32\le x_4\le32≤x4​≤3$，$x4∈Zx_4\in Zx4​∈Z$
$x1,x2,x3≥0x_1,x_2,x_3\ge0x1​,x2​,x3​≥0$
```python
import cplex
from cplex.exceptions import CplexError
my_obj = [1.0, 2.0, 3.0, 1.0]
my_ub = [40.0, cplex.infinity, cplex.infinity, 3.0]
my_lb = [0.0, 0.0, 0.0, 2.0]
my_ctype = "CCCI"
my_colnames = ["x1", "x2", "x3", "x4"]
my_rhs = [20.0, 30.0, 0.0]
my_rownames = ["r1", "r2", "r3"]
my_sense = "LLE"


def populatebyrow(prob):
    prob.objective.set_sense(prob.objective.sense.maximize)
    prob.variables.add(obj=my_obj, lb=my_lb, ub=my_ub, types=my_ctype,
                       names=my_colnames)
    rows = [[my_colnames, [-1.0, 1.0, 1.0, 10.0]],
            [my_colnames, [1.0, -3.0, 1.0, 0.0]],
            [my_colnames, [0.0, 1.0, -3.5, 0.0]]]
    prob.linear_constraints.add(lin_expr=rows, senses=my_sense,rhs=my_rhs, names=my_rownames)

try:
    my_prob = cplex.Cplex()
    handle = populatebyrow(my_prob)
    my_prob.solve()
    
except CplexError as exc:
    print(exc)

print("Solution status = ", my_prob.solution.status[my_prob.solution.get_status()])
print("Solution value  = ", my_prob.solution.get_objective_value())
x = my_prob.solution.get_values()
print('x: ',x)
```

输出为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190103160924749.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)
代码中：

my_ctype中，C表示连续变量，I表示整数型变量

my_sense，G表示大于等于，E表示等于，L表示小于等于

不过由于只用能矩阵和向量形式表示，因此对于某些复杂的模型，写起来还是比较麻烦的。


