
#include "cstd.h"
#include "img/imgopt.inl"
#define IsValid()  NULL!=(im)->tt.data
#include "imgex.inl"
#include "draw/imdraw.inl"
#include "img3d.inl"
//#include "EffectorBuilder.inl"

#define MAX_LOADSTRING 100

// Global Variables:

int test_imgex()
{
	// Initialize global strings
	// Replace color in the images
#if 0
  CEffectorBuilder g_EffectorBuilder;
	CBitmapEx bitmap;
	bitmap.Load(_T("images\\image06.bmp"));
	bitmap.ReplaceColor(345, 275, _RGB(255,0,0), 10, 150, FALSE);		// Sea rock
	bitmap.ReplaceColor(105, 305, _RGB(255,255,0), 10, 90, FALSE);		// Bottom of the sea
	bitmap.Save(_T("images\\image06_recolor.bmp"));
	bitmap.Load(_T("images\\enterior.bmp"));
	bitmap.ReplaceColor(340, 110, _RGB(0,0,255), 40, 193, FALSE);		// Wall shelf
	bitmap.ReplaceColor(475, 230, _RGB(255,0,0), 30, 60, FALSE);		// Chairs
	bitmap.ReplaceColor(435, 280, _RGB(255,0,0), 30, 40, FALSE);
	bitmap.ReplaceColor(505, 245, _RGB(255,0,0), 30, 40, FALSE);
	bitmap.ReplaceColor(460, 300, _RGB(255,0,0), 30, 50, FALSE);
	bitmap.ReplaceColor(485, 280, _RGB(255,0,0), 30, 50, FALSE);
	bitmap.ReplaceColor(475, 290, _RGB(255,0,0), 30, 80, FALSE);
	bitmap.ReplaceColor(458, 270, _RGB(255,0,0), 30, 80, FALSE);
	bitmap.ReplaceColor(398, 300, _RGB(255,0,0), 30, 80, FALSE);
	bitmap.ReplaceColor(400, 277, _RGB(255,0,0), 30, 80, FALSE);
	bitmap.ReplaceColor(40, 280, _RGB(0,255,0), 30, 42, FALSE);			// Floor shelf
	bitmap.ReplaceColor(10, 295, _RGB(0,255,0), 30, 30, FALSE);
	bitmap.ReplaceColor(120, 280, _RGB(0,255,0), 30, 20, FALSE);
	bitmap.ReplaceColor(190, 290, _RGB(0,255,0), 30, 30, FALSE);
	bitmap.ReplaceColor(275, 295, _RGB(0,255,0), 30, 22, FALSE);
	bitmap.ReplaceColor(245, 306, _RGB(0,255,0), 30, 20, FALSE);
	bitmap.ReplaceColor(350, 300, _RGB(0,255,0), 30, 20, FALSE);
	bitmap.ReplaceColor(315, 280, _RGB(0,255,0), 30, 20, FALSE);
	bitmap.ReplaceColor(395, 295, _RGB(0,255,0), 30, 15, FALSE);
	bitmap.ReplaceColor(407, 290, _RGB(0,255,0), 30, 15, FALSE);
	bitmap.ReplaceColor(405, 305, _RGB(0,255,0), 30, 15, FALSE);
	bitmap.ReplaceColor(422, 307, _RGB(0,255,0), 30, 15, FALSE);
	bitmap.ReplaceColor(470, 40, _RGB(255,255,0), 30, 50, FALSE);		// Right wall
	bitmap.ReplaceColor(500, 200, _RGB(255,255,0), 30, 50, FALSE);
	bitmap.ReplaceColor(455, 160, _RGB(255,255,0), 30, 70, FALSE);
	bitmap.ReplaceColor(447, 260, _RGB(255,255,0), 30, 20, FALSE);
	bitmap.ReplaceColor(465, 260, _RGB(255,255,0), 30, 30, FALSE);
	bitmap.ReplaceColor(505, 280, _RGB(255,255,0), 30, 50, FALSE);
	bitmap.ReplaceColor(500, 300, _RGB(255,255,0), 30, 30, FALSE);
	bitmap.ReplaceColor(507, 232, _RGB(255,255,0), 30, 50, FALSE);
	bitmap.ReplaceColor(20, 10, _RGB(255,255,0), 30, 35, FALSE);		// Left wall
	bitmap.Save(_T("images\\enterior_recolor.bmp"));

	// Change image brightness
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Brightness(32);
	bitmap.Save(_T("images\\image09_brightness.bmp"));

	// Change image contrast
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Contrast(5);
	bitmap.Save(_T("images\\image09_contrast.bmp"));

	// Blur image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Blur();
	bitmap.Save(_T("images\\image09_blur.bmp"));

	// Gaussian blur image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.GaussianBlur();
	bitmap.Save(_T("images\\image09_gblur.bmp"));

	// Sharp image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Sharp();
	bitmap.Save(_T("images\\image09_sharp.bmp"));

	// Colorize image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Colorize(_RGB(255,0,0));
	bitmap.Save(_T("images\\image09_colorize.bmp"));

	// Rank image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Rank();
	bitmap.Save(_T("images\\image09_min.bmp"));
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Rank(FALSE);
	bitmap.Save(_T("images\\image09_max.bmp"));

	// Spread image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Spread();
	bitmap.Save(_T("images\\image09_spread.bmp"));

	// Offset image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Offset(100, 100);
	bitmap.Save(_T("images\\image09_offset.bmp"));

	// Black and white image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.BlackAndWhite();
	bitmap.Save(_T("images\\image09_bw.bmp"));

	// Edge detect image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.EdgeDetect();
	bitmap.Save(_T("images\\image09_edge.bmp"));

	// Glowing edges image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.GlowingEdges();
	bitmap.Save(_T("images\\image09_glow.bmp"));

	// Histogram equalize image
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.EqualizeHistogram();
	bitmap.Save(_T("images\\image09_eh.bmp"));

	// Median filter bitmap
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Median();
	bitmap.Save(_T("images\\image09_median.bmp"));

	// Posterize bitmap
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Posterize();
	bitmap.Save(_T("images\\image09_posterize.bmp"));

	// Solarize bitmap
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.Solarize();
	bitmap.Save(_T("images\\image09_solarize.bmp"));

	// Create drop shadow effect
	CBitmapEx bitmap1, bitmap2, bitmap3;
	bitmap1.Load(_T("images\\image09.bmp"));
	bitmap2.Load(_T("images\\image10.bmp"));
	bitmap3.Create(bitmap2.GetWidth(), bitmap2.GetHeight());
	bitmap3.DrawMasked(0, 0, bitmap2.GetWidth(), bitmap2.GetHeight(), bitmap2, 0, 0, bitmap2.GetPixel(0, 0));
	bitmap3.Blur();
	bitmap3.Blur();
	bitmap3.Blur();
	bitmap1.DrawAlpha(4, 4, bitmap3.GetWidth(), bitmap3.GetHeight(), bitmap3, 0, 0, 25, _RGB(0,0,0));
	bitmap1.DrawTransparent(0, 0, bitmap2.GetWidth(), bitmap2.GetHeight(), bitmap2, 0, 0, 50, bitmap2.GetPixel(0, 0));
	bitmap1.Save(_T("images\\image09_shadow.bmp"));

	// Draw text on the bitmap
	bitmap.Load(_T("images\\image09.bmp"));
	bitmap.DrawText(10, 0, _T("0123456789"), _RGB(255,0,0), 50, _T("Times New Roman"), 16);
	bitmap.DrawText(10, 30, _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), _RGB(255,0,0), 50, _T("Arial"), 16);
	bitmap.DrawText(10, 60, _T("abcdefghijklmnopqrstuvwxyz"), _RGB(255,0,0), 50, _T("Verdana"), 16);
	bitmap.DrawText(10, 90, _T("`~!@#$%^&*()_+[]{};':\",.<>/?"), _RGB(255,0,0), 50, _T("Tahoma"), 16);
	bitmap.DrawText(50, 180, _T("Hello, World !!!"), _RGB(0,0,255), 50, _T("Times New Roman"), 16, FALSE, FALSE);
	bitmap.DrawText(50, 210, _T("Hello, World !!!"), _RGB(0,0,255), 50, _T("Arial"), 16, TRUE, FALSE);
	bitmap.DrawText(50, 240, _T("Hello, World !!!"), _RGB(0,0,255), 50, _T("Verdana"), 16, FALSE, TRUE);
	bitmap.DrawText(50, 270, _T("Hello, World !!!\nThis is a multiline text message..."), _RGB(0,0,255), 50, _T("Tahoma"), 16, TRUE, TRUE);
	bitmap.Save(_T("images\\image09_text.bmp"));

	// Add images
	CBitmapEx* pFireBitmap = new CBitmapEx();
	pFireBitmap->Load(_T("images\\image09.bmp"));
	pFireBitmap->Scale(50, 50);
	CBitmapEx* pWaterBitmap = new CBitmapEx();
	pWaterBitmap->Load(_T("images\\image09.bmp"));
	pWaterBitmap->Scale(50, 50);
	CBitmapEx* pSmokeBitmap = new CBitmapEx();
	pSmokeBitmap->Load(_T("images\\image09.bmp"));
	pSmokeBitmap->Scale(50, 50);
	g_EffectorBuilder.AddImage(_T("images\\image01.bmp"));
	g_EffectorBuilder.AddImage(_T("images\\image02.bmp"));
	g_EffectorBuilder.AddImage(_T("images\\image03.bmp"));
	g_EffectorBuilder.AddImage(_T("images\\image04.bmp"));
	g_EffectorBuilder.AddImage(_T("images\\image05.bmp"));
	g_EffectorBuilder.AddImage(_T("images\\image06.bmp"));
	g_EffectorBuilder.AddImage(_T("images\\image07.bmp"));
	g_EffectorBuilder.AddImage(_T("images\\image08.bmp"));
	g_EffectorBuilder.AddImage(_T("images\\image09.bmp"));
	g_EffectorBuilder.AddImage(pFireBitmap);
	g_EffectorBuilder.AddImage(pWaterBitmap);
	g_EffectorBuilder.AddImage(pSmokeBitmap);
