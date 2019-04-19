# Nebula3绘制基本图元 - 逍遥剑客 - CSDN博客
2008年09月23日 00:55:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2333
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
看了好久好逮能让N3画出东西来了, 感觉它的架构就是专门为DX而设计的^_^. 虽说也能编写GL的中间层(因为N3的API是处于继承树的中间, 而不是最底下), 但是很多习惯一看就是DX嘛. 可能用惯API的会比较好上手点.
命令行程序都是从ConsoleApplication派生, 那么图形程序就是从RenderApplication派生了^_^. 
先看看RenderApplication都有哪些成员:
|Ptr<Core::CoreServer> coreServer;|前面的命令行程序已经用过了, 最基本的核心服务|
|----|----|
|Ptr<IO::IoServer> ioServer;|输入/输出服务, 别忘了把程序所需的资源(如shader)拷贝到正确的目录, 默认是exe文件所在的目录|
|Ptr<Interface::IOInterface> ioInterface;|进行资源异步(多线程)加载的接口(N3 NB就NB在多线程上)|
|Ptr<CoreGraphics::RenderDevice> renderDevice;|核心的图形系统, 可以看做是IDirect3DDevice9的封装^_^, 函数差不多|
|Ptr<CoreGraphics::DisplayDevice> displayDevice;|显示设备, 就是在哪画. 封装了窗口的创建和消息处理, 还有显示适配器等|
|Ptr<CoreGraphics::TransformDevice> transformDevice;|管理全局的矩阵变换与它们的组合|
|Ptr<CoreGraphics::ShaderServer> shaderServer;|Shader的管理器. N3用的是DX的Effect框架, 常用的shader都已经包含在资源文件里了|
|Ptr<CoreGraphics::ShapeRenderer> shapeRenderer;|几何图形的渲染器, 一般用于调试模式的辅助绘制, 如画个包围盒啥的|
|Ptr<CoreGraphics::VertexLayoutServer> vertexLayoutServer;|可以看成是IDirect3DVertexDeclaration9的一个管理器, 用于顶点格式的创建|
|Ptr<Resources::SharedResourceServer> sharedResourceServer;|共享资源的管理器|
|Ptr<Resources::ResourceManager> resourceManager;|相当于客户端与资源之间的一个中间层, 主要的功能是在占用最少内存的情况下为渲染提供资源. 如果资源没有加载完的话, 提供一个资源占位符. 具体的资源管理策略封装在ResourceMapper中, 如加载优先级和LOD等|
|Ptr<Models::ModelServer> modelServer;|用于加载和创建共享的模型对象|
|Ptr<Graphics::GraphicsServer> graphicsServer;|管理一个抽象的图形世界. 一个世界含有一个或多个"舞台(Stage)"和连接到舞台的一个或多个的"视野(View)". (xoyojank: 是不是场景的管理?)|
|Ptr<Lighting::LightServer> lightServer;|管理场景光源和实时的光照处理(SM3.0的, 还好新装了台98000GT的机子@_@)|
|Ptr<Lighting::ShadowServer> shadowServer;|阴影处理, 对于局部光源使用简单的Shadow Mapping, 全局光则使用PSSM.(又是SM3.0的)|
|Ptr<Input::InputServer> inputServer;|输入设备. 好像鼠标用的DirectInput, 而键盘直接用的Windows消息处理|
|Ptr<Frame::FrameServer> frameServer;|基于图像空间的处理, 多用于PostEffect.|
|Ptr<Scripting::ScriptServer> scriptServer;|脚本系统|
|Ptr<Http::HttpServer> httpServer;|N3的HTTP调试信息(这个也很NB, 不过实用吗?)|
|Ptr<Anim::AnimationServer> animationServer;|骨骼动画系统, SVN上最新的代码冒似重写了, 抛弃N2的那套封装|
我只是派生一下画了条线而已........
// TestWindow.cpp : Defines the entry point for the console application. 
// 
#include "stdneb.h" 
#include "apprender/viewerapplication.h" 
#include "coregraphics/memoryvertexbufferloader.h" 
#include "resources/resourceloader.h" 
#include "timing/time.h" 
using namespace App;
using namespace Util;
using namespace Math;
using namespace CoreGraphics;
using namespace Resources;
ImplementNebulaApplication()
class LineRenderApp : public RenderApplication
{
    Ptr<VertexBuffer> vertexBuffer;
    Ptr<ShaderInstance> shaderInstance;
public:
    bool Open()
    {
        if (RenderApplication::Open())
        {
            // vertex 
            Array<VertexComponent> vertexComponents;
            vertexComponents.Append(VertexComponent(VertexComponent::Position, 0, VertexComponent::Float3));
            float vertex[2][3] = { {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f} };
            vertexBuffer = VertexBuffer::Create();
            Ptr<MemoryVertexBufferLoader> vbLoader = MemoryVertexBufferLoader::Create();
            vbLoader->Setup(vertexComponents, 2, vertex, 2 * 3 * sizeof(float));
            vertexBuffer->SetLoader(vbLoader.upcast<ResourceLoader>());
            vertexBuffer->SetAsyncEnabled(false);
            vertexBuffer->Load();
            vertexBuffer->SetLoader(NULL);
            // shader 
            this->shaderInstance = this->shaderServer->CreateShaderInstance(ResourceId("shd:shape"));
            Ptr<ShaderVariable> color = this->shaderInstance->GetVariableBySemantic(ShaderVariable::Semantic("MatDiffuse"));
            color->SetVector(float4(1.0f, 1.0f, 1.0f, 1.0f));
            Ptr<ShaderVariable> mvp = this->shaderInstance->GetVariableBySemantic(ShaderVariable::Semantic("ModelViewProjection"));
            mvp->SetMatrix(matrix44::identity());
        }
        return true;
    }
    void OnRenderFrame()
    {
        this->displayDevice->ProcessWindowMessages();
        this->renderDevice->BeginFrame();
        this->renderDevice->BeginPass(this->renderDevice->GetDefaultRenderTarget(), this->shaderInstance);
        PrimitiveGroup primGroup;
        primGroup.SetBaseVertex(0);
        primGroup.SetNumVertices(2);
        primGroup.SetPrimitiveTopology(PrimitiveTopology::LineList);
        
        this->renderDevice->SetVertexBuffer(this->vertexBuffer);
        this->renderDevice->SetPrimitiveGroup(primGroup);
        this->renderDevice->Draw();
        this->renderDevice->EndPass();
        this->renderDevice->EndFrame();
        this->renderDevice->Present();
    }
};
void
NebulaMain(const CmdLineArgs& args)
{
    LineRenderApp app;
    app.SetAppName("Test Renderer");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}
当然, 画别的图元只要把PrimitiveTopology改改就好了
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20080927/Nebula3_First_RenderWindow.JPG)
