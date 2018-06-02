
// This program is pfree software; you can redistribute it and/or modify  it under the terms of the GNU General Public License as published by  the Free Software Foundation; either version 2 of the License, or (at  your option) any later version.
// This program is distributed in the hope that it will be useful, but  WITHOUT ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  General Public License for more details.
// You should have received a copy of the GNU General Public License  along with this program; if not, write to the Free Software  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,  USA. 

// #region auxiliary functions
/* calculate point of a bezier curve */
static DPOINT bezier(double t, DPOINT p0, DPOINT p1, DPOINT p2, DPOINT p3)
{
    double s = 1 - t;
    DPOINT res;

    // Note: a good optimizing compiler (such as gcc-3) reduces the
    // following to 16 multiplications, using common subexpression
    // elimination. 

    res.x = s * s * s * p0.x + 3 * (s * s * t) * p1.x + 3 * (t * t * s) * p2.x + t * t * t * p3.x;
    res.y = s * s * s * p0.y + 3 * (s * s * t) * p1.y + 3 * (t * t * s) * p2.y + t * t * t * p3.y;

    return res;
}
// calculate p1 x p2
#define xprod(p1, p2)  ((p1).x * (p2).y - (p1).y * (p2).x)

// calculate distance between two points
static double ddist(DPOINT p, DPOINT q)
{
    return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
}

// calculate (p1-p0)x(p3-p2)
static double cprod(DPOINT p0, DPOINT p1, DPOINT p2, DPOINT p3)
{
    double x1, y1, x2, y2;

    x1 = p1.x - p0.x;
    y1 = p1.y - p0.y;
    x2 = p3.x - p2.x;
    y2 = p3.y - p2.y;

    return x1 * y2 - x2 * y1;
}
/* calculate (p1-p0)*(p2-p0) */
static double iprod(DPOINT p0, DPOINT p1, DPOINT p2)
{
    double x1, y1, x2, y2;

    x1 = p1.x - p0.x;
    y1 = p1.y - p0.y;
    x2 = p2.x - p0.x;
    y2 = p2.y - p0.y;

    return x1 * x2 + y1 * y2;
}

/* calculate (p1-p0)*(p3-p2) */
static double iprod1(DPOINT p0, DPOINT p1, DPOINT p2, DPOINT p3)
{
    double x1, y1, x2, y2;

    x1 = p1.x - p0.x;
    y1 = p1.y - p0.y;
    x2 = p3.x - p2.x;
    y2 = p3.y - p2.y;

    return x1 * x2 + y1 * y2;
}

/* return a direction that is 90 degrees counterclockwise from p2-p0,
   but then restricted to one of the major wind directions (n, nw, w, etc) */
static IPOINT dorth_infty(DPOINT p0, DPOINT p2)
{
    IPOINT r;

    r.y = CC_SIGN(p2.x - p0.x);
    r.x = -CC_SIGN(p2.y - p0.y);

    return r;
}
/* range over the straight line segment [a,b] when lambda ranges over [0,1] */
static DPOINT interval(double lambda, DPOINT a, DPOINT b)
{
    DPOINT res;

    res.x = a.x + lambda * (b.x - a.x);
    res.y = a.y + lambda * (b.y - a.y);
    return res;
}
/* return (p1-p0)x(p2-p0), the area of the parallelogram */
static double dpara(DPOINT p0, DPOINT p1, DPOINT p2)
{
    double x1, y1, x2, y2;

    x1 = p1.x - p0.x;
    y1 = p1.y - p0.y;
    x2 = p2.x - p0.x;
    y2 = p2.y - p0.y;

    return x1 * y2 - x2 * y1;
}

// calculate the point t in [0..1] on the (convex) bezier curve
// (p0,p1,p2,p3) which is tangent to q1-q0. Return -1.0 if there is no
// solution in [0..1]. 
static double tangent(DPOINT p0, DPOINT p1, DPOINT p2, DPOINT p3, DPOINT q0, DPOINT q1)
{
    double A, B, C;   /* (1-t)^2 A + 2(1-t)t B + t^2 C = 0 */
    double a, b, c;   /* a t^2 + b t + c = 0 */
    double d, s, r1, r2;

    A = cprod(p0, p1, q0, q1);
    B = cprod(p1, p2, q0, q1);
    C = cprod(p2, p3, q0, q1);

    a = A - 2 * B + C;
    b = -2 * A + 2 * B;
    c = A;

    d = b * b - 4 * a * c;

    if (a == 0 || d < 0)
    {
        return -1.0;
    }

    s = sqrt(d);

    r1 = (-b + s) / (2 * a);
    r2 = (-b - s) / (2 * a);

    if (r1 >= 0 && r1 <= 1)
    {
        return r1;
    }
    else if (r2 >= 0 && r2 <= 1)
    {
        return r2;
    }
    else
    {
        return -1.0;
    }
}
/* ddenom/dpara have the property that the square of radius 1 centered
   at p1 intersects the line p0p2 iff |dpara(p0,p1,p2)| <= ddenom(p0,p2) */
static double ddenom(DPOINT p0, DPOINT p2)
{
    IPOINT r = dorth_infty(p0, p2);

    return r.y * (p2.x - p0.x) - r.x * (p2.y - p0.y);
}

/* return 1 if a <= b < c < a, in a cyclic sense (mod n) */
static bool cyclic(int a, int b, int c)
{
    if (a <= c)
    {
        return (a <= b && b < c);
    }
    else
    {
        return (a <= b || b < c);
    }
}
/* determine the center and slope of the line i..j. Assume i<j. Needs
"sum" components of p to be set. */
static void pointslope(const Path* pp, int i, int j, DPOINT* ctr, DPOINT* dir)
{
    /* assume i<j */

    int n = pp->pt_length;
    SumStruct* sums = pp->Sums;


    double x, y, x2, xy, y2;
    double k;
    double a, b, c, lambda2, l;
    int r = 0; /* rotations from i to j */

    while (j >= n)
    {
        j -= n;
        r += 1;
    }
    while (i >= n)
    {
        i -= n;
        r -= 1;
    }
    while (j < 0)
    {
        j += n;
        r -= 1;
    }
    while (i < 0)
    {
        i += n;
        r += 1;
    }

    x = sums[j + 1].x - sums[i].x + r * sums[n].x;
    y = sums[j + 1].y - sums[i].y + r * sums[n].y;
    x2 = sums[j + 1].x2 - sums[i].x2 + r * sums[n].x2;
    xy = sums[j + 1].xy - sums[i].xy + r * sums[n].xy;
    y2 = sums[j + 1].y2 - sums[i].y2 + r * sums[n].y2;
    k = j + 1 - i + r * n;

    ctr->x = x / k;
    ctr->y = y / k;

    a = (x2 - (double)x * x / k) / k;
    b = (xy - (double)x * y / k) / k;
    c = (y2 - (double)y * y / k) / k;

    lambda2 = (a + c + sqrt((a - c) * (a - c) + 4 * b * b)) / 2; /* larger e.value */

    /* now find e.vector for lambda2 */
    a -= lambda2;
    c -= lambda2;

    if (fabs(a) >= fabs(c))
    {
        l = sqrt(a * a + b * b);
        if (l != 0)
        {
            dir->x = -b / l;
            dir->y = a / l;
        }
    }
    else
    {
        l = sqrt(c * c + b * b);
        if (l != 0)
        {
            dir->x = -c / l;
            dir->y = b / l;
        }
    }
    if (l == 0)
    {
        dir->x = dir->y = 0;   /* sometimes this can happen when k=4: the two eigenvalues coincide */
    }
}



// #region Static function of Potrace


