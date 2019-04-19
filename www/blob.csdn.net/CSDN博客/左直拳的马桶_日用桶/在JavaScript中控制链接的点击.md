# 在JavaScript中控制链接的点击 - 左直拳的马桶_日用桶 - CSDN博客
2006年08月30日 18:19:00[左直拳](https://me.csdn.net/leftfist)阅读数：2449
在JavaScript中控制链接的点击
左直拳 
在JavaScript里，可以控制链接<a>的点击，方法就是：
objA.click()，这里objA为一个链接<a>对象。
示例：
做网站的时候，为了好维护和简洁，每个页面都用<IFRAME>连接了一个页眉文件：header.html和页脚文件：footer.html。
众所周知，在<IFRAME>里点击链接，如果该链接的Target不为“_top”或“_parent”或“_blank”，则新指向的页面会只在IFRAME里打开。
现在，在页眉页面header.html有一个文本框txtNewAddress，一张图片按扭imgButton，要求点击此图片，将文本框txtNewAddress里的内容带到新的页面里，而新的页面要替换目前整个页面，也就是说，不能只在<IFRAME>里显示。
读取文本框内容，当然要用JavaScript了。很自然就想到：
<a href=”javascript:ReadTextBox()” target=”top”><img src=”ok.gif” id=”imgButton”></a>
问题是这样子会报错。因为点击该链接后，在顶层打开了新页面，而新页面却没有函数ReadTextBox()。
解决办法是写成：
<a href=”javascript:ReadTextBox()”><img src=”ok.gif” id=”imgButton”></a>
再添加一个链接：
<aid="aNewPage"target="_top"></a>
思路是在函数ReadTextBox()里读取文本框的内容后，修改链接aNewPage的href，然后触发其click。
function UpToTaxi()
{
var url = "";
txt = document.getElementById("txtNewAddress");
if( txt.value != "" )
{
url += "?newurl=" + txt.value;
}
obj = document.getElementById("aNewPage");
obj.href = "newpage.aspx" + url;
obj.click();
}
