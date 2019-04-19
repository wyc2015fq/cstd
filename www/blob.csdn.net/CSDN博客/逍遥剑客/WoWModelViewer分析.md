# WoWModelViewer分析 - 逍遥剑客 - CSDN博客
2009年02月20日 00:20:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4709标签：[float																[string																[数据库																[图形](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)
个人分类：[山口山寨																[工具开发](https://blog.csdn.net/xoyojank/article/category/1858383)](https://blog.csdn.net/xoyojank/article/category/488683)
启动:
WowModelViewApp::OnInit()
加载配置: WowModelViewApp::LoadSettings()
初始化主界面ModelViewer::ModelViewer()
初始化图形, 载入MPQModelViewer::Init(), 得到文件列表加入树控件里
初始化数据库ModelViewer::InitDatabase()
浏览模型:
ModelViewer::OnTreeSelect(wxTreeEvent &event)
清除原有的资源ModelCanvas::clearAttachments(),TextureManager::clear()
加载模型ModelViewer::LoadModel(constwxStringfn)
加载角色模型ModelCanvas::LoadCharModel(constchar *fn)
创建一个Model对象Model::Model(std::stringname, boolforceAnim)
初始化动画模型Model::initAnimated(MPQFile &f)
初始化模型资源Model::initCommon(MPQFile &f)
顶点, 法线, 包围盒, 纹理, 附件, 颜色, 透明度, 索引, 渲染批次排序
骨骼, 顶点/纹理缓冲, 粒子系统, 条带, 摄像机, 灯光, 动作
重置可见区域ModelCanvas::ResetView()
添加载入的模型Attachment::addChild(Displayable *disp, intid, intslot, floatscale, floatrot, Vec3Dpos)
更新主界面(菜单, 角色控制面板)
更新模型控制面板ModelControl::UpdateModel(Attachment *a)
更新动画控制面板AnimControl::UpdateModel(Model *m)
