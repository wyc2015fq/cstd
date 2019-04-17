# 运筹系列15：routing模型之TSP问题 - kittyzc的博客 - CSDN博客





2018年11月20日 17:42:34[IE06](https://me.csdn.net/kittyzc)阅读数：272
个人分类：[运筹学																[python](https://blog.csdn.net/kittyzc/article/category/7940149)](https://blog.csdn.net/kittyzc/article/category/7940150)

所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)








# 1. 问题模型

TSP问题是旅行商问题的简写，问题非常简单：从原点出发经过所有需求点并回到原点，使得途经的距离最短。

ortools可以使用RoutingModel来进行求解。

# 2. 求解代码

首先建立RoutingModel模型：

```
routing = pywrapcp.RoutingModel(tsp_size, num_routes, depot)
```

为SetArcCostEvaluatorOfAllVehicles方法传入一个计算距离的回调函数，然后使用默认的参数pywrapcp.RoutingModel.DefaultSearchParameters()，就可以使用SolveWithParameters进行求解了。注意使用这个参数的话，给出的是一个可行初始解，而不是最优解。

代码如下

```python
from ortools.constraint_solver import pywrapcp
from ortools.constraint_solver import routing_enums_pb2

# Cities
city_names = ["New York", "Los Angeles", "Chicago", "Minneapolis", "Denver", "Dallas", "Seattle",
            "Boston", "San Francisco", "St. Louis", "Houston", "Phoenix", "Salt Lake City"]
# Distance matrix
dist_matrix = [
[   0, 2451,  713, 1018, 1631, 1374, 2408,  213, 2571,  875, 1420, 2145, 1972], # New York
[2451,    0, 1745, 1524,  831, 1240,  959, 2596,  403, 1589, 1374,  357,  579], # Los Angeles
[ 713, 1745,    0,  355,  920,  803, 1737,  851, 1858,  262,  940, 1453, 1260], # Chicago
[1018, 1524,  355,    0,  700,  862, 1395, 1123, 1584,  466, 1056, 1280,  987], # Minneapolis
[1631,  831,  920,  700,    0,  663, 1021, 1769,  949,  796,  879,  586,  371], # Denver
[1374, 1240,  803,  862,  663,    0, 1681, 1551, 1765,  547,  225,  887,  999], # Dallas
[2408,  959, 1737, 1395, 1021, 1681,    0, 2493,  678, 1724, 1891, 1114,  701], # Seattle
[ 213, 2596,  851, 1123, 1769, 1551, 2493,    0, 2699, 1038, 1605, 2300, 2099], # Boston
[2571,  403, 1858, 1584,  949, 1765,  678, 2699,    0, 1744, 1645,  653,  600], # San Francisco
[ 875, 1589,  262,  466,  796,  547, 1724, 1038, 1744,    0,  679, 1272, 1162], # St. Louis
[1420, 1374,  940, 1056,  879,  225, 1891, 1605, 1645,  679,    0, 1017, 1200], # Houston
[2145,  357, 1453, 1280,  586,  887, 1114, 2300,  653, 1272, 1017,    0,  504], # Phoenix
[1972,  579, 1260,  987,  371,  999,  701, 2099,  600, 1162,  1200,  504,   0]] # Salt Lake City

tsp_size = len(city_names)
num_routes = 1
depot = 0

routing = pywrapcp.RoutingModel(tsp_size, num_routes, depot)
search_parameters = pywrapcp.RoutingModel.DefaultSearchParameters()
dist = lambda from_node,to_node:int(dist_matrix[from_node][to_node])
routing.SetArcCostEvaluatorOfAllVehicles(dist)
# Solve the problem.
assignment = routing.SolveWithParameters(search_parameters)
if assignment:
    # Solution distance.
    print("Total distance: " + str(assignment.ObjectiveValue()) + " miles\n")
    # Display the solution.
    # Only one route here; otherwise iterate from 0 to routing.vehicles() - 1
    route_number = 0
    index = routing.Start(route_number) # Index of the variable for the starting node.
    route = ''
    while not routing.IsEnd(index):
        # Convert variable indices to node indices in the displayed route.
        route += str(city_names[routing.IndexToNode(index)]) + ' -> '
        index = assignment.Value(routing.NextVar(index))
    route += str(city_names[routing.IndexToNode(index)])
    print("Route:\n\n" + route)
```

输出为：

```
Total distance: 7293 miles

Route:

New York -> Boston -> Chicago -> Minneapolis -> Denver -> Salt Lake City -> Seattle -> San Francisco -> Los Angeles -> Phoenix -> Houston -> Dallas -> St. Louis -> New York
```

可以手动设定初始可行解的求解算法：

```python
search_parameters.first_solution_strategy = (routing_enums_pb2.FirstSolutionStrategy.PATH_CHEAPEST_ARC)
```

清单如下：
|Option|Description|
|----|----|
|AUTOMATIC|Lets the solver detect which strategy to use according to the model being solved.|
|PATH_CHEAPEST_ARC|Starting from a route “start” node, connect it to the node which produces the cheapest route segment, then extend the route by iterating on the last node added to the route.|
|PATH_MOST_CONSTRAINED_ARC|Similar to PATH_CHEAPEST_ARC, but arcs are evaluated with a comparison-based selector which will favor the most constrained arc first. To assign a selector to the routing model, use the method ArcIsMoreConstrainedThanArc().|
|EVALUATOR_STRATEGY|Similar to PATH_CHEAPEST_ARC, except that arc costs are evaluated using the function passed to SetFirstSolutionEvaluator().|
|SAVINGS|Savings algorithm (Clarke & Wright). Reference: Clarke, G. & Wright, J.W.: “Scheduling of Vehicles from a Central Depot to a Number of Delivery Points”, Operations Research, Vol. 12, 1964, pp. 568-581.|
|SWEEP|Sweep algorithm (Wren & Holliday). Reference: Anthony Wren & Alan Holliday: Computer Scheduling of Vehicles from One or More Depots to a Number of Delivery Points Operational Research Quarterly (1970-1977), Vol. 23, No. 3 (Sep., 1972), pp. 333-344.|
|CHRISTOFIDES|Christofides algorithm (actually a variant of the Christofides algorithm using a maximal matching instead of a maximum matching, which does not guarantee the 3/2 factor of the approximation on a metric travelling salesman). Works on generic vehicle routing models by extending a route until no nodes can be inserted on it. Reference: Nicos Christofides, Worst-case analysis of a new heuristic for the travelling salesman problem, Report 388, Graduate School of Industrial Administration, CMU, 1976.|
|ALL_UNPERFORMED|Make all nodes inactive. Only finds a solution if nodes are optional (are element of a disjunction constraint with a finite penalty cost).|
|BEST_INSERTION|Iteratively build a solution by inserting the cheapest node at its cheapest position; the cost of insertion is based on the global cost function of the routing model. As of 2/2012, only works on models with optional nodes (with finite penalty costs).|
|PARALLEL_CHEAPEST_INSERTION|Iteratively build a solution by inserting the cheapest node at its cheapest position; the cost of insertion is based on the the arc cost function. Is faster than BEST_INSERTION.|
|LOCAL_CHEAPEST_INSERTION|Iteratively build a solution by inserting each node at its cheapest position; the cost of insertion is based on the the arc cost function. Differs from|
|GLOBAL_CHEAPEST_ARC|Iteratively connect two nodes which produce the cheapest route segment.|
|LOCAL_CHEAPEST_ARC|Select the first node with an unbound successor and connect it to the node which produces the cheapest route segment.|
|FIRST_UNBOUND_MIN_VALUE|Select the first node with an unbound successor and connect it to the first available node. This is equivalent to the CHOOSE_FIRST_UNBOUND strategy combined with ASSIGN_MIN_VALUE (cf. constraint_solver.h).|

返回值的清单如下：
|Value|Description|
|----|----|
|0|ROUTING_NOT_SOLVED: Problem not solved yet.|
|1|ROUTING_SUCCESS: Problem solved successfully.|
|2|ROUTING_FAIL: No solution found to the problem.|
|3|ROUTING_FAIL_TIMEOUT: Time limit reached before finding a solution.|
|4|ROUTING_INVALID: Model, model parameters, or flags are not valid.|

# 3. 第二阶段算法：local search

使用local search可以在初始可行解的基础上进行优化，如下：

```python
search_parameters.local_search_metaheuristic = (
        routing_enums_pb2.LocalSearchMetaheuristic.GUIDED_LOCAL_SEARCH)
 search_parameters.time_limit_ms = 30000 # 30s
```

可使用的清单如下：
|Option|Description|
|----|----|
|AUTOMATIC|Lets the solver select the metaheuristic.|
|GREEDY_DESCENT|Accepts improving (cost-reducing) local search neighbors until a local minimum is reached.|
|GUIDED_LOCAL_SEARCH|Uses guided local search to escape local minima (cf. [http://en.wikipedia.org/wiki/Guided_Local_Search](http://en.wikipedia.org/wiki/Guided_Local_Search)); this is generally the most efficient metaheuristic for vehicle routing.|
|SIMULATED_ANNEALING|Uses simulated annealing to escape local minima (cf. [http://en.wikipedia.org/wiki/Simulated_annealing](http://en.wikipedia.org/wiki/Simulated_annealing)).|
|TABU_SEARCH|Uses tabu search to escape local minima (cf. [http://en.wikipedia.org/wiki/Tabu_search](http://en.wikipedia.org/wiki/Tabu_search)).|
|OBJECTIVE_TABU_SEARCH|Uses tabu search on the objective value of solution to escape local minima|




