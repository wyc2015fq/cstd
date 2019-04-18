# 2018年开发者峰会中关于ArcGIS Pro SDK的相关资料 - 长歌行 - CSDN博客





2018年05月16日 13:16:09[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：243









## Table of Contents

- [1. 2018年开发者峰会中关于ArcGIS Pro SDK的相关资料](#sec-1)- [1.1. 技术讲座](#sec-1-1)- [1.1.1. ArcGIS Pro SDK for .NET: Beginning Pro Customization and Extensibility](#sec-1-1-1)
- [1.1.2. ArcGIS Pro SDK for .NET: Advanced Pro Customization](#sec-1-1-2)
- [1.1.3. ArcGIS Pro SDK for .NET: Beginning Editing and Editing UI Patterns](#sec-1-1-3)
- [1.1.4. ArcGIS Pro SDK for .NET:  Advanced Editing and Edit Operations](#sec-1-1-4)
- [1.1.5. ArcGIS Pro SDK for .NET:  Pro Application Architecture Overview and API Patterns](#sec-1-1-5)
- [1.1.6. ArcGIS Pro SDK for .NET: Mapping and Layout](#sec-1-1-6)
- [1.1.7. ArcGIS Pro SDK for .NET:  Working with Rasters and Imagery](#sec-1-1-7)
- [1.1.8. ArcGIS Pro SDK for .NET: An Overview of the Geodatabase API (video coming soon)](#sec-1-1-8)
- [1.1.9. ArcGIS Pro SDK for .NET: An Overview of the Utility Network Management API   (video coming soon)](#sec-1-1-9)

- [1.2. 演示环节](#sec-1-2)- [1.2.1. ArcGIS Pro SDK for .NET: New UI Controls for the SDK](#sec-1-2-1)
- [1.2.2. ArcGIS Pro SDK for .NET: Raster API and Manipulating Pixel Blocks](#sec-1-2-2)
- [1.2.3. ArcGIS Pro SDK for .NET: Custom States and Conditions   (video coming soon)](#sec-1-2-3)
- [1.2.4. ArcGIS Pro SDK for .NET: Getting Started](#sec-1-2-4)

- [1.3. 实践工坊](#sec-1-3)- [1.3.1. Getting Started with the ArcGIS Pro SDK for .NET](#sec-1-3-1)







# 2018年开发者峰会中关于ArcGIS Pro SDK的相关资料

原文地址：[ArcGIS Pro SDK Sessions Available from Dev Summit 2018](https://www.esri.com/arcgis-blog/products/arcgis-pro-net/announcements/arcgis-pro-sdk-sessions-available-from-dev-summit-2018/)

原作者信息： Christopher Zent        | Announcements        | May 7, 2018 

Chris是ArcGIS Desktop团队的产品经理，专注于ArcGIS Pro SDK并帮助用户实现其专业定制化的目标。
翻译：iuhsihsow 

译者注：在本博客中只翻译这个目录，具体内容以后说不定会进行翻译。对感兴趣的部分可以留言。 

对于之前没有接触过Pro或者Pro SDK的同学可以在本文中搜索”Getting Started”，对相关项目进行阅读和练习。想从宏观角度来看的话，搜索”Architecture”。
2018年Esri开发者峰会中关于ArcGIS Pro SDK的技术会议视频已在YouTube的[Esri Events频道](https://www.youtube.com/channel/UC_yE3TatdZKAXvt_TzGJ6mw)上线。当然，我们也可以从会议网址中找到：[Proceedings site](http://proceedings.esri.com/library/userconf/devsummit18/) 。

以下就是Pro SDK讲座的列表，包括视频和PPT链接。这些讲座是开始接触，更新SDK信息和开发方式的好途径。

## 技术讲座

### [ArcGIS Pro SDK for .NET: Beginning Pro Customization and Extensibility](https://youtu.be/9tQKOMoLa2w)

学会如何使用.NET SDK自定义和扩展ArcGIS Pro。在这里，我们将向你展示如何使用插件和配置自定义模式，使用DAML进行编程以及使用ArcGIS Pro的QueuedTask进行编程。

[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-053.pdf)

### [ArcGIS Pro SDK for .NET: Advanced Pro Customization](https://youtu.be/Mhox_AJEFb0)

本节讲座中，会介绍高级自定义模式，包括自定义项目和应用程序的属性，命令过滤，跨多个解决方案的UI自定义，可嵌入空间和DAML类别的即插即用UI扩展。

[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-049.pdf)

### [ArcGIS Pro SDK for .NET: Beginning Editing and Editing UI Patterns](https://youtu.be/cGPFHHUTcXc)

了解如何在Pro中创建编辑工具，介绍构建工具和编辑工具，以及如何将你的自定义编辑功能集成到Pro UI中。并介绍用于创建和更新功能以及属性编辑的Inspector类，显示自定义的galleries，集成到可停靠面板中，以及如何修改sketch。

[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-052.pdf)

### [ArcGIS Pro SDK for .NET:  Advanced Editing and Edit Operations](https://youtu.be/U4vcNDEkj1w)

本节主要介绍如何使用编辑操作和编辑事件。我们将介绍一些关键编辑操作功能和编辑操作的使用情况。包括：版本编辑， 多个数据集合并操作，链编辑操作。本节也会深入研究编辑实现以及底层事件，例如编辑功能中的撤销/重做。

[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-048.pdf)

### [ArcGIS Pro SDK for .NET:  Pro Application Architecture Overview and API Patterns](https://youtu.be/muWsVjMv1yM)

本节讲座涵盖Pro应用程序的体系结构框架以及对于UI，DAML，扩展性以及模块子系统，线程-并发相关的主要模式的介绍。本讲座适用于Pro的应用程序开发人员。他们会希望详细了解Pro内容部构件以及如何在插件和配置中利用这些知识。    

[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-057.pdf)

### [ArcGIS Pro SDK for .NET: Mapping and Layout](https://youtu.be/qlwHukelHVw)

从本节中可以了解如何创建用于选择和识别的地图工具以及定义弹出窗口。我们将研究新的Pro Layout API以实时创建布局，并与Layout Map Series集成。我们将介绍MVVM与WPF的集成，以便在Pro中编写引人注目的UI，包括多线程等因素。

暂无资料下载地址

### [ArcGIS Pro SDK for .NET:  Working with Rasters and Imagery](https://youtu.be/8MUTQ39JOhs)

从本节可以了解如何使用具有影像和栅格数据的ArcGIS Pro SDK创建令人注目的插件。开发人员可以使用ArcGIS Pro SDK来控制其影像和栅格数据的显示方式，读写像素，属性，以及控制数据处理方式。 
[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-059.pdf)

### ArcGIS Pro SDK for .NET: An Overview of the Geodatabase API (video coming soon)

本节主要概述用于处理ArcGIS Pro中的地理数据库(geodatabase)的.NET API，包括处理数据集，字段，查询，选择，联结和关联。

[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-050.pdf)

### ArcGIS Pro SDK for .NET: An Overview of the Utility Network Management API   (video coming soon)

本节将概述ArcGIS Pro中的公共设施网络.NET API，为电力，燃气，水和废水公共事业创建定制工具和应用程序。

[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-051.pdf)

## 演示环节

### [ArcGIS Pro SDK for .NET: New UI Controls for the SDK](https://youtu.be/DFidQJm5xok)

将介绍ArcGIS Pro SDK for.NET中提供的新控件，包括查询生成器控件，定位器控件和坐标选取控件。了解如何将这些控件加入到你的自定义Pro UI中。

### [ArcGIS Pro SDK for .NET: Raster API and Manipulating Pixel Blocks](https://youtu.be/2pJXcbi0gzA)

本节将使用栅格API来选择和遍历数据。Demo将创建一个栅格指针来遍历栅格数据，并使用像素块读取和写入上个数据集中的像素。   

### ArcGIS Pro SDK for .NET: Custom States and Conditions   (video coming soon)

了解如何利用状态(States)和条件(Conditions)来更好使用上下文行为。Demo将重点介绍插件中的DAML元素，以及如何根据应用程勋中选择的呢日用来操作Pro UI和内容。

[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-054.pdf)

### [ArcGIS Pro SDK for .NET: Getting Started](https://youtu.be/5KNytuFJWWQ)

从头演示，联系如何构建你的第一个Pro插件。其中包括很多小提示。本节将向你展示如何使用DevLabs和示例。并介绍很多实用在线资源。

[下载地址](http://proceedings.esri.com/library/userconf/devsummit18/papers/dev-int-055.pdf)

## 实践工坊

### [Getting Started with the ArcGIS Pro SDK for .NET](https://youtu.be/lkwpq6QuK3U)

在本实践工坊中，会向你介绍ArcGIS Pro的基本概念，使用技巧和实用步骤，以帮助你能快速开发插件以及扩展Pro。参与者将和指导人员一起浏览其中课题并使用Pro SDK和Microsoft Visual Studio在自己的设备上进行练习。





