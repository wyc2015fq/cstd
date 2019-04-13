
# C\#通过模板创建Word文件 - 追求卓越,做到专业 - CSDN博客


2015年06月26日 09:45:15[Waldenz](https://me.csdn.net/enter89)阅读数：2894


**C\#通过模板创建Word文件。**模板文件中创建书签bookmark，并在调用时给书签赋值。
模板创建文件类

```python
/// <summary>
    ///创建Word格式的Report 的摘要说明
    /// </summary>
    public class CreateWordReport
    {
        private _Application wordApp = null;
        private _Document wordDoc = null;
        public _Application Application
        {
            get { return wordApp; }
            set { wordApp = value; }
        }
        public _Document Document
        {
            get { return wordDoc; }
            set { wordDoc = value; }
        }
        //通过模板创建新文档
        public void CreateNewDucument(string filePath)
        {
            killWinWordProcess();
            wordApp = new ApplicationClass();
            wordApp.DisplayAlerts = WdAlertLevel.wdAlertsNone;
            wordApp.Visible = false;
            object miss = System.Reflection.Missing.Value;
            object templateName = filePath;
            wordDoc = wordApp.Documents.Open(ref templateName, ref miss, ref miss, ref miss, ref miss, ref miss,
                ref miss, ref miss, ref miss, ref miss, ref miss, ref miss, ref miss, ref miss, ref miss, ref miss);
        }
        //保存新文件
        public void SaveDocument(string filePath)
        {
            object fileName = filePath;
            object format = WdSaveFormat.wdFormatDocument; //保存格式
            object miss = System.Reflection.Missing.Value;
            wordDoc.SaveAs(ref fileName, ref miss, ref miss, ref miss, ref miss, ref miss, ref miss, ref miss, ref miss,
                ref miss, ref miss, ref miss, ref miss, ref miss, ref miss, ref miss);
            //关闭wordDoc,wordApp对象
            object SaveChanges = WdSaveOptions.wdSaveChanges;
            object OriginalFormat = WdOriginalFormat.wdOriginalDocumentFormat;
            object RouteDocument = false;
            wordDoc.Close(ref SaveChanges, ref OriginalFormat, ref RouteDocument);
            wordApp.Quit(ref SaveChanges, ref OriginalFormat, ref RouteDocument);
        }
        //写入页眉
        public void InsertHeader(string docHeader)
        {
            wordApp.ActiveWindow.View.Type = WdViewType.wdOutlineView;
            wordApp.ActiveWindow.View.SeekView = WdSeekView.wdSeekPrimaryHeader;
            wordApp.ActiveWindow.ActivePane.Selection.InsertAfter(docHeader); //页眉内容
        }
        //在书签处插入值
        public bool InsertValue(string bookmark, string value)
        {
            object bkObj = bookmark;
            if (wordApp.ActiveDocument.Bookmarks.Exists(bookmark))
            {
                //该方法会截取value，输入文本内容显示不全
                //wordApp.ActiveDocument.Bookmarks.get_Item(ref bkObj).Select();
                //wordApp.Selection.TypeText(value);
                Range range = wordApp.ActiveDocument.Bookmarks.get_Item(ref bkObj).Range;//表格插入位置 
                range.Text = value;//在书签处插入文字内容  
                return true;
            }
            return false;
        }
        //在书签处插入表格
        public Table InsertTable(string bookmark, int rows, int columns)
        {
            object miss = System.Reflection.Missing.Value;
            object oStart = bookmark;
            Range range = wordDoc.Bookmarks.get_Item(ref oStart).Range; //表格插入位置
            Table newTable = wordDoc.Tables.Add(range, rows, columns, ref miss, ref miss);
            //设置表的样式
            newTable.Borders.Enable = 1; //允许有实线边框
            newTable.Borders.OutsideLineWidth = WdLineWidth.wdLineWidth050pt; //边框宽度
            //newTable.PreferredWidth = 17;
            newTable.AllowAutoFit = true;
            return newTable;
        }
        //给表格中单元格插入元素，table所在表格，row行号，column列号,value插入的元素
        public void InsertCell(Microsoft.Office.Interop.Word.Table table, int row, int column, string value)
        {
            table.Cell(row, column).Range.Text = value;
        }
        //给表格插入一行数据，n为表格的序号，row行号，column列数，values插入的值
        public void InsertCell(Microsoft.Office.Interop.Word.Table table, int row, int columns, string[] values)
        {
            for (int i = 0; i < columns; i++)
            {
                table.Cell(row, i + 1).Range.Text = values[i];
            }
        }
        // 杀掉winword.exe进程
        public void killWinWordProcess()
        {
            System.Diagnostics.Process[] processes = System.Diagnostics.Process.GetProcessesByName("WINWORD");
            foreach (System.Diagnostics.Process process in processes)
            {
                if (process.MainWindowTitle == "")
                {
                    process.Kill();
                }
            }
        }
    }
```
调用方法

```python
CreateWordReport report = new CreateWordReport();
 string templateFile = System.IO.Path.Combine("Repx", @"NotamListTemplate.dot");//模板文件,也可为doc文件
 report.CreateNewDucument(System.IO.Path.Combine(Application.StartupPath, templateFile));
 //将内容写入书签位置
 report.InsertValue("bookmarkUser", "");
 report.InsertValue("bookmarkDate", DateTime.Now.ToString("yyyy年M月dd日"));
 report.InsertValue("bookmarkNavigateRoute", sbContentFir.ToString());//重要通告(航路)
 report.InsertValue("bookmarkNotamContent", sbContentAd.ToString());//重要通告(机场)
 report.SaveDocument(path);
```




