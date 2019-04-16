# ffdshow 源代码分析 2： 位图覆盖滤镜（对话框部分Dialog） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月24日 21:30:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：96









注：写了一系列的文章，都是关于ffdshow中的位图覆盖滤镜的源代码分析的，在此列一个列表：

[ffdshow 源代码分析 2： 位图覆盖滤镜（对话框部分Dialog）](http://blog.csdn.net/leixiaohua1020/article/details/12981725)
[ffdshow 源代码分析 3： 位图覆盖滤镜（设置部分Settings）](http://blog.csdn.net/leixiaohua1020/article/details/13004051)
[ffdshow 源代码分析 4： 位图覆盖滤镜（滤镜部分Filter）](http://blog.csdn.net/leixiaohua1020/article/details/13006213)
[ffdshow 源代码分析 5： 位图覆盖滤镜（总结）](http://blog.csdn.net/leixiaohua1020/article/details/13660583)

![](https://img-blog.csdn.net/20140616103606531)

本文我们介绍ffdshow的滤镜功能。ffdshow支持很多种滤镜，可以支持多种视频和音频的后期效果。例如OSD滤镜支持在视频的左上角显示视频相关的信息。而可视化滤镜则支持显示视频每一帧的运动矢量以及量化参数。在这里我们介绍一种位图覆盖（Bitmap）滤镜（Filter）。

**效果**

编译完ffdshow之后，在“项目属性->调试->命令”里面将GraphEdit.exe所在位置设置为调试程序，例如在这里我设置了《终极解码》里面自带GraphEdit.exe，路径为“C:\Program Files\Final Codecs\Codecs\GraphEdit.exe”。这样就可以使用GraphEdit.exe调试ffdshow了。

向GraphEdit.exe里面拖入一个文件“五月天 咸鱼.mp4”，结果如下图所示：

![](https://img-blog.csdn.net/20131024181216781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注：有的时候默认的视频解码器可能不是ffdshow，可能是CoreAVC等，可以先删除视频解码器然后添加ffdshow。

点击绿色三角形按钮就可以开始播放视频。

右键点击ffdshow组件，打开属性对话框之后，可以看见右边栏中有很多的滤镜。

勾选“位图覆盖”滤镜，然后选择一张用于覆盖的图片（在这里我选择了一张bmp格式的专辑封面）。

注：可以调整位图所在的水平位置，垂直位置，不透明度，并且可以修改位图叠加模式（在这里用混合）。

![](https://img-blog.csdn.net/20131024181221718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

添加了该滤镜之后，播放窗口的显示内容为：

![](https://img-blog.csdn.net/20131024181226859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可见在右上角显示出了叠加的位图。

**源代码分析**

**1.对话框部分**

与位图覆盖（Bitmap）滤镜的对话框有关的类位于dialog目录下的Cbitmap.cpp和Cbitmap.h文件中。

![](https://img-blog.csdn.net/20131024183912796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

先来看看Cbitmap.h中类的声明：

需要注意的是，里面类的名字居然叫TbitmapPage，而没有和头文件名字一致。= =



```cpp
#ifndef _CBITMAPPAGE_H_
#define _CBITMAPPAGE_H_

#include "TconfPageDecVideo.h"
//Bitmap配置页面
class TbitmapPage : public TconfPageDecVideo
{
private:
    void pos2dlg(void), opacity2dlg(void);
	//设置文件路径
    void onFlnm(void);
protected:
    virtual INT_PTR msgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	//构造函数
    TbitmapPage(TffdshowPageDec *Iparent, const TfilterIDFF *idff);
	//初始化
    virtual void init(void);
	//配置数据传入到对话框界面
    virtual void cfg2dlg(void);
    virtual void translate(void);
};

#endif
```


再看看Cbitmap.cpp文件吧。关键的代码都已经加上了注释。





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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
//Bitmap配置页面
#include "stdafx.h"
#include "TsubtitlesSettings.h"
#include "TbitmapSettings.h"
#include "Cbitmap.h"
//初始化
void TbitmapPage::init(void)
{
	//设置滑动条范围
    edLimitText(IDC_ED_BITMAP_FLNM, MAX_PATH);
    tbrSetRange(IDC_TBR_BITMAP_POSX, 0, 100, 10);
    tbrSetRange(IDC_TBR_BITMAP_POSY, 0, 100, 10);
    tbrSetRange(IDC_TBR_BITMAP_OPACITY, 0, 256);
}
//配置数据传入到对话框界面
void TbitmapPage::cfg2dlg(void)
{
	//各种设置
	//EditControl设置
    setDlgItemText(m_hwnd, IDC_ED_BITMAP_FLNM, cfgGetStr(IDFF_bitmapFlnm));
    pos2dlg();
    cbxSetCurSel(IDC_CBX_BITMAP_ALIGN, cfgGet(IDFF_bitmapAlign));
    cbxSetCurSel(IDC_CBX_BITMAP_MODE, cfgGet(IDFF_bitmapMode));
    opacity2dlg();
}
//Bitmap位置信息
void TbitmapPage::pos2dlg(void)
{
    char_t s[260];
    int x;
	//获取
    x = cfgGet(IDFF_bitmapPosx);
    TsubtitlesSettings::getPosHoriz(x, s, this, IDC_LBL_BITMAP_POSX, countof(s));
    setDlgItemText(m_hwnd, IDC_LBL_BITMAP_POSX, s);
    //设置
	tbrSet(IDC_TBR_BITMAP_POSX, x);

    x = cfgGet(IDFF_bitmapPosy);
    TsubtitlesSettings::getPosVert(x, s, this, IDC_LBL_BITMAP_POSY, countof(s));
    setDlgItemText(m_hwnd, IDC_LBL_BITMAP_POSY, s);
    tbrSet(IDC_TBR_BITMAP_POSY, x);
}
void TbitmapPage::opacity2dlg(void)
{
    int o = cfgGet(IDFF_bitmapStrength);
    tbrSet(IDC_TBR_BITMAP_OPACITY, o);
    setText(IDC_LBL_BITMAP_OPACITY, _l("%s %i%%"), _(IDC_LBL_BITMAP_OPACITY), 100 * o / 256);
}

INT_PTR TbitmapPage::msgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_ED_BITMAP_FLNM:
                    if (HIWORD(wParam) == EN_CHANGE && !isSetWindowText) {
                        char_t flnm[MAX_PATH];
                        GetDlgItemText(m_hwnd, IDC_ED_BITMAP_FLNM, flnm, MAX_PATH);
                        cfgSet(IDFF_bitmapFlnm, flnm);
                    }
                    return TRUE;
            }
            break;
    }
    return TconfPageDecVideo::msgProc(uMsg, wParam, lParam);
}
//设置文件路径
void TbitmapPage::onFlnm(void)
{
    char_t flnm[MAX_PATH];
    cfgGet(IDFF_bitmapFlnm, flnm, MAX_PATH);
    if (dlgGetFile(false, m_hwnd, _(-IDD_BITMAP, _l("Load image file")), _l("All supported (*.jpg,*.bmp,*.gif,*.png)\0*.bmp;*.jpg;*.jpeg;*.gif;*.png\0Windows Bitmap (*.bmp)\0*.bmp\0JPEG (*.jpg)\0*.jpg\0Compuserve Graphics Interchange (*.gif)\0*.gif\0Portable Network Graphics (*.png)\0*.png"), _l("bmp"), flnm, _l("."), 0)) {
        setDlgItemText(m_hwnd, IDC_ED_BITMAP_FLNM, flnm);
        //设置
		cfgSet(IDFF_bitmapFlnm, flnm);
    }
}

void TbitmapPage::translate(void)
{
    TconfPageBase::translate();

    cbxTranslate(IDC_CBX_BITMAP_ALIGN, TsubtitlesSettings::alignments);
    cbxTranslate(IDC_CBX_BITMAP_MODE, TbitmapSettings::modes);
}
//构造函数
TbitmapPage::TbitmapPage(TffdshowPageDec *Iparent, const TfilterIDFF *idff): TconfPageDecVideo(Iparent, idff)
{
	//各种绑定
    resInter = IDC_CHB_BITMAP;
    static const TbindTrackbar<TbitmapPage> htbr[] = {
        IDC_TBR_BITMAP_POSX, IDFF_bitmapPosx, &TbitmapPage::pos2dlg,
        IDC_TBR_BITMAP_POSY, IDFF_bitmapPosy, &TbitmapPage::pos2dlg,
        IDC_TBR_BITMAP_OPACITY, IDFF_bitmapStrength, &TbitmapPage::opacity2dlg,
        0, 0, NULL
    };
    bindHtracks(htbr);
    static const TbindCombobox<TbitmapPage> cbx[] = {
        IDC_CBX_BITMAP_ALIGN, IDFF_bitmapAlign, BINDCBX_SEL, NULL,
        IDC_CBX_BITMAP_MODE, IDFF_bitmapMode, BINDCBX_SEL, NULL,
        0
    };
    bindComboboxes(cbx);
    static const TbindButton<TbitmapPage> bt[] = {
        IDC_BT_BITMAP_FLNM, &TbitmapPage::onFlnm,
        0, NULL
    };
    bindButtons(bt);
}
```


看ffdshow源代码的时候，开始会比较费劲。为什么？因为它使用了大量自己写的API函数，以及自己定义的结构体。这些API函数的种类繁多，如果一个一个都看完，估计就精疲力竭了。经过一段时间的学习之后，我发现最方便的方法还是根据函数名字推测其用法。因此我就不深入剖析ffdshow的API函数了。



以上源代码中包含以下API（大致按出现先后次序，可能没有例举全，在这里只是举例子）：



```cpp
edLimitText();//限制输入字符串长度
tbrSetRange();//设置滑动条范围
setDlgItemText();//设置组件名称
cbxSetCurSel();//设置下拉框当前选项
cfgGet();//从注册表中读取变量的值
tbrSet();//设置滑动条的值
bindHtracks();//绑定注册表变量和滑动条
bindComboboxes();//绑定注册表变量和下拉框
bindButtons();//绑定函数和按钮
```





从以上函数大致可以看出tbr***()基本上都是操作滑动条的，cbx***()基本上都是操作下拉框的，函数基本上可以从名称上理解其的意思。bind***()就是绑定注册表变量和控件的。注意ffdshow里面有注册表变量这么一个概念。这些变量的值存在系统的注册表里面，不会因为程序结束运行而消失。就目前我的观察来看，绝大部分注册表变量存的是一个整数值。这些注册表变量都以IDFF_xxx的名称预编译定义在ffdshow_constants.h头文件中。与MFC控件可以直接与CString，int等变量绑定不同，ffdshow控件只可以和注册表变量绑定。即每次运行的时候都从注册表加载变量的值到界面上。存储的时候把界面上的值存储到注册表中。

注：注册表变量如下所示（截取了一小段）



```cpp
#define IDFF_filterBitmap   1650
#define IDFF_isBitmap       1651
#define IDFF_showBitmap     1652
#define IDFF_orderBitmap    1653
#define IDFF_fullBitmap     1654
#define IDFF_bitmapFlnm     1655
#define IDFF_bitmapPosx     1656
#define IDFF_bitmapPosy     1657
#define IDFF_bitmapPosmode  1658
#define IDFF_bitmapAlign    1659
#define IDFF_bitmapMode     1660
#define IDFF_bitmapStrength 1661
```





此外需要注意的是，ffdshow尽管包含了图形化的属性界面，却没有使用MFC类库，因而MFC的很多函数都不能使用，对此我还不甚了解为什么要这样，以后有机会要探究探究。






