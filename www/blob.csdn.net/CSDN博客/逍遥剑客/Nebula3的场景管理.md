# Nebula3的场景管理 - 逍遥剑客 - CSDN博客
2008年10月20日 13:18:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1742
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
N3的场景管理最为核心的一个类是GrphicsServer, 它包含一些"stage"和"View".
Stage把图形实体(模型, 摄像机, 灯光)进行分类渲染. 它的主要工作是在连接的图形实体间加速可见性查询. 不同的可见性查询由不同的Stage子类来实现. N3会提供了一些不同用途的Stage子类, 但你也可以根据程序需要自己来实现可见性查询机制. 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081020/N3_StageBuilder.JPG)
可见性查询适用于这些实体:
- Camera->Light: 查找对于指定摄像机可见的所有灯光
- Camera->Model: 查找对于指定摄像机可见的所有模型
- Light->MOdel: 查找被指定光源照射到的所有模型
这些可见性查询在图形实体间建立了一些所谓的"可见性链接", 再利用低级的渲染子系统来加速渲染. 
要渲染一个Stage的内容, 需要至少一个View对象. 一个View对象通过绑定一个摄像机实体把Stage渲染到一个render target. 可以并存任意数目的View, 也可能都被绑定到任意Stage. 此外, View对象之间可能存在依赖关系(结果就是一个View对象会在渲染自身时首先请求它所依赖的View对象). 
图形实体表示了可以被连接到Stage的一个最小图形对象, 它分为以下三种:
- ModelEntity: 一个可见的模型实例
- LightEntity: 一个光源
- CameraEntity: 一个摄像机
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081020/N3_GraphicsEntity.JPG)
可见性查询使图形实体间形成一种双向的链接关系. 一个CameraEntity链接到所有对于这个摄像机来说可见的ModelEntity和LightEntity. 因为可见性链接是双向的, 所以ModelEntity和LightEntity也知道它们对于哪个摄像机可见. LightEntity有它们影响到的ModelEntity的链接, ModelEntity也知道它们被哪个光源照亮.
==========================================================
N3 画个东西真简单, 想画个模型, 创建出来设置一下位置扔给Stage就好了
            this->model = ModelEntity::Create();
            this->model->SetTransform(matrix44::translation(0.0f, 3.0f, 0.0f));
            this->model->SetResourceId(ResourceId("mdl:examples/eagle.n2"));
            this->stage->AttachEntity(this->model.upcast<GraphicsEntity>());
模型是黑的? 再往场景里扔个灯就好了:
            // attach a light entity 
            matrix44 lightTransform = matrix44::multiply(matrix44::scaling(100.0f, 100.0f, 100.0f), matrix44::lookatrh(point(20.0f, 20.0f, 20.0f), point::origin(), vector::upvec()));
            this->lightEntity = SpotLightEntity::Create();
            this->lightEntity->SetCastShadows(true);
            this->lightEntity->SetTransform(lightTransform);
            this->lightEntity->SetColor(float4(4.0f, 2.0f, 1.0f, 1.0f));        
            this->stage->AttachEntity(this->lightEntity.upcast<GraphicsEntity>());
想控制的话, 再扔个摄像机进去就OK了.......
        GraphicsServer* gfxServer = GraphicsServer::Instance();
        // setup the camera util object 
        this->mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(0.0f, 0.0f, 10.0f), vector(0.0f, 1.0f, 0.0f));
        // setup a stage 
        this->stage = gfxServer->CreateStage(StringAtom("DefaultStage"), SimpleStageBuilder::Create());
        // attach a camera to the stage 
        this->cameraEntity = CameraEntity::Create();
        cameraEntity->SetTransform(this->mayaCameraUtil.GetCameraTransform());
        this->stage->AttachEntity(cameraEntity.upcast<GraphicsEntity>());
        // setup a default view 
        this->view = gfxServer->CreateView(View::RTTI, StringAtom("DefaultView"), true);
        this->view->SetStage(this->stage);        
        this->view->SetFrameShader(FrameServer::Instance()->GetFrameShaderByName(ResourceId(DEFAULT_FRAMESHADER_NAME)));
        this->view->SetCameraEntity(cameraEntity);
别忘了处理输入事件:
可以参考ViewerApplication::OnProcessInput().
