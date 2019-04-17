# Python自定义HtmlTestRunner测试报告 - zhusongziye的博客 - CSDN博客





2018年04月01日 18:44:43[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1054








## 简述

         使用selenium webdriver + Python做自动化测试，执行完成后要生成测试报告，Python我们使用的HTMLtestrunner 进行生成，但是默认提供的生成报告内容，并不能满足我们的需求，怎么才能让测试报告数据更清晰，内容更丰富呢。对于一些人来说，可能已经重写了报告生成方式，放弃使用HTMLtestrunner。但是对于python小白可能还做不到，只能用现有东西进行展示，那么下面简单介绍通过修改HTMLtestrunner去实现想要的内容。



先通过两张图片对比一下我们将要实现的功能：

![](https://img-blog.csdn.net/20180401184331779?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图表 1

![](https://img-blog.csdn.net/20180401184344357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图表 2

         如图所示，图表1是原始样式输出，图表2是修改后样式输出。通过对比我们可以看到，图表2比图表1多了首位部的两列。我在第一行加caseid，用来记录用例的编号，最后一列，添加了每一个用例执行过程中关键步骤的截图快照。

这样做的好处：
- 
有了用例编号我们可以通过用例ID进行对用例的定位，尤其是对失败用例的定位。

- 
有了用例执行过程中关键位置截图快照，对于失败或者出错异常的用例，在无人值守情况下进行执行用例时，可以进行界面追踪。


## 实现

下面来介绍怎么实现的，首先先说一下我实现的思路。对于这个我们的测试报告而言，我只想调整一下他的展示样式，丰富两列内容。所以先去想怎么去把他的样式给修改了。然后再往修改后的样式表格里面添加数据。

也就是分两步做：

         1改样式

         2加数据

### 改样式

改样式，这里只需要会基础的HTML <table>中行、列标识就可以了。

我们要在界面中加两列，那么很简单，就是加两个<td></td>就可以了。

看源码部分：

从395行左右开始就是存储HTML代码展示的部分，找到table标签和第一行列头展示内容。

#### 脚本片段1:

REPORT_TMPL = “””  ……   “”” 代码段里面包含了<table>的结构内容

如：
<tableid="result_table"><colgroup><colalign="left"/><colalign="right"/><colalign="right"/><colalign="right"/><colalign="right"/><colalign="right"/></colgroup><trid="header_row"><tdcolspan="2">Test Group/Test case</td><td>Count</td><td>Pass</td><td>Fail</td><td>Error</td><td>View</td><td>Screenshot</td></tr>%(test_list)s    <trid="total_row"><tdcolspan="2">Total</td><td>%(count)s</td><td>%(Pass)s</td><td>%(fail)s</td><td>%(error)s</td><td></td><td></td></tr></table>



table标签中的第一个<tr>就是第一行，<tr>中的每一个<td></td>代表一列。标签中间的文本是直接显示在界面的。
<trid='header_row'>
    //这里添加colspan='2' 是合并2个列。
    //因为下面的行中的caseid需要占一列，
    //但是标题列头部分显示是一个整体，所以做了合并 
    <tdcolspan='2'>Test Group/Test case</td><td>Count</td><td>Pass</td><td>Fail</td><td>Error</td><td>View</td><td>Screenshot</td>    //图片快照的列头标题
</tr>
#### 脚本片段2：

REPORT_CLASS_TMPL = “”” … “””代码段 是 展示calss的行，可以叫做测试模块的类，它是单独放在一行中的，每个模块一行。因为要跟上面的第一行标题中的列数对应，所以修改如下：
<trclass='%(style)s'>
    //添加的caseid 的标题，这一行不在主列头，
    //而是放在测试模块用例统计所在的行里面。    <td>case_id</td><td>%(desc)s</td><td>%(count)s</td><td>%(Pass)s</td><td>%(fail)s</td><td>%(error)s</td><td><ahref="javascript:showClassDetail('%(cid)s',%(count)s)">Detail</a></td><td></td>  //保持列数统一，加入空白列
</tr>
#### 脚本片段3：

REPORT_TEST_WITH_OUTPUT_TMPL = “”” … “”” 是测试用例展示部分代码，这段内容是展开测试模块后进行显示的
REPORT_TEST_WITH_OUTPUT_TMPL = r"""<trid='%(tid)s'class='%(Class)s'>
    // %(caseid)s 是python转译html代码中变量替换方式。// caseid 会在python脚本中定义。    <tdalign='center'>%(caseid)s</td><tdclass='%(style)s'><divclass='testcase'>%(desc)s</div></td><tdcolspan='5'align='center'><!--css div popup start--><aclass="popup_link"onfocus='this.blur();'href="javascript:showTestDetail('div_%(tid)s')">
            %(status)s        </a><divid='div_%(tid)s'class="popup_window"><divstyle='text-align: right;color:red;cursor:pointer'><aonfocus='this.blur();'onclick="document.getElementById('div_%(tid)s').style.display ='none'">
                [x]</a></div><pre>%(script)s</pre></div><!--css div popup end--></td><tdalign='center'><ahref="%(image)s"title="%(image)s">  
        //这一列内容加入图片快照的链接访问路径 %(image)s 中替换链接地址    
            <imgsrc="..\data\img.png"height=20width=20border=0/></a>  
            //链接图标    
    </td></tr>


单从HTML界面样式展示来看，上面所有代码段中修改都是在每个<tr>加入两个<td></td>。



这样我们的样式就改完了。然后开始加入数据。

### 加数据

先想一下我们要加什么数据.从样式中看我们增加了两个列，那么有两列的数据是需要增加的，具体加什么呢？想一下？我们在改样式的时候加的两个列中使用到了变量，那么变量取值是需要实现的。那么首先定义变量。



定义报告输出两个变量的取值，放在 REPORT_TEST_OUTPUT_TMPL定义的下面：

#### 脚本片段4：



REPORT_TEST_OUTPUT_TMPL = r"""%(id)s: %(output)s"""
<!--添加内容-->REPORT_TEST_OUTPUT_IMAGE = r"""%(screenshot)s"""REPORT_TEST_OUTPUT_CASEID = r"""%(case_id)s"""



然后找到 _generate_report_test 函数，在这个函数中定义我们要替换html变量，并实现变量的取值。

#### 脚本片段5：     

<!—在原有的script下面分别添加caseid和image变量并赋值，此处不用在意为什么怎么实现的，这么用就对了，script内容展示的其实就是我们case执行时 控制台输出的内容 所以如果你想获取自己在case中print出来的数据就这么做-->


script = self.REPORT_TEST_OUTPUT_TMPL % dict(
    id = tid,
    output = saxutils.escape(uo+ue),
        )

    image = self.REPORT_TEST_OUTPUT_IMAGE % dict(
        screenshot = saxutils.escape(uo+ue))

    caseid = self.REPORT_TEST_OUTPUT_CASEID % dict(
        case_id = saxutils.escape(uo+ue)
        )                   

    <!—html中替换的字符串变量进行详细取值-->

    row = tmpl % dict(
        tid = tid,
        Class = (n == 0 and'hiddenRow'or'none'),
        style = n == 2 and'errorCase'or (n == 1 and'failCase'or'none'),
        desc = desc,
        script = script,                            

    <!—因为获取内容都是字符串，所以只需要进行字符串截取就可以了-->
        image = image[image.find("image"):(int(image.find("png"))+3)],
        caseid = caseid[caseid.find("case"):(int(caseid.find("case"))+9)],
        status = self.STATUS[n],)


上面 image 、caseid 字符串截取位置跟我在控制台输出的内容有关系，所以不是通用的，需要自行修改。字符串截取的方式就不讲了，方式很多种找自己合适的熟悉的就行。



至此修改HTMLtestrunner生成报告模板就修改结束，我的这个方式或许不是你想要的，不是你想展示的，希望能在思路上帮你去解决你的问题。



另：有朋友问能不能内容显示成中文的，看明白了上面内容我想中文的显示就不是什么问题了。直接改HTML标签中显示的英文内容翻译为中文就可以了。



