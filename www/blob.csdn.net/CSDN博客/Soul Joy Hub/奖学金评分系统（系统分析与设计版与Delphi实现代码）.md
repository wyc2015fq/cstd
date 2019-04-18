# 奖学金评分系统（系统分析与设计版与Delphi实现代码） - Soul Joy Hub - CSDN博客

2016年06月13日 09:55:56[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：2754


                
一、系统规划

## 1.1 项目背景介绍

在奖学金评比过程中，学生综合测评是学校普遍采用的评比手段。对学生实施综合素质测评的目的在于正确评价学生的综合素质，为评奖学金提供依据，实现学生教育管理工作的标准化、制度化和科学化，引导和促进学生德、智、体、美全面发展。

## 1.2 现存问题及系统目标

目前我国普遍高校学生奖学金评比还停留在纸质的阶段，许多工作需要传统的手工操作，这不仅浪费了大量的人力物力资源，而且由于人工管理存在着许多不可控因素，导致学生奖学金评比操作不规范，测评结果不全面，不能客观准确地反应学生的综合素质。因此，建立一套基于计算机环境的技术先进、高效稳定、安全可靠的奖学金测评系统是完全有必要的。

奖学金评比系统的目的是自动分析、计算、统计、管理学生综合测评数据,减少人工干预,降低计算失误,提高工作效率,将辅导员、班主任、班委从繁重的综合测评及日常管理中解放出来,使学校的综合测评工作系统化、规范化、自动化。

与传统的纸质评比相比，计算机管理不但能发挥计算机方便、快捷的功能，而且可以节省大量的资源费用等，更方便灵活操作，而且方便管理，增加奖学金评比的公平性、透明性，有效防止奖学金评比的弊端，使学生奖学金评比更加合理、更趋于成熟。这些都能够极大地提高工作效率，特别是校园网应用发展迅速的今天，利用网络进行信息的查询、修改和发布已成为学校现代化管理的象征之一，也是学校科学化和正规化管理的重要条件。因此开发一套比较成熟的学生奖学金评比系统很有必要。

## 1.3 可行性分析

1技术性分析 

使用delphi，支持数据库，操作简单方便。 

2 经济性分析 

花费费用小，节省人力物力。 

3 系统运行可行性分析 

可实现登记、查询、修改，比较等功能，切实可行。 

## 1.4进度安排

![](http://images.cnitblog.com/blog/709693/201501/201643275634103.png)

# 二、系统分析 

## 2.1功能需求分析

本系统主要进行学生的奖学金评比综合成绩考核和班委对学生情况的管理记相关的活动。主要包含如下功能模块：

- 
学生系统管理模块 

该模块提供学生进行学生信息查询。实现学生的个人信息管理。方便了学生管理以及成绩考核查询，奖学金评比。更能实现效率。

- 
班委系统管理模块 

该模块提供学生班委进行学生信息更改。实现班委的班级信息管理。方便了班委管理和成绩考核查询，奖学金评比更改。更能实现效率。

## 2.2非功能需求分析

- 

# 技术需求 

系统的学生、班委和教师组件要求运行在用windows操作系统的笔记本或台式机上。服务器组件要求必须使用C语言编写，并且相互之间使用组件交互标准CORBA（公用对象请求代理结构）或SOAP（缉拿的那对象访问协议）进行通信。

- 

# 性能需求 

系统的学生、班委和教师部分要求在所有的屏幕上都是半秒的响应时间，而服务器组件可能需要在同样的响应时间内支持上千个并发的学生教师操作。

# 3、可用性需求 

基于web的界面符合整个组织范围内的图形设计准则，像菜单布局和格式、色彩设计、组织标志的使用规范化。

# 4、可靠性需求 

当服务出现损耗或出现不正当处理、以及错误检测和恢复将会自动根据程序要求来进行自我修复和改进。

- 

# 安全需求 

对该系统输出的访问会受限于学生和教师，一些来自不属于学校的外人的访问需要得到授权，而其他的一些访问只是来自该机构的本地网络。该校可能需要喜爱通过Internet传送数据的时候要使用到加密技术，并且通过用户名和密码机制来控制对数据库服务器的访问。



![](http://images.cnitblog.com/blog/709693/201501/201643321255662.png)

## 2.3 ER图











## 2.4数据流程图

![](http://images.cnitblog.com/blog/709693/201501/201643330474279.png)

# 顶层DFD 

![](http://images.cnitblog.com/blog/709693/201501/201643342191737.png)

# 第一层DFD 

![](http://images.cnitblog.com/blog/709693/201501/201643362813866.png)

![](http://images.cnitblog.com/blog/709693/201501/201643373137281.png)

# 第二层DPD 

## 2.5数据字典

- 

# 数据项的定义 

|# 数据项编号|# 数据项名称|# 数据类型|# 简述|# 允许Null值|
|----|----|----|----|----|
|# I1|# number|Nvarch（50）|# 学生用户的学号|# false|
|# I2|# points|float|# 学生的分数|# true|
|# I3|# ranking|float|# 学生的优秀等级|# true|
|# I4|# name|Nvarchar（255）|# 学生用户姓名|# true|
|# I5|# credit|float|# 学生的学分|# true|
|# I6|# type|Nvarchar（50）|# 学生选课类型|# false|
|# I7|# Grade|float|# 学生年级|# true|
|# I8|# Usernum|Nvarchar（50）|# 用户账号|# true|
|# I9|# body|float|# 学生身体素质|# true|
|# I10|# mind|float|# 学生心理素质|# true|
|# I11|# self|float|# 学生道德素质|# true|
|# I12|# record|float|# 学生违反道德记录|# true|
|# I13|# course|float|# 学生所选课程名称|# false|
|# I14|# sex|Nvarchar（225）|# 学生性别|# true|
|# I15|# password|Nvarchar（225）|学生用户密码|# true|














- 

# 数据存储的描述 

|# 数据存储编号|# 数据存储名称|# 简述|# 数据存储组成|
|----|----|----|----|
|# S1|# 登录文件|# 记录用户登录|# 用户名+密码|
|# S2|# 权限文件|# 记录用户权限|# 用户名+密码+权限编码|
|# S3|# 基本项文件|# 记录学生基本项信息|# 基本信息+排名|
|# S4|# 综合能力文件|# 记录学生综合能力信息|# 基本项排名+综合能力排名|
|# S5|# 身心素质文件|# 记录学生身心素质信息|# 身体素质+心理素质评议+成绩分数+名次|
|# S6|# 专业素质文件|# 记录学生专业素质信息|# 成绩分数+名次|
|# S7|# 品德素质文件|# 记录学生品德素质信息|# 学生自评+学生代表评议+辅导员评议+记实+评议基本分+总分+名次|
|# S8|# 加分项目|# 记录学生加分项目信息|# 加分项目名称+分数+类型|

|# S9|# 选课表|# 记录学生选课信息|# 学生+学号+课程名称|
|----|----|----|----|
|# S10|# 获奖情况文件|# 记录学生获奖情况|# 学生+学号+获奖情况|
|# S11|# 评比方法文件|# 记录评比方法|# 评比对象+评比方法内容|


- 

# 数据流的描述 

|# 数据流编号|# 数据流名称|# 简述|# 数据流来源|# 数据流去向|# 数据流组成|# 数据流量|# 高峰流量|
|----|----|----|----|----|----|----|----|
|# D1|# 用户学号|# 登陆成功用户的学号|# P3|# P3.1|# 用户学号+姓名|# 11次/月|# 40次/月|
|# D2|# 所查学号|# 用户想要查询的学号|# P3|# P3.1|# 所查学号+姓名|# 22次/月|# 35次/月|
|# D3|# 学号|# 查询对象的相关内容|# 当前进行的操作|# 对应下一级指向操作|# 学生学号|# 15次/月|# 30次/月|
|# D4|# 登陆信息|# 用户账号密码|# P1，P2|# P2，I2，I3|# 账号+密码|# 15次/月|# 30次/月|
|# D5|# 出错信息|# 非法的用户账号或者密码提示|# P1|# 用户|# 出错内容的形式提示|# 10次/月|# 20次/月|
|# D6|# 评比方法|# 奖学金评比相关内容|# S10|# P3.8|# 评比规则+评比项目|# 11次/月|# 23次/月|
|# D7|# 更新后的文件信息|# 更新过基本信息后的待返回总测评更新的信息|# P4.8|# P4.1|# 专业素质成绩+品德素质成绩+身心素质成绩|# 5次/月|# 15次/月|



- 

# 处理的描述 

|# 处理逻辑编号|# 处理逻辑名称|# 输入的数据流|# 处理的逻辑描述|# 输出的数据流|# 处理的频率|
|----|----|----|----|----|----|
|# P1|# 判断登陆是否正确|# D4|# 系统根据输入账号密码判断合法性|# D4，D5|# 20次/月|
|# P2|# 判断权限|# D4|# 系统根据账号密码判断用户使用权限|# D4|# 20次/月|
|# P3.1|# 总测评查询|# D1|# 学生进行测评查询|# D3|# 20次/月|
|# P3.2|# 基本项查询处理|# D2|# 学生进行基本项查询|# D3|# 20次/月|
|# P3.3|# 综合能力查询处理|# D3|# 学生进行综合能力查询|# D3|# 20次/月|
|# P3.4|# 品德素质查询|# D3|# 学生进行品德素质查询|# D3|# 20次/月|
|# P3.5|# 专业素质查询|# D3|# 学生进行专业素质查询|# D3|# 20次/月|
|# P3.6|# 身心素质查询|# D3|# 学生进行身心素质查询|# D3|# 20次/月|
|# P3.7|# 获奖情况查询|# D3|# 学生进行获奖情况查询|# D3|# 20次/月|
|# P3.8|# 评比方法查询|# D3|# 学生可以查询获奖评比方法|# D3|# 20次/月|
|# P4.1|# 总测评更新|# D3|# 班委进行总测评处理|# D3|# 20次/月|
|# P4.2|# 总基本项更新|# D3|# 班委进行总基本项更新|# D3|# 20次/月|
|# P4.3|# 加分项更新|# D3|# 班委更新加分项|# D3|# 20次/月|
|# P4.4|# 品德素质更新|# D3|# 班委更新学生品德素质信息|# D3|# 20次/月|
|# P4.5|# 专业素质更新|# D3|# 班委更新学生专业素质信息|# D3|# 20次/月|
|# P4.6|# 身心素质更新|# D3|# 班委更新学生身心素质信息|# D3|# 20次/月|
|# P4.7|# 获奖情况更新|# D3|# 班委更新获奖情况信息|# D3|# 20次/月|
|# P4.8|# 整合文件信息|# D3|# 班委整合更新过的素质信息准备返回给总测评信息|# D3|# 20次/月|

## 2.6用例图

![](http://images.cnitblog.com/blog/709693/201501/201643386257781.png)

## 2.7 CRUD分析

|用例|数据实体| | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|
|加分项|奖项|综合能力|身心素质|品德素质|专业素质|基本项信息|选课信息|学生| |
|加分项查询|R|R|||||||R|
|专业素质查询||R||||R|||R|
|基本项查询||||R|R|R|R||R|
|选课查询||||||||R|R|
|综合能力查询||R|R||||||R|
|获奖情况查询||R|||||||R|
|评比方法查询|R||R|R|R|R|R|R|R|
|品德素质查询|||||R||R||R|
|专业素质更新||||||RUD|RUD||RUD|
|测评更新|RUD|||RUD|RUD|RUD|RUD||RUD|
|基本项更新||||RUD|RUD|RUD|RUD||RUD|
|综合能力更新|||RUD||||||RUD|
|品德素质更新|||||RUD||RUD||RUD|
|加分项更新|RUD|RUD|||||||RUD|
|选课更新||||||||RUD|RUD|
|获奖情况更新||RUD|||||||RUD|









# 三、系统分析

## 3.1 数据库的设计

![](http://images.cnitblog.com/blog/709693/201501/201643396723670.png)

![](http://images.cnitblog.com/blog/709693/201501/201643401887526.png)

![](http://images.cnitblog.com/blog/709693/201501/201643406885612.png)

![](http://images.cnitblog.com/blog/709693/201501/201643413286741.png)

![](http://images.cnitblog.com/blog/709693/201501/201643420167585.png)

![](http://images.cnitblog.com/blog/709693/201501/201643425312442.png)

![](http://images.cnitblog.com/blog/709693/201501/201643430319527.png)

![](http://images.cnitblog.com/blog/709693/201501/201643448756357.png)







## 3.2 应用程序架构设计

B/S结构（Browser/Server结构）结构即浏览器和服务器结构。它是随着Internet技术的兴起，对C/S结构的一种变化或者改进的结构。在这种结构下，用户工作界面是通过WWW浏览器来实现，极少部分事务逻辑在前端（Browser）实现，但是主要事务逻辑在服务器端（Server）实现，形成所谓三层3-tier结构。这样就大大简化了客户端电脑载荷，减轻了系统维护与升级的成本和工作量，降低了用户的总体成本（TCO）。以目前的技术看，局域网建立B/S结构的网络应用，并通过Internet/Intranet模式下数据库应用，相对易于把握、成本也是较低的。它是一次性到位的开发，能实现不同的人员，从不同的地点，以不同的接入方式（比如LAN,
 WAN, Internet/Intranet等）访问和操作共同的数据库；它能有效地保护数据平台和管理访问权限，服务器数据库也很安全 。特别是在JAVA这样的跨平台语言出现之后，B/S架构管理软件更是方便、速度快、效果优。

B/S模式最大特点是：用户可以通过WWW浏览器去访问Internet上的文本、数据、图像、动画、视频点播和声音信息，这些信息都是由许许多多的Web服务器产生的，而每一个Web服务器又可以通过各种方式与数据库服务器连接，大量的数据实际存放在数据库服务器中。客户端除了WWW浏览器，一般无须任何用户程序，只需从Web服务器上下载程序到本地来执行，在下载过程中若遇到与数据库有关的指令，由Web服务器交给数据库服务器来解释执行，并返回给Web服务器，Web服务器又返回给用户。在这种结构中，将许许多多的网连接到一块，形成一个巨大的网，即全球网。而各个企业可以在此结构的基础上建立自己的Intranet。

- 
B/S模式的优点和缺点 

B/S结构的优点 

（1）、具有分布性特点，可以随时随地进行查询、浏览等业务处理。 

（2）、业务扩展简单方便，通过增加网页即可增加服务器功能。 

（3）、维护简单方便，只需要改变网页，即可实现所有用户的同步更新。 

（4）、开发简单，共享性强 

B/S 模式的缺点 

（1）、个性化特点明显降低，无法实现具有个性化的功能要求。（个性化的要求取决以软件框架，而非架构，分享B/S软件就蛮灵活）

（2）、操作是以鼠标为最基本的操作方式，无法满足快速操作的要求。 （如果辅助于插件，照样可以用键盘快速操作）

（3）、页面动态刷新，响应速度明显降低。（分享软件用分页保证响应速度稳定） 

（4）、功能弱化，难以实现传统模式下的特殊功能要求。（技术问题，用微软C#或java怎么会不能解决任何要求呢）

二、C/S模式的优点和缺点 

C/S 模式的优点 

1.由于客户端实现与服务器的直接相连，没有中间环节，因此响应速度快。 （当数据少时，速度与B/S软件一般；当数据超过十万时，C/S软件变慢，B/S软件能维持稳定速度）

2.操作界面漂亮、形式多样，可以充分满足客户自身的个性化要求。（似乎可以随意排列界面，但遇到第二客户要求时又要从头做起，比较灵活）

3.C/S结构的管理信息系统具有较强的事务处理能力，能实现复杂的业务流程。 

C/S 模式的缺点 

1.需要专门的客户端安装程序，分布功能弱，针对点多面广且不具备网络条件的用户群体，不能够实现快速部署安装和配置。

2.兼容性差，对于不同的开发工具，具有较大的局限性。若采用不同工具，需要重新改写程序。 

3.开发成本较高，需要具有一定专业水准的技术人员才能完成。（就开发企业管理软件而言，C/S开发人员比B/S开发人员的成本低了许多）

综上所述，根据实际情况，我们组决定选择CS架构设计，因为在运行速度、数据安全、人机交互等方面，B/S远不如C/S。对于交互复杂的ERP等企业级应用，B/S则很难胜任，从全球范围看，成熟的ERP产品大多采用二层或三层C/S架构，B/S的ERP产品并不多见。在C/S中，客户端有一套完整的应用程序，在出错提示、在线帮助等方面都有强大的功能，并且可以在子程序间自由切换。B/S虽然提供了一定的交互能力，但与C/S的一整套客户应用相比还是太有限了。

C/S提供了更安全的存取模式。由于C/S是配对的点对点的结构模式，采用使用于局域网、安全性比较好的网络协议，安全性可以得到比较好的保证。而B/S采用点对多点，多点对多点这种开放的结构模式，并TCP/IP这一类运用于Interner的开放性协议，其安全性只能靠数据库服务器上管理密码的数据库来保证。

C/S逻辑上只有两层结构，而B/S采用了三层结构，所以C/S处理大量信息的能力是B/S所无法比拟的。并且C/S对于相同的任务，C/S完成的速度总比B/S快，使得C/S更有利于处理大量数据。

## 3.3 界面设计与功能实现代码

1.用户登录 

界面： 

![](http://images.cnitblog.com/blog/709693/201501/201643455163188.png)

代码： 
- 
unit Unit1; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, DB, ADODB, StdCtrls, Buttons, Menus, bsSkinData, BusinessSkinForm,

- 
  bsSkinCtrls, Mask, bsSkinBoxCtrls;

- 

- 
type 

- 
  TLogin = 
class(TForm) 

- 
    Label1: TLabel; 

- 
    Label2: TLabel; 

- 
    Label3: TLabel; 

- 
    ADOQuery1: TADOQuery;

- 
    RadioButton1: TRadioButton;

- 
    ComboBox1: TComboBox;

- 
    RadioButton2: TRadioButton;

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    BitBtn1: TbsSkinButton;

- 
    BitBtn2: TbsSkinButton;

- 
    ADOQuery2: TADOQuery;

- 
    Edit1: TbsSkinPasswordEdit;

- 
    procedure BitBtn2Click(Sender: TObject);

- 
    procedure FormCreate(Sender: TObject);

- 
    procedure BitBtn1Click(Sender: TObject);

- 
    procedure TBitBtn2Click(Sender: TObject);

- 

- 

- 
private

- 
    { Private declarations }

- 

- 
public

- 
    { Public declarations }

- 

- 
  end; 

- 

- 
var 

- 
  Login: TLogin; 

- 

- 
  var 

- 
i: Integer; 

- 
 num:integer; 

- 
username:String; 

- 
 Quanxian:String; 

- 
implementation 

- 

- 
uses Unit2, Unit8, Unit10;

- 

- 

- 

- 
{$R *.dfm} 

- 

- 

- 

- 

- 

- 

- 

- 

- 
procedure TLogin.TBitBtn2Click(Sender: TObject);

- 
begin 

- 
self.Close; 

- 
end; 

- 

- 
procedure TLogin.BitBtn2Click(Sender: TObject);

- 
begin 

- 
Login.Close; 

- 
end; 

- 

- 
procedure TLogin.BitBtn1Click(Sender: TObject);

- 
var 

- 

- 

- 

- 
  Password:String; 

- 
  str:string;

- 
  n:integer; 

- 
  temp:String; 

- 
  have:integer; 

- 
begin 

- 

- 
      {检查 用户是否存在于Login}

- 
      have:=0; 

- 
for n := 4 downto 1
do

- 
     begin 

- 
       ADOQuery1.Close;

- 
       ADOQuery1.SQL.Clear;

- 
       str:=floattostr(n);

- 
       ADOQuery1.SQL.Text:='select * from Login where Number='+str+'';

- 
       ADOQuery1.Open;

- 
       ADOQuery1.Next;

- 
       temp:=ADOQuery1.fieldbyname('UserNum').AsString;

- 

- 
if Combobox1.Text=temp then

- 
       have:=1; 

- 
     end; 

- 

- 

- 

- 

- 

- 
     {如果不存在 则保存 用户名称}

- 
if have=0 then

- 

- 
      begin 

- 

- 

- 
for n := 4 downto 1
do

- 
     begin 

- 

- 
      ADOQuery1.Close;

- 
    ADOQuery1.SQL.Clear;

- 
    str:=floattostr(n);

- 
    ADOQuery1.SQL.Text:='select * from Login where Number='+str+'';

- 
    ADOQuery1.Open; 

- 
    ADOQuery1.Next; 

- 
    temp:=ADOQuery1.fieldbyname('UserNum').AsString;

- 

- 
    str:=floattostr(n+1) ;

- 

- 

- 

- 
     with ADOQuery1 
do

- 
    begin 

- 
      Close; 

- 
      SQL.Clear; 

- 
      SQL.Add('update Login
set UserNum = :strMarry where Number = :strName ');

- 
      parameters.ParamByName('strMarry').Value := temp;

- 
      parameters.ParamByName('strName').Value := str;

- 
      execsql; 

- 
    end; 

- 

- 
     end; 

- 

- 

- 
     with ADOQuery1 
do

- 
    begin 

- 
      Close; 

- 
      SQL.Clear; 

- 
      SQL.Add('update Login
set UserNum = :strMarry where Number = :strName ');

- 
      parameters.ParamByName('strMarry').Value := ComboBox1.Text;

- 
      parameters.ParamByName('strName').Value := str;

- 
      execsql; 

- 
    end; 

- 

- 

- 
     end; 

- 

- 

- 

- 

- 

- 

- 
    {登入程序} 

- 
try

- 
    with adoquery1 
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('select * from Student where Number =:a and Password=:b and Type=:c');

- 
      parameters.ParamByName('a').Value:=trim(combobox1.Text);

- 
      parameters.ParamByName('b').Value:=trim(edit1.Text);

- 
if combobox1.Text ='' then

- 
        begin 

- 
          application.MessageBox('请输入用户名','提示信息',64);

- 
          combobox1.SetFocus;

- 
          exit; 

- 
        end; 

- 
if edit1.Text ='' then

- 
        begin 

- 
          application.MessageBox('请输入密码','提示信息',64);

- 
          edit1.SetFocus;

- 
          exit; 

- 
        end; 

- 
if radiobutton1.Checked=true then

- 
        begin 

- 
          Quanxian:='0';

- 
        end; 

- 
if radiobutton2.Checked=true then

- 
        begin 

- 
          Quanxian:='1';

- 
        end; 

- 
      parameters.ParamByName('c').Value:=trim(quanxian);

- 
      open; 

- 
      end; 

- 
if (adoquery1.RecordCount<>0) and (Quanxian='1') then

- 
        begin 

- 
          Username:=combobox1.Text;

- 
          Password:=edit1.Text;

- 
          application.MessageBox('登陆成功','提示信息',64);

- 
          self.hide; 

- 
          form8.show;

- 

- 

- 
        end 

- 

- 
elseif (adoquery1.RecordCount<>0) and (Quanxian='0') then

- 
        begin 

- 
          Username:=combobox1.Text;

- 
          Password:=edit1.Text;

- 
          application.MessageBox('登陆成功','提示信息',64);

- 
          self.hide; 

- 
          form8.show;

- 

- 

- 
        end 

- 

- 
else

- 
        begin 

- 
if num=0 then

- 
              Login.Close;

- 
          num:=num-1;

- 
          application.MessageBox('输入的用户名或密码错误','提示信息',64);

- 
        end; 

- 

- 
  except 

- 

- 
  begin 

- 
if num=0 then

- 
              Login.Close;

- 
       num:=num-1; 

- 
     application.MessageBox('输入的用户名或密码错误','提示信息',64);

- 

- 
  end; 

- 

- 
  end; 

- 

- 
end; 

- 

- 
procedure TLogin.FormCreate(Sender: TObject);

- 
var n:integer; 

- 
    str:String; 

- 
begin 

- 

- 

- 

- 

- 

- 

- 

- 

- 

- 
   num:= 3; 

- 

- 

- 

- 

- 
for n := 1 to 4
do

- 
   begin 

- 
    ADOQuery1.Close; 

- 
    ADOQuery1.SQL.Clear;

- 
    str:=floattostr(n);

- 
    ADOQuery1.SQL.Text:='select * from Login where Number='+str+'';

- 
    ADOQuery1.Open; 

- 
    ADOQuery1.Next; 

- 
    str:=ADOQuery1.fieldbyname('UserNum').AsString;

- 
if str<>'no' then

- 
   combobox1.Items.Add(str);

- 

- 

- 
   end; 

- 
end; 

- 

- 
End. 

2. 

![](http://images.cnitblog.com/blog/709693/201501/201644025008642.png)
- 
unit Unit8; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, bsSkinData, BusinessSkinForm, bsSkinCtrls, MPlayer, ExtCtrls,

- 
  OleCtrls, ShockwaveFlashObjects_TLB, StdCtrls, SkyGuage;

- 

- 
type 

- 
  TForm8 = 
class(TForm) 

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    SkyGuage1: TSkyGuage;

- 
    Timer1: TTimer; 

- 
    Image1: TImage; 

- 
    Label1: TLabel; 

- 
    procedure Timer1Timer(Sender: TObject);

- 
    procedure FormShow(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form8: TForm8; 

- 

- 
implementation 

- 

- 
uses Unit2, Unit1, Unit4;

- 

- 
{$R *.dfm} 

- 

- 

- 

- 
procedure TForm8.FormShow(Sender: TObject);

- 
begin 

- 
Image1.Picture.LoadFromFile('E:\微云\delphi\my奖学金评比系统\媒体文件\Login.bmp');

- 
        Timer1.Enabled:=true;

- 
timer1.Interval:=300;

- 
SkyGuage1.Progress:=0;

- 
end; 

- 

- 
procedure TForm8.Timer1Timer(Sender: TObject);

- 
begin 

- 
if label1.Caption='加载中……' then

- 
   label1.Caption:='加载中…'

- 
else

- 
     label1.Caption:='加载中……' ;

- 

- 

- 
SkyGuage1.Progress:=SkyGuage1.Progress+10;

- 
if SkyGuage1.Progress=100 then

- 
 begin 

- 
 timer1.Interval:=0; 

- 
 self.Close; 

- 
 MonitorForm.Show 

- 
 end; 

- 
end; 

- 

- 
End. 

- 
用户 

![](http://images.cnitblog.com/blog/709693/201501/201644055949888.png)
- 
unit Unit2; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, bsSkinData, BusinessSkinForm, bsSkinCtrls;

- 

- 
type 

- 
  TMonitorForm = 
class(TForm) 

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    bsSkinButton1: TbsSkinButton;

- 
    bsSkinButton2: TbsSkinButton;

- 
    bsSkinButton4: TbsSkinButton;

- 
    bsSkinButton3: TbsSkinButton;

- 
    procedure bsSkinButton1Click(Sender: TObject);

- 
    procedure bsSkinButton2Click(Sender: TObject);

- 
    procedure FormClose(Sender: TObject; var Action: TCloseAction);

- 
    procedure bsSkinButton4Click(Sender: TObject);

- 
    procedure bsSkinButton3Click(Sender: TObject);

- 
    procedure FormShow(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  MonitorForm: TMonitorForm;

- 
  bo:integer; 

- 
implementation 

- 

- 
uses Unit4, Unit6, Unit1, Unit12, Unit13;

- 

- 
{$R *.dfm} 

- 

- 
  procedure TMonitorForm.bsSkinButton1Click(Sender: TObject);

- 
begin 

- 
bo:=0; 

- 
MyText.show; 

- 
 self.hide; 

- 
end; 

- 

- 
procedure TMonitorForm.bsSkinButton2Click(Sender: TObject);

- 
begin 

- 
bo:=1; 

- 
self.Hide; 

- 
form6.Show; 

- 
end; 

- 

- 

- 

- 
procedure TMonitorForm.bsSkinButton3Click(Sender: TObject);

- 
begin 

- 
form13.show; 

- 
end; 

- 

- 
procedure TMonitorForm.bsSkinButton4Click(Sender: TObject);

- 
begin 

- 
 self.Hide; 

- 
 Login.show; 

- 
end; 

- 

- 
procedure TMonitorForm.FormClose(Sender: TObject; var Action: TCloseAction);

- 
begin 

- 
  Login.close; 

- 
end; 

- 

- 
procedure TMonitorForm.FormShow(Sender: TObject);

- 
begin 

- 
if quanxian='0' then

- 
     bsskinbutton3.Enabled:=false;

- 
end; 

- 

- 
End. 

- 
我的测评

![](http://images.cnitblog.com/blog/709693/201501/201644081722577.png)
- 
unit Unit4; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, ComCtrls, ExtCtrls, StdCtrls, bsSkinData, BusinessSkinForm,

- 
  bsSkinCtrls, bsPngImageList, DB, ADODB, Menus,ShellApi,WordXP,Comobj;

- 

- 
type 

- 
  TMyText = 
class(TForm) 

- 
    Image1: TImage; 

- 
    GroupBox1: TGroupBox;

- 
    Label1: TLabel; 

- 
    Label2: TLabel; 

- 
    Label3: TLabel; 

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    GroupBox2: TGroupBox;

- 
    Label4: TLabel; 

- 
    Label5: TLabel; 

- 
    Button1: TButton;

- 
    Button2: TButton;

- 
    Label6: TLabel; 

- 
    ADOQuery1: TADOQuery;

- 
    Label7: TLabel; 

- 
    Label8: TLabel; 

- 
    Label9: TLabel; 

- 
    Label10: TLabel; 

- 
    bsSkinMainMenu1: TbsSkinMainMenu;

- 
    N1: TMenuItem; 

- 
    N2: TMenuItem; 

- 
    N3: TMenuItem; 

- 
    N4: TMenuItem; 

- 
    N5: TMenuItem; 

- 
    N6: TMenuItem; 

- 
    N7: TMenuItem; 

- 
    bsSkinMainMenuBar1: TbsSkinMainMenuBar;

- 
    N8: TMenuItem; 

- 
    N9: TMenuItem; 

- 
    N11: TMenuItem; 

- 
    Label11: TLabel; 

- 
    Label12: TLabel; 

- 
    N12: TMenuItem; 

- 
    N13: TMenuItem; 

- 
    OpenDialog1: TOpenDialog;

- 
    N10: TMenuItem; 

- 
    N14: TMenuItem; 

- 
    N15: TMenuItem; 

- 
    N16: TMenuItem; 

- 
    N17: TMenuItem; 

- 
    procedure bsSkinButton1Click(Sender: TObject);

- 
    procedure FormClose(Sender: TObject; var Action: TCloseAction);

- 
    procedure Button1Click(Sender: TObject);

- 
    procedure bsSkinButton4Click(Sender: TObject);

- 
    procedure FormShow(Sender: TObject);

- 
    procedure N7Click(Sender: TObject);

- 
    procedure N9Click(Sender: TObject);

- 
    procedure N4Click(Sender: TObject);

- 
    procedure Button2Click(Sender: TObject);

- 
    procedure N10Click(Sender: TObject);

- 
    procedure N12Click(Sender: TObject);

- 
    procedure N11Click(Sender: TObject);

- 
    procedure N13Click(Sender: TObject);

- 
    procedure N14Click(Sender: TObject);

- 

- 

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  MyText: TMyText; 

- 

- 
implementation 

- 

- 
uses Unit6,Unit5, Unit9, Unit1, Unit2, Unit7, Unit11;

- 

- 

- 

- 
{$R *.dfm} 

- 

- 
procedure TMyText.bsSkinButton1Click(Sender: TObject);

- 

- 
var str:string;

- 
begin 

- 

- 

- 
if bo=0 then

- 

- 
   str:=username 

- 

- 
else

- 
    str:=str6; 

- 

- 

- 
Image1.Picture.LoadFromFile('E:\微云\delphi\my奖学金评比系统\信息1103 班级照片\'+str+'.bmp');

- 
  Label7.Caption:=str;

- 

- 
  ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select * from Student where Number='+str+'';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Label6.Caption:=ADOQuery1.fieldbyname('Name').AsString;

- 
Label8.Caption:=ADOQuery1.fieldbyname('Sex').AsString;

- 

- 

- 
 ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select * from Basic where Number='+str+'';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Label9.Caption:=ADOQuery1.fieldbyname('Ranking').AsString;

- 

- 

- 
ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select * from Comprehensive where Number='+str+'';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Label10.Caption:=ADOQuery1.fieldbyname('Ranking').AsString;

- 

- 

- 
end; 

- 

- 

- 

- 

- 

- 
procedure TMyText.bsSkinButton4Click(Sender: TObject);

- 
begin 

- 

- 

- 
self.hide; 

- 
form6.Show; 

- 

- 

- 
end; 

- 

- 
procedure TMyText.Button1Click(Sender: TObject);

- 
begin 

- 

- 
form5.show; 

- 
end; 

- 

- 
procedure TMyText.Button2Click(Sender: TObject);

- 
begin 

- 

- 
form9.show; 

- 
end; 

- 

- 
procedure TMyText.FormClose(Sender: TObject; var Action: TCloseAction);

- 
begin 

- 
Login.Close 

- 
end; 

- 

- 

- 

- 
procedure TMyText.FormShow(Sender: TObject);

- 
var 

- 
str:string;

- 
begin 

- 

- 

- 
if (bo=0)or (bo=2) then

- 

- 
   str:=username 

- 

- 
else

- 
    str:=str6; 

- 

- 

- 
Image1.Picture.LoadFromFile('E:\微云\delphi\my奖学金评比系统\信息1103 班级照片\'+str+'.bmp');

- 
  Label7.Caption:=str;

- 

- 
  ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select * from Student where Number='+str+'';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Label6.Caption:=ADOQuery1.fieldbyname('Name').AsString;

- 
Label8.Caption:=ADOQuery1.fieldbyname('Sex').AsString;

- 

- 

- 
 ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select * from Basic where Number='+str+'';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Label9.Caption:=ADOQuery1.fieldbyname('Ranking').AsString;

- 
Label11.Caption:=ADOQuery1.fieldbyname('Points').AsString;

- 

- 
ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select * from Comprehensive where Number='+str+'';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Label10.Caption:=ADOQuery1.fieldbyname('Ranking').AsString;

- 
Label12.Caption:=ADOQuery1.fieldbyname('Points').AsString;

- 

- 
end; 

- 

- 
procedure TMyText.N10Click(Sender: TObject);

- 
begin 

- 
docname:='e'; 

- 
form7.Caption:='素质评价方法';

- 
form7.show; 

- 
end; 

- 

- 
procedure TMyText.N11Click(Sender: TObject);

- 
begin 

- 
docname:='b'; 

- 
form7.Caption:='素质评价方法';

- 
form7.show; 

- 
end; 

- 

- 
procedure TMyText.N12Click(Sender: TObject);

- 
var 

- 
    fnm: String; 

- 
    WordApp,WordDoc: Variant;

- 
    sContext: String;

- 
begin 

- 

- 
end; 

- 

- 
procedure TMyText.N13Click(Sender: TObject);

- 
begin 

- 
docname:='d'; 

- 
form7.Caption:='素质评价方法';

- 
form7.show; 

- 
end; 

- 

- 
procedure TMyText.N14Click(Sender: TObject);

- 
begin 

- 
form11.show; 

- 
end; 

- 

- 
procedure TMyText.N4Click(Sender: TObject);

- 
begin 

- 
ShellExecute(Handle, 'open', PChar('C:\Windows\System32\calc.exe'), nil, nil, SW_SHOW);

- 
end; 

- 

- 
procedure TMyText.N7Click(Sender: TObject);

- 
begin 

- 

- 

- 
self.hide; 

- 
if bo=0 then

- 
MonitorForm.show 

- 
elseif bo=1 then 

- 
form6.Show; 

- 

- 
end; 

- 

- 
procedure TMyText.N9Click(Sender: TObject);

- 
begin 

- 
docname:='a'; 

- 
form7.Caption:='素质评价方法';

- 
form7.show; 

- 
end; 

- 

- 
End. 

- 
基本项 

![](http://images.cnitblog.com/blog/709693/201501/201644098759661.png)
- 
unit Unit5; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, StdCtrls, bsSkinData, BusinessSkinForm, bsSkinCtrls, Mask,

- 
  bsSkinBoxCtrls, DB, ADODB, Menus;

- 

- 
type 

- 
  TForm5 = 
class(TForm) 

- 
    GroupBox2: TGroupBox;

- 
    Label4: TLabel; 

- 
    Label5: TLabel; 

- 
    Label6: TLabel; 

- 
    Label7: TLabel; 

- 
    Label8: TLabel; 

- 
    Label9: TLabel; 

- 
    Label10: TLabel; 

- 
    GroupBox3: TGroupBox;

- 
    Label14: TLabel; 

- 
    Label17: TLabel; 

- 
    Button1: TButton;

- 
    GroupBox1: TGroupBox;

- 
    Label1: TLabel; 

- 
    Label2: TLabel; 

- 
    Label3: TLabel; 

- 
    Label11: TLabel; 

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    Label20: TLabel; 

- 
    Label23: TLabel; 

- 
    Label24: TLabel; 

- 
    Label25: TLabel; 

- 
    Label26: TLabel; 

- 
    ADOQuery1: TADOQuery;

- 
    bsSkinMainMenu1: TbsSkinMainMenu;

- 
    N1: TMenuItem; 

- 
    N2: TMenuItem; 

- 
    N3: TMenuItem; 

- 
    N4: TMenuItem; 

- 
    N5: TMenuItem; 

- 
    N7: TMenuItem; 

- 
    bsSkinMainMenuBar1: TbsSkinMainMenuBar;

- 
    bsSkinButton1: TbsSkinButton;

- 
    Edit1: TEdit; 

- 
    Edit2: TEdit; 

- 
    Edit3: TEdit; 

- 
    Button2: TButton;

- 
    Label12: TLabel; 

- 
    Edit6: TEdit; 

- 
    Edit7: TEdit; 

- 
    bsSkinButton2: TbsSkinButton;

- 
    Label13: TLabel; 

- 
    Label15: TLabel; 

- 
    procedure FormClose(Sender: TObject; var Action: TCloseAction);

- 
    procedure bsSkinButton2Click(Sender: TObject);

- 
    procedure FormShow(Sender: TObject);

- 
    procedure Button1Click(Sender: TObject);

- 
    procedure N7Click(Sender: TObject);

- 
    procedure bsSkinButton1Click(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form5: TForm5; 

- 

- 
implementation 

- 

- 
uses Unit4, Unit1, Unit6, Unit10, Unit9, Unit2;

- 

- 
{$R *.dfm} 

- 

- 
procedure TForm5.bsSkinButton1Click(Sender: TObject);

- 
begin 

- 

- 
application.MessageBox('基本测评分=品德素质×25%+专业素质×60%+身心素质'+

- 
'×15%'+#13+'品德素质分=评议成绩×70%+记实成绩×30%'+#13+

- 
'评议成绩=学生自评成绩×5%+学生代表评议成绩×70%+辅导员评议成绩×25%'+#13+

- 
'身心素质分=身体素质评价×70%+心理素质评价×30%','基本项评比方法',64);

- 
end; 

- 

- 
procedure TForm5.bsSkinButton2Click(Sender: TObject);

- 
var m1,m2,m3,m4,Bm:real;

- 
    temp,str:string;

- 
    i,n:integer; 

- 
begin 

- 
if bo=0 then

- 

- 
    str:=username 

- 

- 
else

- 
    str:=str6; 

- 

- 

- 
   {品德素质更新} 

- 

- 
    {Morality表} 

- 
    with adoquery1 
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('UPDATE Morality SET Self = :a, Student = :b,Instructor=:c,Points '+

- 
      '= :d WHERE Number = '+str);

- 
      parameters.ParamByName('a').Value:=trim(edit1.Text);

- 
      parameters.ParamByName('b').Value:=trim(edit2.Text);

- 
      parameters.ParamByName('c').Value:=trim(edit3.Text);

- 
      temp:=floattostr(strtoint(edit1.Text)*0.05+strtoint(edit2.Text)*0.7+strtoint(edit3.Text)*0.25);

- 
      parameters.ParamByName('d').Value:=trim(temp);

- 
       ExecSQL; 

- 
      end; 

- 

- 
   {读取人数} 

- 
ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select count(Number)
as n from Student'; 

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
n:=ADOQuery1.fieldbyname('n').AsInteger;

- 

- 

- 

- 
for I := 0 to n-1
do

- 
    begin 

- 
    {读取第i+1行号的人的学号} 

- 
     ADOQuery1.Close;

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select top 1 Number from Morality where Number not
in ( select'+ 

- 
' top '+inttostr(i)+' Number from Morality)';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
temp:=ADOQuery1.fieldbyname('Number').AsString;

- 

- 

- 

- 

- 
      ADOQuery1.Close;

- 
    ADOQuery1.SQL.Clear;

- 
    ADOQuery1.SQL.Text:='UPDATE Morality SET Ranking =(select COUNT(Number) from'+

- 
      ' Morality where Points>=(select Points from Morality where'+

- 
      ' Number= '''+temp+'''))+1 where Number='+temp+'';

- 
    ADOQuery1.ExecSQL;

- 

- 
    end; 

- 

- 

- 

- 

- 

- 

- 

- 

- 

- 

- 

- 

- 

- 

- 
    {身心素质更新} 

- 
    {MindandBoy} 

- 
      with adoquery1 
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('UPDATE MindAndBody SET Body = :a, Mind = :b,Points=:c '+

- 
      ' WHERE Number = '+str);

- 
      parameters.ParamByName('a').Value:=trim(edit6.Text);

- 
      parameters.ParamByName('b').Value:=trim(edit7.Text);

- 
      temp:=floattostr(strtofloat(edit6.Text)*0.7+strtofloat(edit7.Text)*0.3);

- 
      parameters.ParamByName('c').Value:=trim(temp);

- 
       ExecSQL; 

- 
      end; 

- 

- 

- 

- 
for I := 0 to n-1
do

- 
    begin 

- 
    {读取第i+1行号的人的学号} 

- 
ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select top 1 Number from MindAndBody where Number not
in ( select'+ 

- 
' top '+inttostr(i)+' Number from MindAndBody)';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
temp:=ADOQuery1.fieldbyname('Number').AsString;

- 

- 

- 

- 
      ADOQuery1.Close;

- 
    ADOQuery1.SQL.Clear;

- 
    ADOQuery1.SQL.Text:='UPDATE MindAndBody SET Ranking =(select COUNT(Number) from'+

- 
      ' MindAndBody where Points>=(select Points from MindAndBody where'+

- 
      ' Number= '''+temp+'''))+1 where Number='+temp+'';

- 
    ADOQuery1.ExecSQL;

- 

- 
    end; 

- 

- 

- 

- 

- 

- 
    application.MessageBox('更新成功！','提示信息',1);

- 

- 

- 

- 
end; 

- 

- 
procedure TForm5.Button1Click(Sender: TObject);

- 
begin 

- 
form9.show; 

- 
form9.bsskinComboBox1.ItemIndex:=1;

- 
end; 

- 

- 
procedure TForm5.FormClose(Sender: TObject; var Action: TCloseAction);

- 
begin 

- 
Login.close; 

- 
end; 

- 

- 
procedure TForm5.FormShow(Sender: TObject);

- 
var m1,m2,m3,m4,Bm:real;

- 
    temp,str,Bstr:string;

- 
begin 

- 

- 
if Quanxian='0' then

- 
begin 

- 
bsskinbutton2.Enabled:=false;

- 
edit1.Enabled:= 
false; 

- 
edit2.Enabled:= 
false; 

- 
edit3.Enabled:= 
false; 

- 
edit6.Enabled:= 
false; 

- 
edit7.Enabled:= 
false; 

- 
end; 

- 

- 

- 

- 

- 

- 
if (bo=0) or (bo=2) then

- 

- 
    str:=username 

- 

- 
else

- 
    str:=str6; 

- 

- 

- 
 {品德素质} 

- 
  ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select * from Morality where Number = '+str+'';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 

- 

- 
Edit1.Text:=ADOQuery1.fieldbyname('Self').AsString;

- 
Edit2.Text:=ADOQuery1.fieldbyname('Student').AsString;

- 
Edit3.Text:=ADOQuery1.fieldbyname('Instructor').AsString;

- 
Label13.Caption:=ADOQuery1.fieldbyname('Record').AsString;

- 
 m1:=strtofloat(Edit1.Text) ;

- 
 m2:=strtofloat(Edit2.Text) ;

- 
 m3:=strtofloat(Edit3.Text) ;

- 
 m4:=strtofloat(Label13.Caption) ;

- 
 Bm:=m1*0.05+m2*0.7+m3*0.25;

- 
 Bstr:=floattostr(Bm);

- 
 Bstr:=copy(Bstr,0,4);

- 
label15.Caption:=Bstr ;

- 

- 
temp:=ADOQuery1.fieldbyname('Points').AsString;

- 
temp:=copy(temp,0,4);

- 
Label12.Caption:=temp;

- 

- 
Label20.Caption:=ADOQuery1.fieldbyname('Ranking').AsString;

- 

- 

- 
{身心} 

- 
 ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select * from MindAndBody where Number='+str+'';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 

- 
edit6.Text:=ADOQuery1.fieldbyname('Body').AsString;

- 
edit7.Text:=ADOQuery1.fieldbyname('Mind').AsString;

- 

- 
temp:=ADOQuery1.fieldbyname('Points').AsString;

- 
temp:=copy(temp,0,4);

- 
Label23.Caption:=temp;

- 

- 
Label24.Caption:=ADOQuery1.fieldbyname('Ranking').AsString;

- 

- 

- 

- 
   {专业} 

- 
ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select * from Specialty where Number='+str+'';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 

- 
temp:=ADOQuery1.fieldbyname('Points').AsString;

- 
temp:=copy(temp,0,4);

- 
Label25.Caption:=temp;

- 

- 
Label26.Caption:=ADOQuery1.fieldbyname('Ranking').AsString;

- 
end; 

- 

- 
procedure TForm5.N7Click(Sender: TObject);

- 
begin 

- 
self.Hide; 

- 
   MyText.show 

- 

- 
end; 

- 

- 
End. 

6.专业素质 

![](http://images.cnitblog.com/blog/709693/201501/201644126723351.png)
- 
unit Unit10; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, BusinessSkinForm, bsSkinData, bsSkinCtrls, Menus, bsdbctrls,

- 
  bsSkinGrids, bsDBGrids, DB, ADODB, StdCtrls, bsSkinBoxCtrls;

- 

- 
type 

- 
  TForm10 = 
class(TForm) 

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    bsSkinData1: TbsSkinData;

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinMainMenu1: TbsSkinMainMenu;

- 
    N1: TMenuItem; 

- 
    N2: TMenuItem; 

- 
    N3: TMenuItem; 

- 
    N4: TMenuItem; 

- 
    N5: TMenuItem; 

- 
    N6: TMenuItem; 

- 
    N8: TMenuItem; 

- 
    N9: TMenuItem; 

- 
    N10: TMenuItem; 

- 
    N11: TMenuItem; 

- 
    N7: TMenuItem; 

- 
    bsSkinMainMenuBar1: TbsSkinMainMenuBar;

- 
    bsSkinDBNavigator1: TbsSkinDBNavigator;

- 
    ADOQuery1: TADOQuery;

- 
    ADOConnection1: TADOConnection;

- 
    DataSource1: TDataSource;

- 
    bsSkinDBGrid1: TbsSkinDBGrid;

- 
    Label1: TLabel; 

- 
    Label2: TLabel; 

- 
    Label3: TLabel; 

- 
    bsSkinComboBox1: TbsSkinComboBox;

- 
    Edit1: TEdit; 

- 
    Edit2: TEdit; 

- 
    bsSkinButton1: TbsSkinButton;

- 
    bsSkinButton2: TbsSkinButton;

- 
    bsSkinButton3: TbsSkinButton;

- 
    procedure FormShow(Sender: TObject);

- 
    procedure N7Click(Sender: TObject);

- 
    procedure FormCreate(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form10: TForm10; 

- 

- 
implementation 

- 

- 
uses Unit1, Unit5; 

- 

- 
{$R *.dfm} 

- 

- 
procedure TForm10.FormCreate(Sender: TObject);

- 
begin 

- 
bsSkinCombobox1.Items.Add('全部') ;

- 
bsSkinCombobox1.Items.Add('必修课') ;

- 
bsSkinCombobox1.Items.Add('专业选修课') ;

- 
bsskinComboBox1.ItemIndex:=0;

- 
end; 

- 

- 
procedure TForm10.FormShow(Sender: TObject);

- 
var 

- 
S : 
string; 

- 

- 
begin 

- 
S := 'select * from SC,Course where SC.Course=Course.CNumber and Number='+username;

- 
with ADOQuery1 
do

- 
    begin 

- 
        Close; 

- 
        SQL.Clear; 

- 
        SQL.Add(S); 

- 
        Open; 

- 
    end; 

- 
end; 

- 

- 
procedure TForm10.N7Click(Sender: TObject);

- 
begin 

- 
self.Hide; 

- 
form5.show; 

- 
end; 

- 

- 
End. 

- 
个人加分项

![](http://images.cnitblog.com/blog/709693/201501/201644148137737.png)
- 
unit Unit9; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, BusinessSkinForm, bsSkinData, bsSkinCtrls, Menus, bsdbctrls,

- 
  bsSkinGrids, bsDBGrids, DB, ADODB, StdCtrls, bsSkinBoxCtrls, Grids, DBGrids;

- 

- 
type 

- 
  TForm9 = 
class(TForm) 

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    bsSkinData1: TbsSkinData;

- 
    bsSkinDBNavigator1: TbsSkinDBNavigator;

- 
    bsSkinMainMenu1: TbsSkinMainMenu;

- 
    N1: TMenuItem; 

- 
    N2: TMenuItem; 

- 
    N3: TMenuItem; 

- 
    N4: TMenuItem; 

- 
    N5: TMenuItem; 

- 
    N6: TMenuItem; 

- 
    N7: TMenuItem; 

- 
    bsSkinMainMenuBar1: TbsSkinMainMenuBar;

- 
    ADOConnection1: TADOConnection;

- 
    ADOPerson: TADOQuery;

- 
    DataSource1: TDataSource;

- 
    bsskindbgrid1: TbsSkinDBGrid;

- 
    bsSkinComboBox1: TbsSkinComboBox;

- 
    Edit1: TEdit; 

- 
    Edit2: TEdit; 

- 
    Label1: TLabel; 

- 
    Label2: TLabel; 

- 
    Label3: TLabel; 

- 
    Add: TbsSkinButton;

- 
    bsSkinButton3: TbsSkinButton;

- 
    bsSkinButton1: TbsSkinButton;

- 
    ADOQuery1: TADOQuery;

- 
    procedure FormShow(Sender: TObject);

- 
    procedure N7Click(Sender: TObject);

- 
    procedure AddClick(Sender: TObject);

- 
    procedure FormCreate(Sender: TObject);

- 
    procedure bsskindbgrid1CellClick(Column: TbsColumn);

- 
    procedure bsSkinComboBox1Click(Sender: TObject);

- 
    procedure bsSkinButton3Click(Sender: TObject);

- 
    procedure bsSkinButton1Click(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form9: TForm9; 

- 
  S : 
string; 

- 
  str:string;

- 
implementation 

- 

- 
uses Unit1, Unit4, Unit3, Unit6, Unit2;

- 

- 
{$R *.dfm} 

- 

- 
procedure TForm9.AddClick(Sender: TObject);

- 
begin 

- 
form3.show 

- 
end; 

- 

- 

- 
procedure TForm9.bsSkinButton1Click(Sender: TObject);

- 
var Szuan,Syan,Szu,Sshe,Swen,n,i:integer;

- 
    sum:real; 

- 
    temp:string;

- 
begin 

- 
if bo=0 then

- 

- 
    str:=username 

- 

- 
else

- 
    str:=str6; 

- 

- 
    ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select sum(EGrade)
as n from SE,Extra where SE.EName=Extra.EName and Number= '+str+' and EType =''专业技能'' ';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Szuan:=ADOQuery1.fieldbyname('n').AsInteger;

- 

- 
    ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select sum(EGrade)
as n from SE,Extra where SE.EName=Extra.EName and Number= '+str+' and EType = ''研究创新'' ';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Syan:=ADOQuery1.fieldbyname('n').AsInteger;

- 

- 

- 

- 
    ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select sum(EGrade)
as n from SE,Extra where SE.EName=Extra.EName and Number= '+str+' and EType = ''组织领导'' ';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Szu:=ADOQuery1.fieldbyname('n').AsInteger;

- 

- 
    ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select sum(EGrade)
as n from SE,Extra where SE.EName=Extra.EName and Number= '+str+' and EType = ''社会实践'' ';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Sshe:=ADOQuery1.fieldbyname('n').AsInteger;

- 

- 
 ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select sum(EGrade)
as n from SE,Extra where SE.EName=Extra.EName and Number= '+str+' and EType = ''文体特长'' ';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
Swen:=ADOQuery1.fieldbyname('n').AsInteger;

- 

- 
sum:=75+Syan*0.3+Szuan*0.25+Szu*0.15+Sshe*0.15+Swen*0.15;

- 
  {更新 综合能力表的Point} 

- 
   with adoquery1 
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('UPDATE Comprehensive SET Points = '+floattostr(sum)+' WHERE Number = '+str);

- 
       ExecSQL; 

- 
      end; 

- 

- 
     {读取人数} 

- 
ADOQuery1.Close; 

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select count(Number)
as n from Student'; 

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
n:=ADOQuery1.fieldbyname('n').AsInteger;

- 

- 

- 

- 
{更新 综合能力表的Ranking} 

- 
for I := 0 to n-1
do

- 
    begin 

- 
    {读取第i+1行号的人的学号} 

- 
     ADOQuery1.Close;

- 
ADOQuery1.SQL.Clear; 

- 
ADOQuery1.SQL.Text:='select top 1 Number from Comprehensive where Number not
in ( select'+ 

- 
' top '+inttostr(i)+' Number from Comprehensive)';

- 
ADOQuery1.Open; 

- 
ADOQuery1.Next; 

- 
temp:=ADOQuery1.fieldbyname('Number').AsString;

- 

- 

- 

- 

- 
      ADOQuery1.Close;

- 
    ADOQuery1.SQL.Clear;

- 
    ADOQuery1.SQL.Text:='UPDATE Comprehensive SET Ranking =(select COUNT(Number) from'+

- 
      ' Comprehensive where Points>=(select Points from Comprehensive where'+

- 
      ' Number= '''+temp+'''))+1 where Number='+temp+'';

- 
    ADOQuery1.ExecSQL;

- 

- 
    end; 

- 

- 
application.MessageBox('更新成功！','提示信息',1);

- 

- 

- 

- 
end; 

- 

- 
procedure TForm9.bsSkinButton3Click(Sender: TObject);

- 
var n:integer; 

- 
begin 

- 

- 
with adoperson 
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('DELETE FROM SE WHERE EName=:a and Number = '+str);

- 
      parameters.ParamByName('a').Value:=trim(edit1.Text);

- 
      ExecSQL; 

- 
      end; 

- 

- 

- 

- 
if bsskincombobox1.Text='实记' then n:=1

- 
elseif bsskincombobox1.Text='专业技能' then n:=2 

- 
elseif bsskincombobox1.Text='研究创新' then n:=3 

- 
elseif bsskincombobox1.Text='组织领导' then n:=4 

- 
elseif bsskincombobox1.Text='社会实践' then n:=5 

- 
elseif bsskincombobox1.Text='文体特长' then n:=6; 

- 
  bsskinComboBox1.ItemIndex:=n;

- 

- 
end; 

- 

- 
procedure TForm9.bsSkinComboBox1Click(Sender: TObject);

- 
begin 

- 

- 
if bsskincombobox1.Text='全部' then

- 
s:= 'select * from SE,Extra where SE.EName=Extra.EName and Number= '+str

- 
else

- 
 S := 'select * from SE,Extra where SE.EName=Extra.EName and Number= '+str+' and EType = '''+bsSkinCombobox1.Text+''' ';

- 
  with ADOPerson 
do

- 
    begin 

- 
        Close; 

- 
        SQL.Clear; 

- 
        SQL.Add(S); 

- 
        Open; 

- 
    end; 

- 
end; 

- 

- 
procedure TForm9.bsskindbgrid1CellClick(Column: TbsColumn);

- 
var 

- 
temp:string;

- 
n:integer; 

- 
begin 

- 
edit1.text:=ADOPerson.fieldbyname('EName').asstring;

- 
edit2.text:=ADOPerson.fieldbyname('EGrade').asstring;

- 
temp:= ADOPerson.fieldbyname('EType').asstring;

- 

- 

- 
if temp='实记' then n:=1

- 
elseif temp='专业技能' then n:=2 

- 
elseif temp='研究创新' then n:=3 

- 
elseif temp='组织领导' then n:=4 

- 
elseif temp='社会实践' then n:=5 

- 
elseif temp='文体特长' then n:=6; 

- 
  bsskinComboBox1.ItemIndex:=n;

- 
end; 

- 

- 
procedure TForm9.FormCreate(Sender: TObject);

- 
begin 

- 
bsSkinCombobox1.Items.Add('全部') ;

- 
bsSkinCombobox1.Items.Add('实记') ;

- 
bsSkinCombobox1.Items.Add('专业技能') ;

- 
bsSkinCombobox1.Items.Add('研究创新') ;

- 
bsSkinCombobox1.Items.Add('组织领导') ;

- 
bsSkinCombobox1.Items.Add('社会实践') ;

- 
bsSkinCombobox1.Items.Add('文体特长') ;

- 
bsskinComboBox1.ItemIndex:=0;

- 
end; 

- 

- 
procedure TForm9.FormShow(Sender: TObject);

- 

- 

- 
begin 

- 

- 

- 
if (bo=0)or (bo=2) then

- 

- 
   str:=username 

- 

- 
else

- 
    str:=str6; 

- 

- 

- 
if quanxian='0' then

- 
    begin 

- 
      add.Enabled:=false;

- 
      bsskinbutton3.Enabled:=false;

- 
    end; 

- 

- 

- 

- 

- 
S := 'select * from SE,Extra where SE.EName=Extra.EName and Number='+str;

- 
with ADOPerson 
do

- 
    begin 

- 
        Close; 

- 
        SQL.Clear; 

- 
        SQL.Add(S); 

- 
        Open; 

- 
    end; 

- 
end; 

- 

- 

- 
procedure TForm9.N7Click(Sender: TObject);

- 
begin 

- 
self.Hide; 

- 
MyText.show 

- 
end; 

- 

- 
End. 

- 
加分项总汇

![](http://images.cnitblog.com/blog/709693/201501/201644170007138.png)
- 
unit Unit3; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, bsSkinCtrls, StdCtrls, bsSkinBoxCtrls, bsSkinData, BusinessSkinForm,

- 
  bsSkinGrids, bsDBGrids, bsdbctrls, Menus, DB, ADODB;

- 

- 
type 

- 
  TForm3 = 
class(TForm) 

- 
    bsSkinMainMenuBar1: TbsSkinMainMenuBar;

- 
    bsSkinMainMenu1: TbsSkinMainMenu;

- 
    N1: TMenuItem; 

- 
    N2: TMenuItem; 

- 
    N3: TMenuItem; 

- 
    N4: TMenuItem; 

- 
    N5: TMenuItem; 

- 
    N6: TMenuItem; 

- 
    N7: TMenuItem; 

- 
    bsSkinDBNavigator1: TbsSkinDBNavigator;

- 
    bsSkinDBGrid1: TbsSkinDBGrid;

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    Label1: TLabel; 

- 
    Label2: TLabel; 

- 
    Label3: TLabel; 

- 
    bsSkinComboBox1: TbsSkinComboBox;

- 
    Edit1: TEdit; 

- 
    Edit2: TEdit; 

- 
    bsSkinButton1: TbsSkinButton;

- 
    bsSkinButton2: TbsSkinButton;

- 
    bsSkinButton3: TbsSkinButton;

- 
    ADOQuery1: TADOQuery;

- 
    DataAll: TDataSource;

- 
    bsSkinButton4: TbsSkinButton;

- 
    procedure FormCreate(Sender: TObject);

- 
    procedure FormShow(Sender: TObject);

- 
    procedure bsSkinComboBox1Change(Sender: TObject);

- 
    procedure bsSkinDBGrid1CellClick(Column: TbsColumn);

- 
    procedure FormClose(Sender: TObject; var Action: TCloseAction);

- 
    procedure bsSkinButton1Click(Sender: TObject);

- 
    procedure bsSkinComboBox1Click(Sender: TObject);

- 
    procedure bsSkinButton3Click(Sender: TObject);

- 
    procedure bsSkinButton2Click(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form3: TForm3; 

- 
  str:string;

- 
implementation 

- 

- 
uses Unit1, Unit2, Unit6, Unit9;

- 

- 
{$R *.dfm} 

- 

- 
procedure TForm3.bsSkinButton1Click(Sender: TObject);

- 

- 
begin 

- 
 with form9.ADOPerson
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('INSERT INTO SE VALUES (:a,:b)');

- 
      parameters.ParamByName('a').Value:=trim(str);

- 
       parameters.ParamByName('b').Value:=trim(edit1.Text);

- 
      ExecSQL; 

- 
      end; 

- 

- 
 form9.bsskinComboBox1.ItemIndex:=0;

- 
end; 

- 

- 
procedure TForm3.bsSkinButton2Click(Sender: TObject);

- 
begin 

- 
with ADOQuery1 
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('DELETE FROM Extra WHERE EName=:a');

- 
      parameters.ParamByName('a').Value:=trim(edit1.Text);

- 
      ExecSQL; 

- 
      end; 

- 
bsskinComboBox1.ItemIndex:=0;

- 
end; 

- 

- 
procedure TForm3.bsSkinButton3Click(Sender: TObject);

- 
var n:integer; 

- 
begin 

- 
with ADOQuery1 
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('DELETE FROM Extra WHERE EName=:a');

- 
      parameters.ParamByName('a').Value:=trim(edit1.Text);

- 
      ExecSQL; 

- 
      end; 

- 

- 
if bsskincombobox1.Text='实记' then n:=1

- 
elseif bsskincombobox1.Text='专业技能' then n:=2 

- 
elseif bsskincombobox1.Text='研究创新' then n:=3 

- 
elseif bsskincombobox1.Text='组织领导' then n:=4 

- 
elseif bsskincombobox1.Text='社会实践' then n:=5 

- 
elseif bsskincombobox1.Text='文体特长' then n:=6; 

- 
  bsskinComboBox1.ItemIndex:=n;

- 

- 
end; 

- 

- 
procedure TForm3.bsSkinComboBox1Change(Sender: TObject);

- 
var s:string;

- 
begin 

- 

- 
end; 

- 

- 
procedure TForm3.bsSkinComboBox1Click(Sender: TObject);

- 
begin 

- 

- 
if bsskincombobox1.Text='全部' then

- 
s:= 'select * from Extra'

- 
else

- 
S := 'select * from Extra where EType = '''+bsSkinCombobox1.Text+''' ';

- 

- 

- 

- 

- 
  with ADOQuery1 
do

- 
    begin 

- 
        Close; 

- 
        SQL.Clear; 

- 
        SQL.Add(S); 

- 
        Open; 

- 
    end; 

- 
end; 

- 

- 
procedure TForm3.bsSkinDBGrid1CellClick(Column: TbsColumn);

- 
var 

- 
temp:string;

- 
n:integer; 

- 
begin 

- 
edit1.text:=adoquery1.fieldbyname('EName').asstring;

- 
edit2.text:=adoquery1.fieldbyname('EGrade').asstring;

- 
temp:= adoquery1.fieldbyname('EType').asstring;

- 

- 
if temp='实记' then n:=1

- 
elseif temp='专业技能' then n:=2 

- 
elseif temp='研究创新' then n:=3 

- 
elseif temp='组织领导' then n:=4 

- 
elseif temp='社会实践' then n:=5 

- 
elseif temp='文体特长' then n:=6; 

- 
  bsskinComboBox1.ItemIndex:=n;

- 
end; 

- 

- 
procedure TForm3.FormClose(Sender: TObject; var Action: TCloseAction);

- 
begin 

- 
form9.add.enabled:=true;

- 
end; 

- 

- 
procedure TForm3.FormCreate(Sender: TObject);

- 
begin 

- 
 bsSkinCombobox1.Items.Add('全部') ;

- 
bsSkinCombobox1.Items.Add('实记') ;

- 
bsSkinCombobox1.Items.Add('专业技能') ;

- 
bsSkinCombobox1.Items.Add('研究创新') ;

- 
bsSkinCombobox1.Items.Add('组织领导') ;

- 
bsSkinCombobox1.Items.Add('社会实践') ;

- 
bsSkinCombobox1.Items.Add('文体特长') ;

- 
bsskinComboBox1.ItemIndex:=0;

- 
end; 

- 

- 
procedure TForm3.FormShow(Sender: TObject);

- 
var 

- 
s:string;

- 
begin 

- 
form9.add.enabled:=false;

- 

- 
if (bo=0)or (bo=2) then

- 

- 
   str:=username 

- 

- 
else

- 
    str:=str6; 

- 

- 
S := 'select * from Extra';

- 
with ADOQuery1 
do

- 
    begin 

- 
        Close; 

- 
        SQL.Clear; 

- 
        SQL.Add(S); 

- 
        Open; 

- 
    end; 

- 
end; 

- 

- 
end. 

- 
评比方法

![](http://images.cnitblog.com/blog/709693/201501/201644214695372.png)
- 
unit Unit7; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, bsSkinData, BusinessSkinForm, StdCtrls, bsSkinBoxCtrls, Word2000,

- 
  OleServer,Comobj, OleCtnrs, Menus, ExtCtrls;

- 

- 
type 

- 
  TForm7 = 
class(TForm) 

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    MainMenu1: TMainMenu;

- 
    MainMenu2: TMainMenu;

- 
    OleContainer1: TOleContainer;

- 
    procedure FormShow(Sender: TObject);

- 

- 

- 

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form7: TForm7; 

- 
  docname:string;

- 
implementation 

- 

- 
{$R *.dfm} 

- 

- 

- 

- 
procedure TForm7.FormShow(Sender: TObject);

- 
begin 

- 

- 
oleContainer1.CreateObjectFromFile('E:\微云\delphi\my奖学金评比系统\data\'+docname+'.doc',False);

- 
  oleContainer1.DoVerb(ovShow);

- 

- 
end; 

- 

- 
End. 

- 
公共查询 

![](http://images.cnitblog.com/blog/709693/201501/201644252815302.png)
- 
unit Unit6; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, bsSkinData, BusinessSkinForm, bsSkinCtrls, StdCtrls, Mask,

- 
  bsSkinBoxCtrls, Menus;

- 

- 
type 

- 
  TForm6 = 
class(TForm) 

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    Label1: TLabel; 

- 
    bsSkinEdit1: TbsSkinEdit;

- 
    bsSkinButton1: TbsSkinButton;

- 
    bsSkinMainMenu1: TbsSkinMainMenu;

- 
    N1: TMenuItem; 

- 
    N2: TMenuItem; 

- 
    N3: TMenuItem; 

- 
    N4: TMenuItem; 

- 
    N5: TMenuItem; 

- 
    N6: TMenuItem; 

- 
    N8: TMenuItem; 

- 
    N9: TMenuItem; 

- 
    N10: TMenuItem; 

- 
    N11: TMenuItem; 

- 
    N7: TMenuItem; 

- 
    bsSkinMainMenuBar1: TbsSkinMainMenuBar;

- 
    procedure FormClose(Sender: TObject; var Action: TCloseAction);

- 
    procedure bsSkinButton1Click(Sender: TObject);

- 
    procedure N7Click(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form6: TForm6; 

- 
  str6:string;

- 
implementation 

- 

- 
uses Unit1, Unit4, Unit2;

- 

- 
{$R *.dfm} 

- 

- 
procedure TForm6.bsSkinButton1Click(Sender: TObject);

- 
begin 

- 
str6:=bsSkinEdit1.Text;

- 
self.Hide; 

- 
 MyText.show; 

- 
end; 

- 

- 
procedure TForm6.FormClose(Sender: TObject; var Action: TCloseAction);

- 
begin 

- 
Login.Close; 

- 
end; 

- 

- 
procedure TForm6.N7Click(Sender: TObject);

- 
begin 

- 

- 
self.hide; 

- 
MonitorForm.show; 

- 

- 

- 

- 
end; 

- 

- 
End. 

- 
评奖评优 

![](http://images.cnitblog.com/blog/709693/201501/201644258605646.png)
- 
unit Unit13; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, bsSkinCtrls, bsSkinData, BusinessSkinForm;

- 

- 
type 

- 
  TForm13 = 
class(TForm) 

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    bsSkinButton1: TbsSkinButton;

- 
    bsSkinButton2: TbsSkinButton;

- 
    bsSkinButton3: TbsSkinButton;

- 
    bsSkinButton4: TbsSkinButton;

- 
    procedure bsSkinButton1Click(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form13: TForm13; 

- 

- 
implementation 

- 

- 
uses Unit12; 

- 

- 
{$R *.dfm} 

- 

- 
procedure TForm13.bsSkinButton1Click(Sender: TObject);

- 
begin 

- 
form12.show; 

- 
end; 

- 

- 
End. 

![](http://images.cnitblog.com/blog/709693/201501/201644293283005.png)
- 
unit Unit12; 

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, StdCtrls, Buttons, bsSkinCtrls, bsSkinBoxCtrls, DB, ADODB,

- 
  bsSkinData, BusinessSkinForm, bsSkinGrids, bsDBGrids;

- 

- 
type 

- 
  TForm12 = 
class(TForm) 

- 
    bsSkinDBGrid1: TbsSkinDBGrid;

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    ADOQuery1: TADOQuery;

- 
    DataSource1: TDataSource;

- 
    GroupBox1: TGroupBox;

- 
    GroupBox2: TGroupBox;

- 
    GroupBox3: TGroupBox;

- 
    Label1: TLabel; 

- 
    Label2: TLabel; 

- 
    Label3: TLabel; 

- 
    Label4: TLabel; 

- 
    Label5: TLabel; 

- 
    Label6: TLabel; 

- 
    Label7: TLabel; 

- 
    Label8: TLabel; 

- 
    Label9: TLabel; 

- 
    Label10: TLabel; 

- 
    Label11: TLabel; 

- 
    Label12: TLabel; 

- 
    Label13: TLabel; 

- 
    Label14: TLabel; 

- 
    Edit1: TEdit; 

- 
    Edit2: TEdit; 

- 
    Edit3: TEdit; 

- 
    Edit4: TEdit; 

- 
    bsSkinComboBox1: TbsSkinComboBox;

- 
    BitBtn1: TBitBtn;

- 
    BitBtn2: TBitBtn;

- 
    Label15: TLabel; 

- 
    Label16: TLabel; 

- 
    Label17: TLabel; 

- 
    Label18: TLabel; 

- 
    Label19: TLabel; 

- 
    ADOQuery2: TADOQuery;

- 
    RadioButton1: TRadioButton;

- 
    RadioButton2: TRadioButton;

- 
    RadioButton3: TRadioButton;

- 
    procedure bsSkinDBGrid1CellClick(Column: TbsColumn);

- 
    procedure FormCreate(Sender: TObject);

- 
    procedure BitBtn1Click(Sender: TObject);

- 
    procedure BitBtn2Click(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form12: TForm12; 

- 

- 
implementation 

- 

- 
uses Unit11; 

- 

- 
{$R *.dfm} 

- 

- 
procedure TForm12.BitBtn1Click(Sender: TObject);

- 
begin 

- 
 with adoquery2 
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('select * from Prize where C1=1');

- 
       open; 

- 
    end; 

- 
if adoquery2.RecordCount<>0 then

- 
  application.MessageBox('已有一等奖名额','提示信息',64)

- 
else

- 
 begin 

- 
    label1.Caption:=edit1.Text;

- 
    label2.Caption:=edit2.Text;

- 
    form11.Label5.Caption:=edit2.Text;

- 
    form11.Image2.Picture.LoadFromFile('E:\微云\delphi\my奖学金评比系统\信息1103 班级照片\'+edit1.Text+'.bmp');

- 
     with adoquery2 
do

- 
    begin 

- 
      close; 

- 
      sql.clear; 

- 
      sql.add('UPDATE Prize SET C1=1 WHERE Number =:a ');

- 
      parameters.ParamByName('a').Value:=trim(edit1.Text);

- 
      ExecSQL; 

- 
      end; 

- 
 end; 

- 

- 

- 

- 

- 
end; 

- 

- 
procedure TForm12.BitBtn2Click(Sender: TObject);

- 

- 
begin 

- 
if RadioButton1.Checked then

- 
     begin 

- 

- 
       with adoquery2
do

- 
        begin 

- 
        close; 

- 
        sql.clear; 

- 
         sql.add('UPDATE Prize SET C1=0 WHERE Number =:a ');

- 
        parameters.ParamByName('a').Value:=trim(label1.Caption);

- 
         ExecSQL; 

- 
        end ; 

- 
       label1.Caption:='';

- 
       label2.Caption:='';

- 
     end; 

- 

- 
end; 

- 

- 
procedure TForm12.bsSkinDBGrid1CellClick(Column: TbsColumn);

- 
begin 

- 
edit1.text:=ADOquery1.fieldbyname('Number').asstring;

- 
edit2.text:=ADOquery1.fieldbyname('Name').asstring;

- 
edit3.text:=ADOquery1.fieldbyname('Ranking').asstring;

- 
edit4.text:=ADOquery1.fieldbyname('Ranking_1').asstring;

- 
end; 

- 

- 
procedure TForm12.FormCreate(Sender: TObject);

- 
begin 

- 

- 
bsSkinCombobox1.Items.Add('一等奖') ;

- 
bsSkinCombobox1.Items.Add('二等奖') ;

- 
bsSkinCombobox1.Items.Add('三等奖') ;

- 

- 
end; 

- 

- 
End. 

获奖情况 
- 
![](http://images.cnitblog.com/blog/709693/201501/201644335476695.png) unit Unit11;

- 

- 
interface

- 

- 
uses 

- 
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,

- 
  Dialogs, ExtCtrls, bsSkinData, BusinessSkinForm, bsSkinCtrls, StdCtrls;

- 

- 
type 

- 
  TForm11 = 
class(TForm) 

- 
    Image1: TImage; 

- 
    bsBusinessSkinForm1: TbsBusinessSkinForm;

- 
    bsSkinData1: TbsSkinData;

- 
    bsCompressedStoredSkin1: TbsCompressedStoredSkin;

- 
    GroupBox1: TGroupBox;

- 
    Image2: TImage; 

- 
    bsSkinGroupBox1: TbsSkinGroupBox;

- 
    Image3: TImage; 

- 
    Image4: TImage; 

- 
    Image5: TImage; 

- 
    Image6: TImage; 

- 
    Label1: TLabel; 

- 
    Label2: TLabel; 

- 
    Label3: TLabel; 

- 
    Label4: TLabel; 

- 
    Label5: TLabel; 

- 
    GroupBox2: TGroupBox;

- 
    Image7: TImage; 

- 
    Image8: TImage; 

- 
    Label7: TLabel; 

- 
    Label6: TLabel; 

- 
    procedure FormShow(Sender: TObject);

- 
private

- 
    { Private declarations }

- 
public

- 
    { Public declarations }

- 
  end; 

- 

- 
var 

- 
  Form11: TForm11; 

- 

- 
implementation 

- 

- 
{$R *.dfm} 

- 

- 
procedure TForm11.FormShow(Sender: TObject);

- 
begin 

- 
Image1.Picture.LoadFromFile('E:\微云\delphi\my奖学金评比系统\媒体文件\g.bmp');

- 

- 
end; 

- 

- 
end. 

