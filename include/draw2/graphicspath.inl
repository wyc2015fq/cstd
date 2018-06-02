
typedef struct path_list_node_t path_list_node_t;
struct path_list_node_t {
    PointF pt;
    BYTE type; /* PathPointTypeStart or PathPointTypeLine */
    path_list_node_t *next;
};

/* init list */
static BOOL init_path_list(path_list_node_t **node, REAL x, REAL y)
{
    *node = (path_list_node_t*)pmalloc(sizeof(path_list_node_t));
    if(!*node)
        return FALSE;

    (*node)->pt.X = x;
    (*node)->pt.Y = y;
    (*node)->type = PathPointTypeStart;
    (*node)->next = NULL;

    return TRUE;
}

/* free all nodes including argument */
static void free_path_list(path_list_node_t *node)
{
    path_list_node_t *n = node;

    while(n){
        n = n->next;
        pfree(node);
        node = n;
    }
}

/* Add a node after 'node' */
/*
 * Returns
 *  pointer on success
 *  NULL    on allocation problems
 */
static path_list_node_t* add_path_list_node(path_list_node_t *node, REAL x, REAL y, BOOL type)
{
    path_list_node_t *n;

    n = (path_list_node_t*)pmalloc(sizeof(path_list_node_t));
    if(!n)
        return NULL;

    n->pt.X  = x;
    n->pt.Y  = y;
    n->type  = type;
    n->next  = node->next;
    node->next = n;

    return n;
}

/* returns element count */
static INT path_list_count(path_list_node_t *node)
{
    INT count = 1;

    while((node = node->next))
        ++count;

    return count;
}

/* PathFlatten helper */
/*
 * Used to recursively flatten single Bezier curve
 * Parameters:
 *  - start   : pointer to start point node;
 *  - (x2, y2): first control point;
 *  - (x3, y3): second control point;
 *  - end     : pointer to end point node
 *  - flatness: admissible error of linear approximation.
 *
 * Return value:
 *  TRUE : success
 *  FALSE: out of memory
 *
 * TODO: used quality criteria should be revised to match native as
 *       closer as possible.
 */
static BOOL flatten_bezier(path_list_node_t *start, REAL x2, REAL y2, REAL x3, REAL y3,
                           path_list_node_t *end, REAL flatness)
{
    /* this 5 middle points with start/end define to half-curves */
    PointF mp[5];
    PointF pt, pt_st;
    path_list_node_t *node;

    /* calculate bezier curve middle points == new control points */
    mp[0].X = (start->pt.X + x2) / 2.0;
    mp[0].Y = (start->pt.Y + y2) / 2.0;
    /* middle point between control points */
    pt.X = (x2 + x3) / 2.0;
    pt.Y = (y2 + y3) / 2.0;
    mp[1].X = (mp[0].X + pt.X) / 2.0;
    mp[1].Y = (mp[0].Y + pt.Y) / 2.0;
    mp[4].X = (end->pt.X + x3) / 2.0;
    mp[4].Y = (end->pt.Y + y3) / 2.0;
    mp[3].X = (mp[4].X + pt.X) / 2.0;
    mp[3].Y = (mp[4].Y + pt.Y) / 2.0;

    mp[2].X = (mp[1].X + mp[3].X) / 2.0;
    mp[2].Y = (mp[1].Y + mp[3].Y) / 2.0;

    pt = end->pt;
    pt_st = start->pt;
    /* check flatness as a half of distance between middle point and a linearized path */
    if(fabs(((pt.Y - pt_st.Y)*mp[2].X + (pt_st.X - pt.X)*mp[2].Y +
        (pt_st.Y*pt.X - pt_st.X*pt.Y))) <=
        (0.5 * flatness*sqrtf((powf(pt.Y - pt_st.Y, 2.0) + powf(pt_st.X - pt.X, 2.0))))){
        return TRUE;
    }
    else
        /* add a middle point */
        if(!(node = add_path_list_node(start, mp[2].X, mp[2].Y, PathPointTypeLine)))
            return FALSE;

    /* do the same with halves */
    flatten_bezier(start, mp[0].X, mp[0].Y, mp[1].X, mp[1].Y, node, flatness);
    flatten_bezier(node,  mp[3].X, mp[3].Y, mp[4].X, mp[4].Y, end,  flatness);

    return TRUE;
}

Status PathAddArc(Path *path, REAL x1, REAL y1, REAL x2,
    REAL y2, REAL startAngle, REAL sweepAngle)
{
    INT count, old_count, i;

    TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n",
          path, x1, y1, x2, y2, startAngle, sweepAngle);

    if(!path)
        return InvalidParameter;

    count = arc2polybezier(NULL, x1, y1, x2, y2, startAngle, sweepAngle);

    if(count == 0)
        return Ok;
    if(!lengthen_path(path, count))
        return OutOfMemory;

    old_count = path->pathdata.Count;
    arc2polybezier(&path->pathdata.Points[old_count], x1, y1, x2, y2,
                   startAngle, sweepAngle);

    for(i = 0; i < count; i++){
        path->pathdata.Types[old_count + i] = PathPointTypeBezier;
    }

    path->pathdata.Types[old_count] =
        (path->newfigure ? PathPointTypeStart : PathPointTypeLine);
    path->newfigure = FALSE;
    path->pathdata.Count += count;

    return Ok;
}

Status PathAddArcI(Path *path, INT x1, INT y1, INT x2,
   INT y2, REAL startAngle, REAL sweepAngle)
{
    TRACE("(%p, %d, %d, %d, %d, %.2f, %.2f)\n",
          path, x1, y1, x2, y2, startAngle, sweepAngle);

    return PathAddArc(path,(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2,startAngle,sweepAngle);
}

Status PathAddBezier(Path *path, REAL x1, REAL y1, REAL x2,
    REAL y2, REAL x3, REAL y3, REAL x4, REAL y4)
{
    INT old_count;

    TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n",
          path, x1, y1, x2, y2, x3, y3, x4, y4);

    if(!path)
        return InvalidParameter;

    if(!lengthen_path(path, 4))
        return OutOfMemory;

    old_count = path->pathdata.Count;

    path->pathdata.Points[old_count].X = x1;
    path->pathdata.Points[old_count].Y = y1;
    path->pathdata.Points[old_count + 1].X = x2;
    path->pathdata.Points[old_count + 1].Y = y2;
    path->pathdata.Points[old_count + 2].X = x3;
    path->pathdata.Points[old_count + 2].Y = y3;
    path->pathdata.Points[old_count + 3].X = x4;
    path->pathdata.Points[old_count + 3].Y = y4;

    path->pathdata.Types[old_count] =
        (path->newfigure ? PathPointTypeStart : PathPointTypeLine);
    path->pathdata.Types[old_count + 1] = PathPointTypeBezier;
    path->pathdata.Types[old_count + 2] = PathPointTypeBezier;
    path->pathdata.Types[old_count + 3] = PathPointTypeBezier;

    path->newfigure = FALSE;
    path->pathdata.Count += 4;

    return Ok;
}

