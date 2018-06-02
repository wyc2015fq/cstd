
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <tchar.h>
#include <assert.h>
#include "cstd.h"
#include "macro.h"

#include "fractal.h"
#include "cstd.h"
#include "vec.h"

typedef struct gnode {
  char* name;
  char* text;
  int height;
  int width;
  struct gnode** child;
}
gnode;

typedef struct graphviz {
  int fontsize;
  gnode* root;
}
graphviz;

int gnode_parse(char* str, gnode* g, char** endstr)
{
  return 0;
}

gnode* gnode_new(char* name, char* text)
{
  gnode* n;
  int len;

  if (NULL == name) {
    return 0;
  }

  n = MALLOC(gnode, 1);

  if (NULL == n) {
    return 0;
  }

  MEMSET(n, 0, 1);
  len = strlen(name) + 1;
  vec_new(n->name, len);
  vec_new(n->text, 1);
  vec_new(n->child, 0);
  vec_copy(n->name, name, len);

  if (text) {
    vec_copy(n->text, text, strlen(text) + 1);
  }
  else {
    *(n->text) = 0;
  }

  return n;
}

gnode* gnode_del(gnode* n)
{
  int i = 0;

  if (n) {
    i = vec_used(n->name);
    vec_del(n->name);
    vec_del(n->text);

    for (i = 0; i < (int) vec_used(n->child); ++i) {
      gnode_del(n->child[ i ]);
    }

    vec_del(n->child);
    SAFEFREE(n);
  }

  return n;
}

gnode* gnode_find(gnode* n, char* name)
{
  int i = 0;

  if (n) {
    if (0 == strcmp(n->name, name) && strlen(n->name) == strlen(name)) {
      return n;
    }

    for (i = 0; i < (int) vec_used(n->child); ++i) {
      gnode* n2 = gnode_find(n->child[ i ], name);

      if (NULL != n2) {
        return n2;
      }
    }
  }

  return 0;
}

int graphviz_parse(char* str, graphviz* g, char** endstr)
{
  return 0;
}

graphviz* graphviz_new()
{
  graphviz* g;
  g = MALLOC(graphviz, 1);
  MEMSET(g, 0, 1);
  g->root = gnode_new("#root", 0);
  return g;
}

void graphviz_del(graphviz* g)
{
  gnode_del(g->root);
  SAFEFREE(g);
  return ;
}

gnode* graphviz_add(gnode* g, char* name, char* text)
{
  gnode* n = 0;
  n = gnode_find(g, name);

  if (NULL != n) {
    if (text && strlen(text) > 0) {
      vec_copy(n->text, text, strlen(text) + 1);
    }
  }
  else {
    n = gnode_new(name, text);
    vec_push_back(g->child, n);
  }

  return n;
}

gnode* graphviz_add_edge(graphviz* g, char* name, char* text, char* name_child, char* text_child)
{
  gnode* n = graphviz_add(g->root, name, text);

  if (n && name_child && strlen(name_child) > 0) {
    n = graphviz_add(n, name_child, text_child);
  }

  return n;
}

#define MAX_CHAR_PER_LINE 10

int gnode_set_size(gnode* n)
{
  int i, h, w, w0 = 0, h0 = 0;
  w = 1 + strlen(n->name) + strlen(n->text);
  h = 1;

  if (w > MAX_CHAR_PER_LINE) {
    h = 1 + w / MAX_CHAR_PER_LINE;
    w = MAX_CHAR_PER_LINE;
  }

  for (i = 0; i < (int) vec_used(n->child); ++i) {
    gnode_set_size(n->child[ i ]);
    h0 += n->child[ i ] ->h;
    w0 += n->child[ i ] ->w;
  }

  w = MAX(w, w0);
  h += h0;
  n->h = h + 2;
  n->w = w;
  return 0;
}