/// <summary>
/// 
/// Produces a binary Matrix with Dimensions b->w+2 and b->h +2, where
/// the Border ( of width) in the Matrix is filled with 'true' -values.
/// On this way we avoid a lot of boundsinequalities.
/// For the threshold, we take the Maximum of (R,G,B ) of a Pixel at x,y. If this is less then the threshold the resultMatrix at x+1, y+1 is filled with
/// false else with true.
/// 
/// </summary>
/// <param name="b"> A Bitmap, which will be transformed to a binary Matrix</param>
/// <param name="_treshold">Gives a threshold ( between 1 and 254 ) for Converting</param>
/// <returns>Returns a binaray boolean Matrix </returns>
static int BitMapToBinary(const img_t* im, int _treshold, img_t* Result)
{
    //BitmapData SourceData = b.LockBits(new Rectangle(0, 0, b->w, b->h), System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
    // inflated Result

    int SourceStride = im->s;
    int x, y, H = im->h, W = im->w;
    imsetsize(Result, im->h + 2, im->w + 2, 1, 1);

    {
        int Ydisp = 0;
        for (y = 0; y < H; y++) {
            const byte* SourcePtr = im->tt.data + y*im->s;
            byte* ResultPtr = Result->tt.data + (y + 1)*Result->s;
            for (x = 0; x < W; x++, SourcePtr+=3) {
                int ss = MAX3(SourcePtr[2], SourcePtr[1], SourcePtr[0]);
                ResultPtr[x + 1] = (ss >= _treshold);
            }
        }
    }
    // White Border
    for (x = 0; x < W; x++)
    {
        Result->tt.data[x] = true;
        Result->tt.data[x + (Result->h - 1)*Result->s] = true;
    }
    for (y = 1; y < H-1; y++)
    {
        Result->tt.data[y*Result->s] = true;
        Result->tt.data[Result->w - 1+ y*Result->s] = true;
    }

    return 0;

}
/// <summary>
/// Makes a Black and White Bitmap from the Data of a Binarymatrix.
/// Value with 'true' returns a white Pixel such with 'false' a Black pixel.
/// </summary>
///<param name="IgnoreBorder">If this value is set then a Border with 1 Pixel is ignored</param>
/// <param name="Matrix">A Binary Matrix, which have boolean values</param>
/// <returns>Returns a Black and white Image </returns>
static int BinaryToBitmap(const img_t* im, bool IgnoreBorder, img_t* im2)
{
    int W = im->w, H = im->h, d = 0, i, j;

    if (IgnoreBorder)
    {
        W = W - 2;
        H = H - 2;
        d = 1;
    }

    for (j = 0; j < H; j++) {
      const byte* SrcPtr = im->tt.data + (j + d)*im->s + d;
      byte* DstPtr = im2->tt.data + j*im2->s;
      for (i = 0; i < W; i++, DstPtr+=3) {
        DstPtr[0] = DstPtr[1] = DstPtr[2] = SrcPtr[i] ? 255 : 0;
      }
    }
    
    return 0;
}
/// <summary>
/// Searches a x and a y such that source[x,y] = true and source[x+1,y] false.
/// If this not exists, false will be returned else the result is true. 
/// 
/// </summary>
/// <param name="source">Is a Binary Matrix, which is produced by <see cref="BitMapToBinary"/>
/// <param name="x">x index in the source Matrix</param>
/// <param name="y">y index in the source Matrix</param>
static bool FindNext(const img_t* im, int* px, int* py)
{
  int x, y;
  for (y = 1; y < im->h - 1; y++) {
    for (x = 0; x < im->w - 1; x++) {
      if (!im->tt.data[x + 1 + y*im->s]) {// black found
        *px = x, *py = y;
        return true;
      }
    }
  }
  x = -1;
  *px = x, *py = y;
  return false;
}
// <summary>
/// Searches a x and a y inside the const Path* pp such that source[x,y] = true and source[x+1,y] false.
/// If this not exists, false will be returned else the result is true. 
/// 
/// </summary>
/// <param name="source">Is a Binary Matrix, which is produced by <see cref="BitMapToBinary"/>
/// <param name="x">x index in the source Matrix</param>
/// <param name="y">y index in the source Matrix</param>
static bool FindNextInPath(const img_t* im, int* px, int* py, const Path* pp)
{
    int i = 0, j, k, current_intervals_count=0, step = im->s;
    int x, y;
    const uchar* ptr = im->tt.data;
    int n = pp->pt_length;
    const monoton_interval_t* monoton_intervals = pp->monoton_intervals;
    monoton_interval_t MI={0};
    monoton_interval_t* current_intervals=0;
    
    if (pp->monoton_intervals_count == 0) return false;

    MI = monoton_intervals[0];
    monoton_interval_ResetCurrentID(&MI, n);
    y = pp->pt[MI.CurrentID].y;
    current_intervals = MALLOC(monoton_interval_t, pp->monoton_intervals_count+1);
    current_intervals[current_intervals_count++] = MI;
    MI.CurrentID = monoton_interval_Min(&MI);

    while ((i + 1 < pp->monoton_intervals_count) && (monoton_interval_MinY(monoton_intervals + i + 1, pp->pt) == y))
    {
        MI = monoton_intervals[i + 1];
        monoton_interval_ResetCurrentID(&MI, n);
        current_intervals[current_intervals_count++] = MI;
        i++;
    }

    while (current_intervals_count > 0)
    {
        for (k = 0; k < current_intervals_count - 1; k++)
        {
            int x1 = pp->pt[(current_intervals[k]).CurrentID].x + 1;
            int x2 = pp->pt[(current_intervals[k + 1]).CurrentID].x;

            for (x = x1; x <= x2; x++)
                if (!ptr[y*step+x])
                {
                    x--;
                    return true;
                }
            k++;
        }

        y++;
        for (j = current_intervals_count - 1; j >= 0; j--)
        {
            int CID;
            monoton_interval_t M = current_intervals[j];

            if (y > monoton_interval_MaxY(&M, pp->pt))
            {
                //current_intervals.RemoveAt(j);
                MEMDEL(current_intervals, current_intervals_count, j, 1), --current_intervals_count;
                continue;
            }
            CID = M.CurrentID;
            do
            {
                if (M.Increasing)
                    CID = mod(CID + 1, n);
                else
                    CID = mod(CID - 1, n);
            }
            while (pp->pt[CID].y < y);
            M.CurrentID = CID;
        }
        // Add Items of monoton_intervals with Miny==y
        while ((i + 1 < pp->monoton_intervals_count) && (monoton_interval_MinY(monoton_intervals+i + 1, pp->pt) == y))
        {
            monoton_interval_t NewInt = monoton_intervals[i + 1];
            int j = 0;
            // search the correct x-Position
            int _x = pp->pt[monoton_interval_Min(&NewInt)].x;
            while ((j < current_intervals_count) && (_x > pp->pt[current_intervals[j].CurrentID].x)) j++;
            //current_intervals.Insert(j, NewInt);
            MEMINST(current_intervals, current_intervals_count, j, &NewInt, 1), ++current_intervals_count;
            monoton_interval_ResetCurrentID(&NewInt, n);
            i++;
        }
    }
    FREE(current_intervals);
    return false;
}

// Apply quadratic form Q to vector w = (w.x,w.y)
static double quadform(const double* Q, DPOINT w)
{
    double v[] ={ w.x, w.y, 1 };
    int i, j;
    double sum = 0;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            sum += v[i] * Q[i, j] * v[j];
        }
    }
    return sum;
}