Status PathAddBezierI(Path *path, INT x1, INT y1, INT x2,
    INT y2, INT x3, INT y3, INT x4, INT y4)
{
    TRACE("(%p, %d, %d, %d, %d, %d, %d, %d, %d)\n",
          path, x1, y1, x2, y2, x3, y3, x4, y4);

    return PathAddBezier(path,(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2,(REAL)x3,(REAL)y3,
                                  (REAL)x4,(REAL)y4);
}
Status PathAddBeziers(Path *path, GDIPCONST PointF *points,
    INT count)
{
    INT i, old_count;

    TRACE("(%p, %p, %d)\n", path, points, count);

    if(!path || !points || ((count - 1) % 3))
        return InvalidParameter;

    if(!lengthen_path(path, count))
        return OutOfMemory;

    old_count = path->pathdata.Count;

    for(i = 0; i < count; i++){
        path->pathdata.Points[old_count + i].X = points[i].X;
        path->pathdata.Points[old_count + i].Y = points[i].Y;
        path->pathdata.Types[old_count + i] = PathPointTypeBezier;
    }

    path->pathdata.Types[old_count] =
        (path->newfigure ? PathPointTypeStart : PathPointTypeLine);
    path->newfigure = FALSE;
    path->pathdata.Count += count;

    return Ok;
}

Status PathAddBeziersI(Path *path, GDIPCONST Point *points, INT count)
{
    PointF *ptsF = NULL;
    Status ret;

    TRACE("(%p, %p, %d)\n", path, points, count);

    if(!points || ((count - 1) % 3))
        return InvalidParameter;

    ret = PointI2F(&ptsF, points, count);
    if(!ptsF)
        return OutOfMemory;

    ret = PathAddBeziers(path, ptsF, count);
    pfree(ptsF);

    return ret;
}

Status PathAddClosedCurve2(Path *path, GDIPCONST PointF *points,
    INT count, REAL tension)
{
    INT i, len_pt = (count + 1)*3-2;
    PointF *pt;
    PointF *pts;
    REAL x1, x2, y1, y2;
    Status stat;

    TRACE("(%p, %p, %d, %.2f)\n", path, points, count, tension);

    if(!path || !points || count <= 1)
        return InvalidParameter;

    pt = (PointF*)pmalloc((len_pt + count + 1) * sizeof(PointF));
    pts = pt + len_pt;
    if(!pt){
        pfree(pt);
        return OutOfMemory;
    }

    /* copy source points to extend with the last one */
    memcpy(pts, points, sizeof(PointF)*count);
    pts[count] = pts[0];

    tension = tension * TENSION_CONST;

    for(i = 0; i < count-1; i++){
        calc_curve_bezier(&(pts[i]), tension, &x1, &y1, &x2, &y2);

        pt[3*i+2].X = x1;
        pt[3*i+2].Y = y1;
        pt[3*i+3].X = pts[i+1].X;
        pt[3*i+3].Y = pts[i+1].Y;
        pt[3*i+4].X = x2;
        pt[3*i+4].Y = y2;
    }

    /* points [len_pt-2] and [0] are calculated
       separately to connect splines properly */
    pts[0] = points[count-1];
    pts[1] = points[0]; /* equals to start and end of a resulting path */
    pts[2] = points[1];

    calc_curve_bezier(pts, tension, &x1, &y1, &x2, &y2);
    pt[len_pt-2].X = x1;
    pt[len_pt-2].Y = y1;
    pt[0].X = pts[1].X;
    pt[0].Y = pts[1].Y;
    pt[1].X = x2;
    pt[1].Y = y2;
    /* close path */
    pt[len_pt-1].X = pt[0].X;
    pt[len_pt-1].Y = pt[0].Y;

    stat = PathAddBeziers(path, pt, len_pt);

    /* close figure */
    if(stat == Ok){
        path->pathdata.Types[path->pathdata.Count - 1] |= PathPointTypeCloseSubpath;
        path->newfigure = TRUE;
    }

    pfree(pt);

    return stat;
}

Status PathAddClosedCurve2I(Path *path, GDIPCONST Point *points,
    INT count, REAL tension)
{
    PointF *ptf = NULL;
    Status stat;

    TRACE("(%p, %p, %d, %.2f)\n", path, points, count, tension);

    if(!path || !points || count <= 1)
        return InvalidParameter;
    
    stat = PointI2F(&ptf, points, count);
    if(!ptf)
        return OutOfMemory;

    stat = PathAddClosedCurve2(path, ptf, count, tension);

    pfree(ptf);

    return stat;
}

Status PathAddClosedCurve(Path *path, GDIPCONST PointF *points,
                               INT count)
{
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  return PathAddClosedCurve2(path, points, count, 1.0);
}

Status PathAddClosedCurveI(Path *path, GDIPCONST Point *points,
                                INT count)
{
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  return PathAddClosedCurve2I(path, points, count, 1.0);
}

Status PathAddCurve2(Path *path, GDIPCONST PointF *points, INT count,
    REAL tension)
{
    INT i, len_pt = count*3-2;
    PointF *pt;
    REAL x1, x2, y1, y2;
    Status stat;

    TRACE("(%p, %p, %d, %.2f)\n", path, points, count, tension);

    if(!path || !points || count <= 1)
        return InvalidParameter;

    pt = (PointF*)pmalloc(sizeof(PointF) * len_pt);
    if(!pt)
        return OutOfMemory;

    tension = tension * TENSION_CONST;

    calc_curve_bezier_endp(points[0].X, points[0].Y, points[1].X, points[1].Y,
        tension, &x1, &y1);

    pt[0].X = points[0].X;
    pt[0].Y = points[0].Y;
    pt[1].X = x1;
    pt[1].Y = y1;

    for(i = 0; i < count-2; i++){
        calc_curve_bezier(&(points[i]), tension, &x1, &y1, &x2, &y2);

        pt[3*i+2].X = x1;
        pt[3*i+2].Y = y1;
        pt[3*i+3].X = points[i+1].X;
        pt[3*i+3].Y = points[i+1].Y;
        pt[3*i+4].X = x2;
        pt[3*i+4].Y = y2;
    }

    calc_curve_bezier_endp(points[count-1].X, points[count-1].Y,
        points[count-2].X, points[count-2].Y, tension, &x1, &y1);

    pt[len_pt-2].X = x1;
    pt[len_pt-2].Y = y1;
    pt[len_pt-1].X = points[count-1].X;
    pt[len_pt-1].Y = points[count-1].Y;

    stat = PathAddBeziers(path, pt, len_pt);

    pfree(pt);

    return stat;
}

Status PathAddCurve(Path *path, GDIPCONST PointF *points, INT count)
{
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  if(!path || !points || count <= 1)
    return InvalidParameter;
  
  return PathAddCurve2(path, points, count, 1.0);
}

Status PathAddCurve2I(Path *path, GDIPCONST Point *points,
    INT count, REAL tension)
{
    PointF *ptf;
    INT i;
    Status stat;

    TRACE("(%p, %p, %d, %.2f)\n", path, points, count, tension);

    if(!path || !points || count <= 1)
        return InvalidParameter;

    ptf = (PointF*)pmalloc(sizeof(PointF)*count);
    if(!ptf)
        return OutOfMemory;

    for(i = 0; i < count; i++){
        ptf[i].X = (REAL)points[i].X;
        ptf[i].Y = (REAL)points[i].Y;
    }

    stat = PathAddCurve2(path, ptf, count, tension);

    pfree(ptf);

    return stat;
}

Status PathAddCurveI(Path *path, GDIPCONST Point *points, INT count)
{
  TRACE("(%p, %p, %d)\n", path, points, count);
  
  if(!path || !points || count <= 1)
    return InvalidParameter;
  
  return PathAddCurve2I(path, points, count, 1.0);
}

Status PathAddCurve3(Path *path, GDIPCONST PointF *points,
    INT count, INT offset, INT nseg, REAL tension)
{
    TRACE("(%p, %p, %d, %d, %d, %.2f)\n", path, points, count, offset, nseg, tension);

    if(!path || !points || offset + 1 >= count || count - offset < nseg + 1)
        return InvalidParameter;

    return PathAddCurve2(path, &points[offset], nseg + 1, tension);
}

Status PathAddCurve3I(Path *path, GDIPCONST Point *points,
    INT count, INT offset, INT nseg, REAL tension)
{
    TRACE("(%p, %p, %d, %d, %d, %.2f)\n", path, points, count, offset, nseg, tension);

    if(!path || !points || offset + 1 >= count || count - offset < nseg + 1)
        return InvalidParameter;

    return PathAddCurve2I(path, &points[offset], nseg + 1, tension);
}

Status PathAddEllipse(Path *path, REAL x, REAL y, REAL width,
    REAL height)
{
    INT old_count, numpts;

    TRACE("(%p, %.2f, %.2f, %.2f, %.2f)\n", path, x, y, width, height);

    if(!path)
        return InvalidParameter;

    if(!lengthen_path(path, MAX_ARC_PTS))
        return OutOfMemory;

    old_count = path->pathdata.Count;
    if((numpts = arc2polybezier(&path->pathdata.Points[old_count],  x, y, width,
                               height, 0.0, 360.0)) != MAX_ARC_PTS){
        ERR("expected %d points but got %d\n", MAX_ARC_PTS, numpts);
        return GenericError;
    }

    memset(&path->pathdata.Types[old_count + 1], PathPointTypeBezier,
           MAX_ARC_PTS - 1);

    /* An ellipse is an intrinsic figure (always is its own subpath). */
    path->pathdata.Types[old_count] = PathPointTypeStart;
    path->pathdata.Types[old_count + MAX_ARC_PTS - 1] |= PathPointTypeCloseSubpath;
    path->newfigure = TRUE;
    path->pathdata.Count += MAX_ARC_PTS;

    return Ok;
}

Status PathAddEllipseI(Path *path, INT x, INT y, INT width,
    INT height)
{
    TRACE("(%p, %d, %d, %d, %d)\n", path, x, y, width, height);

    return PathAddEllipse(path,(REAL)x,(REAL)y,(REAL)width,(REAL)height);
}

Status PathAddLine2(Path *path, GDIPCONST PointF *points,
    INT count)
{
    INT i, old_count;

    TRACE("(%p, %p, %d)\n", path, points, count);

    if(!path || !points)
        return InvalidParameter;

    if(!lengthen_path(path, count))
        return OutOfMemory;

    old_count = path->pathdata.Count;

    for(i = 0; i < count; i++){
        path->pathdata.Points[old_count + i].X = points[i].X;
        path->pathdata.Points[old_count + i].Y = points[i].Y;
        path->pathdata.Types[old_count + i] = PathPointTypeLine;
    }

    if(path->newfigure){
        path->pathdata.Types[old_count] = PathPointTypeStart;
        path->newfigure = FALSE;
    }

    path->pathdata.Count += count;

    return Ok;
}

Status PathAddLine2I(Path *path, GDIPCONST Point *points, INT count)
{
    PointF *pointsF;
    INT i;
    Status stat;

    TRACE("(%p, %p, %d)\n", path, points, count);

    if(count <= 0)
        return InvalidParameter;

    pointsF = (PointF*)pmalloc(sizeof(PointF) * count);
    if(!pointsF)    return OutOfMemory;

    for(i = 0;i < count; i++){
        pointsF[i].X = (REAL)points[i].X;
        pointsF[i].Y = (REAL)points[i].Y;
    }

    stat = PathAddLine2(path, pointsF, count);

    pfree(pointsF);

    return stat;
}

Status PathAddLine(Path *path, REAL x1, REAL y1, REAL x2, REAL y2)
{
    INT old_count;

    TRACE("(%p, %.2f, %.2f, %.2f, %.2f)\n", path, x1, y1, x2, y2);

    if(!path)
        return InvalidParameter;

    if(!lengthen_path(path, 2))
        return OutOfMemory;

    old_count = path->pathdata.Count;

    path->pathdata.Points[old_count].X = x1;
    path->pathdata.Points[old_count].Y = y1;
    path->pathdata.Points[old_count + 1].X = x2;
    path->pathdata.Points[old_count + 1].Y = y2;

    path->pathdata.Types[old_count] =
        (path->newfigure ? PathPointTypeStart : PathPointTypeLine);
    path->pathdata.Types[old_count + 1] = PathPointTypeLine;

    path->newfigure = FALSE;
    path->pathdata.Count += 2;

    return Ok;
}

Status PathAddLineI(Path *path, INT x1, INT y1, INT x2, INT y2)
{
    TRACE("(%p, %d, %d, %d, %d)\n", path, x1, y1, x2, y2);

    return PathAddLine(path, (REAL)x1, (REAL)y1, (REAL)x2, (REAL)y2);
}

Status PathAddPath(Path *path, GDIPCONST Path* addingPath,
    BOOL connect)
{
    INT old_count, count;

    TRACE("(%p, %p, %d)\n", path, addingPath, connect);

    if(!path || !addingPath)
        return InvalidParameter;

    old_count = path->pathdata.Count;
    count = addingPath->pathdata.Count;

    if(!lengthen_path(path, count))
        return OutOfMemory;

    memcpy(&path->pathdata.Points[old_count], addingPath->pathdata.Points,
           count * sizeof(PointF));
    memcpy(&path->pathdata.Types[old_count], addingPath->pathdata.Types, count);

    if(path->newfigure || !connect)
        path->pathdata.Types[old_count] = PathPointTypeStart;
    else
        path->pathdata.Types[old_count] = PathPointTypeLine;

    path->newfigure = FALSE;
    path->pathdata.Count += count;

    return Ok;
}

Status PathCloseFigure(Path* path)
{
  TRACE("(%p)\n", path);
  
  if(!path)
    return InvalidParameter;
  
  if(path->pathdata.Count > 0){
    path->pathdata.Types[path->pathdata.Count - 1] |= PathPointTypeCloseSubpath;
    path->newfigure = TRUE;
  }
  
  return Ok;
}

Status PathCloseFigures(Path* path)
{
  INT i;
  
  TRACE("(%p)\n", path);
  
  if(!path)
    return InvalidParameter;
  
  for(i = 1; i < path->pathdata.Count; i++) {
    if(path->pathdata.Types[i] == PathPointTypeStart) {
      path->pathdata.Types[i-1] |= PathPointTypeCloseSubpath;
    }
  }
  
  path->newfigure = TRUE;
  
  return Ok;
}

Status PathAddPie(Path *path, REAL x, REAL y, REAL width, REAL height,
    REAL startAngle, REAL sweepAngle)
{
    PointF *ptf;
    Status status;
    INT i, count;

    TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n",
          path, x, y, width, height, startAngle, sweepAngle);

    if(!path)
        return InvalidParameter;

    /* on zero width/height only start point added */
    if(width <= 1e-7 || height <= 1e-7){
        if(!lengthen_path(path, 1))
            return OutOfMemory;
        path->pathdata.Points[0].X = x + width  / 2.0;
        path->pathdata.Points[0].Y = y + height / 2.0;
        path->pathdata.Types[0] = PathPointTypeStart | PathPointTypeCloseSubpath;
        path->pathdata.Count = 1;
        return InvalidParameter;
    }

    count = arc2polybezier(NULL, x, y, width, height, startAngle, sweepAngle);

    if(count == 0)
        return Ok;

    ptf = (PointF*)pmalloc(sizeof(PointF)*count);
    if(!ptf)
        return OutOfMemory;

    arc2polybezier(ptf, x, y, width, height, startAngle, sweepAngle);

    status = PathAddLine(path, x + width/2, y + height/2, ptf[0].X, ptf[0].Y);
    if(status != Ok){
        pfree(ptf);
        return status;
    }
    /* one spline is already added as a line endpoint */
    if(!lengthen_path(path, count - 1)){
        pfree(ptf);
        return OutOfMemory;
    }

    memcpy(&(path->pathdata.Points[path->pathdata.Count]), &(ptf[1]),sizeof(PointF)*(count-1));
    for(i = 0; i < count-1; i++)
        path->pathdata.Types[path->pathdata.Count+i] = PathPointTypeBezier;

    path->pathdata.Count += count-1;

    PathCloseFigure(path);

    pfree(ptf);

    return status;
}

