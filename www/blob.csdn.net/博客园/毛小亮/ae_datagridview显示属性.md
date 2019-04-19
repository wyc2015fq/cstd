# ae_datagridview显示属性 - 毛小亮 - 博客园
# [ae_datagridview显示属性](https://www.cnblogs.com/xianerwonder/p/4239320.html)
```csharp;gutter
public partial class FrmAttributeTable : Form
    {
        private AxMapControl m_MapCtl;
        public FrmAttributeTable(AxMapControl pMapCtl)
        {
            InitializeComponent();
            m_MapCtl = pMapCtl;
        }
        private void FrmAttributeTable_Load(object sender, EventArgs e)
        {
            ILayer pLayer = m_MapCtl.get_Layer(0);
            IFeatureLayer pFLayer = pLayer as IFeatureLayer;
            IFeatureClass pFC = pFLayer.FeatureClass;
            IFeatureCursor pFCursor = pFC.Search(null, false);
            IFeature pFeature = pFCursor.NextFeature();
            DataTable pTable = new DataTable();
            DataColumn colName = new DataColumn("ID");
            colName.DataType = System.Type.GetType("System.String");
            pTable.Columns.Add(colName);
            DataColumn pBSM = new DataColumn("BSM");
            pBSM.DataType = System.Type.GetType("System.String");
            pTable.Columns.Add(pBSM);
            DataColumn colArea = new DataColumn("TBMJ");
            colArea.DataType = System.Type.GetType("System.String");
            pTable.Columns.Add(colArea);
            int indexOfID = pFC.FindField("ID");
            int indexOfBSM = pFC.FindField("BSM");
            int indexOfTBMJ = pFC.FindField("TBMJ");
            while (pFeature != null)
            {
                string id = pFeature.get_Value(indexOfID).ToString();
                string bsm = pFeature.get_Value(indexOfBSM).ToString();
                string TBMJ = pFeature.get_Value(indexOfTBMJ).ToString();
                DataRow pRow = pTable.NewRow();
                pRow[0] = id;
                pRow[1] = bsm;
                pRow[2] = TBMJ;
                pTable.Rows.Add(pRow);
                pFeature = pFCursor.NextFeature();
            }
            dataGridView1.DataSource = pTable;
        }
    }
```
