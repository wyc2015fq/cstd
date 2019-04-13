
# ArcGIS Engine开发－TOCControl中实现图层的拖放 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcGIS Engine开发－TOCControl中实现图层的拖放](https://www.cnblogs.com/watsonyin/archive/2006/09/14/504100.html)
TOCControl非常好，不用写一行代码就可以将整个地图的图层信息况显示出来；
TOCControl也非常坏，提供的接口非常少，我认为有用的只有三个：HitTest,SetBuddyControl,Update，而且Update方法一执行，整个TocControl就会重新装载一次，闪烁很厉害，实在是让人烦。要想在TOCControl中拖动图层，也并不容易，得动一动脑筋才行。
下面是我写的一个类，用于实现拖动图层，有需要的朋友，可以复制过去了，看一看。
需要说明的是，该类需要传入一个System.Windows.Forms.Control作为移动图层时显示要移到的位置，所以，在TOCControl上最好上一个Panel，然后传入到TocHelper中。另外，在计算同m_MovingLine显示的位置时，偶没找到什么好办法，只好设置了一个行高的参数。在正常字体时，据我的经验，行高是16，在Windows大字体显示时，行高是18，可以精确的显示。但这毕竟不是一个好办法。哪位高人要是看到了这个帖子，也请指点小弟一二，感激不尽！

