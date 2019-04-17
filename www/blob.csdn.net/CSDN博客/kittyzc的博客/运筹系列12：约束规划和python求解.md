# 运筹系列12：约束规划和python求解 - kittyzc的博客 - CSDN博客





2018年11月19日 22:50:23[IE06](https://me.csdn.net/kittyzc)阅读数：294
所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)









# 1. 从算式谜问题说起

SEND + MORE = MONEY，每一个字母代表一个数字，如何求解？

这个问题显然不是传统意义上的整数规划问题，并没有需要进行优化的目标函数，因此使用约束规划进行求解，问题建模如下：

```python
from ortools.sat.python import cp_model
# Creates the model.
model = cp_model.CpModel()
kBase = 10
# Creates the variables.
s = model.NewIntVar(1, kBase - 1, 'S');
e = model.NewIntVar(0, kBase - 1, 'E');
n = model.NewIntVar(0, kBase - 1, 'N');
d = model.NewIntVar(0, kBase - 1, 'D');
m = model.NewIntVar(1, kBase - 1, 'M');
o = model.NewIntVar(0, kBase - 1, 'O');
r = model.NewIntVar(0, kBase - 1, 'R');
y = model.NewIntVar(0, kBase - 1, 'Y');
letters = [s,e,n,d,m,o,r,y]

# Creates the constraints.
model.AddAllDifferent(letters)
model.Add(d  + e + kBase * (n+r) + kBase * kBase * (e+o) + kBase * kBase * kBase * (s+m) == 
          y + kBase * e + kBase * kBase * n + kBase * kBase * kBase * o + kBase * kBase * kBase * kBase * m)
# Creates a solver and solves the model.
solver = cp_model.CpSolver()
```

注意其中有一个model.AddAllDifferent(letters)，是要求所有变量都不相等的快速表达模式。

# 2. 输出一个可行解

如果只需要输出一个可行解，可以参照上一节的内容，将下列代码添加至模型之后：

```python
status = solver.Solve(model)
print('status = %s' % solver.StatusName(status))
for v in letters:
    print('%s=%i' % (v, solver.Value(v)), end=' ')
```

输出为：

```python
status = FEASIBLE
S=9 E=5 N=6 D=7 M=1 O=0 R=8 Y=2
```

# 3. 输出所有解

若要输出所有解，可以调用cp_model.CpSolver.SearchForAllSolutions(cp_model.CpModel, 回调函数(变量list))。

```python
class VarArraySolutionPrinter(cp_model.CpSolverSolutionCallback):
    """Print intermediate solutions."""

    def __init__(self, variables):
        cp_model.CpSolverSolutionCallback.__init__(self)
        self.__variables = variables
        self.__solution_count = 0

    def OnSolutionCallback(self):
        self.__solution_count += 1
        for v in self.__variables:
            print('%s=%i' % (v, self.Value(v)), end=' ')
        print()

    def SolutionCount(self):
        return self.__solution_count
        
solution_printer = VarArraySolutionPrinter(letters)
status = solver.SearchForAllSolutions(model, solution_printer)

print('Status = %s' % solver.StatusName(status))
print('Number of solutions found: %i' % solution_printer.SolutionCount())
```

结果为：

```
S=9 E=5 N=6 D=7 M=1 O=0 R=8 Y=2 
Status = FEASIBLE
Number of solutions found: 1
```

# 4. 设置结束条件

当问题规模比较大时，我们可以设置一些结束条件（解数量限制或者时间限制）。

时间限制的话，添加如下代码即可：

```python
solver.parameters.max_time_in_seconds = 10.0
```

解数量限制的话，修改solver回调函数，将__solution__limit修改为限制值即可，参考如下代码：

```python
class VarArraySolutionPrinterWithLimit(cp_model.CpSolverSolutionCallback):
    def __init__(self, variables, limit):
        cp_model.CpSolverSolutionCallback.__init__(self)
        self.__variables = variables
        self.__solution_count = 0
        self.__solution_limit = limit

    def OnSolutionCallback(self):
        self.__solution_count += 1
        for v in self.__variables:
            print('%s=%i' % (v, self.Value(v)), end=' ')
        print()
        if self.__solution_count >= self.__solution_limit:
            print('Stop search after %i solutions' % self.__solution_limit)
            self.StopSearch()

    def SolutionCount(self):
        return self.__solution_count
```



