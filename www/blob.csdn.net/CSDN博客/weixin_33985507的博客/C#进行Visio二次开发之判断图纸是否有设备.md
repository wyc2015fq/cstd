# C#进行Visio二次开发之判断图纸是否有设备 - weixin_33985507的博客 - CSDN博客
2008年05月28日 20:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
判断Visio图纸上是否图纸有设备(存在图元), 有两种方法,一种是通过Window对象的SelectAll方法,在判断选择的集合是否不为空即可,如下:
![ExpandedBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)/// 判断是否有对象选定
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///</summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///<param name="window"></param>
![ExpandedBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)///<returns></returns>
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)        [CLSCompliant(false)]
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)publicstaticbool HasShapeInWindow(Window window)
![ExpandedBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://blog.csdn.net/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)bool result =false;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)try
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://blog.csdn.net/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                window.SelectAll();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                result = (window.Selection.Count >0);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                window.DeselectAll();
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)catch
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://blog.csdn.net/Images/dot.gif){ ;}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return result;
![ExpandedBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)        }
另外一种方法是判断页面对象Page的Shapes对象集合是否大于0即可,如下: 
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)            Visio.Page page = VisioUtility.GetPage(VisDocument, "馈线图");
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)if (page.Shapes.Count <=0)
![ExpandedBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://blog.csdn.net/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                MessageUtil.ShowTips("图纸上没有设备");
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return;
![ExpandedBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)            }
![ExpandedBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)/// 获取Visio文档制定的页面对象
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///</summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///<param name="visDocument"></param>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///<param name="pageType"></param>
![ExpandedBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)///<returns></returns>
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)        [CLSCompliant(false)]
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)publicstatic Page GetPage(Document visDocument, string pageType)
![ExpandedBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://blog.csdn.net/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            Page visPage;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)for (int i =1; i <= visDocument.Pages.Count; i++)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://blog.csdn.net/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                visPage = visDocument.Pages[i];
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)if (FormulaStringToString(visPage.PageSheet.get_Cells(CUST_PROP_PREFIX +"绘图页").Formula) == pageType)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://blog.csdn.net/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return visPage;
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)                }
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)returnnull;
![ExpandedBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)        }
两种方法基本上效果一样, 第二种方法可能速度会快一些,如果你在Visio的Document对象事件中有处理逻辑的时候, 如下所示, 而且有的情况下,你还不能用第一种方式来进行判断:
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)this.CtrlDrawing.SelectionChanged +=new EVisOcx_SelectionChangedEventHandler(ctrlDrawing_SelectionChanged);
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)
希望你能发现更多好玩的东西,呵呵
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)转载请注明出处：
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)撰写人：伍华聪  http://www.iqidi.com 
