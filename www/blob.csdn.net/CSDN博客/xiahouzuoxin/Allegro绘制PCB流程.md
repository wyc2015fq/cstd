# Allegro绘制PCB流程 - xiahouzuoxin - CSDN博客





2013年08月29日 17:35:24[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：11625











单位换算

1mil = 0.0254 mm

1mm = 39.3701 mil

默认情况下我们更倾向于使用mil单位绘制PCB板。



**1 新建工程，*File --> New...***

*--> [Project Directory]*显示工程路径

*--> [Drawing Name]*工程名称，Browse...可选择工程路径

*--> [Drawing Type]*工程类型，绘制PCB板选择Board，封装选择Packagesymbol



**2 设置画布参数，*Setup --> Design Parameters...***

*--> [Design] *

      单位为Mils，Size为other，2位精度，

      Width与Height分别代表画布的宽高

      LeftX与LowerY代表原点位置坐标

  点击Apply使修改生效

*--> [Display]*

      勾选Gridon, 打开SetupGrids...

      将Non-Etch和AllEtch中的所有Spacing设为1mil=0.0254mm



**3 设置库路径，*Setup --> User Preference...***

将所有绘制好的元件封装复制到同一目录下，方便设置库目录，

*--> [Paths]*

*--> [Library]* 指定modulepathpadpath parampath psmpath到封装所在目录



**4 绘制板框，*Add --> Line***

  Class:SubClass = Board Geometry:Outline



**5 倒角，*Manufacture -->Dimimension/Draft --> fillet***

  倒角半径（Radius）参考：100mmx100mm板倒角100mil~200mil

  分别点击倒角的两条边完成倒角



**6 设置允许布线区，*Setup --> Areas --> RouteKeepin***

  Class:SubClass = Route Keepin:All

  一般情况，RouteKeepin距离板框0.2mm(8mil)~0.5mm(20mil)



  方法2：使用Z-Copy命令，Edit-Z-Copy

      选择Class:SubClass=RouteKeepin:All，

      Size选择Contract向内缩进，Offset填充20mil，

      点击板框完成复制，此方法亦使用步骤7



**7 设置允许元件摆放区，*Setup --> Areas --> PackageKeepin***

  Class:SubClass = Package Keepin:All

  一般情况，PacakgeKeepin与RouteKeepin大小一致



  方法2：使用Z-Copy命令



**8 放置机械安装孔，*Place --> Manual***

*--> [Advanced Settings]*勾选Library

*--> [Placement List]*

*--> [Mechanical symbols]* 选上需要使用的机械安装孔，敲坐标放置



  注：使用“选择多个元件，右键Align components”对齐元件。



**9 设置层叠结构，*Setup --> Cross-section***

  双层板按默认设置，从上到下依次为：表层空气，铜走线Top层，玻璃纤维介质层，铜走线Bottom层，底层空气

  多层板需要做相关层添加[FIXME]



**10 导入网表，*File --> Import -->Logic...***

*--> [Cadence] *

选择Designentry CIS(Capture)，Always，Importdirectory选择网表文件路径 


  导入完成后*File--> Viewlog...*查看导入错误信息，确保0 errors，0warnings



**11 放置元器件，*Place --> QuickPlace...***

  选择Placeall components,点击place完成自动放置

  检查Unpalcedsymbol count显示状态，确认未放置的元件为0



  注：有关元器件突出板框外的KC DRC问题 <--- 删除该DRC

*Display --> Waive DRCs --> Waive*命令，点击DRC删除即可。



**12 约束设置，*Setup --> Constraints -->Constraints Manager...***

*  --> [Physical]*

*      --> [Physical Constraint Set]*

*          --> [All Layers]*

              线宽设置为>=6mil，添加过孔（小于6的非0值都设为6或更大）

*      --> [Net]*

*          --> [All Layers]*

              电源与地网络设置至少30mil，大功率大电流网络也设置大些

*  --> [Spacing]*

      ... 设置线间距、VIA间距等，都至少设为6mil，6mil是根据PCB厂家定的



**13 布局布线**

  接插件（如DB9、JTAG接口、电源接口等）放在PCB板周边；

  。。。



  布线时双击添加过孔，Options中Act可改变当前PCB面，Linewidth设置线宽；

*[Route] --> [PCB Router] --> [Route Automatic…]*可自动布线；

  。。。



**14 添加丝印**

  (1)自动添加丝印

*      Manufacture --> Silkscreen*

*        --> [Layer] Both*

*        --> [Elements] Both*

*        --> [Classes and subclasses]*

*        --> [Package geometry] Silk*

*        --> [Refrence designator] Silk*

        ... 其它选择None

  点击Silkscreen完成丝印添加



  (2)手动添加丝印信息

*--> Add --> Text*

      Class:Subclass=Manufacture:AutoSilk_Top

      设置字号及线宽后输入文字信息



  注：丝印字号修改，*Edit--> Change*，Find中选只Text，

      Class:subclass=Manufacture:空

      设置字号线宽，全选后Done即可



**15 添加覆铜，*Shape --> Polygon***

  Class:Subclass=Etch:Top

  Option中勾选上CreateDinamic Shape，选择Assign netname为Gnd网络



  添加底层覆铜，Class:Subclass=Etch:Bottom



  删除顶层和底层死铜，*Shape--> Delete Islands*，Delete allon layer



**16 查看报告，*Tools --> Quick Reports***

  至少检查如下4项：

  Unconnected Pins Report

  Shape Dynamic State

  Shape Islands

  Design Rules Check Report



**17 数据库检查，*Tools --> Database Check***

  勾选全3项，点击Check检查，Viewlog查看错误日志



**18 钻孔文件生成**

  (1) 钻孔参数文件生成，*Manufacture--> NC --> NC Parameters*

  按默认设置，点close后生成nc_param.txt



  (2) 钻孔文件生成，*Manufacture--> NC --> NC Drill*

  如果有盲孔或埋孔，则Drilling中选择By Layer，否则默认，

  点Drill生成*.drl文件，点击Viewlog查看钻孔文件信息



  (3) 不规则孔的钻孔文件生成，*Manufacture--> NC --> NC Route*

  默认设置，点击Route生成*.rou文件



  (4) 钻孔表及钻孔图的生成，*Manufacture--> NC --> Drill  Legend*

  如果有盲孔或埋孔，则Drilling中选择By Layer，否则默认（单位为mil），

  点击OK生成*.dlt文件



**19 生成光绘(Gerber)文件**

  (1) 设置光绘文件参数，*Manufacture--> Artwork*

*--> [General Parameters]*

*          --> [Device type] Gerber RS274X*

*          --> [OUtput units] Inches*

*          --> [Format]*

*              --> [Integer places] 3*

*              --> [Decimal places] 5*

*--> [Film Control]* 设置层叠结构（10层）

*    -->[Available films]*

***           --> [Bottom]***

***                  --> ETCH/Bottom***

***                  --> PIN/Bottom***

***                  --> VIA Class/Bottom***

***              --> [Top]***

***                  --> ETCH/Top***

***                  --> PIN/Top***

***                  --> VIA Class/Top***

***              --> [Pastemask_Bottom]***

***                  --> PackageGeometry/Pastemask_Bottom***

***                  -->Stack-Up/Pin/Pastemask_Bottom***

***                  -->Stack-Up/Via/Pastemask_Bottom***

***              --> [Pastemask_Top]***

***                  --> PackageGeometry/Pastemask_Top***

***                  -->Stack-Up/Pin/Pastemask_Top***

***                  -->Stack-Up/Via/Pastemask_Top***

***              --> [Soldermask_Bottom]***

***                  --> Board Geometry/Soldermask_Bottom***

***                  --> PackageGeometry/Soldermask_Bottom***

***                  -->Stack-Up/Pin/Soldermask_Bottom***

***              --> [Soldermask_Top]***

***                  --> BoardGeometry/Soldermask_Top***

***                  --> Package Geometry/Soldermask_Top***

***                  -->Stack-Up/Pin/Soldermask_Top***

***              --> [Silkscreen_Bottom]***

***                  --> BoardGeometry/Silkscreen_Bottom***

***                  --> PackageGeometry/Silkscreen_Bottom***

***                  -->Manufacture/Autosilk_Bottom***

***              --> [Silkscreen_Top]***

***                  --> BoardGeometry/Silkscreen_Top***

***                  --> PackageGeometry/Silkscreen_Top***

***                  -->Manufacture/Autosilk_Top***

***              --> [Outline]***

***                  --> Board Geometry/Outline***

***              --> [Drill]***

***                  --> Board Geometry/Outline***

***                  -->Manufacture/Nclegend-1-2***

          选中Checkdatabase before artwork复选框！

*--> [Film options]*

*              --> [Undefined line width]*

                  选中层叠结构中的每一层，都设置为6mil

*--> [Shape bounding box]*

                  选中层叠结构中的每一层，都设置为100

* --> [plot mode]*

                  选中层叠结构中的每一层，无特殊情况都选择Positive

* --> [Vector based pad behavior]*选中每一层都勾选上

  点击OK完成参数设置



  (2) 生成光绘文件，*Manufacture--> Artwork*

  仔细检查层叠结构的设置，很重要，不能出错！

  Select all选择所有层，确认选中Check database before artwork，

  执行CreateArtwork生成光绘文件，点击Viewlog查看生成光绘信息，确保没有任何error！



**20 打包Gerber文件给PCB厂商**

  共14个文件：10{*.art}+ 1{*.drl} + 1{*.rou} + 2{*.txt}

  TOP.art

  Bottom.art

  Pastemask_Top.art

  Pastemask_Bottom.art

  Soldermask_Top.art

  Soldermask_Bottom.art

  Silkscreen_Top.art

  silkscreen_Bottom.art

  Outline.art

  Drill.art

  art_param.txt

  nc_param.txt

  *.rou

  *-1-2.drl



  打包成*.rar等压缩包发给厂商



