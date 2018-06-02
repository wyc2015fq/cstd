#include "ui/window.inl"
#include "img/imgio.inl"
int test_maze()
{
  int h = (20) | 1, w = (h * 3 / 2) | 1;
  char* map = NULL;
  uint seed = 1000;
  map = (char*)malloc(h * w);
  g_map = map;
  g_h = h;
  g_w = w;
  {
    char* path;
    path = "E:/code/cstd/include/game";
    sys_chdir(path);
  }
  if (1) {
    maze_rand(&seed, h, w, map, w);
    maze_print("test_maze.txt", h, w, map, w);
  }
  if (1) {
    img_t im[1] = {0};
    int aa[] = {0, 128, 255, 200};
    int i = 0;
    imsetsize(im, h, w, 1, 1);
    for (i = 0; i < h * w; ++i) {
      im->tt.data[i] = aa[map[i] & 3];
    }
    //imwrite("C:/maze.bmp", im);
    cvNamedWindow("im", 0);
    imshow(im);
    imfree(im);
    cvWaitKey(-1);
  }
  free(map);
  return 0;
}

