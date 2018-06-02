
#include "ImageQuilting.inl"

void TMatrix_Init(TMatrix* s, int Width, int Height, int WidthStep, int Depth, int Channel, byte* Scan0)
{
  s->Width = Width;
  s->Height = Height;
  s->WidthStep = WidthStep;
  s->Depth = Depth;
  s->Channel = Channel;
  s->Data = Scan0;
  s->Reserved = 0;
}

int test_ImageQuilting() {
  img_t im[10] = {0};
  img_t* im1 = im+1;
  sys_chdir("E:/pub/bin/img2");
  imread("water-big_src.jpg", 3, 1, im);
  imsetsize(im1, 512, 512, 3, 1);
  {
    double TileSize_Minimum = (int)MAX(MIN(im->w, im->h) * 0.1, 5);
    double TileSize_Maximum = (int)(MIN(im->w, im->h) * 0.5);
    double Overlap_Minimum = (int)MIN(TileSize_Minimum * 0.5, 2);
    double Overlap_Maximum = (int)MAX(TileSize_Maximum * 0.5, 2);
    double TileSize_Value = (TileSize_Maximum + TileSize_Minimum) / 2;
    double Overlap_Value = (Overlap_Maximum + Overlap_Minimum) / 5;
    TMatrix Src[2] = {0};
    TMatrix* Dest = Src + 1;
    TMatrix_Init(Src, im->w, im->h, im->s, IS_DEPTH_8U, 3, im->tt.data);
    TMatrix_Init(Dest, im1->w, im1->h, im1->s, IS_DEPTH_8U, 3, im1->tt.data);
    ImageQuilting(Src, Dest, TileSize_Value, TileSize_Value, Overlap_Value, Overlap_Value);
  }
  imshow(im);
  imshow(im1);
  cvWaitKey(-1);
  imfrees(im, 10);
  return 0;
}
