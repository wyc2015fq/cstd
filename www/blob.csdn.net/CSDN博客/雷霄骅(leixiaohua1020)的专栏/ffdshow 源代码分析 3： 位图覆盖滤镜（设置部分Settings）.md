# ffdshow 源代码分析 3： 位图覆盖滤镜（设置部分Settings） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月24日 23:52:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：38
个人分类：[ffdshow](https://blog.csdn.net/leixiaohua1020/article/category/1638191)












注：写了一系列的文章，都是关于ffdshow中的位图覆盖滤镜的源代码分析的，在此列一个列表：

[ffdshow 源代码分析 2： 位图覆盖滤镜（对话框部分Dialog）](http://blog.csdn.net/leixiaohua1020/article/details/12981725)
[ffdshow 源代码分析 3： 位图覆盖滤镜（设置部分Settings）](http://blog.csdn.net/leixiaohua1020/article/details/13004051)
[ffdshow 源代码分析 4： 位图覆盖滤镜（滤镜部分Filter）](http://blog.csdn.net/leixiaohua1020/article/details/13006213)
[ffdshow 源代码分析 5： 位图覆盖滤镜（总结）](http://blog.csdn.net/leixiaohua1020/article/details/13660583)

![](https://img-blog.csdn.net/20140616103641843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上一篇文章介绍了ffdshow的位图覆盖滤镜的对话框（Dialog）部分：[ffdshow 源代码分析2 ： 位图覆盖滤镜（对话框部分Dialog）](http://blog.csdn.net/leixiaohua1020/article/details/12981725)

在这里再介绍一下设置部分（Settings），此外还有一个滤镜部分（Filter）。这三个部分就可以组成一个ffdshow的滤镜功能了。

**设置部分（Settings）**

在ffdshow中滤镜的设置部分（Settings）主要用于存储滤镜运行过程中需要用到的各种变量。一般情况下通过读取注册表变量并赋值给该类当中的变量从而达到操作相应滤镜的功能。

与位图覆盖（Bitmap）滤镜的设置有关的类位于settings->filters->video目录下（隐藏的很深啊）的TbitmapSettings.cpp和TbitmapSettings.h文件中。

![](https://img-blog.csdn.net/20131024215918062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**先来看看TbitmapSettings.h**

该类的名字叫TbitmapSettings，从类的定义我们可以看出，

flnm[]存储了打开的位图的路径

posx，posy存储了位图在屏幕上显示的位置

mode存储了显示的方式

等等，所有跟该滤镜（Filter）相关的数据都存储在该类之中。

该类包含一个TfilterIDFF类型的结构体idffs，用于存储该滤镜的一些属性信息（名称，ID，属性对话框ID等等）

此外，有两个函数至关重要。createFilters()用于创建滤镜（Filter）。createPages()用于创建滤镜的配置对话框（Dialog）。



```cpp
#ifndef _TBITMAPSETTINGS_H_
#define _TBITMAPSETTINGS_H_
//各个Filter预设值
#include "TfilterSettings.h"
#include "Tfont.h"
//Bitmap的配置信息
struct TbitmapSettings : TfilterSettingsVideo {
private:
    static const TfilterIDFF idffs;
protected:
    virtual const int *getResets(unsigned int pageId);
public:
    TbitmapSettings(TintStrColl *Icoll = NULL, TfilterIDFFs *filters = NULL);
    //Bitmap文件路径
	char_t flnm[MAX_PATH];
	//x,y坐标，以及坐标的模式
    int posx, posy, posmode;
    int align;
	//叠加方式
    enum {
        MODE_BLEND = 0,
        MODE_DARKEN = 1,
        MODE_LIGHTEN = 2,
        MODE_ADD = 3,
        MODE_SOFTLIGHT = 4,
        MODE_EXCLUSION = 5
    };
    int mode;
    static const char_t *modes[];
    int strength;
	//创建Filter
    virtual void createFilters(size_t filtersorder, Tfilters *filters, TfilterQueue &queue) const;
	//创建属性页面
    virtual void createPages(TffdshowPageDec *parent) const;
    virtual bool getTip(unsigned int pageId, char_t *buf, size_t buflen);
};

#endif
```







****再来看看**TbitmapSettings.cpp**

该类包含了TbitmapSettings类中函数方法的具体实现。首先看一下构造函数TbitmapSettings()。从构造函数中可以看出，绑定了类中的变量和注册表变量，使它们形成一一对应的关系。其他的函数就不再细说了，比较简单，理解起来比较容易。



```cpp
/*
 * Copyright (c) 2004-2006 Milan Cutka
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "stdafx.h"
#include "TbitmapSettings.h"
#include "TimgFilterBitmap.h"
#include "Cbitmap.h"
#include "TffdshowPageDec.h"
#include "TsubtitlesSettings.h"
//几种叠加方式
const char_t* TbitmapSettings::modes[] = {
    _l("blend"),
    _l("darken"),
    _l("lighten"),
    _l("add"),
    _l("softlight"),
    _l("exclusion"),
    NULL
};
//Filter属性
const TfilterIDFF TbitmapSettings::idffs = {
    /*name*/      _l("Bitmap overlay"),
    /*id*/        IDFF_filterBitmap,
    /*is*/        IDFF_isBitmap,
    /*order*/     IDFF_orderBitmap,
    /*show*/      IDFF_showBitmap,
    /*full*/      IDFF_fullBitmap,
    /*half*/      0,
    /*dlgId*/     IDD_BITMAP,
};
//构造函数
TbitmapSettings::TbitmapSettings(TintStrColl *Icoll, TfilterIDFFs *filters): TfilterSettingsVideo(sizeof(*this), Icoll, filters, &idffs)
{
    half = 0;
    memset(flnm, 0, sizeof(flnm));
	//绑定变量
    static const TintOptionT<TbitmapSettings> iopts[] = {
        IDFF_isBitmap       , &TbitmapSettings::is        , 0, 0, _l(""), 1,
        _l("isBitmap"), 0,
        IDFF_showBitmap     , &TbitmapSettings::show      , 0, 0, _l(""), 1,
        _l("showBitmap"), 1,
        IDFF_orderBitmap    , &TbitmapSettings::order     , 1, 1, _l(""), 1,
        _l("orderBitmap"), 0,
        IDFF_fullBitmap     , &TbitmapSettings::full      , 0, 0, _l(""), 1,
        _l("fullBitmap"), 0,
        IDFF_bitmapPosx     , &TbitmapSettings::posx      , -4096, 4096, _l(""), 1,
        _l("bitmapPosX"), 50,
        IDFF_bitmapPosy     , &TbitmapSettings::posy      , -4096, 4096, _l(""), 1,
        _l("bitmapPosY"), 50,
        IDFF_bitmapPosmode  , &TbitmapSettings::posmode   , 0, 1, _l(""), 1,
        _l("bitmapPosMode"), 0,
        IDFF_bitmapAlign    , &TbitmapSettings::align     , 0, 3, _l(""), 1,
        _l("bitmapAlign"), ALIGN_CENTER,
        IDFF_bitmapMode     , &TbitmapSettings::mode      , 0, 5, _l(""), 1,
        _l("bitmapMode"), 0,
        IDFF_bitmapStrength , &TbitmapSettings::strength  , 0, 256, _l(""), 1,
        _l("bitmapStrength"), 128,
        0
    };
    addOptions(iopts);
    static const TstrOption sopts[] = {
        IDFF_bitmapFlnm     , (TstrVal)&TbitmapSettings::flnm  , MAX_PATH, 0, _l(""), 1,
        _l("bitmapFlnm"), _l(""),
        0
    };
    addOptions(sopts);

    static const TcreateParamList1 listMode(modes);
    setParamList(IDFF_bitmapMode, &listMode);
    static const TcreateParamList1 listAlign(TsubtitlesSettings::alignments);
    setParamList(IDFF_bitmapAlign, &listAlign);
}
//创建Filter
void TbitmapSettings::createFilters(size_t filtersorder, Tfilters *filters, TfilterQueue &queue) const
{
    idffOnChange(idffs, filters, queue.temporary);
    if (is && show) {
        queueFilter<TimgFilterBitmap>(filtersorder, filters, queue);
    }
}
//创建属性页面
void TbitmapSettings::createPages(TffdshowPageDec *parent) const
{
    parent->addFilterPage<TbitmapPage>(&idffs);
}

const int* TbitmapSettings::getResets(unsigned int pageId)
{
    static const int idResets[] = {
        IDFF_bitmapPosx, IDFF_bitmapPosy, IDFF_bitmapPosmode, IDFF_bitmapAlign, IDFF_bitmapMode, IDFF_bitmapStrength,
        0
    };
    return idResets;
}

bool TbitmapSettings::getTip(unsigned int pageId, char_t *tipS, size_t len)
{
    if (flnm[0]) {
        tsnprintf_s(tipS, len, _TRUNCATE, _l("%s %s"), modes[mode], flnm);
        tipS[len - 1] = '\0';
    } else {
        tipS[0] = '\0';
    }
    return true;
}
```














