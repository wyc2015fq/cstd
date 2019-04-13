
# DevExpress xtraTabbedMdiManager控件双击关闭MDI标签页 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[DevExpress xtraTabbedMdiManager控件双击关闭MDI标签页](https://www.cnblogs.com/watsonyin/archive/2007/03/14/673989.html)
DevExpress .net组件包中，有一个组件是xtraTabbedMdiManager，可以将MDI窗口显示为TabControl的样式，可以说非常实用。可惜的是，关闭标签页（即子MdiChild)不能通过双击来实现，这对于用惯了傲游等软件的朋友来说，有点不习惯。查看xtraTabbedMdiManager的事件，只有MouseDown,MouseUp等，并没有DoubleClick，好不爽。
其实，转换一下思路，我们可以将连续的两次MouseDown事件模拟成一个双击事件，就可以解决上面的问题了，真是一点就破，技术上没有什么难度，且看代码吧：
//通过MouseDown事件模拟双击关闭标签页
privateDateTime m_LastClick=System.DateTime.Now;
privatevoidxtraTabbedMdiManager1_MouseDown(objectsender, MouseEventArgs e)
{
if(e.Button==MouseButtons.Left)
{
DateTime dt=DateTime.Now;
TimeSpan span=dt.Subtract(m_LastClick);
if(span.TotalMilliseconds<300)//如果两次点击的时间间隔小于300毫秒，则认为是双击
{
if(this.MdiChildren.Length>1)
{
if(this.ActiveMdiChild!=m_MapForm)
{
this.ActiveMdiChild.Close();
}
}
m_LastClick=dt.AddMinutes(-1);
}
else
m_LastClick=dt;
}
elseif(e.Button==MouseButtons.Right)
{
//弹出右键菜单
if(this.ActiveMdiChild!=m_MapForm)
{
POINTAPI pt=newPOINTAPI();
GetCursorPos(refpt);
System.Drawing.Point p=newSystem.Drawing.Point(pt.x, pt.y);
popMenuCloseTab.ShowPopup(this.barManager1, p);
}
}
}
privatevoidmnuCloseTab_ItemClick(objectsender, DevExpress.XtraBars.ItemClickEventArgs e)
{
if(this.ActiveMdiChild!=m_MapForm)
{
this.ActiveMdiChild.Close();
}
}
在这里，我们认为如果两次点击时间间隔小于300毫秒，就认为是双击。注意，m_LastClick=dt.AddMinutes(-1)，这一行代码的作用是，表示双击完成，避免点击三次认为是双击了两次。
上面的代码还提供了点击右链弹出菜单功能。可以在右键菜单中加入你要的功能。




posted on2007-03-14 09:41[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=673989)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
