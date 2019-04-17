# C语言SDL基础库扩展开发-动态显示鼠标的位置信息 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年07月02日 13:23:16[boonya](https://me.csdn.net/boonya)阅读数：179








原文地址：[https://my.oschina.net/freestyletime/blog/166826](https://my.oschina.net/freestyletime/blog/166826)



**今天要讲的是，把鼠标的位置在SDL窗口上动态的显示，也就是画在窗口上。**这里要用到SDL的其中一个extension library叫做SDL_ttf，用于把文字渲染到屏幕上，在[http://www.libsdl.org/projects/SDL_ttf/release-1.2.html](http://www.libsdl.org/projects/SDL_ttf/release-1.2.html)有下载。 

 不过下载完后把头文件库文件什么的加到环境中以后，写了一个小的Demo，并不能编译通过，详细信息请见[http://www.oschina.net/question/250247_127725](http://www.oschina.net/question/250247_127725) ，嘿嘿，也是我今天下午在OSC上提出的问题，并没有人回答，是我最后自己解决的。 

 你可能会问博主，SDL_ttf 2.0.9 版本在哪下，你也不用考虑了，博主已经下好了，关于这几天的SDL文章的代码已经上传到了博主的[OSC](mailto:OSC@GIT)[@GIT](http://my.oschina.net/u/857562) ，请在这里[http://git.oschina.net/ChristianChen/SDL_demo.git](http://git.oschina.net/ChristianChen/SDL_demo.git)将代码拷下来就行了，都是最新的代码。整个工程拷下来以后，SDL_demo目录下有个env_zip的文件夹，里面是所有迄今为止用到的压缩包，包括SDL、SDL_image、SDL_ttf等，你只需把里面的东西放到PC上正确的位置，即搭建好开发环境即可，看到这里是不是觉得博主考虑很周到呢？其实我也是这样想的，哈哈哈...

先来看看博主目前的工程目录：

![](http://static.oschina.net/uploads/space/2013/1004/224013_C6Bd_250247.jpg)

上一篇博主在SDL_fun.h里封装了一个ScreenSurface的class对象，实现代码忘记给大家贴出来了，下面补上:

```cpp
```cpp
//-----------------------Class------------------------------
ScreenSurface::ScreenSurface(SDL_Surface* screen) :
		screen(screen) {
	if (screen != NULL) {
		this->screen = screen;
	} else {
		std::cerr << "creenSurface(SDL_Surface* screen) screen is NULL" << "\n";
	}
}

ScreenSurface::ScreenSurface(int w, int h, int b, Uint32 f) {
	this->screen = SDL_SetVideoMode(w, h, b, f);
}

ScreenSurface::~ScreenSurface(void) {

}

bool ScreenSurface::flip(void) const {

	if (SDL_Flip(this->screen) < 0) {
		std::cerr << SDL_GetError() << "\n";
		SDL_ClearError();
		return false;
	}

	return true;
}

SDL_Surface* ScreenSurface::acquire(void) {
	return this->screen;
}
//-----------------------Class------------------------------
```
```

 由于在用到SDL_ttf，博主又在SDL_init.h头文件中，封装了初始化和退出SDL_ttf环境的函数 

```cpp
```cpp
/*
 * SDL_init.h
 *
 *  Created on: 2013-10-3
 *      Author: Christian
 */

#ifndef SDL_INIT_H_
#define SDL_INIT_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <iostream>

/** 初始化SDL环境  initiate SDL environment
 *
 * @param title represent current window's headline*/
void createSDL(const char* title);

/** 退出SDL环境  quit SDL environment*/
void destroySDL(void);

/** 初始化SDL TTF 环境  initiate SDL TTF environment */
TTF_Font* createSDLTTF(const char* ttfFile, int size);

/** 退出SDL TTF 环境  quit SDL TTF environment*/
void destroySDLTTF(TTF_Font* font);


#endif /* SDL_INIT_H_ */
```
```

 当中的TTF_Font是个结构体，我理解的这个结构体的大概意思是代表某一种字体，比如宋体、楷体等等... 

 下面是实现： 

```cpp
```cpp
TTF_Font* createSDLTTF(const char* ttfFile, int size) {
	//Initialize all SDL subsystems
	if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0) {
		std::cerr << "Call TTF_Init before call SDL_Init\n";
		return NULL;
	}

	if (TTF_WasInit() != 0) {
		std::cerr << "TTF is already running.\n";
		return NULL;
	}

	if (ttfFile == NULL) {
		std::cerr << "TTF_Init ttfFile is NULL" << "\n";
		return NULL;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		std::cerr << "TTF_Init Failed:" << SDL_GetError() << std::endl;
		SDL_ClearError();
		return NULL;
	}

	std::cout << "TTF_Init successfully:\n";
	return TTF_OpenFont(ttfFile, size);
}

void destroySDLTTF(TTF_Font* font) {

	if (TTF_WasInit() == 0) {
		std::cout << "TTF was NOT running!\n";
		return;
	}

	TTF_CloseFont(font);

	//Quit SDL_ttf
	atexit(TTF_Quit);
	std::cout << "TTF_Quit successfully:\n";
}
```
```



 再来看看SDL_fun.h中新增的方法： 



```cpp
```cpp
/*
 * SDL_fun.h
 *
 *  Created on: 2013-10-1
 *      Author: Christian
 */

#ifndef SDL_FUN_H_
#define SDL_FUN_H_

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

class ScreenSurface {

public:
	ScreenSurface(SDL_Surface* screen = NULL);
	ScreenSurface(int width, int height, int bpp = 32, Uint32 flags =
			SDL_SWSURFACE);
	~ScreenSurface(void);
	/** 将数据push到物理屏幕 */
	bool flip(void) const;
	/** 得到当前指向物理屏幕接口的指针 */
	SDL_Surface* acquire(void);

private:
	SDL_Surface* screen;
};

void drawBitmap(ScreenSurface s, const char* fileName);
void readKeyboard(ScreenSurface s);
void getRadioInfo(ScreenSurface s);
void renderPictures(ScreenSurface s, const char* fgFile, const char* bgFile);
void drawClips(ScreenSurface s, const char* file, SDL_Rect* clip = NULL, int size = 0);
void getMousePostion(ScreenSurface s);
void drawTest(ScreenSurface s, TTF_Font* font, const char* message);
void showMousePostion(ScreenSurface s, TTF_Font* font);
void playAudio(ScreenSurface s, const char* fileName);
void playVidio(ScreenSurface s, const char* fileName);

#endif /* SDL_FUN_H_ */
```
```

 其中的showMousePostion便是今天要讲的主要函数，用于显示鼠标在当前窗口的位置信息. 

 下面看看它的实现： 

```cpp
```cpp
/** 画出鼠标位置信息 */
void showMousePostion(ScreenSurface s, TTF_Font* font) {

	if (font == NULL) {
		std::cerr << "font is NULL" << "\n";
		return;
	}

	SDL_Surface* msg = NULL;
	//将文字的颜色设为白色
	SDL_Color textColor = { 0, 0xFF, 0xFF };


//	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_Event event;
	char buf[11];

	do {
		SDL_WaitEvent(&event);

		//在SDL_Surface之上渲染文字
		if (event.type == SDL_MOUSEMOTION) {

			SDL_FillRect(s.acquire(), &s.acquire()->clip_rect,
					SDL_MapRGB(s.acquire()->format, 0xFF, 0xFF, 0xFF));


			sprintf(buf, "%d:%d", event.motion.x, event.motion.y);

			msg = TTF_RenderText_Solid(font, buf, textColor);

			//If there was an error in rendering the text
			if (SDL_BlitSurface(msg, NULL, s.acquire(), NULL) != 0) {
				std::cerr << SDL_GetError() << "\n";
			}

			if (!s.flip()) {
				std::cerr << SDL_GetError() << "\n";
			}

		}
	} while (event.type != SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE);

	SDL_FreeSurface(msg);
}
```
```



  函数还是很简单的吧，哈哈，一看就懂的~再来看看main函数入口是怎么写的： 

```cpp
```cpp
//============================================================================
// Name        : SDL_demo.cpp
// Author      : Christian
// Version     : 1.0
// Copyright   : Copyright (c) 2013,Christian
// Description : SDL in C++
//============================================================================

#include "include/SDL_init.h"
#include "include/SDL_fun.h"
using namespace std;

int SDL_main(int argc, char *argv[]) {

	createSDL("Christian Test");
	TTF_Font* font = createSDLTTF("FreeSans.ttf", 30);
	//创建全局唯一的指向物理屏幕的SDL_Surface
	ScreenSurface screen(600, 600);
	//getMousePostion(screen);
	//drawBitmap(screen, "sheet.jpg");

	//drawTest(screen, "FreeMono.ttf", "Bitch , Fuck you ~");
	showMousePostion(screen, font);

	destroySDLTTF(font);
	destroySDL();
	return EXIT_SUCCESS;
}
```
```

 现在你便知道，工程下的那些ttf文件是干嘛用的啦，传给createSDLTTF函数，打开一种字体并设置字体的字号大小，相当于是拿到了窗口的画笔吧，但是颜色没有单独封装出来，想要更严谨的话，可以自己试试。 

 下面是编译链接运行后的效果图： 
![](http://static.oschina.net/uploads/space/2013/1004/230144_wNsp_250247.jpg)


注意：SDL_ttf1.2依赖FreeType2.0或者更新的库（2.1.3除外）。 



更多更好的SDL入门请参考：[SDL入门教程](https://blog.csdn.net/guoduhua/article/category/1232800)





