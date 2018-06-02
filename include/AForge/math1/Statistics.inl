

/// <summary>
///     Set of statistics functions
/// </summary>
/// 
/// <remarks>
///     This class represents collection of functions used in statistics.
///     Every Matrix function assumes data is organized in a table-like
///     model, where Columns represents variables and Rows represents a
///     observation of each variable.
/// </remarks>
/// 


/// <summary>Computes the Mean of the given values.</summary>
/// <param name="values">A double array containing the vector members.</param>
/// <returns>The mean of the given data.</returns>
static double dv_Mean(int n, const double* values)
{
    double sum = 0.0;
    int i;
    for (i = 0; i < n; i++)
    {
        sum += values[i];
    }
    return sum / n;
}

/// <summary>Computes the Weighted Mean of the given values.</summary>
/// <param name="values">A double array containing the vector members.</param>
/// <param name="weights">An unit vector containing the importance of each sample
/// in <see param="values"/>. The sum of this array elements should add up to 1.</param>
/// <returns>The mean of the given data.</returns>
static double dv_MeanWeight(int n, const double* values, const double* weights)
{
    double sum = 0.0;
    int i;
    for (i = 0; i < n; i++)
    {
        sum += values[i] * weights[i];
    }
    return sum;
}

/// <summary>Computes the Variance of the given values.</summary>
/// <param name="values">A number array containing the vector members.</param>
/// <param name="mean">The mean of the array, if already known.</param>
/// <returns>The variance of the given data.</returns>
static double dv_VarianceM(int n, const double* values, double mean)
{
    double variance = 0.0;
    double N = n;
    double x;
    int i;
    for (i = 0; i < n; i++)
    {
        x = values[i] - mean;
        variance += x * x;
    }

    // Sample variance
    return variance / (N - 1);
}

/// <summary>Computes the Variance of the given values.</summary>
/// <param name="values">A double precision number array containing the vector members.</param>
/// <returns>The variance of the given data.</returns>
static double dv_Variance(int n, const double* values)
{
    return dv_VarianceM(n, values, dv_Mean(n, values));
}

/// <summary>Computes the weighted Variance of the given values.</summary>
/// <param name="values">A number array containing the vector members.</param>
/// <param name="mean">The mean of the array, if already known.</param>
/// <param name="weights">An unit vector containing the importance of each sample
/// in <see param="values"/>. The sum of this array elements should add up to 1.</param>
/// <returns>The variance of the given data.</returns>
static double dv_VarianceWM(int n, const double* values, double mean, const double* weights)
{
    // http://en.wikipedia.org/wiki/Weighted_variance#Weighted_sample_variance
    // http://www.gnu.org/software/gsl/manual/html_node/Weighted-Samples.html

    double variance = 0.0;
    double V2 = 0.0;
    double x, w;
    int i;

    for (i = 0; i < n; i++)
    {
        x = values[i] - mean;
        variance += x * x * weights[i];

        w = weights[i];
        V2 += w * w;
    }

    return variance / (1 - V2);
}

/// <summary>Computes the Standard Deviation of the given values.</summary>
/// <param name="values">A double array containing the vector members.</param>
/// <param name="mean">The mean of the vector, if already known.</param>
/// <returns>The standard deviation of the given data.</returns>
static double dv_StandardDeviationM(int n, const double* values, double mean)
{
    return sqrt(dv_VarianceM(n, values, mean));
}

/// <summary>Computes the Standard Deviation of the given values.</summary>
/// <param name="values">A double array containing the vector members.</param>
/// <returns>The standard deviation of the given data.</returns>
static double dv_StandardDeviation(int n, const double* values)
{
    return dv_StandardDeviationM(n, values, dv_Mean(n, values));
}

/// <summary>Computes the Standard Deviation of the given values.</summary>
/// <param name="values">A double array containing the vector members.</param>
/// <param name="mean">The mean of the vector, if already known.</param>
/// <param name="weights">An unit vector containing the importance of each sample
/// in <see param="values"/>. The sum of this array elements should add up to 1.</param>
/// <returns>The standard deviation of the given data.</returns>
static double dv_StandardDeviationMW(int n, const double* values, double mean, const double* weights)
{
    return sqrt(dv_VarianceWM(n, values, mean, weights));
}


/// <summary>
///   Computes the Standard Error for a sample size, which estimates the
///   standard deviation of the sample mean based on the population mean.
/// </summary>
/// <param name="samples">The sample size.</param>
/// <param name="standardDeviation">The sample standard deviation.</param>
/// <returns>The standard error for the sample.</returns>
static double d_StandardError(int samples, double standardDeviation)
{
    return standardDeviation / sqrt(samples);
}

/// <summary>
///   Computes the Standard Error for a sample size, which estimates the
///   standard deviation of the sample mean based on the population mean.
/// </summary>
/// <param name="values">A double array containing the samples.</param>
/// <returns>The standard error for the sample.</returns>
static double dv_StandardError(int n, const double* values)
{
    return d_StandardError(n, dv_StandardDeviation(n, values));
}


/// <summary>Computes the Median of the given values.</summary>
/// <param name="values">An integer array containing the vector members.</param>
/// <param name="alreadySorted">A boolean parameter informing if the given values have already been sorted.</param>
/// <returns>The median of the given data.</returns>
static double dv_Median(int n, const double* values, bool alreadySorted)
{
    int i;
    double ret=0;
    int N = n;
    int* index = MALLOC(int, n);
    // Creates a copy of the given values,
    for (i=0; i<n; ++i) {
      index[i] = i;
    }

    // So we can sort it without modifying the original array.
#define INDEXLESS(a, b)   (values[a]<values[b])
    QSORT(index, index+n, INDEXLESS, int);
#undef INDEXLESS


    if (N % 2 == 0)
        ret = (values[index[N / 2]] + values[index[(N / 2) - 1]]) * 0.5; // N is even 
    else ret =  values[index[N / 2]];                            // N is odd
    FREE(index);
    return ret;
}

#define INDEXOF(N, A, V, I)   {int _I=0; for (; _I<(N) && (A)[_I]!=(V); ++_I); I=_I<(N)?_I:-1; }

