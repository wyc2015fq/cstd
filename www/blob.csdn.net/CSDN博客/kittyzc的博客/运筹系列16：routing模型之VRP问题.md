# 运筹系列16：routing模型之VRP问题 - kittyzc的博客 - CSDN博客
2018年11月20日 21:28:33[IE06](https://me.csdn.net/kittyzc)阅读数：621所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)
# 1. 问题模型

VRP问题是车辆路径问题的缩写。问题是：有N辆车，都从原点出发，每辆车访问一些点后回到原点，要求所有的点都要被访问到，求最短的车辆行驶距离或最少需要的车辆数或最小化最长行驶距离。

常见的限制要求包括：车辆容量限制、时间窗限制、点访问顺序要求等。

# 2. RoutingModel的一些说明

对于VRP问题，我们可以使用ortools的RoutingModel模型。首先为车添加用于进行优化的dimension：
|方法|输入|说明|
|----|----|----|
|AddDimension|evaluator, slack_max, capacity, fix_start_cumul_to_zero, name|Creates a dimension where the transit variable is constrained to be equal to evaluator(i, next(i)); ‘slack_max’ is the upper bound of the slack variable and ‘capacity’ is the upper bound of the cumul variables. ‘name’ is the name used to reference the dimension; this name is used to get cumul and transit variables from the routing model. Returns false if a dimension with the same name has already been created (and doesn’t create the new dimension). Takes ownership of the callback ‘evaluator’.|

然后使用GetDimensionOrDie方法获取dimension，并设置优化目标。注意在VRP问题中，路径上给点赋的index和点实际的index不一样，需要使用IndexToNode方法进行转换才能得到实际的index。

# 3. 例子

先看一个简单的例子：距离用曼哈顿距离，目标函数是最小化各车辆行驶距离的差别。可以对dimension使用SetGlobalSpanCostCoefficient方法可以获得目标函数。global_span_cost = coefficient * (Max(dimension end value) - Min(dimension start value)).

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120202033404.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)

代码如下：

```python
from __future__ import print_function
from ortools.constraint_solver import pywrapcp
from ortools.constraint_solver import routing_enums_pb2

###########################
# Problem Data Definition #
###########################
def create_data_model():
  """Stores the data for the problem"""
  data = {}
  # Locations in block units
  _locations = \
          [(4, 4), # depot
           (2, 0), (8, 0), # locations to visit
           (0, 1), (1, 1),
           (5, 2), (7, 2),
           (3, 3), (6, 3),
           (5, 5), (8, 5),
           (1, 6), (2, 6),
           (3, 7), (6, 7),
           (0, 8), (7, 8)]
  # Multiply coordinates in block units by the dimensions of an average city block, 114m x 80m,
  # to get location coordinates.
  data["locations"] = [(l[0] * 114, l[1] * 80) for l in _locations]
  data["num_locations"] = len(data["locations"])
  data["num_vehicles"] = 4
  data["depot"] = 0
  return data
#######################
# Problem Constraints #
#######################
def manhattan_distance(position_1, position_2):
  """Computes the Manhattan distance between two points"""
  return (
      abs(position_1[0] - position_2[0]) + abs(position_1[1] - position_2[1]))
def create_distance_callback(data):
  """Creates callback to return distance between points."""
  _distances = {}

  for from_node in range(data["num_locations"]):
    _distances[from_node] = {}
    for to_node in range(data["num_locations"]):
      if from_node == to_node:
        _distances[from_node][to_node] = 0
      else:
        _distances[from_node][to_node] = (
            manhattan_distance(data["locations"][from_node],
                               data["locations"][to_node]))

  def distance_callback(from_node, to_node):
    """Returns the manhattan distance between the two nodes"""
    return _distances[from_node][to_node]

  return distance_callback
def add_distance_dimension(routing, distance_callback):
  """Add Global Span constraint"""
  distance = 'Distance'
  maximum_distance = 3000  # Maximum distance per vehicle.
  routing.AddDimension(
      distance_callback,
      0,  # null slack
      maximum_distance,
      True,  # start cumul to zero
      distance)
  distance_dimension = routing.GetDimensionOrDie(distance)
  # Try to minimize the max distance among vehicles.
  distance_dimension.SetGlobalSpanCostCoefficient(100)
###########
# Printer #
###########
def print_solution(data, routing, assignment):
  """Print routes on console."""
  total_distance = 0
  for vehicle_id in range(data["num_vehicles"]):
    index = routing.Start(vehicle_id)
    plan_output = 'Route for vehicle {}:\n'.format(vehicle_id)
    distance = 0
    while not routing.IsEnd(index):
      plan_output += ' {} ->'.format(routing.IndexToNode(index))
      previous_index = index
      index = assignment.Value(routing.NextVar(index))
      distance += routing.GetArcCostForVehicle(previous_index, index, vehicle_id)
    plan_output += ' {}\n'.format(routing.IndexToNode(index))
    plan_output += 'Distance of route: {}m\n'.format(distance)
    print(plan_output)
    total_distance += distance
  print('Total distance of all routes: {}m'.format(total_distance))
########
# Main #
########
def main():
  """Entry point of the program"""
  # Instantiate the data problem.
  data = create_data_model()
  # Create Routing Model
  routing = pywrapcp.RoutingModel(
      data["num_locations"],
      data["num_vehicles"],
      data["depot"])
  # Define weight of each edge
  distance_callback = create_distance_callback(data)
  routing.SetArcCostEvaluatorOfAllVehicles(distance_callback)
  add_distance_dimension(routing, distance_callback)
  # Setting first solution heuristic (cheapest addition).
  search_parameters = pywrapcp.RoutingModel.DefaultSearchParameters()
  search_parameters.first_solution_strategy = (
      routing_enums_pb2.FirstSolutionStrategy.PATH_CHEAPEST_ARC) # pylint: disable=no-member
  # Solve the problem.
  assignment = routing.SolveWithParameters(search_parameters)
  if assignment:
    print_solution(data, routing, assignment)
if __name__ == '__main__':
  main()
```

输出为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120212722597.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)

结果如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120212636192.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)
