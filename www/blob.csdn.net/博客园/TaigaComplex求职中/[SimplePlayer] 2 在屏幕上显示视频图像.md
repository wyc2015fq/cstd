# [SimplePlayer] 2. 在屏幕上显示视频图像 - TaigaComplex求职中 - 博客园







# [[SimplePlayer] 2. 在屏幕上显示视频图像](https://www.cnblogs.com/TaigaCon/p/9622016.html)





我们这里采用SDL（本文所用版本为SDL2.0.5）来进行图像输出，SDL在进行图像渲染时一般采用的会是direct3D或者opengl，SDL对它们进行了封装，不过我们这里只讨论SDL的使用，并不会去涉及这些底层实现。尽管如此，我们还是有必要了解其中一些基本概念，这些概念能帮助我们很好地理解所使用的SDL api的实际作用。

其中最重要的一个概念就是render，中文译为渲染，[rendering](https://en.wikipedia.org/wiki/Rendering_(computer_graphics))在计算机图像领域指的就是图像合成（image synthesis）。通俗一点的解释就是：render指的是把2D图像（图形）或者3D模型变成所展示出来的图像的过程，实际进行这种转换操作的物件被称为renderer（渲染器）。在SDL中，renderer主要处理的是2D的数据，其中包括
- 图形：[画点](https://wiki.libsdl.org/SDL_RenderDrawPoint)、[画线](https://wiki.libsdl.org/SDL_RenderDrawLine)、[画面](https://wiki.libsdl.org/SDL_RenderDrawRect)、[笔触上色](https://wiki.libsdl.org/SDL_SetRenderDrawColor)、[填充颜色](https://wiki.libsdl.org/SDL_RenderFillRect)等。
- 图像：[裁剪](https://wiki.libsdl.org/SDL_RenderSetClipRect)、[旋转](https://wiki.libsdl.org/SDL_RenderCopyEx)等。

不过本文只用到renderer最基本的功能，即把图像输出到窗口。在SDL中，为了方便renderer的实现，renderer所处理的对象被规定为texture（纹理），因此图像需要先被包装成texture，texture会被render成rendered image，然后就可以输出到window。

![image](https://img2018.cnblogs.com/blog/421096/201809/421096-20180910192125551-533246998.png)





# 初始化

在使用SDL的函数之前，需要先调用SDL_Init来进行初始化，由于本文的目的是进行视频图像输出，因此需要指定flag为SDL_INIT_VIDEO来初始化视频子系统。
    if(SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "SDL init video failed\n");
        return -1;
    }


接下来创建window、renderer、texture。在创建texture时需要指定输入的图像像素格式，通常视频的像素格式都是yuv420p，对应的flag为SDL_PIXELFORMAT_IYUV。对于会频繁更换的视频图像，还需要指定texture为SDL_TEXTUREACCESS_STREAMING。
    window = SDL_CreateWindow("Simple Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEF_WIDTH, DEF_HEIGHT, 0);
    if(!window){
        fprintf(stderr, "SDL create window failed\n");
        return -1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
        fprintf(stderr, "SDL create renderer failed\n");
        return -1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, DEF_WIDTH, DEF_HEIGHT);
    if(!renderer){
        fprintf(stderr, "SDL create renderer failed\n");
        return -1;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


创建用于存储图像像素数据的缓冲区，并打开yuv文件，以供后续读取图像
    YPlane = (unsigned char *)malloc(DEF_WIDTH*DEF_HEIGHT);
    UPlane = (unsigned char *)malloc(DEF_WIDTH*DEF_HEIGHT/4);
    VPlane = (unsigned char *)malloc(DEF_WIDTH*DEF_HEIGHT/4);

    pFile = fopen(argv[1], "rb");
    if(pFile==NULL)
        return -1;





# 显示

循环读取图像进入缓冲区，然后把图像传入texture进行包装、渲染、最后输出，每帧间隔为1/24秒。对于视频子系统来说，还有一个必不可少的步骤，就是在循环中调用[SDL_PumpEvents](https://wiki.libsdl.org/SDL_PumpEvents)。SDL_PumpEvents如其名称，主要的功能是推动event队列以进行队列状态的更新，不过它还有一个作用是进行视频子系统的设备状态更新，如果不调用这个函数，所显示的视频会在大约10秒后丢失色彩。
    while(1){
        sizeY = fread(YPlane, 1, DEF_WIDTH*DEF_HEIGHT, pFile);
        sizeU = fread(UPlane, 1, DEF_WIDTH*DEF_HEIGHT/4, pFile);
        sizeV = fread(VPlane, 1, DEF_WIDTH*DEF_HEIGHT/4, pFile);
        if(!sizeY||!sizeU||!sizeV)
            break;
        if(0!=SDL_UpdateYUVTexture(texture, NULL, YPlane, DEF_WIDTH, UPlane, DEF_WIDTH/2, VPlane, DEF_WIDTH/2)){
            fprintf(stdout, "Render Update Texture failed, reason: %s\n", SDL_GetError());
        }
        SDL_RenderCopyEx(renderer, texture, NULL, NULL, 0, NULL, 0);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FRAMERATE);

        SDL_PumpEvents();
    }




# 退出

退出前的收尾处理
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(YPlane);
    free(UPlane);
    free(VPlane);
    fclose(pFile);











