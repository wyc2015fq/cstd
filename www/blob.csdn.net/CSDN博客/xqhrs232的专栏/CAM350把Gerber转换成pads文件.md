# CAM350把Gerber转换成pads文件 - xqhrs232的专栏 - CSDN博客
2014年03月05日 11:45:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5268
原文地址::[http://javimat.blog.163.com/blog/static/1719143262010914113147968/](http://javimat.blog.163.com/blog/static/1719143262010914113147968/)
相关文章
1、PADS中CAM输出的相关设置----[http://www.elecfans.com/article/80/112/2011/PADS.html](http://www.elecfans.com/article/80/112/2011/PADS.html)
CAM350把Gerber转换成pads文件
CAM350是我们SMT过程中能用到的一个好软件。本文对这个软件的一些基本功能进行介绍，如文件的输入与输出、可制造性分析等，使大家能在实际工作中应用，并带来方便。
**1 前言**
DownStream Technologies发布的CAM350软件是PCB设计和制造、SMT生产过程中一个很有用的工具，它可以自动识别GERBER文件和CAM文档。而且还可以导入CAD文件，如MENTOR、POWERPCB等，另外还可以读入ODB++、GenCAD等文档，和别的CAD软件有了统一的接口。利用好CAM350软件，可以让我们的工作事半功倍，极大提高生产效率。CAM350软件唯一不足是不能直接识别Protel格式的PCB文件。图1是目前最新版本CAM350 V9.5的界面。
下面主要结合我们的SMT生产过程，来介绍下CAM350的一些常用功能。
![CAM350把Gerber转换成pads文件 - javimat - javimat的博客](http://www.smte.net/Files/UploadFile/2007%E5%B9%B410%E6%9C%88/CAMCAD%E5%9C%A8SMT%E8%BF%87%E7%A8%8B%E4%B8%AD%E7%9A%84%E5%BA%94%E7%94%A81.jpg)
![CAM350把Gerber转换成pads文件 - javimat - javimat的博客](http://www.smte.net/Files/UploadFile/2007%E5%B9%B410%E6%9C%88/CAMCAD%E5%9C%A8SMT%E8%BF%87%E7%A8%8B%E4%B8%AD%E7%9A%84%E5%BA%94%E7%94%A82.jpg)
**2 缩小DXF文件的大小**在电子组装生产过程中，各种电子组装工艺卡是指导员工生产的重要资料，例如手工插件的工艺卡等，需要在元件位置图上标注每个工位的插装元件的位置、数量等必要信息。一般的做法是从电路板设计文件输出AutoCAD格式文件（例如DXF文件），然后用AutoCAD软件打开，合适缩放后复制到专门已设计好的表格文件中，加入必要的文字信息，再将指定的元件进行醒目处理（例如涂红等），这样做符合规范化的设计管理，图2是一个规范化设计的工艺卡。常见的电路板设计软件一般都具有输出AutoCAD格式文件的功能。但我们遇到了个问题：用Protel99导出的DXF文件通常比较大，有时候会达到100多M，用AutoCAD打开很慢或者根本就打不开。这时我们就可以借助CAM350软件来解决这个问题，首先从Protel99软件中输出Gerber文件（只需丝印层和外框就足够了），然后CAM350导入Gerber文件并输出DXF文件，一般文件大小仅有几M甚至几百K，用AUTOCAD软件可以很轻松打开。
**3 PCB文件的转换**CAM350是一个灵活的、开发的系统，它提供了范围广泛的输入和输出能力，包括ODB++、Gerber、HPGL、IPC-D-356A、Gencad等格式。CAM350 还具有输入输出高级CAD数据格式的选项，包括PowerPCB和PCAD等等。利用这些功能，可以实现不同格式PCB文件间的相互转换，我们以Mentor Expedition文件转换为Pads 文件为例进行介绍：
1）从Mentor Expedition文件中输出Gencad文件（也可以输出ODB++文件）。选择菜单“File→ Export→General Interface…”，在弹出对话框中选择“Mitron Gencad”，按确定键后在当前PCB文件中PCB\OUTPUT目录下生成Gencad文件。
2）CAM350导入Gencad文件。选择菜单“File→Import→CAD Data…”，弹出CAD格式输入对话框，选择“Gencad”（如图3），出现文件打开对话框选择前面输出的Gencad文件。
3）输出PowerPCB文件。选择菜单“File→Export→CAD Data…”，弹出CAD格式输出对话框（如图4），选择输出的PowerPCB文件版本，这里选择的是默认的PowerPCB3.0。按确定键后出现文件另存对话框，设定保存的路径和文件名，按保存按钮后PowerPCB文件生成，至此转换过程结束，生成的PowerPCB文件可以用PADS软件打开。
![CAM350把Gerber转换成pads文件 - javimat - javimat的博客](http://www.smte.net/Files/UploadFile/2007%E5%B9%B410%E6%9C%88/CAMCAD%E5%9C%A8SMT%E8%BF%87%E7%A8%8B%E4%B8%AD%E7%9A%84%E5%BA%94%E7%94%A83.jpg)
**4 Gerber文件的输出**
由于制作模板以及电路板等的需要，需要提供Gerber文件给相关生产制造部门。Gerber文件是所有电路设计软件都可以产生的文件，在电子组装行业又称为模版文件（stencil data）,在PCB制造业又称为光绘文件。可以说Gerber文件是电子组装业中最通用最广泛的文件格式。但对于很多工艺人员来说，不一定熟悉各种PCB设计软件的Gerber文件输出方法，并且有些PCB设计软件的导出过程相当繁琐，不容易被人掌握。在这里将介绍一种通用的简易方法，可适合所有PCB设计软件，我们以Allegro文件为例介绍：
1）从Allegro文件中输出ODB++文件。选择菜单“File→ Export→ODB++Inside…”，弹出ODB++文件输出对话框（如图5），指定输入输出文件路径和输出ODB++文件名，并勾选“GZIP”和“Partial ODB++”，按“开始转换”图标在指定路径下生成ODB++文件。
![CAM350把Gerber转换成pads文件 - javimat - javimat的博客](http://www.smte.net/Files/UploadFile/2007%E5%B9%B410%E6%9C%88/CAMCAD%E5%9C%A8SMT%E8%BF%87%E7%A8%8B%E4%B8%AD%E7%9A%84%E5%BA%94%E7%94%A84.jpg)
2）CAM350输入ODB++文件。选择菜单“File→Import→ODB++→ Compressed File…”，选择前面输出的ODB++文件。
3）输出Gerber文件。选择菜单"File→Export→Gerber Data…"，弹出Gerber文件输出对话框（如图6），Data Format（数据格式）选择RS274-X,按"Export Path"指定输出Gerber文件路径，按“OK”按钮输出Gerber文件。
对于其它的PCB设计软件，都可以采用上述输出方法，一般从PCB文件中输出的格式文件推荐采用ODB++，这是目前大多数PCB设计软件都支持的数据格式。而对于PADS软件来说，操作更为简单，因为PADS软件可以直接输出CAM350文档。
![CAM350把Gerber转换成pads文件 - javimat - javimat的博客](http://www.smte.net/Files/UploadFile/2007%E5%B9%B410%E6%9C%88/CAMCAD%E5%9C%A8SMT%E8%BF%87%E7%A8%8B%E4%B8%AD%E7%9A%84%E5%BA%94%E7%94%A85.jpg)
**5 可制造性设计（Design for Manufacturing）**
使用DFM Audit，能够确保设计中不会包含任何制造规则方面的冲突（Manufacturing Rule Violations）。DFM Audit可执行80 多种裸板分析检查，包括制造、丝印、电源和地、信号层、钻孔、阻焊等等。运行这些检查仅仅需要几分钟的时间，并具有很高的精度。
在提交PCB去加工制造之前，就能够定位、标识并立刻修改所有的冲突，而不是在PCB板制造加工之后。DFM Audit将自动地检查酸角(acid traps)、阻焊条(soldermask slivers) 、 铜条(copper slivers)、焊锡搭桥(soldermask coverage)等等。它将能够确保阻焊数据的产生是根据一定安全间距，取保没有潜在的焊锡搭桥的条件、解决酸角(acid traps)的问题，避免在任何制造车间的CAM部门产生加工瓶颈。分析的结果将图形化的方式显示在图中，很容易观察，冲突能够立刻观察到，并且潜在的可能的问题立刻得以修复，直接使用正确的数据更新你的数据库，确保所有内容的完整性和一致性。
下面以分析丝印和阻焊层间距为例进行介绍，读入CAD文件，选择菜单“Analysis→Silk to Solder Spac-ing…”，弹出图7对话框，设置丝印层、阻焊层以及要分析的间距，这里设置的是0.5mil，按OK按钮后开始分析，如果有丝印和阻焊之间间距小于0.5mil，将会有错误信息提示，并将具体的地方用白框标示意出来（图8），可以看到丝印与焊盘重合了，以便采取相应的措施。
![CAM350把Gerber转换成pads文件 - javimat - javimat的博客](http://www.smte.net/Files/UploadFile/2007%E5%B9%B410%E6%9C%88/CAMCAD%E5%9C%A8SMT%E8%BF%87%E7%A8%8B%E4%B8%AD%E7%9A%84%E5%BA%94%E7%94%A86.jpg)
**6 结束语**
本文所介绍的只是我们在SMT生产过程中经常要用到的一些功能，软件所能带来的功能当然远远不止这些，还有待于我们在实际工作中提高和完善。这里我讲这些的目的是让大家能对这个软件有一个初步的了解，通过使用这个软件，给大家的工作带来方便。
//=============================================================================================================================
备注：
对于其它的PCB设计软件，都可以采用上述输出方法，一般从PCB文件中输出的格式文件推荐采用ODB++，这是目前大多数PCB设计软件都支持的数据格式。而对于PADS软件来说，操作更为简单，因为PADS软件可以直接输出CAM350文档。

