# 有一动态生成的TABLE,当点击该TABLE中的按钮时,如何得到该按钮所在行的某一列的值啊?... - weixin_33985507的博客 - CSDN博客
2007年07月31日 10:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
前几天遇到此问题：
终于得到解决。
protected   System.Web.UI.HtmlControls.HtmlTable   t_copy;   
   private   void   Page_Load(object   sender,   System.EventArgs   e)   
   {   
   	DataSet   ds_yours   =   new   DataSet();   //自己充值。。。比如说有三列值   
   	for(int   i=0;i<ds_yours.Tables[0].Rows.Count;i++)   
   	{   
   		HtmlTableRow   tr   =   new   HtmlTableRow();   
   		//第一列   
   		HtmlTableCell   tc   =   new   HtmlTableCell();   
   		Literal   lit   =   new   Literal();   
   		lit.Text   =   ds_yours.Tables[0].Rows[i][0].ToString();   
   		lit.ID   =   "YourColumns1_"+i.ToString();   
   		tc.Controls.Add(lit);   
   		tr.Cells.Add(tc);   
   		//第二列   
   		tc   =   new   HtmlTableCell();   
   		lit   =   new   Literal();   
   		lit.Text   =   ds_yours.Tables[0].Rows[i][1].ToString();   
   		lit.ID   =   "YourColumns2_"+i.ToString();   
   		tc.Controls.Add(lit);   
   		tr.Cells.Add(tc);   
   		//第三列   
   		tc   =   new   HtmlTableCell();   
   		lit   =   new   Literal();   
   		lit.Text   =   ds_yours.Tables[0].Rows[i][2].ToString();   
   		lit.ID   =   "YourColumns3_"+i.ToString();   
   		tc.Controls.Add(lit);   
   		tr.Cells.Add(tc);   
   		//按钮列   
   		tc   =   new   HtmlTableCell();   
   		Button   bt   =   new   Button();   
   		bt.ID   =   "bt"+i.ToString();   
   		bt.CommandArgument   =   i.ToString();   
   		bt.Command   +=   new   CommandEventHandler(bt_Command);   
   		tc.Controls.Add(bt);   
   		tr.Cells.Add(tc);   
   		t_copy.Rows.Add(tr);   
   	}   
   }   
   private   void   bt_Command(object   sender,   CommandEventArgs   e)   
   {   
   	string   thisrow   =   e.CommandArgument.ToString();//这一行的行数   
   	string   YourColumns1   =   ((Literal)(t_copy.FindControl("YourColumns1_"+thisrow))).Text;//此行第一列的值   
   	string   YourColumns2   =   ((Literal)(t_copy.FindControl("YourColumns2_"+thisrow))).Text;//此行第一列的值   
   	string   YourColumns3   =   ((Literal)(t_copy.FindControl("YourColumns3_"+thisrow))).Text;//此行第一列的值   
   }   
