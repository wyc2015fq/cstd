
typedef struct CvWindow {
  int x;
} CvWindow;
int cvNamedWindow(const char* name, int flags)
{
  return 0;
}
int cvShowImagePal(const char* name, int height, int width, const unsigned char* arr, int step, int cn, const void* inpal)
{
  return 0;
}
int cvWaitKey(int delay)
{
  return 0;
}
int cvDestroyAllWindows(int x)
{
  return 0;
}
static int cvSetMouseCallback(const char* window_name, CvMouseCallback on_mouse, void* param)
{
  return 0;
}
