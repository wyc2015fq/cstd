
# DevExpress的DateEdit设置显示日期和时间 - 追求卓越,做到专业 - CSDN博客


2015年06月26日 16:28:06[Waldenz](https://me.csdn.net/enter89)阅读数：41458


1. 设置Mask.EditMask和DisplayFormat,EditFormat属性，设置为一致：'yyyy-MM-dd HH:mm';  //按照想要的显示格式设置此字符串。

```python
this.dateEdit1.Properties.DisplayFormat.FormatString = "yyyy-MM-dd HH:mm";
            this.dateEdit1.Properties.DisplayFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dateEdit1.Properties.EditFormat.FormatString = "yyyy-MM-dd HH:mm";
            this.dateEdit1.Properties.EditFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dateEdit1.Properties.Mask.EditMask = "yyyy-MM-dd HH:mm";
```
2. 如果要显示时间需要设置VistaDisplayMode=true, VistaEditTime=true。意思为显示时间，在时间控件中会出现时钟图标，并可以编辑时间。

```python
this.dateEdit1.Properties.VistaDisplayMode = DevExpress.Utils.DefaultBoolean.True;
            this.dateEdit1.Properties.VistaEditTime = DevExpress.Utils.DefaultBoolean.True;
```
3. 设置时间部分的格式，时间部分指的是打开日期空间后，在时钟图标下的显示的日期格式。

```python
this.dateEdit1.Properties.VistaTimeProperties.DisplayFormat.FormatString = "HH:mm";
            this.dateEdit1.Properties.VistaTimeProperties.DisplayFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dateEdit1.Properties.VistaTimeProperties.EditFormat.FormatString = "HH:mm";
            this.dateEdit1.Properties.VistaTimeProperties.EditFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dateEdit1.Properties.VistaTimeProperties.Mask.EditMask = "HH:mm";
```
下图为设置后的日期控件
![](https://img-blog.csdn.net/20150626161916246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZW50ZXI4OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
该日期控件的设置全代码

```python
// 
            // dateEdit1
            // 
            this.dateEdit1.EditValue = new System.DateTime(2015, 6, 26, 14, 29, 8, 441);
            this.dateEdit1.Location = new System.Drawing.Point(24, 154);
            this.dateEdit1.Name = "dateEdit1";
            this.dateEdit1.Properties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.dateEdit1.Properties.DisplayFormat.FormatString = "yyyy-MM-dd HH:mm";
            this.dateEdit1.Properties.DisplayFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dateEdit1.Properties.EditFormat.FormatString = "yyyy-MM-dd HH:mm";
            this.dateEdit1.Properties.EditFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dateEdit1.Properties.Mask.EditMask = "yyyy-MM-dd HH:mm";
            this.dateEdit1.Properties.VistaDisplayMode = DevExpress.Utils.DefaultBoolean.True;
            this.dateEdit1.Properties.VistaEditTime = DevExpress.Utils.DefaultBoolean.True;
            this.dateEdit1.Properties.VistaTimeProperties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton()});
            this.dateEdit1.Properties.VistaTimeProperties.DisplayFormat.FormatString = "HH:mm";
            this.dateEdit1.Properties.VistaTimeProperties.DisplayFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dateEdit1.Properties.VistaTimeProperties.EditFormat.FormatString = "HH:mm";
            this.dateEdit1.Properties.VistaTimeProperties.EditFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dateEdit1.Properties.VistaTimeProperties.Mask.EditMask = "HH:mm";
            this.dateEdit1.Size = new System.Drawing.Size(134, 21);
            this.dateEdit1.TabIndex = 5;
```


