
# Winform鼠标滑轮控制自定义滚动条 - 致林 - 博客园






# [Winform鼠标滑轮控制自定义滚动条](https://www.cnblogs.com/bincoding/p/7798365.html)
场景：类似QQ聊天的窗体中，需要添加自定义滚动条vScroll。主窗体中panel存放空间，右边有垂直的滚动条vScroll。
问题：已经实现vScroll和Panel.VerticalScroll滚动条联动，鼠标可拖动，但是鼠标滑轮事件不能触发。
原因：Panel不能聚焦，鼠标点击后不能触发wheel事件。
解决方法：点击Panel后vScroll获得焦点，然后自定义鼠标Wheel事件。
代码：
//鼠标滑轮事件protectedoverridevoidOnMouseWheel(MouseEventArgs e)
{base.OnMouseWheel(e);if(e.Delta <0)
     {
          vScrollBar.Value+=vScrollBar.LargeChange;          
     }else{
          vScrollBar.Value-=vScrollBar.LargeChange;   
     }
     mPanel.VerticalScroll.value=vScrollbar.Value;
}//点击panel后vscroll获得焦点privatevoidPanel_Click(objectsender, EventArgs e)
{
     vScrollBar.focus();  
}





