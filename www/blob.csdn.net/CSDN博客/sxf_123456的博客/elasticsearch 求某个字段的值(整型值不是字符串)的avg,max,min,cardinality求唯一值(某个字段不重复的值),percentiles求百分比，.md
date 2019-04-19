# elasticsearch 求某个字段的值(整型值不是字符串)的avg,max,min,cardinality求唯一值(某个字段不重复的值),percentiles求百分比， - sxf_123456的博客 - CSDN博客
2017年10月10日 18:02:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：815
                
elasticsearch 求某个字段的值(整型值不是字符串)的avg,max,min,cardinality求唯一值(某个字段不重复的值),percentiles求百分比，
stats 可以统计数量，min,max,avg,sum的值
metric的聚合按照值的返回类型可以分为两种：单值聚合 和 多值聚合。
单值聚合
Sum 求和
这个聚合返回的是单个值，dsl可以参考如下：
"aggs" : {
        "intraday_return" : { "sum" : { "field" : "change" } }
    }
返回的是change字段的和：
{
    ...
    "aggregations": {
        "intraday_return": {
           "value": 2.18
        }
    }
}
其中intraday_return是聚合的名字，同时也会作为请求返回的id值。另外，聚合中是支持脚本的，这里就不过多赘述了，详细参考官方文档即可。
Min 求最小值
{
    "aggs" : {
        "min_price" : { "min" : { "field" : "price" } }
    }
}
Max 求最大值
{
    "aggs" : {
        "max_price" : { "max" : { "field" : "price" } }
    }
}
avg 求平均值
{
    "aggs" : {
        "avg_grade" : { "avg" : { "field" : "grade" } }
    }
}
cardinality 求唯一值，即不重复的字段有多少
{
    "aggs" : {
        "author_count" : {
            "cardinality" : {
                "field" : "author"
            }
        }
    }
}
多值聚合
percentiles 求百分比
{
    "aggs" : {
        "load_time_outlier" : {
            "percentile_ranks" : {
                "field" : "load_time", 
                "values" : [15, 30]
            }
        }
    }
}
返回的结果包含多个值：
{
    ...
   "aggregations": {
      "load_time_outlier": {
         "values" : {
            "15": 92,
            "30": 100
         }
      }
   }
}
stats 统计
{
    "aggs" : {
        "grades_stats" : { "stats" : { "field" : "grade" } }
    }
}
请求后会直接显示多种聚合结果：
{
    ...
    "aggregations": {
        "grades_stats": {
            "count": 6,
            "min": 60,
            "max": 98,
            "avg": 78.5,
            "sum": 471
        }
    }
}
extend stats 扩展统计
{
    "aggs" : {
        "grades_stats" : { "extended_stats" : { "field" : "grade" } }
    }
}
在统计的基础上还增加了多种复杂的统计信息：
{
    ...
    "aggregations": {
        "grade_stats": {
           "count": 9,
           "min": 72,
           "max": 99,
           "avg": 86,
           "sum": 774,
           "sum_of_squares": 67028,
           "variance": 51.55555555555556,
           "std_deviation": 7.180219742846005,
           "std_deviation_bounds": {
            "upper": 100.36043948569201,
            "lower": 71.63956051430799
           }
        }
    }
}
在性能上，ES也做了很多的优化：比如max和min，如果对于排序的字段，那么就直接跳过了计算的步骤，直接取出目标值即可。
当然有些聚合也是需要特定的场合的，比如cardinality计算唯一值是通过哈希的方式，如果字段数据规模很大，那么会消耗很多的性能。
另外桶之间是可以嵌套的，比如在range聚合下嵌套了一个max聚合，那么会在range得到的每个结果组上，再次进行max的统计。
在聚合中支持脚本的使用，可以增加统计的灵活度。
            
