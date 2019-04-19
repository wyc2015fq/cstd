# 用MFC构造DirectX应用框架 ( 转载) - xqhrs232的专栏 - CSDN博客
2009年07月09日 22:35:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：730标签：[mfc																[框架																[null																[microsoft																[windows																[direct3d](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[3D/OpenGL](https://blog.csdn.net/xqhrs232/article/category/906930)
摘  要:把directxsdk封装在mfc的类中，建立一个directx应用程序框架。
关键词:mfc类库，directxsdk
**一、 mfc类库与directxsdk**
microsoft directx sdk是开发基于 windows平台游戏的一个软件开发工具，其主要功能主要包括在五个组件中: directdraw、 directsound、directplay、direct3d和directinput，每个组件都具不同的功能:
．directdraw使用直接写存技术加快游戏的动画速度；
．direcsound控制游戏声音的合成和播放；
．directplay使游戏具有网络多人游戏功能；
．direct3d让程序员更方便地开发三维游戏:
．directinput使游戏支待更多的输入设备(现在只支持游戏杆、鼠标和键盘)。
可以说directxsdk提供了编写一个游戏所必须的功能及基层函数，所以大多windows游戏都使用了ditrectxsdk。
mfc(microsoft foundation class)类库是microsoft visual c++中提供的一个功能强大的windows应用程序开发类，使用这些类我们可以避免和繁琐的windows apl打交道，而且在visualc++中我们还可以利用c1asswizard对mfc类进行windows消息映射，所以如果能用mfc类库来开发directxsdk的应用程序，至少有以下几个好处:
1．可以用 vc++的 c1asswizard方便地对windows消息进行映射；
2．增加了程序的可读性，并且可以用vc++的classview方便的管理所用的类；
3．增加程序代码的可重用性，可以在原有的基础上开发出功能更强大的应用程序更进一步，如果我们能开发出一个能生成directxsdk应用程序基本框架的vc++的工程向导，则为以后开发directx sdk应用程序提供及大的方便。下面，我们将用visua1 c++5．0先编写一个directxsdk应用程序的基本框架。
**二、编写directxsdk应用程序基本框架**
我们按下列步骤建立一个勘rectxsdk程序的基本框架:
1．用 visual c++的 mfc appwizard(exe)生成一个基于对话框的工程文件，取名为directx，在向导第二步时取消about box的复选框，然后按finish按钮。
2．删除在directx工程目录中生成的directxdlg．cpp和directxdlg．h两个文件，并在visual c++的fileview中删除以上两个文件，按ctrl十w启动classwizard删除cdirectxdlg类，然后在resourceview中删除 idd_directx_
dialog。
3．建立两个文件 directxwnd.cpp和directxwnd．h(这两个文件在本文的附录中，请注意不要删除有“//{”和“//}”之间的内容，否则将不能使用classwizard对窗口消息进行映射)，并把它们加入到工程中。这时工程中将增加一个基于cwnd的cdirectxwnd类，这是我们的dirrectx应用程序的基类。cdirectxwnd类创建一个窗口并生成一个与该窗口相关联的directdraw对象lpdd，同时还生成一个显示平面(lpfrontbuffer)和一个显示缓冲平面(lpbackbuffer)，该类使用了几个虚函数，必要时其派生类可以覆盖这些函数。
4．打开directx．cpp，把# include“directxdlg．h”改为#include“directxwnd．h”然后把cdirectxapp::initinstance()函数修改如下，其中黑体字为要增加的内容:
bool cdirectxapp::lnitlnstnnce()
{
    #ifdef _afxdll    enable3dcontrols()；//call this when using mfc in a shared dll
    #else    enable3dcontrolsstatic()；//call this when linking to mfc statically
    #endif
**    cdirectxwnd *pwnd=new cdirectxwnd();    pwnd->create("directxwnd test");    m_pmainwnd = pwnd;    pwnd->updatewindow();    pwnd->setfocus();    if(pwnd->initializegame(640,480,8)==false)    {        pwnd->destroywindow();        return false;    }    msg msg;    while(1)    {        if(peekmessage(&msg,null,0,0,pm_noremove))        {            if(!getmessage(&msg,null,0,0))                return msg.wparam;            translatemessage(&msg);            dispatchmessage(&msg);        }        else        {            if(pwnd->blsactive)            {                pwnd->updateframe();            }        }    }**    return false;
}
编译该程序并运行，可以看到出现一个黑色的屏幕窗口，按esc或f12则可退出程序。至此，我们的基本框架已经建立好了，虽然这个框架还比较简单，但我们可以在此基础上开发出更强大的应用框架。为了方便使用该框架，我们可以为该框架写一个custom appwizard，当然也可以不写，只要把该工程目录下的文件拷贝到另一个工程目录中即可。
**三、测试框架**
现在，我们按下列步骤写一个测试程序来测试这个框架:
1．把刚才创建的工程框架拷贝到一个新目录下，并打开。用c1assview创建一个基于cwnd的类ctestwnd，然后把ctestwnd.h和ctestwnd.cpp文件中的所有"cwnd"字符串替换为"cdirectxwnd"，并在ctestwnd.h文件头加入下列
字符串:#include"directxwnd.h"。
2．打开directx.cpp文件，在文件头加入#include"testwnd.h"，并把该文件中的所有"cdirectxwnd"字符串替换成"ctestwnd"并保存。
3．为 ctestwnd类增加一个虚函数updateframe()，这个函数覆盖了其基类cdirectwnd的updateframe():
void ctestwnd::updateframe()
{
    static int x=0,dx=5;
    static int y=0,dy=5;
    hdc hdc;
    ddbltfx ddbltfx;
    hresult ddrval;
    updatewindow();
    ddbltfx.dwsize=sizeof(ddbltfx);
    ddbltfx.dwfillcolor=0;
    ddrval=lpbackbuffer->blt(
        null,//dest rect
        null,//src surface
        null,//src rect
        ddblt_colorfill|ddblt_wait,
        &ddbltfx);
    if(ddrval!=dd_ok)
    {
        msg("fill failed ddrval=0x%081x",ddrval);
        return;
    }
    if(lpbackbuffer->getdc(&hdc)==dd_ok)
    {
        if(x<0)dx=5;
        if(x>590)dx=-5;
        if(y<0)dy=5;
        if(y>430)dy=-5;
        x+=dx;y+=dy;
        ellipse(hdc,x,y,x+50,y+50);
        lpbackbuffer->releasedc(hdc);
    }
    while(1)
    {
        hresult ddrval;
        ddrval=lpfrontbuffer->flip(null,0);
        if(ddrval==dd_ok)
        {
            break;
        };
        if(ddrval==dderr_surfacelost)
        {
            if(!cdirectxwnd::restoresurfaces())
            {
                break;
            }
        }
        if(ddrval!=dderr_wasstilldrawing)
        {
            break;
        }
    }
}
无心柳按：光是这么追加在测试中没有成功，可能要在ctestwnd.h中追加两行：
public:
    void    updateframe();
否则编译可能出错。
**4、编译并运行该程序，屏幕上会出现一个白色球在移动。**
附录：
文件：directxwnd.h
#if!defined(directxwnd_h)
#define directxwnd_h
//directxwnd.h:header file
#include<ddraw.h>
#pragma comment(lib,"ddraw.lib")//链接时加入ddraw.lib库
class cdirectxwnd:public cwnd
{
//construction
public:
    cdirectxwnd();
    //attributes
public:
    bool blsactive;//应用程序是否被激活
protected:
    lpdirectdraw lpdd;//directdraw对象指针
    lpdirectdrawsurface lpfrontbuffer;//directdraw主缓冲区
    lpdirectdrawsurface lpbackbuffer;//directdraw后台缓冲区
    int nbuffercount;//后备缓冲区个数
    //operations
protected:
    void msg(lpstr fmt,...);
public:
    bool create(lpcstr lpszappname);//创建窗体
    //overrides
    virtual bool initializegame(uint gmodex,uint gmodey,uint gbpp);
    virtual bool cleansurface();
    virtual void updateframe();
    virtual bool restoresurfaces(void);
    //{{afx_virtual(cdirectxwnd)
    //}}afx_virtual
    //implementation
public:
    virtual ~cdirectxwnd();
    //generated message map functions
protected:
    //{{afx_msg(cdirectxwnd)
    afx_msg void onkeydown(uint nchar,uint nrepcnt,uint nflags);
    afx_msg void onactivateapp(bool bactive,htask htask);
    //}}afx_msg
    declare_message_map()
};
////////////////////{{afx_insert_location}}
#endif//!define(directxwnd_h)
文件：directxwnd.cpp
//directxwnd.cpp:implementation file
#include "stdafx.h"
#include "directx.h"
#include "directxwnd.h"
#ifdef _dubug
#define new debug_new
#undef this_file
static char this_file[]=__file__;
#endif
cdirectxwnd::cdirectxwnd()
{
    lpdd=null;
    lpfrontbuffer=null;
    lpbackbuffer=null;
    nbuffercount=0;
    blsactive=true;
}
cdirectxwnd::~cdirectxwnd()
{
    if(lpdd)
    {
        cleansurface();
        lpdd->release();
        lpdd=null;
    }
}
bool cdirectxwnd::create(lpcstr lpszappname)
{
    cstring classname=afxregisterwndclass(cs_dblclks,::loadcursor(null,idc_arrow),
        null,afxgetapp()->loadicon(idr_mainframe));
    return(cwnd::createex(ws_ex_appwindow,classname,lpszappname,
        ws_visible|ws_sysmenu|ws_popup,0,0,getsystemmetrics(sm_cxscreen),
        getsystemmetrics(sm_cyscreen),null,null));
}
bool cdirectxwnd::initializegame(uint gmodex,uint gmodey,uint gbpp)
{
    hresult ddrval;
    ddrval=directdrawcreate(null,&lpdd,null);
    if(ddrval!=dd_ok)
    {
        msg("directdrawcreate failed err=%d",ddrval);
        return false;
    }
    ddrval=lpdd->setcooperativelevel(m_hwnd,ddscl_exclusive|ddscl_fullscreen);
    if(ddrval!=dd_ok)
    {
        msg("setcooperativelevel failed err=%d",ddrval);
        return false;
    }
    ddrval=lpdd->setdisplaymode(gmodex,gmodey,gbpp);
    if(ddrval!=dd_ok)
    {
        msg("setdisplaymode failed err=%d",ddrval);
        return false;
    }
    //check capabilites
    ddcaps ddcaps;
    ddcaps.dwsize=sizeof(ddcaps);
    ddrval=lpdd->getcaps(&ddcaps,null);
    if(ddrval!=dd_ok)
    {
        msg("getcaps failed err=%d",ddrval);
        return false;
    }
    if(ddcaps.dwcaps&ddcaps_nohardware)
    {
        msg("no hardware support at all");
    }
    //default to double buffered on 1mb,triple buffered
    if(nbuffercount==0)
    {
        if(ddcaps.dwvidmemtotal<=1024l*1024l*(gbpp/8)||gmodex>640)
        {
            nbuffercount=2;
        }
        else
        {
            nbuffercount=3;
        }
    }
    ddsurfacedesc ddsd;
    ::zeromemory(&ddsd,sizeof(ddsd));
    ddsd.dwsize=sizeof(ddsd);
    ddsd.dwflags=ddsd_caps|ddsd_backbuffercount;
    ddsd.ddscaps.dwcaps=ddscaps_primarysurface|ddscaps_flip|ddscaps_complex;
    ddsd.dwbackbuffercount=nbuffercount-1;
    ddrval=lpdd->createsurface(&ddsd,&lpfrontbuffer,null);
    if(ddrval!=dd_ok)
    {
        msg("createsurface failed err=%d",ddrval);
        return false;
    }
    else msg("显示内存允许建立的缓冲区数=%d（最多需要3）",nbuffercount);
    ddscaps ddscaps;
    ddscaps.dwcaps=ddscaps_backbuffer;
    ddrval=lpfrontbuffer->getattachedsurface(&ddscaps,&lpbackbuffer);
    if(ddrval!=dd_ok)
    {
        msg("getattachedsurface failed err=%d",ddrval);
        return false;
    }
    return true;
}
void cdirectxwnd::msg(lpstr fmt,...)
{
    char buff[256];
    va_list va;
    lstrcpy(buff,"directxwnd:");
    va_start(va,fmt);
    wvsprintf(&buff[lstrlen(buff)],fmt,va);
    va_end(va);
    lstrcat(buff,"/r/n");
    afxmessagebox(buff);
}
////////////////////////virtual function
bool cdirectxwnd::restoresurfaces()
{
    hresult ddrval;
    ddrval=lpfrontbuffer->restore();
    if(ddrval!=dd_ok)
        return false;
    return true;
}
bool cdirectxwnd::cleansurface()
{
    if(lpbackbuffer)
    {
        lpbackbuffer->release();
        lpbackbuffer=null;
    }
    if(lpfrontbuffer)
    {
        lpfrontbuffer->release();
        lpfrontbuffer=null;
    }
    return true;
}
void cdirectxwnd::updateframe()
{
}
begin_message_map(cdirectxwnd,cwnd)
//{{afx_msg_map(cdirectxwnd,cwnd)
on_wm_keydown()
on_wm_activateapp()
//}}afx_msg_map
end_message_map()
///////////////////cdirectxwnd message handlers
void cdirectxwnd::onkeydown(uint nchar,uint nrepcnt,uint nflags)
{
    switch(nchar)
    {
    case vk_escape:
    case vk_f12:
        postmessage(wm_close);
        break;
    }
    cwnd::onkeydown(nchar,nrepcnt,nflags);
}
void cdirectxwnd::onactivateapp(bool bactive,htask htask)
{
    cwnd::onactivateapp(bactive,htask);
    blsactive=bactive;
    return;
}
请下载：1、directx.zip，工程框架源文件；2、ctestwnd.zip，测试程序源文件。
使用这两个源程序时不要直接编译，因为没有包含编译需要的所有文件，而应该根据本文提示做，需要输入代码的时候，将该文件替换成下载包中的相应文件就可以。另外，请在运行时不要用alt+tab切换出来，否则会被锁住，非reset不可。要想不被锁住，请深入研究directdraw技术修改此程序。----无心柳