static int findNextTrace(const img_t* im, int* px, int* py, int* pDir)
{
  int Dir = *pDir, x = *px, y=*py;
  switch (Dir)
  {
  case West:
    {
      if (!im->tt.data[(x + 1)+im->s*(y + 1)])
      {
        y++;
        Dir = North;
      }
      else
        
        if (!im->tt.data[(x + 1)+im->s*(y)])
        {
          x++;
          Dir = West;
        }
        else
        {
          y--;
          Dir = South;
        }
        break;
    }
    
  case South:
    {
      if (!im->tt.data[(x + 1)+im->s*(y)])
      {
        x++;
        Dir = West;
      }
      else
        if (!im->tt.data[(x)+im->s*(y)])
        {
          y--;
          Dir = South;
        }
        else
        {
          x--;
          Dir = East;
        }
        break;
    }
  case East:
    {
      if (!im->tt.data[(x)+im->s*(y)])
      {
        y--;
        Dir = South;
      } else if (!im->tt.data[(x)+im->s*(y + 1)]) {
        x--;
        Dir = East;
      }
      else
      {
        y++;
        Dir = North;
      }
      break;
    }
    
  case North:
    {
      if (!im->tt.data[(x)+im->s*(y + 1)]) {
        x--;
        Dir = East;
      } else if (!im->tt.data[(x + 1)+im->s*(y + 1)]) {
        y++;
        Dir = North;
      } else {
        x++;
        Dir = West;
      }
      break;
    }
  }
  *pDir = Dir, *px = x, *py=y;
  return 0;
}

static int Getmonoton_intervals(const IPOINT* Pts, int Pts_Length, Path* pp)
{
  int n = Pts_Length, FirstStrongMonoton, Up, i;
  monoton_interval_t Interval={0};
  monoton_interval_t* L=0;
  int L_Count=0;

  if (n == 0) return 0;
  L = MALLOC(monoton_interval_t, Pts_Length+1);
  pp->monoton_intervals = MALLOC(monoton_interval_t, Pts_Length+1);
  
  //----- Start with Strong Monoton (Pts[i].y < Pts[i+1].y) or (Pts[i].y > Pts[i+1].y)
  FirstStrongMonoton = 0;
  while (Pts[FirstStrongMonoton].y == Pts[FirstStrongMonoton + 1].y) FirstStrongMonoton++;
  Up = (Pts[FirstStrongMonoton].y < Pts[FirstStrongMonoton + 1].y);
  Interval = make_monoton_interval(Up, FirstStrongMonoton, FirstStrongMonoton);
  L[L_Count++] = Interval;
  i = FirstStrongMonoton;
  do
  {
    // Interval.to = i;
    if ((Pts[i].y == Pts[mod(i + 1, n)].y) || (Up == (Pts[i].y < Pts[mod(i + 1, n)].y))) {
      Interval.to = i;
    } else {
      Up = (Pts[i].y < Pts[mod(i + 1, n)].y);
      Interval = make_monoton_interval(Up, i, i);
      L[L_Count++] = (Interval);
    }
    i = mod(i + 1, n);
  }
  while (i != FirstStrongMonoton);

  // Connect the Last with first 
  if (L_Count / 2 * 2 != L_Count) {
    monoton_interval_t M0 = L[0];
    monoton_interval_t ML = L[L_Count - 1];
    M0.from = ML.from;
    //L.RemoveAt(L_Count - 1);
    --L_Count;
  }
  
  //----- order now by the MIN y - value of interval to result 
  // and as second Key by the x-value
  //
  while (L_Count > 0)
  {
    monoton_interval_t M = L[0];
    i = 0;
    // order by y-value
    while ((i < pp->monoton_intervals_count) && (Pts[monoton_interval_Min(&M)].y > Pts[monoton_interval_Min(pp->monoton_intervals+i)].y)) i++;
    // order by x- value as second Key
    while ((i < pp->monoton_intervals_count) && (Pts[monoton_interval_Min(&M)].y == Pts[monoton_interval_Min(pp->monoton_intervals+i)].y) &&
      (Pts[monoton_interval_Min(&M)].x > (Pts[monoton_interval_Min(pp->monoton_intervals+i)].x))) i++;
    //pp->monoton_intervals_count.Insert(i, M);
    MEMINST(pp->monoton_intervals, pp->monoton_intervals_count, i, &M, 1), pp->monoton_intervals_count++;
    //L.RemoveAt(0);
    MEMDEL(L, L_Count, 0, 1), --L_Count;
  }
  FREE(L);
  return 0;
}

/// <summary>
/// Compute a path in the binary matrix.
/// Start path at the point (x0,x1), which must be an upper left corner
/// of the path. Also compute the area enclosed by the path. Return a
/// new path_t object, or NULL on error (note that a legitimate path
/// cannot have length 0). 
/// We omit turnpolicies and SIGN
/// </summary>
/// <param name="Matrix">Binary Matrix</param>
/// <returns></returns>
static int findpath(const img_t* im, IPOINT Start, Path* pp)
{
    IPOINT* L = 0;
    int Dir = North;
    int i, x, y, _y;
    int area = 0;
    int diry = -1;
    int L_Count=0;
    x = Start.x;
    y = Start.y;

    do
    {
        // area += x * diry;
        L[L_Count++] = iPOINT(x, y);
        _y = y;
        findNextTrace(im, &x, &y, &Dir);
        diry = _y - y;
        area += x * diry;
    }
    while ((x != Start.x) || (y != Start.y));

    if (L_Count == 0) {
      return 0;
    }
    //Path result = new Path();
    pp->pt = MALLOC(IPOINT, L_Count);
    pp->area = area;

    for (i = 0; i < L_Count; i++) pp->pt[i] = L[i];

    // Shift 1 to be compatible with Potrace

    if (pp->pt_length > 0)
    {
        IPOINT P = pp->pt[pp->pt_length - 1];
        for (i = pp->pt_length - 1; i >= 0; i--)
        {
            if (i > 0)
                pp->pt[i] = pp->pt[i - 1];
            else
                pp->pt[0] = P;
        }
    }

    Getmonoton_intervals(pp->pt, pp->pt_length, pp);
    return 0;
}

static void Xor_Path(const img_t* im, Path* pp)
{
    int i = 0, j, k, x, y, current_intervals_count=0;
    int n = pp->pt_length;
    monoton_interval_t* monoton_intervals = 0;
    monoton_interval_t* current_intervals = 0;
    monoton_interval_t MI;
    if (pp->monoton_intervals_count == 0) return;

    monoton_intervals = pp->monoton_intervals;
    MI = monoton_intervals[0];
    monoton_interval_ResetCurrentID(&MI, n);
    y = pp->pt[MI.CurrentID].y;
    current_intervals = MALLOC(monoton_interval_t, pp->monoton_intervals_count);
    current_intervals[current_intervals_count++] = (MI);
    MI.CurrentID = monoton_interval_Min(&MI);

    while ((i + 1 < pp->monoton_intervals_count) && (monoton_interval_MinY(monoton_intervals+i + 1, pp->pt) == y))
    {
        MI = monoton_intervals[i + 1];
        monoton_interval_ResetCurrentID(&MI, n);
        current_intervals[current_intervals_count++] = (MI);
        i++;
    }

    while (current_intervals_count > 0)
    {   // invertLine
        for (k = 0; k < current_intervals_count - 1; k++)
        {
            int x1 = pp->pt[(current_intervals[k]).CurrentID].x + 1;
            int x2 = pp->pt[(current_intervals[k + 1]).CurrentID].x;
            for (x = x1; x <= x2; x++) im->tt.data[x+im->s*y] = !im->tt.data[x+im->s*y];
            k++;
        }

        y++;
        for (j = current_intervals_count - 1; j >= 0; j--)
        {
            int CID;
            monoton_interval_t M = current_intervals[j];

            if (y > monoton_interval_MaxY(&M, pp->pt))
            {
                //current_intervals.RemoveAt(j);
                MEMDEL(current_intervals, current_intervals_count, j, 1), --current_intervals_count;
                continue;
            }
            CID = M.CurrentID;
            do
            {
                if (M.Increasing)
                    CID = mod(CID + 1, n);
                else
                    CID = mod(CID - 1, n);
            }
            while (pp->pt[CID].y < y);
            M.CurrentID = CID;
        }
        // Add Items of monoton_intervals with Down.y==y
        while ((i + 1 < pp->monoton_intervals_count) && (monoton_interval_MinY(monoton_intervals+i + 1, pp->pt) == y))
        {
            monoton_interval_t NewInt = monoton_intervals[i + 1];
            int j = 0;
            // search the correct x-Position
            int _x = pp->pt[monoton_interval_Min(&NewInt)].x;
            while ((j < current_intervals_count) && (_x > pp->pt[(current_intervals[j]).CurrentID].x)) j++;
            //current_intervals.Insert(j, NewInt);
            MEMINST(current_intervals, current_intervals_count, j, &NewInt, 1), ++current_intervals_count;
            monoton_interval_ResetCurrentID(&NewInt, n);
            i++;
        }
    }
}

