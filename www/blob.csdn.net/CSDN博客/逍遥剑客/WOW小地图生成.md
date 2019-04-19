# WOW小地图生成 - 逍遥剑客 - CSDN博客
2008年11月24日 22:53:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2727
参考wowmapview写了一个小地图查看的控件, 可以载入*.wdl文件解析出一张类似于"卫星图"的东西
WDL文件保存的是WOW地图的低精度高度数据, 冒似用来做远处的LOD的
每个地图最大是64*64个tile, 而一个tile它保存了17*17 + 16*16个16位的高度数据. 
如果生成一张512*512大小的小地图的话, 每个tile就只有8*8个像素
因此, 我们只取17*17中的偶数点
下面这张是Kalimdor的生成效果
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081124/WOWMiniMap.JPG)
#pragma once 
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::IO;
namespace WOWMinimap {
    typedef unsigned int uint;
    typedef unsigned char byte;
    /// <summary> 
    /// Summary for MiniMap 
    /// </summary> 
    public ref class MiniMap : public System::Windows::Forms::UserControl
    {
    public:
        static const int MINIMAP_SIZE = 512;
        static const int MAX_WIDTH_IN_TILE = 64;
        static const int MAX_TILE_NUM = MAX_WIDTH_IN_TILE * MAX_WIDTH_IN_TILE;
        static const int TILE_WIDTH_IN_PIXEL = 8;
        static const int TILE_WIDTH_IN_VERTEX = 17;
        static const int TILE_VERTEX_NUM = TILE_WIDTH_IN_VERTEX * TILE_WIDTH_IN_VERTEX;
    public:
        MiniMap(void)
        {
            InitializeComponent();
            // 
            //TODO: Add the constructor code here 
            // 
            this->bitmap = gcnew Bitmap(MINIMAP_SIZE, MINIMAP_SIZE, PixelFormat::Format32bppArgb);
        }
        void LoadMapHeight(String^ filename)
        {
            FileStream^ fs = gcnew FileStream(filename, FileMode::Open);
            BinaryReader^ br = gcnew BinaryReader(fs);
            // abort the version chunk 
            fs->Seek(0x10, SeekOrigin::Begin);
            // load tile's file offsets 
            array<uint>^ offsets = gcnew array<uint>(MAX_WIDTH_IN_TILE*MAX_WIDTH_IN_TILE);
            for (int i = 0; i < MAX_TILE_NUM; i++)
            {
                offsets[i] = br->ReadUInt32();
            }
            // load tile height 
            for (int i = 0; i < MAX_TILE_NUM; i++)
            {
                Console::WriteLine("{0}%", 100.0f * i / float(MAX_TILE_NUM-1));
                if (0 == offsets[i])
                {
                    // tile is not used 
                    continue;
                }
                LoadTileHeight(i, offsets[i], fs);
            }
            // repaint 
            this->Invalidate();
            fs->Close();
        }
    private:
        void LoadTileHeight(uint index, uint offset, FileStream^ fs)
        {
            fs->Seek(offset + 8, SeekOrigin::Begin);
            BinaryReader^ br = gcnew BinaryReader(fs);
            // read heights 
            array<short>^ heights = gcnew array<short>(TILE_VERTEX_NUM);
            for (int i = 0; i < TILE_VERTEX_NUM; i++)
            {
                heights[i] = br->ReadUInt16();
            }
            int left = index % MAX_WIDTH_IN_TILE * TILE_WIDTH_IN_PIXEL;
            int top = index / MAX_WIDTH_IN_TILE * TILE_WIDTH_IN_PIXEL;
            Rectangle rect(left, top, TILE_WIDTH_IN_PIXEL, TILE_WIDTH_IN_PIXEL);
            BitmapData^ bmpData = this->bitmap->LockBits(rect, ImageLockMode::WriteOnly, this->bitmap->PixelFormat);
            int* color = (int*)bmpData->Scan0.ToPointer();
            // convert to color 
            for (int z = 0; z < TILE_WIDTH_IN_PIXEL; z++)
            {
                for (int x = 0; x < TILE_WIDTH_IN_PIXEL; x++)
                {
                    short height = heights[(z * 2) * TILE_WIDTH_IN_VERTEX + x * 2];
                    color[x] = HeightToColor(height).ToArgb();
                }
                color += MINIMAP_SIZE;
            }
            this->bitmap->UnlockBits(bmpData);
        }
        Color HeightToColor(short height)
        {
            byte r1,r2,g1,g2,b1,b2;
            float t;
            byte r, g, b;
            if (height < 0)
            {
                // water = blue 
                if (height < -511) height = -511;
                height /= -2;
                r = g = 0;
                b = 255 - height;
            }
            else
            {
                // green: 20,149,7      0-600 
                // brown: 137, 84, 21   600-1200 
                // gray: 96, 96, 96     1200-1600 
                // white: 255, 255, 255 
                if (height < 600)
                {
                    r1 = 20;
                    r2 = 137;
                    g1 = 149;
                    g2 = 84;
                    b1 = 7;
                    b2 = 21;
                    t = height / 600.0f;
                }
                else if (height < 1200)
                {
                    r2 = 96;
                    r1 = 137;
                    g2 = 96;
                    g1 = 84;
                    b2 = 96;
                    b1 = 21;
                    t = (height-600) / 600.0f;
                }
                else /*if (height < 1600)*/
                {
                    r1 = 96;
                    r2 = 255;
                    g1 = 96;
                    g2 = 255;
                    b1 = 96;
                    b2 = 255;
                    if (height >= 1600) height = 1599;
                    t = (height-1200) / 600.0f;
                }
                r = (byte)(r2*t + r1*(1.0f-t));
                g = (byte)(g2*t + g1*(1.0f-t));
                b = (byte)(b2*t + b1*(1.0f-t));
            }
            return Color::FromArgb(r, g, b);
        }
    protected:
        /// <summary> 
        /// Clean up any resources being used. 
        /// </summary> 
        ~MiniMap()
        {
            if (components)
            {
                delete components;
            }
        }
        virtual void OnPaint(PaintEventArgs^ e) override
        {
            e->Graphics->DrawImage(this->bitmap, 0, 0, this->Width, this->Height);
        }
    private:
        /// <summary> 
        /// Required designer variable. 
        /// </summary> 
        System::ComponentModel::Container ^components;
        Bitmap^ bitmap;
#pragma region Windows Form Designer generated code 
        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor. 
        /// </summary> 
        void InitializeComponent(void)
        {
            this->SuspendLayout();
            //  
            // MiniMap 
            //  
            this->Name = L"MiniMap";
            this->SizeChanged += gcnew System::EventHandler(this, &MiniMap::MiniMap_SizeChanged);
            this->ResumeLayout(false);
        }
#pragma endregion 
    private: System::Void MiniMap_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
                 this->Invalidate();
             }
};
}