//出现次数最多的
/// <summary>Computes the Mode of the given values.</summary>
/// <param name="values">A number array containing the vector values.</param>
/// <returns>The variance of the given data.</returns>
static double dv_Mode(int n, const double* values)
{
    int* itemCount = MALLOC(int, n);
    double* itemArray = MALLOC(double, n);
    double ret=0;
    int i, count = 0, index = 0;
    int maxValue = 0;
    int maxIndex = 0;
    ASSERT(n>0);

    FILL(n, itemCount, 0);

    for (i = 0; i < n; i++)
    {
        INDEXOF(count, itemArray, values[i], index);

        if (index >= 0)
        {
            itemCount[index]++;
        }
        else
        {
            itemArray[count] = values[i];
            itemCount[count] = 1;
            count++;
        }
    }

    for (i = 0; i < count; i++)
    {
        if (itemCount[i] > maxValue)
        {
            maxValue = itemCount[i];
            maxIndex = i;
        }
    }

    ret = itemArray[maxIndex];
    FREE(itemArray);
    FREE(itemCount);
    return ret;
}


/// <summary>Calculates the matrix Mean vector.</summary>
/// <param name="matrix">A matrix whose means will be calculated.</param>
/// <param name="dimension">
///   The dimension along which the means will be calculated. Pass
///   0 to compute a row vector containing the mean of each column,
///   or 1 to compute a column vector containing the mean of each row.
///   Default value is 0.
/// </param>
/// <returns>Returns a vector containing the means of the given matrix.</returns>
/// <example>
///   <code>
///   double[,] matrix = 
///   {
///      { 2, -1.0, 5 },
///      { 7,  0.5, 9 },
///   };
///   
///   // column means are equal to (4.5, -0.25, 7.0)
///   double[] colMeans = Accord.Statistics.Tools.Mean(matrix, 0);
///     
///   // row means are equal to (2.0, 5.5)
///   double[] rowMeans = Accord.Statistics.Tools.Mean(matrix, 1);
///   </code>
/// </example>
static int dm_Mean(const double* matrix, int ml, int dimension, double* mean, IDIM msz)
{
    int i, j;
    int rows = msz.len[0];
    int cols = msz.len[1];
    if (dimension == 0)
    {
      //double[] mean = new double[cols];
      
      for (j = 0; j < cols; j++) {
        mean[j] = 0;
      }
      // for each row
      for (i = 0; i < rows; i++) {
        // for each column
        for (j = 0; j < cols; j++) {
          mean[j] += matrix[i*ml+j];
        }
      }
      for (j = 0; j < cols; j++) {
        mean[j] /= rows;
      }

    }
    else if (dimension == 1)
    {
      //double[] mean = new double[rows];
      
      // for each row
      for (i = 0; i < rows; i++) {
        double s = 0;
        // for each column
        for (j = 0; j < cols; j++) {
          s += matrix[i*ml+j];
        }
        mean[i] = s / cols;
      }
      
    }
    else
    {
        ArgumentException("Invalid dimension.");
    }
    return 0;
}

/// <summary>Calculates the matrix Mean vector.</summary>
/// <param name="matrix">A matrix whose means will be calculated.</param>
/// <returns>Returns a row vector containing the column means of the given matrix.</returns>
/// <example>
///   <code>
///   double[,] matrix = 
///   {
///      { 2, -1.0, 5 },
///      { 7,  0.5, 9 },
///   };
///    
///   // column means are equal to (4.5, -0.25, 7.0)
///   double[] means = Accord.Statistics.Tools.Mean(matrix);
///   </code>
/// </example>
static int dm_Mean1(const double* matrix, int ml, double* mean, IDIM msz)
{
    return dm_Mean(matrix, ml, 0, mean, msz);
}

/// <summary>Calculates the weighted matrix Mean vector.</summary>
/// <param name="matrix">A matrix whose means will be calculated.</param>
/// <param name="weights">A vector containing the importance of each sample in the matrix.</param>
/// <param name="dimension">
///   The dimension along which the means will be calculated. Pass
///   0 to compute a row vector containing the mean of each column,
///   or 1 to compute a column vector containing the mean of each row.
///   Default value is 0.
/// </param>
/// <returns>Returns a vector containing the means of the given matrix.</returns>
/// 
static int dm_MeanW(const double* matrix, int ml, const double* weights, int dimension, double* mean, IDIM msz)
{
    int i, j;
    int rows = msz.len[0];
    int cols = msz.len[1];

    if (dimension == 0)
    {
        //double[] mean = new double[cols];
      
      for (j = 0; j < cols; j++) {
        mean[j] = 0;
      }
      // for each row
      for (i = 0; i < rows; i++) {
        // for each column
        for (j = 0; j < cols; j++) {
          mean[j] += matrix[i*ml+j] * weights[j];
        }
      }

    }
    else if (dimension == 1)
    {
        //double[] mean = new double[rows];
      
      // for each row
      for (i = 0; i < rows; i++) {
        double s = 0, w = weights[i];
        // for each column
        for (j = 0; j < cols; j++) {
          mean[i] += matrix[i*ml+j] * w;
        }
        mean[i] = s / cols;
      }
      
    }
    else
    {
        ArgumentException("Invalid dimension.");
    }
    return 0;
}

/// <summary>Calculates the weighted matrix Mean vector.</summary>
/// <param name="matrix">A matrix whose means will be calculated.</param>
/// <param name="weights">A vector containing the importance of each sample in the matrix.</param>
/// <returns>Returns a vector containing the means of the given matrix.</returns>
/// 
static int dm_MeanW1(const double* matrix, int ml, const double* weights, double* mean, IDIM msz)
{
    return dm_MeanW(matrix, ml, weights, 0, mean, msz);
}

/// <summary>Calculates the matrix Variance vector.</summary>
/// <param name="matrix">A matrix whose variances will be calculated.</param>
/// <param name="means">The mean vector containing already calculated means for each column of the matix.</param>
/// <returns>Returns a vector containing the variances of the given matrix.</returns>
static int dm_VarianceM(const double* matrix, int ml, int dimension, const double* means, double* variance, IDIM msz)
{
    int i, j;
    int rows = msz.len[0];
    int cols = msz.len[1];
    double N = rows;

    //double[] variance = new double[cols];
    
    if (dimension == 0)
    {
      //double[] mean = new double[cols];
      
      // for each column (for each variable)
      for (j = 0; j < cols; j++)
      {
        double sum1 = 0.0;
        double sum2 = 0.0;
        double x = 0.0;
        
        // for each row (observation of the variable)
        for (i = 0; i < rows; i++)
        {
          x = matrix[i*ml+j] - means[j];
          sum1 += x;
          sum2 += x * x;
        }
        
        // calculate the variance
        variance[j] = (sum2 - ((sum1 * sum1) / N)) / (N - 1);
      }
    }
    else if (dimension == 1)
    {
      //double[] mean = new double[rows];
      
      for (i = 0; i < rows; i++)
      {
        double sum1 = 0.0;
        double sum2 = 0.0;
        double x = 0.0;
        
        // for each row (observation of the variable)
        for (j = 0; j < cols; j++)
        {
          x = matrix[i*ml+j] - means[i];
          sum1 += x;
          sum2 += x * x;
        }
        
        // calculate the variance
        variance[i] = (sum2 - ((sum1 * sum1) / N)) / (N - 1);
      }
    }
    else
    {
      ArgumentException("Invalid dimension.");
    }


    return 0;
}

