# caffe深度学习网络结构可视化
# caffe深度学习网络结构可视化


**[Chrispher](http://chrispher.github.com)

- [关于](http://chrispher.github.com/page/about.html)
- [留言板](http://chrispher.github.com/page/message.html)
- [时间轴](http://chrispher.github.com/page/timing.html)
- [笔记专题](#)- [PRML](http://chrispher.github.com/category/#PRML)
- [实践练习](http://chrispher.github.com/category/#实践练习)
- [Ng-机器学习](https://raw.githubusercontent.com/chrispher/chrispher.github.com/master/_drafts/machine_learning_notes_ng.docx)

- [课程图谱](#)- [pySpark 公开课](/page/graph/big_data_with_spark_python.html)













### ****caffe深度学习网络结构可视化                
**
                    2016年01月10日
                    ****[深度学习](http://chrispher.github.com/category/#深度学习)****[环境搭建](http://chrispher.github.com/tag/#环境搭建)**[caffe](http://chrispher.github.com/tag/#caffe)**** 字数:9930
                


之前写得一个小工具，用来可视化caffe里各种prototxt定义的网络。当然，代码比较简单，可以修改一下用户可视化各种相关的网络模型，不依赖于caffe和proto解析。

### 目录
- [1.code](#1code)
- [2.例子](#2例子)

### 1.code



```
#!/usr/bin/env python
# -*- coding: utf-8 -*-
import re, sys
from graphviz import Digraph


def parse_line(line):
    name = re.findall(r'}name:"(.*?)"', line)
    types = re.findall(r'type:"(.*?)"', line)
    inputs = re.findall(r'bottom:"(.*?)"', line)
    outputs = re.findall(r'top:"(.*?)"', line)
    return name, types, inputs, outputs


def parse_file(filename):
    with open(filename) as f:
        data = f.read()
        data = data.replace("\n", "")
        data = data.replace(" ", "")
        data = data.split("layer{")
    plot_data = []
    for i in xrange(len(data) - ):
        res = {}
        line = "}" + data[i+]
        name, types, inputs, outputs = parse_line(line)
        res["name"] = name[]
        res["types"] = types[].lower()
        res["inputs"] = inputs
        res["outputs"] = outputs
        plot_data.append(res)
    # print(plot_data)
    return plot_data



def plot(plot_data, width="1.0"):
    g = Digraph("LR", filename='er.gv', engine='dot', format='pdf')
    node_attr = {"shape": "record", "fixedsize": "true",
                "style": "rounded,filled",
                 "color": 'lightblue2',"concentrate":"true"}
    g.node_attr.update(node_attr)
    cm = ("#8dd3c7", "#fb8072", "#bebada", "#80b1d3",
          "#fdb462", "#b3de69", "#fccde5")

    size = 1.0
    for l, d in enumerate(plot_data):
        name = d['name']
        outputs = d['outputs']
        e = Digraph(name)

        if "pool" in d['types']:
            e.attr('node', shape='box', color=cm[])
        elif 'loss' in d['types']:
            e.attr('node', shape='ellipse', color=cm[])
        elif "convolution" in d['types']:
            e.attr('node', shape='box', color=cm[])
        elif "concat" in d['types']:
            e.attr('node', shape='box', color=cm[])
        elif "relu" in d['types']:
            e.attr('node', shape="box", color=cm[])
        else:
            e.attr('node', shape="box", color=cm[])

        if len(outputs) < :
            name = outputs[]
        # special for the layer whoes inputs are sample with outputs
        if d['inputs'] == d['outputs']:
            e.node(name, width=width, height="0.6")
        else:
            label= '''<<TABLE BORDER="0">                      <TR><TD><FONT POINT-SIZE="12">%s</FONT></TD></TR>                      <TR><TD><FONT POINT-SIZE="8" COLOR="blue">%s</FONT></TD></TR>                    </TABLE>>''' %(name, d['types'])
            e.node(name, label, width=width, height="0.6")
        e.attr('node', shape='ellipse')
        for i in d['inputs']:
            if 'slot' in i:
                e.node(i, fontsize="8", width="0.5", height="0.4", color=cm[])
            else:
                e.node(i, fontsize="12", width=width, height="0.6")
            if i == name:
                e.edge(i, name, label=d['types'], fontsize="5", color=cm[])
            else:
                e.edge(i, name)

        # mult outputs
        if len(outputs) > :
            for i in outputs:
                e.node(i, fontsize="12", width=width, height="0.6")
                e.edge(name, i)
        g.subgraph(e)

    g.view()


if __name__ == "__main__":
    filename = sys.argv[]
    # filename = "googlenet.prototxt"
    res = parse_file(filename)
    width = "1.5"
    plot(res, width)
```



### 2.例子

这里可视化了各种网络结构，比如google的部分如下：

![](https://www.datakit.cn/images/deeplearning/GoogleNet_part.png)





