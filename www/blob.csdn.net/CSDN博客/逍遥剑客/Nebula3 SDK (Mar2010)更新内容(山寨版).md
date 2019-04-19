# Nebula3 SDK (Mar2010)更新内容(山寨版) - 逍遥剑客 - CSDN博客
2010年04月12日 23:18:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3144
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
注: 本文非官方博客发布, 是我自己用版本工具对比出来的. 所以新加的我可能不了解其思想, 零碎的改动也被我忽略, 写出来的也不保证完全正确…
# Foundation
lCore
n增加OSX相关支持
nconfig.h增加PUBLIC_BUILD相关配置
n增加n_assert2宏
nRtti/Factory增加无FourCC的Register方法
lFrameSync
n增加FrameSyncSharedData类, 用于两个线程间的数据交换
nFrameSyncHandlerThread增加锁定帧时间模式
lInterface
nInterfaceHandlerBase增加一个可选的DoWork虚函数, 用于连续处理
lIo
n增加OSX相关支持
nAssignRegistry增加”sysmsh”, “systex”, “sysanim”三个系统专用Assign
nTextWriter增加一个模板方法Write<T>
nWin360FSWrapper增加宽字符路径支持
nXmlWriter/XmlReader增加一个模板方法Set<T>/Get<T>
nZipFileStream不再缓存内容, 所以现在只支持顺序Seek
lJobs
nJob分割成Slice, 轮询调用
nTPWorkerThread增加Stop方法
lMath
n增加frustum类
n删除pquatfeedbackloop
nbbox/float2/matrx44增加as<T>方法用于类型转换
ntransform44增加offset矩阵
nplane增加从3个点或点+向量的方式构造
lMemory
n增加OSX相关支持
lNet
n增加MessageClient用于以Stream方式进行消息传输
nStdTcpServer增加Broadcast方法
lSystem
n增加OSX相关支持
lThreading
n增加OSX相关支持
lUtil
n增加OSX相关支持
n增加Array::As<T>方法用于通用类型转换
nString类增加几个模板方法
# Render
lShared
n新增, 用于InternalGraphics和Graphics之间的数据同步
lVisibility
n新增, 用于场景可见性管理
lAnimation
n移除AnimMapper/AnimEventRegistry
n针对新的Job系统做了相关改动
lApp
nRenderApplication增加OnSetupResourceMappers()方法
nViewerAppliaction增加可见性系统支持, 移除命令行参数初始化方式
lCharacter
n增加CharacterVariationSet和CharacterVariationLibrary类
nCharacterAnimationController重写
lCoreGraphics
nShapeRenderer增加AddWireFrameBox方法
nMousePointer增加Alpha支持
lDebug
n增加PerfHUD开关.
lFrame
n增加FramePassBase基类
n删除一些不必要的方法
lGraphics
n增加DisplaySettings类
nCameraEntity增加CameraSettings属性
nDisplay的属性独立成DisplaySettings
nGraphicsEntity增加BoundingBox
nModelEntity增加一些Animation相关方法
lInternalGraphics
n移除原来的StageBuilder/Cell类
n适应新可见性系统的相关改动
nInternalGraphicsEntity增加HandleMessage虚函数, 子类对象可以单独进行消息处理了
lLighting
n增加LightPrePassServer, 实现pre-light-pass rendering
nInternalAbstractLightEntity增加ShadowIntensity属性
nShadowServer增加根据光源优先级排序的功能
lResources
n增加ResourcePool机制
n纹理Streaming载入支持异步
n其他Streaming相关新增内容(挺多的)
# Addons
lAttr
n从Render移过来的
lAudio2
n从Render移过来的
lForest
n新增, 类似于SpeedTree的东东
lFrameCapture
n新增, 用于连续的屏幕截取
lFx
n把原来的一些Animation命名空间下的类改到了Fx命名空间下
lNebula2
n兼容Nebula2新增的临时方法实现
lRemote
n增加设置端口号(port number)的相关方法
lUI
n新增许多控件, 但是没有实现. 也就是说, UI部分还是不完善
n另: 看到多了个Scaleform的例子, 不过没有相关代码(商业UI解决方案)
lVideo
nVideoServer改为BinkVideo的实现, 但是代码没有集成进来(又是一个商业解决方案)
# ExtLibs
lBullet
n新增第三方开源物理引擎, 但是还没有集成到引擎里, 只有一个测试程序
# Application
lApp
nGameApplication默认组件增加Remote子系统
lBaseGameFeature
n针对Fx的更改删减部分代码
lCommandHandlers
n新增, 只有一个CreateEntityCommand类
# MayaPlugin
lMayaCore
n新增MayaVariableRegistry用于设置选项的注册
lMayaExporters
n新增, 暂时只有Mesh的导出实现
lMayaExtractors
n新增MayaShapeNodeExtractor类, 用于解析场景几何结点
nMayaExtractorBase::Result枚举独立成MayaStatus
lMayaLegacy
n新增, 老代码的移植
lMayaMain
n删除MayaSettings
nMayaPlugin增加MEL(冒似是Maya Embedded Language?)的支持
lMayaShaders
n新增, Maya插件Shader部分实现
lMayaUtil
n新增MayaShaderUtil, 用于Maya shader node的查找
# Toolkit
lN2BatchConverter3
n新增, 用于批量转换.n2文件及其相关资源(mesh/texture/anims)到N3文件格式
lN2Converter3
n增加log功能
lN3Viewer
n新增, 支持远程控制的N3标准查看器
lTookitUtil
n改了一些类的路径, 使分类更加清晰
# Tests
新增以下工程:
lbullet_collision_simple
lbullet_dynamics_simple
ldrasa3testui
lfmodtest
lforestviewer
ltexturestreamingtest_win32
lziptest
# 其它
l基于XACT的Audio被干掉了
lwork/frame/win32/发现一个esmshadowmapping.xml, shaders下也有相关实现, 估计GlobalShadow要改成Exponential Shadow Mapping
l光照系统默认基于Light-Pre Pass的方式, 有兴趣的可以把testviewerapplication.cc的94行的numSpotLights改一下, 有惊喜
l有需要补充的, 请跟帖…