/// <summary>Calculates the matrix Variance vector.</summary>
/// <param name="matrix">A matrix whose variancees will be calculated.</param>
/// <returns>Returns a vector containing the variances of the given matrix.</returns>
static int dm_Variance(const double* matrix, int ml, double* variance, IDIM msz)
{
    double* mean = MALLOC(double, msz.len[1]);
    dm_Mean(matrix, ml, 0, mean, msz);
    dm_VarianceM(matrix, ml, 0, mean, variance, msz);
    FREE(mean);
    return 0;
}


/// <summary>Calculates the matrix Medians vector.</summary>
/// <param name="matrix">A matrix whose medians will be calculated.</param>
/// <returns>Returns a vector containing the medians of the given matrix.</returns>
static int dm_Median(const double* matrix, int ml, int dimension, double* medians, IDIM msz)
{
    int i, j;
    int rows = msz.len[0];
    int cols = msz.len[1];
    
    //double[] variance = new double[cols];
    
    if (dimension == 0)
    {
      int N = rows;
      int* index = MALLOC(int, N);
      
      for (i = 0; i < cols; i++)
      {
        // Creates a copy of the given values
        for (j = 0; j < N; j++) {
            index[j] = j*ml+i;
        }
#define INDEXLESS(a, b)   matrix[a]<matrix[b]
        QSORT(index, index+rows, INDEXLESS, int); // Sort it
#undef INDEXLESS
        
        if (N % 2 == 0) {
          medians[i] = (matrix[index[N / 2]] + matrix[index[(N / 2) - 1]]) * 0.5; // N is even 
        } else {
          medians[i] = matrix[index[N / 2]];                      // N is odd
        }
      }
      FREE(index);
    }
    else if (dimension == 1)
    {
      int N = cols;
      int* index = MALLOC(int, N);
      
      for (i = 0; i < rows; i++)
      {
        // Creates a copy of the given values
        for (j = 0; j < N; j++) {
          index[j] = i*ml+j;
        }
#define INDEXLESS(a, b)   matrix[a]<matrix[b]
        QSORT(index, index+N, INDEXLESS, int); // Sort it
#undef INDEXLESS
        
        if (N % 2 == 0) {
          medians[i] = (matrix[index[N / 2]] + matrix[index[(N / 2) - 1]]) * 0.5; // N is even 
        } else {
          medians[i] = matrix[index[N / 2]];                      // N is odd
        }
      }
      FREE(index);
    }
    else
    {
      ArgumentException("Invalid dimension.");
    }

    return 0;
}


/// <summary>Calculates the matrix Modes vector.</summary>
/// <param name="matrix">A matrix whose modes will be calculated.</param>
/// <returns>Returns a vector containing the modes of the given matrix.</returns>
static int dm_Mode(const double* matrix, int ml, int dimension, double* mode, IDIM msz)
{
    int i, j;
    int rows = msz.len[0];
    int cols = msz.len[1];
    
    //double[] variance = new double[cols];
    
    if (dimension == 0)
    {
      //double[] mode = new double[cols];
      int N = rows;
      int* itemCount = MALLOC(int, N);
      double* itemArray = MALLOC(double, N);
      
      for (i = 0; i < cols; i++)
      {
        int count = 0;
        int maxValue = 0;
        int maxIndex = 0;
        FILL(N, itemCount, 0);
        FILL(N, itemArray, 0);
        
        // for each row
        for (j = 0; j < N; j++)
        {
          int index = -1;
          double m = matrix[j*ml+i];
          INDEXOF(count, itemArray, m, index);
          
          if (index >= 0)
          {
            itemCount[index]++;
          }
          else
          {
            itemArray[count] = m;
            itemCount[count] = 1;
            count++;
          }
        }
        
        for (j = 0; j < count; j++)
        {
          if (itemCount[j] > maxValue)
          {
            maxValue = itemCount[j];
            maxIndex = j;
          }
        }
        
        mode[i] = itemArray[maxIndex];
      }
      
      FREE(itemCount);
      FREE(itemArray);
    }
    else if (dimension == 1)
    {
      int N = cols;
      int* itemCount = MALLOC(int, N);
      double* itemArray = MALLOC(double, N);
      
      for (i = 0; i < rows; i++)
      {
        int count = 0;
        int maxValue = 0;
        int maxIndex = 0;
        FILL(N, itemCount, 0);
        FILL(N, itemArray, 0);
        
        // for each row
        for (j = 0; j < N; j++)
        {
          int index = -1;
          double m = matrix[i*ml+j];
          INDEXOF(count, itemArray, m, index);
          
          if (index >= 0)
          {
            itemCount[index]++;
          }
          else
          {
            itemArray[count] = m;
            itemCount[count] = 1;
            count++;
          }
        }
        
        for (j = 0; j < count; j++)
        {
          if (itemCount[j] > maxValue)
          {
            maxValue = itemCount[j];
            maxIndex = j;
          }
        }
        
        mode[i] = itemArray[maxIndex];
      }
      
      FREE(itemCount);
      FREE(itemArray);
    }
    else
    {
      ArgumentException("Invalid dimension.");
    }

    return 0;
}

/// <summary>Calculates the matrix Mean vector.</summary>
/// <param name="matrix">A matrix whose means will be calculated.</param>
/// <param name="sums">The sum vector containing already calculated sums for each column of the matix.</param>
/// <returns>Returns a vector containing the means of the given matrix.</returns>

// static double[] Mean(double[,] matrix, double[] sums)
// {
//     int rows = matrix.GetLength(0);
//     int cols = matrix.GetLength(1);
// 
//     double[] mean = new double[cols];
//     double N = rows;
// 
//     for (j = 0; j < cols; j++)
//         mean[j] = sums[j] / N;
// 
//     return mean;
// }


