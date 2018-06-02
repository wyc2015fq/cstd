
int po2_polygon_triangulate(int n, const DPOINT2* polygon, DTRIANGLE2* out)
{
  DPOINT2* internal_polygon = MALLOC(DPOINT2, n);
  int i, n2 = n, j = 0;
  MEMCPY(internal_polygon, polygon, n);

  if (po2_polygon_orientation(n2, internal_polygon) != Clockwise) {
    //internal_polygon.reverse();
    for (i = 0; i < n / 2; ++i) {
      T_SWAP(DPOINT2, internal_polygon[i], internal_polygon[n - 1 - i]);
    }
  }

  while (n2 > 3) {
    for (i = 0; i < n2; ++i) {
      if (po2_convex_vertex(i, n2, internal_polygon, Clockwise) && po2_vertex_is_ear(i, n2, internal_polygon)) {
        out[j++] = po2_vertex_triangle(i, n2, internal_polygon);
        //internal_polygon.erase(i);
        --n2;
        MEMCPY(internal_polygon + i, internal_polygon + i + 1, n2 - i);
        break;
      }
    }
  }

  out[j++] = po2_vertex_triangle(1, n2, internal_polygon);
  FREE(internal_polygon);
  return j;
}

