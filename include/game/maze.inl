//自动生成迷宫
//首先迷宫初始化全部为墙
//然后随机选择从中间一个点开始，
//开始递归，随机选择方向尝试移动，如果是墙，并且不与其他的路相通，就把墙设置成路。
//使用深度优先的方法，从新的点继续递归，如果周围全部无法走通，则回退到上次节点，选择其他方向。
//如此一直递归，直到所有的点都探索完。最终的效果图如下：
//后来研究下别人的算法，是先假设地图上有相间隔的点，然后将这些点进行打通，
//只要这个点是孤立的，就可以与其他点连通，这样的算法，会好看些，处理上会简单很多，
//生成的图形也没有一个个的小块。
#include "cstd.h"
// 递归版本，栈容易溢出
//随机选择从中间一个点开始递归，
//随机选择方向尝试移动，如果是墙，并且不与其他的路相通，就把墙设置成路。
//使用深度优先的方法，从新的点继续递归，如果周围全部无法走通，则回退到上次节点，选择其他方向。
//如此一直递归，直到所有的点都探索完。
int maze_search_path(uint* pseed, char* zmap, int* dir)
{
  int next, turn, i;
  *zmap = 0;
  turn = myrand32(pseed) % 2 ? 1 : 3;
  for (i = 0, next = myrand32(pseed) % 4; i < 4; ++i, next = (next + turn) % 4) {
    if (zmap[2 * dir[next]] == 0) {
      zmap[dir[next]] = 1;
      maze_search_path(pseed, zmap + 2 * dir[next], dir);
    }
  }
  return 0;
}
#define PUSH(P) (dts[ptslen++]=P, zmap+=2*dir[P])
#define POP() (zmap-=2*dir[dts[--ptslen]])
// 非递归版本，没大小限制
int maze_search_path2(uint* pseed, char* zmap, const int* dir, char* dts)
{
  int ptslen = 0;
  PUSH(4);
  for (; ptslen > 0;) {
    int next, turn, i, candel = ptslen;
    //maze_print("asdf.txt", 11, 11, map, 11);
    *zmap = 1;
    turn = myrand32(pseed) % 2 ? 1 : 3;
    for (i = 0, next = myrand32(pseed) % 4; i < 4; ++i, next = (next + turn) % 4) {
      if (zmap[2 * dir[next]] == 0) {
        zmap[dir[next]] = 1;
        PUSH(next);
        break;
      }
    }
    if (candel == ptslen) {
      POP();
    }
  }
  return 0;
}
#undef PUSH
#undef POP
#define PUSH(P, pp) (nodes[ptslen].zmap=zmap+dir[P], nodes[ptslen++].parent = pp, ASSERT(ptslen<len))
#define POP() (zmap=nodes[--ptslen].zmap)
typedef struct mazenode_t_ mazenode_t;
struct mazenode_t_ {
  mazenode_t* parent;
  char* zmap;
};
char* g_map;
int g_h, g_w;
mazenode_t* maze_search_path3(char* zmap, char* end, const int* dir, char* dts, int len)
{
  int ptslen = 0;
  mazenode_t* nodes = (mazenode_t*)dts;
  mazenode_t* parent = NULL;
  len /= sizeof(mazenode_t);
  PUSH(4, NULL);
  for (; ptslen > 0;) {
    int i, candel = ptslen;
    mazenode_t* parent = nodes + ptslen - 1;
    zmap = nodes[ptslen - 1].zmap;
    *zmap = 2;
    if (end == zmap) {
      return parent;
    }
    for (i = 0; i < 4; ++i) {
      if (1 == zmap[dir[i]]) {
        PUSH(i, parent);
      }
    }
    if (candel == ptslen) {
      POP();
    }
  }
  return NULL;
}
#undef PUSH
#undef POP
//边界填v
int maze_fill_bord(int h, int w, char* map, int step, int v)
{
  int i, j;
  for (i = 0, j = w - 1; i < h; ++i) {
    map[i * step + 0] = v;
    map[i * step + j] = v;
  }
  for (i = 0, j = h - 1; i < w; ++i) {
    map[0 * step + i] = v;
    map[j * step + i] = v;
  }
  return 0;
}
int maze_rand(uint* pseed, int h, int w, char* map, int step)
{
  //首先迷宫初始化全部为墙
  int i, j, dir[5] = {1, step, -1, -step, 0};
  char* dts = NULL;
  //assert((h & 1) && (w & 1));
  assert((h > 3) && (w > 3));
  h = (h - 1) | 1;
  w = (w - 1) | 1;
  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      map[i * step + j] = 0;
    }
  }
  //边界填0，方便越界判断
  maze_fill_bord(h, w, map, step, 1);
  map[2 * step + 1] = 1;
  map[(h - 3)*step + w - 2] = 1;
  //maze_print("asdf.txt", h, w, map, step);
  srand((unsigned)time(NULL));
  i = myrand32(pseed) % ((h - 3) / 2) + 1;
  j = myrand32(pseed) % ((w - 3) / 2) + 1;
  dts = (char*)malloc(h * w * 4);
  maze_search_path2(pseed, map + (i * step + j) * 2, dir, dts);
  maze_fill_bord(h, w, map, step, 0);
  {
    mazenode_t* p = maze_search_path3(map + 2 * step + 1, map + (h - 3) * step + w - 2, dir, dts, h * w * 4);
    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        map[i * step + j] = !!map[i * step + j];
      }
    }
    for (; p; p = p->parent) {
      *p->zmap = 2;
    }
  }
  free(dts);
  //maze_search_path(2, 1);
  return 0;
}
int maze_print(const char* fn, int h, int w, char* map, int step)
{
  int i, j;
  FILE* pf = fn ? fopen(fn, "wb") : stdout;
  char* aa[] = {"█", "　", "×", "　"};
  if (fn) {
    for (i = 1; i < h - 1; i++) {
      for (j = 1; j < w - 1; j++) {
        fputs(aa[map[i * step + j] & 3], pf);
      }
      fputs("\r\n", pf);
    }
    fclose(pf);
  }
  return 0;
}

