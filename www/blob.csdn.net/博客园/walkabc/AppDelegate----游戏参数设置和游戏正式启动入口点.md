# AppDelegate----游戏参数设置和游戏正式启动入口点 - walkabc - 博客园
# [AppDelegate----游戏参数设置和游戏正式启动入口点](https://www.cnblogs.com/hitfire/articles/3163038.html)
```
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "AppMacros.h"
USING_NS_CC;
/**应用程序代表*/
AppDelegate::AppDelegate() {
}
AppDelegate::~AppDelegate() 
{
}
/**
 * 这个方法会被CCApplication::sharedApplication()->run()调用
 * */
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    /**
     * 获取到系统级别的静态变量
     * */
    /**我是导演*/
    CCDirector* pDirector = CCDirector::sharedDirector();
    /**
     * 拍摄场地，这个场地可以随便搭建一些场景，放置演员
     * */
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    /**
     * 把这个场地租给导演
     * */
    pDirector->setOpenGLView(pEGLView);
    // Set the design resolution
    /**
     * 这是这个场地的大小
     * */
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
    /**
     * 在这个场地中，导演拍摄电影，这个size就是最终我们在电影院看到的屏幕的大小
     * */
    CCSize frameSize = pEGLView->getFrameSize();
    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.
    // if the frame's height is larger than the height of medium resource size, select large resource.
    if (frameSize.height > mediumResource.size.height)
    { 
        CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
        pDirector->setContentScaleFactor(largeResource.size.height/designResolutionSize.height);
    }
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    { 
        CCFileUtils::sharedFileUtils()->setResourceDirectory(mediumResource.directory);
        pDirector->setContentScaleFactor(mediumResource.size.height/designResolutionSize.height);
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
    else
    { 
        CCFileUtils::sharedFileUtils()->setResourceDirectory(smallResource.directory);
        pDirector->setContentScaleFactor(smallResource.size.height/designResolutionSize.height);
    }
    
    // turn on display FPS
    /**
     * 在cocos2d-x的demo中，左下角会显示帧频，如何在这个设置为false的画，左下角就不会显示帧频了
     * */
    pDirector->setDisplayStats(true);
    // set FPS. the default value is 1.0/60 if you don't call this
    /**
     * 设置游戏的最大的帧频，理论上游戏画面的最快刷新速度是六十分之一秒
     * 具体能达到多少秒，取决与玩家的硬件
     * */
    pDirector->setAnimationInterval(1.0 / 60);
    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();
    // run
    pDirector->runWithScene(pScene);
    return true;
}
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
```

