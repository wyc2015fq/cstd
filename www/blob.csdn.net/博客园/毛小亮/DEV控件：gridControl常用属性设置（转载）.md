# DEV控件：gridControl常用属性设置（转载） - 毛小亮 - 博客园
# [DEV控件：gridControl常用属性设置（转载）](https://www.cnblogs.com/xianerwonder/p/4437504.html)
特别长，先撸下来再说
1.隐藏最上面的GroupPanel
  gridView1.OptionsView.ShowGroupPanel=false;
2.得到当前选定记录某字段的值
  sValue=Table.Rows[gridView1.FocusedRowHandle][FieldName].ToString();
3.数据只读
  gridView1.OptionsBehavior.Editable=false;
4.不显示MasterDetailView
  gridView1.OptionsDetail.EnableMasterViewMode=false;
5.修改最上面的GroupPanel内容
   gridView1.GroupPanelText="柳生飘絮";
6.设置数据源:
   gridControl1.DataSource = dt;
   绑定每一列的FiledName--属性
   对于Oracle数据库,所有的在设计器里写的程序中的字段名必须大写,否则可能绑定不上字段,sqlserver没有这个限制.
7.读写拷贝权限设置
  只读不可拷贝：
    ColumnViewOptionsBehavior.Editable = False
  只读可拷贝:
    ColumnViewOptionsBehavior.Editable = True
    OptionsColumn.AllowEdit = True
    OptionsColumn.ReadOnly = True
  可编辑:
    ColumnViewOptionsBehavior.Editable = True
    OptionsColumn.AllowEdit = True
    OptionsColumn.ReadOnly = False
8.模板列的设置:
  到Columns中，在他的属性中找到ColumnEdit.
  以LookUpEdit为例：
  首先从Designer左边菜单In-Place Editor Repository中添加LookUpEdit.取名为Re1.然后.在他的Columns属性中添加3列.Caption依次为:编号,姓名,性别.FieldName依次为:FID,FNAME,FSEX.然后将Re1的NullText设置成空.
　　AutoSearchColumnIndex属性设置为2.ImmediatePopup属性设置为True.
　　SearchMode设置为OnlyInPopup.
　　然后将这个模板列附加到我们上面提到的列1(也就是将列1的ColumnEdit属性设成Re1)
　　最后我们还要在代码里面给Re1绑定数据源和显示项.
         Re1.DataSource = DALUse.Query("select fid,fname,fsex from dual").Tables[0];
         Re1.DisplayMember = "FSEX";
         Re1.ValueMember = "FNAME";
9.设某一列文字和标题局中显示                  
   gridView1.Columns[0].AppearanceHeader.TextOptions.HAlignment = DevExpress.Utils.HorzAlignment.Center;
   gridView1.Columns[0].AppearanceCell.TextOptions.HAlignment = DevExpress.Utils.HorzAlignment.Center;
10.去掉某一列上面的自动筛选功能(Filter)                  
   gridView1.Columns[0].OptionsFilter.AllowAutoFilter = false;
   gridView1.Columns[0].OptionsFilter.AllowFilter = false;                
   gridView1.Columns[0].OptionsFilter.ImmediateUpdateAutoFilter = false;
11.设置冻结列(左冻结)
  gridView1.Columns[0].Fixed= DevExpress.XtraGrid.Columns.FixedStyle.Left;
12.得到单元格数据(0行0列)
   string ss=gridView1.GetRowCellDisplayText(0, gridView1.Columns[0]);
   string ss = gridView1.GetRowCellValue(0, gridView1.Columns[0]);
13.设置单元格数据(将0行0列的单元格赋值123)
   gridView1.SetRowCellValue(0, gridView1.Columns[0], "123");
13.手动添加dev的列
  DevExpress.XtraGrid.Columns.GridColumn Col1=new DevExpress.XtraGrid.Columns.GridColumn ();
  Col1.FieldName="FID";
  Col1.Visible=true;
  Col1.VisibleIndex=gridView1.Columns.Count;
  gridView1.Columns.Add(Col1);
