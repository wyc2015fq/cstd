# 【Python】pandas.DataFrame大写json格式 - zkq_1986的博客 - CSDN博客





2018年10月12日 14:49:28[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：159








json的格式如下 

split，样式为 {index -> [index], columns -> [columns], data -> [values]}

records，样式为[{column -> value}, … , {column -> value}]

index ，样式为 {index -> {column -> value}}

columns，样式为 {index -> {column -> value}}

values，数组样式

table，样式为{‘schema’: {schema}, ‘data’: {data}}，和records类似


样例1：

```python
#将json格式转化为python格式
js = """{"name":"Wes",
"places_lived":["US","Spain","Germany"],
"pet":null,
"siblings":[{"name":"Scott","age":25,"pet":"Zuko"},{"name":"Katie","age":33,"pet":"Cisco"}]
}"""
import json
data = json.loads(js)
print(data)

#将pandas.Dataframe格式输出到json文件
import pandas as pd
ls=[{"name":"Scott","age":25},{"name":"Katie","age":33}]
data = pd.DataFrame(ls,columns=['name','age'])
data.to_json('a.txt',orient='records')
```



样例2：

df = pd.DataFrame([['a', 'b'], ['c', 'd']],

                   index=['row 1', 'row 2'],

                   columns=['col 1', 'col 2'])

###########

split

###########

df.to_json(orient='split')

>'{"columns":["col 1","col 2"],

  "index":["row 1","row 2"],

  "data":[["a","b"],["c","d"]]}'

###########

index

###########

df.to_json(orient='index')

>'{"row 1":{"col 1":"a","col 2":"b"},"row 2":{"col 1":"c","col 2":"d"}}'

###########

records

###########

df.to_json(orient='records')

>'[{"col 1":"a","col 2":"b"},{"col 1":"c","col 2":"d"}]'

###########

table

###########

df.to_json(orient='table')

>'{"schema": {"fields": [{"name": "index", "type": "string"},

                        {"name": "col 1", "type": "string"},

                        {"name": "col 2", "type": "string"}],

             "primaryKey": "index",

             "pandas_version": "0.20.0"},

  "data": [{"index": "row 1", "col 1": "a", "col 2": "b"},

           {"index": "row 2", "col 1": "c", "col 2": "d"}]}'


参考文献：

1.  https://blog.csdn.net/huanbia/article/details/72674832?utm_source=copy 

2. [http://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.to_json.html](http://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.to_json.html)