/// <summary>
///   Calculates the scatter matrix of a sample matrix.
/// </summary>
/// <remarks>
///   By dividing the Scatter matrix by the sample size, we get the population
///   Covariance matrix. By dividing by the sample size minus one, we get the
///   sample Covariance matrix.
/// </remarks>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <param name="means">The values' mean vector, if already known. Length of the mean vector should equal the number of columns(0) or rows(1)</param>
/// <param name="divisor">A real number to divide each member of the matrix.</param>
/// <param name="dimension">
///   Pass 0 if the mean vector is a row vector, 1 otherwise. Default value is 0.
/// </param>
/// <returns>The covariance matrix.</returns>
static int dm_ScatterMD(const double* matrix, int ml, const double* means, double divisor, int dimension, double* cov, int cl, IDIM msz)
{
    int i, j, k;
    int rows = msz.len[0];
    int cols = msz.len[1];

    if (dimension == 0)
    {
        for (i = 0; i < cols; i++)
        {
            for (j = i; j < cols; j++)
            {
                double s = 0.0;
                for (k = 0; k < rows; k++) {
                    s += (matrix[k*ml+j] - means[j]) * (matrix[k*ml+i] - means[i]);
                }
                s /= divisor;
                cov[j*cl+i] = cov[i*cl+j] = s;
            }
        }
    }
    else if (dimension == 1)
    {
        for (i = 0; i < rows; i++)
        {
            for (j = i; j < rows; j++)
            {
                double s = 0.0;
                for (k = 0; k < cols; k++) {
                    s += (matrix[j*ml+k] - means[j]) * (matrix[i*ml+k] - means[i]);
                }
                s /= divisor;
                cov[j*cl+i] = cov[i*cl+j] = s;
            }
        }
    }
    else
    {
        ArgumentException("Invalid dimension.");
    }

    return 0;
}

/// <summary>
///   Calculates the scatter matrix of a sample matrix.
/// </summary>
/// <remarks>
///   By dividing the Scatter matrix by the sample size, we get the population
///   Covariance matrix. By dividing by the sample size minus one, we get the
///   sample Covariance matrix.
/// </remarks>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <param name="means">The values' mean vector, if already known.</param>
/// <param name="divisor">A real number to divide each member of the matrix.</param>
/// <param name="dimension">
///   Pass 0 to if mean vector is a row vector, 1 otherwise. Default value is 0.
/// </param>
/// <param name="weights">An unit vector containing the importance of each sample
/// in <see param="values"/>. The sum of this array elements should add up to 1.</param>
/// <returns>The covariance matrix.</returns>
static int dm_ScatterMDW(const double* matrix, int ml, const double* means, double divisor, int dimension, const double* weights, double* cov, int cl, IDIM msz)
{
  int i, j, k;
  int rows = msz.len[0];
  int cols = msz.len[1];
  
  if (dimension == 0)
  {
    for (i = 0; i < cols; i++)
    {
      for (j = i; j < cols; j++)
      {
        double s = 0.0;
        for (k = 0; k < rows; k++) {
          s += weights[k] * (matrix[k*ml+j] - means[j]) * (matrix[k*ml+i] - means[i]);
        }
        s /= divisor;
        cov[j*cl+i] = cov[i*cl+j] = s;
      }
    }
  }
  else if (dimension == 1)
  {
    for (i = 0; i < rows; i++)
    {
      for (j = i; j < rows; j++)
      {
        double s = 0.0;
        for (k = 0; k < cols; k++) {
          s += weights[k] * (matrix[j*ml+k] - means[j]) * (matrix[i*ml+k] - means[i]);
        }
        s /= divisor;
        cov[j*cl+i] = cov[i*cl+j] = s;
      }
    }
  }
  else
  {
    ArgumentException("Invalid dimension.");
  }
  
  return 0;
}

/// <summary>
///   Calculates the scatter matrix of a sample matrix.
/// </summary>
/// <remarks>
///   By dividing the Scatter matrix by the sample size, we get the population
///   Covariance matrix. By dividing by the sample size minus one, we get the
///   sample Covariance matrix.
/// </remarks>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <param name="means">The values' mean vector, if already known.</param>
/// <param name="dimension">
///   Pass 0 to if mean vector is a row vector, 1 otherwise. Default value is 0.
/// </param>
/// <returns>The covariance matrix.</returns>
static int dm_ScatterM(const double* matrix, int ml, const double* means, int dimension, double* cov, int cl, IDIM msz)
{
    return dm_ScatterMD(matrix, ml, means, 1.0, dimension, cov, cl, msz);
}

/// <summary>Computes the Covariance between two values arrays.</summary>
/// <param name="u">A number array containing the first vector members.</param>
/// <param name="v">A number array containing the second vector members.</param>
/// <returns>The variance of the given data.</returns>
static int dv_Covariance(int n, const double* u, const double* v, double* cov, int cl)
{
    IDIM sz = iDIM(2, n);
    double* vectors = MALLOC(double, 2*n);
    double* mean = MALLOC(double, n);
    COPY(n, u, vectors+0);
    COPY(v, u, vectors+n);
    dm_Mean(vectors, n, 1, mean, sz);
    dm_ScatterMD(vectors, n, mean, 2, 1, cov, cl, sz);
    FREE(vectors);
    FREE(mean);
    return 0;
}


/// <summary>
///   Computes the Skewness for the given values.
/// </summary>
/// <remarks>
///   Skewness characterizes the degree of asymmetry of a distribution
///   around its mean. Positive skewness indicates a distribution with
///   an asymmetric tail extending towards more positive values. Negative
///   skewness indicates a distribution with an asymmetric tail extending
///   towards more negative values.
/// </remarks>
/// <param name="values">A number array containing the vector values.</param>
/// <param name="mean">The values' mean, if already known.</param>
/// <param name="standardDeviation">The values' standard deviations, if already known.</param>
/// <returns>The skewness of the given data.</returns>
static double dv_SkewnessM(int n, const double* values, double mean, double standardDeviation)
{
    int i;
    double sum = 0.0;
    for (i = 0; i < n; i++)
    {
        // Sum of third moment deviations
        sum += pow(values[i] - mean, 3);
    }

    return sum / ((double)n * pow(standardDeviation, 3));
}

