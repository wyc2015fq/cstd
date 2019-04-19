# form中动态生成Radiobutton控件 - 毛小亮 - 博客园
# [form中动态生成Radiobutton控件](https://www.cnblogs.com/xianerwonder/p/4296116.html)
```
public partial class GetLabelFields : Form
    {
        int tableCount;
        public GetLabelFields(AxMapControl axmapcontrol )
        {
            InitializeComponent();
            if(axmapcontrol.LayerCount<=0)
            {
                MessageBox.Show("无图层，请加载图层");
                return;
            }
            ILayer pLayer = axmapcontrol.get_Layer(0);
            IFeatureClass pFeatureClass = ((IFeatureLayer)pLayer).FeatureClass;
            ITable pTable = pFeatureClass as ITable;
            tableCount = pTable.Fields.FieldCount;
            if (tableCount >= 16)
            {
                MessageBox.Show("字段过多，不能全部显示");
            }
            for (int i = 0; i < tableCount-2; i++)
            {
Point pPoint = new Point(20,30*(i+1));
                RadioButton radioButton = new RadioButton();
                radioButton.Size = new System.Drawing.Size(50,20);
                radioButton.Location = pPoint;
                radioButton.Text = pTable.Fields.get_Field(i + 2).Name;
                this.gbxLabelField.Controls.Add(radioButton);
                radioButton.Show();
            }
        }private void btnOK_Click(object sender, EventArgs e)
        {
                 string filename = null;
            foreach (GroupBox gbxLabelField in this.Controls.OfType<GroupBox>())
            {
                foreach (RadioButton radiobutton in gbxLabelField.Controls.OfType<RadioButton>())
                {
                    if (radiobutton.Checked == true)
                    {
                        filename = radiobutton.Text;
                    }
                }
            }
            this.Tag =(object)filename;
            this.Close();
        }
    }
```

