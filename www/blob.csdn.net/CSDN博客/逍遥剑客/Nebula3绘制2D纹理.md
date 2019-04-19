# Nebula3绘制2D纹理 - 逍遥剑客 - CSDN博客
2008年11月30日 23:13:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2000
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
上次已经绘制过[基本图元](http://blog.csdn.net/xoyojank/archive/2008/09/23/2964086.aspx)了, 这次只不过要贴张图而已.....
本来我想用Graphics的Model渲染流程来做, 不过这一层太高级了, 都是什么场景管理资源映射之类的
做低级的事情, 就要用低级的API嘛
图形渲染的底层是CoreGraphics, 这个层我不打算再单独写(翻译)一篇了, 因为都是Direct3D概念的一些抽象. 也就是说D3D用熟了基本上一看就明白(用GL的我就不清楚啦, 嘿嘿, N3的作者都放弃用GL去实现@_@). 
还记得D3D Tutorial中的Textured例子不? 需要的东西有带纹理坐标的点, 纹理. N3中也一样, 不过, 这里没法用固定管线了. 
N3的设计的时候就放弃了固定管线(多么明智呀, 别喷我-_-, 我只会shader.......), 所以在这之前我们要先写一个shader来进行绘制. 
因为我们只是进行简单的演示, 就尽量简单了, 写一个2D的纹理绘制, 你可以用来做UI:
//------------------------------------------------------------------------------ 
//  texture2d.fx 
//  texture shader for 2D(UI) 
//  (C) xoyojank 
//------------------------------------------------------------------------------ 
float2 halfWidthHeight  : HalfWidthHeight;
texture diffMap     : DiffMap0;
sampler diffMapSampler = sampler_state
{
    Texture = <diffMap>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = None;
};
struct VS_INPUT
{
    float3 pos  : POSITION;
    float2 uv       : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 pos  : POSITION;
    float2 uv       : TEXCOORD;
};
//------------------------------------------------------------------------------ 
/**
*/
VS_OUTPUT
VertexShaderFunc(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.pos.xy = float2(input.pos.x - halfWidthHeight.x, halfWidthHeight.y - input.pos.y) / halfWidthHeight;
    output.pos.zw = float2(input.pos.z, 1.0f);
    output.uv = input.uv;
    
    return output;
}
//------------------------------------------------------------------------------ 
/**
*/
float4
PixelShaderFunc(float2 uv : TEXCOORD0) : COLOR
{
    return tex2D(diffMapSampler, uv);
}
//------------------------------------------------------------------------------ 
/**
*/
technique Default
{
    pass p0
    {
        ColorWriteEnable  = RED|GREEN|BLUE|ALPHA;
        ZEnable           = False;
        ZWriteEnable      = False;
        StencilEnable     = False;
        FogEnable         = False;
        AlphaBlendEnable  = True;
        SrcBlend          = SrcAlpha;
        DestBlend         = InvSrcAlpha;
        AlphaTestEnable   = False;
        ScissorTestEnable = False;
        CullMode          = CW;        
        VertexShader = compile vs_3_0 VertexShaderFunc();
        PixelShader = compile ps_3_0 PixelShaderFunc();
    }
}
值得一提的是CullMode = CW, 为什么? 因为N3用的右手坐标系, 这点又跟D3D不一样了........为什么呢? 难道写MAYA跟MAX的插件的时候比较省事?
还是要跟上一次一样设置顶点格式并载入VertexBuffer:
            // vertex 
            Array<VertexComponent> vertexComponents;
            vertexComponents.Append(VertexComponent(VertexComponent::Position, 0, VertexComponent::Float3));
            vertexComponents.Append(VertexComponent(VertexComponent::TexCoord, 0, VertexComponent::Float2));
            float vertex[4][5] = {
                {0.0f,  0.0f,   0.0f,   0.0f, 0.0f},
                {0.0f,  256.0f, 0.0f,   0.0f, 1.0f}, 
                {256.0f,0.0f,   0.0f,   1.0f, 0.0f}, 
                {256.0f,256.0f, 0.0f,   1.0f, 1.0f}
            };
            vertexBuffer = VertexBuffer::Create();
            Ptr<MemoryVertexBufferLoader> vbLoader = MemoryVertexBufferLoader::Create();
            vbLoader->Setup(vertexComponents, 4, vertex, 4 * 5 * sizeof(float));
            vertexBuffer->SetLoader(vbLoader.upcast<ResourceLoader>());
            vertexBuffer->Load();
            vertexBuffer->SetLoader(NULL);
纹理的创建其实跟顶点差不多, 因为它都是属于资源的一种, 详见[Nebula3资源子系统](http://blog.csdn.net/xoyojank/archive/2008/10/30/3179377.aspx)
            // texture 
            texture = Texture::Create();
            texture->SetResourceId(ResourceId("bin:razor.jpg"));
            texture->SetLoader(StreamTextureLoader::Create());
            texture->Load();
            texture->SetLoader(NULL);
shader的加载跟上一次一样, 只是参数不同:
            // shader 
            this->shaderInstance = this->shaderServer->CreateShaderInstance(ResourceId("shd:texture2d"));
            Ptr<ShaderVariable> halfWidthHeight = this->shaderInstance->GetVariableBySemantic(ShaderVariable::Semantic("HalfWidthHeight"));
            float2 halfWH = float2(this->renderDevice->GetDefaultRenderTarget()->GetWidth(), this->renderDevice->GetDefaultRenderTarget()->GetHeight()) * 0.5f;
            halfWidthHeight->SetFloatArray(&halfWH.x(), 2);
            Ptr<ShaderVariable> diffMap = this->shaderInstance->GetVariableBySemantic(ShaderVariable::Semantic("DiffMap0"));
            diffMap->SetTexture(texture);
绘制嘛, 当然改成矩形了, 图片可贴不到一跟线上:
        this->renderDevice->BeginFrame();
        this->renderDevice->BeginPass(this->renderDevice->GetDefaultRenderTarget(), this->shaderInstance);
        PrimitiveGroup primGroup;
        primGroup.SetBaseVertex(0);
        primGroup.SetNumVertices(4);
        primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);
        
        this->renderDevice->SetVertexBuffer(this->vertexBuffer);
        this->renderDevice->SetPrimitiveGroup(primGroup);
        this->renderDevice->Draw();
        this->renderDevice->EndPass();
        this->renderDevice->EndFrame();
        this->renderDevice->Present();
上图:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081130/N3_Texture_Quad.JPG)
