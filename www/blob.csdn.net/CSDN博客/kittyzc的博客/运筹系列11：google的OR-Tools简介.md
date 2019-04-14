# 运筹系列11：google的OR-Tools简介 - kittyzc的博客 - CSDN博客
2018年11月16日 18:07:42[IE06](https://me.csdn.net/kittyzc)阅读数：962所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)
# 1. 介绍

google的开源优化算法包ortools，支持线性规划、整数规划，可以方便的求解Routing、Bin packing、Network flows、Assignment、Scheduling等问题。

官网地址为：[https://developers.google.com/optimization](https://developers.google.com/optimization)

开源代码地址为 [https://github.com/google/or-tools](https://github.com/google/or-tools)

算法包支持java、c++、c#、python。这里使用python环境，安装非常简单：
```
pip install ortools
```

# 2. 线性规划示例

ortools默认使用Glop（谷歌自己开发的线性规划求解器）

这里粘贴一下官网的例子。相比其他专业的优化软件，语法是有点冗长。

```python
from ortools.linear_solver import pywraplp
solver = pywraplp.Solver('LinearExample',pywraplp.Solver.GLOP_LINEAR_PROGRAMMING)

# Create the two variables and let them take on any value.
x = solver.NumVar(-solver.infinity(), solver.infinity(), 'x')
y = solver.NumVar(-solver.infinity(), solver.infinity(), 'y')

# Constraint 1: x + 2y <= 14.
constraint1 = solver.Constraint(-solver.infinity(), 14)
constraint1.SetCoefficient(x, 1)
constraint1.SetCoefficient(y, 2)

# Constraint 2: 3x - y >= 0.
constraint2 = solver.Constraint(0, solver.infinity())
constraint2.SetCoefficient(x, 3)
constraint2.SetCoefficient(y, -1)

# Constraint 3: x - y <= 2.
constraint3 = solver.Constraint(-solver.infinity(), 2)
constraint3.SetCoefficient(x, 1)
constraint3.SetCoefficient(y, -1)

# Objective function: 3x + 4y.
objective = solver.Objective()
objective.SetCoefficient(x, 3)
objective.SetCoefficient(y, 4)
objective.SetMaximization()

# Solve the system.
solver.Solve()
opt_solution = 3 * x.solution_value() + 4 * y.solution_value()
print('Number of variables =', solver.NumVariables())
print('Number of constraints =', solver.NumConstraints())
# The value of each variable in the solution.
print('Solution:')
print('x = ', x.solution_value())
print('y = ', y.solution_value())
# The objective value of the solution.
print('Optimal objective value =', opt_solution)
```

输出为：

```
Number of variables = 2
Number of constraints = 3
Solution:
x =  5.999999999999998
y =  3.9999999999999996
Optimal objective value = 33.99999999999999
```

# 3. 混合整数规划

OR-Tools默认使用Coin-or branch and cut (CBC)求解整数规划问题。此外，还支持SCIP、GLPK、Gurobi等。下面是官方例子，注意求解器和变量定义发生了变化。

```python
from ortools.linear_solver import pywraplp
solver = pywraplp.Solver('SolveIntegerProblem',pywraplp.Solver.CBC_MIXED_INTEGER_PROGRAMMING)

# x and y are integer non-negative variables.
x = solver.IntVar(0.0, solver.infinity(), 'x')
y = solver.IntVar(0.0, solver.infinity(), 'y')

# x + 7 * y <= 17.5
constraint1 = solver.Constraint(-solver.infinity(), 17.5)
constraint1.SetCoefficient(x, 1)
constraint1.SetCoefficient(y, 7)

# x <= 3.5
constraint2 = solver.Constraint(-solver.infinity(), 3.5)
constraint2.SetCoefficient(x, 1)
constraint2.SetCoefficient(y, 0)

# Maximize x + 10 * y.
objective = solver.Objective()
objective.SetCoefficient(x, 1)
objective.SetCoefficient(y, 10)
objective.SetMaximization()

"""Solve the problem and print the solution."""
result_status = solver.Solve()
# The problem has an optimal solution.
assert result_status == pywraplp.Solver.OPTIMAL

# The solution looks legit (when using solvers other than
# GLOP_LINEAR_PROGRAMMING, verifying the solution is highly recommended!).
assert solver.VerifySolution(1e-7, True)

print('Number of variables =', solver.NumVariables())
print('Number of constraints =', solver.NumConstraints())

# The objective value of the solution.
print('Optimal objective value = %d' % solver.Objective().Value())
print()
# The value of each variable in the solution.
variable_list = [x, y]

for variable in variable_list:
    print('%s = %d' % (variable.name(), variable.solution_value()))
```

输出为

```
Number of variables = 2
Number of constraints = 2
Optimal objective value = 23

x = 3
y = 2
```

# 4. 约束优化

约束优化（constraint programming）和混合整数规划的数学模型并无差异，但是它的求解方法是使用拉格朗日变换去掉约束条件，然后使用诸如牛顿法、梯度法、模拟退火、遗传算法、蚁群算法等方法来快速得到一个**可行解**，而不是传统MIP问题中寻找**最优解**。

ortools可以使用原生CP求解器和CP-SAT求解器。根据官方文档说明，推荐使用CP-SAT求解器。求解status有如下几种可能：OPTIMAL(4)、MODEL_SAT(2)、MODEL_UNSAT(3)、MODEL_INVALID(1)、UNKNOWN(0)。

另外注意CP-SAT目前不支持3.6版本，使用3.6会报错：This program was compiled against version 3.5.1 of the Protocol Buffer runtime library, which is not compatible with the installed version (3.6.1).  Contact the program author for an update.

下面举例说明用法。假设有3个变量x, y, z，取值范围为[0,1,2]，约束条件为x≠y。
```python
from ortools.sat.python import cp_model
# Creates the model.
model = cp_model.CpModel()

# Creates the variables.
num_vals = 3
x = model.NewIntVar(0, num_vals - 1, 'x')
y = model.NewIntVar(0, num_vals - 1, 'y')
z = model.NewIntVar(0, num_vals - 1, 'z')

# Creates the constraints.
model.Add(x != y)

# Creates a solver and solves the model.
solver = cp_model.CpSolver()
status = solver.Solve(model)
print('status = %i' % status)
print('x = %i' % solver.Value(x))
print('y = %i' % solver.Value(y))
print('z = %i' % solver.Value(z))
```

求解结果为：

```
status = 2
x = 1
y = 0
z = 0
```

然后我们添加目标函数：

```python
model.Maximize(x + 2 * y + 3 * z)
```

求解结果为：

```
status = 4
x = 1
y = 2
z = 2
```

假如想获得中间结果，可以使用cp_model.CpSolver.**SolveWithSolutionCallback**(cp_model.CpModel, 回调函数(变量list))。参考如下代码：

```python
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from ortools.sat.python import cp_model
class VarArrayAndObjectiveSolutionPrinter(cp_model.CpSolverSolutionCallback):
    """Print intermediate solutions."""

    def __init__(self, variables):
        cp_model.CpSolverSolutionCallback.__init__(self)
        self.__variables = variables
        self.__solution_count = 0

    def OnSolutionCallback(self):
        print('Solution %i' % self.__solution_count)
        print('  objective value = %i' % self.ObjectiveValue())
        for v in self.__variables:
            print('  %s = %i' % (v, self.Value(v)), end=' ')
        print()
        self.__solution_count += 1

    def SolutionCount(self):
        return self.__solution_count

model = cp_model.CpModel()

# Creates the variables.
num_vals = 3
x = model.NewIntVar(0, num_vals - 1, 'x')
y = model.NewIntVar(0, num_vals - 1, 'y')
z = model.NewIntVar(0, num_vals - 1, 'z')

# Creates the constraints.
model.Add(x != y)

model.Maximize(x + 2 * y + 3 * z)

# Creates a solver and solves.
solver = cp_model.CpSolver()
solution_printer = VarArrayAndObjectiveSolutionPrinter([x, y, z])
status = solver.SolveWithSolutionCallback(model, solution_printer)

print('Status = %s' % solver.StatusName(status))
print('Number of solutions found: %i' % solution_printer.SolutionCount())
```

输出结果为：

```
Solution 0
  objective value = 1
  x = 1   y = 0   z = 0
Solution 1
  objective value = 2
  x = 2   y = 0   z = 0
Solution 2
  objective value = 4
  x = 2   y = 1   z = 0
Solution 3
  objective value = 5
  x = 1   y = 2   z = 0
Solution 4
  objective value = 8
  x = 0   y = 1   z = 2
Solution 5
  objective value = 10
  x = 0   y = 2   z = 2
Solution 6
  objective value = 11
  x = 1   y = 2   z = 2
Status = OPTIMAL
Number of solutions found: 7
```