14.设置自动增加的行号,需要先添加给gridview添加事件CustomDrawRowIndicator
    private void gridview_CustomDrawRowIndicator(object sender,                            DevExpress.XtraGrid.Views.Grid.RowIndicatorCustomDrawEventArgs e)
    {
        if (e.Info.IsRowIndicator && e.RowHandle >= 0)
                e.Info.DisplayText = (e.RowHandle + 1).ToString();
    }
15.删除: (修改了dgvdel里的datagridviewdel方法)
    public static void datagridviewdel_Dev(DevExpress.XtraGrid.Views.Grid.GridView Mydgv)
    {
       if (MessageBox.Show("你确定要删除选中的记录吗？", "删除提示", MessageBoxButtons.YesNo,               MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2, 0, false) == DialogResult.Yes)
       {
           int iSelectRowCount = Mydgv.SelectedRowsCount;
           if (iSelectRowCount > 0)
           {
             Mydgv.DeleteSelectedRows();
           }
       }
    }
16. 新增: (对于新增,其本身的AddNewRow方法就可以做到)
    private void btn_add_Click(object sender, EventArgs e)
    {  
       gridView1.AddNewRow();   
     }
    具体如果对于新加行还有什么特别的设置,可以在它gridView1_InitNewRow事件中填写:
    private void gridView1_InitNewRow(object sender, DevExpress.XtraGrid.Views.Grid.InitNewRowEventArgs e)
    {
       ColumnView View = sender as ColumnView;
       View.SetRowCellValue(e.RowHandle, View.Columns[0], gridView1.GetRowCellValue(gridView1.GetRowHandle(gridView1.RowCount - 2), gridView1.Columns[0])); //复制最后一行的数据到新行
            View.SetRowCellValue(e.RowHandle, View.Columns[1], gridView1.GetRowCellValue(gridView1.GetRowHandle(gridView1.RowCount - 2), gridView1.Columns[1])); //复制最后一行的数据到新行
    }
17. 保存 (第三方控件提供的RefreshData和RefreshDataSource方法对于保存数据都不好使,最后还是使用了Dgvsave的datagridviewsave方法,用这个方法就可以)
18.特效：gridcontrol中有5种view 型式，普通的是gridview，然后分别为cardview、BandedView、Advanced BandedView、LayoutView；共5种。
  1）、view组中把OptionView下的viewmode 设置成
“Carousel”就达到这种“旋转木马”式的gridcontrol view 特效了
  2）、layoutView1.OptionsCarouselMode.PitchAngle 这个属性决定“旋转木马”的pitch angle 螺距角; 螺旋角; 螺旋升角; 俯仰角; 倾角; 节锥半角 
  3）、Roll Angle 属性决定着 倾侧角度
  4）、指定数据源，显示数据：
  //显示数据
        private void showData(List<Employee > list)
        {
            DataTable dt = new DataTable("OneEmployee");
            dt.Columns.Add("Caption", System.Type.GetType("System.String"));
            dt.Columns.Add("Department", System.Type.GetType("System.String"));
            dt.Columns.Add("PhotoName", System.Type.GetType("System.Byte[]"));
            for (int i = 0; i < list.Count; i++)
            {
                DataRow dr = dt.NewRow();
                dr["Caption"] = list[i].Name;
                dr["Department"] = list[i].Department;
                string imagePath = @"D:\C#\photos\" + list[i].PhotoPath;
                dr["PhotoName"] = getImageByte(imagePath);
                dt.Rows.Add(dr);
            }
            gridControl1.DataSource = dt;
        }
        //返回图片的字节流byte[]
        private byte[] getImageByte(string imagePath)
        {
            FileStream files = new FileStream(imagePath, FileMode.Open);
            byte[] imgByte = new byte [files.Length ];
            files.Read(imgByte, 0, imgByte.Length);
            files.Close();
            return imgByte;
        }
