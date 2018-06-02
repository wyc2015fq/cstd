
#define NEXTLINE(szText)  {szText=strchr(szText, '\n'); if (NULL==szText++) return 0;}

int delObject(OBJECT3D* pO, int n)
{
  for (; --n; ++pO) {
    if (pO->vlist_local != NULL) {
      free(pO->vlist_local);
      pO->vlist_local = NULL;
    }

    if (pO->vlist_trans != NULL) {
      free(pO->vlist_trans);
      pO->vlist_trans = NULL;
    }

    if (pO->list_texel != NULL) {
      free(pO->list_texel);
      pO->list_texel = NULL;
    }

    free(pO->plist);
    pO->plist = NULL;
    pO->numberPolys = 0;
    pO->numberVertices = 0;
  }

  return 0;
}

int loadObject_PLG(OBJECT3D* pO, int n, LPTSTR lpszFileName)
{
  int i, poly, vertex, len;
  char* buf = loadfile(lpszFileName, &len);
  memset(pO, 0, sizeof(OBJECT3D)*n);

  for (; n--; ++pO) {
    char* szText = buf;
    // Read header information
    i = _stscanf(szText, _T("%s %d %d"), pO->szName, &pO->numberVertices, &pO->numberPolys);
    NEXTLINE(szText);
    //lpObject3D->world_pos = pos;

    // Read vertex information
    pO->vlist_local = MALLOC(VERTEX3D, pO->numberVertices);
    pO->vlist_trans = MALLOC(VERTEX3D, pO->numberVertices);
    pO->list_texel = MALLOC(POINT3D, pO->numberVertices);
    pO->max_radius = 0;

    for (i = 0; i < pO->numberVertices; i++) {
      float max_vertex;
      _stscanf(szText, _T("%f %f %f"), &pO->vlist_local[i].p.x, &pO->vlist_local[i].p.y, &pO->vlist_local[i].p.z);
      pO->vlist_local[i].p.w = 1.0f;
      max_vertex = MAX(ABS(pO->vlist_local[i].p.x), MAX(ABS(pO->vlist_local[i].p.y), ABS(pO->vlist_local[i].p.z)));

      NEXTLINE(szText);

      if (pO->max_radius < max_vertex) {
        pO->max_radius = max_vertex;
      }

      V3SET(pO->vlist_local[i].n, 0, 0, 0);
      pO->vlist_local[i].p.w = 0;
    }

    // Read polygon information
    pO->plist = MALLOC(POLY, pO->numberPolys);

    for (i = 0; i < pO->numberPolys; i++) {
      _TCHAR szTemp[16];
      int number_vertices;
      int poly_surface_descr = 0;
      BYTE shade;

      _stscanf(szText, _T("%s %d %d %d %d"), szTemp, &number_vertices, &pO->plist[i].vertex[0], &pO->plist[i].vertex[1], &pO->plist[i].vertex[2]);

      NEXTLINE(szText);

      pO->plist[i].vList = pO->vlist_local;
      pO->plist[i].tList = pO->vlist_trans;
      pO->plist[i].uvList = pO->list_texel;

      if ((szTemp[0] == '0') && (_totupper(szTemp[1]) == 'X')) {
        _stscanf(szTemp, _T("%X"), &poly_surface_descr);
      }
      else {
        poly_surface_descr = _ttoi(szTemp);
      }

      if (poly_surface_descr & 0x8000) {
        BYTE red = (poly_surface_descr & 0x0F00) >> 8;
        BYTE green = (poly_surface_descr & 0x00F0) >> 4;
        BYTE blue = poly_surface_descr & 0x000F;
        pO->plist[i].color = _RGB(red << 4, green << 4, blue << 4);
      }
      else {
        pO->plist[i].color = poly_surface_descr & 0x00FF;
      }

      pO->plist[i].shade_color = pO->plist[i].color;

      shade = (poly_surface_descr & 0x6000) >> 13;

      if (shade == 3) {
        pO->plist[i].shade = _SHADING_MODE_NONE;
      }
      else if (shade == 2) {
        pO->plist[i].shade = _SHADING_MODE_GOURAUD;
      }
      else if (shade == 1) {
        pO->plist[i].shade = _SHADING_MODE_FLAT;
      }
      else {
        pO->plist[i].shade = _SHADING_MODE_NONE;
      }

      //        lpObject3D->plist[i].shade = _SHADING_MODE_NONE;
      //        lpObject3D->plist[i].shade = _SHADING_MODE_CONSTANT;
      //        lpObject3D->plist[i].shade = _SHADING_MODE_GOURAUD;
      //        lpObject3D->plist[i].shade = _SHADING_MODE_FLAT;
      //        lpObject3D->plist[i].shade = _SHADING_MODE_TEXTURE;
    }

    for (poly = 0; poly < pO->numberPolys; poly++) {
      if (pO->plist[poly].shade & _SHADING_MODE_GOURAUD) {
        POINT3D u, v, n;
        int vindex_0 = pO->plist[poly].vertex[0];
        int vindex_1 = pO->plist[poly].vertex[1];
        int vindex_2 = pO->plist[poly].vertex[2];

        V3SUB(u, pO->vlist_local[vindex_1].p, pO->vlist_local[vindex_0].p);
        V3SUB(v, pO->vlist_local[vindex_2].p, pO->vlist_local[vindex_0].p);
        V3CROSS(n, u, v);

        pO->vlist_local[vindex_0].n.w += 1;
        pO->vlist_local[vindex_1].n.w += 1;
        pO->vlist_local[vindex_2].n.w += 1;

        V3ADD(pO->vlist_local[vindex_0].n, pO->vlist_local[vindex_0].n, n);
        V3ADD(pO->vlist_local[vindex_1].n, pO->vlist_local[vindex_1].n, n);
        V3ADD(pO->vlist_local[vindex_2].n, pO->vlist_local[vindex_2].n, n);
      }
    }

    for (vertex = 0; vertex < pO->numberVertices; vertex++) {
      float w = pO->vlist_local[vertex].n.w;

      if (w > 1) {
        pO->vlist_local[vertex].n.x /= w;
        pO->vlist_local[vertex].n.y /= w;
        pO->vlist_local[vertex].n.z /= w;
      }

      pO->vlist_local[vertex].n.w = 1.f;
    }
  }

  free(buf);
  return 0;
}

