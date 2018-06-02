#include "_cv.h"

CC_IMPL CvSubdiv2D *
cvCreateSubdiv2D(int subdiv_type, int header_size,
                  int vtx_size, int quadedge_size, CvMemStorage * storage)
{
    CvSubdiv2D *subdiv = 0;

    CC_FUNCNAME("cvCleateSubdiv2D");

    __BEGIN__;

    if(!storage)
        CC_ERROR(CC_StsNullPtr, "");

    if(header_size < (int)sizeof(*subdiv) ||
        quadedge_size < (int)sizeof(CvQuadEdge2D) ||
        vtx_size < (int)sizeof(CvSubdiv2DPoint))
        CC_ERROR_FROM_STATUS(CC_BADSIZE_ERR);

    subdiv = (CvSubdiv2D *) cvCreateGraph(subdiv_type, header_size,
                                           vtx_size, quadedge_size, storage);

    
    __END__;

    return subdiv;
}


/****************************************************************************************\
*                                    Quad Edge  algebra                                  *
\****************************************************************************************/

static CvSubdiv2DEdge
cvSubdiv2DMakeEdge(CvSubdiv2D * subdiv)
{
    CvQuadEdge2D *edge = 0;
    CvSubdiv2DEdge edgehandle = 0;

    CC_FUNCNAME("cvSubdiv2DMakeEdge");

    __BEGIN__;

    if(!subdiv)
        CC_ERROR(CC_StsNullPtr, "");

    edge = (CvQuadEdge2D*)cvSetNew((CvSet*)subdiv->edges);
    CC_CHECK();

    memset(edge->pt, 0, sizeof(edge->pt));
    edgehandle = (CvSubdiv2DEdge) edge;

    edge->next[0] = edgehandle;
    edge->next[1] = edgehandle + 3;
    edge->next[2] = edgehandle + 2;
    edge->next[3] = edgehandle + 1;

    subdiv->quad_edges++;

    
    __END__;

    return edgehandle;
}


static CvSubdiv2DPoint *
cvSubdiv2DAddPoint(CvSubdiv2D * subdiv, CPoint2D32f pt, int is_virtual)
{
    CvSubdiv2DPoint *subdiv_point = 0;

    subdiv_point = (CvSubdiv2DPoint*)cvSetNew((CvSet*)subdiv);
    if(subdiv_point)
    {
        memset(subdiv_point, 0, subdiv->elem_size);
        subdiv_point->pt = pt;
        subdiv_point->first = 0;
        subdiv_point->flags |= is_virtual ? CC_SUBDIV2D_VIRTUAL_POINT_FLAG : 0;
    }

    return subdiv_point;
}


static void
cvSubdiv2DSplice(CvSubdiv2DEdge edgeA, CvSubdiv2DEdge edgeB)
{
    CvSubdiv2DEdge *a_next = &CC_SUBDIV2D_NEXT_EDGE(edgeA);
    CvSubdiv2DEdge *b_next = &CC_SUBDIV2D_NEXT_EDGE(edgeB);
    CvSubdiv2DEdge a_rot = cvSubdiv2DRotateEdge(*a_next, 1);
    CvSubdiv2DEdge b_rot = cvSubdiv2DRotateEdge(*b_next, 1);
    CvSubdiv2DEdge *a_rot_next = &CC_SUBDIV2D_NEXT_EDGE(a_rot);
    CvSubdiv2DEdge *b_rot_next = &CC_SUBDIV2D_NEXT_EDGE(b_rot);
    CvSubdiv2DEdge t;

    CC_SWAP(*a_next, *b_next, t);
    CC_SWAP(*a_rot_next, *b_rot_next, t);
}


static void
cvSubdiv2DSetEdgePoints(CvSubdiv2DEdge edge,
                         CvSubdiv2DPoint * org_pt, CvSubdiv2DPoint * dst_pt)
{
    CvQuadEdge2D *quadedge = (CvQuadEdge2D *) (edge & ~3);

    CC_FUNCNAME("cvSubdiv2DSetEdgePoints");

    __BEGIN__;

    if(!quadedge)
        CC_ERROR(CC_StsNullPtr, "");

    quadedge->pt[edge & 3] = org_pt;
    quadedge->pt[(edge + 2) & 3] = dst_pt;

    
    __END__;
}


