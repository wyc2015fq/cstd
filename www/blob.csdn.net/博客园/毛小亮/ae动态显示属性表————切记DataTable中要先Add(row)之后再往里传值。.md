# ae动态显示属性表————切记DataTable中要先Add(row)之后再往里传值。 - 毛小亮 - 博客园
# [ae动态显示属性表————切记DataTable中要先Add(row)之后再往里传值。](https://www.cnblogs.com/xianerwonder/p/4296107.html)
```
public partial class FrmAttributeTable : Form
    {
        private AxMapControl m_MapCtl;
        public FrmAttributeTable(AxMapControl pMapCtl)
        {
            InitializeComponent();
            m_MapCtl = pMapCtl;
        }
        private void FrmAttributeTable_Load_1(object sender, EventArgs e)
        {
            ILayer pLayer = m_MapCtl.get_Layer(0);
            IFeatureLayer pFLayer = pLayer as IFeatureLayer;
            IFeatureClass pFC = pFLayer.FeatureClass;
            IFeatureCursor pFCursor = pFC.Search(null, false);
            IFeature pFeature = pFCursor.NextFeature();
            DataTable pTable = new DataTable();
            for (int i = 0; i < pFC.Fields.FieldCount-2; i++)
            {
                string fieldName = pFC.Fields.get_Field(i+2).Name;
                DataColumn colName = new DataColumn(fieldName);
                colName.DataType = System.Type.GetType("System.String");
                //colName.Caption为显示各用户看的名称。columnName改变的同时，caption也会变。
                pTable.Columns.Add(colName);
            }
            for (int j = 0; pFeature != null; j++)
            {
                DataRow pRow = pTable.NewRow();
                pTable.Rows.Add(pRow);
                for (int i = 0; i < pTable.Columns.Count; i++)
                {
                    string fieldValue = pFeature.get_Value(i + 2).ToString();//依次拿到各字段的值
                    
                    pTable.Rows[j][i] = fieldValue;
                }
                pFeature =  pFCursor.NextFeature();
            }
            
            dataGridView1.DataSource = pTable;
            //DataColumn colName = new DataColumn("ID");
            //colName.DataType = System.Type.GetType("System.String");
            //pTable.Columns.Add(colName);
            //DataColumn pBSM = new DataColumn("BSM");
            //pBSM.DataType = System.Type.GetType("System.String");
            //pTable.Columns.Add(pBSM);
            //DataColumn colArea = new DataColumn("TBMJ");
            //colArea.DataType = System.Type.GetType("System.String");
            //pTable.Columns.Add(colArea);
            //DataColumn colParameter = new DataColumn("TBZC");
            //colArea.DataType = System.Type.GetType("System.String");
            //pTable.Columns.Add(colParameter);
          
            
            
              
            //int indexOfID = pFC.FindField("ID");
            //int indexOfBSM = pFC.FindField("BSM");
            //int indexOfTBMJ = pFC.FindField("TBMJ");
            //int indexOfTBZC = pFC.FindField("TBZC");
            //while (pFeature != null)
            //{
            //    string id = pFeature.get_Value(indexOfID).ToString();
            //    string bsm = pFeature.get_Value(indexOfBSM).ToString();
            //    string TBMJ = pFeature.get_Value(indexOfTBMJ).ToString();
            //    string TBZC = pFeature.get_Value(indexOfTBZC).ToString();
            //    DataRow pRow = pTable.NewRow();
            //    pRow[0] = id;
            //    pRow[1] = bsm;
            //    pRow[2] = TBMJ;
            //    pRow[3] = TBZC;
            //    pTable.Rows.Add(pRow);
            //    pFeature = pFCursor.NextFeature();
            //}
            
        }
    }
```

