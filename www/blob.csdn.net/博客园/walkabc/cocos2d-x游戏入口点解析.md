# cocos2d-x游戏入口点解析 - walkabc - 博客园
# [cocos2d-x游戏入口点解析](https://www.cnblogs.com/hitfire/articles/3163075.html)
```
#include "main.h"
#include "../Classes/AppDelegate.h"
#include "cocos2d.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
USING_NS_CC;
// 500 is enough?
#define MAXPATHLEN 500
int main(int argc, char **argv)
{
    // get application path
    int length;
    char fullpath[MAXPATHLEN];
    length = readlink("/proc/self/exe", fullpath, sizeof(fullpath));
    fullpath[length] = '\0';
    std::string resourcePath = fullpath;
    resourcePath = resourcePath.substr(0, resourcePath.find_last_of("/"));
    resourcePath += "/../../../Resources/";
    
    // create the application instance
    AppDelegate app;
    CCApplication::sharedApplication()->setResourceRootPath(resourcePath.c_str());
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    /**设置游戏的窗口大小,屏幕的大小和屏幕的像素不一定是相等的，看看现在的手机就知道了
     * android的手机屏幕虽然大，但像素不一定比iphone的高
     * 所以，要设置一个scale来完成像素到屏幕位置的转换，比如像素(800, 600)映射到屏幕(400, 300)位置
     * 那么就需要有个一个scale来完成这个转换，这里显然scale的值是0.5
     * scale是在pDirector->setContentScaleFactor方法中完成的。
     * 注意：cocos2d-x中的显示对象的位置，我们最好设置其像素位置，而不是其屏幕位置
     * cocos2d-x会自动完成，像素位置到屏幕位置的转换
     * */
    eglView->setFrameSize(400, 300);
    return CCApplication::sharedApplication()->run();
}
```

