
/// <summary>
///   Maximum cross-correlation feature point matching algorithm.
/// </summary>
/// <remarks>
///   <para>
///     This class matches feature points by using a maximum cross-correlation measure.</para>
///   <para>
///     References:
///     <list type="bullet">
///       <item><description>
///         P. D. Kovesi. MATLAB and Octave Functions for Computer Vision and Image Processing.
///         School of Computer Science and Software Engineering, The University of Western Australia.
///         Available in: <a href="http://www.csse.uwa.edu.au/~pk/Research/MatlabFns/Match/matchbycorrelation.m">
///         http://www.csse.uwa.edu.au/~pk/Research/MatlabFns/Match/matchbycorrelation.m</a>
///       </description></item>
///       <item><description>
///         <a href="http://www.instructor.com.br/unesp2006/premiados/PauloHenrique.pdf">
///         http://www.instructor.com.br/unesp2006/premiados/PauloHenrique.pdf</a>
///       </description></item>
///       <item><description>
///         <a href="http://siddhantahuja.wordpress.com/2010/04/11/correlation-based-similarity-measures-summary/">
///         http://siddhantahuja.wordpress.com/2010/04/11/correlation-based-similarity-measures-summary/</a>
///       </description></item>
///   </list></para>
/// </remarks>
///
/// <seealso cref="RansacHomographyEstimator"/>
///


/// <summary>
///   double maxDistance      Gets or sets the maximum distance to consider points as correlated.
///   int windowSize   Gets or sets the size of the correlation window.
/// </summary>