/* ---------------------------------------------------------------------- */
/// <summary>
/// Preparation: fill in the sum* fields of a path (used for later rapid summing). 
/// </summary>
/// <param name="pp">Path for which the preparation will be done</param>
/// <returns></returns>
static void calc_sums(Path* pp)
{
    int i, x, y, x0, y0;
    int n = pp->pt_length;
    ASSERT(pp->pt && pp->pt_length>0);

    pp->Sums = MALLOC(SumStruct, n + 1);
    // origin 
    x0 = pp->pt[0].x;
    y0 = pp->pt[0].y;

    // preparatory computation for later fast summing 
    //pp->sums[0].x2 = pp->sums[0].xy = pp->sums[0].y2 = pp->sums[0].x = pp->sums[0].y = 0;
    pp->Sums[0].x2 = pp->Sums[0].xy = pp->Sums[0].y2 = pp->Sums[0].x = pp->Sums[0].y = 0;

    for (i = 0; i < n; i++)
    {
        x = pp->pt[i].x - x0;
        y = pp->pt[i].y - y0;
        pp->Sums[i + 1].x = pp->Sums[i].x + x;
        pp->Sums[i + 1].y = pp->Sums[i].y + y;
        pp->Sums[i + 1].x2 = pp->Sums[i].x2 + x * x;
        pp->Sums[i + 1].xy = pp->Sums[i].xy + x * y;
        pp->Sums[i + 1].y2 = pp->Sums[i].y2 + y * y;
    }
}
/* ---------------------------------------------------------------------- */
/* Stage 2: calculate the optimal polygon (Sec. 2.2.2-2.2.4). */

/* Auxiliary function: calculate the penalty of an edge from i to j in
   the given path. This needs the "lon" and "sum*" data. */

static double penalty3(const Path* pp, int i, int j)
{
    int n = pp->pt_length;

    /* assume 0<=i<j<=n  */
    double x, y, x2, xy, y2;
    double k;
    double a, b, c, s;
    double px, py, ex, ey;
    const SumStruct* sums = pp->Sums;
    const IPOINT* pt = pp->pt;

    int r = 0; /* rotations from i to j */
    if (j >= n)
    {
        j -= n;
        r += 1;
    }


    x = sums[j + 1].x - sums[i].x + r * sums[n].x;
    y = sums[j + 1].y - sums[i].y + r * sums[n].y;
    x2 = sums[j + 1].x2 - sums[i].x2 + r * sums[n].x2;
    xy = sums[j + 1].xy - sums[i].xy + r * sums[n].xy;
    y2 = sums[j + 1].y2 - sums[i].y2 + r * sums[n].y2;
    k = j + 1 - i + r * n;

    px = (pt[i].x + pt[j].x) / 2.0 - pt[0].x;
    py = (pt[i].y + pt[j].y) / 2.0 - pt[0].y;
    ey = (pt[j].x - pt[i].x);
    ex = -(pt[j].y - pt[i].y);

    a = ((x2 - 2 * x * px) / k + px * px);
    b = ((xy - x * py - y * px) / k + px * py);
    c = ((y2 - 2 * y * py) / k + py * py);

    s = ex * ex * a + 2 * ex * ey * b + ey * ey * c;

    return sqrt(s);
}

static void calc_lon(Path* pp)
{
    int i, j, k, k1;
    int a, b, c, d;
    int ct[] = { 0, 0, 0, 0 };
    int dir;
    IPOINT constraint[2] = {0};
    IPOINT cur;
    IPOINT off;
    IPOINT dk;  /* direction of k-k1 */
    IPOINT* pt = pp->pt;
    int n = pp->pt_length;
    int* Pivot = MALLOC(int, n);
    int* nc = MALLOC(int, n);
    // initialize the nc data structure. Point from each point to the
    // furthest future point to which it is connected by a vertical or
    // horizontal segment. We take advantage of the fact that there is
    // always a direction change at 0 (due to the path decomposition
    // algorithm). But even if this were not so, there is no harm, as
    // in practice, correctness does not depend on the word "furthest"
    // above.  

    k = 0;
    for (i = n - 1; i >= 0; i--)
    {
        if (pt[i].x != pt[k].x && pt[i].y != pt[k].y)
        {
            k = i + 1;  /* necessarily i<n-1 in this case */
        }
        nc[i] = k;
    }


    pp->Lon = MALLOC(int, n);
    /* determine pivot points: for each i, let pivk[i] be the furthest k
such that all j with i<j<k lie on a line connecting i,k. */

    for (i = n - 1; i >= 0; i--)
    {

        ct[0] = ct[1] = ct[2] = ct[3] = 0;

        /* keep track of "directions" that have occurred */
        dir = (3 + 3 * (pt[mod(i + 1, n)].x - pt[i].x) + (pt[mod(i + 1, n)].y - pt[i].y)) / 2;
        ct[dir]++;

        constraint[0].x = 0;
        constraint[0].y = 0;
        constraint[1].x = 0;
        constraint[1].y = 0;

        /* find the next k such that no straight line from i to k */
        k = nc[i];
        k1 = i;
        while (true)
        {
            dir = (3 + 3 * SIGN(pt[k].x - pt[k1].x) + SIGN(pt[k].y - pt[k1].y)) / 2;
            ct[dir]++;


            /* if all four "directions" have occurred, cut this path */
            if ((ct[0] == 1) && (ct[1] == 1) && (ct[2] == 1) && (ct[3] == 1))
            {
                Pivot[i] = k1;
                goto foundk;
            }

            cur.x = pt[k].x - pt[i].x;
            cur.y = pt[k].y - pt[i].y;

            /* see if current constraint is violated */
            if (xprod(constraint[0], cur) < 0 || xprod(constraint[1], cur) > 0)
            {
                goto constraint_viol;
            }

            /* else, update constraint */
            if (ABS(cur.x) <= 1 && ABS(cur.y) <= 1)
            {
                /* no constraint */
            }
            else
            {
                off.x = cur.x + ((cur.y >= 0 && (cur.y > 0 || cur.x < 0)) ? 1 : -1);
                off.y = cur.y + ((cur.x <= 0 && (cur.x < 0 || cur.y < 0)) ? 1 : -1);
                if (xprod(constraint[0], off) >= 0)
                {
                    constraint[0] = off;
                }
                off.x = cur.x + ((cur.y <= 0 && (cur.y < 0 || cur.x < 0)) ? 1 : -1);
                off.y = cur.y + ((cur.x >= 0 && (cur.x > 0 || cur.y < 0)) ? 1 : -1);
                if (xprod(constraint[1], off) <= 0)
                {
                    constraint[1] = off;
                }
            }
            k1 = k;
            k = nc[k1];
            if (!cyclic(k, i, k1))
            {
                break;
            }
        }
    constraint_viol:
        /* k1 was the last "corner" satisfying the current constraint, and
           k is the first one violating it. We now need to find the last
           point along k1..k which satisfied the constraint. */

        dk.x = SIGN(pt[k].x - pt[k1].x);
        dk.y = SIGN(pt[k].y - pt[k1].y);
        cur.x = pt[k1].x - pt[i].x;
        cur.y = pt[k1].y - pt[i].y;

        /* find largest integer j such that xprod(constraint[0], cur+j*dk)
           >= 0 and xprod(constraint[1], cur+j*dk) <= 0. Use bilinearity
           of xprod. */
        a = xprod(constraint[0], cur);
        b = xprod(constraint[0], dk);
        c = xprod(constraint[1], cur);
        d = xprod(constraint[1], dk);
        /* find largest integer j such that a+j*b>=0 and c+j*d<=0. This
           can be solved with integer arithmetic. */
        j = INT_MAX;
        if (b < 0)
        {
            j = floordiv(a, -b);
        }
        if (d > 0)
        {
            j = MIN(j, floordiv(-c, d));
        }
        Pivot[i] = mod(k1 + j, n);
    foundk:
        ;
    } /* for i */

    /* clean up: for each i, let lon[i] be the largest k such that for
       all i' with i<=i'<k, i'<k<=pivk[i']. */

    j = Pivot[n - 1];
    pp->Lon[n - 1] = j;

    for (i = n - 2; i >= 0; i--)
    {
        if (cyclic(i + 1, Pivot[i], j))
        {
            j = Pivot[i];
        }
        pp->Lon[i] = j;

    }

    for (i = n - 1; cyclic(mod(i + 1, n), j, pp->Lon[i]); i--)
    {
        pp->Lon[i] = j;

    }
    FREE(Pivot);
    FREE(nc);
}

