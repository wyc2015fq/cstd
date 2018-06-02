
/// <summary>
///   RANSAC Robust Homography Matrix Estimator.
/// </summary>
/// 
/// <remarks>
/// <para>
///   Fitting a homography using RANSAC is pretty straightforward. Being a iterative method,
///   in a single iteration a random sample of four correspondences is selected from the 
///   given correspondence points and a homography H is then computed from those points.</para>
/// <para>
///   The original points are then transformed using this homography and their distances to
///   where those transforms should be is then computed and matching points can classified
///   as inliers and non-matching points as outliers.</para>  
/// <para>
///   After a given number of iterations, the iteration which produced the largest number
///   of inliers is then selected as the best estimation for H.</para>  
///
/// <para>
///   References:
///   <list type="bullet">
///     <item><description>
///       http://www.cs.ubc.ca/grads/resources/thesis/May09/Dubrofsky_Elan.pdf </description></item>
///     <item><description>
///       http://www.cc.gatech.edu/classes/AY2005/cs4495_fall/assignment4.pdf </description></item>
///   </list></para>
/// </remarks>
///

/// <summary>
///   Estimates a homography with the given points.
/// </summary>
int homography(int n, const int* points, const DPOINT* pointSet1, const DPOINT* pointSet2, double* T)
{
  int i, t;
  // Retrieve the original points
  DPOINT* x1 = MALLOC(DPOINT, n);
  DPOINT* x2 = MALLOC(DPOINT, n);
  for (i=0; i<n; ++i) {
    t = points[i];
    x1[i] = pointSet1[t];
    x2[i] = pointSet2[t];
  }
  
  // Compute the homography
  Homography(n, x1, n, x2, T);
  FREE(x1);
  FREE(x2);
  return 0;
}


/// <summary>
///   Compute inliers using the Symmetric Transfer Error,
/// </summary>
int Homography_distance(const double* H, double t, int npt1, const DPOINT* pointSet1, const DPOINT* pointSet2, int* index)
{
    int i, j=0, n = npt1;

    // Compute the projections (both directions)
    DPOINT* p1=0, *p2=0;
    double invH[8]={0};
    double* d2=0;
    MatrixH_Inverse(H, invH);
    p1 = MALLOC(DPOINT, n);
    p2 = MALLOC(DPOINT, n);
    MatrixH_TransformPoints(H, pointSet1, p1, n);
    MatrixH_TransformPoints(invH, pointSet2, p2, n);

    // Compute the distances
    d2 = MALLOC(double, n);
    for (i = 0; i < n; i++)
    {
        // Compute the distance as
        double ax = pointSet1[i].x - p2[i].x;
        double ay = pointSet1[i].y - p2[i].y;
        double bx = pointSet2[i].x - p1[i].x;
        double by = pointSet2[i].y - p1[i].y;
        double d2 = (ax * ax) + (ay * ay) + (bx * bx) + (by * by);
        if (d2<t) {
            index[j++] = i;
        }
    }

    FREE(p1);
    FREE(p2);
    // Find and return the inliers
    return j;
}

/// <summary>
///   Checks if the selected points will result in a degenerate homography.
/// </summary>
bool Homography_degenerate(const int* points, int n, const DPOINT* pointSet1, const DPOINT* pointSet2)
{
    int i;
    DPOINT x1[4], x2[4];
    ASSERT(n>3);
    for (i=0; i<4; ++i) {
      x1[i] = pointSet1[points[i]];
      x2[i] = pointSet2[points[i]];
    }

    // If any three of the four points in each set is colinear,
    //  the resulting homography matrix will be degenerate.

    return Colinear(x1[0], x1[1], x1[2]) ||
           Colinear(x1[0], x1[1], x1[3]) ||
           Colinear(x1[0], x1[2], x1[3]) ||
           Colinear(x1[1], x1[2], x1[3]) ||

           Colinear(x2[0], x2[1], x2[2]) ||
           Colinear(x2[0], x2[1], x2[3]) ||
           Colinear(x2[0], x2[2], x2[3]) ||
           Colinear(x2[1], x2[2], x2[3]);
}


/// <summary>
///   Creates a new RANSAC homography estimator.
/// </summary>
/// <param name="threshold">Inlier threshold.</param>
/// <param name="probability">Inlier probability.</param>

/// <summary>
///   Matches two sets of points using RANSAC.
/// </summary>
/// <returns>The homography matrix matching x1 and x2.</returns>
int RansacHomographyEstimator(int npt1, const DPOINT* points1, int npt2, const DPOINT* points2, double* H, double threshold, double probability)
{
  // Create a new RANSAC with the selected threshold
  //ransac = new RANSAC<MatrixH>(4, threshold, probability);
  
  // Set RANSAC functions
  //ransac.Fitting = homography;
  //ransac.Degenerate = degenerate;
  //ransac.Distances = distance;
  
  // RANSAC<MatrixH> ransac;
  // int[] inliers;
  
  DPOINT* pointSet1=0;
  DPOINT* pointSet2=0;
    double T1[8], T2[8];

    // Initial argument checkings
    if (npt1 != npt2) {
        ArgumentException("The number of points should be equal.");
    }

    if (npt1 < 4) {
        ArgumentException("At least four points are required to fit an homography");
    }

    pointSet1 = MALLOC(DPOINT, npt1);
    pointSet2 = MALLOC(DPOINT, npt2);
    // Normalize each set of points so that the origin is
    //  at centroid and mean distance from origin is sqrt(2).
    Normalize(npt1, points1, T1, pointSet1);
    Normalize(npt2, points2, T2, pointSet2);

    // Compute RANSAC and find the inlier points
    //H = ransac.Compute(npt1, out inliers);

    if (inliers == null || inliers.Length < 4) {
        //throw new Exception("RANSAC could not find enough points to fit an homography.");
        return null;
    }


    // Compute the final homography considering all inliers
    H = homography(inliers);

    // Denormalise
    H = T2.Inverse() * (H * T1);
    
    FREE(pointSet1);
    FREE(pointSet2);
    return H;
}

