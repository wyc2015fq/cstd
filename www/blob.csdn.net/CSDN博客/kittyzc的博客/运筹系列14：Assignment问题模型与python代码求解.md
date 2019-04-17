# 运筹系列14：Assignment问题模型与python代码求解 - kittyzc的博客 - CSDN博客





2018年11月16日 20:49:29[IE06](https://me.csdn.net/kittyzc)阅读数：168
所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)









# 1. 问题描述

分配问题可以简单描述为：有数个人和数个任务，人做任务有不同的费用。每个人最多只能做一项任务，每个任务只能由一个人做。如何将任务分配给人可以使总费用最小？

用数学语言表示为：
$min$$\Sigma_{i\in I,j\in J}c_{ij}x_{ij}$

s.t.
$\Sigma_{i\in I} x_{ij} =1, \forall j\in J$
$\Sigma_{j\in J} x_{ij} \leq 1, \forall i\in I$

来看一个例子，有4个人和4个任务，费用矩阵如下表：|人\任务|0|1|2|3|
|----|----|----|----|----|
|0|90|76|75|70|
|1|35|85|55|65|
|2|125|95|90|105|
|3|45|110|95|115|

ortools有现成的模型LinearSumAssignment可以使用，下面是python代码：

```python
from ortools.graph import pywrapgraph
cost = [[90, 76, 75, 70],[35, 85, 55, 65],[125, 95, 90, 105],[45, 110, 95, 115]]
rows = len(cost)
cols = len(cost[0])

assignment = pywrapgraph.LinearSumAssignment()
for worker in range(rows):
	for task in range(cols):
		if cost[worker][task]:
			assignment.AddArcWithCost(worker, task, cost[worker][task])
solve_status = assignment.Solve()
if solve_status == assignment.OPTIMAL:
	print('Total cost = ', assignment.OptimalCost())
	print()
	for i in range(0, assignment.NumNodes()):
		print('Worker %d assigned to task %d.  Cost = %d' % (i,assignment.RightMate(i),assignment.AssignmentCost(i)))
elif solve_status == assignment.INFEASIBLE:
	print('No assignment is possible.')
elif solve_status == assignment.POSSIBLE_OVERFLOW:
	print('Some input costs are too large and may cause an integer overflow.')
```

输出结果为：

```
Total cost =  265

Worker 0 assigned to task 3.  Cost = 70
Worker 1 assigned to task 2.  Cost = 55
Worker 2 assigned to task 1.  Cost = 95
Worker 3 assigned to task 0.  Cost = 45
```

# 2. 问题变形1

假设我们有2个团队（每个团队中3个人）和4个任务，要求每个团队分配2个任务使得总费用最小该怎么建模？

这种情况下，我们可以将问题建模为最小费用流问题，如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120105452734.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)

最小费用流的求解方法见上一篇文章。
# 3. 问题变形2

有时候任务本身有一个size的属性，比如每项工作需要一定的时间或费用，任务分配的时候不是限制每个人一个任务，而是要求task_size的和不能超过人能处理的size_max的限制。这里可以使用前面文章提到的CP进行建模求解：

```python
from __future__ import print_function
from ortools.sat.python import cp_model
import numpy as np

model = cp_model.CpModel()
start = time.time()
cost = [[90, 76, 75, 70, 50, 74, 12, 68],
        [35, 85, 55, 65, 48, 101, 70, 83],
        [125, 95, 90, 105, 59, 120, 36, 73],
        [45, 110, 95, 115, 104, 83, 37, 71],
        [60, 105, 80, 75, 59, 62, 93, 88],
        [45, 65, 110, 95, 47, 31, 81, 34],
        [38, 51, 107, 41, 69, 99, 115, 48],
        [47, 85, 57, 71, 92, 77, 109, 36],
        [39, 63, 97, 49, 118, 56, 92, 61],
        [47, 101, 71, 60, 88, 109, 52, 90]]
sizes = [10, 7, 3, 12, 15, 4, 11, 5]
total_size_max = 15
num_workers = len(cost)
num_tasks = len(cost[1])
# Variables
x = []
for i in range(num_workers):
  t = []
  for j in range(num_tasks):
    t.append(model.NewIntVar(0, 1, "x[%i,%i]" % (i, j)))
  x.append(t)
x_array = [x[i][j] for i in range(num_workers) for j in range(num_tasks)]

# Constraints

# Each task is assigned to at least one worker.
[model.Add(sum(x[i][j] for i in range(num_workers)) >= 1)
for j in range(num_tasks)]

# Total size of tasks for each worker is at most total_size_max.

[model.Add(sum(sizes[j] * x[i][j] for j in range(num_tasks)) <= total_size_max)
for i in range(num_workers)]
model.Minimize(sum([np.dot(x_row, cost_row) for (x_row, cost_row) in zip(x, cost)]))
solver = cp_model.CpSolver()
status = solver.Solve(model)

if status == cp_model.OPTIMAL:
  print('Minimum cost = %i' % solver.ObjectiveValue())
  print()

  for i in range(num_workers):
    for j in range(num_tasks):
      if solver.Value(x[i][j]) == 1:
        print('Worker ', i, ' assigned to task ', j, '  Cost = ', cost[i][j])
  print()
  end = time.time()
  print("Time = ", round(end - start, 4), "seconds")
```