static void
cvSubdiv2DDeleteEdge(CvSubdiv2D * subdiv, CvSubdiv2DEdge edge)
{
    CvQuadEdge2D *quadedge = (CvQuadEdge2D *) (edge & ~3);

    CC_FUNCNAME("cvSubdiv2DDeleteEdge");

    __BEGIN__;

    if(!subdiv || !quadedge)
        CC_ERROR(CC_StsNullPtr, "");

    cvSubdiv2DSplice(edge, cvSubdiv2DGetEdge(edge, CC_PREV_AROUND_ORG));

    {
    CvSubdiv2DEdge sym_edge = cvSubdiv2DSymEdge(edge);
    cvSubdiv2DSplice(sym_edge, cvSubdiv2DGetEdge(sym_edge, CC_PREV_AROUND_ORG));
    }

    cvSetRemoveByPtr((CvSet*)(subdiv->edges), quadedge);
    subdiv->quad_edges--;

    
    __END__;
}


static CvSubdiv2DEdge
cvSubdiv2DConnectEdges(CvSubdiv2D * subdiv, CvSubdiv2DEdge edgeA, CvSubdiv2DEdge edgeB)
{
    CvSubdiv2DEdge new_edge = 0;

    CC_FUNCNAME("cvSubdiv2DConnectPoints");

    __BEGIN__;

    CvSubdiv2DPoint *orgB, *dstA;

    if(!subdiv)
        CC_ERROR(CC_StsNullPtr, "");

    new_edge = cvSubdiv2DMakeEdge(subdiv);

    cvSubdiv2DSplice(new_edge, cvSubdiv2DGetEdge(edgeA, CC_NEXT_AROUND_LEFT));
    cvSubdiv2DSplice(cvSubdiv2DSymEdge(new_edge), edgeB);

    dstA = cvSubdiv2DEdgeDst(edgeA);
    orgB = cvSubdiv2DEdgeOrg(edgeB);
    cvSubdiv2DSetEdgePoints(new_edge, dstA, orgB);
    
    __END__;

    return new_edge;
}


static void
cvSubdiv2DSwapEdges(CvSubdiv2DEdge edge)
{
    CvSubdiv2DEdge sym_edge = cvSubdiv2DSymEdge(edge);
    CvSubdiv2DEdge a = cvSubdiv2DGetEdge(edge, CC_PREV_AROUND_ORG);
    CvSubdiv2DEdge b = cvSubdiv2DGetEdge(sym_edge, CC_PREV_AROUND_ORG);
    CvSubdiv2DPoint *dstB, *dstA;

    cvSubdiv2DSplice(edge, a);
    cvSubdiv2DSplice(sym_edge, b);

    dstA = cvSubdiv2DEdgeDst(a);
    dstB = cvSubdiv2DEdgeDst(b);
    cvSubdiv2DSetEdgePoints(edge, dstA, dstB);

    cvSubdiv2DSplice(edge, cvSubdiv2DGetEdge(a, CC_NEXT_AROUND_LEFT));
    cvSubdiv2DSplice(sym_edge, cvSubdiv2DGetEdge(b, CC_NEXT_AROUND_LEFT));
}


static int
icvIsRightOf(CPoint2D32f& pt, CvSubdiv2DEdge edge)
{
    CvSubdiv2DPoint *org = cvSubdiv2DEdgeOrg(edge), *dst = cvSubdiv2DEdgeDst(edge);
    suf32_t cw_area;
    cw_area.f = (float)cvTriangleArea(pt, dst->pt, org->pt);

    return (cw_area.i > 0)*2 - (cw_area.i*2 != 0);
}


