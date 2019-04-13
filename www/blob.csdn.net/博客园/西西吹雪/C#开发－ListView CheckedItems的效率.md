
# C\#开发－ListView.CheckedItems的效率 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[C\#开发－ListView.CheckedItems的效率](https://www.cnblogs.com/watsonyin/archive/2006/11/02/548197.html)
看一段代码：

StringCollection arr=newStringCollection();
for(inti=0;i<=lvwExtents.CheckedItems-1;i++)
{
arr.Add(lvwExtents.CheckedItems[i].Text);
}
看上去没什么问题。可是今天我运行时，发现速度缓慢得像蜗牛一般。我的ListView有两千多条记录，全部处于选中状态。速度无法忍受，估计秒种处理的记录只有两三条。
可是换成了以下代码后，就完全正常了，飞快的就执行完了。
ListViewItem item;
for(inti=0;i<=this.lvwExtents.Items.Count-1;i++)
{
item=this.lvwExtents.Items[i];
if(item.Checked)
arr.Add(item.Text);
}
奇怪吧。也许是因为每次循环时调用的lvwExtents.CheckedItems[i].Text代码，都会重新检测哪些项处于选中的状态的缘故吧。




posted on2006-11-02 18:00[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=548197)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
