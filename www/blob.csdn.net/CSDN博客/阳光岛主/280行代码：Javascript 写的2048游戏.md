
# 280行代码：Javascript 写的2048游戏 - 阳光岛主 - CSDN博客

2014年05月31日 08:36:56[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：11238个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)



2048 原作者就是用Js写的，一直想尝试，但久久未动手。
昨天教学生学习JS代码。不妨就做个有趣的游戏好了。2048这么火，是一个不错的选择。

思路：
1. 数组 ，2维数组4x4
2. 移动算法，移动后有数字的对齐，无数字（我用的0，但不显示）补齐。

移动前

![](https://img-blog.csdn.net/20140512113410937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYXNtY29z/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

移动后（注意程序合并了第一行2个2，并产生了新的2）
![](https://img-blog.csdn.net/20140512113448984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYXNtY29z/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

移动算法分2步：
第一步骤：移动
第二步骤：合并

移动代码参考：
**[html]**[view plain](http://blog.csdn.net/asmcos/article/details/25605207#)[copy](http://blog.csdn.net/asmcos/article/details/25605207#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

function left(t,i)
{
var j;
varlen=t[i].length;
for (j=0;j<len-1;j++)
{
if (t[i][j] == 0 && t[i][j+1] != 0)
{
temp=t[i][j];
t[i][j] = t[i][j+1];
t[i][j+1] = temp;
left(t,i);
}
}
}


合并代码参考：
**[html]**[view plain](http://blog.csdn.net/asmcos/article/details/25605207#)[copy](http://blog.csdn.net/asmcos/article/details/25605207#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

function lcombine(a,i)
{
varlen=a[i].length;
for(varj=0;j<len-2;j++)
{
if (a[i][j] == a[i][j+1])
{
a[i][j] *=2;
a[i][j+1] = 0;
left(a,i);
break;
}
}
}

3.显示

显示部分CSS来源 2048源作者程序。
显示代码：

**[html]**[view plain](http://blog.csdn.net/asmcos/article/details/25605207#)[copy](http://blog.csdn.net/asmcos/article/details/25605207#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

function display_div ()
{
var i,j;
varn="\#d";
for (i=0;i<4;i++)
{
for(j=0;j<4;j++)
{
if (a[i][j] !=0)
$(n+(i*4+j)).html("<divclass='tile tile-"+a[i][j]+"'><divclass='tile-inner'>"+a[i][j]+"</div></div>");
else
$(n+(i*4+j)).html("");

}
}
}

这段代码是把数组内容显示到  4x4表格内。
源代码及在线演示：[http://jsbin.com/biximuho/6/edit](http://jsbin.com/biximuho/6/edit)


