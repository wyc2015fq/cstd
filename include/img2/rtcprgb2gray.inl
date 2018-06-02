double* wei()
{
  return NULL;
}
#include "img/imgopt.inl"
int rtcprgb2gray(const img_t* im, img_t* im1)
{
  // Proprocessing
  int n = im->h, m = im->w, ch = im->c;
  double sigma = 0.05;
  double* W = wei();
  uchar ims[64 * 64 * 3];
  IMRESIZE(im->h, im->w, im->tt.data, im->s, im->c, 64, 64, ims, 64 * 3, 3, INTER_NEARSAMPLE);
  cvShowImage("ims", 64, 64, ims, 64 * 3, 3);
  cvWaitKey(-1);
#if 0
  // Global and Local Contrast Computing
R = ims(: , : , 1);
G = ims(: , : , 2);
B = ims(: , : , 3);
imV = [R(:), G(:), B(:)];
  defaultStream = RandStream.getDefaultStream;
  savedState = defaultStream.State;
  t1 = randperm(size(imV, 1));
Pg = [imV - imV(t1, :)];
  ims = imresize(ims, 0.5 , 'nearest');
Rx = ims(: , 1: end - 1, 1) - ims(: , 2: end, 1);
Gx = ims(: , 1: end - 1, 2) - ims(: , 2: end, 2);
Bx = ims(: , 1: end - 1, 3) - ims(: , 2: end, 3);
Ry = ims(1: end - 1, : , 1) - ims(2: end, : , 1);
Gy = ims(1: end - 1, : , 2) - ims(2: end, : , 2);
By = ims(1: end - 1, : , 3) - ims(2: end, : , 3);
Pl = [[Rx(:), Gx(:), Bx(:)]; [Ry(:), Gy(:), By(:)]];
  P = [Pg; Pl ];
  det = sqrt(sum(P. ^ 2, 2)) / 1.41 ;
P((det < 0.05), :) = [];
  det((det < 0.05)) = [];
  detM = repmat(det, [1, size(W, 1)]);
  L = P * W';
      // Energy optimization
      U = log(exp(- (L + detM ).^2/sigma.^2) + exp(- (L- detM).^2/sigma.^2));
      Es = mean(U);
      // Output
      [NULLval,bw] = max(Es);
      img = imlincomb(W(bw,1),im(:,:,1) , W(bw,2),im(:,:,2) , W(bw,3),im(:,:,3));
#endif
      return 0;
    }

