
#if 0

int render_edge_fine(scanline_rasterizer& ras,
    solid_renderer& ren_fine,
    draft_renderer& ren_draft,
    Source& src)
{
  double x, y;

  switch (m_draw) {
  case 0:
    src.rewind(0);

    while (!agg::is_stop(src.vertex(&x, &y)));

    break;

  case 1:
    ras.reset();
    ras.add_path(src);
    break;

  case 2:
    ras.reset();
    ras.add_path(src);
    ras.sort();
    break;

  case 3: {
    int r = rand() & 0x7F;
    int g = rand() & 0x7F;
    int b = rand() & 0x7F;
    int a = 255;

    if (m_translucent->hit) {
      a = 80;
    }

    ras.add_path(src);

    if (m_type->hit < 4) {
      ren_fine.color(agg::rgba8(r, g, b, a));
      agg::render_scanlines(ras, m_sl, ren_fine);
    }
    else {
      ren_draft.color(agg::rgba8(r, g, b, a));
      agg::render_scanlines(ras, m_sl, ren_draft);
    }
  }
  break;
  }

  return 0;
}

#endif

int test_graph_test()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};

  const char* m_items0[] = {
    "Solid lines",
    "Bezier curves",
    "Dashed curves",
    "Poygons AA",
    "Poygons Bin",
  };
  XCTRLADD(disp, layout, lay1);
  XCTRLADD2(disp, rbox, m_type, (m_type, countof(m_items0), m_items0));
  XCTRLADD2(disp, slider, m_width, (m_width, 0, 5, "Width=%1.2f"));
  XCTRLADD2(disp, cbox, m_benchmark, (m_benchmark, "Benchmark"));
  XCTRLADD2(disp, cbox, m_draw_nodes, (m_draw_nodes, "Draw Nodes"));
  XCTRLADD2(disp, cbox, m_draw_edges, (m_draw_edges, "Draw Edges"));
  XCTRLADD2(disp, cbox, m_draft, (m_draft, "Draft Mode"));
  XCTRLADD2(disp, cbox, m_translucent, (m_translucent, "Translucent Mode"));

  uistyle_default_t uidrawer[1] = {0};

  int iid[] = {
    0,                  200,              200,             layx,
    layx,               0,                0,                0,
    20,         id_m_type,   id_m_benchmark,                0,
    20,              layv,       id_m_draft,                0,
    20,              layv,  id_m_draw_edges,                0,
    20,              layv,  id_m_draw_nodes,                0,
    20,              layv, id_m_translucent,                0,
    20,        id_m_width,             layh,                0,
  };

  int i;
  COLOR c1 = rgba(1, 1, 0, 0.25);
  COLOR c2 = rgb(0, 0, 1);

  typedef struct edge {
    int node1, node2;
  } edge;
  enum { m_num_nodes = 200, m_num_edges = 100};
  DPOINT m_nodes[m_num_nodes];
  edge m_edges[m_num_edges];
  COLOR m_gradient_colors[256];

  fill_color_array(256, m_gradient_colors, c1, c2);

  uistyle_default_set(uidrawer);
  xlayout_set(lay1, countof(iid) / 4 - 1, 4 - 1, iid, 4, 4);

  XCreateWindow(disp, mainwin, "AGG gradients with Mach bands compensation", 100, 100, 800, 600, id_lay1);

  srand(100);

  for (i = 0; i < m_num_nodes; i++) {
    m_nodes[i].x = ((double)(rand()) / RAND_MAX) * 0.75 + 0.2;
    m_nodes[i].y = ((double)(rand()) / RAND_MAX) * 0.85 + 0.1;
  }

  for (i = 0; i < m_num_edges; i++) {
    m_edges[i].node1 = rand() % m_num_nodes;
    m_edges[i].node2 = rand() % m_num_nodes;

    if (m_edges[i].node1 == m_edges[i].node2) {
      i--;
    }
  }

  for (; XWaitMsg(disp, msg, 50, MSG_MASK_KEY | MSG_MASK_MOUSE) >= 0;) {
    xwin_t* win = msg->win;

    if (win && mainwin == win->winid) {
      IRECT crc = win->fr->rcs[HITCLIENT].rc;
      int i, x = msg->x - crc.l, y = msg->y - crc.t;
      win = msg->win;

      switch (msg->msg) {
      case MSG_INIT:
        if (1) {
          m_draw_nodes->hit = 1;
          m_draft->hit = 1;
        }

        break;

      case MSG_EXIT:
        if (1) {
        }

        break;

      case MSG_LBUTTONUP:
        if (1) {
          force_redraw(msg);
        }

        break;

      case MSG_COMMAND:
        if (1) {
          if (XCMD_CLOSE == msg->key) {
            int asdf = 0;
          }
        }

        break;

      case MSG_LBUTTONDOWN:
        if (1) {
          force_redraw(msg);
        }

        break;

      case MSG_MOUSEMOVE:
        if (1) {
        }

        break;
      }

      if (msg->redraw) {
        int n_drawn = 0;
        img_t im[1] = {0};
        int i;
        int h = im->h, w = im->w;
        IRECT rcClient = xGetClient(win);
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);

        imdraw_fill(IMARG(im), rgb(1, 1, 1));

        {
          int h = im->h, w = im->w;
          uchar gamma[256];
          srand(100);

          if (m_draw_nodes->hit) {
            if (m_draft->hit) {
              // draw_nodes_draft
              for (i = 0; i < m_num_nodes; i++) {
                DPOINT n = dPOINT(m_nodes[i].x * w, m_nodes[i].y * h);
                imdraw_outlined_ellipse(IMARG(im), (int)(n.x), (int)(n.y), 10, 10, m_gradient_colors[147], m_gradient_colors[255]);
                imdraw_solid_ellipse(IMARG(im), (int)(n.x), (int)(n.y), 4, 4, m_gradient_colors[50]);
              }
            }
            else {
              // draw_nodes_fine
              double r = 5.0 * m_width->m_value;

              for (i = 0; i < m_num_nodes; i++) {
#define get_node(i)  dPOINT(m_nodes[i].x*w, m_nodes[i].y*h)
                DPOINT n = dPOINT(m_nodes[i].x * w, m_nodes[i].y * h);
                imdrawaa_ellipse_lut_radial(IMARG(im), n.x, n.y, r, r, n.x, n.y, r, r, m_gradient_colors);
              }
            }
          }

          if (m_draw_edges->hit) {
            if (m_draft->hit) {
              switch (m_type->hit) {
              case 0:
                if (1) {
                  // draw_lines_draft()
                  for (i = 0; i < m_num_edges; i++) {
                    edge e  = m_edges[i];
                    DPOINT n1 = get_node(e.node1);
                    DPOINT n2 = get_node(e.node2);
                    //line l(n1.x, n1.y, n2.x, n2.y);
                    //stroke_draft<line> s(l, m_width->m_value);

                    int r = rand() & 0x7F;
                    int g = rand() & 0x7F;
                    int b = rand() & 0x7F;
                    int a = 255;

                    if (m_translucent->hit) {
                      a = 80;
                    }

                    //prim.line_color(_RGBA(r, g, b, a));
                  }

                }

                break;

              case 1:

                // draw_curves_draft
                if (1) {
                  for (i = 0; i < m_num_edges; i++) {
                    edge e  = m_edges[i];
                    DPOINT n1 = get_node(e.node1);
                    DPOINT n2 = get_node(e.node2);
                    //curve c(n1.x, n1.y, n2.x, n2.y);
                    //stroke_draft<curve> s(c, m_width->m_value);

                    int r = rand() & 0x7F;
                    int g = rand() & 0x7F;
                    int b = rand() & 0x7F;
                    int a = 255;

                    if (m_translucent->hit) {
                      a = 80;
                    }

                    //prim.line_color(agg::rgba8(r, g, b, a));
                    //ras.add_path(s);
                  }
                }

                break;

              case 2:
                if (1) {
                  // draw_dashes_draft();
                  for (i = 0; i < m_num_edges; i++) {
                    edge e  = m_edges[i];
                    DPOINT n1 = get_node(e.node1);
                    DPOINT n2 = get_node(e.node2);
                    //curve c(n1.x, n1.y, n2.x, n2.y);
                    //dash_stroke_draft<curve> s(c, 6.0, 3.0, m_width->m_value);

                    int r = rand() & 0x7F;
                    int g = rand() & 0x7F;
                    int b = rand() & 0x7F;
                    int a = 255;

                    if (m_translucent->hit) {
                      a = 80;
                    }

                    //prim.line_color(agg::rgba8(r, g, b, a));
                    //ras.add_path(s);
                  }
                }

                break;
              }
            }
            else {
              switch (m_type->hit) {
              case 0:

                //draw_lines_fine(ras, solid, draft);
                if (1) {
                  for (i = 0; i < m_num_edges; i++) {
                    edge b  = m_edges[i];
                    DPOINT n1 = get_node(b.node1);
                    DPOINT n2 = get_node(b.node2);
                    //line l(n1.x, n1.y, n2.x, n2.y);
                    //stroke_fine<line> s(l, m_width->m_value);
                    //render_edge_fine(ras, solid, draft, s);
                  }
                }

                break;

              case 1:

                //draw_curves_fine(ras, solid, draft);
                if (1) {
                  for (i = 0; i < m_num_edges; i++) {
                    edge b  = m_edges[i];
                    DPOINT n1 = get_node(b.node1);
                    DPOINT n2 = get_node(b.node2);
                    //curve c(n1.x, n1.y, n2.x, n2.y);
                    //stroke_fine<curve> s(c, m_width->m_value);
                    //render_edge_fine(ras, solid, draft, s);
                  }
                }

                break;

              case 2:

                //draw_dashes_fine(ras, solid, draft);
                if (1) {
                  int i;

                  for (i = 0; i < m_num_edges; i++) {
                    edge b  = m_edges[i];
                    DPOINT n1 = get_node(b.node1);
                    DPOINT n2 = get_node(b.node2);
                    //curve c(n1.x, n1.y, n2.x, n2.y);
                    //dash_stroke_fine<curve> s(c, 6.0, 3.0, m_width->m_value);
                    //render_edge_fine(ras, solid, draft, s);
                  }
                }

                break;

              case 3:
              case 4:

                //draw_polygons(ras, solid, draft);
                if (1) {
                  if (m_type->hit == 4) {
                    //ras.gamma(agg::gamma_threshold(0.5));
                  }

                  for (i = 0; i < m_num_edges; i++) {
                    edge b  = m_edges[i];
                    DPOINT n1 = get_node(b.node1);
                    DPOINT n2 = get_node(b.node2);
                    //curve c(n1.x, n1.y, n2.x, n2.y);
                    //render_edge_fine(ras, solid, draft, c);
                  }

                  //ras.gamma(agg::gamma_none());
                }

                break;
              }
            }
          }
        }
#undef get_node
        //imdrawui_default_polygon(uidrawer, im, m_poly1);
        //imdrawui_default_polygon(uidrawer, im, m_poly2);
        xEndPaint(win, imdrawui_default, uidrawer);
      }
    }
  }

  return 0;
}

