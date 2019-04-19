# 对GridView模板列内控件的操作代码 - 逍遥剑客 - CSDN博客
2006年12月01日 00:55:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1801标签：[button																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)
个人分类：[乱七八糟](https://blog.csdn.net/xoyojank/article/category/259560)
情况：假设一个已绑定数据的GridView，在前面加一个模板列，放入一个CheckBox。然后用一个按钮获取CheckBox选中行的一些信息获对该行进行一些其他操作。
对按钮Button1编写Click事件如下：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)protectedvoid Button1_Click(object sender, EventArgs e)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)for (int i =0; i < GridView1.Rows.Count; i++)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        CheckBox cb = (CheckBox)GridView1.Rows[i].FindControl("CheckBox1");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//让没选中的行隐藏
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//if (cb.Checked != true)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//GridView1.Rows[i].Visible = false;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//得到选中行的信息
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (cb.Checked ==true) 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//获取该行某列的内容
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            Response.Write(Convert.ToString(GridView1.Rows[i].Cells[2].Text) +"<br/>");        
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
其他控件或其他操作也应该同样通过这种方法去实现。  