19.检查数据的有效性
  在gridview的ValidateRow事件中加入检查代码：
  #region 检查数据
  private void gridView1_ValidateRow(object sender, ValidateRowEventArgs e)
  {
  GridView view = sender as GridView;
  view.ClearColumnErrors();
  if (view.GetRowCellValue(e.RowHandle, "ReceiveDate") == DBNull.Value)
  {
  e.Valid = false;
  view.SetColumnError(view.Columns["ReceiveDate"], "必须指定日期");
  }
  }
  #endregion
  调用gridview.UpdateCurrentRow()方法执行检查
最常用的DevExpress Winform 4个代码片段：
一 、GridControl的删除操作
private void rILinkEditInfoDel_Click(object sender, EventArgs e)
{
     if (XtraMessageBox.Show("请确定是否删除当前记录？", "警告", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes)
     {
         DataRow row = gvInfos.GetDataRow(gvInfos.FocusedRowHandle);
          delByCode(row["Code"].ToString());
         XtraMessageBox.Show("操作成功！");
     }
}
二、绑定非数据表中列
Hashtable ht = new Hashtable();
private void gridView6_CustomUnboundColumnData(object sender, DevExpress.XtraGrid.Views.Base.CustomColumnDataEventArgs e)
{
     GridView View = sender as GridView;
     if (e.RowHandle >= 0)
     {
         object needAlert = View.GetRowCellValue(e.RowHandle, View.Columns["needAlert"]);
         if (needAlert != null & needAlert != DBNull.Value && needAlert.ToString().Trim() != "0" & View.GetRowCellValue(e.RowHandle, View.Columns["Value"]) != DBNull.Value)
         {
             decimal AverValue = Convert.ToDecimal(View.GetRowCellValue(e.RowHandle, View.Columns["Value"]));
             object MinValue = View.GetRowCellValue(e.RowHandle, View.Columns["MinValue"]);
             object MaxVlaue = View.GetRowCellValue(e.RowHandle, View.Columns["MaxValue"]);
             if (MinValue != DBNull.Value & MinValue != null & MaxVlaue.ToString() != "" & MaxVlaue != DBNull.Value && MaxVlaue != null & MaxVlaue.ToString() != "")
             {
                 decimal gridColumn2 = Convert.ToDecimal(View.GetRowCellValue(e.RowHandle, View.Columns["MinValue"]));
                 decimal gridColumn1 = Convert.ToDecimal(View.GetRowCellValue(e.RowHandle, View.Columns["MaxValue"]));
                 if (gridColumn2 > AverValue || AverValue > gridColumn1)
                 {
                     if (!ht.ContainsKey("pic"))
                         ht.Add("pic", GetImage(1));
                     e.Value = ht["pic"];
                 }
             }
         }
     }
}
/// <summary>
/// 由资源文件获取图片
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
byte[] GetImage(int key)
{
     Image img = DevExpress.Utils.Controls.ImageHelper.CreateImageFromResources(string.Format("RiverSys.Resources.{0}.gif", key.ToString()), typeof(RiverInfos).Assembly);
     return DevExpress.XtraEditors.Controls.ByteImageConverter.ToByteArray(img, ImageFormat.Gif);
}
/// <summary>
/// 动态根据条件设置行样式
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
private void gridView6_RowStyle(object sender, DevExpress.XtraGrid.Views.Grid.RowStyleEventArgs e)
{
     GridView View = sender as GridView;
     if (e.RowHandle >= 0)
     {
         object needAlert = View.GetRowCellValue(e.RowHandle, View.Columns["needAlert"]);
         if (needAlert != null & needAlert != DBNull.Value && needAlert.ToString().Trim() != "0" & View.GetRowCellValue(e.RowHandle, View.Columns["Value"]) != DBNull.Value)
         {
             decimal AverValue = Convert.ToDecimal(View.GetRowCellValue(e.RowHandle, View.Columns["Value"]));
             object MinValue = View.GetRowCellValue(e.RowHandle, View.Columns["MinValue"]);
             object MaxVlaue = View.GetRowCellValue(e.RowHandle, View.Columns["MaxValue"]);
             if (MinValue != DBNull.Value & MinValue != null & MaxVlaue.ToString() != "" & MaxVlaue != DBNull.Value && MaxVlaue != null & MaxVlaue.ToString() != "")
             {
                 decimal gridColumn2 = Convert.ToDecimal(MinValue);
                 decimal gridColumn1 = Convert.ToDecimal(MaxVlaue);
                 if (gridColumn2 > AverValue || AverValue > gridColumn1)
                 {
                     e.Appearance.ForeColor = Color.Red;
                     e.Appearance.BackColor = Color.LightGray;
                 }
             }
         }
     }
}
三、GridControl 中颜色选择控件
private void gvMapColor_CustomUnboundColumnData(object sender, DevExpress.XtraGrid.Views.Base.CustomColumnDataEventArgs e)
{
     GridView view = sender as GridView;
     DataView dv = view.DataSource as DataView;
     if (e.IsGetData)
     {
         string strVal = dv[e.ListSourceRowIndex]["Color"].ToString();
         if (strVal != "")
         {
             //e.Value = DevExpress.Utils.StyleLayout.ColorFromString(strVal);
             e.Value = Common.HexToColor(strVal);
         }
     }
     else
     {
         //Color colorVal = DevExpress.Utils.StyleLayout.ColorFromString(e.Value.ToString());
         Color colorVal = (Color)e.Value;
         dv[e.ListSourceRowIndex]["Color"] = Common.RGB_HEX(colorVal.ToArgb());
     }
}
四、关于 GridControl 验证示例
/**//// <summary>
/// 初始化GridView，绑定数据
/// </summary>
/// <param name="parentId"></param>
private void GridViewBindData(string parentId)
{
this.gridView1.Columns.Clear();
this.FDs= areaSetupActionHelper.getDsRegionByParentId(parentId);
this.gridCArea.DataSource =this.FDs.Tables[0].DefaultView;
    this.gridView1.Columns["id"].VisibleIndex = -1;
    this.gridView1.Columns["childCounts"].VisibleIndex = -1;
    this.gridView1.Columns["reg_id"].Caption = "区划编号";
    this.gridView1.Columns["reg_name"].Caption = "区划名称";
    this.gridView1.Columns["parent_id"].Caption = "父区划编号";
    this.gridView1.Columns["reg_desc"].Caption = "区划描述";
    this.gridView1.Columns["parent_id"].ImageIndex =1;
    this.gridView1.Columns["reg_desc"].ImageIndex = 0;
    RepositoryItemTextEdit textEditReg_Id = new RepositoryItemTextEdit();
    textEditReg_Id.Mask.EditMask =parentId+"file://d%7b2,3/}";
    textEditReg_Id.Mask.MaskType = DevExpress.XtraEditors.Mask.MaskType.Regular;
    this.gridView1.Columns["reg_id"].ColumnEdit = textEditReg_Id;
    this.gridView1.Columns["reg_desc"].ColumnEdit = new RepositoryItemMemoExEdit();
    TreeListNode node = this.treelArea.FocusedNode.ParentNode;
    string fid = node==null"0":node.GetValue("RegID").ToString().Trim();
    DataSet ds = areaSetupActionHelper.getDsRegionByParentId(fid);
    RepositoryItemLookUpEdit lookUEParent_Id = new RepositoryItemLookUpEdit();
    lookUEParent_Id.Columns.Add(new LookUpColumnInfo("reg_id", 40, "区划编号"));
    lookUEParent_Id.Columns.Add(new LookUpColumnInfo("reg_name", 40, "区划名称"));
    lookUEParent_Id.DataSource = ds.Tables[0].DefaultView;
    lookUEParent_Id.ValueMember = "reg_id";
    lookUEParent_Id.DisplayMember = "reg_id";
    this.gridView1.Columns["parent_id"].ColumnEdit = lookUEParent_Id;
}
/**//// <summary>
/// gridView单元格验证的相关处理程序
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
private void gridView1_ValidatingEditor(object sender, DevExpress.XtraEditors.Controls.BaseContainerValidateEditorEventArgs e)
{
if (e.Valid == false&this.gridView1.FocusedColumn.FieldName == "reg_id")
{
      e.ErrorText = "区划编号不合法！\n应为父区划编号加2～3位数据组成！";   
    }
    if (this.gridView1.FocusedColumn.FieldName == "reg_name")
    {     
     Regex reg=new Regex(@"[\u4e00-\u9fa5]{1,20}");
     Match m=reg.Match(e.Value.ToString().Trim());
     if (m.Length != e.Value.ToString().Trim().Length)
     {
      e.Valid = false;
      e.ErrorText = "区划名称应为汉字\n长度为1至20";
     }
    }
}
private void gridView1_InvalidValueException(object sender, InvalidValueExceptionEventArgs e)
{
    if (MyDialog.Alert("　您所填写的内容不符合规则\n　要放弃您刚才对此项所做的更改吗？", "您所编辑的内容不符合规则", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes)
    {
     e.ExceptionMode = ExceptionMode.Ignore;
    }
}
/**//// <summary>
/// gridView行验证的相关处理程序
/// </summary>
private void gridView1_ValidateRow(object sender, DevExpress.XtraGrid.Views.Base.ValidateRowEventArgs e)
{
string regid = this.gridView1.GetRowCellValue(e.RowHandle, "reg_id").ToString().Trim();
string regName = this.gridView1.GetRowCellValue(e.RowHandle, "reg_name").ToString().Trim();
if ( regid.Length < 1)
{
     e.Valid = false;
     this.gridView1.SetColumnError(this.gridView1.Columns["reg_id"], "请填写区划编号!", DevExpress.XtraEditors.DXErrorProvider.ErrorType.Default);
      // e.ErrorText = "区划名称不能为空!";
    }
    if (regName.Length < 1)
    {
     e.Valid = false;
     this.gridView1.SetColumnError(this.gridView1.Columns["reg_name"], "区划名称不能为空!", DevExpress.XtraEditors.DXErrorProvider.ErrorType.Default);
    }
}
private void gridView1_InvalidRowException(object sender, DevExpress.XtraGrid.Views.Base.InvalidRowExceptionEventArgs e)
{
    if (e.RowHandle >= 0)
    {
     if (this.gridView1.GetRowCellValue(e.RowHandle, this.gridView1.Columns["reg_id"]).ToString().Trim() == "" || this.gridView1.GetRowCellValue(e.RowHandle, this.gridView1.Columns["reg_name"]).ToString().Trim() == "")
     {
      if (MyDialog.Alert(" 　您所填写的内容不符合规则\n　要放弃您刚才对此项所做的更改吗？", "您所编辑的内容不符合规则", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes)
      {
       e.ExceptionMode = ExceptionMode.Ignore;
      }
      else
      {
       e.ExceptionMode = ExceptionMode.NoAction;
      }
     }
    }
    else
    {
     e.ExceptionMode = ExceptionMode.Ignore;
    }
}
关于XtraGrid的使用（GridControl小结）来源：51编程网发布时间：2010-04-19Hits：146
DevExpress XtraGrid的使用（AspxGridControl小结） 收藏
自定义焦点行的方法： gridView_bcode.FocusedRowHandle = focuseRowInt; //通过设置GridView 的FocusedRowHandle属性
view plaincopy to clipboardprint
//获取焦点行任意单元格的数据    
ColumnView cv = (ColumnView)gridControl_Gongzi.FocusedView;//重新获取此ID 否则无法从表头连删获取不到id   
                        int focusedhandle = cv.FocusedRowHandle;   
                        object rowIdObj = gridView1.GetRowCellValue(focusedhandle, "id");   
                        if (DBNull.Value != rowIdObj)   
                        {   
                            FocusedRow_id = Convert.ToInt32(rowIdObj);   
                        }  
//获取焦点行任意单元格的数据 
ColumnView cv = (ColumnView)gridControl_Gongzi.FocusedView;//重新获取此ID 否则无法从表头连删获取不到id
                        int focusedhandle = cv.FocusedRowHandle;
                        object rowIdObj = gridView1.GetRowCellValue(focusedhandle, "id");
                        if (DBNull.Value != rowIdObj)
                        {
                            FocusedRow_id = Convert.ToInt32(rowIdObj);
                        }
 view plaincopy to clipboardprint
//当数据发生变化时执行    
     private void gridView1_CellValueChanged(object sender, CellValueChangedEventArgs e)   
     {   
         int intRowHandle = e.RowHandle;   
         FocusedRow_bumen = Convert.ToString(gridView1.GetRowCellValue(intRowHandle, "bumen"));  
         FocusedRow_xingming = Convert.ToString(gridView1.GetRowCellValue(intRowHandle, "xingming"));   
         //FocusedRow_jibengongzi = Convert.ToDecimal(gridView1.GetRowCellValue(intRowHandle, "jibengongzi"));   
         object rowJibengongziObj = gridView1.GetRowCellValue(intRowHandle, "jibengongzi");   
         if (DBNull.Value != rowJibengongziObj)   
         {   
             FocusedRow_jibengongzi = Convert.ToDecimal(rowJibengongziObj);   
         }   
      }  
   //当数据发生变化时执行 
        private void gridView1_CellValueChanged(object sender, CellValueChangedEventArgs e)
        {
            int intRowHandle = e.RowHandle;
            FocusedRow_bumen = Convert.ToString(gridView1.GetRowCellValue(intRowHandle, "bumen"));
            FocusedRow_xingming = Convert.ToString(gridView1.GetRowCellValue(intRowHandle, "xingming"));
            //FocusedRow_jibengongzi = Convert.ToDecimal(gridView1.GetRowCellValue(intRowHandle, "jibengongzi"));
            object rowJibengongziObj = gridView1.GetRowCellValue(intRowHandle, "jibengongzi");
            if (DBNull.Value != rowJibengongziObj)
            {
                FocusedRow_jibengongzi = Convert.ToDecimal(rowJibengongziObj);
            }
         } view plaincopy to clipboardprint
//设置焦点行的焦点单元格的位置   
ColumnView view = (ColumnView)gridControl_Gongzi.FocusedView;   
view.FocusedColumn = view.Columns["bumen"];  
//设置焦点行的焦点单元格的位置
ColumnView view = (ColumnView)gridControl_Gongzi.FocusedView;
view.FocusedColumn = view.Columns["bumen"]; view plaincopy to clipboardprint
//当焦点行发生改变时执行 获取选中焦点行id   
       private void gridView1_FocusedRowChanged(object sender, DevExpress.XtraGrid.Views.Base.FocusedRowChangedEventArgs e)   
       {   
           int intRowHandle = e.FocusedRowHandle;   
           object rowIdObj = gridView1.GetRowCellValue(intRowHandle, "id");   
           if (DBNull.Value != rowIdObj)//做个判断否则获取不到id后报错   
           {   
               FocusedRow_id = Convert.ToInt32(rowIdObj);   
           }   
       }  
 //当焦点行发生改变时执行 获取选中焦点行id
        private void gridView1_FocusedRowChanged(object sender, DevExpress.XtraGrid.Views.Base.FocusedRowChangedEventArgs e)
        {
            int intRowHandle = e.FocusedRowHandle;
            object rowIdObj = gridView1.GetRowCellValue(intRowHandle, "id");
            if (DBNull.Value != rowIdObj)//做个判断否则获取不到id后报错
            {
                FocusedRow_id = Convert.ToInt32(rowIdObj);
            }
        }   
 view plaincopy to clipboardprint
//焦点行的FocusedHandle为：   
 FocuseRow_Handle = -999998;   
//获取焦点行的handle   