// find the optimal polygon. Fill in the m and po components. Return 1
// on failure with errno set, else 0. Non-cyclic version: assumes i=0
// is in the polygon. Fixme: ### implement cyclic version. 
static void bestpolygon(Path* pp)
{
    int i, j, m, k;
    int n = pp->pt_length;
    double* pen = MALLOC(double, n + 1); /* pen[n+1]: penalty vector */
    int* prev = MALLOC(int, n + 1);   /* prev[n+1]: best path pointer vector */
    int* clip0 = MALLOC(int, n);  /* clip0[n]: longest segment pointer, non-cyclic */
    int* clip1 = MALLOC(int, n + 1);  /* clip1[n+1]: backwards segment pointer, non-cyclic */
    int* seg0 = MALLOC(int, n + 1);    /* seg0[m+1]: forward segment bounds, m<=n */
    int* seg1 = MALLOC(int, n + 1);   /* seg1[m+1]: backward segment bounds, m<=n */
    int* B=0;

    double thispen;
    double best;
    int c;

    /* calculate clipped paths */
    for (i = 0; i < n; i++)
    {
        c = mod(pp->Lon[mod(i - 1, n)] - 1, n);

        if (c == i)
        {
            c = mod(i + 1, n);
        }
        if (c < i)
        {
            clip0[i] = n;
        }
        else
        {
            clip0[i] = c;
        }
    }

    /* calculate backwards path clipping, non-cyclic. j <= clip0[i] iff
    clip1[j] <= i, for i,j=0..n. */
    j = 1;
    for (i = 0; i < n; i++)
    {
        while (j <= clip0[i])
        {
            clip1[j] = i;
            j++;
        }
    }

    /* calculate seg0[j] = longest path from 0 with j segments */
    i = 0;
    for (j = 0; i < n; j++)
    {
        seg0[j] = i;
        i = clip0[i];
    }
    seg0[j] = n;
    m = j;

    /* calculate seg1[j] = longest path to n with m-j segments */
    i = n;
    for (j = m; j > 0; j--)
    {
        seg1[j] = i;
        i = clip1[i];
    }
    seg1[0] = 0;

    /* now find the shortest path with m segments, based on penalty3 */
    /* note: the outer 2 loops jointly have at most n interations, thus
    the worst-case behavior here is quadratic. In practice, it is
    close to linear since the inner loop tends to be short. */
    pen[0] = 0;
    for (j = 1; j <= m; j++)
    {
        for (i = seg1[j]; i <= seg0[j]; i++)
        {
            best = -1;
            for (k = seg0[j - 1]; k >= clip1[i]; k--)
            {
                thispen = penalty3(pp, k, i) + pen[k];
                if (best < 0 || thispen < best)
                {
                    prev[i] = k;
                    best = thispen;
                }
            }
            pen[i] = best;
        }
    }


    /* read off shortest path */

    B = MALLOC(int, m);

    //pp->po = new int[m];
    for (i = n, j = m - 1; i > 0; j--)
    {
        i = prev[i];
        B[j] = i;
    }

    pp->po = B;
    pp->po_Length = m;
    FREE(pen);
    FREE(prev);
    FREE(clip0);
    FREE(clip1);
    FREE(seg0);
    FREE(seg1);
}

/* Stage 3: vertex adjustment (Sec. 2.3.1). */

/* Adjust vertices of optimal polygon: calculate the intersection of
   the two "optimal" line segments, then move it into the unit square
   if it lies outside. Return 1 with errno set on error; 0 on
   success. */

