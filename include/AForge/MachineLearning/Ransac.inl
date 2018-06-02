
/// <summary>
///   Multipurpose RANSAC algorithm.
/// </summary>
/// <typeparam name="TModel">The model type to be trained by RANSAC.</typeparam>
/// <remarks>
///   RANSAC is an abbreviation for "RANdom SAmple Consensus". It is an iterative
///   method to estimate parameters of a mathematical model from a set of observed
///   data which contains outliers. It is a non-deterministic algorithm in the sense
///   that it produces a reasonable result only with a certain probability, with this
///   probability increasing as more iterations are allowed.
/// </remarks>
/// 

/// fitting     Model fitting function.
/// distances   Distance function.
/// degenerate  Degenerative set detection function.

/// <summary>
/// Threshold
///   Gets or sets the minimum distance between a data point and
///   the model used to decide whether the point is an inlier or not.
/// </summary>

/// <summary>
///  MaxSamplings  Maximum number of attempts to select a non-degenerate data set.
///   The default value is 100.
/// </summary>


/// <summary>
///  Samples  Gets or sets the minimum number of samples from the data
///   required by the fitting function to fit a model.
/// </summary>

/// <summary>
///  MaxEvaluations Maximum number of iterations. The default value is 1000.
/// </summary>

/// <summary>
///  Probability  Gets or sets the probability of obtaining a random
///   sample of the input points that contains no outliers.
/// </summary>

/// <summary>
///   Constructs a new RANSAC algorithm.
/// </summary>
/// <param name="minSamples">
///   The minimum number of samples from the data
///   required by the fitting function to fit a model.
/// </param>
/// <param name="threshold">
///   The minimum distance between a data point and
///   the model used to decide whether the point is
///   an inlier or not.
/// </param>
/// <param name="probability">
///   The probability of obtaining a random sample of
///   the input points that contains no outliers.
/// </param>
/// <summary>
///   Computes the model using the RANSAC algorithm.
/// </summary>
/// <param name="size">The total number of points in the data set.</param>

typedef int (fitting_t)(void* userdata, int size, int* inliers, void* model);
typedef int (distances_t)(void* userdata, void* model, double t, int* inliers, int maxSamples );
typedef bool (degenerate_t)(void* userdata, int size, int* inliers);

// minSamples   number of samples
// threshold    inlier threshold
int RANSAC(void* userdata, void* bestModel, void* model, int modelsize, int* bestInliers, int size, int minSamples, int maxSamples,
           fitting_t fitting, distances_t distances, degenerate_t degenerate, double threshold, double probability, int maxSamplings, int maxEvaluations)
{
    //int maxSamplings = 100;
    //int maxEvaluations = 1000;
    // double probability = 0.99;
    // Ransac parameters
    int maxInliers, inliers_Length, count;
    double N;


    if (minSamples < 0) ArgumentOutOfRangeException("minSamples");
    if (threshold < 0) ArgumentOutOfRangeException("threshold");
    if (probability > 1.0 || probability < 0.0) ArgumentException("Probability should be a value between 0 and 1");
    
    /// <summary>
    ///   Computes the model using the RANSAC algorithm.
    /// </summary>
    /// <param name="size">The total number of points in the data set.</param>
    /// <param name="inliers">The indexes of the outlier points in the data set.</param>
    
    // We are going to find the best model (which fits
    //  the maximum number of inlier points as possible).
    maxInliers = 0;
    inliers_Length =0;
    
    // For this we are going to search for random samples
    //  of the original points which contains no outliers.
    
    count = 0;  // Total number of trials performed
    N = maxEvaluations;   // Estimative of number of trials needed.
    
    // While the number of trials is less than our estimative,
    //   and we have not surpassed the maximum number of trials
    while (count < N && count < maxEvaluations) {
      int* sample = NULL;
      int samplings = 0;
      
      // While the number of samples attempted is less
      //   than the maximum limit of attempts
      while (samplings < maxSamplings)
      {
        // Select at random s datapoints to form a trial model.
        random_permutation2(size, minSamples, sample);
        
        // If the sampled points are not in a degenerate configuration,
        if (!degenerate(userdata, minSamples, sample)) 
        {
          // Fit model using the random selection of points
          fitting(userdata, minSamples, sample, model);
          break; // Exit the while loop.
        }
        
        samplings++; // Increase the samplings counter
      }
      
      // Now, evaluate the distances between total points and the model returning the
      //  indices of the points that are inliers (according to a distance threshold t).
      inliers_Length = distances(userdata, model, threshold, inliers, maxSamples);
      
      // Check if the model was the model which highest number of inliers:
      if (inliers_Length > maxInliers)
      {
        double pInlier, pNoOutliers;
        // Yes, this model has the highest number of inliers.
        
        maxInliers = inliers_Length;  // Set the new maximum,
        memcpy(bestModel, model, modelsize);            // This is the best model found so far,
        MEMCPY(bestInliers, inliers, inliers_Length);        // Store the indices of the current inliers.
        
        // Update estimate of N, the number of trials to ensure we pick, 
        //   with probability p, a data set with no outliers.
        pInlier = (double)inliers_Length / (double)size;
        pNoOutliers = 1.0 - pow(pInlier, minSamples);
        
        N = log(1.0 - probability) / log(pNoOutliers);
      }
      
      count++; // Increase the trial counter.
    }
    
    inliers = bestInliers;
    return inliers_Length;
}