Status PathAddPieI(Path *path, INT x, INT y, INT width, INT height,
    REAL startAngle, REAL sweepAngle)
{
    TRACE("(%p, %d, %d, %d, %d, %.2f, %.2f)\n",
          path, x, y, width, height, startAngle, sweepAngle);

    return PathAddPie(path, (REAL)x, (REAL)y, (REAL)width, (REAL)height, startAngle, sweepAngle);
}

Status PathAddPolygon(Path *path, GDIPCONST PointF *points, INT count)
{
    INT old_count;

    TRACE("(%p, %p, %d)\n", path, points, count);

    if(!path || !points || count < 3)
        return InvalidParameter;

    if(!lengthen_path(path, count))
        return OutOfMemory;

    old_count = path->pathdata.Count;

    memcpy(&path->pathdata.Points[old_count], points, count*sizeof(PointF));
    memset(&path->pathdata.Types[old_count + 1], PathPointTypeLine, count - 1);

    /* A polygon is an intrinsic figure */
    path->pathdata.Types[old_count] = PathPointTypeStart;
    path->pathdata.Types[old_count + count - 1] |= PathPointTypeCloseSubpath;
    path->newfigure = TRUE;
    path->pathdata.Count += count;

    return Ok;
}

Status PathAddPolygonI(Path *path, GDIPCONST Point *points, INT count)
{
    PointF *ptf;
    Status status;
    INT i;

    TRACE("(%p, %p, %d)\n", path, points, count);

    if(!points || count < 3)
        return InvalidParameter;

    ptf = (PointF*)pmalloc(sizeof(PointF) * count);
    if(!ptf)
        return OutOfMemory;

    for(i = 0; i < count; i++){
        ptf[i].X = (REAL)points[i].X;
        ptf[i].Y = (REAL)points[i].Y;
    }

    status = PathAddPolygon(path, ptf, count);

    pfree(ptf);

    return status;
}