/* calculate "optimal" point-slope representation for each line
segment */
static void adjust_vertices(const Path* pp)
{
    int m = pp->po_Length;
    int* po = pp->po;
    IPOINT* pt = pp->pt;
    int n = pp->pt_length;

    int x0 = pt[0].x;
    int y0 = pt[0].y;

    DPOINT[] ctr = new DPOINT[m];      /* ctr[m] */
    DPOINT[] dir = new DPOINT[m];      /* dir[m] */
    //quadform_t *q = NULL;      /* q[m] */
    double[, ,] q = new double[m, 3, 3];
    double[] v = new double[3];
    double d;
    int i, j, k, l;
    DPOINT s;
    pp->Curves = new privcurve(m);
    /* calculate "optimal" point-slope representation for each line
    segment */
    for (i = 0; i < m; i++)
    {
        j = po[mod(i + 1, m)];
        j = mod(j - po[i], n) + po[i];
        pointslope(pp, po[i], j, ref ctr[i], ref dir[i]);
    }
    /* represent each line segment as a singular quadratic form; the
         distance of a point (x,y) from the line segment will be
         (x,y,1)Q(x,y,1)^t, where Q=q[i]. */

    for (i = 0; i < m; i++)
    {
        d = dir[i].x * dir[i].x + dir[i].y * dir[i].y;

        if (d == 0.0)
        {
            for (j = 0; j < 3; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    q[i, j, k] = 0;
                }
            }
        }
        else
        {
            v[0] = dir[i].y;
            v[1] = -dir[i].x;
            v[2] = -v[1] * ctr[i].y - v[0] * ctr[i].x;
            for (l = 0; l < 3; l++)
            {
                for (k = 0; k < 3; k++)
                {
                    q[i, l, k] = v[l] * v[k] / d;
                }
            }
        }
    }
    /* now calculate the "intersections" of consecutive segments.
       Instead of using the actual intersection, we find the point
       within a given unit square which minimizes the square distance to
       the two lines. */
    for (i = 0; i < m; i++)
    {
        double[,] Q = new double[3, 3];
        DPOINT w;
        double dx, dy;
        double det;
        double MIN, cand; /* minimum and candidate for minimum of quad. form */
        double xmin, ymin;	/* coordinates of minimum */
        int z;

        /* let s be the vertex, in coordinates relative to x0/y0 */
        s.x = pt[po[i]].x - x0;
        s.y = pt[po[i]].y - y0;

        /* intersect segments i-1 and i */

        j = mod(i - 1, m);

        /* add quadratic forms */
        for (l = 0; l < 3; l++)
        {
            for (k = 0; k < 3; k++)
            {
                Q[l, k] = q[j, l, k] + q[i, l, k];
            }
        }
        while (true)
        {
            /* minimize the quadratic form Q on the unit square */
            /* find intersection */
            det = Q[0, 0] * Q[1, 1] - Q[0, 1] * Q[1, 0];
            if (det != 0.0)
            {
                w.x = (-Q[0, 2] * Q[1, 1] + Q[1, 2] * Q[0, 1]) / det;
                w.y = (Q[0, 2] * Q[1, 0] - Q[1, 2] * Q[0, 0]) / det;
                break;
            }

            /* matrix is singular - lines are parallel. Add another,
           orthogonal axis, through the center of the unit square */
            if (Q[0, 0] > Q[1, 1])
            {
                v[0] = -Q[0, 1];
                v[1] = Q[0, 0];
            }
            else if (Q[1, 1] != 0) // nur if (Q[1,1])
            {
                v[0] = -Q[1, 1];
                v[1] = Q[1, 0];
            }
            else
            {
                v[0] = 1;
                v[1] = 0;
            }
            d = v[0] * v[0] + v[1] * v[1];
            v[2] = -v[1] * s.y - v[0] * s.x;
            for (l = 0; l < 3; l++)
            {
                for (k = 0; k < 3; k++)
                {
                    Q[l, k] += v[l] * v[k] / d;
                }
            }
        }
        dx = fabs(w.x - s.x);
        dy = fabs(w.y - s.y);
        if (dx <= .5 && dy <= .5)
        {
            // - 1 because we have a additional border set to the bitmap
            pp->Curves.vertex[i].x = w.x + x0;
            pp->Curves.vertex[i].y = w.y + y0;


            continue;
        }

        /* the minimum was not in the unit square; now minimize quadratic
           on boundary of square */
        MIN = quadform(Q, s);
        xmin = s.x;
        ymin = s.y;

        if (Q[0, 0] == 0.0)
        {
            goto fixx;
        }
        for (z = 0; z < 2; z++)
        {   /* value of the y-coordinate */
            w.y = s.y - 0.5 + z;
            w.x = -(Q[0, 1] * w.y + Q[0, 2]) / Q[0, 0];
            dx = fabs(w.x - s.x);
            cand = quadform(Q, w);
            if (dx <= .5 && cand < MIN)
            {
                MIN = cand;
                xmin = w.x;
                ymin = w.y;
            }
        }
    fixx:
        if (Q[1, 1] == 0.0)
        {
            goto corners;
        }
        for (z = 0; z < 2; z++)
        {   /* value of the x-coordinate */
            w.x = s.x - 0.5 + z;
            w.y = -(Q[1, 0] * w.x + Q[1, 2]) / Q[1, 1];
            dy = fabs(w.y - s.y);
            cand = quadform(Q, w);
            if (dy <= .5 && cand < MIN)
            {
                MIN = cand;
                xmin = w.x;
                ymin = w.y;
            }
        }
    corners:
        /* check four corners */
        for (l = 0; l < 2; l++)
        {
            for (k = 0; k < 2; k++)
            {
                w.x = s.x - 0.5 + l;
                w.y = s.y - 0.5 + k;
                cand = quadform(Q, w);
                if (cand < MIN)
                {
                    MIN = cand;
                    xmin = w.x;
                    ymin = w.y;
                }
            }
        }
        // - 1 because we have a additional border set to the bitmap
        pp->Curves.vertex[i].x = xmin + x0 - 1;
        pp->Curves.vertex[i].y = ymin + y0 - 1;
        continue;
    }
}

/* ---------------------------------------------------------------------- */
/* Stage 4: smoothing and corner analysis (Sec. 2.3.3) */

/* Always succeeds and returns 0 */
static void smooth(privcurve curve, int SIGN, double alphamax)
{
    int m = curve.n;

    int i, j, k;
    double dd, denom, alpha;
    DPOINT p2, p3, p4;

    if (SIGN == '-')
    {
        /* reverse orientation of negative paths */
        for (i = 0, j = m - 1; i < j; i++, j--)
        {
            DPOINT tmp;
            tmp = curve.vertex[i];
            curve.vertex[i] = curve.vertex[j];
            curve.vertex[j] = tmp;
        }
    }

    /* examine each vertex and find its best fit */
    for (i = 0; i < m; i++)
    {
        j = mod(i + 1, m);
        k = mod(i + 2, m);
        p4 = interval(1 / 2.0, curve.vertex[k], curve.vertex[j]);

        denom = ddenom(curve.vertex[i], curve.vertex[k]);
        if (denom != 0.0)
        {
            dd = dpara(curve.vertex[i], curve.vertex[j], curve.vertex[k]) / denom;
            dd = fabs(dd);
            alpha = dd > 1 ? (1 - 1.0 / dd) : 0;
            alpha = alpha / 0.75;
        }
        else
        {
            alpha = 4 / 3.0;
        }
        curve.alpha0[j] = alpha;	 /* remember "original" value of alpha */
      
        if (alpha > alphamax)
        {  /* pointed corner */
            curve.tag[j] = POTRACE_CORNER;
            //curve.c[j][1] = curve->vertex[j];
            curve.ControlPoints[j, 1] = curve.vertex[j];
            curve.ControlPoints[j, 2] = p4;
        }
        else
        {
            if (alpha < 0.55)
            {
                alpha = 0.55;
            }
            else if (alpha > 1)
            {
                alpha = 1;
            }
            p2 = interval(.5 + .5 * alpha, curve.vertex[i], curve.vertex[j]);
            p3 = interval(.5 + .5 * alpha, curve.vertex[k], curve.vertex[j]);
            curve.tag[j] = POTRACE_CURVETO;
            curve.ControlPoints[j, 0] = p2;
            curve.ControlPoints[j, 1] = p3;
            curve.ControlPoints[j, 2] = p4;
        }
        curve.alpha[j] = alpha;	/* store the "cropped" value of alpha */
        curve.beta[j] = 0.5;
    }


}
/* ---------------------------------------------------------------------- */
/* Stage 5: DCURVE optimization (Sec. 2.4) */

/* a type for the result of opti_penalty */
struct opti
{
    double pen;	   /* penalty */
    DPOINT[] c;   /* curve parameters */
    double t, s;	   /* curve parameters */
    double alpha;	   /* curve parameter */
};

/* calculate best fit from i+.5 to j+.5.  Assume i<j (cyclically).
   Return 0 and set badness and parameters (alpha, beta), if
   possible. Return 1 if impossible. */