CC_IMPL CvSubdiv2DPointLocation
cvSubdiv2DLocate(CvSubdiv2D * subdiv, CPoint2D32f pt,
                  CvSubdiv2DEdge * _edge, CvSubdiv2DPoint ** _point)
{
    CvSubdiv2DEdge edge = 0;
    CvSubdiv2DPoint *point = 0;
    CvSubdiv2DPointLocation location = CC_PTLOC_ERROR;

    int i, max_edges;
    int right_of_curr = 0;

    CC_FUNCNAME("cvSubdiv2DLocate");

    __BEGIN__;

    if(!subdiv)
        CC_ERROR(CC_StsNullPtr, "");

    if(!CC_IS_SUBDIV2D(subdiv))
        CC_ERROR_FROM_STATUS(CC_BADFLAG_ERR);

    max_edges = subdiv->quad_edges * 4;
    edge = subdiv->recent_edge;

    if(max_edges == 0)
        CC_ERROR_FROM_STATUS(CC_BADSIZE_ERR);
    if(!edge)
        CC_ERROR_FROM_STATUS(CC_NOTDEFINED_ERR);

    location = CC_PTLOC_OUTSIDE_RECT;
    if(pt.x < subdiv->topleft.x || pt.y < subdiv->topleft.y ||
        pt.x >= subdiv->bottomright.x || pt.y >= subdiv->bottomright.y)
        CC_ERROR_FROM_STATUS(CC_BADRANGE_ERR);

    location = CC_PTLOC_ERROR;

    right_of_curr = icvIsRightOf(pt, edge);
    if(right_of_curr > 0)
    {
        edge = cvSubdiv2DSymEdge(edge);
        right_of_curr = -right_of_curr;
    }

    for(i = 0; i < max_edges; i++)
    {
        CvSubdiv2DEdge onext_edge = cvSubdiv2DNextEdge(edge);
        CvSubdiv2DEdge dprev_edge = cvSubdiv2DGetEdge(edge, CC_PREV_AROUND_DST);

        int right_of_onext = icvIsRightOf(pt, onext_edge);
        int right_of_dprev = icvIsRightOf(pt, dprev_edge);

        if(right_of_dprev > 0)
        {
            if(right_of_onext > 0 || right_of_onext == 0 && right_of_curr == 0)
            {
                location = CC_PTLOC_INSIDE;
                EXIT;
            }
            else
            {
                right_of_curr = right_of_onext;
                edge = onext_edge;
            }
        }
        else
        {
            if(right_of_onext > 0)
            {
                if(right_of_dprev == 0 && right_of_curr == 0)
                {
                    location = CC_PTLOC_INSIDE;
                    EXIT;
                }
                else
                {
                    right_of_curr = right_of_dprev;
                    edge = dprev_edge;
                }
            }
            else if(right_of_curr == 0 &&
                     icvIsRightOf(cvSubdiv2DEdgeDst(onext_edge)->pt, edge) >= 0)
            {
                edge = cvSubdiv2DSymEdge(edge);
            }
            else
            {
                right_of_curr = right_of_onext;
                edge = onext_edge;
            }
        }
    }

    
    __END__;

    subdiv->recent_edge = edge;

    if(location == CC_PTLOC_INSIDE)
    {
        double t1, t2, t3;
        CPoint2D32f org_pt = cvSubdiv2DEdgeOrg(edge)->pt;
        CPoint2D32f dst_pt = cvSubdiv2DEdgeDst(edge)->pt;

        t1 = fabs(pt.x - org_pt.x);
        t1 += fabs(pt.y - org_pt.y);
        t2 = fabs(pt.x - dst_pt.x);
        t2 += fabs(pt.y - dst_pt.y);
        t3 = fabs(org_pt.x - dst_pt.x);
        t3 += fabs(org_pt.y - dst_pt.y);

        if(t1 < FLT_EPSILON)
        {
            location = CC_PTLOC_VERTEX;
            point = cvSubdiv2DEdgeOrg(edge);
            edge = 0;
        }
        else if(t2 < FLT_EPSILON)
        {
            location = CC_PTLOC_VERTEX;
            point = cvSubdiv2DEdgeDst(edge);
            edge = 0;
        }
        else if((t1 < t3 || t2 < t3) &&
                 fabs(cvTriangleArea(pt, org_pt, dst_pt)) < FLT_EPSILON)
        {
            location = CC_PTLOC_ON_EDGE;
            point = 0;
        }
    }

    if(location == CC_PTLOC_ERROR)
    {
        edge = 0;
        point = 0;
    }

    if(_edge)
        *_edge = edge;
    if(_point)
        *_point = point;

    return location;
}