#define morphQuad1  {{50, 50}, {0, 0}, {-50, -50}, {0, 0}}
	#define morphQuad2  {{0, 0}, {-50, 50}, {0, 0}, {50, -50}}
	#define morphQuad3  {{50, 50}, {0, 0}, {0, 0}, {50, -50}}
	#define morphQuad4  {{50, 0}, {0, 50}, {0, -50}, {50, 0}}
	#define morphQuad5  {{0, 50}, {0, 50}, {-50, 0}, {50, 0}}
	#define morphQuad6  {{50, 50}, {-50, 50}, {0, 0}, {0, 0}}
	POINT ptPosition1[] = {{200, 100}, {200, 100}};
	POINT ptPosition2[] = {{450, 100}, {450, 100}};
	POINT ptPosition3[] = {{700, 100}, {700, 100}};
	POINT ptPosition4[] = {{200, 500}, {200, 500}};
	POINT ptPosition5[] = {{450, 500}, {450, 500}};
	POINT ptPosition6[] = {{700, 500}, {700, 500}};
	POINT ptPosition7[] = {{200, 300}, {700, 300}};
	POINT ptPosition8[] = {{700, 300}, {200, 300}};
	POINT ptPosition9[] = {{450, 300}};
	POINT ptPosition10[] = {{450, 300}};
	POINT ptPosition11[] = {{120, 300}};
	POINT ptPosition12[] = {{800, 300}};
	_ANIMDESC2 lpAnimDesc1[] = {
    {AT_ALPHA|AT_SIZE|AT_ROTATE, 1000, 0, 0, ptPosition1[0].x, ptPosition1[0].y, ptPosition1[1].x, ptPosition1[1].y, 20, 50, 0, 360, 64, 192, DM_NONE, false, FT_SEPIA},
    {AT_ALPHA|AT_SIZE|AT_ROTATE|AT_REPLAY, 1000, 0, 0, ptPosition1[1].x, ptPosition1[1].y, ptPosition1[0].x, ptPosition1[0].y, 50, 20, 360, 0, 192, 64, DM_NONE, true, FT_SEPIA}
  };
	_ANIMDESC2 lpAnimDesc2[] = {{AT_ALPHA|AT_SIZE|AT_ROTATE, 1000, 0, 0, ptPosition2[0].x, ptPosition2[0].y, ptPosition2[1].x, ptPosition2[1].y, 20, 50, 360, 0, 64, 192, DM_NONE, false, FT_NONE},      {AT_ALPHA|AT_SIZE|AT_ROTATE|AT_REPLAY, 1000, 0, 0, ptPosition2[1].x, ptPosition2[1].y, ptPosition2[0].x, ptPosition2[0].y, 50, 20, 0, 360, 192, 64, DM_NONE, true, FT_NONE}};
	_ANIMDESC2 lpAnimDesc3[] = {{AT_ALPHA|AT_SIZE|AT_ROTATE, 1000, 0, 0, ptPosition3[0].x, ptPosition3[0].y, ptPosition3[1].x, ptPosition3[1].y, 20, 50, 0, 360, 64, 192, DM_NONE, false, FT_GRAYSCALE}, {AT_ALPHA|AT_SIZE|AT_ROTATE|AT_REPLAY, 1000, 0, 0, ptPosition3[1].x, ptPosition3[1].y, ptPosition3[0].x, ptPosition3[0].y, 50, 20, 360, 0, 192, 64, DM_NONE, true, FT_GRAYSCALE}};
	_ANIMDESC2 lpAnimDesc4[] = {{AT_ALPHA|AT_SIZE|AT_ROTATE, 1000, 0, 0, ptPosition4[0].x, ptPosition4[0].y, ptPosition4[1].x, ptPosition4[1].y, 20, 50, 360, 0, 64, 192, DM_NONE, false, FT_EMBOSS},    {AT_ALPHA|AT_SIZE|AT_ROTATE|AT_REPLAY, 1000, 0, 0, ptPosition4[1].x, ptPosition4[1].y, ptPosition4[0].x, ptPosition4[0].y, 50, 20, 0, 360, 192, 64, DM_NONE, true, FT_EMBOSS}};
	_ANIMDESC2 lpAnimDesc5[] = {{AT_ALPHA|AT_SIZE|AT_ROTATE, 1000, 0, 0, ptPosition5[0].x, ptPosition5[0].y, ptPosition5[1].x, ptPosition5[1].y, 20, 50, 0, 360, 64, 192, DM_NONE, false, FT_NONE},      {AT_ALPHA|AT_SIZE|AT_ROTATE|AT_REPLAY, 1000, 0, 0, ptPosition5[1].x, ptPosition5[1].y, ptPosition5[0].x, ptPosition5[0].y, 50, 20, 360, 0, 192, 64, DM_NONE, true, FT_NONE}};
	_ANIMDESC2 lpAnimDesc6[] = {{AT_ALPHA|AT_SIZE|AT_ROTATE, 1000, 0, 0, ptPosition6[0].x, ptPosition6[0].y, ptPosition6[1].x, ptPosition6[1].y, 20, 50, 360, 0, 64, 192, DM_NONE, false, FT_ENGRAVE},   {AT_ALPHA|AT_SIZE|AT_ROTATE|AT_REPLAY, 1000, 0, 0, ptPosition6[1].x, ptPosition6[1].y, ptPosition6[0].x, ptPosition6[0].y, 50, 20, 0, 360, 192, 64, DM_NONE, true, FT_ENGRAVE}};
	_ANIMDESC2 lpAnimDesc7[] = {{AT_ALPHA|AT_MOVE|AT_SIZE, 1000, 0, 0,  ptPosition7[0].x, ptPosition7[0].y, ptPosition7[1].x, ptPosition7[1].y, 20, 50, 0, 0, 64, 192, DM_NONE, false, FT_PIXELIZE},       {AT_ALPHA|AT_MOVE|AT_SIZE|AT_REPLAY, 1000, 0, 0, ptPosition7[1].x, ptPosition7[1].y, ptPosition7[0].x, ptPosition7[0].y, 50, 20, 0, 0, 192, 64, DM_NONE, true, FT_PIXELIZE}};
	_ANIMDESC2 lpAnimDesc8[] = {{AT_ALPHA|AT_MOVE|AT_SIZE, 1000, 0, 0,  ptPosition8[0].x, ptPosition8[0].y, ptPosition8[1].x, ptPosition8[1].y, 20, 50, 0, 0, 64, 192, DM_NONE, false, FT_NEGATIVE},       {AT_ALPHA|AT_MOVE|AT_SIZE|AT_REPLAY, 1000, 0, 0, ptPosition8[1].x, ptPosition8[1].y, ptPosition8[0].x, ptPosition8[0].y, 50, 20, 0, 0, 192, 64, DM_NONE, true, FT_NEGATIVE}};
	                _ANIMDESC2 lpAnimDesc9[] =  {{AT_ALPHA, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 0, 128, DM_RANDOM, false, FT_NONE}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 128, 0, DM_NONE, false, FT_NONE, morphQuad1}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 0, 128, DM_NONE, true, FT_FLIPVERTICAL|FT_FLIPHORIZONTAL, morphQuad1}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 128, 0, DM_NONE, false, FT_FLIPVERTICAL|FT_FLIPHORIZONTAL, morphQuad2}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 0, 128, DM_NONE, true, FT_NONE, morphQuad2}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 128, 0, DM_NONE, false, FT_NONE, morphQuad3}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 0, 128, DM_NONE, true, FT_FLIPHORIZONTAL, morphQuad4}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 128, 0, DM_NONE, false, FT_FLIPHORIZONTAL, morphQuad4},
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 0, 128, DM_NONE, true, FT_NONE, morphQuad3}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 128, 0, DM_NONE, false, FT_NONE, morphQuad5}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 0, 128, DM_NONE, true, FT_FLIPVERTICAL, morphQuad6}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 128, 0, DM_NONE, false, FT_FLIPVERTICAL, morphQuad6}, 
								                      {AT_ALPHA|AT_MORPH, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 0, 128, DM_NONE, true, FT_NONE, morphQuad5}, 
								                     {AT_ALPHA|AT_REPLAY, 1000, 0, 0, ptPosition9[0].x, ptPosition9[0].y, ptPosition9[0].x, ptPosition9[0].y, 80, 80, 0, 0, 128, 0, DM_RANDOM, true, FT_NONE}
	};
	_ANIMDESC2 lpAnimDesc10[] = {{AT_REPLAY, 200, 0, 0, ptPosition10[0].x, ptPosition10[0].y, ptPosition10[0].x, ptPosition10[0].y, 100, 100, 0, 0, 255, 255, DM_NONE, false, FT_FIRE}};
	_ANIMDESC2 lpAnimDesc11[] = {{AT_REPLAY, 200, 0, 0, ptPosition11[0].x, ptPosition11[0].y, ptPosition11[0].x, ptPosition11[0].y, 100, 100, 0, 0, 255, 255, DM_NONE, false, FT_WATER}};
	_ANIMDESC2 lpAnimDesc12[] = {{AT_REPLAY, 200, 0, 0, ptPosition12[0].x, ptPosition12[0].y, ptPosition12[0].x, ptPosition12[0].y, 100, 100, 0, 0, 255, 255, DM_NONE, false, FT_SMOKE}};
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc1, 2, 0);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc2, 2, 1);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc3, 2, 2);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc4, 2, 3);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc5, 2, 4);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc6, 2, 5);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc7, 2, 6);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc8, 2, 7);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc9, 14, 8);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc10, 1, 9);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc11, 1, 10);
	g_EffectorBuilder.SetAnimationInfo(lpAnimDesc12, 1, 11);
	g_EffectorBuilder.SetQuality(QT_LOW);
  
    // Run animation loop
    while (g_bRunning)
    {
      // Update animation
      g_EffectorBuilder.Update();
      g_EffectorBuilder.Draw(im);
      
      // Sleep thread for some time
      cvW
    }
    
#endif

	return 0;
}
