# SDL2画一个简单的矩形 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年08月21日 20:38:01[boonya](https://me.csdn.net/boonya)阅读数：493








SDL2画矩形很简单，不需要SDL_Draw来辅助。下面的函数定义了一个画矩形的方法。

```
SDL.SDL_Rect rectangle= new SDL.SDL_Rect();;

        /// <summary>
        /// SDL画矩形长方形
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <param name="color">0:红，1：绿，2：蓝 （默认值为1:绿色）</param>
        public void DrawRect(int width, int height,int color=1)
        {
            if (color == 0)
            {
                SDL.SDL_SetRenderDrawColor(sdlrenderer, 255, 0, 0,1);// 红
            }
            else if (color == 1)
            {
                SDL.SDL_SetRenderDrawColor(sdlrenderer, 0, 255, 0, 1);// 绿
            }
            else if (color == 2)
            {
                SDL.SDL_SetRenderDrawColor(sdlrenderer, 0, 0, 255, 1);// 蓝
            }
            rectangle.x = 0;
            rectangle.y = 0;
            rectangle.w = width;
            rectangle.h = height;
            SDL.SDL_RenderFillRect(sdlrenderer, ref rectangle);
        }
```

注意：SDL是一层一层往上添加的，跟地图的覆盖物是同一个道理。例如下面的代码，标识了绘制图像的位置，防止影响其他操作。

```
// 画矩形边框（位置一定要靠前不然会遮挡视频）
                    if (DRAW_BORDER_IN_SDL)
                    {
                        DrawRect(windowWidth, windowHeight);
                        //重新设置YUV大小====开始（边框效果）
                        yuv_srcrect = new SDL.SDL_Rect();
                        yuv_srcrect.x = 0;
                        yuv_srcrect.y = 0;
                        yuv_srcrect.w = windowWidth;
                        yuv_srcrect.h = windowHeight;

                        yuv_destrect = yuv_srcrect;
                        yuv_destrect.x = 1;
                        yuv_destrect.y = 1;
                        yuv_destrect.w = windowWidth - 2;
                        yuv_destrect.h = windowHeight - 2;
                        //重新设置YUV大小====开始（边框效果）
                    }

                    //dlgIClientChange main_thread_run = InvokeMethodRender;
                    //main_thread_run.Invoke();

                    //画最后一张图或者画背景
                    ret = SDL.SDL_UpdateTexture(sdltexture, IntPtr.Zero, m_yuv_buffer, m_video_width);

                    ret = SDL.SDL_RenderClear(sdlrenderer);

                    // 渲染视频图像
                    //if (DRAW_BORDER_IN_SDL)
                    //{
                    //    // 边框样式渲染方式
                    //    ret = SDL.SDL_RenderCopy(sdlrenderer, sdltexture, ref yuv_srcrect,ref yuv_destrect);
                    //}
                    //else
                    {
                        // YUV自适应窗口大小
                        ret = SDL.SDL_RenderCopy(sdlrenderer, sdltexture, IntPtr.Zero, IntPtr.Zero);
                    }
```



如何清除矩形，可以自己了解下。



