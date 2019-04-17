# ArcGIS Beta操作仪表板 - 10月更新 - 长歌行 - CSDN博客





2017年10月22日 21:01:57[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：243标签：[arcgis																[应用](https://so.csdn.net/so/search/s.do?q=应用&t=blog)](https://so.csdn.net/so/search/s.do?q=arcgis&t=blog)
个人分类：[GIS/RS/GPS																[Esri](https://blog.csdn.net/iuhsihsow/article/category/7197878)](https://blog.csdn.net/iuhsihsow/article/category/1078364)

所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)







[原文地址](https://blogs.esri.com/esri/arcgis/2017/10/16/88428/)

by Law, Jeff Shaner, and Emily Gibson on October 16, 2017

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/DashboardIcon1-150x150.png)

对于已经[使用和探索ArcGIS测试版操作仪表板](https://blogs.esri.com/esri/arcgis/2017/08/07/author-beautiful-dashboards-in-your-browser-with-operations-dashboard-for-arcgis/)的用户，感谢您对[Esri Early Adopter](https://earlyadopter.esri.com/welcome/)社区论坛的宝贵意见和反馈！我们还有很多活动会非常感谢您参与。我们还看到一些用测试版制作的漂亮的仪表板应用程序。

自从上次测试版更新以来已经有几个月了，我们一直在听你的意见和反馈。我们已经努力继续改进和增强操作仪表盘，以满足您的需求，同事我们会更新一个测试版给您测试。

### 改进仪表板管理

当您开始使用ArcGIS组织中的仪表板时，通常从仪表板主页开始。每个仪表板都表示为项目卡，并且添加了新功能，以便您可以将光标悬停在卡上，以便您可以查看，编辑，访问项目详细信息以及删除（如果您具有相应权限）仪表板项目。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/Fig1-1024x631.png)

### 增强仪表板的创作方式

创建仪表板时，导航栏已经精简和重新设计，以获得更好的可用性。当您决定退出编辑模式时，我们会提示您确认任何未保存的编辑内容，以免意外丢失任何更改。只有在创建仪表板时才能看到导航栏，在查看仪表板时不会显示该导航栏。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/Fig2-1024x342.png)

这将导致下一个大的改进：无头标签仪表板。我们认识到要求屏幕部分显示标题并不总是适用于所有用例。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/Fig3-1024x639.png)

虽然在某些情况下需要一个无标题的仪表板，但其他用例将从有一个使用中获益。标题可用于清楚说明仪表板的目的，添加企业标识和品牌，提供其他网页的导航链接等。为了满足这一需求，我们为仪表板引入了一个新的元素：标题面板 - 这个是仪表板顶部的固定区域，可以配置。只需几个配置步骤，您可以快速将标题添加到任何仪表板。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/Fig4-1024x638.png)

量表（Gauges ）显示单个指标的值。它们非常适合于实现目标的进展，代表一个百分位数的度量，例如关键绩效指标（KPI），或仅仅显示参数的“健康状况”。操作仪表板包括两种不同类型的仪表：i）进度仪表和ii）仪表。

进度计表（Progress gauges）明有些事情正在向前或向后移动。在进度计上，使用彩色带显示当前值，该数字可以以百分比或原始值显示。也可以定义间隔，这些间隔可用于更改填充带颜色。这使得测量仪可以随着测量值的进行而改变其外观（例如，绿色=好，黄色=好，红色=不好）。进度量表可以呈现为三种形状之一：圆形，马蹄形和半甜甜圈。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/Fig5-1024x432.png)

仪表计（Meter gauges）显示参数的数量，级别或内容。它们还允许将间隔范围定义为定性范围（例如，良好，平均和差）。有诸如：速度计，温度计，电压表，电流表和压力表的仪表的许多实际的现实世界例子。仪表可以显示为两种形状之一：马掌和半甜甜圈。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/Fig61.png)

### 数字和日期

我们改进的操作仪表板的另一个方面是为您提供如何在仪表板中显示数据的更精确的控制。

单位前缀是使数字（大或小）更容易解释的技术。这可以用于消除和简化仪表板上显示的信息。在最后一次beta版本刷新时，它与“系列图”巧妙地引入，但是我们已经在此测试版更新中扩展了其曝光率。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/Fig7.1-1024x263.png)

此外，还对数字格式进行了大量的工作。当操作仪表板中显示数字数据时，应用程序将尝试格式化和解析数据，并使其符合ArcGIS组织帐户本地设置（即在帐户用户配置文件中设置的区域设置）。例如，加拿大或美国的用户预计将会有100万的小数位数被表示为“1,000,000.00”，德国的用户将会在“法国”预计“1 000 000,00”。对于内置默认值不符合特定需求的情况，仪表板作者可以通过定义用于在可视化中显示数字的格式的模式来覆盖默认行为，例如：Gauge，Indicator和Serial Chart。结合单位前缀，仪表板中数字的显示从未如此强大。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/Fig8-1024x667.png)

在仪表板上显示日期（日期和时间）通常很重要。 日期的元素是：年，月，日，小时，分和秒。 配置串行图表已得到改进，使仪表板作者能够更好地控制沿类别轴格式化日期。 类似于为显示数字所做的工作，仪表板作者可以使用串行图表上的日期格式显示尽可能多的（或少量）有关日期的信息。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/Fig9.png)

在实现日期格式化时，我们认识到并不是所有的日期都存储在类型date的字段中。 在某些数据集中，日期存储在字符串字段中。 某些数据集也可能具有存储在字符串字段中的数字数据。 为了满足这一要求，我们在配置串行图表时添加了日期和号码解析功能。 当字符串字段用于定义串行图类别时，仪表板作者可以指示字符串实际表示日期或数字，并且图表将自动将字符串解析为日期或数字。 字符串解析的模式语法示例如下所示：

yyyy 

yyyyMM 

yyyy/MM 

yyyy-MM 

yyyyMMdd 

yyyy/MM/dd 

yyyy-MM-dd 

yyyyMMddHHmmss 

yyyy/MM/dd HH:mm:ss 

yyyy-MM-dd HH:mm:ss
期待下个月初的另一个测试版更新，仪表板作者将能够配置高度交互式的仪表板，而无需任何编程。 现在，我们希望您喜欢这些新的增强功能，我们很高兴听到您对Esri Early Adopter社区论坛的反馈。

此致

The Operations Dashboard for ArcGIS Dev team






