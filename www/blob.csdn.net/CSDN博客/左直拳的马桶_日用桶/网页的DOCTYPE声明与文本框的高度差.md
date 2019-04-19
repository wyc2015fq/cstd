# 网页的DOCTYPE声明与文本框的高度差 - 左直拳的马桶_日用桶 - CSDN博客
2010年05月08日 17:54:00[左直拳](https://me.csdn.net/leftfist)阅读数：2029标签：[xhtml																[脚本																[html																[button																[div](https://so.csdn.net/so/search/s.do?q=div&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=xhtml&t=blog)
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
网页开头声明DOCTYPE为<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">的情况下，文本框的实际高度会比设定值大一些，大约有6px的差距。
比如说，一个文本框和一个按钮放一起，排一行，如果要大家看上去高度一致，按钮height=34px,那么文本框height=28px。
![](http://hi.csdn.net/attachment/201005/8/1037695_1273312347kPK0.jpg)
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head><title>高度不一致</title></head>
<body>
<div style="height:34px;width:300px;border:solid 1px red;">
<input type="text" value="28px" style="height:28px;width:200px;font-size:16px;line-height:28px;float:left;" /><input type="button" value="34px" style="height:34px;width:65px;float:left;" />
</div>
</body>
</html>
为什么会这样？不知道。
如果将这个DOCTYPE声明去掉，高度立刻恢复正常：
![](http://hi.csdn.net/attachment/201005/8/1037695_1273312347ggGG.jpg)
有一个问题。如果页面代码用这种方式输出脚本：Response.Write(“<script>alert(‘Hello   World!’);</script>”);，DOCTYPE声明也会立即失效，如果文本框设计的时候因为迁就这个高度差，这时候会立即露出原形。究其原因，这种方式输出脚本，脚本会位于DOCTYPE声明的前面，而DOCTYPE声明是要排在最前面的。
<mce:script type="text/javascript"><!--
    alert('Hello World!');
// --></mce:script>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head><title>高度不一致</title></head>
<body>
<div style="height:34px;width:300px;border:solid 1px red;">
<input type="text" value="28px" style="height:28px;width:200px;font-size:16px;line-height:28px;float:left;" /><input type="button" value="34px" style="height:34px;width:65px;float:left;" />
</div>
</body>
</html>
对付这种情况，最好用Page.ClientScript.RegisterStartupScript来输出脚本。
我想，<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">这种声明里面，文本框的高度差应该有个什么属性来消除的。但如何消除，我还不知道。
有时候，Reponse输出脚本后，本来用margin:auto方式定位居中的DIV一下子跑到左边，原因也在此。