static float fromfixedpoint(const fixed_t v)
{
    float f = ((float)v.fract) / (1<<(sizeof(v.fract)*8));
    f += v.value;
    return f;
}

struct format_string_args
{
    Path *path;
    float maxY;
    float scale;
    float ascent;
};
#if 0
static Status format_string_callback(HDC dc,
    GDIPCONST WCHAR *string, INT index, INT length, GDIPCONST Font *font,
    GDIPCONST RectF *rect, GDIPCONST StringFormat *format,
    INT lineno, const RectF *bounds, INT *underlined_indexes,
    INT underlined_index_count, void *priv)
{
    static const MAT2 identity = { {0,1}, {0,0}, {0,0}, {0,1} };
    struct format_string_args *args = priv;
    Path *path = args->path;
    Status status = Ok;
    float x = rect->X + (bounds->X - rect->X) * args->scale;
    float y = rect->Y + (bounds->Y - rect->Y) * args->scale;
    int i;

    if (underlined_index_count)
        FIXME("hotkey underlines not drawn yet\n");

    if (y + bounds->Height * args->scale > args->maxY)
        args->maxY = y + bounds->Height * args->scale;

    for (i = index; i < length; ++i)
    {
        GLYPHMETRICS gm;
        TTPOLYGONHEADER *ph = NULL;
        char *start;
        DWORD len, ofs = 0;
        len = GetGlyphOutlineW(dc, string[i], GGO_BEZIER, &gm, 0, NULL, &identity);
        if (len == GDI_ERROR)
        {
            status = GenericError;
            break;
        }
        ph = (path_list_node_t*)GdipAlloc(len);
        start = (char *)ph;
        if (!ph || !lengthen_path(path, len / sizeof(POINTFX)))
        {
            status = OutOfMemory;
            break;
        }
        GetGlyphOutlineW(dc, string[i], GGO_BEZIER, &gm, len, start, &identity);

        ofs = 0;
        while (ofs < len)
        {
            DWORD ofs_start = ofs;
            ph = (TTPOLYGONHEADER*)&start[ofs];
            path->pathdata.Types[path->pathdata.Count] = PathPointTypeStart;
            path->pathdata.Points[path->pathdata.Count].X = x + fromfixedpoint(ph->pfxStart.x) * args->scale;
            path->pathdata.Points[path->pathdata.Count++].Y = y + args->ascent - fromfixedpoint(ph->pfxStart.y) * args->scale;
            TRACE("Starting at count %i with pos %f, %f)\n", path->pathdata.Count, x, y);
            ofs += sizeof(*ph);
            while (ofs - ofs_start < ph->cb)
            {
                TTPOLYCURVE *curve = (TTPOLYCURVE*)&start[ofs];
                int j;
                ofs += sizeof(TTPOLYCURVE) + (curve->cpfx - 1) * sizeof(POINTFX);

                switch (curve->wType)
                {
                case TT_PRIM_LINE:
                    for (j = 0; j < curve->cpfx; ++j)
                    {
                        path->pathdata.Types[path->pathdata.Count] = PathPointTypeLine;
                        path->pathdata.Points[path->pathdata.Count].X = x + fromfixedpoint(curve->apfx[j].x) * args->scale;
                        path->pathdata.Points[path->pathdata.Count++].Y = y + args->ascent - fromfixedpoint(curve->apfx[j].y) * args->scale;
                    }
                    break;
                case TT_PRIM_CSPLINE:
                    for (j = 0; j < curve->cpfx; ++j)
                    {
                        path->pathdata.Types[path->pathdata.Count] = PathPointTypeBezier;
                        path->pathdata.Points[path->pathdata.Count].X = x + fromfixedpoint(curve->apfx[j].x) * args->scale;
                        path->pathdata.Points[path->pathdata.Count++].Y = y + args->ascent - fromfixedpoint(curve->apfx[j].y) * args->scale;
                    }
                    break;
                default:
                    ERR("Unhandled type: %u\n", curve->wType);
                    status = GenericError;
                }
            }
            path->pathdata.Types[path->pathdata.Count - 1] |= PathPointTypeCloseSubpath;
        }
        path->newfigure = TRUE;
        x += gm.gmCellIncX * args->scale;
        y += gm.gmCellIncY * args->scale;

        GdipFree(ph);
        if (status != Ok)
            break;
    }

    return status;
}

Status PathAddString(Path* path, GDIPCONST WCHAR* string, INT length, GDIPCONST FontFamily* family, INT style, REAL emSize, GDIPCONST RectF* layoutRect, GDIPCONST StringFormat* format)
{
    Font *font;
    Status status;
    LOGFONTW lfw;
    HANDLE hfont;
    HDC dc;
    Graphics *graphics;
    Path *backup;
    struct format_string_args args;
    int i;
    UINT16 native_height;
    RectF scaled_layout_rect;
    TEXTMETRICW textmetric;

    TRACE("(%p, %s, %d, %p, %d, %f, %p, %p)\n", path, debugstr_w(string), length, family, style, emSize, layoutRect, format);
    if (!path || !string || !family || !emSize || !layoutRect || !format)
        return InvalidParameter;

    status = GdipGetEmHeight(family, style, &native_height);
    if (status != Ok)
        return status;

    scaled_layout_rect.X = layoutRect->X;
    scaled_layout_rect.Y = layoutRect->Y;
    scaled_layout_rect.Width = layoutRect->Width * native_height / emSize;
    scaled_layout_rect.Height = layoutRect->Height * native_height / emSize;

    if ((status = GdipClonePath(path, &backup)) != Ok)
        return status;

    dc = CreateCompatibleDC(0);
    status = GdipCreateFromHDC(dc, &graphics);
    if (status != Ok)
    {
        DeleteDC(dc);
        PathDelete(backup);
        return status;
    }

    status = GdipCreateFont(family, native_height, style, UnitPixel, &font);
    if (status != Ok)
    {
        GdipDeleteGraphics(graphics);
        DeleteDC(dc);
        PathDelete(backup);
        return status;
    }

    get_log_fontW(font, graphics, &lfw);
    GdipDeleteFont(font);
    GdipDeleteGraphics(graphics);

    hfont = CreateFontIndirectW(&lfw);
    if (!hfont)
    {
        WARN("Failed to create font\n");
        DeleteDC(dc);
        PathDelete(backup);
        return GenericError;
    }

    SelectObject(dc, hfont);

    GetTextMetricsW(dc, &textmetric);

    args.path = path;
    args.maxY = 0;
    args.scale = emSize / native_height;
    args.ascent = textmetric.tmAscent * args.scale;
    status = gdip_format_string(dc, string, length, NULL, &scaled_layout_rect, format, format_string_callback, &args);

    DeleteDC(dc);
    DeleteObject(hfont);

    if (status != Ok) /* free backup */
    {
        GdipFree(path->pathdata.Points);
        GdipFree(path->pathdata.Types);
        *path = *backup;
        GdipFree(backup);
        return status;
    }
    if (format && format->vertalign == StringAlignmentCenter && layoutRect->Y + args.maxY < layoutRect->Height)
    {
        float inc = layoutRect->Height + layoutRect->Y - args.maxY;
        inc /= 2;
        for (i = backup->pathdata.Count; i < path->pathdata.Count; ++i)
            path->pathdata.Points[i].Y += inc;
    } else if (format && format->vertalign == StringAlignmentFar) {
        float inc = layoutRect->Height + layoutRect->Y - args.maxY;
        for (i = backup->pathdata.Count; i < path->pathdata.Count; ++i)
            path->pathdata.Points[i].Y += inc;
    }
    PathDelete(backup);
    return status;
}