/// <summary>
///   Computes the Skewness for the given values.
/// </summary>
/// <remarks>
///   Skewness characterizes the degree of asymmetry of a distribution
///   around its mean. Positive skewness indicates a distribution with
///   an asymmetric tail extending towards more positive values. Negative
///   skewness indicates a distribution with an asymmetric tail extending
///   towards more negative values.
/// </remarks>
/// <param name="values">A number array containing the vector values.</param>
/// <returns>The skewness of the given data.</returns>
static double dv_Skewness(int n, const double* values)
{
    double mean = dv_Mean(n, values);
    double standardDeviation = dv_StandardDeviationM(n, values, mean);
    return dv_SkewnessM(n, values, mean, standardDeviation);
}

/// <summary>
///   Computes the Kurtosis for the given values.
/// </summary>
/// <param name="values">A number array containing the vector values.</param>
/// <param name="mean">The values' mean, if already known.</param>
/// <param name="standardDeviation">The values' variance, if already known.</param>
/// <returns>The kurtosis of the given data.</returns>
static double dv_KurtosisM(int n, const double* values, double mean, double standardDeviation)
{
    int i;
    double sum = 0.0, deviation;
    for (i = 0; i < n; i++)
    {
        // Sum of fourth moment deviations
        deviation = (values[i] - mean);
        sum += pow(deviation, 4);
    }

    return sum / ((double)n * pow(standardDeviation, 4)) - 3.0;
}

/// <summary>
///   Computes the Kurtosis for the given values.
/// </summary>
/// <param name="values">A number array containing the vector values.</param>
/// <returns>The kurtosis of the given data.</returns>
static double dv_Kurtosis(int n, const double* values)
{
    double mean = dv_Mean(n, values);
    double standardDeviation = dv_StandardDeviationM(n, values, mean);
    return dv_KurtosisM(n, values, mean, standardDeviation);
}

/// <summary>Calculates the matrix Standard Deviations vector.</summary>
/// <param name="matrix">A matrix whose deviations will be calculated.</param>
/// <param name="means">The mean vector containing already calculated means for each column of the matix.</param>
/// <returns>Returns a vector containing the standard deviations of the given matrix.</returns>
static int dm_StandardDeviationM(const double* matrix, int ml, int dimension, const double* means, double* standardDeviations, IDIM msz)
{
  int i;
  dm_VarianceM(matrix, ml, dimension, means, standardDeviations, msz);
  for (i=0; i<msz.len[!dimension]; ++i) {
    standardDeviations[i] = sqrt(standardDeviations[i]);
  }
  return 0;
}

/// <summary>Calculates the matrix Standard Deviations vector.</summary>
/// <param name="matrix">A matrix whose deviations will be calculated.</param>
/// <returns>Returns a vector containing the standard deviations of the given matrix.</returns>
static int dm_StandardDeviation(const double* matrix, int ml, int dimension, double* standardDeviations, IDIM msz)
{
  double* means = MALLOC(double, msz.len[!dimension]);
  dm_Mean(matrix, ml, dimension, means, msz);
  dm_StandardDeviationM(matrix, ml, dimension, means, standardDeviations, msz);
  return 0;
}

/// <summary>Centers an observation, subtracting the empirical mean from the variable.</summary>
static int dv_CenterM(int n, double* observation, double mean)
{
  int i;
  for (i = 0; i < n; i++) {
    observation[i] -= mean;
  }
  return 0;
}

/// <summary>Centers an observation, subtracting the empirical mean from the variable.</summary>
static int dv_Center(int n, double* observation)
{
    double mean = dv_Mean(n, observation);
    return dv_CenterM(n, observation, mean);
}


/// <summary>
///   Computes the Skewness vector for the given matrix.
/// </summary>
/// <remarks>
///   Skewness characterizes the degree of asymmetry of a distribution
///   around its mean. Positive skewness indicates a distribution with
///   an asymmetric tail extending towards more positive values. Negative
///   skewness indicates a distribution with an asymmetric tail extending
///   towards more negative values.
/// </remarks>
/// <param name="matrix">A number array containing the vector values.</param>
/// <param name="means">The values' mean, if already known.</param>
/// <param name="standardDeviations">The values' standard deviations, if already known.</param>
/// <returns>The skewness of the given data.</returns>
static int dm_SkewnessMS(const double* matrix, int ml, int dimension, const double* means, const double* standardDeviations, double* skewness, IDIM msz)
{
    int i, j;
    int rows = msz.len[0];
    int cols = msz.len[1];
    
    if (dimension == 0)
    {
      //double[] skewness = new double[cols];
      int n = rows;
      for (j = 0; j < cols; j++)
      {
        double sum = 0.0;
        for (i = 0; i < n; i++)
        {
          // Sum of third moment deviations
          sum += pow(matrix[i*ml+j] - means[j], 3);
        }
        
        skewness[j] = sum / ((n - 1) * pow(standardDeviations[j], 3));
      }

    }
    else if (dimension == 1)
    {
      int n = cols;
      for (j = 0; j < rows; j++)
      {
        double sum = 0.0;
        for (i = 0; i < n; i++)
        {
          // Sum of third moment deviations
          sum += pow(matrix[j*ml+i] - means[j], 3);
        }
        
        skewness[j] = sum / ((n - 1) * pow(standardDeviations[j], 3));
      }
    }
    else
    {
      ArgumentException("Invalid dimension.");
    }

    return 0;
}

/// <summary>
///   Computes the Skewness for the given values.
/// </summary>
/// <remarks>
///   Skewness characterizes the degree of asymmetry of a distribution
///   around its mean. Positive skewness indicates a distribution with
///   an asymmetric tail extending towards more positive values. Negative
///   skewness indicates a distribution with an asymmetric tail extending
///   towards more negative values.
/// </remarks>
/// <param name="matrix">A number matrix containing the matrix values.</param>
/// <returns>The skewness of the given data.</returns>
static int dm_Skewness(const double* matrix, int ml, int dimension, double* skewness, IDIM msz)
{
  double* means = MALLOC(double, msz.len[!dimension]);
  double* standardDeviations = MALLOC(double, msz.len[!dimension]);
  dm_Mean(matrix, ml, dimension, means, msz);
  dm_StandardDeviationM(matrix, ml, dimension, standardDeviations, means, msz);
  dm_SkewnessMS(matrix, ml, dimension, means, standardDeviations, skewness, msz);
  FREE(means);
  FREE(standardDeviations);
  return 0;
}

