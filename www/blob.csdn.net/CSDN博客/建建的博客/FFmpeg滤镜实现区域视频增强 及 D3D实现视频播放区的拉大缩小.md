# FFmpeg滤镜实现区域视频增强 及 D3D实现视频播放区的拉大缩小 - 建建的博客 - CSDN博客
2018年06月05日 18:38:59[纪建](https://me.csdn.net/u013898698)阅读数：66
个人分类：[ffmpeg学习](https://blog.csdn.net/u013898698/article/category/6692312)
# 1.区域视频增强
    FFmpeg滤镜功能十分强大，用滤镜可以实现视频的区域增强功能。
![QQ截图20161219210653](https://images2015.cnblogs.com/blog/979097/201612/979097-20161219212310588-1491204475.png)
    用eq滤镜就可以实现亮度、对比度、饱和度等的常用视频增强功能。
    推荐两篇写得不错的博文：
        （1）[ffmpeg综合应用示例（二）——为直播流添加特效 - 张晖的专栏 - 博客频道 - CSDN.NET](http://blog.csdn.net/nonmarking/article/details/48140653)；
        （2）[ffmpeg 滤镜及其效果 - 党玉涛 - 博客频道 - CSDN.NET](http://blog.csdn.net/dangxw_/article/details/49001413)
     第（1）篇博客对于如何用代码来写滤镜讲得比较清楚，第（2）篇则列出了许多滤镜写法的例子。
参考第（1）篇博客，滤镜的代码如下：
设置滤镜：
```
int FFmpeg_filter::apply_filters(AVFormatContext *ifmt_ctx)
{
    char args[512];
    int ret;
    AVFilterInOut *outputs = avfilter_inout_alloc();
    if (!outputs)
    {
        printf("Cannot alloc output\n");
        return -1;
    }
    AVFilterInOut *inputs = avfilter_inout_alloc();
    if (!inputs)
    {
        printf("Cannot alloc input\n");
        return -1;
    }
    AVFilterGraph *filter_graph = NULL;
    if (filter_graph)
        avfilter_graph_free(&filter_graph);
    filter_graph = avfilter_graph_alloc();
    if (!filter_graph)
    {
        printf("Cannot create filter graph\n");
        return -1;
    }
    /* buffer video source: the decoded frames from the decoder will be inserted here. */
    snprintf(args, sizeof(args),
        "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
        ifmt_ctx->streams[0]->codec->width, ifmt_ctx->streams[0]->codec->height, ifmt_ctx->streams[0]->codec->pix_fmt,
        ifmt_ctx->streams[0]->time_base.num, ifmt_ctx->streams[0]->time_base.den,
        ifmt_ctx->streams[0]->codec->sample_aspect_ratio.num, ifmt_ctx->streams[0]->codec->sample_aspect_ratio.den);
    ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
        args, NULL, filter_graph);
    if (ret < 0) {
        printf("Cannot create buffer source\n");
        return ret;
    }
    /* buffer video sink: to terminate the filter chain. */
    ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
        NULL, NULL, filter_graph);
    if (ret < 0) {
        printf("Cannot create buffer sink\n");
        return ret;
    }
    /* Endpoints for the filter graph. */
    outputs->name = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx = 0;
    outputs->next = NULL;
    inputs->name = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx = 0;
    inputs->next = NULL;
    const char *filter_descr = "null";
    const char *filter_mirror = "crop=iw/2:ih:0:0,split[left][tmp];[tmp]hflip[right];[left]pad=iw*2[a];[a][right]overlay=w";
    const char *filter_watermark = "movie=logo.png[wm];[in][wm]overlay=5:5[out]";
    const char *filter_negate = "negate[out]";
    const char *filter_edge = "edgedetect[out]";
    const char *filter_split4 = "scale=iw/2:ih/2[in_tmp];[in_tmp]split=4[in_1][in_2][in_3][in_4];[in_1]pad=iw*2:ih*2[a];[a][in_2]overlay=w[b];[b][in_3]overlay=0:h[d];[d][in_4]overlay=w:h[out]";
    const char *filter_vintage = "curves=vintage";
    const char *filter_brightness = "eq=brightness=0.5[out] ";    //亮度。The value must be a float value in range -1.0 to 1.0. The default value is "0". 
    const char *filter_contrast = "eq=contrast=1.5[out] ";        //对比度。The value must be a float value in range -2.0 to 2.0. The default value is "1". 
    const char *filter_saturation = "eq=saturation=1.5[out] ";    //饱和度。The value must be a float in range 0.0 to 3.0. The default value is "1". 
    //const char *filter_eq = "eq=contrast=1.0:brightness=-0.0:saturation=1.0 ";
    char filter_eq[512];
    float t_brightness_value = _brightness_value ;
    float t_contrast_value = _contrast_value ;
    float t_saturation_value = _saturation_value ;
    snprintf(filter_eq, sizeof(filter_eq), "eq=brightness=%f:contrast=%f:saturation=%f", t_brightness_value, t_contrast_value, t_saturation_value);
    printf("eq=brightness=%f:contrast=%f:saturation=%f \n", t_brightness_value, t_contrast_value, t_saturation_value);
    int x = 50 ;
    int y = 60 ;
    int iWidth = 300 ;
    int iHeight = 300 ;
    char filter_test[512];
    snprintf(filter_test, sizeof(filter_test), "[in]split[ori][tmp];[tmp]crop=%d:%d:%d:%d,eq=brightness=%f:contrast=%f:saturation=%f[eq_enhance];[ori][eq_enhance]overlay=%d:%d[out]", 
        iWidth, iHeight, x, y, t_brightness_value, t_contrast_value, t_saturation_value, x, y);
    switch(_filter)
    {
        case FILTER_NULL:
            filter_descr = "null";
            break;
        case FILTER_MIRROR:
            filter_descr = filter_mirror;
            break;
        case FILTER_WATERMARK:
            filter_descr = filter_watermark;
            break;
        case FILTER_NEGATE:
            filter_descr = filter_negate;
            break;
        case FILTER_EDGE:
            filter_descr = filter_edge;
            break;
        case FILTER_SPLIT4:
            filter_descr = filter_split4;
            break;
        case FILTER_VINTAGE:
            filter_descr = filter_vintage;
            break;
        case FILTER_BRIGHTNESS:
            filter_descr = filter_brightness;
            break;
        case FILTER_CONTRAST:
            filter_descr = filter_contrast;
            break;
        case FILTER_SATURATION:
            filter_descr = filter_saturation;
            break;
        case FILTER_EQ:
            filter_descr = filter_eq;
            break;
        case FILTER_TEST:
            filter_descr = filter_test;
            break;
        default:
            break;
    }
    
    if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_descr,
        &inputs, &outputs, NULL)) < 0)
        return ret;
    if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
        return ret;
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);
    return 0;
}
```
应用滤镜：
```
while (av_read_frame(pFormatCtx, packet) >= 0){
    EnterCriticalSection(&cs) ;
    if (packet->stream_index == videoindex){
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
        if (ret < 0){
            printf("Decode Error.\n");
            return -1;
        }
        if (got_picture){
            pFrame->pts = av_frame_get_best_effort_timestamp(pFrame);
            if (filter_change)
                apply_filters(pFormatCtx);
            filter_change = 0;
            /* push the decoded frame into the filtergraph */
            if (av_buffersrc_add_frame(buffersrc_ctx, pFrame) < 0) {
                printf("Error while feeding the filtergraph\n");
                break;
            }
            picref = av_frame_alloc();
            while (1) {
                ret = av_buffersink_get_frame_flags(buffersink_ctx, picref, 0);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                    break;
                if (ret < 0)
                    return ret;
                if (picref) {
                    img_convert_ctx = sws_getContext(picref->width, picref->height, (AVPixelFormat)picref->format, pCodecCtx->width, pCodecCtx->height, target_format, SWS_BICUBIC, NULL, NULL, NULL);
                    sws_scale(img_convert_ctx, (const uint8_t* const*)picref->data, picref->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
                    _d3d_video_render.Render_Texture(pFrameYUV->data[0],picref->width,picref->height) ;
                    sws_freeContext(img_convert_ctx);
                }
                av_frame_unref(picref);
            }
            SDL_Delay(10);
        }
    }
    av_free_packet(packet);
    LeaveCriticalSection(&cs) ;
}
```
apply_filters函数修改滤镜的设置。另外有两个比较重要的函数：av_buffersrc_add_frame函数和av_buffersink_get_frame_flags函数。至于区域增强，其实主要就是如下代码：
```
snprintf(filter_test, sizeof(filter_test), "[in]split[ori][tmp];[tmp]crop=%d:%d:%d:%d,eq=brightness=%f:contrast=%f:saturation=%f[eq_enhance];[ori][eq_enhance]overlay=%d:%d[out]", 
        iWidth, iHeight, x, y, t_brightness_value, t_contrast_value, t_saturation_value, x, y);
```
含义就是把输入流分成ori和tmp，把tmp从像素坐标（x,y）开始，以iWidth为宽，以iHeight为高的部分crop出来，使用eq滤镜后再叠加到ori对应位置。
# 2.D3D实现视频播放区的拉大缩小
    用D3D的纹理可以做出许多非常有意思的效果。使用纹理可以做各种变换轻松实现视频播放区的拉大缩小，视频旋转也可以非常轻松的实现，我没有做，不过只需要修改一下相机的向上方向就可以轻松实现90°的旋转，但要实现任意角度，则还需要稍作计算，我没有做，就不多说了。
初始化D3D:
```
BOOL CD3DVidRender::InitD3D_Texure(HWND hwnd, int img_width, int img_height, RENDER_DATA_TYPE data_type)
{
    if (!hwnd)
    {
        return false ;
    }
    D3DDEVTYPE deviceType = D3DDEVTYPE_HAL ;
    bool windowed = true ;
    HRESULT hr = 0;
    // Step 1: Create the IDirect3D9 object.
    IDirect3D9* d3d9 = 0;
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
    if( !d3d9 )
    {
        ::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
        return false;
    }
    // Step 2: Check for hardware vp.
    D3DCAPS9 caps;
    d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
    int vp = 0;
    if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    // Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
    D3DDISPLAYMODE        d3ddm;
    UINT adapter = D3DADAPTER_DEFAULT;
    IDirect3D9_GetAdapterDisplayMode(d3d9, adapter, &d3ddm);
    D3DPRESENT_PARAMETERS d3dpp;
    // 默认不使用多采样
    D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;
    if(d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, !windowed,
        D3DMULTISAMPLE_4_SAMPLES,
        NULL) == D3D_OK)
    {
        multiType = D3DMULTISAMPLE_4_SAMPLES;
    }
    d3dpp.BackBufferFormat             = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth            = img_width;
    d3dpp.BackBufferHeight           = img_height;
    d3dpp.BackBufferCount            = 1;
    d3dpp.MultiSampleType            = multiType;
    d3dpp.MultiSampleQuality         = 0;
    d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
    d3dpp.hDeviceWindow              = hwnd;
    d3dpp.Windowed                   = windowed;
    d3dpp.EnableAutoDepthStencil     = true; 
    d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
    d3dpp.Flags                      = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
    // Step 4: Create the device.
    hr = d3d9->CreateDevice(
        D3DADAPTER_DEFAULT, // primary adapter
        deviceType,         // device type
        hwnd,               // window associated with device
        vp,                 // vertex processing
        &d3dpp,             // present parameters
        &m_pDirect3DDevice);            // return created device
    if( FAILED(hr) )
    {
        // try again using a 16-bit depth buffer
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        hr = d3d9->CreateDevice(
            D3DADAPTER_DEFAULT,
            deviceType,
            hwnd,
            vp,
            &d3dpp,
            &m_pDirect3DDevice);
        if( FAILED(hr) )
        {
            d3d9->Release(); // done with d3d9 object
            ::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
            return false;
        }
    }
    d3d9->Release(); // done with d3d9 object
    _data_type = data_type ;
    m_Hwnd = hwnd ;
    if (!Setup_Texture(hwnd,m_pDirect3DDevice,img_width,img_height,data_type))
    {
        return false ;
    }
    return TRUE;  
}
```
设置纹理：
```
BOOL CD3DVidRender::Setup_Texture(HWND hwnd,IDirect3DDevice9* Device, int Width, int Height, RENDER_DATA_TYPE data_type)
{
    if (!Device)
    {
        return false ;
    }
    Device = m_pDirect3DDevice ;
    HRESULT hr = 0;
    float x_w = Width / 2 ;
    float y_h = Height / 2 ;
    hr = Device->CreateVertexBuffer(
        4 * sizeof(MultiTexVertex), 
        D3DUSAGE_WRITEONLY,
        MultiTexVertex::FVF,
        D3DPOOL_MANAGED,
        &QuadVB,
        0);
    MultiTexVertex* v = 0;
    QuadVB->Lock(0, 0, (void**)&v, 0);
    v[0] = MultiTexVertex(-20.0f,  20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[1] = MultiTexVertex( 20.0f,  20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    v[2] = MultiTexVertex( 20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    v[3] = MultiTexVertex(-20.0f, -20.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    QuadVB->Unlock();
    RECT rc ;
    GetClientRect(hwnd,&rc) ;
    float v_z = (float)Width / (float)rc.right ;
    D3DXMATRIX P;
    D3DXMatrixPerspectiveFovLH(&P,    
        D3DX_PI * 0.5f,
        1.0f,
        1.0f,        //近裁减面到坐标原点的距离
        1000.0f    //远裁减面到原点的距离
        );
    Device->SetTransform(D3DTS_PROJECTION, &P);
    Device->SetRenderState(D3DRS_LIGHTING, false);
    m_RadiusFirst = m_Radius = 20 ;
    m_AngleFirst = m_Angle  = (3.0f * D3DX_PI) / 2.0f;
    D3DXVECTOR3 position( x_Cam, y_Cam, sinf(m_Angle) * m_Radius);
    D3DXVECTOR3 target(x_Cam, y_Cam, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);//计算取景变换矩阵
    m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &V);//取景变换
    if(data_type == RENDER_DATA_TYPE_YUV420P)
    {
        ID3DXBuffer* shader      = 0;
        ID3DXBuffer* errorBuffer = 0;
        hr = D3DXCompileShaderFromFile(
            "E:\\workspace\\Shader\\SDKTexShaderDemo\\SDKTexShaderDemo\\ps_yuv420p.txt",
            0,
            0,
            "Main", // entry point function name
            "ps_2_0",
            D3DXSHADER_DEBUG, 
            &shader,
            &errorBuffer,
            &MultiTexCT);
        if( errorBuffer )
        {
            ::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
            errorBuffer->Release() ;
        }
        if(FAILED(hr))
        {
            ::MessageBox(0, "D3DXCompileShaderFromFile() - FAILED", 0, 0);
            return false;
        }
        hr = Device->CreatePixelShader((DWORD*)shader->GetBufferPointer(), &MultiTexPS);
        if(FAILED(hr))
        {
            ::MessageBox(0, "CreateVertexShader - FAILED", 0, 0);
            return false;
        }
        if (shader)
        {
            shader->Release() ;
        }
        Device->CreateTexture ( Width, Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &YTex, NULL ) ;
        Device->CreateTexture ( Width / 2, Height / 2, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &UTex, NULL ) ;
        Device->CreateTexture ( Width / 2, Height / 2, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &VTex, NULL ) ;
        D3DXHANDLE YTexHandle    = MultiTexCT->GetConstantByName(0, "YTex");
        D3DXHANDLE UTexHandle    = MultiTexCT->GetConstantByName(0, "UTex");
        D3DXHANDLE VTexHandle    = MultiTexCT->GetConstantByName(0, "VTex");
        UINT count;
        MultiTexCT->GetConstantDesc(YTexHandle, &YTexDesc, &count);
        MultiTexCT->GetConstantDesc(UTexHandle, &UTexDesc, &count);
        MultiTexCT->GetConstantDesc(VTexHandle, &VTexDesc, &count);
        MultiTexCT->SetDefaults(Device);
    }
    else
    {
        Device->CreateTexture ( Width, Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &YTex, NULL ) ;
    }
    return true;
}
```
以上代码中，投影变换部分需要做一点说明，
```
MultiTexVertex* v = 0;
QuadVB->Lock(0, 0, (void**)&v, 0);
v[0] = MultiTexVertex(-20.0f,  20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
v[1] = MultiTexVertex( 20.0f,  20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
v[2] = MultiTexVertex( 20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
v[3] = MultiTexVertex(-20.0f, -20.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
QuadVB->Unlock();
RECT rc ;
GetClientRect(hwnd,&rc) ;
float v_z = (float)Width / (float)rc.right ;
D3DXMATRIX P;
D3DXMatrixPerspectiveFovLH(&P,    
    D3DX_PI * 0.5f,
    1.0f,
    1.0f,        //近裁减面到坐标原点的距离
    1000.0f    //远裁减面到原点的距离
    );
Device->SetTransform(D3DTS_PROJECTION, &P);
```
我把纹理坐标设成了宽慰40（20X2）、位于z=0处的正方形，把D3DXMatrixPerspectiveFovLH的第3个参数设为了1，这样可以图像就会与hwnd指向的窗口保持统一宽高比，事实上应该是可以通过调整参数使得图像按自身的比例置于窗口的中间位置的，但我看了很多网上的博文，做了许多尝试都没有达到理想的效果，如果有能实现的，还请指教。另外，我把D3DXMatrixPerspectiveFovLH的第2个参数设为90°（D3DX_PI * 0.5f），这样就可以知道当相机与纹理的距离为20的时候，图像刚好完整填充窗口。这一块若有疑问，建议最好参考一下网上的资料。
接下来要做电子放大的效果就很容易了，只需要修改相机与纹理的距离，做取景变换就可以轻松实现电子放大的拉大缩小效果，代码如下：
```
void CD3DVidRender::ZoomIn()
{
    m_Radius += m_RadiusFirst / 25.0f;
    if (m_Radius >= 200)
    {
        m_Radius = 200 ;
    }
    D3DXVECTOR3 position( -x_curCam, y_curCam, sinf(m_Angle) * m_Radius);
    D3DXVECTOR3 target(-x_curCam, y_curCam, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);//计算取景变换矩阵
    m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &V);//取景变换
}
void CD3DVidRender::ZoomOut()
{
    m_Radius -= m_RadiusFirst / 20.0f;
    if (m_Radius <= 1.1f)
    {
        m_Radius = 1.1f ;
    }
    D3DXVECTOR3 position( -x_curCam, y_curCam, sinf(m_Angle) * m_Radius);
    D3DXVECTOR3 target(-x_curCam, y_curCam, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);//计算取景变换矩阵
    m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &V);//取景变换
}
```
完整工程代码：[http://download.csdn.net/download/qq_33892166/9725905](http://download.csdn.net/download/qq_33892166/9725905)