Status PathAddStringI(Path* path, GDIPCONST WCHAR* string, INT length, GDIPCONST FontFamily* family, INT style, REAL emSize, GDIPCONST Rect* layoutRect, GDIPCONST StringFormat* format)
{
    if (layoutRect)
    {
        RectF layoutRectF = {
            (REAL)layoutRect->X,
            (REAL)layoutRect->Y,
            (REAL)layoutRect->Width,
            (REAL)layoutRect->Height
        };
        return PathAddString(path, string, length, family, style, emSize, &layoutRectF, format);
    }
    return InvalidParameter;
}

#endif

Status PathCreate(FillMode fill, Path *path)
{
    TRACE("(%d, %p)\n", fill, path);

    if(!path)
        return InvalidParameter;

    (path)->fill = fill;
    (path)->newfigure = TRUE;

    return Ok;
}

Status PathCreate2(GDIPCONST PointF* points,
    GDIPCONST BYTE* types, INT count, FillMode fill, Path *path)
{
    TRACE("(%p, %p, %d, %d, %p)\n", points, types, count, fill, path);

    if(!path)
        return InvalidParameter;

    if(!path)  return OutOfMemory;

    if(!PathData_SetSize(&(path)->pathdata, count)){
        return OutOfMemory;
    }

    memcpy((path)->pathdata.Points, points, count * sizeof(PointF));
    memcpy((path)->pathdata.Types, types, count);
    (path)->pathdata.Count = count;
    (path)->datalen = count;

    (path)->fill = fill;
    (path)->newfigure = TRUE;

    return Ok;
}

Status PathCreatePath2I(GDIPCONST Point* points,
    GDIPCONST BYTE* types, INT count, FillMode fill, Path *path)
{
    PointF *ptF = NULL;
    Status ret;

    TRACE("(%p, %p, %d, %d, %p)\n", points, types, count, fill, path);

    ret = PointI2F(&ptF, points, count);
    ret = PathCreate2(ptF, types, count, fill, path);
    pfree(ptF);
    return ret;
}

Status PathDelete(Path *path)
{
    TRACE("(%p)\n", path);

    if(!path)
        return InvalidParameter;

    PathData_Free(&path->pathdata);

    return Ok;
}

Status PathTransform(Path *path, Matrix *matrix)
{
  TRACE("(%p, %p)\n", path, matrix);
  
  if(!path)
    return InvalidParameter;
  
  if(path->pathdata.Count == 0)
    return Ok;
  
  return GdipTransformMatrixPoints(matrix, path->pathdata.Points,
    path->pathdata.Count);
}

Status PathFlatten(Path *path, Matrix* matrix, REAL flatness)
{
    path_list_node_t *list, *node;
    PointF pt;
    INT i = 1;
    INT startidx = 0;
    Status stat;

    TRACE("(%p, %p, %.2f)\n", path, matrix, flatness);

    if(!path)
        return InvalidParameter;

    if(path->pathdata.Count == 0)
        return Ok;

    if(matrix){
        stat = PathTransform(path, matrix);
        if (stat != Ok)
            return stat;
    }

    pt = path->pathdata.Points[0];
    if(!init_path_list(&list, pt.X, pt.Y))
        return OutOfMemory;

    node = list;

    while(i < path->pathdata.Count){

        BYTE type = path->pathdata.Types[i] & PathPointTypePathTypeMask;
        path_list_node_t *start;

        pt = path->pathdata.Points[i];

        /* save last start point index */
        if(type == PathPointTypeStart)
            startidx = i;

        /* always add line points and start points */
        if((type == PathPointTypeStart) || (type == PathPointTypeLine)){
            if(!add_path_list_node(node, pt.X, pt.Y, path->pathdata.Types[i]))
                goto memout;

            node = node->next;
            ++i;
            continue;
        }

        /* Bezier curve */

        /* test for closed figure */
        if(path->pathdata.Types[i+1] & PathPointTypeCloseSubpath){
            pt = path->pathdata.Points[startidx];
            ++i;
        }
        else
        {
            i += 2;
            pt = path->pathdata.Points[i];
        };

        start = node;
        /* add Bezier end point */
        type = (path->pathdata.Types[i] & ~PathPointTypePathTypeMask) | PathPointTypeLine;
        if(!add_path_list_node(node, pt.X, pt.Y, type))
            goto memout;
        node = node->next;

        /* flatten curve */
        if(!flatten_bezier(start, path->pathdata.Points[i-2].X, path->pathdata.Points[i-2].Y,
                                  path->pathdata.Points[i-1].X, path->pathdata.Points[i-1].Y,
                           node, flatness))
            goto memout;

        ++i;
    }/* while */

    /* store path data back */
    i = path_list_count(list);
    if(!lengthen_path(path, i))
        goto memout;
    path->pathdata.Count = i;

    node = list;
    for(i = 0; i < path->pathdata.Count; i++){
        path->pathdata.Points[i] = node->pt;
        path->pathdata.Types[i]  = node->type;
        node = node->next;
    }

    free_path_list(list);
    return Ok;

memout:
    free_path_list(list);
    return OutOfMemory;
}

Status PathGetData(Path *path, PathData* pathData)
{
    TRACE("(%p, %p)\n", path, pathData);

    if(!path || !pathData)
        return InvalidParameter;

    /* Only copy data. pathData allocation/freeing controlled by wrapper class.
       Assumed that pathData is enough wide to get all data - controlled by wrapper too. */
    memcpy(pathData->Points, path->pathdata.Points, sizeof(PointF) * pathData->Count);
    memcpy(pathData->Types , path->pathdata.Types , pathData->Count);

    return Ok;
}

Status PathGetFillMode(Path *path, FillMode *fillmode)
{
    TRACE("(%p, %p)\n", path, fillmode);

    if(!path || !fillmode)
        return InvalidParameter;

    *fillmode = path->fill;

    return Ok;
}

Status PathGetLastPoint(Path* path, PointF* lastPoint)
{
    INT count;

    TRACE("(%p, %p)\n", path, lastPoint);

    if(!path || !lastPoint)
        return InvalidParameter;

    count = path->pathdata.Count;
    if(count > 0)
        *lastPoint = path->pathdata.Points[count-1];

    return Ok;
}

Status PathGetPoints(const Path *path, PointF* points, INT count)
{
    TRACE("(%p, %p, %d)\n", path, points, count);

    if(!path)
        return InvalidParameter;

    if(count < path->pathdata.Count)
        return InsufficientBuffer;

    memcpy(points, path->pathdata.Points, path->pathdata.Count * sizeof(PointF));

    return Ok;
}

Status PathGetPointsI(const Path *path, Point* points, INT count)
{
    Status ret;
    PointF *ptf;
    INT i;

    TRACE("(%p, %p, %d)\n", path, points, count);

    if(count <= 0)
        return InvalidParameter;

    ptf = (PointF*)pmalloc(sizeof(PointF)*count);
    if(!ptf)    return OutOfMemory;

    ret = PathGetPoints(path,ptf,count);
    if(ret == Ok) {
        for(i = 0;i < count;i++){
            points[i].X = gdip_round(ptf[i].X);
            points[i].Y = gdip_round(ptf[i].Y);
        }
    }
    pfree(ptf);

    return ret;
}

