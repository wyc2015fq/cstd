# ae保存图层 - 毛小亮 - 博客园
# [ae保存图层](https://www.cnblogs.com/xianerwonder/p/4325421.html)
```csharp;gutter
public static void OperateMapDoc(AxMapControl axMapC,string path, string strOperateType)
        {
            try
            {
          
            ////定义打开文件对话框
            //OpenFileDialog OpenFileDlg = new OpenFileDialog();
            ////定义保存文件对话框
            //SaveFileDialog SaveFileDlg = new SaveFileDialog();
            //OpenFileDlg.Filter = "地图文档文件(*.mxd)|*.mxd";
            //SaveFileDlg.Filter = "地图文档文件(*.mxd)|*.mxd";
            string strDocFileN = string.Empty;
            
            //判断操作文档地图的类型
            switch (strOperateType)
            {
                case "SaveDoc":
                    {
                        //pMapDocument = new MapDocumentClass();
                        //pMapDocument.Open(path, "");
                        ////判断文档是否为只读文档
                        //if (pMapDocument.get_IsReadOnly(pMapDocument.DocumentFilename) == true)
                        //{
                        //    DevExpress.XtraEditors.XtraMessageBox.Show("此地图文档为只读文档！", "信息提示");
                        //    return;
                        //}
                        //用相对路径保存地图文档
                        //pMapDocument.Save(true, true);
                        IMxdContents pMxdC;
                        pMxdC = axMapC.Map as IMxdContents;
                        IMapDocument pMapDocument = new MapDocumentClass();
                        pMapDocument.New(path);
                        IActiveView pActiveView = axMapC.Map as IActiveView;
                        pMapDocument.ReplaceContents(pMxdC);
                        pMapDocument.Save(true, true);
                        pMapDocument.Close();
                        DevExpress.XtraEditors.XtraMessageBox.Show("保存成功！", "信息提示");
                        break;
                    }
                default:
                    break;
            }
            }
            catch (Exception e)
            {
                DevExpress.XtraEditors.XtraMessageBox.Show(e.Message, "信息提示");
                return;
            }
        }
```