static bool opti_penalty(const Path* pp, int i, int j, ref opti res, double opttolerance, int* convc, double[] areac)
{
    int m = pp->Curves.n;
    int k, k1, k2, conv, i1;
    double area, alpha, d, d1, d2;
    DPOINT p0, p1, p2, p3, pt;
    double A, R, A1, A2, A3, A4;
    double s, t;

    /* check convexity, corner-freeness, and maximum bend < 179 degrees */

    if (i == j)
    {  /* sanity - a full loop can never be an opticurve */
        return true;
    }

    k = i;
    i1 = mod(i + 1, m);
    k1 = mod(k + 1, m);
    conv = convc[k1];
    if (conv == 0)
    {
        return true;
    }
    d = ddist(pp->Curves.vertex[i], pp->Curves.vertex[i1]);
    for (k = k1; k != j; k = k1)
    {
        k1 = mod(k + 1, m);
        k2 = mod(k + 2, m);
        if (convc[k1] != conv)
        {
            return true;
        }
        if (SIGN(cprod(pp->Curves.vertex[i], pp->Curves.vertex[i1], pp->Curves.vertex[k1], pp->Curves.vertex[k2])) != conv)
        {
            return true;
        }
        if (iprod1(pp->Curves.vertex[i], pp->Curves.vertex[i1], pp->Curves.vertex[k1], pp->Curves.vertex[k2]) < d * ddist(pp->Curves.vertex[k1], pp->Curves.vertex[k2]) * COS179)
        {
            return true;
        }
    }

    /* the curve we're working in: */
    p0 = pp->Curves.ControlPoints[mod(i, m), 2];
    p1 = pp->Curves.vertex[mod(i + 1, m)];
    p2 = pp->Curves.vertex[mod(j, m)];
    p3 = pp->Curves.ControlPoints[mod(j, m), 2];

    /* determine its area */
    area = areac[j] - areac[i];
    area -= dpara(pp->Curves.vertex[0], pp->Curves.ControlPoints[i, 2], pp->Curves.ControlPoints[j, 2]) / 2;
    if (i >= j)
    {
        area += areac[m];
    }

    /* find intersection o of p0p1 and p2p3. Let t,s such that o =
       interval(t,p0,p1) = interval(s,p3,p2). Let A be the area of the
       triangle (p0,o,p3). */

    A1 = dpara(p0, p1, p2);
    A2 = dpara(p0, p1, p3);
    A3 = dpara(p0, p2, p3);
    /* A4 = dpara(p1, p2, p3); */
    A4 = A1 + A3 - A2;

    if (A2 == A1)
    {  /* this should never happen */
        return true;
    }

    t = A3 / (A3 - A4);
    s = A2 / (A2 - A1);
    A = A2 * t / 2.0;

    if (A == 0.0)
    {  /* this should never happen */
        return true;
    }

    R = area / A;	 /* relative area */
    alpha = 2 - sqrt(4 - R / 0.3);  /* overall alpha for p0-o-p3 curve */
    res.c = new DPOINT[2];
    res.c[0] = interval(t * alpha, p0, p1);
    res.c[1] = interval(s * alpha, p3, p2);
    res.alpha = alpha;
    res.t = t;
    res.s = s;

    p1 = res.c[0];
    p2 = res.c[1];  /* the proposed curve is now (p0,p1,p2,p3) */

    res.pen = 0;

    /* calculate penalty */
    /* check tangency with edges */
    for (k = mod(i + 1, m); k != j; k = k1)
    {
        k1 = mod(k + 1, m);
        t = tangent(p0, p1, p2, p3, pp->Curves.vertex[k], pp->Curves.vertex[k1]);
        if (t < -.5)
        {
            return true;
        }
        pt = bezier(t, p0, p1, p2, p3);
        d = ddist(pp->Curves.vertex[k], pp->Curves.vertex[k1]);
        if (d == 0.0)
        {  /* this should never happen */

            return true;
        }
        d1 = dpara(pp->Curves.vertex[k], pp->Curves.vertex[k1], pt) / d;
        if (fabs(d1) > opttolerance)
        {
            return true;
        }
        if (iprod(pp->Curves.vertex[k], pp->Curves.vertex[k1], pt) < 0 || iprod(pp->Curves.vertex[k1], pp->Curves.vertex[k], pt) < 0)
        {
            return true;
        }
        res.pen += d1 * d1;
    }

    /* check corners */
    for (k = i; k != j; k = k1)
    {
        k1 = mod(k + 1, m);
        t = tangent(p0, p1, p2, p3, pp->Curves.ControlPoints[k, 2], pp->Curves.ControlPoints[k1, 2]);
        if (t < -.5)
        {
            return true;
        }
        pt = bezier(t, p0, p1, p2, p3);
        d = ddist(pp->Curves.ControlPoints[k, 2], pp->Curves.ControlPoints[k1, 2]);
        if (d == 0.0)
        {  /* this should never happen */
            return true;
        }
        d1 = dpara(pp->Curves.ControlPoints[k, 2], pp->Curves.ControlPoints[k1, 2], pt) / d;
        d2 = dpara(pp->Curves.ControlPoints[k, 2], pp->Curves.ControlPoints[k1, 2], pp->Curves.vertex[k1]) / d;
        d2 *= 0.75 * pp->Curves.alpha[k1];
        if (d2 < 0)
        {
            d1 = -d1;
            d2 = -d2;
        }
        if (d1 < d2 - opttolerance)
        {
            return true;
        }
        if (d1 < d2)
        {
            res.pen += (d1 - d2) * (d1 - d2);
        }
    }

    return false;
}

void i() { }

