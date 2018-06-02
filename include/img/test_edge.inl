
#include "macro.h"
#include "window.h"

char wndname[] = "Edge";
char tbarname[] = "Threshold";
int edge_thresh = 1;

unsigned char* image = 0, *cedge = 0, *gray = 0, *edge = 0;

// define a trackbar callback
void on_trackbar(int h)
{
  //cvSmooth( gray, edge, CC_BLUR, 3, 3, 0, 0 );
  //cvNot( gray, edge );

  // Run the edge detector on grayscale
  cvCanny(gray, edge, (float) edge_thresh, (float) edge_thresh * 3, 3);

  cvZero(cedge);
  // copy edge points
  cvCopy(image, cedge, edge);

  cvShowImage(wndname, cedge);
}

int main(int argc, char** argv)
{
  char* filename = argc == 2 ? argv[ 1 ] : (char*) "fruits.jpg";

  if ((image = cvLoadImage(filename, 1)) == 0) {
    return -1;
  }

  // Create the output image
  cedge = cvCreateImage(cvSize(image->w, image->h), IMG_DEPTH_8U, 3);

  // Convert to grayscale
  gray = cvCreateImage(cvSize(image->w, image->h), IMG_DEPTH_8U, 1);
  edge = cvCreateImage(cvSize(image->w, image->h), IMG_DEPTH_8U, 1);
  cvCvtColor(image, gray, CC_BGR2GRAY);

  // Create a window
  cvNamedWindow(wndname, 1);

  // create a toolbar
  cvCreateTrackbar(tbarname, wndname, &edge_thresh, 100, on_trackbar);

  // Show the image
  on_trackbar(0);

  // Wait for a key stroke; the same function arranges events processing
  cvWaitKey(0);
  cvReleaseImage(&image);
  cvReleaseImage(&gray);
  cvReleaseImage(&edge);
  cvDestroyWindow(wndname);

  return 0;
}
