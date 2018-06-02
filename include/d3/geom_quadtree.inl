
bool quadtree_visit_1(void* user, quadnode_t* node, double x1, double y1, double x2, double y2) {
  gc_t* g = (gc_t*)user;
  gcCircle(g, GcOptFill, x1, y1, 4);
  gcRect(g, GcOptStroke, x1, y1, x2-x1, y2-y1);
  return false;
}
int geom_quadtree(ctrl_t* c) {
  SYSIO;
  int i;
#if 0
  static FPOINT data1[] = { 10,100,150,16,33,150,75,80,160,160 };
  static FPOINT data[] = { 84, 304, 271, 456, 49, 155, 472, 341, 191, 130, 18, 204, 439, 452,
    227, 40, 0, 76, 95, 222, 247, 185, 411, 9, 196, 466, 284, 158,
    429, 239, 392, 7, 250, 333, 191, 288, 184, 497, 257, 299, 446, 55,
   300, 95, 171, 139, 493, 263, 195, 148, 246, 320, 237, 173, 415, 290, 468, 230, 239, 429 };
#else
  static FPOINT data[30];
  static int inited = 0;
  if (!inited) {
    inited = 1;
    for (i=0; i<countof(data); ++i) {
      data[i].x = 10+rng_int32(NULL)%500;
      data[i].y = 10+rng_int32(NULL)%500;
      //printf("%g, %g,\n", data[i].x, data[i].y);
    }
  }
#endif
  quadtree_t quadtree[1] = {0};
  d3_geom_quadtree(quadtree, countof(data), data, sizeof(*data), NULL);
  
  gcSolidBrush(g, ColorGreen);
  gcSolidPen(g, ColorBlack);
  d3_geom_quadtreeVisit_root(quadtree, quadtree_visit_1, g);
  //gcRect(g, GcOptStroke, 10, 10, 100, 100);
  gcSolidBrush(g, ColorRed);
  for (i=0; i<countof(data); ++i) {
    gcCircle(g, GcOptFill, data[i].x, data[i].y, 4);
  }
  quadtree_free(quadtree);
  return 0;
}
