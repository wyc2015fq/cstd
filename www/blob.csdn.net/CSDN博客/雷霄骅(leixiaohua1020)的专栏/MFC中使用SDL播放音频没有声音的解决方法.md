# MFC中使用SDL播放音频没有声音的解决方法 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月10日 20:30:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：27
个人分类：[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)










本文所说的音频是指的纯音频，不包含视频的那种。

在控制台中使用SDL播放音频，一般情况下不会有问题。

但是在MFC中使用SDL播放音频的时候，会出现没有声音的情况。经过长时间探索，没有找到特别好的解决方案，但是有一种方式可以让声音播放出来：那就是让SDL显示图像（视频）时候的那个对话框弹出来，声音就会出现了。

具体的方法可以加载一张图片（比如说BMP），下图所示代码片段为加载BMP图片的代码。



```cpp
SDL_Surface *screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
	SDL_Surface *image;
	/* Load the BMP file into a surface */
	image = SDL_LoadBMP("background.bmp");
	if (image == NULL) {
		return 0;
	}
	/*
	* Palettized screen modes will have a default palette (a standard
	* 8*8*4 colour cube), but if the image is palettized as well we can
	* use that palette for a nicer colour matching
	*/
	if (image->format->palette && screen->format->palette) {
		SDL_SetColors(screen, image->format->palette->colors, 0,
			image->format->palette->ncolors);
	}
	/* Blit onto the screen surface */
	if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
	SDL_UpdateRect(screen, 0, 0, image->w, image->h);
```







不明白这是为什么，但是程序就可以出声了。




