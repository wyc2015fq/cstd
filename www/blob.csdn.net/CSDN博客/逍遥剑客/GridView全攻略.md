# GridView全攻略 - 逍遥剑客 - CSDN博客
2006年12月01日 00:57:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1487标签：[null																[datagrid																[postback																[object																[textbox																[javascript](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=textbox&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=postback&t=blog)](https://so.csdn.net/so/search/s.do?q=datagrid&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[乱七八糟](https://blog.csdn.net/xoyojank/article/category/259560)
作者: zyf0808 发表日期: 2006-09-21 21:47 
    小文将通过实例对 2.0 下的数据控件GridView进行一个完整的应用描述，涉及基本数据绑定，高级数据绑定，自定义模板设计，分页状态保持等等。本文根据项目实例，代码已经经过详细测试，蕴涵了作者的辛苦劳动，欢迎转载，请注明出处。
1、基本绑定，方式和DataGrid没什么区别。简单附代码如下：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif) String strSQL ="SELECT * FROM ....";
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        WorksData workData =new WorkSystem().LoadWork(strSQL);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        gvWorks.DataSource = workData;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        gvWorks.DataBind();
2、通过模板列来增加“编辑 删除”命令的连接，并在单击“删除”时给出确认提示，这是数据操作中经常使用的方式。将列转化为模板列后，通过右键单击GridView控件，选择“编辑模板”，然后选择相应的命令列，去掉默认的TextBox，然后添加两个LinkButton，Text属性分别为："编辑"和"删除"，ID分别为lbEdit和lbDelete。模板列
a.为删除按钮添加确认对话框：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foreach (GridViewRow gvr in gvWorks.Rows)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            LinkButton lbDelete= (LinkButton)gvr.FindControl("lbDelete");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (lbDelete!=null)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)               lbDelete.Attributes.Add("onclick", "javascript:return confirm('是否确定要删除？');");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)        }
b.为两个命令按钮绑定参数：(以下代码是通过右键在lbEdit的可绑定参数中将CommandArgument的值绑定为Container.DataItemIndex的方式，需要注意的是，如果分页，需要做一个处理，即代码中- gvWorks.PageSize * gvWorks.PageIndex，以得到处于该页的正确的索引，然后通过DataKeys[commandArg].Value来得到ID值；当然，也可以直接将该属性绑定为你的ID，这样，更易于理解和操作，也不必处理分页的索引问题)编辑模板
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//编辑
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)protectedvoid lbEdit_Click(object sender, EventArgs e)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         LinkButton lb = (LinkButton)sender;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int commandArg =int.Parse(lb.CommandArgument) - gvWorks.PageSize * gvWorks.PageIndex;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int id = (int)this.gvWorks.DataKeys[commandArg].Value;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         Response.Redirect("EditWork.aspx?workid="+ id);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)     }
同理，删除按钮也可以采用两种方式的处理。
3、如何保持分页的状态：如果你需要在一个GridView中加一个CheckBox，以选择某行数据，然后进入下一页继续选择，而返回上一页时仍旧能够显示你已经选择了的数据行，那么，这里将提供一种方式给你。因为分页时，页面发生变化时，是要postback回服务器的，这样，虽然你做了选择，但是未更新到服务器，所以，不做处理的话是保存不住已经选择的项的。
    思路：通过一个ArrayList来保持你已经选择的项的ID，当页索引变化时，将触发DataBinding事件，在该事件内调用CollectSelected()方法，来保存所有CheckBox选中的行，去掉那些未选中的行ID。在RowDataBound()，通过SelectedItems中保存的ID来设置是否选中，以回显你的操作。需要注意的是，在你提交的时候，一定要再调用一次CollectSelected()，以最后一次收集当前页的选中情况。
以下代码参考了部分网络资源，特此表示感谢。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)protected ArrayList SelectedItems
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)get
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return (ViewState["mySelectedItems"] !=null) ? (ArrayList)ViewState["mySelectedItems"] : null;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)set
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            ViewState["mySelectedItems"] = value;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
/**////<summary>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)/// 从当前页收集选中项的情况
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)///</summary>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)protectedvoid CollectSelected()
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        ArrayList selectedItems =null;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (this.SelectedItems ==null)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            selectedItems =new ArrayList();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)else
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            selectedItems =this.SelectedItems;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)for (int i =0; i <this.gvWorks.Rows.Count; i++)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            CheckBox cb = (CheckBox)gvWorks.Rows.FindControl("cbChoose");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)string id = ((Label)gvWorks.Rows.FindControl("lbID")).Text;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (selectedItems.Contains(id) &&!cb.Checked)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                selectedItems.Remove(id);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (!selectedItems.Contains(id) && cb.Checked)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)               selectedItems.Add(id);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)       }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)       SelectedItems = selectedItems;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)protectedvoid gvWorks_DataBinding(object sender, EventArgs e)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//在每一次重新绑定之前，需要调用CollectSelected方法从当前页收集选中项的情况
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)       CollectSelected();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)protectedvoid gvWorks_RowDataBound(object sender, GridViewRowEventArgs e)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//这里的处理是为了回显之前选中的情况
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (e.Row.RowIndex >-1&& SelectedItems!=null)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            DataRowView row = e.Row.DataItem as DataRowView;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            CheckBox cb = (CheckBox)e.Row.FindControl("cbChoose");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            Label lbID = (Label)e.Row.FindControl("lbID");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if(SelectedItems.Contains(lbID.Text))
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                cb.Checked =true;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)else
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                cb.Checked =false;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