int gnode_draw(gnode* n, IDC* pDC)
{
  int i = 0;
  int y = 0, x = 0;
  int cy = 0, cx = 10;

  if (n) {
    if (n->name[ 0 ] != '#') {
      char* name0 = MALLOC(char, strlen(n->name) + strlen(n->text) + 10);
      char* name = name0;
      strcpy(name0, n->name);

      if (n->text && strlen(n->text) > 0) {
        strcat(name0, ":\n");
        strcat(name0, n->text);
      }

      y = cy = pDC->font_cy;

      if (strlen(name0) < MAX_CHAR_PER_LINE) {
        cx = pDC->font_cx * strlen(name);
        x = (pDC->w - cx) / 2;
        PutText(pDC, y, x, name0);
      }
      else {
        int len = 0;
        char buf[ MAX_CHAR_PER_LINE + 1 ];
        cx = pDC->font_cx * MAX_CHAR_PER_LINE;
        x = (pDC->w - cx) / 2;

        for (i = 0; i < (int) strlen(name0); i += len) {
          len = MAX_CHAR_PER_LINE;
          MEMSET(buf, 0, MAX_CHAR_PER_LINE + 1);
          len = MIN(MAX_CHAR_PER_LINE, strlen(name));

          if (buf[ len - 1 ] < 0) {
            --len; // 处理汉字
          }

          memcpy(buf, name, len);

          if (strchr(buf, '\n')) {
            len = strchr(buf, '\n') - buf;
            buf[ len ] = 0;
            ++len;
          }

          PutText(pDC, cy, x, buf);
          cy += pDC->font_cy;
          name += len;
        }
      }

      Rectangle(pDC, x, y, x + cx, y + cy);
      x += cx / 2;
      y += cy;
      SAFEFREE(name0);
    }

    cx = 10;

    for (i = 0; i < (int) vec_used(n->child); ++i) {
      IDC dc;
      int h, w;
      h = n->child[ i ] ->h * pDC->font_cy;
      w = n->child[ i ] ->w * pDC->font_cx;
      InitDC(&dc, pDC->h, w,
          pDC->data + y * pDC->s + cx * pDC->c,
          pDC->s, pDC->c,
          pDC->pen_color, pDC->thickness);
      gnode_draw(n->child[ i ], &dc);

      if (n->name[ 0 ] != '#') {
        MoveTo(pDC, x, y);
        LineTo(pDC, cx + w / 2, y + pDC->font_cy);
      }

      cx += w;
    }
  }

  return 0;
}

int graphviz_draw(graphviz* g, char* fname)
{
  int height = 0, width = 0, step = 0, cn = 3;
  CvScalar pen_color = CC_RGB(255, 0, 0);
  int thickness = 2;
  int fontsize = 40;
  IDC dc;
  IDC* pDC = &dc;
  char* data = NULL;
  gnode_set_size(g->root);
  height = g->root->h * fontsize;
  width = g->root->w * fontsize;
  step = width * cn;
  data = MALLOC(char, step * height);
  MEMSET(data, 255, step * height);
  InitDC(pDC, height, width, data, step, cn, pen_color, thickness);
  gnode_draw(g->root, pDC);
  imwrite(fname, pDC->h, pDC->w, pDC->data, pDC->s, pDC->c);
  SAFEFREE(data);
  return 0;
}

/*
digraph G {
main -> parse -> execute;
main -> init;
main -> cleanup;
execute -> make_string;
execute -> printf
init -> make_string;
main -> printf;
execute -> compare;
}

1.把图片缩放到用户指定大小。
2.计算缩放后的图片的积分图像。
3.设置遍历框的初值为人脸分类器中指定的大小，如20×20。
4.遍历框按先从左到右再从上到下的顺序在积分图上移动。每次移动用户指定的距离，如果用户指定的距离为0,则把遍历框的宽的20分之一当做移动距离。
5.每移动一次用分类器确定当前位置是不是人脸，如果是人脸则把当前矩形框的位置加入结果。
6.遍历完积分图后，把遍历框的长和宽放大1.1倍(由用户指定)再回到步骤4。直到遍历框大小超过图像大小或存放结果的缓冲区用光为止。
7.返回人脸框位置和人脸个数。
*/
int test_graphviz()
{
  graphviz* g = graphviz_new();
#if 0

  graphviz_add_edge(g, "main", "你好", "parse", "");
  graphviz_add_edge(g, "parse", "", "execute", "");
  graphviz_add_edge(g, "main", "", "init", "");
  graphviz_add_edge(g, "main", "", "cleanup", "");
  graphviz_add_edge(g, "execute", "", "make_string", "");
  graphviz_add_edge(g, "execute", "", "printf", "");
  graphviz_add_edge(g, "init", "", "printf", "");
  graphviz_add_edge(g, "execute", "", "compare", "");
#else

  graphviz_add_edge(g, "1", "把图片缩放到用户指定大小", "2", "");
  graphviz_add_edge(g, "2", "计算缩放后的图片的积分图像。", "3", "");
  graphviz_add_edge(g, "3", "设置遍历框的初值为人脸分类器中指定的大小，如20×20。", "4", "");
  graphviz_add_edge(g, "4", "遍历框按先从左到右再从上到下的顺序在积分图上移动。每次移动用户指定的距离，如果用户指定的距离为0,则把遍历框的宽的20分之一当做移动距离。", "5", 0);

  graphviz_add_edge(g, "5", "遍历完积分图后，把遍历框的长和宽放大1.1倍(由用户指定)再回到步骤4。直到遍历框大小超过图像大小或存放结果的缓冲区用光为止。", "6", "");
  graphviz_add_edge(g, "6", "遍历完积分图后，把遍历框的长和宽放大1.1倍(由用户指定)再回到步骤4。直到遍历框大小超过图像大小或存放结果的缓冲区用光为止。", "7", "");
  graphviz_add_edge(g, "7", "返回人脸框位置和人脸个数。", 0, 0);
#endif

  graphviz_draw(g, "a.jpg");
  graphviz_del(g);
  return 0;
}