/// <summary>
///   Computes the Kurtosis vector for the given matrix.
/// </summary>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <param name="means">The values' mean vector, if already known.</param>
/// <param name="standardDeviations">The values' standard deviation vector, if already known.</param>
/// <returns>The kurtosis vector of the given data.</returns>
static int dm_KurtosisMS(const double* matrix, int ml, int dimension, const double* means, const double* standardDeviations, double* kurtosis, IDIM msz)
{
  int i, j;
  int rows = msz.len[0];
  int cols = msz.len[1];
  
  if (dimension == 0)
  {
    //double[] skewness = new double[cols];
    int n = rows;
    for (j = 0; j < cols; j++)
    {
        double sum = 0.0;
        for (i = 0; i < n; i++)
        {
            // Sum of fourth moment deviations
            sum += pow(matrix[i*ml+j] - means[j], 4);
        }

        kurtosis[j] = sum / (n * pow(standardDeviations[j], 4)) - 3.0;
    }
  }
  else if (dimension == 1)
  {
    int n = cols;
    for (j = 0; j < rows; j++)
    {
      double sum = 0.0;
      for (i = 0; i < n; i++)
      {
        // Sum of fourth moment deviations
        sum += pow(matrix[j*ml+i] - means[j], 4);
      }
      
      kurtosis[j] = sum / (n * pow(standardDeviations[j], 4)) - 3.0;
    }
  }
  else
  {
    ArgumentException("Invalid dimension.");
  }

  return 0;
}


/// <summary>
///   Computes the Kurtosis vector for the given matrix.
/// </summary>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <returns>The kurtosis vector of the given data.</returns>
static int dm_Kurtosis(const double* matrix, int ml, int dimension, double* kurtosis, IDIM msz)
{
  double* means = MALLOC(double, msz.len[!dimension]);
  double* standardDeviations = MALLOC(double, msz.len[!dimension]);
  dm_Mean(matrix, ml, dimension, means, msz);
  dm_StandardDeviationM(matrix, ml, dimension, standardDeviations, means, msz);
  dm_KurtosisMS(matrix, ml, dimension, means, standardDeviations, kurtosis, msz);
  FREE(means);
  FREE(standardDeviations);
  return 0;
}

/// <summary>
///   Computes the Standard Error vector for a given matrix.
/// </summary>
/// <param name="samples">The number of samples in the matrix.</param>
/// <param name="standardDeviations">The values' standard deviation vector, if already known.</param>
/// <returns>Returns the standard error vector for the matrix.</returns>
static int dm_StandardErrorS(int samples, int n, const double* standardDeviations, double* standardErrors)
{
    double sqrt_samples = sqrt(samples);
    int i;
    for (i = 0; i < n; i++)
    {
        standardErrors[i] = standardDeviations[i] / sqrt_samples;
    }
    return 0;
}


/// <summary>
///   Computes the Standard Error vector for a given matrix.
/// </summary>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <returns>Returns the standard error vector for the matrix.</returns>
static int dm_StandardError(const double* matrix, int ml, int dimension, double* standardErrors, IDIM msz)
{
  double* standardDeviations = MALLOC(double, msz.len[!dimension]);
  dm_StandardDeviation(matrix, ml, dimension, standardDeviations, msz);
  dm_StandardErrorS(msz.len[dimension], msz.len[!dimension], standardDeviations, standardErrors);
  FREE(standardDeviations);
  return 0;
}

/// <summary>
///   Calculates the covariance matrix of a sample matrix.
/// </summary>        /// <remarks>
///   In statistics and probability theory, the covariance matrix is a matrix of
///   covariances between elements of a vector. It is the natural generalization
///   to higher dimensions of the concept of the variance of a scalar-valued
///   random variable.
/// </remarks>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <param name="means">The values' mean vector, if already known.</param>
/// <returns>The covariance matrix.</returns>
static int dm_CovarianceM(const double* matrix, int ml, const double* means, int dimension, double* cov, int cl, IDIM msz)
{
  dm_ScatterMD(matrix, ml, means, msz.len[dimension] - 1, dimension, cov, cl, msz);
  return 0;
}

/// <summary>
///   Calculates the covariance matrix of a sample matrix.
/// </summary>
/// <remarks>
///   In statistics and probability theory, the covariance matrix is a matrix of
///   covariances between elements of a vector. It is the natural generalization
///   to higher dimensions of the concept of the variance of a scalar-valued
///   random variable.
/// </remarks>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <param name="dimension">
///   The dimension of the matrix to consider as observations. Pass 0 if the matrix has
///   observations as rows and variables as columns, pass 1 otherwise. Default is 0.
/// </param>
/// <returns>The covariance matrix.</returns>
static int dm_Covariance(const double* matrix, int ml, int dimension, double* cov, int cl, IDIM msz)
{
  double* means = MALLOC(double, msz.len[!dimension]);
  dm_Mean(matrix, ml, dimension, means, msz);
  dm_ScatterMD(matrix, ml, means, msz.len[dimension] - 1, dimension, cov, cl, msz);
  FREE(means);
  return 0;
}

/// <summary>
///   Calculates the scatter matrix of a sample matrix.
/// </summary>
/// <remarks>
///   By dividing the Scatter matrix by the sample size, we get the population
///   Covariance matrix. By dividing by the sample size minus one, we get the
///   sample Covariance matrix.
/// </remarks>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <param name="means">The values' mean vector, if already known.</param>
/// <param name="weights">An unit vector containing the importance of each sample
/// in <see param="values"/>. The sum of this array elements should add up to 1.</param>
/// <returns>The covariance matrix.</returns>

//static int dm_ScatterMDW(const double* matrix, int ml, const double* means, double divisor, int dimension, const double* weights, double* cov, int cl, IDIM msz)
static double dm_CovarianceMW(const double* matrix, int ml, const double* means, int dimension, const double* weights, double* cov, int cl, IDIM msz)
{
    double sw = 1.0;
    int i;
    for (i = 0; i < msz.len[dimension]; i++) {
      sw -= weights[i] * weights[i];
    }

    dm_ScatterMDW(matrix, ml, means, sw, dimension, weights, cov, cl, msz);
    return 0;
}

/// <summary>Centers column data, subtracting the empirical mean from each variable.</summary>
/// <param name="matrix">A matrix where each column represent a variable and each row represent a observation.</param>
/// <param name="means">The values' mean vector, if already known.</param>
static int dm_CenterM(double* matrix, int ml, const double* means, int dimension, IDIM msz)
{
  int i, j;
  int rows = msz.len[0];
  int cols = msz.len[1];
  
  if (dimension == 0)
  {
    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        matrix[i*ml+j] -= means[j];
      }
    }
  }
  else if (dimension == 1)
  {
    // for each row
    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        matrix[i*ml+j] -= means[i];
      }
    }
  }
  else
  {
    ArgumentException("Invalid dimension.");
  }
  return 0;
  
}

