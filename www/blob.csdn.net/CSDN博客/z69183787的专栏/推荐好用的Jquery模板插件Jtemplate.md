# 推荐好用的Jquery模板插件Jtemplate - z69183787的专栏 - CSDN博客
2014年05月14日 16:15:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1837
jtemplate是javascript的模板引擎。官方网址：http://jtemplates.tpython.com/数据准备：
var data ={ 
TotalCount:64, 
Lists:[ 
{Id:'2001' ,Title:'新闻11',CreateDate:'2011-08-08'}, 
{Id:'2002' ,Title:'新闻22',CreateDate:'2011-08-08'}, 
{Id:'2003' ,Title:'新闻33',CreateDate:'2011-08-08'}, 
{Id:'2004' ,Title:'新闻44',CreateDate:'2011-08-08'}, 
{Id:'2005' ,Title:'新闻55',CreateDate:'2011-08-08'}, 
] 
} 
1、引入库文件
<script type="text/javascript" src="jquery.js"></script> 
<script type="text/javascript" src="jquery-jtemplates.js"></script> 
2、编写模板
<div id="<span style="color:#FF0000;">result</span>"></div> 
<div id="templateContainer" style="display:none;"> 
<table> 
<tr><td>Id</td><td>标题</td><td>发布时间</td></tr> 
{#foreach $T.table as row} 
<tr><td>{$T.row.Id}</td><td>{$T.row.Title}</td><td>{$T.row.CreateDate}</td></tr> 
{#/for} 
</table> 
</div> 
语法：
1、大括号{..} ，在这里面可以写任何javascript的代码，比如 {$T.toUpperCase()}
2、{$T} : 表示数据，例如上面的例子，$T.table表示得到data的table对象，$T.TotalCount 为 64。
3、{#foreach} : 循环获取数据，如上面：{#foreach $T.table as row}      {$T.row.Title}      {/for}  
扩展语法：
{#if}
例子：
{#if $T=="true"} good {#else} fail {#/if} 
{#if $T.list_id == 3} System list {#elseif $T.list_id == 4} Users List {#elseif $T.list_id == 5} Errors list {#/if} 
{#foreach}
{#foreach |VAR| as |NAME| [begin=|CODE|] [count=|CODE|] [step=|CODE|]}..{#else}..{#/for} 
例子：
a、输出所有数据：
{#foreach $T.table as row}      {$T.row.Title}      {/for}    
b、从第二条记录开始输出：
{#foreach $T.table as row begin=1}      {$T.row.Title}      {/for}    
c、从第二条开始且只取2条
{#foreach $T.table as row begin=1 count=2}      {$T.row.Title}      {/for}    
d、使用step
{#foreach $T.table as row step=2}      {$T.row.Title}      {/for}  
e、使用else
{#foreach $T.table as row step=2}      {$T.row.Title}  {#else}   无记录   {/for}  
{#for}
例子：
{#for index = 1 to 10} {$T.index} {#/for} 
{#for index = 1 to 10 step=3} {$T.index} {#/for} 
3、渲染模板并展示
<script type="text/javascript">  
        $(document).ready(function() { 
            // 设置模板 
            $("#result").setTemplateElement("templateContainer"); 
            // 处理模板 
            $("#result").processTemplate(data); 
        });  
    </script>  
设置模板的几种方法：
a. setTemplateElement:参数为页面中的一个元素ID
如上面的例子
b. setTemplate: 参数为具体的模板内容，
如：$("#result").setTemplate("Template by {$T.bold()} version <em>{$Q.version}</em>.");
c.setTemplateURL:使用外部独立模板文件Url作为参数
如：$("#result").setTemplateURL("example_multitemplate1.tpl");
4、运行结果：
![\](http://up.2cto.com/2012/0217/20120217103634612.gif)
完整代码
<[html](http://www.2cto.com/kf/qianduan/css/)>  
<head>  
    <script type="text/javascript" src="jquery.js"></script> 
    <script type="text/javascript" src="jquery-jtemplates.js"></script> 
    <title>jTemplates</title> 
    <script type="text/javascript">  
        var data ={ 
                TotalCount:64, 
                Lists:[ 
                    {Id:'2001' ,Title:'新闻11',CreateDate:'2011-08-08'}, 
                    {Id:'2002' ,Title:'新闻22',CreateDate:'2011-08-08'}, 
                    {Id:'2003' ,Title:'新闻33',CreateDate:'2011-08-08'}, 
                    {Id:'2004' ,Title:'新闻44',CreateDate:'2011-08-08'}, 
                    {Id:'2005' ,Title:'新闻55',CreateDate:'2011-08-08'}, 
                ] 
        }; 
        $(document).ready(function() { 
            // 设置模板 
            $("#result").setTemplateElement("templateContainer"); 
            // 处理模板 
            $("#result").processTemplate(data); 
        });  
    </script> 
</head> 
<body> 
    <div id="result"> 
    </div> 
    <textarea id="templateContainer" style="display: none;"> 
        <table border="1"> 
            <tr> 
                <td> 
                    Id 
                </td> 
                <td> 
                    标题 
                </td> 
                <td> 
                    发布时间  www.2cto.com
                </td> 
            </tr> 
            {#foreach $T.Lists as row} 
            <tr> 
                <td> 
                    {$T.row.Id} 
                </td> 
                <td> 
                    {$T.row.Title} 
                </td> 
                <td> 
                    {$T.row.CreateDate} 
                </td> 
            </tr> 
            {#/for} 
        </table> 
    </textarea> 
</body> 
</html> 
