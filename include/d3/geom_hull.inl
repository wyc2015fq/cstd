
int geom_hull(ctrl_t* c) {
  SYSIO;
  static FPOINT vertices[] = { 100,100,140,200,140,180,130,170,230,60,90,220,200,200 };
  static FPOINT hull[countof(vertices)];
  // hull=d3.geom.hull(vertices);
  int i, len;
  //len = convex_closure(countof(vertices), vertices, hull);
  len = d3_geom_hull(countof(vertices), vertices, hull);
  gcSolidBrush(g, ColorGreen);
  gcFillPolygon(g, hull, len, 1, GC_FLOAT, sizeof(*hull));
  gcSolidPen(g, ColorBlack);
  gcSolidBrush(g, ColorWhite);
  for (i=0; i<countof(vertices); ++i) {
    gcCircle(g, GcOptFillStroke, vertices[i].x, vertices[i].y, 3);
  }
  return 0;
}