/// <summary>
///   Matches two sets of feature points computed from the given images.
/// </summary>
int CorrelationMatching(const img_t* im1, const img_t* im2, int npt1, const IPOINT* points1, int npt2, const IPOINT* points2, int windowSize, double maxDistance, IRANGE* ra, int maxralen)
{
    int ralen=0;
    int i, j, i1, i2, ml;
    img_t grayImage1[1]={0}, grayImage2[1]={0};
    double* correlationMatrix=0;

    if (windowSize % 2 == 0) {
        ArgumentException("Window size should be odd");
    }


    // Make sure we are dealing with grayscale images.
    if (im1->t == PixelFormat_8bppIndexed)
    {
        Image_Clone(im1, grayImage1);
    }
    else
    {
        // create temporary grayscale image
        Grayscale(im1, grayImage1, GRY_BT709);
    }

    if (im2->t == PixelFormat_8bppIndexed)
    {
        Image_Clone(im2, grayImage2);
    }
    else
    {
        // create temporary grayscale image
        Grayscale(im2, grayImage2, GRY_BT709);
    }


    // Generate correlation matrix
    //correlationMatrix = computeCorrelationMatrix(grayImage1, points1, grayImage2, points2, windowSize, maxDistance);

    /// <summary>
    ///   Constructs the correlation matrix between selected points from two images.
    /// </summary>
    /// <remarks>
    ///   Rows correspond to points from the first image, columns correspond to points
    ///   in the second.
    /// </remarks>
    //static int computeCorrelationMatrix( Bitmap im1, IPOINT[] points1, Bitmap im2, IPOINT[] points2, int windowSize, double maxDistance, double* matrix)
    // Create the initial correlation matrix
    
    correlationMatrix = MALLOC(double, npt1*npt2);
    correlationMatrix[0] = NegativeInfinity();
    FILL(npt1*npt2, correlationMatrix, -1);
    ml = npt2;
    {
      // Gather some information
      int r = (windowSize - 1) / 2; //  'radius' of correlation window
      double m = maxDistance * maxDistance; // maximum considered distance
      double* w1 = MALLOC(double, windowSize*windowSize); // first window
      double* w2 = MALLOC(double, windowSize*windowSize); // second window
      int wl = windowSize;
      
      // For each index in the first set of points
      for(i1=0; i1<npt1; ++i1)
      {
        // Get the current point
        IPOINT p1 = points1[i1];
        // We will ignore points at the edge
        if (!(p1.x >= r && p1.x < im1->w - r && p1.y >= r && p1.y < im1->h - r)) {
          continue;
        }
        
        // Create the first window for the current point
        {
          double sum=0;
          const byte* src = im1->tt.data + (p1.x - r) + (p1.y - r) * im1->s;
          
          for (j = 0; j < windowSize; j++)
          {
            for (i = 0; i < windowSize; i++) {
              w1[i*wl+j] = (*(src + i));
              sum += w1[i*wl+j] * w1[i*wl+j];
            }
            src += im1->s;
          }
          
          // Normalize the window
          sum = sqrt(sum);
          for (i = 0; i < windowSize; i++) {
            for (j = 0; j < windowSize; j++) {
              w1[i*wl+j] /= sum;
            }
          }
        }
        for(i2=0; i2<npt2; ++i2) {
          IPOINT p2 = points2[i2];
          int dx, dy;
          int dis;
          if (!(p2.x >= r && p2.x < im2->w - r && p2.y >= r && p2.y < im2->h - r)) {
            continue;
          }
          
          
          // We should consider points that are within
          //  distance maxDistance apart
          
          // Compute distances from the current point
          //  to all points in the second image.
          
          // Calculate normalized correlation measure
          dx = p1.x - p2.x, dy = p1.y - p2.y;
          dis = dx * dx + dy * dy;
          
          // Generate window in 2nd image
          if (dis<m) {
            double sum1 = 0, sum2 = 0;
            const byte* src = im2->tt.data + (p2.x - r) + (p2.y - r) * im2->s;
            
            for (j = 0; j < windowSize; j++) {
              for (i = 0; i < windowSize; i++) {
                w2[i*wl+j] = (*(src + i));
              }
              src += im2->s;
            }
            
            for (i = 0; i < windowSize; i++) {
              for (j = 0; j < windowSize; j++) {
                sum1 += w1[i*wl+j] * w2[i*wl+j];
                sum2 += w2[i*wl+j] * w2[i*wl+j];
              }
            }
            
            correlationMatrix[i1*ml+i2] = sum1 / sqrt(sum2);
          }
        }
      }
      
      // Release the images
      FREE(w1);
      FREE(w2);
      
    }


    // Free allocated resources
    imfree(grayImage1);
    imfree(grayImage2);



    {
      // Select points with maximum correlation measures
      int* colp2forp1=0, *rowp1forp2=0;
      int rows = npt1;
      int cols = npt2;
      IDIM msz = iDIM(npt1, npt2);
      rowp1forp2 = MALLOC(int, cols);
      colp2forp1 = MALLOC(int, rows);
      dm_MaxIndex(correlationMatrix, ml, 1, colp2forp1, msz);
      dm_MaxIndex(correlationMatrix, ml, 0, rowp1forp2, msz);
      // Construct the lists of matched point indices
      
      // For each point in the first set of points,
      for (i = 0; i < rows; i++)
      {
        // Get the point j in the second set of points with which
        // this point i has a maximum correlation measure. (i->j)
        int j = colp2forp1[i];
        
        // Now, check if this point j in the second set also has
        // a maximum correlation measure with the point i. (j->i)
        if (rowp1forp2[j] == i)
        {
          // The points are consistent. Ensure they are valid.
          if (correlationMatrix[i*ml+j] > 0)
          {
            // We have a corresponding pair (i,j)
            //p1ind.Add(i); p2ind.Add(j);
            if (ralen<maxralen) {
              ra[ralen].l = i;
              ra[ralen].r = j;
              ralen++;
            }
          }
        }
      }
      
      // Extract matched points from original arrays
      
      // Create matching point pairs
      FREE(correlationMatrix);
      FREE(rowp1forp2);
      FREE(colp2forp1);
    }   
    return ralen;
}

