# Stata 中 profile.do 的设定 - stata连享会 - CSDN博客





2018年11月13日 09:49:26[arlionn](https://me.csdn.net/arlionn)阅读数：894
个人分类：[stata基础](https://blog.csdn.net/arlionn/article/category/7157148)

所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)









> 
作者：连玉君 ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))


Stata连享会： [精品专题现场班](https://gitee.com/arlionn/stata_training/blob/master/README.md)- Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
下载：[老连的 profile.do](https://gitee.com/arlionn/StataProfile)

Note：对于微信公众号用户，请点击底部 **【阅读原文】** 查看下载链接。


今天说说多数 Stata 用户都知道的东西 (知道和熟悉是两回事) —— 就是这货：

![Stata 连享会：profile.do 文档的设定](https://upload-images.jianshu.io/upload_images/7692714-ccb2756cb8aa3340.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

每次一开机，Stata 会自动找到这个名为 **[profile.do](http://profile.do)** 的文件，并执行里面的所有命令，显示为 **running D:\stata15\[profile.do](http://profile.do) …**

这些命令可能是为了满足如下诉求：

> 
**Stata: 开机基本诉求**
- 每次打开 Stata，我希望她能把屏幕上呈现的那些结果自动记录在一个 txt 文档中，以便发给老板查看，让他知道我有多努力；或是在电脑崩溃时可以找回自己悲惨兮兮写出的那些命令，……。**这很容易设定！**
- 每次打开 Stata，我都希望只需输入几个字母就能打开我经常放问的一些文件夹。比如，查看 [Stata Journal](https://gitee.com/Stata002/StataSX2018/tree/master/_001_StataJournal) 各期 PDF 文件，比如进入我备课的文件夹，……。**这些也很容易设定！**

……


### 1. 最重要的设定

根据我的经验，最重要的设定只有**两个**：

**一个**是设定文件路径，以便每次使用 `ssc install` 或 `github install` 等命令安装外部命令时，Stata 可以自动将下载的文件放到合适的位置，方便我们调用 (参见 [「Stata: 外部命令的搜索、安装与使用」](https://www.jianshu.com/p/9b8ecf8f332e))；

**另一个**是开机是自动生成日志文件，记录在屏幕上显示的所有信息，以便于我们追踪自己的工作历程，在突然死机等突发情况下找回那些已经执行过但没有及时保存的代码。

其他诸多可以自定设定的参数都在这个页面中：`help set`。包括：结果窗口中的行距 (`help linegap`)，图形模板 (`help set scheme`) 等等。

#### 1.1 设定外部命令的下载路径

**[profile.do](http://profile.do)** 文件中最核心的命令就两条：(详情参见 `help sysdir` 和 `help set`)

```
*-外部命令的存放位置 (下载路径)
sysdir set PLUS "`c(sysdir_stata)'ado\plus"    
*-个人文件夹位置
sysdir set PERSONAL "`c(sysdir_stata)'ado\personal"
```

这里，`sysdir set PLUS ……` 指定了日后外部命令的存放位置：在 Stata 安装路径下的 **ado\plus** 子文件夹中。其中，**`c(sysdir_stata)’** 是一个暂元，里面存放了 Stata 的安装路径：输入 `sysdir` 后显示的第一个文件路径。例如，我的 stata15 存放于 **D盘** 根目录下，所以，**`c(sysdir_stata)’ = D:\stata15** 。

**补充说明：**
- 你可以临时更改外部命令的下载路径，命令为：
`net set ado dirname` (存放 **.ado** 和 **.hlp** 文件的路径)；
`net set other dirname` (存放作者提供的数据文件和范例 dofiles 等附件文件)。
#### 1.2 开机自动生成日志文件

在 Stata 中，可以配合使用 `log using filename` 和 `log close` 命令来开启和关闭日志文件。对于老用户来讲，这显得很麻烦。在 **[profile.do](http://profile.do)** 文件中添加如下几行命令就可以实现自动生成以开启 Stata 的时间为文件名称的日志文件：

```
cap cd `c(sysdir_stata)'do
if _rc{
   mkdir `c(sysdir_stata)'do  //检测后发现无 do 文件夹，则自行建立一个
}

local fn = subinstr("`c(current_time)'",":","-",2)
local fn1 = subinstr("`c(current_date)'"," ","",3)
log    using `c(sysdir_stata)'do\log-`fn1'-`fn'.log, text replace
cmdlog using `c(sysdir_stata)'do\cmd-`fn1'-`fn'.log, replace
```

前面四行语句用于确认 Stata 安装目录下是否有 **【do】** 文件夹，如果没有就自动新建一个。

后面四行用于从电脑中的日期和时间中去除字符串，将其作为新生成日志文件的文件名。主要的好处是，即使你一天内开启-关闭无数次 Stata，日志文件都不会重名：光阴滚滚向东流呀！

下面是我的 **【D:\stata15\do】** 文件夹下自动存放的日志文件：

![Stata-profile.do - 自动存储的 log文件](https://upload-images.jianshu.io/upload_images/7692714-de7d9564f7aa43e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 2. 一些锦上添花的设定

对于一个每天都会开着 Stata 工作的用户来讲，如果可以把 Stata 作为一个控制台，从这里发布指令，也是一个蛮幸福的事情。比如，可以迅速打开常用文件夹、快捷地打开经常访问的网站，等等。

#### 2.1 显示常用网址

![Stata-profile.do - 开机显示常用网址](https://upload-images.jianshu.io/upload_images/7692714-eb320485d377d2d0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 2.2 快速打开常用文件夹

在 **[profile.do](http://profile.do)** 文件中定义一些小程序，核心命令其实就是 `cd` 和 `cdout`，就可以快捷地打开常用文件夹了。

比如，定义一个名为 **`sj`** (Stata Joural 的简称) 的小程序：

![Stata-profile.do - 快速打开常用文件夹](https://upload-images.jianshu.io/upload_images/7692714-613880d443722f1a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

任何时候，只需在 Stata 命令窗口输入 `sj` 命令就可以打开存放 **[Stata Journal](https://gitee.com/Stata002/StataSX2018/tree/master/_001_StataJournal)** 的文件夹了：

![image.png](https://upload-images.jianshu.io/upload_images/7692714-2982f0e6561a83db.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 附录：连玉君的 [profile.do](http://profile.do) 文档
- 
> 
下载：[老连的 profile.do](https://gitee.com/arlionn/StataProfile)



> 
Note：对于微信公众号用户，请点击底部 **【阅读原文】** 查看下载链接。

- 在线查看：

```
*--------------------------
* 连玉君的 profile.do 文档
*--------------------------

*-说明：
* 此文件设定了每次启动 Stata 时需要做的一些基本设定
* 你可以在此文件中添加你希望在stata启动时立刻执行的命令

*-不要自动更新
  set update_query  off

*-基本参数设定								
  set type double           // 设定 generate 命令产生的新变量为双精度类型
  set matsize 800           // 设定矩阵的维度为 2000x2000
  set scrollbufsize 2000000 // 结果窗口中显示的行数上限
  set more off, perma       // 关闭分页提示符
  
  set cformat  %4.3f  //回归结果中系数的显示格式
  set pformat  %4.3f  //回归结果中 p 值的显示格式      
  set sformat  %4.2f  //回归结果中 se值的显示格式     
  
  set showbaselevels off, permanently
  set showemptycells off, permanently
  set showomitted off, permanently
  set fvlabel on, permanently


*-PLUS 和 PERSONAL 文件夹  
*-有关这一部分的完整设定命令，请输入 help set 命令进行查看
  sysdir set PLUS "`c(sysdir_stata)'ado\plus"    // 外部命令的存放位置
  sysdir set PERSONAL "`c(sysdir_stata)'ado\personal"  // 个人文件夹位置

*-采用相似的方式，可添加其它允许stata搜索的目录
  adopath + "`c(sysdir_stata)'\ado\personal\_myado"
 *adopath + "路径2"


* log文件：自动以当前日期为名存放于 stata15\do 文件夹下
* 若 stata 安装目录下无【do】文件夹，则自动建立一个,用于存放日志文件
  cap cd `c(sysdir_stata)'do
  if _rc{
     mkdir `c(sysdir_stata)'do  //检测后发现无 do 文件夹，则自行建立一个
  }

*-启动时自动创建日志文件
  local fn = subinstr("`c(current_time)'",":","-",2)
  local fn1 = subinstr("`c(current_date)'"," ","",3)
  log    using `c(sysdir_stata)'do\log-`fn1'-`fn'.log, text replace
  cmdlog using `c(sysdir_stata)'do\cmd-`fn1'-`fn'.log, replace

 
*-----------------------------------------------
*-以下是我的个性设定，可以忽略，也可以自行修改
*-----------------------------------------------

  
dis in w _n(5) ///
           "    ------------------------------------------------------"
dis in w   "    -------------- Stata 15 dofile 转码方法 --------------" 
dis in w   "    ------------------------------------------------------" _n
  
  dis in w _n(1) ///
           "    用 Stata15 打开 Stata14 以下的 dofile 时，屏幕会提示 " _n
  dis in w "    ....... The document is not encoded in UTF-8! ......." _n
  dis in w "    处理方法：在 Encoding: 下拉菜单中选择 「Chinese(GBK)」，点击 OK " _n
  dis in w "    注意：不要勾选「[ ] Dot not show this message again」" _n _n
  
dis in w _n _n ///
         "    ------------------------------------------------------"
dis in w "    ----- Stata 15 转码方法(一次性处理 .dta 转码问题) ----" 
dis in w "    ------------------------------------------------------" _n
dis in w "    *-说明: dofile 或 数据文件中包含中文字符时，需要转码才能正常显示"
dis in w "                      "
dis in w "    *-Step 1: 分析当前工作路径下的编码情况(可省略)                "
dis in w "      ua: unicode analyze *                                         "    
dis in w "    *-Step 2: 设定转码类型                                          "   
dis in w "      ua: unicode encoding set gb18030  // 中文编码                 "                       
dis in w "    *-Step 3: 转换文件                                              "
dis in w "      ua: unicode translate *                                       "    

*---------
*-常逛网址
*---------
  dis in w _n "   "
  
  dis _n in w _col(10) _dup(45) "="
  dis    in w _col(10) _n _skip(20) "Hello World! Hello Stata!" _n
  dis    in w _col(10) _dup(45) "=" _n 
  
  dis in w  "Stata官网：" ///
      `"{browse "http://www.stata.com": [Stata.com] }"' ///
      `"{browse "http://www.stata.com/support/faqs/":   [Stata-FAQ] }"' ///
      `"{browse "https://blog.stata.com/":      [Stata-Blogs] }"' ///
      `"{browse "http://www.stata.com/links/resources.html":   [Stata-资源链接] }"' _n
	  
  dis in w  "Stata论坛：" ///
	  `"{browse "http://www.statalist.com": [Stata-list] }"'      ///
      `"{browse "https://stackoverflow.com":  [Stack-Overflow] }"' ///
      `"{browse "http://bbs.pinggu.org/": [经管之家-人大论坛] }"'  //_n
  
  dis in w  "Stata资源：" /// 
      `"{browse "http://www.jianshu.com/u/69a30474ef33": [Stata连享会-简书] }"' ///
      `"{browse "https://www.zhihu.com/people/arlionn/":    [Stata连享会-知乎] }"'  ///
	  `"{browse "https://gitee.com/arlionn":    [Stata连享会-码云] }"'
	  
  dis in w  _col(12)  /// 
      `"{browse "http://www.jianshu.com/p/f1c4b8762709": [Stata书单] }"' ///
	  `"{browse "http://www.jianshu.com/p/c723bb0dbf98":           [Stata资源汇总] }"' //_n
	  
  dis in w  "Stata课程：" ///
      `"{browse "https://stats.idre.ucla.edu/stata/": [UCLA在线课程] }"' ///
      `"{browse "http://www.princeton.edu/~otorres/Stata/":        [Princeton在线课程] }"'  _n
	  
  dis in w  "Stata现场：" ///
	  `"{browse "http://www.peixun.net/view/307.html": [Stata初级班] }"'  ///
	  `"{browse "http://www.peixun.net/view/308.html":       [Stata高级班] }"' ///
	  `"{browse "http://i.youku.com/arlion":       [Stata优酷视频] }"' 
	  
  dis in w  "学术论文：" ///
	  `"{browse "http://scholar.chongbuluo.com/":  [学术搜索] }"'  ///
	  `"{browse "http://scholar.cnki.net/":       [CNKI] }"' ///
	  `"{browse "http://xueshu.baidu.com/":       [百度学术] }"'  ///
	  `"{browse "http://www.jianshu.com/p/494e6feab565":         [Super Link] }"' _n  
  
  
*----------------- 
*-快速进入相应目录
*----------------- 
* 定义一些小程序，在命令窗口中输入几个字母便可打开指定文件

  dis in w _n(2) "   "  
  dis _n _n _n in w "myQuick:  sj | wx | mylec | myado | mytech | mysof | sougou | uniall"
  
  local p "sj"
  cap program drop `p'
  program define `p'
    local pwd : pwd
    qui cd "E:\BaiduYun\百度云同步盘\计量\STATA文档\Stata_Journal"
    qui cdout 
	qui cd "`pwd'"
  end
  
  local p "mysof"
  cap program drop `p'
  program define `p'
    qui cd "E:\BaiduYun\百度云同步盘\D\Software"
    qui cdout 
  end
  
  local p "mylec"
  cap program drop `p'
  program define `p'
    qui cd "E:\BaiduYun\百度云同步盘\计量\[]Finished"
    qui cdout 
  end

  local p "myado"
  cap program drop `p'
  program define `p'
    qui cd "`c(sysdir_stata)'\ado\personal\_myado"
    qui cdout 
  end

  local p "mytech"
  cap program drop `p'
  program define `p'
    cd "E:\BaiduYun\百度云同步盘\授课"
    cdout 
  end

  local p "wx"
  cap program drop `p'
  program define `p'
    cd "E:\BaiduYun\百度云同步盘\F\微博-微信\"
    cdout 
  end

  *-------------打开搜狗短语定义---------------
  *-文件地址查询: 
  *-方法1: 可以依次点击 [开始]->[搜索]，然后输入 [Phrases.ini]
  * 进而从地址栏中复制地址
  *-方法2: 用 everything 软件查询，然后右击文件名，选择[复制完整路径和文件名(F)]
  local p "sougou" 
  cap program drop `p'
  program define `p'
	shellout C:\Users\Administrator\AppData\LocalLow\SogouPY.users\00000002\PhraseEdit.txt
  end


*------------------------------------------------------
*----- Stata 15 转码方法(一次性处理 .dta 转码问题) ----
*------------------------------------------------------
*-一次性转换当前工作路径下的所有文件
* 使用方法：在命令窗口中输入 uniall 命令即可。
cap program drop uniall
program define uniall
    *-说明: dofile 或 数据文件中包含中文字符时，需要转码才能正常显示
    *-Step 1: 分析当前工作路径下的编码情况                         
      *unicode analyze *                                         
    *-Step 2: 设定转码类型                                          
      ua: unicode encoding set gb18030  // 中文编码                     
    *-Step 3: 转换文件                                              
      ua: unicode translate *
end
```

> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [CSDN-Stata连享会](https://blog.csdn.net/arlionn) 、[简书-Stata连享会](http://www.jianshu.com/u/69a30474ef33) 和 [知乎-连玉君Stata专栏](https://www.zhihu.com/people/arlionn)。可以在上述网站中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
#### 往期精彩推文

- [Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)
- [Stata连享会推文列表2](https://github.com/arlionn/Stata_Blogs/blob/master/README.md)
- Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-e29cff7572db2d83.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)








