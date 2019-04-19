# 如何使用Kibana的仪表板与可视化功能 - 零度的博客专栏 - CSDN博客
2018年07月10日 17:53:02[零度anngle](https://me.csdn.net/zmx729618)阅读数：335
### 系列教程
本教程为[利用ELK堆栈(Elasticsearch, Logstash与Kibana)在Ubuntu 14.04上实现集中式日志记录](https://www.digitalocean.com/community/tutorials/how-to-use-kibana-dashboards-and-visualizations#tutorial_series_12)系列五篇中的第四篇。
本教程亦为[在CentOS 7上利用Logstash与Kibana实现集中式日志记录](https://www.digitalocean.com/community/tutorials/how-to-use-kibana-dashboards-and-visualizations#tutorial_series_15)系列五篇中的第四篇。
#### 内容介绍
Kibana 4是一套分析与可视化平台，构建于Elasticsearch之上，旨在帮助用户更好地实现数据理解。在本教程中，我们将从Kibana入手，探讨如何使用其界面对由ELasticsearch ELK堆栈收集到的日志消息进行过滤与可视化处理。我们将谈到多种主要界面组件，说明如何创建搜索、可视化与仪表板。
### 先决条件
这里我们假定大家已经拥有一套ELK设置，其能够收集各syslog与Nginx访问日志。
如果大家希望一步步按照本教程的指引进行，还需要首先完成以下设置：
- 负责收集syslog的ELK堆栈：[如何在Ubuntu 14.04上安装Elasticsearch、Logstash以及Kibana 4](https://www.digitalocean.com/community/tutorials/how-to-install-elasticsearch-logstash-and-kibana-4-on-ubuntu-14-04)
- Nginx负责访问日志与过滤机制：[添加过滤器以改进集中化日志记录机制](https://www.digitalocean.com/community/tutorials/adding-logstash-filters-to-improve-centralized-logging)
准备就绪后，让我们从Kibana界面开始入手。
### Kibana界面概述
Kibana的界面被划分为四大主要部分：
- Discover（发现）
- Visualize（可视化）
- Dashboard（仪表板）
- Settings（设置）
### Kibana Discover
初次接触Kibana 4时，大家最先看到的应该就是Discover页面。在默认情况下，此页面会显示ELK堆栈最近接收到的全部日志。在这里，我们可以利用Search Queries过滤并找到特定日志消息，同时利用Time Filter指定时段以降低搜索结果数量。
下面来看Kibana Discover界面中的各个元素：
- Search Bar: 位于主导航菜单下，用于搜索特定字段及/或完整消息
- Time Filter: 右上方的时钟图标，用于根据多种相对及绝对时间段进行日志过滤
- Field Selector: 左侧搜索栏下方，用于选定Log View内显示的字段以进行修改
- Date Histogram: 搜索栏下方的条形图，默认情况下显示所有日志计数、按时间（x轴）且匹配搜索与时间过滤条件。大家可以点击该条形或者拖拽以缩小时间过滤范围。
- Log View: 界面右下方，用于查看各条日志消息，同时显示根据fields筛选后的日志数据。如果未选定fields，则显示完整日志消息。
以下动画演示了本页面中的几项主要功能：
下面分布讲解其中的操作：
- 选定“type”字段，其限定每条日志记录的显示内容——默认情况下，显示完整消息。
- 搜索type: “nginx-access”, 即只匹配Nginx访问日志。
- 展开最近Nginx访问日志，查看更多细节信息。
请注意，所有结束被限定为“最近15分钟”。如果大家未能获取任何结果，请确保确实存在与搜索查询相匹配且在指定时间段内生成的日志。
日志消息的收集与过滤方式取决于Logstash与Logstash Forwarder的配置内容。在本示例中，我们收集syslog与Nginx访问日志，并以“type”进行过滤。
#### 搜索语法
搜索功能提供了简单而强大的方式，允许我们对日志消息内的特定子集加以选定。搜索语法非常直白，支持布尔运算符、通配符与字段筛选。例如，如果大家希望搜索由谷歌Chrome用户生成的Nginx访问日志，可使用type: “nginx-access” AND agent: “chrome”。另外，搜索条件还支持特定主机、客户端IP地址范围或者其它包含于日志内的数据。
在创建了需要保留的搜索查询之后，大家可以点击Save Search图标而后点击Save按钮，如动画所示：
已经保存的搜索可随时通过点击Load Saved Search图标打开。
这里我们将type: “nginx-access”搜索保存为”type nginx access”，并将利用它创建一套可视化结果。
### Kibana Visualize
Kibana Visualize页面用于创建、修改及查看我们的定制化可视结果。其中包含多种可视类型，包括垂直柱状图、饼状图、活动地图（用于在地图上显示数据）以及数据表。可视结果还能够与访问Kibana实例的其他用户共享。
如果大家对Kibana可视化方案还不熟悉，可以参阅[Kibana设置](https://www.digitalocean.com/community/tutorials/how-to-use-kibana-dashboards-and-visualizations#kibana-settings) 以了解相关指令。
#### 创建垂直柱状图
我们首先点击Visualize菜单条目。
在决定了需要的可视化类型后，选定它。我们在这里创建垂直柱状图，即Vertical bar chart。
现在选定搜索源。大家可以创建新搜索或者直接使用已经保存的搜索。这里我们选择后一种方式，选定之前创建的type nginx access搜索。
在右侧的预览图形处，我们可以看到一大块条形图案（假定搜索找到了对应的日志消息），这是因为其中只包含“Count（计数）”作为Y轴。
为了让可视结果更有意义，我们做出进一步调整。
首先添加X轴，而后点击Aggregation下拉菜单并选择“Date Histogram”。如果大家点击Apply按钮，则刚才的条形将被拆分为多个沿X轴排布的柱体。现在计数已经按时间进行分隔（可以通过下拉菜单设定需要的时间间隔），并在Discover页面中供用户查看。
如果我们希望让图形更为有趣，则可以点击Add Sub Aggregation按钮。选定Split Bars类型，点击Sub Aggregation下拉菜单并选定”Significant Terms”，而后是Field下拉菜单中的“clientip.raw”，最后点击Size字段并输入“10”。按下Apply按钮以创建新图形。
以下为生成结果的截屏：
如果图形按照多个IP地址进行可视化处理，则可看到每个柱状图形皆被拆分为彩色部分。每个彩色段表示某特定IP地址生成的日志数量，而图形可最多显示10种不同分段（具体取决于Size设定）。大家可以将鼠标悬停于其上，并点击图形中的条目以深入查看特定日志消息。
构建完成后，点击Save VIsualization图标保存可视结果，而后为其命名再点击Save按钮。
#### 创建另一可视结果
为了进入下一议题，即仪表板，我们至少需要两套可视结果。
大家可以借此机会尝试其它可视类型，例如将计数最高的5条日志“types”汇总为饼状图。点击Visualize并选定Pie chart。而后使用新搜索，将搜索保留为“_”（即全部日志）。而后选定*_Split Slices**。点击Aggregation下拉菜单再选定“Significant Terms”，点击Field下拉菜单选定“type.raw”，而后点击Size字段并输入“5”。现在点击Apply按钮并将此可视结果保存为“Top 5”。
以下为该设置的显示结果：
在本示例中，由于我们只收集syslog与Nginx访问日志，因此该饼状图只分为两块。
### Kibana Dashboard
Kibana Dashboard页面用于创建、修改及查看我们的定制化仪表板。在仪表板中，我们可以将多套可视结果整合至单一页面内，而后提供搜索查询或者点击可视结果内的某元素指定过滤条件，从而实现结果过滤。仪表板能够帮助我们更全面地了解总体日志内容，并将各可视结果同日志关联起来。
#### 创建仪表板
要创建一套Kibana仪表板，我们首先点击Dashboard菜单选项。
如果此前尚未创建仪表板，大家会看到空白页面，其中提示“Ready to get started？”如果未看到此屏幕，则证明已经存在已有仪表板。这时点击New Dashboard图标（搜索栏右侧）。
以下动画显示了如何向仪表板内添加可视结果：
下面分步进行解释：
- 点击*Add Visualization图标*
- 添加”Log Counts”饼状图与”Nginx: Top 10 client IP”直方图
- 收回 *Add Visualization菜单*
- 重新排布并调整仪表板上的可视结果
- 点击*Save Dashboard*图标
保存时为这套仪表板设定一个名称。
#### 使用仪表板
我们可以输入一条搜索查询、变更时间过滤条件或者点击可视结果内的元素对仪表板进行进一步过滤。
例如，如果我们点击此直方图内的特定彩色区段，Kibana亦允许大家针对该区段所代表的特定含义进行过滤。以下截屏为使用过滤条件后的仪表板：
请确保点击Apply Now按钮进行结果过滤并重绘仪表板可视结果。过滤条件可根据需要进行应用与移除。
### Kibana Settings
Kibana Settings页面允许大家对各类条目进行变更，包括默认值与索引模式。在本教程中，我们只谈谈Indices与Objects两部分。
#### 重载字段数据
在向Logstash数据内添加新字段时，例如为新日志类型添加过滤条件，大家可能需要重载字段列表。另外，如果大家无法找到过滤后的字段，也可尝试重载。
点击Settings菜单选项并点击”logstash-*”：
而后点击黄色的Reload Field List按钮，最后按OK按钮确认。
#### 编辑已保存的Objects
Objects允许大家对已经保存的仪表板、搜索以及可视结果进行编辑、查看与删除。
要实现操作，点击Settings菜单，而后选定Objects子菜单。
在这里，大家可以选定标签以找到需要编辑、查看或删除的对象：
在截屏中可以看到，我们已经选定了一项重复的可视结果。按下对应按钮即可完成编辑、查看或者删除操作。
### 总结
在本教程中，大家已经掌握了如何使用Kibana 4。另外，大家也了解了如何搜索日志消息以及创建可视化结果与仪表板。
本文来源自[DigitalOcean Community](https://www.digitalocean.com/community)。英文原文：[How To Use Kibana Dashboards and Visualizations](https://www.digitalocean.com/community/tutorials/how-to-use-kibana-dashboards-and-visualizations) By Mitchell Anicas
