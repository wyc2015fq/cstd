
# ERP与EWM集成配置---ERP 端（一） - 喜欢打酱油的老鸟 - CSDN博客


2018年08月02日 08:09:34[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：230


[http://www.cnblogs.com/byfhd/archive/2012/03/15/2398015.html](http://www.cnblogs.com/byfhd/archive/2012/03/15/2398015.html)
1. 分配逻辑系统到client
Integration with Other mySAP.com Components ® Extended Warehouse Management ® Basic Settings for Setting Up the System Landscape ® Assign Logical System to a Client.
![clip_image002](https://images.cnblogs.com/cnblogs_com/byfhd/201203/201203151427354640.jpg)
激活跨client端
2. 定义逻辑系统名称
Integration with Other mySAP.com Components® Extended Warehouse Management ® Basic Settings for Setting Up the System Landscape ® Name Logical System
Tcode:BD64
SAP SCM, for example define SCMCLNT001
3. 指定 SAP APO 版本
Integration with mySAP.com Components ® Extended Warehouse Management ® Basic Settings for Setting Up the System Landscape® Specify SAP APO Release
![clip_image004](https://images.cnblogs.com/cnblogs_com/byfhd/201203/20120315142738165.jpg)
逻辑系统：如：SCMCLNT001
系统类型：SAP_APO
4. 设置目标系统和队列类型
Integration with mySAP.com Components ® Extended Warehouse Management ® Basic Settings for Setting Up the System Landscape ® Set Target System and Queue Type
|Queue type
|I|(Inbound queue)
|
5. 在ERP中维护qRFC监控维护
TCODE：SMQR
Register without activation
|Field
|Value
|
|Queue Name
|<used queue name>|, we recommend that you use either|*|or any of the following:|CF*|,|DLV*|,|DLW*, QI*, EWM*, WM*, X*
|
|Mode
|D
|
|Max. Runtime
|60
|
|USERDEST
|- leave blank -
|
|Attempts
|30
|
|Pause
|300
|
![clip_image006](https://images.cnblogs.com/cnblogs_com/byfhd/201203/201203151427412343.jpg)
TCODE：SMQS
Register without activation.
|Field
|Value
|
|Destination
|<RFC Destination>|, for example|SCMCLNT001|or|P6QEWM800
|
|Max.Conn.
|10
|
|Max. Runtime
|60
|
|W/o tRFC
|- leave blank -
|
6. 为数据传输设计用户参数
Integration with mySAP.com Components ® Extended Warehouse Management ® Basic Settings for Data Transfer ® Set User Parameters. Alternatively,
TCODE：CFC2.
|Field
|Value
|
|User Name
|*|,|ALEREMOTE
|
|RFC Mode
|Q
|
|Logging
|D
|
|Debug
|- leave blank -
|
![clip_image008](https://images.cnblogs.com/cnblogs_com/byfhd/201203/201203151427442603.jpg)
7. ERP激活附加的商品主数据维护
![clip_image010](https://images.cnblogs.com/cnblogs_com/byfhd/201203/201203151427479241.jpg)
Tools ® Customizing ® Business Configuration Sets ® Activation of BC Sets. Alternatively,
TCODE： SCPR20
BC： /SPE/MATERIAL_SCREENS
BC Set ® Activate
Overwrite All Data
Default Mode (Recommend)
![clip_image012](https://images.cnblogs.com/cnblogs_com/byfhd/201203/201203151427505846.jpg)
For Service Parts Management, additional fields are provided on the client level (table MARA) and in the units of measure for the material (table MARM). The relevant maintenance screens are provided and grouped in screen sequence "SL". This BC set includes the screens in the screen sequence 21, which means a complete screen sequence is available.
**Note**: After executing this BC set, you may need to reorder the screens in sequence 21 using transaction OMT3R.
![clip_image014](https://images.cnblogs.com/cnblogs_com/byfhd/201203/201203151427532201.jpg)
8. 维护Number Ranges
TCODE：SNRO
RV_BELEG Unchecked Deliveries
Logistics Execution ® Service Parts Management (SPM) ® Outbound Process (SPM) ® Unchecked Deliveries ® Maintain Deviating Number Range for Unchecked Deliveries.
Logistics Execution ® Service Parts Management (SPM) ® Extended Inbound Delivery Processing (SPM) ® Settings for SPM Inbound Delivery Processing.
9. 激活服务部分管理SPM
service parts management (SPM) in the ERP system
Logistics Execution ® Service Parts Management ® Activate Enhancements for Service Parts Management (SPM) ® Activate Enhancements for Service Parts Management.
![clip_image016](https://images.cnblogs.com/cnblogs_com/byfhd/201203/201203151428021833.jpg)
1. 新增
2. Service Parts Management active (SPM).
3. 确认消息
![clip_image018](https://images.cnblogs.com/cnblogs_com/byfhd/201203/201203151428042964.jpg)
10. 扩展的内向交货
extended inbound delivery
为扩展内向交货处理用户定义参数
![clip_image020](https://images.cnblogs.com/cnblogs_com/byfhd/201203/201203151428084553.jpg)
Logistics Execution ® Service Parts Management (SPM) ® Extended Inbound Delivery Processing (SPM) ® UI Profiles ® Maintain Profile ® Assign Users. Alternatively,
TCODE：VL60P2.
1. 新增
2. 创建用户组
a. Double-click Assignment User - User Groups
b. Choose New Entries
c. Enter all users
d. For each user, select the Default Group indicator
e. In the Dialog Structure, double-click Assignment Profiles - User Groups
f. Choose New Entries
g. Assign profile VL60ALL and select the Default Profile indicator