输出为：

```
Minimum cost: 326

Worker  0  assigned to task  6   Cost =  12
Worker  1  assigned to task  0   Cost =  35
Worker  1  assigned to task  2   Cost =  55
Worker  2  assigned to task  4   Cost =  59
Worker  5  assigned to task  5   Cost =  31
Worker  5  assigned to task  7   Cost =  34
Worker  6  assigned to task  1   Cost =  51
Worker  8  assigned to task  3   Cost =  49
```

# 4. 使用MIP方法进行求解

一般来说，就求解速度来说LinearSumAssignment<minCostFlow<CP<MIP，而求解问题的范围则是反过来的。这里对上一节的问题使用传统的MIP进行建模求解：

```python
cost = [[90, 76, 75, 70, 50, 74, 12, 68],
          [35, 85, 55, 65, 48, 101, 70, 83],
          [125, 95, 90, 105, 59, 120, 36, 73],
          [45, 110, 95, 115, 104, 83, 37, 71],
          [60, 105, 80, 75, 59, 62, 93, 88],
          [45, 65, 110, 95, 47, 31, 81, 34],
          [38, 51, 107, 41, 69, 99, 115, 48],
          [47, 85, 57, 71, 92, 77, 109, 36],
          [39, 63, 97, 49, 118, 56, 92, 61],
          [47, 101, 71, 60, 88, 109, 52, 90]]
task_sizes = [10, 7, 3, 12, 15, 4, 11, 5]
total_size_max = 15
from ortools.linear_solver import pywraplp
solver = pywraplp.Solver('SolveAssignmentProblem',pywraplp.Solver.CBC_MIXED_INTEGER_PROGRAMMING)

# Maximum total of task sizes for any worker
num_workers = len(cost)
num_tasks = len(cost[1])
# Variables
x = {}
for i in range(num_workers):
    for j in range(num_tasks):
        x[i, j] = solver.IntVar(0, 1, 'x[%i,%i]' % (i, j))

# Constraints
# The total size of the tasks each worker takes on is at most total_size_max.
for i in range(num_workers):
    solver.Add(solver.Sum([task_sizes[j] * x[i, j] for j in range(num_tasks)]) <= total_size_max)

# Each task is assigned to at least one worker.
for j in range(num_tasks):
    solver.Add(solver.Sum([x[i, j] for i in range(num_workers)]) >= 1)

solver.Minimize(solver.Sum([cost[i][j] * x[i,j] for i in range(num_workers) for j in range(num_tasks)]))
sol = solver.Solve()

print('Minimum cost = ', solver.Objective().Value())
print()
for i in range(num_workers):
    for j in range(num_tasks):
        if x[i, j].solution_value() > 0:
            print('Worker', i,' assigned to task', j, '  Cost = ', cost[i][j])
```

结果为：

```
Minimum cost =  326.0

Worker 0  assigned to task 6   Cost =  12
Worker 1  assigned to task 0   Cost =  35
Worker 1  assigned to task 2   Cost =  55
Worker 4  assigned to task 4   Cost =  59
Worker 5  assigned to task 5   Cost =  31
Worker 5  assigned to task 7   Cost =  34
Worker 6  assigned to task 1   Cost =  51
Worker 8  assigned to task 3   Cost =  49
```





