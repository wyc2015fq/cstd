# c#数据与Excel交互（导入与导出） - yixianfeng41的专栏 - CSDN博客
2015年07月09日 17:20:36[第2梦](https://me.csdn.net/yixianfeng41)阅读数：1876
## 一、将Excel中的数据导入到数据库中
 注意，数据库表的header与Excel的header要一一对应;而且WPS不行，只有Micosoft Office才能正常工作，同时要添加相应的引用。
### 1.将当前DataGridView页面的内容导出
```
//将当前DataGridView页面的内容导出
        public static void print(DataGridView dataGridView1)
        {
            //导出到execl   
            try
            {
                SaveFileDialog saveFileDialog = new SaveFileDialog();
                saveFileDialog.Filter = "导出Excel (*.xls)|*.xls";
                saveFileDialog.FilterIndex = 0;
                saveFileDialog.RestoreDirectory = true;
                saveFileDialog.CreatePrompt = true;
                saveFileDialog.Title = "导出文件保存路径";
                saveFileDialog.ShowDialog();
                string strName = saveFileDialog.FileName;
                if (strName.Length != 0)
                {
                    //没有数据的话就不往下执行   
                    if (dataGridView1.Rows.Count == 0)
                        return;
                    // toolStripProgressBar1.Visible = true;
                    System.Reflection.Missing miss = System.Reflection.Missing.Value;
                    //实例化一个Excel.Application对象
                    Excel.Application excel = new Excel.Application();
                    excel.Application.Workbooks.Add(true);
                    excel.Visible = false;//若是true，则在导出的时候会显示EXcel界面。
                    if (excel == null)
                    {
                        MessageBox.Show("EXCEL无法启动！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                    Excel.Workbooks books = (Excel.Workbooks)excel.Workbooks;
                    Excel.Workbook book = (Excel.Workbook)(books.Add(miss));
                    Excel.Worksheet sheet = (Excel.Worksheet)book.ActiveSheet;
                    sheet.Name = "test";
                    int colCount = dataGridView1.Columns.Count - 1;//列数
                    int rowCount = dataGridView1.RowCount;//行数
                    for (int i = 0; i < colCount; i++)//设置标题
                    {
                        excel.Cells[1, i + 1] = dataGridView1.Columns[i].HeaderText;
                    }
                    for (int i = 0; i < rowCount - 1; i++)//填充数据
                    {
                        for (int j = 0; j < colCount; j++)
                        {
                            excel.Cells[i + 2, j + 1] = dataGridView1.Rows[i].Cells[j].Value.ToString();
                        }
                    }
                    sheet.SaveAs(strName, miss, miss, miss, miss, miss, Excel.XlSaveAsAccessMode.xlNoChange, miss, miss, miss);
                    book.Close(false, miss, miss);
                    books.Close();
                    excel.Quit();
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(sheet);
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(book);
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(books);
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(excel);
                    GC.Collect();
                    MessageBox.Show("数据已经成功导出!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    //System.Diagnostics.Process.Start(strName);
                }
            }
            catch
            {
                MessageBox.Show("导出失败");
            }
        }
```
### 2.将数据源DataTable导出
```
/// <summary>
        /// 把DataTable导出到EXCEL
        /// </summary>
        /// <param name="reportName">报表名称</param>
        /// <param name="dt">数据源表</param>
        /// <param name="saveFileName">Excel全路径文件名</param>
        /// <returns>导出是否成功</returns>
        public static bool ExportExcel(string reportName, System.Data.DataTable dt, string saveFileName)
        {
            if (dt == null)
            {
                _ReturnStatus = -1;
                _ReturnMessage = "数据集为空！";
                return false;
            }
            bool fileSaved = false;
            Excel.Application xlApp = new Excel.Application();
            if (xlApp == null)
            {
                _ReturnStatus = -1;
                _ReturnMessage = "无法创建Excel对象，可能您的计算机未安装Excel";
                return false;
            }
            Excel.Workbooks workbooks = xlApp.Workbooks;
            Excel.Workbook workbook = workbooks.Add(Excel.XlWBATemplate.xlWBATWorksheet);
            Excel.Worksheet worksheet = (Excel.Worksheet)workbook.Worksheets[1];//取得sheet1
            worksheet.Cells.Font.Size = 10;
            Excel.Range range;
            long totalCount = dt.Rows.Count;
            long rowRead = 0;
            float percent = 0;
            worksheet.Cells[1, 1] = reportName;
            ((Excel.Range)worksheet.Cells[1, 1]).Font.Size = 12;
            ((Excel.Range)worksheet.Cells[1, 1]).Font.Bold = true;
            //写入字段
            for (int i = 0; i < dt.Columns.Count; i++)
            {
                worksheet.Cells[2, i + 1] = dt.Columns[i].ColumnName;
                range = (Excel.Range)worksheet.Cells[2, i + 1];
                range.Interior.ColorIndex = 15;
                range.Font.Bold = true;
            }
            //写入数值
            for (int r = 0; r < dt.Rows.Count; r++)
            {
                for (int i = 0; i < dt.Columns.Count; i++)
                {
                    worksheet.Cells[r + 3, i + 1] = dt.Rows[r][i].ToString();
                }
                rowRead++;
                percent = ((float)(100 * rowRead)) / totalCount;
            }
            range = worksheet.get_Range(worksheet.Cells[2, 1], worksheet.Cells[dt.Rows.Count + 2, dt.Columns.Count]);
            range.BorderAround(Excel.XlLineStyle.xlContinuous, Excel.XlBorderWeight.xlThin, Excel.XlColorIndex.xlColorIndexAutomatic, null);
            if (dt.Rows.Count > 0)
            {
                range.Borders[Excel.XlBordersIndex.xlInsideHorizontal].ColorIndex = Excel.XlColorIndex.xlColorIndexAutomatic;
                range.Borders[Excel.XlBordersIndex.xlInsideHorizontal].LineStyle = Excel.XlLineStyle.xlContinuous;
                range.Borders[Excel.XlBordersIndex.xlInsideHorizontal].Weight = Excel.XlBorderWeight.xlThin;
            }
            if (dt.Columns.Count > 1)
            {
                range.Borders[Excel.XlBordersIndex.xlInsideVertical].ColorIndex = Excel.XlColorIndex.xlColorIndexAutomatic;
                range.Borders[Excel.XlBordersIndex.xlInsideVertical].LineStyle = Excel.XlLineStyle.xlContinuous;
                range.Borders[Excel.XlBordersIndex.xlInsideVertical].Weight = Excel.XlBorderWeight.xlThin;
            }
            //保存文件
            if (saveFileName != "")
            {
                try
                {
                    workbook.Saved = true;
                    workbook.SaveCopyAs(saveFileName);
                    fileSaved = true;
                }
                catch (Exception ex)
                {
                    fileSaved = false;
                    _ReturnStatus = -1;
                    _ReturnMessage = "导出文件时出错,文件可能正被打开！\n" + ex.Message;
                }
            }
            else
            {
                fileSaved = false;
            }
            //释放Excel对应的对象
            if (range != null)
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(range);
                range = null;
            }
            if (worksheet != null)
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(worksheet);
                worksheet = null;
            }
            if (workbook != null)
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(workbook);
                workbook = null;
            }
            if (workbooks != null)
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(workbooks);
                workbooks = null;
            }
            xlApp.Application.Workbooks.Close();
            xlApp.Quit();
            if (xlApp != null)
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(xlApp);
                xlApp = null;
            }
            GC.Collect();
            return fileSaved;
        }
```
## 二、将Excel中的数据导入到数据库
```
#region 导入方法
        /// <summary>
        /// 导入EXCEL到DataSet
        /// </summary>
        /// <param name="fileName">Excel全路径文件名</param>
        /// <returns>导入成功的DataSet</returns>
        private static DataTable ImportExcel(string fileName)
        {
            //判断是否安装EXCEL
            Excel.Application xlApp = new Excel.Application();
            if (xlApp == null)
            {
                _ReturnStatus = -1;
                _ReturnMessage = "无法创建Excel对象，可能您的计算机未安装Excel";
                return null;
            }
            //判断文件是否被其他进程使用            
            Excel.Workbook workbook;
            try
            {
                workbook = xlApp.Workbooks.Open(fileName, 0, false, 5, "", "", false, Excel.XlPlatform.xlWindows, "", true, false, 0, true, 1, 0);
            }
            catch
            {
                _ReturnStatus = -1;
                _ReturnMessage = "Excel文件处于打开状态，请保存关闭";
                return null;
            }
            //获得所有Sheet名称
            int n = workbook.Worksheets.Count;
            string[] SheetSet = new string[n];
            System.Collections.ArrayList al = new System.Collections.ArrayList();
            for (int i = 1; i <= n; i++)
            {
                SheetSet[i - 1] = ((Excel.Worksheet)workbook.Worksheets[i]).Name;
            }
            //释放Excel相关对象
            workbook.Close(null, null, null);
            xlApp.Quit();
            if (workbook != null)
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(workbook);
                workbook = null;
            }
            if (xlApp != null)
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(xlApp);
                xlApp = null;
            }
            GC.Collect();
            //把EXCEL导入到DataSet
            DataSet ds = new DataSet();
            DataTable table = new DataTable();
            string connStr = " Provider = Microsoft.Jet.OLEDB.4.0 ; Data Source = " + fileName + ";Extended Properties=Excel 8.0";
            using (OleDbConnection conn = new OleDbConnection(connStr))
            {
                conn.Open();
                OleDbDataAdapter da;
                string sql = "select * from [" + SheetSet[0] + "$] ";
                da = new OleDbDataAdapter(sql, conn);
                da.Fill(ds, SheetSet[0]);
                da.Dispose();
                table = ds.Tables[0];
                conn.Close();
                conn.Dispose();
            }
            return table;
        }
        //使用<span style="background-color: rgb(255, 0, 0);">SqlBulkCopy</span>将datatable中的内容导入到数据库中
        private static void SqlBulkCopyByDatatable(string connectionString, string TableName, DataTable dt)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                using (SqlBulkCopy sqlbulkcopy = new SqlBulkCopy(connectionString, SqlBulkCopyOptions.UseInternalTransaction))
                {
                    try
                    {
                        sqlbulkcopy.DestinationTableName = TableName;
                        for (int i = 0; i < dt.Columns.Count; i++)
                        {
                            sqlbulkcopy.ColumnMappings.Add(dt.Columns[i].ColumnName, dt.Columns[i].ColumnName);
                        }
                        sqlbulkcopy.WriteToServer(dt);
                        sqlbulkcopy.Close();
                    }
                    catch (System.Exception ex)
                    {
                        return;
                    }
                }
            }
        }
        //调用前两个方法完成从Excel到数据库
        public static void InExcelToDatabase( string DataTableName)
        {
             //首先导入到datatable中
            OpenFileDialog of = new OpenFileDialog();
            of.ShowDialog();
            of.Filter = "导入Excel (*.xls)|*.xls";
            string filename = of.FileName;
            DataTable table = ImportExcel(filename);
            SqlBulkCopyByDatatable(connectionString, DataTableName, table);
        }
```
﻿﻿
