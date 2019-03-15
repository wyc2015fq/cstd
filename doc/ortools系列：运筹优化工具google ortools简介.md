# ortools系列：运筹优化工具google ortools简介

[![master苏](https://pic2.zhimg.com/v2-ea1ab69866b8aa701149291670747657_xs.jpg)](https://www.zhihu.com/people/zhenyu22)

[master苏](https://www.zhihu.com/people/zhenyu22)

智慧供应链算法工程师，人生规划导师

6 人赞同了该文章

**ortools系列：运筹优化工具google ortools简介**

- [1. 前言](https://zhuanlan.zhihu.com/p/55089642/edit#1-%E5%89%8D%E8%A8%80)
- [2. 什么是运筹优化求解器](https://zhuanlan.zhihu.com/p/55089642/edit#2-%E4%BB%80%E4%B9%88%E6%98%AF%E8%BF%90%E7%AD%B9%E4%BC%98%E5%8C%96%E6%B1%82%E8%A7%A3%E5%99%A8)
- [3. google ortools 简介](https://zhuanlan.zhihu.com/p/55089642/edit#3-google-ortools-%E7%AE%80%E4%BB%8B)
- [一个简单的例子](https://zhuanlan.zhihu.com/p/55089642/edit#%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E4%BE%8B%E5%AD%90)
- [4. 参考](https://zhuanlan.zhihu.com/p/55089642/edit#4-%E5%8F%82%E8%80%83)

## 1. 前言

了解运筹学的同学都知道，虽然运筹优化的求解思路很简答，但是要得到一个满意或者堪用的结果却是很难的事情，原因无他，只是因为现在的求解思路有点像暴力搜索，而求解的空间又是非常巨大的，根本不能在约束的时间内得到比较好的结果。尽管后来发展了很多有用的算法和理论，但是如果细细查看其理论本身，思路就是暴力搜索的改进，比如分支定界法或者割平面法，以及其他启发式算法，都是为了减少搜索空间，快速得到一个可行解。

如果是凸优化还好，比如线性规划，一般可以直接得到最优解，如果是非凸优化就很难说了，既不知道是全局最优解也不知道是局部最优解，非常尴尬。对于非凸优化问题，在业界使用比较到的是像遗传算法等这类启发式算法，结果也还过得去。

运筹优化问题，理论是不难的，求解问题才难，难就难在怎么优化代码，怎么设计搜索路径，怎么设计数据结构和存储等，这些都不是搞优化问题的人的优势，于是各种求解器就出来了，包括我们即将学习的`google ortools`。

## 2. 什么是运筹优化求解器

前面说了，运筹问题的求解思路不难，基本上是把目标函数写出来，把各种约束条件写出来，基本就把问题说明白了，但是编程始终是个问题。于是，一些商业公司就组织了一帮PHD专门开发这类的软件，比较有名的有`IBM ILOG Cplex`,`Gurobi`,`FICO Xpress`,`MOSEK`等，也有一些专门针对具体领域定制的求解器。商业求解器的好处是计算速度快，能解决问题的规模也更大，缺点是死贵死贵，普通企业根本用不起。

开源的求解器也有一些，比如 德国柏林ZIB研究中心的`SCIP`, `GLPK`,`LP_Solve`,COIN-OR旗下的`CBC`和`SYMPHONY`，Google开发的`ortools`，国内的杉树科技也有一个开源求解器`LEAVES`，但是杉树忙着搞商业化，对开源的`LEAVES`似乎不是很上心，支持的功能不多。对于无约束优化问题，很多软件如`MATLAB`,`SCIPY`也提供了很好用的函数调用。

开源求解器毕竟不挣钱，所以在计算性能和问题规模上也不能苛责太多，不多对于大部分中小企业也够用了。

各个求解器的比较可以参考这篇文章：[运筹学数学规划|离散优化求解器大搜罗](https://zhuanlan.zhihu.com/p/40579450)，讲得比较详细了。

## 3. google ortools 简介

从ortools的页面上看到它是这样说的：

OR-Tools是一个用于优化的开源软件套件，用于解决车辆路径、流程、整数和线性规划以及约束编程等世界上最棘手的问题。

同时OR-Tools提供了C++,Python,Java,.NET的接口，同时提供统一接口封装来调用商业求解器如Gurobi, CPLEX等，也包括开源求解器如SCIP, GLPK, ortools等。提供运筹优化工具统一接口的概念和`coin-or`正在做的事情有点像呢。

当前ortools提供的优化器包括： - 约束规划 - 线性与混合整数规划 - 路径规划 - 调度规划 - 网络规划 - 装箱

通过查阅官方提供的案列，基本能解决大部分的优化问题，美中不足的是，装箱问题提供的只要背包问题解法，对于三维装箱问题，需要使用约束规划来求解。

ortools文档托管在`developers.google.cn`上，需要越墙，所以我把官方文档copy了一份到github上，地址是：[google_ortools_guide](http://link.zhihu.com/?target=https%3A//github.com/suzhenyu22/google_ortools_guide)

## 一个简单的例子

这里我们以一个简单的线性规划的例子让大家对ortools有个基本了解，和大部分优化求解器的使用方法差不多，定义变量，定义约束，定义目标函数，调用求解，查看结果。

**问题背景**：有76中食物 ![food_i](https://www.zhihu.com/equation?tex=food_i) ，每种食物单位重量或体积包含j中营养物质 ![nutrient{{\rm{s}}_{ij}}](https://www.zhihu.com/equation?tex=nutrient%7B%7B%5Crm%7Bs%7D%7D_%7Bij%7D%7D) ，每种食物的价格是 ![price_i](https://www.zhihu.com/equation?tex=price_i) ，每年需要的各项营养物质的总量是 ![need_j](https://www.zhihu.com/equation?tex=need_j) .

**求**：如何搭配食物，既能满足营养成分的需求，又能花最少的钱。

用公式表示就是： 假设每样食物需求量是 ![w_i](https://www.zhihu.com/equation?tex=w_i) ,

目标函数： ![ \min :{w_i} \times pric{e_i}](https://www.zhihu.com/equation?tex=+%5Cmin+%3A%7Bw_i%7D+%5Ctimes+pric%7Be_i%7D)

约束：

![nee{d_i} \ge \sum\limits_{i = 1}^n {{w_i} \times foo{d_i}}](https://www.zhihu.com/equation?tex=nee%7Bd_i%7D+%5Cge+%5Csum%5Climits_%7Bi+%3D+1%7D%5En+%7B%7Bw_i%7D+%5Ctimes+foo%7Bd_i%7D%7D)

![{w_i} \ge 0 ](https://www.zhihu.com/equation?tex=%7Bw_i%7D+%5Cge+0+)

```python
from ortools.linear_solver import pywraplp


def main():
    # Commodity, Unit, 1939 price (cents), Calories, Protein (g), Calcium (g), Iron (mg),
    # Vitamin A (IU), Thiamine (mg), Riboflavin (mg), Niacin (mg), Ascorbic Acid (mg)
    data = [
        ['Wheat Flour (Enriched)', '10 lb.', 36, 44.7, 1411, 2, 365, 0, 55.4, 33.3, 441, 0],
        ['Macaroni', '1 lb.', 14.1, 11.6, 418, 0.7, 54, 0, 3.2, 1.9, 68, 0],
        ['Wheat Cereal (Enriched)', '28 oz.', 24.2, 11.8, 377, 14.4, 175, 0, 14.4, 8.8, 114, 0],
        ['Corn Flakes', '8 oz.', 7.1, 11.4, 252, 0.1, 56, 0, 13.5, 2.3, 68, 0],
        ['Corn Meal', '1 lb.', 4.6, 36.0, 897, 1.7, 99, 30.9, 17.4, 7.9, 106, 0],
        ['Hominy Grits', '24 oz.', 8.5, 28.6, 680, 0.8, 80, 0, 10.6, 1.6, 110, 0],
        ['Rice', '1 lb.', 7.5, 21.2, 460, 0.6, 41, 0, 2, 4.8, 60, 0],
        ['Rolled Oats', '1 lb.', 7.1, 25.3, 907, 5.1, 341, 0, 37.1, 8.9, 64, 0],
        ['White Bread (Enriched)', '1 lb.', 7.9, 15.0, 488, 2.5, 115, 0, 13.8, 8.5, 126, 0],
        ['Whole Wheat Bread', '1 lb.', 9.1, 12.2, 484, 2.7, 125, 0, 13.9, 6.4, 160, 0],
        ['Rye Bread', '1 lb.', 9.1, 12.4, 439, 1.1, 82, 0, 9.9, 3, 66, 0],
        ['Pound Cake', '1 lb.', 24.8, 8.0, 130, 0.4, 31, 18.9, 2.8, 3, 17, 0],
        ['Soda Crackers', '1 lb.', 15.1, 12.5, 288, 0.5, 50, 0, 0, 0, 0, 0],
        ['Milk', '1 qt.', 11, 6.1, 310, 10.5, 18, 16.8, 4, 16, 7, 177],
        ['Evaporated Milk (can)', '14.5 oz.', 6.7, 8.4, 422, 15.1, 9, 26, 3, 23.5, 11, 60],
        ['Butter', '1 lb.', 30.8, 10.8, 9, 0.2, 3, 44.2, 0, 0.2, 2, 0],
        ['Oleomargarine', '1 lb.', 16.1, 20.6, 17, 0.6, 6, 55.8, 0.2, 0, 0, 0],
        ['Eggs', '1 doz.', 32.6, 2.9, 238, 1.0, 52, 18.6, 2.8, 6.5, 1, 0],
        ['Cheese (Cheddar)', '1 lb.', 24.2, 7.4, 448, 16.4, 19, 28.1, 0.8, 10.3, 4, 0],
        ['Cream', '1/2 pt.', 14.1, 3.5, 49, 1.7, 3, 16.9, 0.6, 2.5, 0, 17],
        ['Peanut Butter', '1 lb.', 17.9, 15.7, 661, 1.0, 48, 0, 9.6, 8.1, 471, 0],
        ['Mayonnaise', '1/2 pt.', 16.7, 8.6, 18, 0.2, 8, 2.7, 0.4, 0.5, 0, 0],
        ['Crisco', '1 lb.', 20.3, 20.1, 0, 0, 0, 0, 0, 0, 0, 0],
        ['Lard', '1 lb.', 9.8, 41.7, 0, 0, 0, 0.2, 0, 0.5, 5, 0],
        ['Sirloin Steak', '1 lb.', 39.6, 2.9, 166, 0.1, 34, 0.2, 2.1, 2.9, 69, 0],
        ['Round Steak', '1 lb.', 36.4, 2.2, 214, 0.1, 32, 0.4, 2.5, 2.4, 87, 0],
        ['Rib Roast', '1 lb.', 29.2, 3.4, 213, 0.1, 33, 0, 0, 2, 0, 0],
        ['Chuck Roast', '1 lb.', 22.6, 3.6, 309, 0.2, 46, 0.4, 1, 4, 120, 0],
        ['Plate', '1 lb.', 14.6, 8.5, 404, 0.2, 62, 0, 0.9, 0, 0, 0],
        ['Liver (Beef)', '1 lb.', 26.8, 2.2, 333, 0.2, 139, 169.2, 6.4, 50.8, 316, 525],
        ['Leg of Lamb', '1 lb.', 27.6, 3.1, 245, 0.1, 20, 0, 2.8, 3.9, 86, 0],
        ['Lamb Chops (Rib)', '1 lb.', 36.6, 3.3, 140, 0.1, 15, 0, 1.7, 2.7, 54, 0],
        ['Pork Chops', '1 lb.', 30.7, 3.5, 196, 0.2, 30, 0, 17.4, 2.7, 60, 0],
        ['Pork Loin Roast', '1 lb.', 24.2, 4.4, 249, 0.3, 37, 0, 18.2, 3.6, 79, 0],
        ['Bacon', '1 lb.', 25.6, 10.4, 152, 0.2, 23, 0, 1.8, 1.8, 71, 0],
        ['Ham, smoked', '1 lb.', 27.4, 6.7, 212, 0.2, 31, 0, 9.9, 3.3, 50, 0],
        ['Salt Pork', '1 lb.', 16, 18.8, 164, 0.1, 26, 0, 1.4, 1.8, 0, 0],
        ['Roasting Chicken', '1 lb.', 30.3, 1.8, 184, 0.1, 30, 0.1, 0.9, 1.8, 68, 46],
        ['Veal Cutlets', '1 lb.', 42.3, 1.7, 156, 0.1, 24, 0, 1.4, 2.4, 57, 0],
        ['Salmon, Pink (can)', '16 oz.', 13, 5.8, 705, 6.8, 45, 3.5, 1, 4.9, 209, 0],
        ['Apples', '1 lb.', 4.4, 5.8, 27, 0.5, 36, 7.3, 3.6, 2.7, 5, 544],
        ['Bananas', '1 lb.', 6.1, 4.9, 60, 0.4, 30, 17.4, 2.5, 3.5, 28, 498],
        ['Lemons', '1 doz.', 26, 1.0, 21, 0.5, 14, 0, 0.5, 0, 4, 952],
        ['Oranges', '1 doz.', 30.9, 2.2, 40, 1.1, 18, 11.1, 3.6, 1.3, 10, 1998],
        ['Green Beans', '1 lb.', 7.1, 2.4, 138, 3.7, 80, 69, 4.3, 5.8, 37, 862],
        ['Cabbage', '1 lb.', 3.7, 2.6, 125, 4.0, 36, 7.2, 9, 4.5, 26, 5369],
        ['Carrots', '1 bunch', 4.7, 2.7, 73, 2.8, 43, 188.5, 6.1, 4.3, 89, 608],
        ['Celery', '1 stalk', 7.3, 0.9, 51, 3.0, 23, 0.9, 1.4, 1.4, 9, 313],
        ['Lettuce', '1 head', 8.2, 0.4, 27, 1.1, 22, 112.4, 1.8, 3.4, 11, 449],
        ['Onions', '1 lb.', 3.6, 5.8, 166, 3.8, 59, 16.6, 4.7, 5.9, 21, 1184],
        ['Potatoes', '15 lb.', 34, 14.3, 336, 1.8, 118, 6.7, 29.4, 7.1, 198, 2522],
        ['Spinach', '1 lb.', 8.1, 1.1, 106, 0, 138, 918.4, 5.7, 13.8, 33, 2755],
        ['Sweet Potatoes', '1 lb.', 5.1, 9.6, 138, 2.7, 54, 290.7, 8.4, 5.4, 83, 1912],
        ['Peaches (can)', 'No. 2 1/2', 16.8, 3.7, 20, 0.4, 10, 21.5, 0.5, 1, 31, 196],
        ['Pears (can)', 'No. 2 1/2', 20.4, 3.0, 8, 0.3, 8, 0.8, 0.8, 0.8, 5, 81],
        ['Pineapple (can)', 'No. 2 1/2', 21.3, 2.4, 16, 0.4, 8, 2, 2.8, 0.8, 7, 399],
        ['Asparagus (can)', 'No. 2', 27.7, 0.4, 33, 0.3, 12, 16.3, 1.4, 2.1, 17, 272],
        ['Green Beans (can)', 'No. 2', 10, 1.0, 54, 2, 65, 53.9, 1.6, 4.3, 32, 431],
        ['Pork and Beans (can)', '16 oz.', 7.1, 7.5, 364, 4, 134, 3.5, 8.3, 7.7, 56, 0],
        ['Corn (can)', 'No. 2', 10.4, 5.2, 136, 0.2, 16, 12, 1.6, 2.7, 42, 218],
        ['Peas (can)', 'No. 2', 13.8, 2.3, 136, 0.6, 45, 34.9, 4.9, 2.5, 37, 370],
        ['Tomatoes (can)', 'No. 2', 8.6, 1.3, 63, 0.7, 38, 53.2, 3.4, 2.5, 36, 1253],
        ['Tomato Soup (can)', '10 1/2 oz.', 7.6, 1.6, 71, 0.6, 43, 57.9, 3.5, 2.4, 67, 862],
        ['Peaches, Dried', '1 lb.', 15.7, 8.5, 87, 1.7, 173, 86.8, 1.2, 4.3, 55, 57],
        ['Prunes, Dried', '1 lb.', 9, 12.8, 99, 2.5, 154, 85.7, 3.9, 4.3, 65, 257],
        ['Raisins, Dried', '15 oz.', 9.4, 13.5, 104, 2.5, 136, 4.5, 6.3, 1.4, 24, 136],
        ['Peas, Dried', '1 lb.', 7.9, 20.0, 1367, 4.2, 345, 2.9, 28.7, 18.4, 162, 0],
        ['Lima Beans, Dried', '1 lb.', 8.9, 17.4, 1055, 3.7, 459, 5.1, 26.9, 38.2, 93, 0],
        ['Navy Beans, Dried', '1 lb.', 5.9, 26.9, 1691, 11.4, 792, 0, 38.4, 24.6, 217, 0],
        ['Coffee', '1 lb.', 22.4, 0, 0, 0, 0, 0, 4, 5.1, 50, 0],
        ['Tea', '1/4 lb.', 17.4, 0, 0, 0, 0, 0, 0, 2.3, 42, 0],
        ['Cocoa', '8 oz.', 8.6, 8.7, 237, 3, 72, 0, 2, 11.9, 40, 0],
        ['Chocolate', '8 oz.', 16.2, 8.0, 77, 1.3, 39, 0, 0.9, 3.4, 14, 0],
        ['Sugar', '10 lb.', 51.7, 34.9, 0, 0, 0, 0, 0, 0, 0, 0],
        ['Corn Syrup', '24 oz.', 13.7, 14.7, 0, 0.5, 74, 0, 0, 0, 5, 0],
        ['Molasses', '18 oz.', 13.6, 9.0, 0, 10.3, 244, 0, 1.9, 7.5, 146, 0],
        ['Strawberry Preserves', '1 lb.', 20.5, 6.4, 11, 0.4, 7, 0.2, 0.2, 0.4, 3, 0]];

    # 需要的各种营养素最小需求量
    nutrients = [
        ['Calories (1000s)', 3],
        ['Protein (grams)', 70],
        ['Calcium (grams)', 0.8],
        ['Iron (mg)', 12],
        ['Vitamin A (1000 IU)', 5],
        ['Vitamin B1 (mg)', 1.8],
        ['Vitamin B2 (mg)', 2.7],
        ['Niacin (mg)', 18],
        ['Vitamin C (mg)', 75]]

    # 初始化求解器
    solver = pywraplp.Solver('SolveStigler',
                             pywraplp.Solver.GLOP_LINEAR_PROGRAMMING)
    # 
    food = [[]] * len(data)

    # Objective: minimize the sum of (price-normalized) foods.
    objective = solver.Objective()
    for i in range(0, len(data)):
        food[i] = solver.NumVar(0.0, solver.infinity(), data[i][0])     # 定义变量：最小值，最大值，名称
        objective.SetCoefficient(food[i], 1)                            # 定义目标函数的变量的系数

    # 定义求最大值还是最小值
    objective.SetMinimization()

    # 定义约束
    constraints = [0] * len(nutrients)
    for i in range(0, len(nutrients)):
        constraints[i] = solver.Constraint(nutrients[i][1], solver.infinity())
        for j in range(0, len(data)):
            constraints[i].SetCoefficient(food[j], data[j][i + 3])

    # 求解 
    status = solver.Solve()

    # 打印结果
    if status == solver.OPTIMAL:
        # Display the amounts (in dollars) to purchase of each food.
        price = 0
        num_nutrients = len(data[i]) - 3
        nutrients = [0] * (len(data[i]) - 3)
        for i in range(0, len(data)):
            price += food[i].solution_value()

            for nutrient in range(0, num_nutrients):
                nutrients[nutrient] += data[i][nutrient + 3] * food[i].solution_value()

            if food[i].solution_value() > 0:
                print("%s = %f" % (data[i][0], food[i].solution_value()))

        print('Optimal annual price: $%.2f' % (365 * price))
    else:  # No optimal solution was found.
        if status == solver.FEASIBLE:
            print('A potentially suboptimal solution was found.')
        else:
            print('The solver could not solve the problem.')


if __name__ == '__main__':
    main()
```

## 4. 参考

- [运筹学数学规划|离散优化求解器大搜罗](https://zhuanlan.zhihu.com/p/40579450)
- [杉树科技](http://link.zhihu.com/?target=https%3A//www.shanshu.ai/product/leaves)
- [www.coin-or.org](http://link.zhihu.com/?target=https%3A//www.coin-or.org/projects/%23ffs-tabbed-118)
- [google_ortools_guide](http://link.zhihu.com/?target=https%3A//github.com/suzhenyu22/google_ortools_guide)

大家看完记得关注点赞.