Status PathGetTypes(Path *path, BYTE* types, INT count)
{
    TRACE("(%p, %p, %d)\n", path, types, count);

    if(!path)
        return InvalidParameter;

    if(count < path->pathdata.Count)
        return InsufficientBuffer;

    memcpy(types, path->pathdata.Types, path->pathdata.Count);

    return Ok;
}

/* Windows expands the bounding box to the maximum possible bounding box
 * for a given pen.  For example, if a line join can extend past the point
 * it's joining by x units, the bounding box is extended by x units in every
 * direction (even though this is too conservative for most cases). */
Status PathGetWorldBounds(const Path* path, RectF* bounds,
    GDIPCONST Matrix *matrix, GDIPCONST Pen *pen)
{
    PointF * points, temp_pts[4];
    INT count, i;
    REAL path_width = 1.0, width, height, temp, low_x, low_y, high_x, high_y;

    TRACE("(%p, %p, %p, %p)\n", path, bounds, matrix, pen);

    /* Matrix and pen can be null. */
    if(!path || !bounds)
        return InvalidParameter;

    /* If path is empty just return. */
    count = path->pathdata.Count;
    if(count == 0){
        bounds->X = bounds->Y = bounds->Width = bounds->Height = 0.0;
        return Ok;
    }

    points = path->pathdata.Points;

    low_x = high_x = points[0].X;
    low_y = high_y = points[0].Y;

    for(i = 1; i < count; i++){
        low_x = MIN(low_x, points[i].X);
        low_y = MIN(low_y, points[i].Y);
        high_x = MAX(high_x, points[i].X);
        high_y = MAX(high_y, points[i].Y);
    }

    width = high_x - low_x;
    height = high_y - low_y;

    /* This looks unusual but it's the only way I can imitate windows. */
    if(matrix){
        temp_pts[0].X = low_x;
        temp_pts[0].Y = low_y;
        temp_pts[1].X = low_x;
        temp_pts[1].Y = high_y;
        temp_pts[2].X = high_x;
        temp_pts[2].Y = high_y;
        temp_pts[3].X = high_x;
        temp_pts[3].Y = low_y;

        GdipTransformMatrixPoints((Matrix*)matrix, temp_pts, 4);
        low_x = temp_pts[0].X;
        low_y = temp_pts[0].Y;

        for(i = 1; i < 4; i++){
            low_x = MIN(low_x, temp_pts[i].X);
            low_y = MIN(low_y, temp_pts[i].Y);
        }

        temp = width;
        width = height * fabs(matrix->matrix[2]) + width * fabs(matrix->matrix[0]);
        height = height * fabs(matrix->matrix[3]) + temp * fabs(matrix->matrix[1]);
    }

    if(pen){
        path_width = pen->width / 2.0;

        if(count > 2)
            path_width = MAX(path_width,  pen->width * pen->miterlimit / 2.0);
        /* FIXME: this should probably also check for the startcap */
        if(pen->endcap & LineCapNoAnchor)
            path_width = MAX(path_width,  pen->width * 2.2);

        low_x -= path_width;
        low_y -= path_width;
        width += 2.0 * path_width;
        height += 2.0 * path_width;
    }

    bounds->X = low_x;
    bounds->Y = low_y;
    bounds->Width = width;
    bounds->Height = height;

    return Ok;
}

Status PathGetWorldBoundsI(Path* path, Rect* bounds,
    GDIPCONST Matrix *matrix, GDIPCONST Pen *pen)
{
    Status ret;
    RectF boundsF;

    TRACE("(%p, %p, %p, %p)\n", path, bounds, matrix, pen);

    ret = PathGetWorldBounds(path,&boundsF,matrix,pen);

    if(ret == Ok){
        bounds->X      = gdip_round(boundsF.X);
        bounds->Y      = gdip_round(boundsF.Y);
        bounds->Width  = gdip_round(boundsF.Width);
        bounds->Height = gdip_round(boundsF.Height);
    }

    return ret;
}

Status PathGetPointCount(Path *path, INT *count)
{
    TRACE("(%p, %p)\n", path, count);

    if(!path)
        return InvalidParameter;

    *count = path->pathdata.Count;

    return Ok;
}

Status PathReversePath(Path* path)
{
    INT i, count;
    INT start = 0; /* position in reversed path */
    PathData revpath = {0};

    TRACE("(%p)\n", path);

    if(!path)
        return InvalidParameter;

    count = path->pathdata.Count;

    if(count == 0) return Ok;

    if(!PathData_SetSize(&revpath, count)){
        return OutOfMemory;
    }

    for(i = 0; i < count; i++){
        /* find next start point */
        if(path->pathdata.Types[count-i-1] == PathPointTypeStart){
            INT j;
            for(j = start; j <= i; j++){
                revpath.Points[j] = path->pathdata.Points[count-j-1];
                revpath.Types[j] = path->pathdata.Types[count-j-1];
            }
            /* mark start point */
            revpath.Types[start] = PathPointTypeStart;
            /* set 'figure' endpoint type */
            if(i-start > 1){
                revpath.Types[i] = path->pathdata.Types[count-start-1] & ~PathPointTypePathTypeMask;
                revpath.Types[i] |= revpath.Types[i-1];
            }
            else
                revpath.Types[i] = path->pathdata.Types[start];

            start = i+1;
        }
    }

    memcpy(path->pathdata.Points, revpath.Points, sizeof(PointF)*count);
    memcpy(path->pathdata.Types,  revpath.Types,  sizeof(BYTE)*count);

    PathData_Free(&revpath);
    return Ok;
}

