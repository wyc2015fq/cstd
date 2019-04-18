# HGE系列之四    小试牛刀 - tkokof1的专栏 - CSDN博客

2010年04月04日 09:49:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：2291



HGE系列之四小试牛刀

经过前面几次的摸索，我们终算是对于HGE有了一些浅显的感性认识，那么接下来我想便是时候自己操刀，来尝试着使用HGE自己编写游戏了！ ：）

那么应该编写一个什么游戏呢？是那种画面绚烂的养眼型，还是那种极富创意的益智型，亦或是干脆搞个个人版的“魔兽争霸”？的确，这些听起来都是那么令人激情澎湃，但是我还是必须即时的泼上点冷水以让你冷静下来：首先，拥有华丽画面的游戏，编写起来都不简单，甚至可以说是复杂；再者优秀的创意更不是俯拾即是，更不必说在当下这个“游戏倍出”的年代了；而妄想自个儿搞出“魔兽争霸”的朋友，你以为Blizzard的那群家伙是吃干饭的吗，“魔兽争霸”岂是一人说搞就能搞的呢？

当然，如果你够牛够坚持，以上的问题还是可以解决的，但是我们本次的目的仅仅是通过编写一个游戏来进一步熟悉HGE，仅此而已 ：）

为了最大化的省却前期的策划、美术等问题，我干脆在HGE官网上找到了一个[简单的游戏](http://gamezdev.fatal.ru/files/15game.rar)，虽说很小，但是绝对可算是个完整的游戏了，我下载了他的代码和运行文件，发现了一点内存泄露，稍稍修正了一下，并加入了简单的粒子系统，最后，由于该游戏年代“古老”的缘故，并不能在HGE最新版中完成编译，我也顺便来了次“升级”，最终，我们第一个HGE游戏终于完成了 ！！！样子就像这样：

![](http://hi.csdn.net/attachment/201005/16/0_12739723455YK9.gif)

好了，就让我们细细的回顾一下这个简单游戏的代码，相信能给我们一些关于HGE的启示：

首先是settings.h这个头文件，让我们看看他里面究竟是些什么东西：

    /* some pre-defined data */

    #ifndef SETTINGS_H

    #define SETTINGS_H

    // below data are self-documentation :)

    #define ROWS4

    #define COLUMNS4

    #define DESK_X230

    #define DESK_Y140

    #endif  

原来是几个常量的预定义宏，名字上都很清晰，如果你认真看过游戏的运行效果图，相信不难猜出这几个常量的意义；再者，也许有些朋友习惯将这些不便的预定义宏直接编写在某个类文件的头部或是main函数的前面，但其实，单独辟出一个文件来进行管理更加清晰明白，也更具有维护性；最后值得一提的是，在C++中，这些直接进行文本替换的宏其实早就不被提倡，完全可以以const常量来达到更好的效果，而#ifndef预编译宏则是所谓的编译哨岗，用以解决头文件重复包含的问题。

接着让我们看看game.h文件的内容：

    /* the logic implementation of the Game */

    #ifndef CGAME_H

    #define CGAME_H

    #include "settings.h"// include some pre-defined data

    class CGame

    {

    public:

CGame();

~CGame();

int GetCell( int row, int col );// return the value of the Desk[row][col]

bool MoveIt( int row, int col );// move the value of the position

void Reset();// reset the values of the Desk Array

void Mix( int quality );// mix the value of the Desk Array

int GetMoves() { return m_moves; };// get the move counts

    private:

intm_desk[ROWS][COLUMNS];// the array to store the value

intm_width;// the array_width

intm_height;// the array_height

intm_moves;// the move counts

    };

    #endif

代码相当简洁，说白了，就是游戏的逻辑表示，相关方法的描述都已经经过注释，想必在此大可不必赘言，如果你再有不解之处，相信瞅瞅game.cpp文件应该就无大碍了 ：）

既然game文件实现的是游戏的逻辑，那么游戏的视图表示应该由谁来负责呢？哈哈，就是这个imager类，不多废话，让我们先看看他的头文件：

    /* the graphic implementation of the game */

    #ifndef CIMAGER_H

    #define CIMAGER_H

    #include "hge.h"

    #include "hgesprite.h"

    #include "hgefont.h"

    #include "game.h"

    class CImager

    {

    public:

CImager( CGame* _game );

~CImager();

void LoadGraphics( HTEXTURE* tex );// load the graphic( use HGE-defined type HTEXTURE* )

void DrawScene( int mousex, int mousey );// draw the game 

void Click( int mousex, int mousey );// handle the click of the position( mousex, mousey )

void SetXY( int _x, int _y ) { m_x = _x; m_y = _y; }// set the beginning position of the drawing

void IncScale() { if ( m_scale >= 5 ) return; m_scale += 0.05f; } // inc the scale of the number

void DecScale() { if ( m_scale <= 0 ) return; m_scale -= 0.05f; } // dec the scale of the number

    private:

CGame* m_game;

HTEXTURE*m_cells_tex;

hgeSprite*m_cell_sprs[ROWS*COLUMNS-1];

hgeFont*m_fnt;

intm_x;

int     m_y;

intm_curx;

int     m_cury;

floatm_scale;

floatm_angle;

floatm_rot_time;

    };

    #endif

由于涉及了图形显示，imager文件则自然要与HGE建立必要的联系，大体的思路就是：通过CImager的翻译，将CGame类的数据信息传达给HGE，以最终显示图形，用这种角度来看，CImager更像是一个“中介者”、或者说“适配器” ：）

让我们再细细的看一下CImager的实现：

首先，由于游戏的图形显示必须依赖于游戏相关数据，而这些数据都是由CGame类来控制，所以CImager必须以某种方式与CGame建立联系，以获取游戏显示的必要信息，当前CImager采用的方式是保存一个CGame的指针，并通过其构造函数参数传递进去，虽然这种方式有其潜在的资源控制问题，但是作为示例来讲，还是很足够了：

    CImager::CImager( CGame* _game )

    {

m_game = _game;

    }

再者，对于图形显示资源的管理，CImager做的也是十分简单，直接在LoadGraphics函数中加载进来便是，值得注意一下的便是该函数的参数HTEXTURE，如果你仔细看过HGE的文档，你便会知道HTEXTURE这个类型其实只是一个简单DWORD typedef，而如果你再看过HGE关于这方面的源码的话，你便会进一步明晰其实这个DWORD皆是由DX8中的纹理资源指针（LPDIRECT3DTEXTURE8）强制转换而来 ，这种方式让人不禁想起C语言中的void使用：虽然“法力无边”，但同时也“危机重重”，本人不才，但仍认为HGE在这个方面需要进一步改善。。。好了，说的有些离题，还是让我们看看该方法的实现吧：

    // load the graphic( use HGE-defined type HTEXTURE* )

    void CImager::LoadGraphics( HTEXTURE* tex )

    {

m_cells_tex = tex;

m_fnt = new hgeFont( "font1.fnt" );// just load the default font, just for simple :)

// set the m_cell_sprs

for ( int i = 0; i < ROWS*COLUMNS-1; ++i ) 

{

// construct the hgeSprite( each one is 40*40 big )

m_cell_sprs[i] = new hgeSprite( *m_cells_tex, i*40, 0, 40, 40 );

// set the sprite's hotspot

m_cell_sprs[i]->SetHotSpot( 20, 20 );

}

m_x = DESK_X;

m_y = DESK_Y;

m_curx = -1;

m_cury = -1;

m_scale = 2;

m_angle = 0;

    }

代码并不复杂，值得一说的可能就是m_cell_sprs数组了，在游戏中，他代表那15个数字图形表示的集合（如果你不明白我在说什么，请运行一下游戏 ：） ），CImager中实现的方法就是依赖传递进来的HTEXTURE一个个的创建hgeSprite，一个hgeSprite即代表一个数字，最后，默认设置一些用以显示参数，如m_scale（数字显示的大小比例）、m_angle（数字显示的角度）等等。

资源既然已经OK，那么接下来的问题就是如何显示，让我们顺势来看一下DrawScene的实现代码：

    // draw the game scene

    void CImager::DrawScene( int mousex, int mousey )

    {

int _x,_y;

        int n;

// if the cursor is moved on to it

bool mouse_in = false;

for ( int i = 0; i< ROWS; ++i )

for ( int j = 0; j< COLUMNS; ++j )

{

n = m_game->GetCell( i, j );

// set the drawing position 

_x = m_x+j*80;

_y = m_y+i*80;

//hgeRect* rect = new hgeRect();

hgeRect rect;

// check the boundary

if ( (n >= 0) && (n < ROWS*COLUMNS-1) )

{

// get the bounding box of the m_cell_sprs[n]

m_cell_sprs[n]->GetBoundingBoxEx( (float)_x, (float)_y, (float)m_angle, (float)m_scale, 1.0f/*vscale*/, &rect );

// test the point ( mousex, mousey )

mouse_in = rect.TestPoint( mousex, mousey );

} 

else

mouse_in = false;

if ( mouse_in ) 

{

m_curx = j;

m_cury = i;

// adjust the coordination

_x -= 3;

_y -= 3;

}

if ( (n >= 0) && (n < ROWS*COLUMNS-1) )

{

// if the sprite is mouse_in

if ( (j == m_curx) && (i == m_cury) )

{

// render the sprite

m_cell_sprs[n]->SetColor( 0xFFE83724 );

m_cell_sprs[n]->RenderEx( _x, _y, m_angle-0.2, m_scale );// pay attention the "m_angle-0.2"

}

else

{

m_cell_sprs[n]->SetColor( 0xFFF1CA15 );

m_cell_sprs[n]->RenderEx( _x, _y, m_angle, m_scale );

}

}

}

        // show the text information

DWORD tmp = m_game->GetMoves();

m_fnt->printf( 30, 20, HGETEXT_LEFT, "Moves counter: %d", tmp );

m_fnt->printf( 30, 520, HGETEXT_LEFT, "Hold SHIFT - Move the Desk | A & Z - Scale Cells" );

m_fnt->printf( 30, 550, HGETEXT_LEFT, "Left Button - Move | Right Button - Mix | Space - Reset | Esc - Exit" );

    }

相关代码果然未有出乎意料，

        for ( int i = 0; i< ROWS; ++i )

for ( int j = 0; j< COLUMNS; ++j )

以上双重循环显然是用以逐个显示那15个数字，当然还有一个空位置需要处理，所以16便是最终的循环次数，值得再提一下的便是循环中的mouse_in变量，他的作用便是用以标示鼠标是否停置于该数字上，如果是则采用特殊的显示方式（通过设置颜色和角度），如果不是则采用普通的显示方法，最后，使用m_fnt显示出相应的信息，另外提一点，HGE在实现字体类时使用的是位图存储字元（点阵位图）的方式，目前暂不支持中文显示。

剩下Click方法，CImager便仅仅是将其转发至内部的CGame罢了，而CGame则是处理相应的游戏逻辑（即移动棋子）：

    void CImager::Click( int mousex, int mousey )

    {

// just transmit to the m_game

m_game->MoveIt( m_cury, m_curx );

    }

    // move the value of the position

    bool CGame::MoveIt( int row, int col )

    {

bool can_move = false;

// check the boundary

if ( (row < 0) || (row > m_height) || (col < 0) || (col > m_width) )

return false;

// the position is empty

if ( m_desk[row][col] == -1 )

return false;

// the left position is empty

if ( m_desk[row][col-1] == -1 )

{

m_desk[row][col-1] = m_desk[row][col];

m_desk[row][col] = -1;

can_move = true;

}

// the codes below are a little ugly ... 

// the right position is empty

if ( m_desk[row][col+1] == -1 )

{

m_desk[row][col+1] = m_desk[row][col];

m_desk[row][col] = -1;

can_move = true;

}

// the up position is empty

if ( m_desk[row-1][col] == -1 )

{

m_desk[row-1][col] = m_desk[row][col];

m_desk[row][col] = -1;

can_move = true;

}

// the below position is empty

if ( m_desk[row+1][col] == -1 )

{

m_desk[row+1][col] = m_desk[row][col];

m_desk[row][col] = -1;

can_move = true;

}

if ( can_move ) 

++m_moves;

return can_move;

    }

最后，资源的释放也是必不可少的 ：）（ PS：C++最令人诟病的一点可能就是复杂的内存管理了，不信的人士大可以看看网上那些硝烟弥漫的论坛，不过就我个人而言，内存自动管理与否基本只是一个权衡问题，仅是问题相关的，或者说领域相关的，没有孰对孰错之分，更不能以其为理由来评价语言的好坏 ）

    CImager::~CImager()

    {

        for( int i = 0; i < ROWS*COLUMNS - 1; ++i )

        {

            if( m_cell_sprs[i] ) delete m_cell_sprs;

        }

        if( m_fnt ) delete m_fnt;

    }    

至此，游戏中的所有类以及其的工作职责想必大家都一一了然于胸了，那么是时候让WinMain登场了，不过之前呢还需要看一下这两个函数：

    // frame funcation, HGE will call it every frame

    bool FrameFunc()

    {

if ( hge->Input_GetKeyState( HGEK_ESCAPE ) ) return true;// end the game loop

if ( hge->Input_GetKeyState( HGEK_A ) ) Imager->IncScale();// inc the number

if ( hge->Input_GetKeyState( HGEK_Z ) ) Imager->DecScale();// dec the number

if ( hge->Input_GetKeyState( HGEK_LBUTTON ) ) Imager->Click( (int)x,(int)y );// handle the click

if ( hge->Input_GetKeyState( HGEK_RBUTTON ) ) Game->Mix( 100 );// mix the game

if ( hge->Input_GetKeyState( HGEK_SPACE ) ) Game->Reset();// reset the game

if ( hge->Input_GetKeyState( HGEK_SHIFT ) ) Imager->SetXY( (int)x-120,(int)y-120 );// set the beginning coordination of the drawing

hge->Input_GetMousePos( &x, &y );// get the mouse coordination

float dt = hge->Timer_GetDelta();

par->MoveTo( x, y );

par->Update( dt );

return false;

    }

    bool RenderFunc()

    {

        hge->Gfx_BeginScene();// begin render

// render the background

back->Render( 0, 0 );

// render the sprites

Imager->DrawScene( (int)x,(int)y );

// render the cursor

spr->Render( x, y );

// render the particles

par->Render();

        // render the frame information

fnt->printf( 500, 20, HGETEXT_LEFT, "FPS: %d", hge->Timer_GetFPS() );

hge->Gfx_EndScene();// end render

return false;

    }

首先我们来看一下编写HGE游戏程序必须熟知的FrameFunc和RenderFunc（如果对此不太清楚的朋友，也许我以前的[几篇博客](http://blog.csdn.net/tkokof1/category/642710.aspx)可能对你有些帮助），前者主管游戏每帧的逻辑处理，后者则主负游戏每帧渲染的重任，两者相互配合，以使游戏正常的运作 ：）依就上面的代码来看，FrameFunc函数相对简单，仅是对于几个按键的处理，最后需要关注一下的可能就是处在最后位置的这两行：

        par->MoveTo( x, y );

par->Update( dt );

其实现的便是粒子系统的更新，欲详细了解的朋友，请参阅[HGE文档及实现源码](http://hge.relishgames.com/)：）

而RenderFunc则更加简单，仅是一个个的调用相应的渲染函数罢了。

最后，让我们看看WinMain吧：

    int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )

    {

// create the HGE object

hge = hgeCreate( HGE_VERSION );

// set the frame function

hge->System_SetState( HGE_FRAMEFUNC, FrameFunc );

// set the render function

hge->System_SetState( HGE_RENDERFUNC, RenderFunc );

// set the program windowed

hge->System_SetState( HGE_WINDOWED, true );

// set not to use sound function

hge->System_SetState( HGE_USESOUND, false );

// set the program title

hge->System_SetState( HGE_TITLE, "Simple-Game" );

if( hge->System_Initiate() )// ini the HGE

{

LoadIt();// load the resource

hge->System_Start();// Let's Rock !!!

}

else

{

// if can't initiate the HGE, post error 

MessageBox( NULL, hge->System_GetErrorMessage(), "Error",

MB_OK | MB_ICONERROR | MB_SYSTEMMODAL );

}

        ReleaseIt();// release the resource

hge->System_Shutdown();// shut down the HGE

hge->Release();// release HGE object

return 0;

    }

没想到竟然这么简单 ！！！初期创建HGE示例，然后调用System_SetState函数以设置游戏运行的各类参数，接着System_Initiate根据给定的参数尝试初始化HGE，如果成功，那么万事顺利，加载必要的游戏资源，然后运行游戏：

            LoadIt();// load the resource

hge->System_Start();// Let's Rock !!!

如果不幸中途夭折，那么使用System_GetErrorMessage函数，配以Windows下标志性的MessageBox来输出错误信息，并跳出循环：

            MessageBox( NULL, hge->System_GetErrorMessage(), "Error",

MB_OK | MB_ICONERROR | MB_SYSTEMMODAL );

最后，在游戏循环跳出时，调用相应的释放函数，以作最后一次的清扫工作，然后便是平稳着陆，一切就是这么简单 ：）

整个工程的源码与可执行文件在[这里](http://download.csdn.net/source/2201231)，由于某些“不足为外人道也”的原因，我的编程环境是Dev-C++，有兴趣的朋友可以[点此](http://zh.wikipedia.org/wiki/Dev-C%2B%2B)开始了解，另外，对于不知如何在IDE中配置HGE的朋友，我的[这篇拙文](http://blog.csdn.net/tkokof1/archive/2009/12/29/5098806.aspx)中可能有些对你有用的信息 ：）

好了，差不多了，让我们 下次再见吧 ：）