CC_INLINE int
icvIsPtInCircle3(CPoint2D32f pt, CPoint2D32f a, CPoint2D32f b, CPoint2D32f c)
{
    double val = (a.x * a.x + a.y * a.y) * cvTriangleArea(b, c, pt);
    val -= (b.x * b.x + b.y * b.y) * cvTriangleArea(a, c, pt);
    val += (c.x * c.x + c.y * c.y) * cvTriangleArea(a, b, pt);
    val -= (pt.x * pt.x + pt.y * pt.y) * cvTriangleArea(a, b, c);

    return val > FLT_EPSILON ? 1 : val < -FLT_EPSILON ? -1 : 0;
}


CC_IMPL CvSubdiv2DPoint *
cvSubdivDelaunay2DInsert(CvSubdiv2D * subdiv, CPoint2D32f pt)
{
    CvSubdiv2DPoint *point = 0;
    CvSubdiv2DPointLocation location = CC_PTLOC_ERROR;

    CvSubdiv2DPoint *curr_point = 0, *first_point = 0;
    CvSubdiv2DEdge curr_edge = 0, deleted_edge = 0, base_edge = 0;
    int i, max_edges;

    CC_FUNCNAME("cvSubdivDelaunay2DInsert");

    __BEGIN__;

    if(!subdiv)
        CC_ERROR(CC_StsNullPtr, "");

    if(!CC_IS_SUBDIV2D(subdiv))
        CC_ERROR_FROM_STATUS(CC_BADFLAG_ERR);


    location = cvSubdiv2DLocate(subdiv, pt, &curr_edge, &curr_point);

    switch (location)
    {
    case CC_PTLOC_ERROR:
        CC_ERROR_FROM_STATUS(CC_BADSIZE_ERR);

    case CC_PTLOC_OUTSIDE_RECT:
        CC_ERROR_FROM_STATUS(CC_BADRANGE_ERR);

    case CC_PTLOC_VERTEX:
        point = curr_point;
        break;

    case CC_PTLOC_ON_EDGE:
        deleted_edge = curr_edge;
        subdiv->recent_edge = curr_edge = cvSubdiv2DGetEdge(curr_edge, CC_PREV_AROUND_ORG);
        cvSubdiv2DDeleteEdge(subdiv, deleted_edge);
        /* no break */

    case CC_PTLOC_INSIDE:

        assert(curr_edge != 0);
        subdiv->is_geometry_valid = 0;

        curr_point = cvSubdiv2DAddPoint(subdiv, pt, 0);
        CC_CHECK();

        base_edge = cvSubdiv2DMakeEdge(subdiv);
        first_point = cvSubdiv2DEdgeOrg(curr_edge);
        cvSubdiv2DSetEdgePoints(base_edge, first_point, curr_point);
        cvSubdiv2DSplice(base_edge, curr_edge);

        do
        {
            base_edge = cvSubdiv2DConnectEdges(subdiv, curr_edge,
                                                cvSubdiv2DSymEdge(base_edge));
            curr_edge = cvSubdiv2DGetEdge(base_edge, CC_PREV_AROUND_ORG);
        }
        while(cvSubdiv2DEdgeDst(curr_edge) != first_point);

        curr_edge = cvSubdiv2DGetEdge(base_edge, CC_PREV_AROUND_ORG);

        max_edges = subdiv->quad_edges * 4;

        for(i = 0; i < max_edges; i++)
        {
            CvSubdiv2DPoint *temp_dst = 0, *curr_org = 0, *curr_dst = 0;
            CvSubdiv2DEdge temp_edge = cvSubdiv2DGetEdge(curr_edge, CC_PREV_AROUND_ORG);

            temp_dst = cvSubdiv2DEdgeDst(temp_edge);
            curr_org = cvSubdiv2DEdgeOrg(curr_edge);
            curr_dst = cvSubdiv2DEdgeDst(curr_edge);

            if(icvIsRightOf(temp_dst->pt, curr_edge) > 0 &&
                icvIsPtInCircle3(curr_org->pt, temp_dst->pt,
                                  curr_dst->pt, curr_point->pt) < 0)
            {
                cvSubdiv2DSwapEdges(curr_edge);
                curr_edge = cvSubdiv2DGetEdge(curr_edge, CC_PREV_AROUND_ORG);
            }
            else if(curr_org == first_point)
            {
                break;
            }
            else
            {
                curr_edge = cvSubdiv2DGetEdge(cvSubdiv2DNextEdge(curr_edge),
                                               CC_PREV_AROUND_LEFT);
            }
        }
        break;
    default:
        assert(0);
        CC_ERROR_FROM_STATUS(CC_NOTDEFINED_ERR);
    }

    point = curr_point;

    
    __END__;

    //icvSubdiv2DCheck(subdiv);

    return point;
}


