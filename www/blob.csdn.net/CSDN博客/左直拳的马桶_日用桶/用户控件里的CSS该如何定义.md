# 用户控件里的CSS该如何定义 - 左直拳的马桶_日用桶 - CSDN博客
2010年03月24日 18:16:00[左直拳](https://me.csdn.net/leftfist)阅读数：2409标签：[css																[scroll																[object																[ajax																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=ajax&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=scroll&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
用户控件如果要集中定义类似
<style type="text/css">
    .C {border-right:1px #D9AE7F solid;border-bottom:1px #D9AE7F solid;cursor:pointer;font-family:arial;font-weight:bold;height:23px;background:#FFF7DD none repeat scroll 0 0; line-height:20px;}
    #ctem td {border-bottom:1px solid #F0F0F0;color:black;height:25px;}
    #ctem a:link,#cctvitem a:active,#cctvitem a:visited {color:black;text-decoration:none;}
    #ctem a:hover {color:red;text-decoration:underline;}
    .TMSpan {cursor:pointer;display:inline;width:70px;margin:3px 2px 0px 2px;height:16px;border-left:1px solid #D9AE7F;border-right:1px solid #D9AE7F;border-top:1px solid #D9AE7F;text-align:center;font-weight:bold;}
</style>
这种CSS怎么办？
如果是写在控件中，不符合规范，虽然一般情况下也起作用，但如果是动态加载，如AJAX加载这个用户控件，就不行了。
如果由调用页预先写在页头，当然没问题，但控件的意义就打了折扣。
可以使用如下方法：
控件代码：
    const string STYLE = @"<style type=""text/css"">
    .C {border-right:1px #D9AE7F solid;border-bottom:1px #D9AE7F 
solid;cursor:pointer;font-family:arial;font-weight:bold;height:23px;background:#FFF7DD
 none repeat scroll 0 0; line-height:20px;}
    #ctem td {border-bottom:1px solid #F0F0F0;color:black;height:25px;}
    #ctem a:link,#cctvitem a:active,#cctvitem a:visited 
{color:black;text-decoration:none;}
    #ctem a:hover {color:red;text-decoration:underline;}
    .TMSpan {cursor:pointer;display:inline;width:70px;margin:3px 2px 0px
 2px;height:16px;border-left:1px solid #D9AE7F;border-right:1px solid 
#D9AE7F;border-top:1px solid 
#D9AE7F;text-align:center;font-weight:bold;}
</style>
";
protected void Page_Load(object sender, EventArgs e)
    {
//控件自有CSS
this.Page.Header.Controls.Add(new LiteralControl(STYLE));
    }
这样控件的CSS就会出现在调用页的<head/>里了。
