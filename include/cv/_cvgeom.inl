
#ifndef _CC_GEOM_H_
#define _CC_GEOM_H_

/* Finds distance between two points */
CC_INLINE  float  icvDistanceL2_32f(CPoint2D32f pt1, CPoint2D32f pt2)
{
    float dx = pt2.x - pt1.x;
    float dy = pt2.y - pt1.y;

    return cvSqrt(dx*dx + dy*dy);
}


int  icvIntersectLines(double x1, double dx1, double y1, double dy1,
                        double x2, double dx2, double y2, double dy2,
                        double* t2);


void icvCreateCenterNormalLine(CvSubdiv2DEdge edge, double* a, double* b, double* c);

void icvIntersectLines3(double* a0, double* b0, double* c0,
                         double* a1, double* b1, double* c1,
                         CPoint2D32f* point);


#define _CC_BINTREE_LIST()                                          \
   struct _CvTrianAttr* prev_v;   /* pointer to the parent  element on the previous level of the tree  */    \
   struct _CvTrianAttr* next_v1;   /* pointer to the child  element on the next level of the tree  */        \
   struct _CvTrianAttr* next_v2;   /* pointer to the child  element on the next level of the tree  */        

typedef struct _CvTrianAttr
{
   CPoint pt;    /* Coordinates x and y of the vertex  which don't lie on the base line LMIAT  */
   char sign;             /*  sign of the triangle   */
   double area;       /*   area of the triangle    */
   double r1;   /*  The ratio of the height of triangle to the base of the triangle  */
   double r2;  /*   The ratio of the projection of the left side of the triangle on the base to the base */
   _CC_BINTREE_LIST()    /* structure double list   */
}
_CvTrianAttr;


/* curvature: 0 - 1-curvature, 1 - k-cosine curvature. */
CStatus  icvApproximateChainTC89(CChain*      chain,
                                   int header_size,
                                   CvMemStorage* storage,
                                   CvSeq**   contour,
                                   int method);

#endif /*_IPCVGEOM_H_*/