CC_IMPL void
cvInitSubdivDelaunay2D(CvSubdiv2D * subdiv, CRect rect)
{
    float big_coord = 3.f * MAX(rect.width, rect.height);
    CPoint2D32f ppA, ppB, ppC;
    CvSubdiv2DPoint *pA, *pB, *pC;
    CvSubdiv2DEdge edge_AB, edge_BC, edge_CA;
    float rx = (float) rect.x;
    float ry = (float) rect.y;

    CC_FUNCNAME("cvSubdivDelaunay2DInit");

    __BEGIN__;

    if(!subdiv)
        CC_ERROR(CC_StsNullPtr, "");

    cvClearSet((CvSet *) (subdiv->edges));
    cvClearSet((CvSet *) subdiv);

    subdiv->quad_edges = 0;
    subdiv->recent_edge = 0;
    subdiv->is_geometry_valid = 0;

    subdiv->topleft = cPoint2D32f(rx, ry);
    subdiv->bottomright = cPoint2D32f(rx + rect.width, ry + rect.height);

    ppA = cPoint2D32f(rx + big_coord, ry);
    ppB = cPoint2D32f(rx, ry + big_coord);
    ppC = cPoint2D32f(rx - big_coord, ry - big_coord);

    pA = cvSubdiv2DAddPoint(subdiv, ppA, 0);
    pB = cvSubdiv2DAddPoint(subdiv, ppB, 0);
    pC = cvSubdiv2DAddPoint(subdiv, ppC, 0);

    edge_AB = cvSubdiv2DMakeEdge(subdiv);
    edge_BC = cvSubdiv2DMakeEdge(subdiv);
    edge_CA = cvSubdiv2DMakeEdge(subdiv);

    cvSubdiv2DSetEdgePoints(edge_AB, pA, pB);
    cvSubdiv2DSetEdgePoints(edge_BC, pB, pC);
    cvSubdiv2DSetEdgePoints(edge_CA, pC, pA);

    cvSubdiv2DSplice(edge_AB, cvSubdiv2DSymEdge(edge_CA));
    cvSubdiv2DSplice(edge_BC, cvSubdiv2DSymEdge(edge_AB));
    cvSubdiv2DSplice(edge_CA, cvSubdiv2DSymEdge(edge_BC));

    subdiv->recent_edge = edge_AB;

    
    __END__;
}


CC_IMPL void
cvClearSubdivVoronoi2D(CvSubdiv2D * subdiv)
{
    int elem_size;
    int i, total;
    CvSeqReader reader;

    CC_FUNCNAME("cvClearVoronoi2D");

    __BEGIN__;

    if(!subdiv)
        CC_ERROR(CC_StsNullPtr, "");

    /* clear pointers to voronoi points */
    total = subdiv->edges->total;
    elem_size = subdiv->edges->elem_size;

    cvStartReadSeq((CvSeq *) (subdiv->edges), &reader, 0);

    for(i = 0; i < total; i++)
    {
        CvQuadEdge2D *quadedge = (CvQuadEdge2D *) reader.ptr;

        quadedge->pt[1] = quadedge->pt[3] = 0;
        CC_NEXT_SEQ_ELEM(elem_size, reader);
    }

    /* remove voronoi points */
    total = subdiv->total;
    elem_size = subdiv->elem_size;

    cvStartReadSeq((CvSeq *) subdiv, &reader, 0);

    for(i = 0; i < total; i++)
    {
        CvSubdiv2DPoint *pt = (CvSubdiv2DPoint *) reader.ptr;

        /* check for virtual point. it is also check that the point exists */
        if(pt->flags & CC_SUBDIV2D_VIRTUAL_POINT_FLAG)
        {
            cvSetRemoveByPtr((CvSet*)subdiv, pt);
        }
        CC_NEXT_SEQ_ELEM(elem_size, reader);
    }

    subdiv->is_geometry_valid = 0;

    
    __END__;
}