![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)publicclassTocHelper
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)[    ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateESRI.ArcGIS.TOCControl.AxTOCControl m_toc;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateILayer m_layer=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateobjectm_other,m_index;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateLayerPopmenu m_LyMenu ;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateDataFramePopmenu m_FrameMenu=newDataFramePopmenu();
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publiceventCurrentLayerChangedHandler CurrentLayerChanged;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateboolm_Dragging=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateSystem.Windows.Forms.Control m_MovingLine;//= new System.Windows.Forms.Panel();
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicTocHelper(ESRI.ArcGIS.TOCControl.AxTOCControl toc)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_toc ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=toc;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_LyMenu ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=newLayerPopmenu(this);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_LyMenu.TOCControl ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=m_toc;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_FrameMenu.TOCControl ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=m_toc;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_toc.LabelEdit ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=esriTOCControlEdit.esriTOCControlManual;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)/**////处理事件
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)m_toc.OnMouseDown+=newITOCControlEvents_OnMouseDownEventHandler(m_toc_OnMouseDown);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_toc.OnMouseMove ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)+=newITOCControlEvents_OnMouseMoveEventHandler(m_toc_OnMouseMove);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_toc.OnMouseUp ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)+=newITOCControlEvents_OnMouseUpEventHandler(m_toc_OnMouseUp);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_toc.OnBeginLabelEdit ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)+=newITOCControlEvents_OnBeginLabelEditEventHandler(m_toc_OnBeginLabelEdit);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_toc.OnEndLabelEdit ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)+=newITOCControlEvents_OnEndLabelEditEventHandler(m_toc_OnEndLabelEdit);
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicvoidSetMoveLine(System.Windows.Forms.Control line)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_MovingLine ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=line;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_MovingLine.Size ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=newSystem.Drawing.Size(100,2);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_MovingLine.BackColor ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=System.Drawing.Color.Black;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_MovingLine.Visible ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateDevExpress.XtraBars.BarManager m_pBarManager;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicvoidSetBarManager(DevExpress.XtraBars.BarManager barMan)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_pBarManager ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=barMan;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_LyMenu.SetBarManager(barMan);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_FrameMenu.SetBarManager(barMan);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///获取当前图层
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///</summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicESRI.ArcGIS.Carto.ILayer CurrentLayer
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)get
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returnm_layer;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///刷新视图
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///</summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///<param name="layer"></param>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatevoidRefreshView(ILayer layer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_toc==null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_toc.Buddy==null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            IActiveView pView ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_toc.BuddyisESRI.ArcGIS.MapControl.IMapControl2)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                pView ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=(m_toc.BuddyasESRI.ArcGIS.MapControl.IMapControl2).ActiveView;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)elseif(m_toc.BuddyisESRI.ArcGIS.SceneControl.ISceneControl)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                IScene scene ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=(m_toc.BuddyasESRI.ArcGIS.SceneControl.ISceneControl).Scene;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(sceneisIActiveView)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    pView ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=sceneasIActiveView;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(pView!=null)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(layer!=null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    pView.PartialRefresh(esriViewDrawPhase.esriViewGeography,layer,pView.Extent);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    pView.Refresh();](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateintm_DragStartY;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicintMouseX,MouseY;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateintm_TextHeight=18;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatevoidm_toc_OnMouseDown(objectsender, ITOCControlEvents_OnMouseDownEvent e)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)/**////选中对象
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)ITOCControl m_TOCControl=(ITOCControl) m_toc.Object;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            esriTOCControlItem item ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=esriTOCControlItem.esriTOCControlItemNone;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ILayer layer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            IBasicMap map ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_TOCControl.HitTest(e.x,e.y,](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)refitem,refmap,reflayer,refm_other,refm_index);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            MouseX ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=e.x;MouseY=e.y;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_DragStartY ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=e.y;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//设置当前图层
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//if (item == esriTOCControlItem.esriTOCControlItemLayer)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//{
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_layer!=layer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_layer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=layer;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(CurrentLayerChanged!=null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        CurrentLayerChanged();](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//}
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//else
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//{
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//if (m_layer != null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//{
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//m_layer = null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//if (CurrentLayerChanged != null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//CurrentLayerChanged();
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//}
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//}
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)m_LyMenu.CurrentLayer=m_layer;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_FrameMenu.CurrentLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=m_layer;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//如果点中的图例，则弹出符号设置窗口
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if((e.button==1)&&(item==esriTOCControlItem.esriTOCControlItemLegendClass))
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ILegendGroup legendGroup;](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ILegendClass legendClass;](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                legendGroup ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=m_otherasILegendGroup;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                Debug.Assert(legendGroup ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)!=null);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                legendClass ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=legendGroup.get_Class(Convert.ToInt32(m_index.ToString()));
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                Debug.Assert(legendClass ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)!=null);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ISymbol pSymbol ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=legendClass.Symbol;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                //选择符号窗口代码去掉了。](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//如果是鼠标右键，则弹出右键菜单
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(e.button==2)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                System.Diagnostics.Debug.Assert(m_pBarManager ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)!=null);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if((item==esriTOCControlItem.esriTOCControlItemLayer)&&(layer!=null))
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_pBarManager.SetPopupContextMenu(m_toc,m_LyMenu.PopMenu);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)elseif(item==esriTOCControlItem.esriTOCControlItemMap)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_pBarManager.SetPopupContextMenu(m_toc,m_FrameMenu.PopMenu);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_pBarManager.SetPopupContextMenu(m_toc,](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)null);
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//如果鼠标左键选中了一个图层，则设为拖动状态
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if((e.button==1)&&(layer!=null))
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_Dragging ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=true;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_DestLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            m_TextHeight ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=m_toc.Parent.Font.Height+2;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateintGetLayerIndex(IBasicMap map,ILayer layer,boolDragUp)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ILayer tmpLayer;](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)for(inti=0;i<=map.LayerCount-1;i++)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                tmpLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=map.get_Layer(i);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(tmpLayer==layer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(DragUp==true)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returni-1;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returni;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return0;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateintGetLayerIndex(ICompositeLayer groupLayer,ILayer layer,boolDragUp)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)for(inti=0;i<=groupLayer.Count-1;i++)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(groupLayer.get_Layer(i)==layer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(i==groupLayer.Count-1)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returni;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(DragUp==true)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returni;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returni+1;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                    }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return0;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateILayer m_DestLayer;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateboolm_DestIsMap=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateboolm_DragToCorrectPos=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatevoidm_toc_OnMouseMove(objectsender, ITOCControlEvents_OnMouseMoveEvent e)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)/**////如果正在拖动图层
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_Dragging==true)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_DragToCorrectPos ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ITOCControl m_TOCControl ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=(ITOCControl) m_toc.Object;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                esriTOCControlItem item ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=esriTOCControlItem.esriTOCControlItemNone;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ILayer layer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                IBasicMap map ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)objectother=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)objectindex=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_TOCControl.HitTest(e.x,e.y,](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)refitem,refmap,reflayer,refother,refindex);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_DestIsMap ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_DestLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=layer;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(item==esriTOCControlItem.esriTOCControlItemMap)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_DestIsMap ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=true;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)intyy;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    yy ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=Convert.ToInt32(Math.Floor(e.y/m_TextHeight)*m_TextHeight)+m_TextHeight;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_MovingLine.Location ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=newSystem.Drawing.Point(30,yy);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_MovingLine.Width ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=m_toc.Width-35;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_MovingLine.Visible ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=true;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_toc.MousePointer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=ESRI.ArcGIS.SystemUI.esriControlsMousePointer.esriPointerDefault;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_DragToCorrectPos ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=true;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)elseif((item==esriTOCControlItem.esriTOCControlItemLayer)&&(layer!=m_layer)&&(layer!=null))
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_DestLayerisIGroupLayer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        m_MovingLine.Visible ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        m_toc.MousePointer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=ESRI.ArcGIS.SystemUI.esriControlsMousePointer.esriPointerLabel;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        m_DragToCorrectPos ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=true;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                    }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)intyy;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(e.y>m_DragStartY)//向下拖放
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                            yy ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=Convert.ToInt32(Math.Floor(e.y/m_TextHeight)*m_TextHeight)+m_TextHeight;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else//向上拖放
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                            yy ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=Convert.ToInt32(Math.Floor(e.y/m_TextHeight)*m_TextHeight);
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        m_MovingLine.Location ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=newSystem.Drawing.Point(30,yy);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        m_MovingLine.Width ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=m_toc.Width-35;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        m_MovingLine.Visible ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=true;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        m_toc.MousePointer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=ESRI.ArcGIS.SystemUI.esriControlsMousePointer.esriPointerDefault;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        m_DragToCorrectPos ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=true;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                    }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_MovingLine.Visible ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_toc.MousePointer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=ESRI.ArcGIS.SystemUI.esriControlsMousePointer.esriPointerDefault;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///取得图层的上一级对象，可能是igrouplayer,或ibasicmap
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///</summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///<param name="map"></param>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///<param name="layer"></param>
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///<returns></returns>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateobjectGetLayerParent(IBasicMap map,ILayer layer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ILayer tmpLayer;](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)for(inti=0;i<=map.LayerCount-1;i++)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                tmpLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=map.get_Layer(i);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(tmpLayer==layer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returnmap;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)elseif(tmpLayerisIGroupLayer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    IGroupLayer ly ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=FindParentGroupLayer(tmpLayerasIGroupLayer,layer);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(ly!=null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returnly;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returnmap;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateIGroupLayer FindParentGroupLayer(IGroupLayer groupLayer,ILayer layer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(!(groupLayerisICompositeLayer))
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returngroupLayer;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ICompositeLayer comLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=groupLayerasICompositeLayer;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ILayer tmpLayer;](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)for(inti=0;i<=comLayer.Count-1;i++)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                tmpLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=comLayer.get_Layer(i);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(tmpLayer==layer)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returngroupLayer;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)elseif(tmpLayerisIGroupLayer)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returnFindParentGroupLayer(tmpLayerasIGroupLayer,layer);
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returnnull;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///在grouplayer中移动图层
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///</summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///<param name="pGroupLayer"></param>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///<param name="pLayer"></param>
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///<param name="nIndex"></param>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatevoidMoveLayerTo(IGroupLayer pGroupLayer, ILayer pLayer,intnIndex)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ICompositeLayer pCompositeLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=pGroupLayerasICompositeLayer;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//if(pCompositeLayer.Count < 2)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//return ;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(pCompositeLayer.Count-1==nIndex)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                pGroupLayer.Delete(pLayer);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                pGroupLayer.Add(pLayer);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            IArray pArray ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=newArrayClass();
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)for(inti=0; i<pCompositeLayer.Count; i++)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                pArray.Add(pCompositeLayer.get_Layer(i));](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            pGroupLayer.Clear();](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ILayer pLayer1;](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)for(inti=0; i<pArray.Count; i++)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(pCompositeLayer.Count==nIndex)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    pGroupLayer.Add(pLayer);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                pLayer1  ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=pArray.get_Element(i)asILayer;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(pLayer1==pLayer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)continue;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                pGroupLayer.Add(pLayer1);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatevoidm_toc_OnMouseUp(objectsender, ITOCControlEvents_OnMouseUpEvent e)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_Dragging==true)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)check dragging conditions\#regioncheck dragging conditions
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_Dragging ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_toc.MousePointer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=ESRI.ArcGIS.SystemUI.esriControlsMousePointer.esriPointerDefault;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_toc==null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_toc.Buddy==null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_DragToCorrectPos==false)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_DragToCorrectPos ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_MovingLine.Visible ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                IMap map](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                IScene scene](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=null;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_toc.BuddyisIMapControl2)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    map ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=(m_toc.BuddyasIMapControl2).Map;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)elseif(m_toc.BuddyisESRI.ArcGIS.SceneControl.ISceneControl)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    scene ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=(m_toc.BuddyasESRI.ArcGIS.SceneControl.ISceneControl).Scene;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                IBasicMap bmap;](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(map!=null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    bmap ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=mapasIBasicMap;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    bmap ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=sceneasIBasicMap;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(bmap.LayerCount==0)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)\#endregion
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)booldestIgnoreGroupLayer=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_DestIsMap==true)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_DestLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=bmap.get_Layer(0);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    destIgnoreGroupLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=true;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_DestLayer==null)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m_layer==m_DestLayer)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)boolDragUp;//是否正向上拖放
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)DragUp=(e.y<m_DragStartY);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)intdestIndex;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)objectbuddy=m_toc.Buddy;
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                m_toc.SetBuddyControl(](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)null);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)try
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)objectsrcGroupLayer=GetLayerParent(bmap,m_layer);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)objectdestGroupLayer=GetLayerParent(bmap,m_DestLayer);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//先删除源图层
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(srcGroupLayerisGroupLayer)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        (srcGroupLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)asIGroupLayer).Delete(m_layer);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        bmap.DeleteLayer(m_layer);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//再加入，并移到合适位置
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if((m_DestLayerisIGroupLayer)&&(destIgnoreGroupLayer==false))
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        (m_DestLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)asIGroupLayer).Add(m_layer);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        destIndex ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=GetLayerIndex(m_DestLayerasICompositeLayer,m_layer,DragUp);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        MoveLayerTo(m_DestLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)asIGroupLayer,m_layer,destIndex);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        RefreshView(m_DestLayer);                                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                    }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)elseif(destGroupLayerisIGroupLayer)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        (destGroupLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)asIGroupLayer).Add(m_layer);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        destIndex ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=GetLayerIndex(destGroupLayerasICompositeLayer,m_DestLayer,DragUp);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        MoveLayerTo(destGroupLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)asIGroupLayer,m_layer,destIndex);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        RefreshView(destGroupLayer ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)asILayer);
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                    }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)else
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                    ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        bmap.AddLayer(m_layer);](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        destIndex ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=GetLayerIndex(bmap,m_DestLayer,DragUp);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                                            ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(bmapisIMap)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                            (bmap ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)asIMap).MoveLayer(m_layer,destIndex);
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)elseif(bmapisIScene)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                            (bmap ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)asIScene).MoveLayer(m_layer,destIndex);
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                    }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[    ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[                }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)finally
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[                ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[                    m_toc.SetBuddyControl(buddy);  ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//重新绑定，并刷新toc
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatevoidm_toc_OnBeginLabelEdit(objectsender, ITOCControlEvents_OnBeginLabelEditEvent e)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[            e.canEdit ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)=false;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatevoidm_toc_OnEndLabelEdit(objectsender, ITOCControlEvents_OnEndLabelEditEvent e)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[        }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)[    }](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)





posted on2006-09-14 13:14[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=504100)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
