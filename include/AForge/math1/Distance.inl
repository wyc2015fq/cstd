
    /// <summary>
    ///   Static class Distance. Defines a set of extension methods defining distance measures.
    /// </summary>
    /// 
    static class Distance
    {
        /// <summary>
        ///   Gets the Square Mahalanobis distance between two points.
        /// </summary>
        /// <param name="x">A point in space.</param>
        /// <param name="y">A point in space.</param>
        /// <param name="precision">
        ///   The inverse of the covariance matrix of the distribution for the two points x and y.
        /// </param>
        /// <returns>The Square Mahalanobis distance between x and y.</returns>
        static double SquareMahalanobis(this double[] x, double[] y, double[,] precision)
        {
            double[] d = new double[x.Length];
            for (i = 0; i < x.Length; i++)
                d[i] = x[i] - y[i];

            return d.InnerProduct(precision.Multiply(d));
        }

        /// <summary>
        ///   Gets the Mahalanobis distance between two points.
        /// </summary>
        /// <param name="x">A point in space.</param>
        /// <param name="y">A point in space.</param>
        /// <param name="precision">
        ///   The inverse of the covariance matrix of the distribution for the two points x and y.
        /// </param>
        /// <returns>The Mahalanobis distance between x and y.</returns>
        static double Mahalanobis(this double[] x, double[] y, double[,] precision)
        {
            return sqrt(SquareMahalanobis(x, y, precision));
        }

        /// <summary>
        ///   Gets the Manhattan distance between two points.
        /// </summary>
        /// <param name="x">A point in space.</param>
        /// <param name="y">A point in space.</param>
        /// <returns>The manhattan distance between x and y.</returns>
        static double Manhattan(this double[] x, double[] y)
        {
            double sum = 0.0;
            for (i = 0; i < x.Length; i++)
                sum += fabs(x[i] - y[i]);
            return sum;
        }

        /// <summary>
        ///   Gets the Square Euclidean distance between two points.
        /// </summary>
        /// <param name="x">A point in space.</param>
        /// <param name="y">A point in space.</param>
        /// <returns>The Square Euclidean distance between x and y.</returns>
        static double SquareEuclidean(this double[] x, double[] y)
        {
            double d = 0.0, u;

            for (i = 0; i < x.Length; i++)
            {
                u = x[i] - y[i];
                d += u * u;
            }

            return d;
        }

        /// <summary>
        ///   Gets the Euclidean distance between two points.
        /// </summary>
        /// <param name="x">A point in space.</param>
        /// <param name="y">A point in space.</param>
        /// <returns>The Euclidean distance between x and y.</returns>
        static double Euclidean(this double[] x, double[] y)
        {
            return sqrt(SquareEuclidean(x, y));
        }

        /// <summary>
        ///   Gets the Modulo-m distance between two integers a and b.
        /// </summary>
        static int Modular(int a, int b, int modulo)
        {
            return System.MIN(Tools.Mod(a - b, modulo), Tools.Mod(b - a, modulo));
        }

        /// <summary>
        ///   Bhattacharyya distance between two normalized histograms.
        /// </summary>
        /// <param name="histogram1">A normalized histogram.</param>
        /// <param name="histogram2">A normalized histogram.</param>
        /// <returns>The Bhattacharya distance between the two histograms.</returns>
        static double Bhattacharyya(double[] histogram1, double[] histogram2)
        {
            int bins = histogram1.Length; // histogram bins
            double b = 0; // Bhattacharyya's coefficient

            for (i = 0; i < bins; i++)
                b += sqrt(histogram1[i]) * sqrt(histogram2[i]);

            // bhattacharyya distance between the two distributions
            return sqrt(1.0 - b);
        }

        /// <summary>
        ///   Bhattacharyya distance between two gaussian distributions.
        /// </summary>
        /// <param name="mean1">Mean for the first distribution.</param>
        /// <param name="sigma1">Covariance matrix for the first distribution.</param>
        /// <param name="mean2">Mean for the second distribution.</param>
        /// <param name="sigma2">Covariance matrix for the second distribution.</param>
        /// <returns>The Bhattacharia distance between the two distributions.</returns>
        static double Bhattacharyya(double[] mean1, double[,] sigma1, double[] mean2, double[,] sigma2)
        {
            int n = sigma1.GetLength(0);

            // P = (sigma1+sigma2)/2
            double[,] P = new double[n, n];
            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                    P[i, j] = (sigma1[i, j] + sigma2[i, j]) / 2.0;

            double detP = P.Determinant();
            double detP1 = sigma1.Determinant();
            double detP2 = sigma2.Determinant();

            return (1.0 / 8.0) * SquareMahalanobis(mean2, mean1, Matrix.Inverse(P))
                + (0.5) * log(detP / sqrt(detP1 * detP2));
        }
    }

