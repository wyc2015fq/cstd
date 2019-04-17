# C# 使用SDL2进行视频播放窗口截图和字幕添加 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年03月06日 14:51:46[boonya](https://me.csdn.net/boonya)阅读数：957










使用SDL2进行视频播放窗口截图和字幕添加

SDL API查看：[https://wiki.libsdl.org/APIByCategory](https://wiki.libsdl.org/APIByCategory)

#### 视频截图

```
/// <summary>
    /// SDL2截图操作类
    /// </summary>
    public unsafe class SDLScreenshot
    {
        IntPtr window;// 窗口对象
        IntPtr renderer;// 播放窗口的渲染器（来自于已初始化的播放窗口渲染器）


        public SDLScreenshot(IntPtr window, IntPtr renderer)
        {
            this.window = window;
            this.renderer = renderer;
        }


        /// <summary>
        /// 保存截图
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <param name="path"></param>
        public void SaveBMP(int width, int height,string path)
        {
            // 判断渲染器是否初始化
            if (renderer == IntPtr.Zero)
            {
                Console.WriteLine("renderer is null ,please call Init() method.");
                return;
            }
            uint Rmask=0x00FF0000, Gmask = 0x0000FF00, Bmask = 0x000000FF, Amask = 0x00000000;
            // 获取图像数据
            SDL.SDL_Surface* surface= (SDL.SDL_Surface*)SDL.SDL_CreateRGBSurface(0, width, height, 32, Rmask, Gmask, Bmask, Amask);
            //设置纹理的数据
            SDL.SDL_Rect destrect;
            destrect.x = 0;
            destrect.y = 0;
            destrect.w = width;
            destrect.h = height;


            // 读取并渲染图像数据
            SDL.SDL_RenderReadPixels(renderer, ref destrect, SDL.SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);


            //保存图片
            int i = SDL.SDL_SaveBMP((IntPtr)surface, path);
            if (i != 0)
            {
                Console.WriteLine("screenshot failed." + SDL.SDL_GetError());
            }


            SDL.SDL_FreeSurface((IntPtr)surface);
            //SDL.SDL_RenderClear(renderer);
            //SDL.SDL_DestroyRenderer(renderer);


        }


        /// <summary>
        /// 加载截图
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <param name="path"></param>
        public void LoadBMP(int width, int height, string path)
        {
            // 判断渲染器是否初始化
            if (renderer == IntPtr.Zero)
            {
                Console.WriteLine("renderer is null ,please call Init() method.");
                return;
            }
            // 加载图片
            IntPtr surface = SDL.SDL_LoadBMP(path);
            if (surface == IntPtr.Zero)
            {
                Console.WriteLine("load bmp failed." + SDL.SDL_GetError());
                return;
            }
            IntPtr texture = SDL.SDL_CreateTextureFromSurface(renderer, surface);
            if (texture == IntPtr.Zero)
            {
                Console.WriteLine("create texture failed." + SDL.SDL_GetError());
                return;
            }
            SDL.SDL_FreeSurface(surface);


            //设置纹理的数据
            SDL.SDL_Rect destrect;
            destrect.x = 0;
            destrect.y = 0;
            destrect.w = width;
            destrect.h = height;


            SDL.SDL_Rect srcrect = destrect;


            //SDL.SDL_RenderClear(renderer);
            SDL.SDL_RenderCopy(renderer, texture, ref srcrect, ref destrect);
            SDL.SDL_RenderPresent(renderer);


            //SDL.SDL_Delay(20);


            SDL.SDL_DestroyTexture(texture);
            //SDL.SDL_DestroyRenderer(renderer);
            //SDL.SDL_DestroyWindow(screen);
            //Quit SDL    
            //SDL.SDL_Quit();
        }
    }
```

播放测试代码：

```
if (isSaveScreenshot)
                            {
                                SDLScreenshot screenshot = new SDLScreenshot(sdlVideo.window, sdlVideo.sdlrenderer);
                                screenshot.SaveBMP(nvVideoframe.VideoFrame->width, nvVideoframe.VideoFrame->height, "screenshot.bmp");
                                isSaveScreenshot = false;
                 }
```

测试效果图：

![](https://img-blog.csdn.net/20180309175958841)


注：此处截图是直接获取的播放窗口的图像像素来实现的。

#### 视频字幕

```
/// <summary>
    /// SDL2字幕显示类
    /// </summary>
    public unsafe class SDLTTF
    {
        IntPtr renderer;// 播放窗口的渲染器（来自于已初始化的播放窗口渲染器）

        public SDLTTF(IntPtr renderer)
        {
            this.renderer = renderer;
        }

        /// <summary>
        /// 展示字幕文字
        /// </summary>
        /// <param name="text"></param>
        public void ShowText(string ttfPath, int fontSize,string text)
        {
            // 初始化 ttf
            if (SDL_ttf.TTF_Init() < 0)
            {
                Console.WriteLine("SDL_ttf.TTF_Init() failed.");
                return;
            }
            // 是否初始化完成
            int was_init = SDL_ttf.TTF_WasInit();

            if (was_init == 1)
                // SDL_ttf was already initialized
                Console.WriteLine("SDL_ttf was already initialized");
            else if (was_init == 0)
                // SDL_ttf was not already initialized
                Console.WriteLine("SDL_ttf was not already initialized");

            // 判断是否初始化
            if (renderer == IntPtr.Zero)
            {
                Console.WriteLine("Not initialized by  SDL_ttf.TTF_Init() ,please call Init() method.");
                return;
            }

            //如：打开ttfPath=simfang.ttf 字库，设字体为fontSize=20号
            IntPtr font = SDL_ttf.TTF_OpenFont(ttfPath, fontSize);
            if (font == IntPtr.Zero)
            {
                Console.WriteLine("open font failed." + SDL.SDL_GetError());
                return;
            }

            // 设置文字颜色
            SDL.SDL_Color color;
            color.a = 255;
            color.r = 255;
            color.g = 255;
            color.b = 255;

            // 渲染文字效果
            //IntPtr surface = SDL_ttf.TTF_RenderUTF8_Blended(font, text, color);
            IntPtr surface = SDL_ttf.TTF_RenderUNICODE_Blended(font, text, color);
            if (surface == IntPtr.Zero)
            {
                Console.WriteLine("show surface failed." + SDL.SDL_GetError());
            }
            IntPtr texture = SDL.SDL_CreateTextureFromSurface(renderer, surface);
            if (texture == IntPtr.Zero)
            {
                Console.WriteLine("create texture failed." + SDL.SDL_GetError());
            }
            SDL.SDL_FreeSurface(surface);
            // 关闭字体
            SDL_ttf.TTF_CloseFont(font);

            // 停止显示
            SDL_ttf.TTF_Quit();

            //设置纹理的数据
            SDL.SDL_Rect destrect;
            destrect.x = 0;
            destrect.y = 0;
            destrect.w = text.Length * 20;
            destrect.h = 20;

            SDL.SDL_Rect srcrect = destrect;

            SDL.SDL_RenderClear(renderer);
            SDL.SDL_RenderCopy(renderer, texture, ref srcrect, ref destrect);
            SDL.SDL_RenderPresent(renderer);
            SDL.SDL_DestroyTexture(texture);
            SDL.SDL_DestroyRenderer(renderer);
        }
    }
```

事件测试字幕添加：

需要的引用库下载：[https://www.libsdl.org/projects/SDL_ttf/](https://www.libsdl.org/projects/SDL_ttf/)

```
/// <summary>
        /// 字幕叠加****需要添加三个dll库：SDL2_ttf.dll 、libfreetype-6.dll 、zlib1.dll
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void mbtnAddFontText_Click(object sender, EventArgs e)
        {
            Console.WriteLine("叠加字幕...............");

            sdlTTF = new SDLTTF(sdlVideo.sdlrenderer);
            // 中英文都需要兼容
            string text = "Hello 世界!";
            // 设置一个字体库并设置字体大小和显示文字内容
            sdlTTF.ShowText("simkai.ttf",12, text);
        }
```

测试效果图：

![](https://img-blog.csdn.net/20180309153610512)


如果是播放过程中显示字幕一定要在视频渲染完成后渲染字幕，如下面工具类的方法：

```
/// <summary>
        /// 播放视频
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <param name="pixels"></param>
        /// <param name="pixelsSize"></param>
        /// <param name="pitch"></param>
        /// <returns></returns>
        public int SDL_Display(int width, int height, IntPtr pixels, int pixelsSize,
            int pitch)
        {
            lock (this)
            {
                while (isPause)
                {
                    SDL.SDL_Delay(20);//延迟播放
                }

                #region SDL 视频数据渲染播放
                //设置纹理的数据
                sdlrect.x = 0;
                sdlrect.y = 0;
                sdlrect.w = width;
                sdlrect.h = height;
                SDL.SDL_UpdateTexture(sdltexture, ref sdlrect, pixels, pitch);
                //SDL.SDL_UpdateTexture(sdltexture, IntPtr.Zero, pixels, pitch);//此处代码导致播放窗口绿色阴影
                //复制纹理信息到渲染器目标
                SDL.SDL_RenderClear(sdltexture);
                //SDL.SDL_Rect srcRect = sdlrect;
                //SDL.SDL_RenderCopy(sdlrenderer, sdltexture, ref srcRect, ref sdlrect);

                SDL.SDL_RenderCopy(sdlrenderer, sdltexture, IntPtr.Zero, IntPtr.Zero);
                //字幕渲染显示-特别提醒：此处必须放置于视频渲染之后，否则字幕不会显示
                if (ttfText!=null&&!ttfText.Equals(""))
                {
                    RenderToShowTTF(ttfText);
                }
                //else
                //{
                //    RenderToShowTTF( "未设置字幕内容");
                //}
                //视频渲染显示
                SDL.SDL_RenderPresent(sdlrenderer);
                //SDL.SDL_Delay(40);
                //SDL.SDL_PollEvent(out sdlevent);
                //switch (sdlevent.type)
                //{
                //    case SDL.SDL_EventType.SDL_QUIT:
                //        SDL.SDL_Quit();
                //        return -1;
                //    default:
                //        break;
                //}
                return 0;
            }


            //SDL.SDL_RenderClear(sdlrenderer);
            //SDL.SDL_RenderCopy(sdlrenderer, sdltexture, ref srcRect, ref sdlrect);
            //SDL.SDL_RenderPresent(sdlrenderer);
            ////Delay 40ms  
            //SDL.SDL_Delay(40);
            #endregion


            //#region SDL 视频数据渲染播放
            //////设置纹理的数据
            ////sdlrect.x = 0;
            ////sdlrect.y = 0;
            ////sdlrect.w = width;
            ////sdlrect.h = height;
            ////SDL.SDL_UpdateTexture(sdltexture, ref sdlrect, pixels, pitch);
            //////复制纹理信息到渲染器目标
            ////SDL.SDL_Rect srcRect = sdlrect;
            ////SDL.SDL_RenderCopy(sdlrenderer, sdltexture, ref srcRect, ref sdlrect);
            //////视频渲染显示
            ////SDL.SDL_RenderPresent(sdlrenderer);
            //////SDL.SDL_Delay(40);
            ////SDL.SDL_PollEvent(out sdlevent);
            ////switch (sdlevent.type)
            ////{
            ////    case SDL.SDL_EventType.SDL_QUIT:
            ////        SDL.SDL_Quit();
            ////        return -1;
            ////    default:
            ////        break;
            ////}
            ////return 0;
            //#endregion
        }

        /// <summary>
        ///  设置字幕显示内容
        /// </summary>
        /// <param name="ttfPath"></param>
        /// <param name="fontSize"></param>
        public void SDL_TTF_TEXT(string ttfPath, string text, int fontSize)
        {
            this.ttfPath = ttfPath;
            this.ttfText = text;
            this.ttfFontSize = fontSize;
        }

        /// <summary>
        /// 渲染字幕
        /// </summary>
        /// <param name="text"></param>
        private void RenderToShowTTF(string text)
        {
            // 初始化 ttf
            if (SDL_ttf.TTF_Init() < 0)
            {
                Console.WriteLine("SDL_ttf.TTF_Init() failed.");
                return;
            }
            // 是否初始化完成
            int was_init = SDL_ttf.TTF_WasInit();

            if (was_init == 1)
                // SDL_ttf was already initialized
                Console.WriteLine("SDL_ttf was already initialized");
            else if (was_init == 0)
                // SDL_ttf was not already initialized
                Console.WriteLine("SDL_ttf was not already initialized");

            //如：打开ttfPath=simfang.ttf 字库，设字体为fontSize=20号
            IntPtr font = SDL_ttf.TTF_OpenFont(ttfPath, ttfFontSize);
            if (font == IntPtr.Zero)
            {
                Console.WriteLine("open font failed." + SDL.SDL_GetError());
                return;
            }

            // 设置文字字体
            SDL_ttf.TTF_SetFontStyle(font, SDL_ttf.TTF_STYLE_BOLD);

            // 设置文字颜色
            SDL.SDL_Color color;
            color.a = 255;
            color.r = 255;
            color.g = 255;
            color.b = 255;

            // 渲染文字效果
            //IntPtr surface = SDL_ttf.TTF_RenderText_Blended(font, text, color);
            IntPtr surface = SDL_ttf.TTF_RenderUTF8_Blended(font, text, color);
            //IntPtr surface = SDL_ttf.TTF_RenderUNICODE_Blended(font, text, color);
            if (surface == IntPtr.Zero)
            {
                Console.WriteLine("show surface failed." + SDL.SDL_GetError());
            }
            IntPtr texture = SDL.SDL_CreateTextureFromSurface(sdlrenderer, surface);
            if (texture == IntPtr.Zero)
            {
                Console.WriteLine("create texture failed." + SDL.SDL_GetError());
            }

            SDL.SDL_FreeSurface(surface);
            // 关闭字体
            SDL_ttf.TTF_CloseFont(font);

            // 停止显示
            SDL_ttf.TTF_Quit();

            // 计算合适的宽度和高度
            int texWidth = 0;
            int texHeight = 0;
            uint format = 0;
            int access = 0;
            // 下面这行代码解决字体虚浮不清问题
            SDL.SDL_QueryTexture(texture, out format, out access, out texWidth, out texHeight);
            //设置纹理的数据
            SDL.SDL_Rect destrect;
            destrect.x = 0;
            destrect.y = 0;
            destrect.w = texWidth;
            destrect.h = texHeight;

            SDL.SDL_Rect srcrect = destrect;
            SDL.SDL_RenderCopy(sdlrenderer, texture, ref srcrect, ref destrect);
        }
```

效果就会好很多：

![](https://img-blog.csdn.net/20180319170059580)，请看这里的“中华人民共和国”


注意：常用中英文ttf字体包中包含了：times new roman，中山行书百年纪念版，calibri，Christopherhand，DejaVuSansMono，方正兰亭黑，James Fajardo，Monaco，[微软雅黑](http://www.downcc.com/k/weiruanyahei/)，仿宋，黑体，楷体，[宋体](http://www.downcc.com/k/songti/)，yahei_mono，仿宋_GB2312，楷体_GB2312，迷你简行楷碑等。本文使用的是simkai.ttf。下面是部分字体文件名：

bb1550.ttf
calibri.ttf
calibrib.ttf
calibrii.ttf
calibriz.ttf
comesinhandy.ttf
DejaVuSansMono-Bold.ttf
DejaVuSansMono-BoldOblique.ttf
DejaVuSansMono-Oblique.ttf
DejaVuSansMono.ttf
DroidSansFallback.ttf
James_Fajardo.ttf
Monaco.ttf
msyh.ttf
msyhbd.ttf
simfang.ttf
simhei.ttf
simkai.ttf
simsun.ttc
times.ttf
timesbd.ttf
timesbi.ttf
timesi.ttf
yahei_mono.ttf


如果懒得下载，Windows里面有字体，在C:\Windows\Fonts目录下。

#### 参考资料

sdl2.0 如何才能实现截图功能：[http://tieba.baidu.com/p/5214857530](http://tieba.baidu.com/p/5214857530)

True type fonts with SDL_ttf  : [http://www.willusher.io/sdl2%20tutorials/2013/12/18/lesson-6-true-type-fonts-with-sdl_ttf](http://www.willusher.io/sdl2%20tutorials/2013/12/18/lesson-6-true-type-fonts-with-sdl_ttf)

SDL TTF:   [http://www.sdltutorials.com/sdl-ttf](http://www.sdltutorials.com/sdl-ttf)

SDL 使用SDL_ttf显示文字: [http://blog.csdn.net/rankun1/article/details/51393732](http://blog.csdn.net/rankun1/article/details/51393732)



