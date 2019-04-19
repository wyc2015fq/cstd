# 实现一个同步的RenderApplication - 逍遥剑客 - CSDN博客
2009年02月26日 00:14:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1981标签：[application																[semantic																[destructor																[float																[timer																[constructor](https://so.csdn.net/so/search/s.do?q=constructor&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=destructor&t=blog)](https://so.csdn.net/so/search/s.do?q=semantic&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
Nebula3 Sep2008 SDK之后渲染改成异步的了, 对于想直接操作内部API接口的我来说, 没法直接下手了
因为RenderApplication的图形接口也变成了异步-_-
没办法, 参照Feb2008 SDK写了一个SyncRenderApplication. 
有一个问题待解决: InputServer要用到Display, 而Display用的是异步的Graphics, 汗
反正暂时不需要进行操作, 先这样凑合着用吧:
#pragma once
//------------------------------------------------------------------------------
/** 
    @class App::SyncRenderApplication
    
    Render application class with single threaded rendering.
    
    (C) 2009 xoyojank
*/
#include "app/application.h"
#include "core/coreserver.h"
#include "debug/debuginterface.h"
#include "io/ioserver.h"
#include "interface/iointerface.h"
#include "input/inputserver.h"
#include "timing/timer.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/vertexlayoutserver.h"
#include "resources/sharedresourceserver.h"
#include "resources/resourcemanager.h"
#include "models/modelserver.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "lighting/lightserver.h"
#include "lighting/shadowserver.h"
#include "frame/frameserver.h"
#include "addons/nebula2/nebula2wrapper.h"
#include "http/httpinterface.h"
#include "coregraphics/textrenderer.h"
//------------------------------------------------------------------------------
namespace App
{
class SyncRenderApplication : public Application
{
public:
    /// constructor
    SyncRenderApplication();
    /// destructor
    virtual ~SyncRenderApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();
    /// run the application
    virtual void Run();
private:
    /// update the current time values
    void UpdateTime();
protected:
    /// set quit requested flag
    void SetQuitRequested(bool b);
    /// return true if quit requested
    bool IsQuitRequested() const;
    /// called to configure display
    virtual void OnConfigureDisplay();
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// update world 
    virtual void OnUpdateFrame();
	/// render current frame
	virtual void OnRenderFrame();
    /// get current absolute time
    Timing::Time GetTime() const;
    /// get current frame time
    Timing::Time GetFrameTime() const;
    Ptr<Core::CoreServer> coreServer;
	Ptr<Debug::DebugInterface> debugInterface;
    Ptr<IO::IoServer> ioServer;
    Ptr<Interface::IOInterface> ioInterface;
	Ptr<CoreGraphics::RenderDevice> renderDevice;
	Ptr<CoreGraphics::DisplayDevice> displayDevice;
	Ptr<CoreGraphics::TransformDevice> transformDevice;
	Ptr<CoreGraphics::ShaderServer> shaderServer;
	Ptr<CoreGraphics::ShapeRenderer> shapeRenderer;
	Ptr<CoreGraphics::VertexLayoutServer> vertexLayoutServer;
	Ptr<CoreGraphics::TextRenderer> textRenderer;
	Ptr<Resources::SharedResourceServer> sharedResourceServer;
	Ptr<Resources::ResourceManager> resourceManager;
	Ptr<Models::ModelServer> modelServer;
	Ptr<InternalGraphics::InternalGraphicsServer> graphicsServer;
	Ptr<Lighting::LightServer> lightServer;
	Ptr<Lighting::ShadowServer> shadowServer;
	Ptr<Frame::FrameServer> frameServer;
	Ptr<Anim::AnimationServer> animationServer;
    Ptr<Input::InputServer> inputServer;
	Ptr<Http::HttpInterface> httpInterface;
    Timing::Timer timer;
    Timing::Time time;
    Timing::Time frameTime;
    bool quitRequested;
};
//------------------------------------------------------------------------------
/**
*/
inline void
SyncRenderApplication::SetQuitRequested(bool b)
{
    this->quitRequested = b;
}
//------------------------------------------------------------------------------
/**
*/
inline bool
SyncRenderApplication::IsQuitRequested() const
{
    return this->quitRequested;
}
//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
SyncRenderApplication::GetTime() const
{
    return this->time;
}
//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
SyncRenderApplication::GetFrameTime() const
{
    return this->frameTime;
}
} // namespace App
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  syncrenderapplication.cc
//  (C) 2009 xoyojank
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/syncrenderapplication.h"
#include "io/filestream.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "coregraphics/shaperenderer.h"
#include "resources/managedtexture.h"
#include "resources/managedmesh.h"
#include "coreanimation/streamanimationloader.h"
#include "internalgraphics/stagebuilder.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalview.h"
#include "lighting/internalspotlightentity.h"
#include "apprender/platformconfig.h"
#include "debugrender/debugshaperenderer.h"
#include "models/nodes/statenodeinstance.h"
namespace App
{
using namespace Core;
using namespace IO;
using namespace Interface;
using namespace CoreGraphics;
using namespace InternalGraphics;
using namespace Resources;
using namespace Models;
using namespace Input;
using namespace Timing;
using namespace CoreAnimation;
using namespace Lighting;
using namespace Frame;
using namespace Anim;
using namespace Debug;
//------------------------------------------------------------------------------
/**
*/
SyncRenderApplication::SyncRenderApplication() :
    time(0.0),
    frameTime(0.0),
    quitRequested(false)
{
    // empty
}
//------------------------------------------------------------------------------
/**
*/
SyncRenderApplication::~SyncRenderApplication()
{
    n_assert(!this->IsOpen());
}
//------------------------------------------------------------------------------
/**
*/
bool
SyncRenderApplication::Open()
{
    n_assert(!this->IsOpen());
    if (Application::Open())
    {
        // setup core subsystem
        this->coreServer = CoreServer::Create();
        this->coreServer->SetCompanyName(this->companyName);
        this->coreServer->SetAppName(this->appName);
        this->coreServer->Open();
        // setup io subsystem
        // FIXME: REDUNDANT!!! 
        this->ioServer = IoServer::Create();
        this->ioServer->RegisterStandardUriSchemes();
        this->ioServer->SetupStandardAssigns();
        this->ioServer->MountStandardZipArchives();
        this->ioInterface = IOInterface::Create();
        this->ioInterface->Open();
		// setup http subsystem
		this->httpInterface = Http::HttpInterface::Create();
		this->httpInterface->Open();
		// setup debug subsystem
		this->debugInterface = DebugInterface::Create();
		this->debugInterface->Open();
        // setup graphics subsystem
		this->renderDevice = RenderDevice::Create();
		this->displayDevice = DisplayDevice::Create();
		this->transformDevice = TransformDevice::Create();
		this->shaderServer = ShaderServer::Create();
		this->shapeRenderer = ShapeRenderer::Create();
		this->textRenderer = TextRenderer::Create();
		this->vertexLayoutServer = VertexLayoutServer::Create();
		this->sharedResourceServer = SharedResourceServer::Create();
		this->resourceManager = ResourceManager::Create();
		this->modelServer = ModelServer::Create();
		this->graphicsServer = InternalGraphicsServer::Create();
		this->lightServer = LightServer::Create();
		this->shadowServer = ShadowServer::Create();
		this->frameServer = FrameServer::Create();
		this->animationServer = AnimationServer::Create();
        this->OnConfigureDisplay();
		if (!this->displayDevice->Open())
		{
			n_error("SyncRenderApplication: failed to open display device!");
		}
		if (!this->renderDevice->Open())
		{
			n_error("SyncRenderApplication: failed to open render device!");
		}
		this->vertexLayoutServer->Open();
		this->shaderServer->Open();
		this->transformDevice->Open();
		this->shapeRenderer->Open();
		this->sharedResourceServer->Open();
		this->resourceManager->Open();
		this->frameServer->Open();
		this->textRenderer->Open();
		// setup resource mapper for textures
		// FIXME: should be configurable!
		Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
		texMapper->SetPlaceholderResourceId(ResourceId(PLACEHOLDER_TEXTURENAME));
		texMapper->SetResourceClass(Texture::RTTI);
		texMapper->SetResourceLoaderClass(StreamTextureLoader::RTTI);
		texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
		this->resourceManager->AttachMapper(texMapper.upcast<ResourceMapper>());
		// setup resource mapper for meshes
		// FIXME: should be configurable!
		Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
		meshMapper->SetPlaceholderResourceId(ResourceId(PLACEHOLDER_MESHNAME));
		meshMapper->SetResourceClass(Mesh::RTTI);
		meshMapper->SetResourceLoaderClass(StreamMeshLoader::RTTI);
		meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
		this->resourceManager->AttachMapper(meshMapper.upcast<ResourceMapper>());
		// setup resource mapper for animations
		// FIXME: should be configurable!
		//Ptr<SimpleResourceMapper> animMapper = SimpleResourceMapper::Create();
		//animMapper->SetPlaceholderResourceId(ResourceId("ani:examples/eagle.nax2"));
		//animMapper->SetResourceClass(MemoryAnimation::RTTI);
		//animMapper->SetResourceLoaderClass(StreamAnimationLoader::RTTI);
		//animMapper->SetManagedResourceClass(ManagedAnimation::RTTI);
		//this->resourceManager->AttachMapper(animMapper.upcast<ResourceMapper>());
		this->modelServer->Open();
		this->graphicsServer->Open();
		this->lightServer->Open();
		this->shadowServer->Open();
        // setup input subsystem
        //this->inputServer = InputServer::Create();
        //this->inputServer->Open();
        // setup frame timer
        this->timer.Start();
        this->time = 0.0;
        this->frameTime = 0.01;
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
/**
*/
void
SyncRenderApplication::OnConfigureDisplay()
{
    // display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->displayDevice->AdapterExists(adapter))
        {
            this->displayDevice->SetAdapter(adapter);
        }
    }
    // display mode
    DisplayMode displayMode;
    if (this->args.HasArg("-x"))
    {
        displayMode.SetXPos(this->args.GetInt("-x"));
    }
    if (this->args.HasArg("-y"))
    {
        displayMode.SetYPos(this->args.GetInt("-y"));
    }
    if (this->args.HasArg("-w"))
    {
        displayMode.SetWidth(this->args.GetInt("-w"));
    }
    if (this->args.HasArg("-h"))
    {
        displayMode.SetHeight(this->args.GetInt("-h"));
    }
    this->displayDevice->SetDisplayMode(displayMode);
    this->displayDevice->SetFullscreen(this->args.GetBool("-fullscreen"));
    this->displayDevice->SetAlwaysOnTop(this->args.GetBool("-alwaysontop"));
    this->displayDevice->SetVerticalSyncEnabled(this->args.GetBool("-vsync"));
    if (this->args.HasArg("-aa"))
    {
        this->displayDevice->SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }
    #if __XBOX360__
        this->displayDevice->SetAntiAliasQuality(AntiAliasQuality::Medium);
    #endif
}
//------------------------------------------------------------------------------
/**
*/
void
SyncRenderApplication::Close()
{
    n_assert(this->IsOpen());
    this->timer.Stop();
    //this->inputServer->Close();
    //this->inputServer = 0;
	this->frameServer = 0;
	this->shadowServer = 0;
	this->lightServer = 0;
	this->graphicsServer = 0;
	this->modelServer = 0;
	this->resourceManager = 0;
	this->sharedResourceServer = 0;
	this->vertexLayoutServer = 0;
	this->shapeRenderer = 0;
	this->shaderServer = 0;
	this->transformDevice = 0;
	this->displayDevice = 0;
	this->renderDevice = 0;
	this->animationServer = 0;
	this->textRenderer = 0;
	this->debugInterface->Close();
	this->debugInterface = 0;
	this->httpInterface->Close();
	this->httpInterface = 0;
    this->ioInterface->Close();
    this->ioInterface = 0;
    this->ioServer = 0;
    this->coreServer->Close();
    this->coreServer = 0;
    Application::Close();
}
//------------------------------------------------------------------------------
/**
*/
void
SyncRenderApplication::Run()
{
    n_assert(this->isOpen);
    //while (!(this->inputServer->IsQuitRequested() || this->IsQuitRequested()))
    //{
    //    // process input
    //    this->inputServer->BeginFrame();
    //    this->inputServer->OnFrame();
	while(true)
	{
        this->OnProcessInput();
        // update time
        this->UpdateTime();
        // run "game logic"
        this->OnUpdateFrame();
		// render frame
		this->OnRenderFrame();
        //this->inputServer->EndFrame();
        if (!this->displayDevice->IsFullscreen())
        {
            Core::SysFunc::Sleep(0.0);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void
SyncRenderApplication::OnProcessInput()
{
    // empty, override this method in a subclass
}
//------------------------------------------------------------------------------
/**
*/
void
SyncRenderApplication::OnUpdateFrame()
{
    // empty, override this method in a subclass
}
//------------------------------------------------------------------------------
/**
*/
void
SyncRenderApplication::UpdateTime()
{
    Time curTime = this->timer.GetTime();
    this->frameTime = curTime - this->time;
    this->time = curTime;
}
//------------------------------------------------------------------------------
void
SyncRenderApplication::OnRenderFrame()
{
	// empty, override this method in a subclass
}
	
} // namespace App
测试程序, 仍然还是一条线:
#include "stdneb.h"    
#include "apprender/syncrenderapplication.h"    
#include "coregraphics/memoryvertexbufferloader.h"    
#include "resources/resourceloader.h"    
#include "timing/time.h"    
using namespace App;   
using namespace Util;   
using namespace Math;   
using namespace CoreGraphics;   
using namespace Resources;   
ImplementNebulaApplication()   
class LineRenderApp : public SyncRenderApplication   
{   
	Ptr<VertexBuffer> vertexBuffer;   
	Ptr<ShaderInstance> shaderInstance;   
public:   
	bool Open()   
	{   
		if (SyncRenderApplication::Open())   
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