/// <summary>Centers column data, subtracting the empirical mean from each variable.</summary>
/// <param name="matrix">A matrix where each column represent a variable and each row represent a observation.</param>
static int dm_Center(double* matrix, int ml, int dimension, IDIM msz)
{
  double* means = MALLOC(double, msz.len[!dimension]);
  dm_Mean(matrix, ml, dimension, means, msz);
  dm_CenterM(matrix, ml, means, dimension, msz);
  FREE(means);
  return 0;
}

/// <summary>Standardizes column data, removing the empirical standard deviation from each variable.</summary>
/// <param name="matrix">A matrix where each column represent a variable and each row represent a observation.</param>
/// <remarks>This method does not remove the empirical mean prior to execution.</remarks>
/// <param name="standardDeviations">The values' standard deviation vector, if already known.</param>
static int dm_StandardizeS(double* matrix, int ml, const double* standardDeviations, int dimension, IDIM msz)
{
  int i, j;
  int rows = msz.len[0];
  int cols = msz.len[1];
  
  if (dimension == 0)
  {
    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        matrix[i*ml+j] /= standardDeviations[j];
      }
    }
  }
  else if (dimension == 1)
  {
    // for each row
    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        matrix[i*ml+j] /= standardDeviations[i];
      }
    }
  }
  else
  {
    ArgumentException("Invalid dimension.");
  }
}



/// <summary>Standardizes column data, removing the empirical standard deviation from each variable.</summary>
/// <param name="matrix">A matrix where each column represent a variable and each row represent a observation.</param>
/// <remarks>This method does not remove the empirical mean prior to execution.</remarks>
static int dm_Standardize(double* matrix, int ml, int dimension, IDIM msz)
{
  double* standardDeviation = MALLOC(double, msz.len[!dimension]);
  dm_StandardDeviation(matrix, ml, dimension, standardDeviation, msz);
  dm_StandardizeS(matrix, ml, standardDeviation, dimension, msz);
  FREE(standardDeviation);
  return 0;
}

/// <summary>Generates the Standard Scores, also known as Z-Scores, the core from the given data.</summary>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <param name="means">The values' mean vector, if already known.</param>
/// <param name="standardDeviations">The values' standard deviation vector, if already known.</param>
/// <returns>The Z-Scores for the matrix.</returns>
static int dm_ZScoresMS(double* m, int ml, const double* means, const double* standardDeviations, int dimension, IDIM msz)
{
  //double* m = MALLOC(double, msz.len[0]*msz.len[1]);
  
  dm_CenterM(m, ml, means, dimension, msz);
  dm_StandardizeS(m, ml, standardDeviations, dimension, msz);
  return 0;
}

/// <summary>Generates the Standard Scores, also known as Z-Scores, the core from the given data.</summary>
/// <param name="matrix">A number multi-dimensional array containing the matrix values.</param>
/// <returns>The Z-Scores for the matrix.</returns>
static int ZScores(double* m, int ml, int dimension, IDIM msz)
{
  double* means = MALLOC(double, msz.len[!dimension]);
  double* standardDeviation = MALLOC(double, msz.len[!dimension]);
  dm_Mean(m, ml, dimension, means, msz);
  dm_StandardDeviation(m, ml, dimension, standardDeviation, msz);
  dm_ZScoresMS(m, ml, means, standardDeviation, dimension, msz);
  FREE(standardDeviation);
  FREE(means);
  return 0;
}

#if 0

/// <summary>
///   Calculates the correlation matrix for a matrix of samples.
/// </summary>
/// <remarks>
///   In statistics and probability theory, the correlation matrix is the same
///   as the covariance matrix of the standardized random variables.
/// </remarks>
/// <param name="matrix">A multi-dimensional array containing the matrix values.</param>
/// <param name="means">The values' mean vector, if already known.</param>
/// <param name="standardDeviations">The values' standard deviation vector, if already known.</param>
/// <returns>The correlation matrix.</returns>
static int dm_CorrelationMS(const double[,] matrix, double[] means, double[] standardDeviations)
{
  int i, j;
  int rows = msz.len[0];
  int cols = msz.len[1];
  
  if (dimension == 0)
  {
    
    double[,] scores = ZScores(matrix, means, standardDeviations);
    //double[] mean = new double[cols];
    
    for (j = 0; j < cols; j++) {
      mean[j] = 0;
    }
    // for each row
    for (i = 0; i < rows; i++) {
      // for each column
      for (j = 0; j < cols; j++) {
        mean[j] += matrix[i*ml+j] * weights[j];
      }
    }
    
  }
  else if (dimension == 1)
  {
    //double[] mean = new double[rows];
    
    // for each row
    for (i = 0; i < rows; i++) {
      double s = 0, w = weights[i];
      // for each column
      for (j = 0; j < cols; j++) {
        mean[i] += matrix[i*ml+j] * w;
      }
      mean[i] = s / cols;
    }
    
  }
  else
  {
    ArgumentException("Invalid dimension.");
  }
    return 0;
  
  double N = rows;
  double[,] cor = new double[cols, cols];
  for (i = 0; i < cols; i++)
  {
    for (j = i; j < cols; j++)
    {
      double c = 0.0;
      for (k = 0; k < rows; k++)
        c += scores[k, j] * scores[k, i];
      c /= N - 1.0;
      cor[i, j] = c;
      cor[j, i] = c;
    }
  }
  
  return cor;
}

/// <summary>
///   Calculates the correlation matrix for a matrix of samples.
/// </summary>
/// <remarks>
///   In statistics and probability theory, the correlation matrix is the same
///   as the covariance matrix of the standardized random variables.
/// </remarks>
/// <param name="matrix">A multi-dimensional array containing the matrix values.</param>
/// <returns>The correlation matrix.</returns>
static double[,] Correlation(double[,] matrix)
{
    double[] means = Mean(matrix);
    return dm_CorrelationMS(matrix, means, StandardDeviation(matrix, means));
}


// ------------------------------------------------------------


