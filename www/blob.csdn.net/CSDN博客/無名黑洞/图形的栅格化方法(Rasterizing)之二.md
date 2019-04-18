# 图形的栅格化方法(Rasterizing)之二 - 無名黑洞 - CSDN博客
2009年10月05日 05:06:00[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：4891
如果我们不但要扫描图形轮廓，而且要在图形内部填充颜色。基本方法是建立一个最小包围盒(Bounding Box)将图形包住，然后依次扫描盒中的像素，看是否在图形内部，是的话就涂上颜色。下面介绍一些具体方法。
**Half-space tests**
假设有一个三角形，如何确定某个点是否在三角形内部？考虑三角形的三条边，这三条边各自把二维空间分成了两个半面。对三条边各取一个半面，使它们两两互相有交集，其交集就在三角形内。
缺点是不能作用在凹面体上。如果要判断凹面体内部，必须先把它分割成小三角形。
**Jordan Curve Theorem**
假设多边形内部有一个点向某个方向放出一条射线， 若击中多边形的边奇数次，则在内部。否则，就在外部。
有一些具体情况需要讨论。比如射线刚好击中一个顶点，或者划过一条边。此外，当图形是自交错的时候(self - intersection)，此方法也会失效。
**克劳算法 Crow's Algorithm**
克劳算法是一种扫描线算法( Scan Line Algorithms )。算法认为1）内点和外点的辨别只需要在图形的边界判定; 2)边界的改变只发生在顶点处。因此，我们只需要建立一个存储顶点的数组，利用顶点来建立边的约束，再逐行在两条边界之间扫描就行了！这种算法具有很好的效果和效率。为了帮助理解，下面给出该算法的全部c++实现：
//Crow.h
typedef float   GzCoord[3];
void scanX(GzRender	*render, GzCoord *l, GzCoord *r, int y);
void GzScanConverter(GzRender	*render, GzCoord vList[], int n);
void scanY (GzRender	*render, GzCoord vList[], int n, int i);
GzPutDisplay()的作用是把像素数据写到显示缓存当中。
//Crow.cpp
void difference(float v1, float v2, float *s, float *ds, float d, float f)
{
    *ds = (v2  - v1 ) / d;
    *s  = v1  + f * (*ds);
}
void differenceX(GzCoord *v1, GzCoord *v2, GzCoord *s, GzCoord *ds, int x1)
{
	float f = x1 - (*v1)[X];
	float d = ((*v2)[X] - (*v1)[X]);
    difference((*v1)[Z], (*v2)[Z], &(*s)[Z], &(*ds)[Z], d, f);
}
void differenceY(GzCoord *v1, GzCoord *v2, GzCoord *s, GzCoord *ds, int y1)
{
	float f = y1 - (*v1)[Y];
	float d = ((*v2)[Y] - (*v1)[Y]);
	difference((*v1)[X], (*v2)[X], &(*s)[X], &(*ds)[X], d, f);
	difference((*v1)[Z], (*v2)[Z], &(*s)[Z], &(*ds)[Z], d, f);
}
void increment(GzCoord *edge, GzCoord *delta)
{
    (*edge)[X] += (*delta)[X];
	(*edge)[Z] += (*delta)[Z];
}
//vList is an ordered list of the polygon’s vertices
void GzScanConverter(GzRender	*render, GzCoord vList[], int n)
{
    int iMin = 0;
    for (int i = 0; i < n; i++)
        if (vList[i][Y] < vList[iMin][Y])
            iMin = i;
    scanY (render, vList, n, iMin);
}
void scanY (GzRender	*render, GzCoord vList[], int n, int i)
{
    int li, ri; // left & right upper endpoint indices
    int ly, ry; // left & right upper endpoint y values
    GzCoord l, dl; // current left edge and delta
    GzCoord r, dr; // current right edge and delta
    int rem; // number of remaining vertices
    int y; // current scanline
    li = ri = i;
    ly = ry = y = (int)ceil(vList[i][Y]);
    for( rem = n; rem > 0;)
    {
        // find appropriate left edge
        while( ly <= y && rem > 0 )
        {
            rem--;
            i = li - 1;
            if( i < 0 ) i = n - 1;
            ly = (int)ceil( vList[i][Y] );
            if( ly > y )
                differenceY( &vList[li], &vList[i], &l,&dl, y);
            li = i;
        }
        // find appropriate right edge
        while( ry <= y && rem > 0 )
        {
            rem--;
            i = ri + 1;
            if( i >= n ) i = 0;
            ry = (int)ceil( vList[i][Y]);
            if( ry > y )
                differenceY( &vList[ri], &vList[i], &r,&dr, y);
            ri = i;
        }
        // while l & r span y (the current scanline), draw the span
        for( ; y < ly && y < ry; y++)
        {
            scanX(render, &l, &r, y );
            increment( &l, &dl );
            increment( &r, &dr );
        }
    }
}
void scanX(GzRender	*render,GzCoord *l, GzCoord *r, int y)
{
    int lx, rx;
    GzCoord s, ds;
    
    lx = (int)ceil((*l)[X]);
    rx = (int)ceil((*r)[X]);
 	int no_backward_face_culling = 1;
	if(lx <= rx) {
        differenceX(l, r, &s, &ds, lx);
        for(int x = lx; x < rx; x++)
        {
            //setPixel(x,y);
			GzPutDisplay(render->display, x, y,
				ctoi( render->flatcolor[RED] ) , 
				ctoi( render->flatcolor[GREEN] ), 
				ctoi( render->flatcolor[BLUE] ), 
				1, int(s[Z]) );
            increment(&s,&ds);
        }
	} 
	else if (no_backward_face_culling) {
        differenceX(r, l, &s, &ds, rx);
        for(int x = rx; x < lx; x++)
        {
            //setPixel(x,y);
			GzPutDisplay(render->display, x, y,
				ctoi( render->flatcolor[RED] ) , 
				ctoi( render->flatcolor[GREEN] ), 
				ctoi( render->flatcolor[BLUE] ), 
				1, int(s[Z]) );
            increment(&s,&ds);
        }
	}
}
