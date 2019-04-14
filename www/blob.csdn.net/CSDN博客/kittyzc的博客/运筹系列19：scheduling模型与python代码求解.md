# 运筹系列19：scheduling模型与python代码求解 - kittyzc的博客 - CSDN博客
2018年11月20日 15:02:24[IE06](https://me.csdn.net/kittyzc)阅读数：308所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)
# 1. 问题模型

scheduling问题比assignment问题又要复杂很多。在调度问题中，除了要考虑任务分配外，还要考虑时间约束。

来看一个官方例子：job shop problem。数据如下：

job 0 = [(0, 3), (1, 2), (2, 2)]

job 1 = [(0, 2), (2, 1), (1, 4)]

job 2 = [(1, 4), (2, 3)]

有3台机器编号0、1、2。job数据总每个tuple的第一个数字表示机器编号，第二个数字表示加工小时数，下图是方案之一。使用CP求最短需要花费的时间。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120145922912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)
# 2. 求解代码

使用CP模型进行求解。ortools可以创建NewIntervalVar(start_var, length, end_var)类型的变量，并且可以调用cp_model.AddNoOverlap简单添加无重叠的约束。

在上述问题中，约束条件主要有两个：
- 每台机器上分配的任务的interval不能重叠（同一时间只能加工一个产品）
- 每个产品分配的机器有前后顺序

目标函数是min{max{interval.end_var}}。可以简单调用cp_model.AddMaxEquality来进行表示。

python代码如下：

```python
import collections
from ortools.sat.python import cp_model
# Create the model.
model = cp_model.CpModel()

jobs_data = [  # task = (machine_id, processing_time).
    [(0, 3), (1, 2), (2, 2)],  # Job0
    [(0, 2), (2, 1), (1, 4)],  # Job1
    [(1, 4), (2, 3)]  # Job2
]

machines_count = 1 + max(task[0] for job in jobs_data for task in job)
all_machines = range(machines_count)
jobs_count = len(jobs_data)
all_jobs = range(jobs_count)

# Compute horizon.
horizon = sum(task[1] for job in jobs_data for task in job)

task_type = collections.namedtuple('task_type', 'start end interval')
assigned_task_type = collections.namedtuple('assigned_task_type',
                                            'start job index')

# Create jobs.
all_tasks = {}
for job in all_jobs:
    for task_id, task in enumerate(jobs_data[job]):
        start_var = model.NewIntVar(0, horizon,
                                    'start_%i_%i' % (job, task_id))
        duration = task[1]
        end_var = model.NewIntVar(0, horizon, 'end_%i_%i' % (job, task_id))
        interval_var = model.NewIntervalVar(
            start_var, duration, end_var, 'interval_%i_%i' % (job, task_id))
        all_tasks[job, task_id] = task_type(
            start=start_var, end=end_var, interval=interval_var)

# Create and add disjunctive constraints.
for machine in all_machines:
    intervals = []
    for job in all_jobs:
        for task_id, task in enumerate(jobs_data[job]):
            if task[0] == machine:
                intervals.append(all_tasks[job, task_id].interval)
    model.AddNoOverlap(intervals)

# Add precedence contraints.
for job in all_jobs:
    for task_id in range(0, len(jobs_data[job]) - 1):
        model.Add(all_tasks[job, task_id +
                            1].start >= all_tasks[job, task_id].end)

# Makespan objective.
obj_var = model.NewIntVar(0, horizon, 'makespan')
model.AddMaxEquality(
    obj_var,
    [all_tasks[(job, len(jobs_data[job]) - 1)].end for job in all_jobs])
model.Minimize(obj_var)

# Solve model.
solver = cp_model.CpSolver()
status = solver.Solve(model)

if status == cp_model.OPTIMAL:
    # Print out makespan.
    print('Optimal Schedule Length: %i' % solver.ObjectiveValue())
    print()

    # Create one list of assigned tasks per machine.
    assigned_jobs = [[] for _ in all_machines]
    for job in all_jobs:
        for task_id, task in enumerate(jobs_data[job]):
            machine = task[0]
            assigned_jobs[machine].append(
                assigned_task_type(
                    start=solver.Value(all_tasks[job, task_id].start),
                    job=job,
                    index=task_id))

    disp_col_width = 10
    sol_line = ''
    sol_line_tasks = ''

    print('Optimal Schedule', '\n')

    for machine in all_machines:
        # Sort by starting time.
        assigned_jobs[machine].sort()
        sol_line += 'Machine ' + str(machine) + ': '
        sol_line_tasks += 'Machine ' + str(machine) + ': '

        for assigned_task in assigned_jobs[machine]:
            name = 'job_%i_%i' % (assigned_task.job, assigned_task.index)
            # Add spaces to output to align columns.
            sol_line_tasks += name + ' ' * (disp_col_width - len(name))
            start = assigned_task.start
            duration = jobs_data[assigned_task.job][assigned_task.index][1]

            sol_tmp = '[%i,%i]' % (start, start + duration)
            # Add spaces to output to align columns.
            sol_line += sol_tmp + ' ' * (disp_col_width - len(sol_tmp))

        sol_line += '\n'
        sol_line_tasks += '\n'

    print(sol_line_tasks)
    print('Task Time Intervals\n')
    print(sol_line)
```

输出如下：

```
Optimal Schedule Length: 11

Optimal Schedule

Machine 0: Job_0_0   Job_1_0
Machine 1: Job_2_0   Job_0_1   Job_1_2
Machine 2: Job_1_1   Job_0_2   Job_2_1

Task Time Intervals

Machine 0: [0,3]     [3,5]
Machine 1: [0,4]     [4,6]     [6,10]
Machine 2: [5,6]     [6,8]     [8,11]
```

如下图，用时11小时。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120150212408.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)

