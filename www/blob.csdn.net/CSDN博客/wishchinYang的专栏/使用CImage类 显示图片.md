# 使用CImage类 显示图片 - wishchinYang的专栏 - CSDN博客
2013年07月17日 12:42:07[wishchin](https://me.csdn.net/wishchin)阅读数：1256
在不适用openCv的一种时候，使用CImage显示图片数据，并且直接嵌入DC框中。
使用CImage 在pic控件里显示图片
```cpp
void CMyCalLawsDlg::MyShowImage( CImage Image, UINT ID )
{
    CBitmap m_bmp;
    BITMAP bm;
    m_bmp.Attach(Image);
    m_bmp.GetObject(sizeof(BITMAP),&bm);
    //bm.bmWidth中保存图片的宽度
    //bm.bmHeight中保存图片的高度
    int ImageHeight =bm.bmHeight;
    int ImageWidth  =bm.bmWidth;
    CDC* pDC = GetDlgItem( ID ) ->GetDC();        // 获得显示控件的 DC wishchin
    HDC hDC = pDC ->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作
    Image.Draw(pDC->m_hDC,0,0,ImageWidth,ImageHeight);
    m_bmp.Detach();
}
```
调用实例：
```cpp
string  FileName.Format("E:\\SignSample\\Sample (%d).bmp",i);
        image.Load(FileName);
        int width =image.GetWidth();
        int height=image.GetHeight();
        MyShowImage(image,IDC_SHOWPIC);
```