CC_IMPL void
cvCalcSubdivVoronoi2D(CvSubdiv2D * subdiv)
{
    CvSeqReader reader;
    int i, total, elem_size;

    CC_FUNCNAME("cvCalcSubdivVoronoi2D");

    __BEGIN__;

    if(!subdiv)
        CC_ERROR(CC_StsNullPtr, "");

    /* check if it is already calculated */
    if(subdiv->is_geometry_valid)
        EXIT;

    total = subdiv->edges->total;
    elem_size = subdiv->edges->elem_size;

    cvClearSubdivVoronoi2D(subdiv);

    cvStartReadSeq((CvSeq *) (subdiv->edges), &reader, 0);

    if(total <= 3)
        EXIT;

    /* skip first three edges (bounding triangle) */
    for(i = 0; i < 3; i++)
        CC_NEXT_SEQ_ELEM(elem_size, reader);

    /* loop through all quad-edges */
    for(; i < total; i++)
    {
        CvQuadEdge2D *quadedge = (CvQuadEdge2D *) (reader.ptr);

        if(CC_IS_SET_ELEM(quadedge))
        {
            CvSubdiv2DEdge edge0 = (CvSubdiv2DEdge) quadedge, edge1, edge2;
            double a0, b0, c0, a1, b1, c1;
            CPoint2D32f virt_point;
            CvSubdiv2DPoint *voronoi_point;

            if(!quadedge->pt[3])
            {
                edge1 = cvSubdiv2DGetEdge(edge0, CC_NEXT_AROUND_LEFT);
                edge2 = cvSubdiv2DGetEdge(edge1, CC_NEXT_AROUND_LEFT);

                icvCreateCenterNormalLine(edge0, &a0, &b0, &c0);
                icvCreateCenterNormalLine(edge1, &a1, &b1, &c1);

                icvIntersectLines3(&a0, &b0, &c0, &a1, &b1, &c1, &virt_point);
                if(fabs(virt_point.x) < FLT_MAX * 0.5 &&
                    fabs(virt_point.y) < FLT_MAX * 0.5)
                {
                    voronoi_point = cvSubdiv2DAddPoint(subdiv, virt_point, 1);

                    quadedge->pt[3] =
                        ((CvQuadEdge2D *) (edge1 & ~3))->pt[3 - (edge1 & 2)] =
                        ((CvQuadEdge2D *) (edge2 & ~3))->pt[3 - (edge2 & 2)] = voronoi_point;
                }
            }

            if(!quadedge->pt[1])
            {
                edge1 = cvSubdiv2DGetEdge(edge0, CC_NEXT_AROUND_RIGHT);
                edge2 = cvSubdiv2DGetEdge(edge1, CC_NEXT_AROUND_RIGHT);

                icvCreateCenterNormalLine(edge0, &a0, &b0, &c0);
                icvCreateCenterNormalLine(edge1, &a1, &b1, &c1);

                icvIntersectLines3(&a0, &b0, &c0, &a1, &b1, &c1, &virt_point);

                if(fabs(virt_point.x) < FLT_MAX * 0.5 &&
                    fabs(virt_point.y) < FLT_MAX * 0.5)
                {
                    voronoi_point = cvSubdiv2DAddPoint(subdiv, virt_point, 1);

                    quadedge->pt[1] =
                        ((CvQuadEdge2D *) (edge1 & ~3))->pt[1 + (edge1 & 2)] =
                        ((CvQuadEdge2D *) (edge2 & ~3))->pt[1 + (edge2 & 2)] = voronoi_point;
                }
            }
        }

        CC_NEXT_SEQ_ELEM(elem_size, reader);
    }

    subdiv->is_geometry_valid = 1;

    
    __END__;
}


static int
icvIsRightOf2(const CPoint2D32f& pt, const CPoint2D32f& org, const CPoint2D32f& diff)
{
    suf32_t cw_area;
    cw_area.f = (org.x - pt.x)*diff.y - (org.y - pt.y)*diff.x;
    return (cw_area.i > 0)*2 - (cw_area.i*2 != 0);
}