/* optimize the path p, replacing sequences of Bezier segments by a
single segment when possible. Return 0 on success, 1 with errno set
on failure. */
static void opticurve(const Path* pp, double opttolerance)
{
    int m = pp->Curves.n;
    int* pt = new int[m + 1];     /* pt[m+1] */
    double[] pen = new double[m + 1];  /* pen[m+1] */
    int* len = new int[m + 1];     /* len[m+1] */
    opti* opt = new opti[m + 1];    /* opt[m+1] */
    int* convc = new int[m];       /* conv[m]: pre-computed convexities */
    double* areac = new double[m + 1];  /* cumarea[m+1]: cache for fast area computation */


    int om;
    int i, j;
    bool r;
    opti o = new opti();
    DPOINT p0;
    int i1;
    double area;
    double alpha;
    double[] s;
    double[] t;




    /* pre-calculate convexity: +1 = right turn, -1 = left turn, 0 = corner */
    for (i = 0; i < m; i++)
    {
        if (pp->Curves.tag[i] == POTRACE_CURVETO)
        {
            convc[i] = SIGN(dpara(pp->Curves.vertex[mod(i - 1, m)], pp->Curves.vertex[i], pp->Curves.vertex[mod(i + 1, m)]));
        }
        else
        {
            convc[i] = 0;
        }
    }

    /* pre-calculate areas */
    area = 0.0;
    areac[0] = 0.0;
    p0 = pp->Curves.vertex[0];
    for (i = 0; i < m; i++)
    {
        i1 = mod(i + 1, m);
        if (pp->Curves.tag[i1] == POTRACE_CURVETO)
        {
            alpha = pp->Curves.alpha[i1];
            area += 0.3 * alpha * (4 - alpha) * dpara(pp->Curves.ControlPoints[i, 2], pp->Curves.vertex[i1], pp->Curves.ControlPoints[i1, 2]) / 2;
            area += dpara(p0, pp->Curves.ControlPoints[i, 2], pp->Curves.ControlPoints[i1, 2]) / 2;
        }
        areac[i + 1] = area;
    }

    pt[0] = -1;
    pen[0] = 0;
    len[0] = 0;

    /* Fixme: we always start from a fixed point -- should find the best
       curve cyclically ### */

    for (j = 1; j <= m; j++)
    {
        /* calculate best path from 0 to j */
        pt[j] = j - 1;
        pen[j] = pen[j - 1];
        len[j] = len[j - 1] + 1;

        for (i = j - 2; i >= 0; i--)
        {
            r = opti_penalty(pp, i, mod(j, m), ref o, opttolerance, convc, areac);
            if (r)
            {
                break;
            }
            if (len[j] > len[i] + 1 || (len[j] == len[i] + 1 && pen[j] > pen[i] + o.pen))
            {
                pt[j] = i;
                pen[j] = pen[i] + o.pen;
                len[j] = len[i] + 1;
                opt[j] = o;
            }
        }
    }
    om = len[m];
    pp->OptimizedCurves = new privcurve(om);

    s = new double[om];
    t = new double[om];


    j = m;
    for (i = om - 1; i >= 0; i--)
    {
        if (pt[j] == j - 1)
        {
            pp->OptimizedCurves.tag[i] = pp->Curves.tag[mod(j, m)];
            pp->OptimizedCurves.ControlPoints[i, 0] = pp->Curves.ControlPoints[mod(j, m), 0];
            pp->OptimizedCurves.ControlPoints[i, 1] = pp->Curves.ControlPoints[mod(j, m), 1];
            pp->OptimizedCurves.ControlPoints[i, 2] = pp->Curves.ControlPoints[mod(j, m), 2];
            pp->OptimizedCurves.vertex[i] = pp->Curves.vertex[mod(j, m)];
            pp->OptimizedCurves.alpha[i] = pp->Curves.alpha[mod(j, m)];
            pp->OptimizedCurves.alpha0[i] = pp->Curves.alpha0[mod(j, m)];
            pp->OptimizedCurves.beta[i] = pp->Curves.beta[mod(j, m)];
            s[i] = t[i] = 1.0;
        }
        else
        {
            pp->OptimizedCurves.tag[i] = POTRACE_CURVETO;
            pp->OptimizedCurves.ControlPoints[i, 0] = opt[j].c[0];
            pp->OptimizedCurves.ControlPoints[i, 1] = opt[j].c[1];
            pp->OptimizedCurves.ControlPoints[i, 2] = pp->Curves.ControlPoints[mod(j, m), 2];
            pp->OptimizedCurves.vertex[i] = interval(opt[j].s, pp->Curves.ControlPoints[mod(j, m), 2], pp->Curves.vertex[mod(j, m)]);
            pp->OptimizedCurves.alpha[i] = opt[j].alpha;
            pp->OptimizedCurves.alpha0[i] = opt[j].alpha;
            s[i] = opt[j].s;
            t[i] = opt[j].t;
        }
        j = pt[j];
    }

    /* calculate beta parameters */
    for (i = 0; i < om; i++)
    {
        i1 = mod(i + 1, om);
        pp->OptimizedCurves.beta[i] = s[i] / (s[i] + t[i1]);
    }

}
static void getContur(bool[,] bm, int x, int y, ArrayList plistp)
{
    Path Contur = findpath(bm, new dPOINT(x, y));

    Xor_Path(bm, Contur);
    ArrayList PolyPath = new ArrayList();
    // only area > turdsize is taken

    if (Contur.area > turdsize)
    {
        plistp.Add(PolyPath);
        PolyPath.Add(Contur); // Path with index 0 is a conture
    }


    while (FindNext(bm, ref x, ref y, Contur))
    {
        Path Hole = findpath(bm, new IPOINT(x, y));
        //Path Hole = findpath(bm, x, y);
        Xor_Path(bm, Hole);
        if (Hole.area > turdsize)

            PolyPath.Add(Hole); // Path with index > 0 is a hole,
        if (FindNext(bm, ref x, ref y, Hole))
            getContur(bm, x, y, plistp);

    }

}
static void potrace_trace(bool[,] bm, ArrayList ListOfCurveArrays)
{
    ArrayList plistp = new ArrayList();
    bm_to_pathlist(bm, plistp);
    process_path(plistp);
    PathList_to_ListOfCurveArrays(plistp, ListOfCurveArrays);
}
static void AddCurve(ArrayList Curves, DPOINT A, DPOINT ControlPointA, DPOINT ControlPointB, DPOINT B)
{
    //   Curves.Add(new DCURVE(CurveKind.Bezier, A, ControlPointA, ControlPointB, B));
    //   return;
    CurveKind Kind;
    if ((fabs(xprod(new DPOINT(ControlPointA.x - A.x, ControlPointA.y - A.y),
                           new DPOINT(B.x - A.x, B.y - A.y))) < 0.01) &&
                       (fabs(xprod(new DPOINT(ControlPointB.x - B.x, ControlPointB.y - B.y),
                           new DPOINT(B.x - A.x, B.y - A.y))) < 0.01))
        Kind = CurveKind.Line;
    else
        Kind = CurveKind.Bezier;
    /*    Curves.Add(new DCURVE(Kind,A,ControlPointA,ControlPointB,B));
        return;*/
    if ((Kind == CurveKind.Line))
    {
        if ((Curves_Count > 0) && (((DCURVE)Curves[Curves_Count - 1]).Kind == CurveKind.Line))
        {
            DCURVE C = (DCURVE)Curves[Curves_Count - 1];
            if ((fabs(xprod(new DPOINT(C.B.x - C.A.x, C.B.y - C.A.y), new DPOINT(B.x - A.x, B.y - A.y))) < 0.01) &&
                (iprod(C.B, C.A, B) < 0))
                Curves[Curves_Count - 1] = new DCURVE(Kind, C.A, C.A, C.A, B);
            else
                Curves.Add(new DCURVE(CurveKind.Line, A, ControlPointA, ControlPointB, B));



        }
        else
            Curves.Add(new DCURVE(CurveKind.Line, A, ControlPointA, ControlPointB, B));



    }
    else
        Curves.Add(new DCURVE(CurveKind.Bezier, A, ControlPointA, ControlPointB, B));

}
static void PathList_to_ListOfCurveArrays(ArrayList plistp, ArrayList ListOfCurveArrays)
{
    ArrayList plist;

    /* call downstream function with each path */
    for (int j = 0; j < plistp_Count; j++)
    {
        plist = (ArrayList)plistp[j];
        ArrayList clist = new ArrayList();
        ListOfCurveArrays.Add(clist);

        for (int i = 0; i < plist_Count; i++)
        {
            Path p = (Path)plist[i];
            DPOINT A = p.Curves.ControlPoints[p.Curves.n - 1, 2];
            ArrayList Curves = new ArrayList();
            for (int k = 0; k < p.Curves.n; k++)
            {
                DPOINT C = p.Curves.ControlPoints[k, 0];
                DPOINT D = p.Curves.ControlPoints[k, 1];
                DPOINT E = p.Curves.ControlPoints[k, 2];
                if (p.Curves.tag[k] == POTRACE_CORNER)
                {
                    AddCurve(Curves, A, A, D, D);
                    AddCurve(Curves, D, D, E, E);

                }
                else
                    AddCurve(Curves, A, C, D, E);
                A = E;
            }
            if (Curves_Count > 0)
            {
                DCURVE CL = (DCURVE)Curves[Curves_Count - 1];
                DCURVE CF = (DCURVE)Curves[0];
                if ((CL.Kind == CurveKind.Line) && ((CF.Kind == CurveKind.Line))
                    && (iprod(CL.B, CL.A, CF.B) < 0)
                    && (fabs(xprod(new DPOINT(CF.B.x - CF.A.x, CF.B.y - CF.A.y), new DPOINT(CL.A.x - CL.A.x, CL.B.y - CL.A.y))) < 0.01))
                {
                    Curves[0] = new DCURVE(CurveKind.Line, CL.A, CL.A, CL.A, CF.B);
                    Curves.RemoveAt(Curves_Count - 1);
                }


                DCURVE[] CList = new DCURVE[Curves_Count];
                for (int ci = 0; ci < Curves_Count; ci++) CList[ci] = (DCURVE)Curves[ci];
                clist.Add(CList);
            }
        }
        //---- Check Last with first



    }

}

/// <summary>
/// Decompose the given bitmap into paths. Returns a linked list of
/// Path objects with the fields len, pt, area filled
/// </summary>
/// <param name="bm">A binary bitmap which holds the imageinformations.</param>
/// <param name="plistp">List of Path objects</param>
static void bm_to_pathlist(bool[,] bm, ArrayList plistp)
{
    int x = 0, y = 0;
    while (FindNext(bm, ref x, ref y))
        getContur(bm, x, y, plistp);
}

static void process_path(ArrayList plistp)
{
    Path p;
    ArrayList plist;
    /* call downstream function with each path */
    for (int j = 0; j < plistp_Count; j++)
    {
        plist = (ArrayList)plistp[j];
        for (int i = 0; i < plist_Count; i++)
        {
            {
                p = (Path)plist[i];
                calc_sums(p);
                calc_lon(p);
                bestpolygon(p);
                adjust_vertices(p);
                smooth(p.Curves, 1, alphamax);
                if (curveoptimizing)
                {
                    opticurve(p, opttolerance);
                    p.FCurves = p.OptimizedCurves;
                }
                else
                {
                    p.FCurves = p.Curves;
                }
                p.Curves = p.FCurves;
            }

        }
    }

}