//#region Summarizing, grouping and extending operations
/// <summary>
///   Calculates the prevalence of a class.
/// </summary>
/// <param name="positives">An array of counts detailing the occurence of the first class.</param>
/// <param name="negatives">An array of counts detailing the occurence of the second class.</param>
/// <returns>An array containing the proportion of the first class over the total of occurances.</returns>
static double[] Proportions(int[] positives, int[] negatives)
{
    double[] r = new double[positives.Length];
    for (i = 0; i < r.Length; i++)
        r[i] = (double)positives[i] / (positives[i] + negatives[i]);
    return r;
}

/// <summary>
///   Calculates the prevalence of a class.
/// </summary>
/// <param name="data">A matrix containing counted, grouped data.</param>
/// <param name="positiveColumn">The index for the column which contains counts for occurence of the first class.</param>
/// <param name="negativeColumn">The index for the column which contains counts for occurence of the second class.</param>
/// <returns>An array containing the proportion of the first class over the total of occurances.</returns>
static double[] Proportions(int[][] data, int positiveColumn, int negativeColumn)
{
    double[] r = new double[data.Length];
    for (i = 0; i < r.Length; i++)
        r[i] = (double)data[i][positiveColumn] / (data[i][positiveColumn] + data[i][negativeColumn]);
    return r;
}

/// <summary>
///   Groups the occurances contained in data matrix of binary (dichotomous) data.
/// </summary>
/// <param name="data">A data matrix containing at least a column of binary data.</param>
/// <param name="labelColumn">Index of the column which contains the group label name.</param>
/// <param name="dataColumn">Index of the column which contains the binary [0,1] data.</param>
/// <returns>
///    A matrix containing the group label in the first column, the number of occurances of the first class
///    in the second column and the number of occurances of the second class in the third column.
/// </returns>
static int[][] Group(int[][] data, int labelColumn, int dataColumn)
{
    var groups = new List<int>();
    var groupings = new List<int[]>();

    for (i = 0; i < data.Length; i++)
    {
        int group = data[i][labelColumn];
        if (!groups.Contains(group))
        {
            groups.Add(group);

            int positives = 0, negatives = 0;
            for (j = 0; j < data.Length; j++)
            {
                if (data[j][labelColumn] == group)
                {
                    if (data[j][dataColumn] == 0)
                        negatives++;
                    else positives++;
                }
            }

            groupings.Add(new int[] { group, positives, negatives });
        }
    }

    return groupings.ToArray();
}

/// <summary>
///   Extends a grouped data into a full observation matrix.
/// </summary>
/// <param name="group">The group labels.</param>
/// <param name="positives">
///   An array containing he occurence of the positive class
///   for each of the groups.</param>
/// <param name="negatives">
///   An array containing he occurence of the negative class
///   for each of the groups.</param>
/// <returns>A full sized observation matrix.</returns>
static int[][] Extend(int[] group, int[] positives, int[] negatives)
{
    List<int[]> rows = new List<int[]>();

    for (i = 0; i < group.Length; i++)
    {
        for (j = 0; j < positives[i]; j++)
            rows.Add(new int[] { group[i], 1 });

        for (j = 0; j < negatives[i]; j++)
            rows.Add(new int[] { group[i], 0 });
    }

    return rows.ToArray();
}

/// <summary>
///   Extendes a grouped data into a full observation matrix.
/// </summary>
/// <param name="data">The grouped data matrix.</param>
/// <param name="labelColumn">Index of the column which contains the labels
/// in the grouped data matrix. </param>
/// <param name="positiveColumn">Index of the column which contains
///   the occurances for the first class.</param>
/// <param name="negativeColumn">Index of the column which contains
///   the occurances for the second class.</param>
/// <returns>A full sized observation matrix.</returns>
static int[][] Extend(int[][] data, int labelColumn, int positiveColumn, int negativeColumn)
{
    List<int[]> rows = new List<int[]>();

    for (i = 0; i < data.Length; i++)
    {
        for (j = 0; j < data[i][positiveColumn]; j++)
            rows.Add(new int[] { data[i][labelColumn], 1 });

        for (j = 0; j < data[i][negativeColumn]; j++)
            rows.Add(new int[] { data[i][labelColumn], 0 });
    }

    return rows.ToArray();
}




#region Determination and performance measures
/// <summary>
///   Gets the coefficient of determination, as known as the R-Squared (R虏)
/// </summary>
/// <remarks>
///    The coefficient of determination is used in the context of statistical models
///    whose main purpose is the prediction of future outcomes on the basis of other
///    related information. It is the proportion of variability in a data set that
///    is accounted for by the statistical model. It provides a measure of how well
///    future outcomes are likely to be predicted by the model.
///    
///    The R^2 coefficient of determination is a statistical measure of how well the
///    regression approximates the real data points. An R^2 of 1.0 indicates that the
///    regression perfectly fits the data.
/// </remarks>
static double Determination(double[] actual, double[] expected)
{
    // R-squared = 100 * SS(regression) / SS(total)

    int N = actual.Length;
    double SSe = 0.0;
    double SSt = 0.0;
    double avg = 0.0;
    double d;

    // Calculate expected output mean
    for (i = 0; i < N; i++)
        avg += expected[i];
    avg /= N;

    // Calculate SSe and SSt
    for (i = 0; i < N; i++)
    {
        d = expected[i] - actual[i];
        SSe += d * d;

        d = expected[i] - avg;
        SSt += d * d;
    }

    // Calculate R-Squared
    return 1.0 - (SSe / SSt);
}

#endif


/// <summary>
///   Shuffles an array.
/// </summary>
static int iv_shuffle(int n, int* x)
{
  int i;
  int t;
  // i is the number of items remaining to be shuffled.
  for (i = n; i > 1; i--)
  {
    // Pick a random element to swap with the i-th element.
    //int j = random.Next(i);
    int j = rand()%(i);
    
    // Swap array elements.
    CV_SWAP(x[j], x[i - 1], t);
  }
  return 0;
}


/// <summary>
///   Returns a random permutation of size n.
/// </summary>
static int random_permutation(int n, int* x)
{
    int i, j, t;
    for (i=0; i<n; ++i) {
        x[i] = i;
    }
    for (i = 0; i < n; ++i) {
        j = rand()%n;
        CV_SWAP(x[i], x[j], t);
    }
    return 0;
}

//#region Permutations and combinatorials
/// <summary>
///   Returns a random sample of size k from a population of size n.
/// </summary>
static int random_permutation2(int n, int k, int* x)
{
    int* x1 = MALLOC(int, n);
    ASSERT(k<=n);
    random_permutation(n, x1);
    MEMCPY(x, x1, k);
    return 0;
}