Status PathIsOutlineVisiblePoint(Path* path, REAL x, REAL y,
    Pen *pen, Graphics *graphics, BOOL *result)
{
    static int calls;

    TRACE("(%p,%0.2f,%0.2f,%p,%p,%p)\n", path, x, y, pen, graphics, result);

    if(!path || !pen)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status PathIsOutlineVisiblePointI(Path* path, INT x, INT y,
                                       Pen *pen, Graphics *graphics, BOOL *result)
{
  TRACE("(%p, %d, %d, %p, %p, %p)\n", path, x, y, pen, graphics, result);
  
  return PathIsOutlineVisiblePoint(path, x, y, pen, graphics, result);
}

/*****************************************************************************
 * GdipIsVisiblePathPoint [GDIPLUS.@]
 */
Status PathIsVisiblePoint(Path* path, REAL x, REAL y, Graphics *graphics, BOOL *result)
{
#if 0
    Region *region;
    HRGN hrgn;
    Status status;

    if(!path || !result) return InvalidParameter;

    status = GdipCreateRegionPath(path, &region);
    if(status != Ok)
        return status;

    status = GdipGetRegionHRgn(region, graphics, &hrgn);
    if(status != Ok){
        GdipDeleteRegion(region);
        return status;
    }

    *result = PtInRegion(hrgn, gdip_round(x), gdip_round(y));

    DeleteObject(hrgn);
    GdipDeleteRegion(region);
    
#endif
    ASSERT(0);
    return Ok;
}

Status PathIsVisiblePointI(Path* path, INT x, INT y, Graphics *graphics, BOOL *result)
{
  TRACE("(%p, %d, %d, %p, %p)\n", path, x, y, graphics, result);
  
  return PathIsVisiblePoint(path, x, y, graphics, result);
}

Status PathStartPathFigure(Path *path)
{
    TRACE("(%p)\n", path);

    if(!path)
        return InvalidParameter;

    path->newfigure = TRUE;

    return Ok;
}

Status PathReset(Path *path)
{
    TRACE("(%p)\n", path);

    if(!path)
        return InvalidParameter;

    path->pathdata.Count = 0;
    path->newfigure = TRUE;
    path->fill = FillModeAlternate;

    return Ok;
}

Status PathSetPathFillMode(Path *path, FillMode fill)
{
    TRACE("(%p, %d)\n", path, fill);

    if(!path)
        return InvalidParameter;

    path->fill = fill;

    return Ok;
}

Status PathWarpPath(Path *path, Matrix* matrix,
    GDIPCONST PointF *points, INT count, REAL x, REAL y, REAL width,
    REAL height, WarpMode warpmode, REAL flatness)
{
    FIXME("(%p,%p,%p,%i,%0.2f,%0.2f,%0.2f,%0.2f,%i,%0.2f)\n", path, matrix,
        points, count, x, y, width, height, warpmode, flatness);

    return NotImplemented;
}

static void add_bevel_point(const PointF *endpoint, const PointF *nextpoint,
    Pen *pen, int right_side, path_list_node_t **last_point)
{
    REAL segment_dy = nextpoint->Y-endpoint->Y;
    REAL segment_dx = nextpoint->X-endpoint->X;
    REAL segment_length = sqrtf(segment_dy*segment_dy + segment_dx*segment_dx);
    REAL distance = pen->width/2.0;
    REAL bevel_dx, bevel_dy;

    if (right_side)
    {
        bevel_dx = -distance * segment_dy / segment_length;
        bevel_dy = distance * segment_dx / segment_length;
    }
    else
    {
        bevel_dx = distance * segment_dy / segment_length;
        bevel_dy = -distance * segment_dx / segment_length;
    }

    *last_point = add_path_list_node(*last_point, endpoint->X + bevel_dx,
        endpoint->Y + bevel_dy, PathPointTypeLine);
}

static void widen_joint(const PointF *p1, const PointF *p2, const PointF *p3,
    Pen* pen, path_list_node_t **last_point)
{
    switch (pen->join)
    {
    case LineJoinMiter:
    case LineJoinMiterClipped:
        if ((p2->X - p1->X) * (p3->Y - p1->Y) > (p2->Y - p1->Y) * (p3->X - p1->X))
        {
            float distance = pen->width/2.0;
            float length_0 = sqrtf((p2->X-p1->X)*(p2->X-p1->X)+(p2->Y-p1->Y)*(p2->Y-p1->Y));
            float length_1 = sqrtf((p3->X-p2->X)*(p3->X-p2->X)+(p3->Y-p2->Y)*(p3->Y-p2->Y));
            float dx0 = distance * (p2->X - p1->X) / length_0;
            float dy0 = distance * (p2->Y - p1->Y) / length_0;
            float dx1 = distance * (p3->X - p2->X) / length_1;
            float dy1 = distance * (p3->Y - p2->Y) / length_1;
            float det = (dy0*dx1 - dx0*dy1);
            float dx = (dx0*dx1*(dx0-dx1) + dy0*dy0*dx1 - dy1*dy1*dx0)/det;
            float dy = (dy0*dy1*(dy0-dy1) + dx0*dx0*dy1 - dx1*dx1*dy0)/det;
            if (dx*dx + dy*dy < pen->miterlimit*pen->miterlimit * distance*distance)
            {
                *last_point = add_path_list_node(*last_point, p2->X + dx,
                    p2->Y + dy, PathPointTypeLine);
                break;
            }
            else if (pen->join == LineJoinMiter)
            {
                static int once;
                if (!once++)
                    FIXME("should add a clipped corner\n");
            }
            /* else fall-through */
        }
        /* else fall-through */
    default:
    case LineJoinBevel:
        add_bevel_point(p2, p1, pen, 1, last_point);
        add_bevel_point(p2, p3, pen, 0, last_point);
        break;
    }
}

static void widen_cap(const PointF *endpoint, const PointF *nextpoint,
    Pen *pen, LineCap cap, CustomLineCap *custom, int add_first_points,
    int add_last_point, path_list_node_t **last_point)
{
    switch (cap)
    {
    default:
    case LineCapFlat:
        if (add_first_points)
            add_bevel_point(endpoint, nextpoint, pen, 1, last_point);
        if (add_last_point)
            add_bevel_point(endpoint, nextpoint, pen, 0, last_point);
        break;
    case LineCapSquare:
    {
        REAL segment_dy = nextpoint->Y-endpoint->Y;
        REAL segment_dx = nextpoint->X-endpoint->X;
        REAL segment_length = sqrtf(segment_dy*segment_dy + segment_dx*segment_dx);
        REAL distance = pen->width/2.0;
        REAL bevel_dx, bevel_dy;
        REAL extend_dx, extend_dy;

        extend_dx = -distance * segment_dx / segment_length;
        extend_dy = -distance * segment_dy / segment_length;

        bevel_dx = -distance * segment_dy / segment_length;
        bevel_dy = distance * segment_dx / segment_length;

        if (add_first_points)
            *last_point = add_path_list_node(*last_point, endpoint->X + extend_dx + bevel_dx,
                endpoint->Y + extend_dy + bevel_dy, PathPointTypeLine);

        if (add_last_point)
            *last_point = add_path_list_node(*last_point, endpoint->X + extend_dx - bevel_dx,
                endpoint->Y + extend_dy - bevel_dy, PathPointTypeLine);

        break;
    }
    case LineCapRound:
    {
        REAL segment_dy = nextpoint->Y-endpoint->Y;
        REAL segment_dx = nextpoint->X-endpoint->X;
        REAL segment_length = sqrtf(segment_dy*segment_dy + segment_dx*segment_dx);
        REAL distance = pen->width/2.0;
        REAL dx, dy, dx2, dy2;
        const REAL control_point_distance = (REAL)(0.5522847498307935); /* 4/3 * (sqrt(2) - 1) */

        if (add_first_points)
        {
            dx = -distance * segment_dx / segment_length;
            dy = -distance * segment_dy / segment_length;

            dx2 = dx * control_point_distance;
            dy2 = dy * control_point_distance;

            /* first 90-degree arc */
            *last_point = add_path_list_node(*last_point, endpoint->X + dy,
                endpoint->Y - dx, PathPointTypeLine);

            *last_point = add_path_list_node(*last_point, endpoint->X + dy + dx2,
                endpoint->Y - dx + dy2, PathPointTypeBezier);

            *last_point = add_path_list_node(*last_point, endpoint->X + dx + dy2,
                endpoint->Y + dy - dx2, PathPointTypeBezier);

            /* midpoint */
            *last_point = add_path_list_node(*last_point, endpoint->X + dx,
                endpoint->Y + dy, PathPointTypeBezier);

            /* second 90-degree arc */
            *last_point = add_path_list_node(*last_point, endpoint->X + dx - dy2,
                endpoint->Y + dy + dx2, PathPointTypeBezier);

            *last_point = add_path_list_node(*last_point, endpoint->X - dy + dx2,
                endpoint->Y + dx + dy2, PathPointTypeBezier);

            *last_point = add_path_list_node(*last_point, endpoint->X - dy,
                endpoint->Y + dx, PathPointTypeBezier);
        }
        break;
    }
    }
}

static void widen_open_figure(Path *path, Pen *pen, int start, int end,
    path_list_node_t **last_point)
{
    int i;
    path_list_node_t *prev_point;

    if (end <= start)
        return;

    prev_point = *last_point;

    widen_cap(&path->pathdata.Points[start], &path->pathdata.Points[start+1],
        pen, pen->startcap, pen->customstart, FALSE, TRUE, last_point);

    for (i=start+1; i<end; i++)
        widen_joint(&path->pathdata.Points[i-1], &path->pathdata.Points[i],
            &path->pathdata.Points[i+1], pen, last_point);

    widen_cap(&path->pathdata.Points[end], &path->pathdata.Points[end-1],
        pen, pen->endcap, pen->customend, TRUE, TRUE, last_point);

    for (i=end-1; i>start; i--)
        widen_joint(&path->pathdata.Points[i+1], &path->pathdata.Points[i],
            &path->pathdata.Points[i-1], pen, last_point);

    widen_cap(&path->pathdata.Points[start], &path->pathdata.Points[start+1],
        pen, pen->startcap, pen->customstart, TRUE, FALSE, last_point);

    prev_point->next->type = PathPointTypeStart;
    (*last_point)->type |= PathPointTypeCloseSubpath;
}

static void widen_closed_figure(Path *path, Pen *pen, int start, int end,
    path_list_node_t **last_point)
{
    int i;
    path_list_node_t *prev_point;

    if (end <= start+1)
        return;

    /* left outline */
    prev_point = *last_point;

    widen_joint(&path->pathdata.Points[end], &path->pathdata.Points[start],
        &path->pathdata.Points[start+1], pen, last_point);

    for (i=start+1; i<end; i++)
        widen_joint(&path->pathdata.Points[i-1], &path->pathdata.Points[i],
            &path->pathdata.Points[i+1], pen, last_point);

    widen_joint(&path->pathdata.Points[end-1], &path->pathdata.Points[end],
        &path->pathdata.Points[start], pen, last_point);

    prev_point->next->type = PathPointTypeStart;
    (*last_point)->type |= PathPointTypeCloseSubpath;

    /* right outline */
    prev_point = *last_point;

    widen_joint(&path->pathdata.Points[start], &path->pathdata.Points[end],
        &path->pathdata.Points[end-1], pen, last_point);

    for (i=end-1; i>start; i--)
        widen_joint(&path->pathdata.Points[i+1], &path->pathdata.Points[i],
            &path->pathdata.Points[i-1], pen, last_point);

    widen_joint(&path->pathdata.Points[start+1], &path->pathdata.Points[start],
        &path->pathdata.Points[end], pen, last_point);

    prev_point->next->type = PathPointTypeStart;
    (*last_point)->type |= PathPointTypeCloseSubpath;
}

Status PathWiden(Path *path, Pen *pen, Matrix *matrix,
    REAL flatness)
{
  Path flat_path[1]={0};
    Status status;
    path_list_node_t *points=NULL, *last_point=NULL;
    int i, subpath_start=0, new_length;
    BYTE type;

    TRACE("(%p,%p,%p,%0.2f)\n", path, pen, matrix, flatness);

    if (!path || !pen)
        return InvalidParameter;

    if (path->pathdata.Count <= 1)
        return OutOfMemory;

    status = GdipClonePath(path, flat_path);

    if (status == Ok)
        status = PathFlatten(flat_path, matrix, flatness);

    if (status == Ok && !init_path_list(&points, 314.0, 22.0))
        status = OutOfMemory;

    if (status == Ok)
    {
        last_point = points;

        if (pen->endcap > LineCapRound)
            FIXME("unimplemented end cap %x\n", pen->endcap);

        if (pen->startcap > LineCapRound)
            FIXME("unimplemented start cap %x\n", pen->startcap);

        if (pen->dashcap != DashCapFlat)
            FIXME("unimplemented dash cap %d\n", pen->dashcap);

        if (pen->join == LineJoinRound)
            FIXME("unimplemented line join %d\n", pen->join);

        if (pen->dash != DashStyleSolid)
            FIXME("unimplemented dash style %d\n", pen->dash);

        if (pen->align != PenAlignmentCenter)
            FIXME("unimplemented pen alignment %d\n", pen->align);

        for (i=0; i < flat_path->pathdata.Count; i++)
        {
            type = flat_path->pathdata.Types[i];

            if ((type&PathPointTypePathTypeMask) == PathPointTypeStart)
                subpath_start = i;

            if ((type&PathPointTypeCloseSubpath) == PathPointTypeCloseSubpath)
            {
                widen_closed_figure(flat_path, pen, subpath_start, i, &last_point);
            }
            else if (i == flat_path->pathdata.Count-1 ||
                (flat_path->pathdata.Types[i+1]&PathPointTypePathTypeMask) == PathPointTypeStart)
            {
                widen_open_figure(flat_path, pen, subpath_start, i, &last_point);
            }
        }

        new_length = path_list_count(points)-1;

        if (!lengthen_path(path, new_length))
            status = OutOfMemory;
    }

    if (status == Ok)
    {
        path->pathdata.Count = new_length;

        last_point = points->next;
        for (i = 0; i < new_length; i++)
        {
            path->pathdata.Points[i] = last_point->pt;
            path->pathdata.Types[i] = last_point->type;
            last_point = last_point->next;
        }

        path->fill = FillModeWinding;
    }

    free_path_list(points);

    PathDelete(flat_path);

    return status;
}

Status PathAddRectangle(Path *path, REAL x, REAL y, REAL width, REAL height)
{
  Path backup[1] = {0};
    PointF ptf[2];
    Status retstat;
    BOOL old_new;

    TRACE("(%p, %.2f, %.2f, %.2f, %.2f)\n", path, x, y, width, height);

    if(!path)
        return InvalidParameter;

    /* make a backup copy of path data */
    if((retstat = GdipClonePath(path, backup)) != Ok)
        return retstat;

    /* rectangle should start as new path */
    old_new = path->newfigure;
    path->newfigure = TRUE;
    if((retstat = PathAddLine(path,x,y,x+width,y)) != Ok){
        path->newfigure = old_new;
        goto fail;
    }

    ptf[0].X = x+width;
    ptf[0].Y = y+height;
    ptf[1].X = x;
    ptf[1].Y = y+height;

    if((retstat = PathAddLine2(path, ptf, 2)) != Ok)  goto fail;
    path->pathdata.Types[path->pathdata.Count-1] |= PathPointTypeCloseSubpath;

    /* free backup */
    PathDelete(backup);
    return Ok;

fail:
    /* reverting */
    PathData_Free(&path->pathdata);
    *path = *backup;
    return retstat;
}

Status PathAddRectangleI(Path *path, INT x, INT y,
    INT width, INT height)
{
    TRACE("(%p, %d, %d, %d, %d)\n", path, x, y, width, height);

    return PathAddRectangle(path,(REAL)x,(REAL)y,(REAL)width,(REAL)height);
}

Status PathAddRectangles(Path *path, GDIPCONST RectF *rects, INT count)
{
  Path backup[1] = {0};
    Status retstat;
    INT i;

    TRACE("(%p, %p, %d)\n", path, rects, count);

    /* count == 0 - verified condition  */
    if(!path || !rects || count == 0)
        return InvalidParameter;

    if(count < 0)
        return OutOfMemory;

    /* make a backup copy */
    if((retstat = GdipClonePath(path, backup)) != Ok)
        return retstat;

    for(i = 0; i < count; i++){
        if((retstat = PathAddRectangle(path,rects[i].X,rects[i].Y,rects[i].Width,rects[i].Height)) != Ok)
            goto fail;
    }

    /* free backup */
    PathDelete(backup);
    return Ok;

fail:
    /* reverting */
    PathData_Free(&path->pathdata);
    *path = *backup;

    return retstat;
}

Status PathAddRectanglesI(Path *path, GDIPCONST Rect *rects, INT count)
{
    RectF *rectsF = NULL;
    Status retstat;
    INT i;

    TRACE("(%p, %p, %d)\n", path, rects, count);

    if(!rects || count == 0)
        return InvalidParameter;

    if(count < 0)
        return OutOfMemory;

    rectsF = (RectF*)pmalloc(sizeof(RectF)*count);

    for(i = 0;i < count;i++){
        rectsF[i].X      = (REAL)rects[i].X;
        rectsF[i].Y      = (REAL)rects[i].Y;
        rectsF[i].Width  = (REAL)rects[i].Width;
        rectsF[i].Height = (REAL)rects[i].Height;
    }

    retstat = PathAddRectangles(path, rectsF, count);
    pfree(rectsF);

    return retstat;
}

Status PathSetPathMarker(Path* path)
{
    INT count;

    TRACE("(%p)\n", path);

    if(!path)
        return InvalidParameter;

    count = path->pathdata.Count;

    /* set marker flag */
    if(count > 0)
        path->pathdata.Types[count-1] |= PathPointTypePathMarker;

    return Ok;
}

Status PathClearPathMarkers(Path* path)
{
    INT count;
    INT i;

    TRACE("(%p)\n", path);

    if(!path)
        return InvalidParameter;

    count = path->pathdata.Count;

    for(i = 0; i < count - 1; i++){
        path->pathdata.Types[i] &= ~PathPointTypePathMarker;
    }

    return Ok;
}

Status PathWindingModeOutline(Path *path, Matrix *matrix, REAL flatness)
{
   FIXME("stub: %p, %p, %.2f\n", path, matrix, flatness);
   return NotImplemented;
}