CC_IMPL CvSubdiv2DPoint*
cvFindNearestPoint2D(CvSubdiv2D* subdiv, CPoint2D32f pt)
{
    CvSubdiv2DPoint* point = 0;
    CPoint2D32f start;
    CPoint2D32f diff;
    CvSubdiv2DPointLocation loc;
    CvSubdiv2DEdge edge; 
    int i;
    
    CC_FUNCNAME("cvFindNearestPoint2D");

    __BEGIN__;

    if(!subdiv)
        CC_ERROR(CC_StsNullPtr, "");

    if(!CC_IS_SUBDIV2D(subdiv))
        CC_ERROR(CC_StsNullPtr, "");

    if(!subdiv->is_geometry_valid)
        cvCalcSubdivVoronoi2D(subdiv);

    loc = cvSubdiv2DLocate(subdiv, pt, &edge, &point);

    switch(loc)
    {
    case CC_PTLOC_ON_EDGE:
    case CC_PTLOC_INSIDE:
        break;
    default:
        EXIT;
    }

    point = 0;

    start = cvSubdiv2DEdgeOrg(edge)->pt;
    diff.x = pt.x - start.x;
    diff.y = pt.y - start.y;

    edge = cvSubdiv2DRotateEdge(edge, 1);

    for(i = 0; i < subdiv->total; i++)
    {
        CPoint2D32f t;
        
        for(;;)
        {
            assert(cvSubdiv2DEdgeDst(edge));
            
            t = cvSubdiv2DEdgeDst(edge)->pt;
            if(icvIsRightOf2(t, start, diff) >= 0)
                break;

            edge = cvSubdiv2DGetEdge(edge, CC_NEXT_AROUND_LEFT);
        }

        for(;;)
        {
            assert(cvSubdiv2DEdgeOrg(edge));

            t = cvSubdiv2DEdgeOrg(edge)->pt;
            if(icvIsRightOf2(t, start, diff) < 0)
                break;

            edge = cvSubdiv2DGetEdge(edge, CC_PREV_AROUND_LEFT);
        }

        {
            CPoint2D32f tempDiff = cvSubdiv2DEdgeDst(edge)->pt;
            t = cvSubdiv2DEdgeOrg(edge)->pt;
            tempDiff.x -= t.x;
            tempDiff.y -= t.y;

            if(icvIsRightOf2(pt, t, tempDiff) >= 0)
            {
                point = cvSubdiv2DEdgeOrg(cvSubdiv2DRotateEdge(edge, 3));
                break;
            }
        }

        edge = cvSubdiv2DSymEdge(edge);
    }

    __END__;

    return point;
}

/* Removed from the main interface */

#if 0
/* Adds new isolated quadedge to the subdivision */
OPENCVAPI  CvSubdiv2DEdge  cvSubdiv2DMakeEdge(CvSubdiv2D* subdiv);


/* Adds new isolated point to subdivision */
OPENCVAPI  CvSubdiv2DPoint*   cvSubdiv2DAddPoint(CvSubdiv2D* subdiv,
                                                  CPoint2D32f pt, int is_virtual);


/* Does a splice operation for two quadedges */
OPENCVAPI  void  cvSubdiv2DSplice(CvSubdiv2DEdge  edgeA,  CvSubdiv2DEdge  edgeB);


/* Assigns ending [non-virtual] points for given quadedge */
OPENCVAPI  void  cvSubdiv2DSetEdgePoints(CvSubdiv2DEdge edge,
                                          CvSubdiv2DPoint* org_pt,
                                          CvSubdiv2DPoint* dst_pt);

/* Removes quadedge from subdivision */
OPENCVAPI  void  cvSubdiv2DDeleteEdge(CvSubdiv2D* subdiv, CvSubdiv2DEdge edge);


/* Connects estination point of the first edge with origin point of the second edge */
OPENCVAPI  CvSubdiv2DEdge  cvSubdiv2DConnectEdges(CvSubdiv2D* subdiv,
                                                   CvSubdiv2DEdge edgeA,
                                                   CvSubdiv2DEdge edgeB);

/* Swaps diagonal in two connected Delaunay facets */
OPENCVAPI  void  cvSubdiv2DSwapEdges(CvSubdiv2DEdge edge);
#endif


