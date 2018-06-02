
/// <summary>
///   Singular Value Decomposition for a rectangular matrix.
/// </summary>
/// <remarks>
///  <para>
///	  For an m-by-n matrix <c>A</c> with <c>m >= n</c>, the singular value decomposition
///   is an m-by-n orthogonal matrix <c>U</c>, an n-by-n diagonal matrix <c>S</c>, and
///   an n-by-n orthogonal matrix <c>V</c> so that <c>A = U * S * V'</c>.
///   The singular values, <c>sigma[k] = S[k,k]</c>, are ordered so that
///   <c>sigma[0] >= sigma[1] >= ... >= sigma[n-1]</c>.</para>
///  <para>
///   The singular value decomposition always exists, so the constructor will
///   never fail. The matrix condition number and the effective numerical
///   rank can be computed from this decomposition.</para>
///  <para>
///   WARNING! Please be aware that if A has less rows than columns, it is better
///   to compute the decomposition on the transpose of A and then swap the left
///   and right eigenvectors. If the routine is computed on A directly, the diagonal
///   of singular values may contain one or more zeros. The identity A = U * S * V'
///   may still hold, however. To overcome this problem, pass true to the
///   <see cref="SingularValueDecomposition(double[,], bool, bool, bool)">autoTranspose</see> argument of the class constructor.</para>
///  <para>
///   This routine computes the economy decomposition of A.</para> 
///   
/// </remarks>

/// <summary>Returns the condition number <c>max(S) / min(S)</c>.</summary>
// double Condition    {        get { return s[0] / s[System.MIN(m, n) - 1]; }    }

/// <summary>Returns the singularity threshold.</summary>
// double Threshold    {        get { return Double.Epsilon * MAX(m, n) * s[0]; }    }

/// <summary>Returns the Two norm.</summary>
// double TwoNorm    {        get { return s[0]; }    }

/// <summary>Returns the effective numerical matrix rank.</summary>
/// <value>Number of non-negligible singular values.</value>
int svd_rank(int m, int n, int sn, const double* s)
{
    double eps = pow(2.0, -52.0);
    double tol = MAX(m, n) * s[0] * eps;
    int i, r = 0;
    for (i = 0; i < sn; i++)
    {
        if (s[i] > tol)
            r++;
    }

    return r;
}

/// <summary>Returns the one-dimensional array of singular values.</summary>		
// double[] Diagonal    {        get { return this.s; }    }

/// <summary>Returns the block diagonal matrix of singular values.</summary>		
// double[,] DiagonalMatrix    {        get { return Matrix.Diagonal(s); }    }

/// <summary>Returns the V matrix of Singular Vectors.</summary>		
// double[,] RightSingularVectors    {        get { return v; }    }

/// <summary>Return the U matrix of Singular Vectors.</summary>		
// double[,] LeftSingularVectors    {        get { return u; }    }

/// <summary>Construct singular value decomposition.</summary>
//s = double[MIN(m + 1, n)];
//u = double[m, nu];
//v = double[n, n];
int svd_init(int m, int n, const double* value, int ml, bool computeLeftSingularVectors, bool computeRightSingularVectors, bool autoTranspose, double* u, int ul, double* v, int vl, double* s, int sl)
{
    //m = value.GetLength(0); // rows
    //n = value.GetLength(1); // cols
    double* a=0;
    double eps = pow(2.0, -52.0);
    double tiny = pow(2.0, -966.0);
    int i, j, k, p;
    int al, t, nu, nct, nrt;
    int pp;
    int iter;
    double* e;
    double* work;
    bool swapped, wantu, wantv;

    if (value == NULL)
    {
        ArgumentNullException("Matrix cannot be null.");
    }
    
    a = MALLOC(double, m*n);
    al = n;

    if (m < n) // Check if we are violating JAMA's assumption
    {
        if (!autoTranspose) // Yes, check if we should correct it
        {
            // Warning! This routine is not guaranteed to work when A has less rows
            //  than columns. If this is the case, you should compute SVD on the
            //  transpose of A and then swap the left and right eigenvectors.
            // However, as the solution found can still be useful, the exception below
            // will not be thrown, and only a warning will be output in the trace.
            // throw new ArgumentException("Matrix should have more rows than columns.");
            // printf("WARNING: Computing SVD on a matrix with more columns than rows.");

            // Proceed anyway
            MEMCPY2D(m, n, value, ml, a, al);
        }
        else
        {
            // Transposing and swapping
            mem_Transpose(value, ml*sizeof(double), a, al*sizeof(double), iDIM(m, n), sizeof(double));
            CV_SWAP(m, n, t);
            swapped = true;
        }
    }
    else
    {
        // Input matrix is ok
        MEMCPY2D(m, n, value, ml, a, al);
    }


    nu = MIN(m, n);
    //s = double[MIN(m + 1, n)];
    //u = double[m, nu];
    //v = double[n, n];
    e = MALLOC(double, n);
    work = MALLOC(double, m);
    wantu = computeLeftSingularVectors;
    wantv = computeRightSingularVectors;

    // Reduce A to bidiagonal form, storing the diagonal elements in s and the super-diagonal elements in e.
    nct = MIN(m - 1, n);
    nrt = MAX(0, MIN(n - 2, m));
    for (k = 0; k < MAX(nct, nrt); k++)
    {
        if (k < nct)
        {
            // Compute the transformation for the k-th column and place the k-th diagonal in s[k].
            // Compute 2-norm of k-th column without under/overflow.
            s[k] = 0;
            for (i = k; i < m; i++)
            {
                s[k] = Hypotenuse(s[k], a[i*al+k]);
            }

            if (s[k] != 0.0)
            {
                if (a[k*al+k] < 0.0)
                {
                    s[k] = -s[k];
                }

                for (i = k; i < m; i++)
                {
                    a[i*al+k] /= s[k];
                }

                a[k*al+k] += 1.0;
            }

            s[k] = -s[k];
        }

        
        {
            for (j = k + 1; j < n; j++)
            {
                double* ptr_ak = &a[k*al+k], *ptr_aj = &a[k*al+j];
                {
                    if ((k < nct) & (s[k] != 0.0))
                    {
                        // Apply the transformation.
                        double t = 0;
                        double* ak = ptr_ak;
                        double* aj = ptr_aj;

                        for (i = k; i < m; i++)
                        {
                            t += (*ak) * (*aj);
                            ak += n; aj += n;
                        }

                        t = -t / *ptr_ak;
                        ak = ptr_ak;
                        aj = ptr_aj;

                        for (i = k; i < m; i++)
                        {
                            *aj += t * (*ak);
                            ak += n; aj += n;
                        }
                    }

                    // Place the k-th row of A into e for the subsequent calculation of the row transformation.
                    e[j] = *ptr_aj;
                }
            }
        }


        if (wantu & (k < nct))
        {
            // Place the transformation in U for subsequent back
            // multiplication.
            for (i = k; i < m; i++)
                u[i*ul+k] = a[i*al+k];
        }

        if (k < nrt)
        {
            // Compute the k-th row transformation and place the k-th super-diagonal in e[k].
            // Compute 2-norm without under/overflow.
            e[k] = 0;
            for (i = k + 1; i < n; i++)
            {
                e[k] = Hypotenuse(e[k], e[i]);
            }

            if (e[k] != 0.0)
            {
                if (e[k + 1] < 0.0)
                    e[k] = -e[k];

                for (i = k + 1; i < n; i++)
                    e[i] /= e[k];

                e[k + 1] += 1.0;
            }

            e[k] = -e[k];
            if ((k + 1 < m) & (e[k] != 0.0))
            {
                // Apply the transformation.
                for (i = k + 1; i < m; i++)
                    work[i] = 0.0;

                
                {
                    double* ptr_a = a;
                    {
                        int k1 = k + 1;
                        for (i = k1; i < m; i++)
                        {
                            double* ai = ptr_a + (i * n) + k1;
                            for (j = k1; j < n; j++, ai++)
                            {
                                work[i] += e[j] * (*ai);
                            }
                        }

                        for (j = k1; j < n; j++)
                        {
                            double t = -e[j] / e[k1];
                            double* aj = ptr_a + (k1 * n) + j;
                            for (i = k1; i < m; i++, aj += n)
                            {
                                *aj += t * work[i];
                            }
                        }
                    }
                }
            }

            if (wantv)
            {
                // Place the transformation in V for subsequent back multiplication.
                for (i = k + 1; i < n; i++)
                    v[i*vl+k] = e[i];
            }
        }
    }

    // Set up the final bidiagonal matrix or order p.
    p = MIN(n, m + 1);
    if (nct < n) s[nct] = a[nct, nct];
    if (m < p) s[p - 1] = 0.0;
    if (nrt + 1 < p) e[nrt] = a[nrt, p - 1];
    e[p - 1] = 0.0;

    // If required, generate U.
    if (wantu)
    {
        for (j = nct; j < nu; j++)
        {
            for (i = 0; i < m; i++)
                u[i*ul+j] = 0.0;
            u[j*ul+j] = 1.0;
        }

        
        {
            for (k = nct - 1; k >= 0; k--)
            {
                if (s[k] != 0.0)
                {
                    double* ptr_uk = &u[k*ul+k];
                    {
                        double* uk, *uj;
                        for (j = k + 1; j < nu; j++)
                        {
                            double* ptr_uj = &u[k*ul+j];
                            {
                                double t = 0;
                                uk = ptr_uk;
                                uj = ptr_uj;

                                for (i = k; i < m; i++)
                                {
                                    t += *uk * *uj;
                                    uk += nu; uj += nu;
                                }

                                t = -t / *ptr_uk;
                                uk = ptr_uk;
                                uj = ptr_uj;

                                for (i = k; i < m; i++)
                                {
                                    *uj += t * *uk;
                                    uk += nu; uj += nu;
                                }
                            }
                        }

                        uk = ptr_uk;
                        for (i = k; i < m; i++)
                        {
                            *uk = -(*uk);
                            uk += nu;
                        }

                        u[k*ul+k] = 1.0 + u[k*ul+k];
                        for (i = 0; i < k - 1; i++)
                            u[i*ul+k] = 0.0;
                    }
                }
                else
                {
                    for (i = 0; i < m; i++)
                        u[i*ul+k] = 0.0;
                    u[k*ul+k] = 1.0;
                }
            }
        }
    }

    // If required, generate V.
    if (wantv)
    {
        for (k = n - 1; k >= 0; k--)
        {
            if ((k < nrt) & (e[k] != 0.0))
            {
                // TODO: The following is a pseudo correction to make SVD
                //  work on matrices with n > m (less rows than columns).

                // For the proper correction, compute the decomposition of the
                //  transpose of A and swap the left and right eigenvectors

                // Original line:
                //   for (j = k + 1; j < nu; j++)
                // Pseudo correction:
                //   for (j = k + 1; j < n; j++)

                for (j = k + 1; j < n; j++) // pseudo-correction
                {
                    
                    {
                        double* ptr_vk = &v[(k + 1)*vl+k], *ptr_vj = &v[(k + 1)*vl+j];
                        {
                            double t = 0;
                            double* vk = ptr_vk;
                            double* vj = ptr_vj;

                            for (i = k + 1; i < n; i++)
                            {
                                t += *vk * *vj;
                                vk += n; vj += n;
                            }

                            t = -t / *ptr_vk;
                            vk = ptr_vk;
                            vj = ptr_vj;

                            for (i = k + 1; i < n; i++)
                            {
                                *vj += t * *vk;
                                vk += n; vj += n;
                            }
                        }
                    }
                }
            }

            for (i = 0; i < n; i++)
                v[i*vl+k] = 0.0;
            v[k, k] = 1.0;
        }
    }

    // Main iteration loop for the singular values.
    pp = p - 1;
    iter = 0;
    while (p > 0)
    {
        int k, kase;

        // Here is where a test for too many iterations would go.

        // This section of the program inspects for
        // negligible elements in the s and e arrays.  On
        // completion the variables kase and k are set as follows.

        // kase = 1     if s(p) and e[k-1] are negligible and k<p
        // kase = 2     if s(k) is negligible and k<p
        // kase = 3     if e[k-1] is negligible, k<p, and
        //              s(k), ..., s(p) are not negligible (qr step).
        // kase = 4     if e(p-1) is negligible (convergence).

        for (k = p - 2; k >= -1; k--)
        {
            if (k == -1)
                break;

            if (fabs(e[k]) <=
               tiny + eps * (fabs(s[k]) + fabs(s[k + 1])))
            {
                e[k] = 0.0;
                break;
            }
        }

        if (k == p - 2)
        {
            kase = 4;
        }
        else
        {
            int ks;
            for (ks = p - 1; ks >= k; ks--)
            {
                double t;
                if (ks == k)
                    break;

                t = (ks != p ? fabs(e[ks]) : 0.0) +
                           (ks != k + 1 ? fabs(e[ks - 1]) : 0.0);
                if (fabs(s[ks]) <= tiny + eps * t)
                {
                    s[ks] = 0.0;
                    break;
                }
            }

            if (ks == k)
                kase = 3;
            else if (ks == p - 1)
                kase = 1;
            else
            {
                kase = 2;
                k = ks;
            }
        }

        k++;

        // Perform the task indicated by kase.
        switch (kase)
        {
            // Deflate negligible s(p).
            case 1:
                {
                    double f = e[p - 2];
                    e[p - 2] = 0.0;
                    for (j = p - 2; j >= k; j--)
                    {
                        double t = Hypotenuse(s[j], f);
                        double cs = s[j] / t;
                        double sn = f / t;
                        s[j] = t;
                        if (j != k)
                        {
                            f = -sn * e[j - 1];
                            e[j - 1] = cs * e[j - 1];
                        }

                        if (wantv)
                        {
                            for (i = 0; i < n; i++)
                            {
                                t = cs * v[i*vl+j] + sn * v[i*vl+p - 1];
                                v[i*vl+p - 1] = -sn * v[i*vl+j] + cs * v[i*vl+p - 1];
                                v[i*vl+j] = t;
                            }
                        }
                    }
                }
                break;

            // Split at negligible s(k).
            case 2:
                {
                    double f = e[k - 1];
                    e[k - 1] = 0.0;
                    for (j = k; j < p; j++)
                    {
                        double t = Hypotenuse(s[j], f);
                        double cs = s[j] / t;
                        double sn = f / t;
                        s[j] = t;
                        f = -sn * e[j];
                        e[j] = cs * e[j];

                        if (wantu)
                        {
                            for (i = 0; i < m; i++)
                            {
                                t = cs * u[i*ul+j] + sn * u[i*ul+k - 1];
                                u[i*ul+k - 1] = -sn * u[i*ul+j] + cs * u[i*ul+k - 1];
                                u[i*ul+j] = t;
                            }
                        }
                    }
                }
                break;

            // Perform one qr step.
            case 3:
                {
                    // Calculate the shift.
                    double scale = MAX(MAX(MAX(MAX(fabs(s[p - 1]), fabs(s[p - 2])), fabs(e[p - 2])), fabs(s[k])), fabs(e[k]));
                    double sp = s[p - 1] / scale;
                    double spm1 = s[p - 2] / scale;
                    double epm1 = e[p - 2] / scale;
                    double sk = s[k] / scale;
                    double ek = e[k] / scale;
                    double b = ((spm1 + sp) * (spm1 - sp) + epm1 * epm1) / 2.0;
                    double c = (sp * epm1) * (sp * epm1);
                    double shift = 0.0, f, g;

                    if ((b != 0.0) | (c != 0.0))
                    {
                        if (b < 0.0)
                            shift = -sqrt(b * b + c);
                        else
                            shift = sqrt(b * b + c);

                        shift = c / (b + shift);
                    }

                    f = (sk + sp) * (sk - sp) + shift;
                    g = sk * ek;

                    // Chase zeros.
                    for (j = k; j < p - 1; j++)
                    {
                        double t = Hypotenuse(f, g);
                        double cs = f / t;
                        double sn = g / t;
                        if (j != k) e[j - 1] = t;
                        f = cs * s[j] + sn * e[j];
                        e[j] = cs * e[j] - sn * s[j];
                        g = sn * s[j + 1];
                        s[j + 1] = cs * s[j + 1];

                        if (wantv)
                        {
                            
                            {
                                double* ptr_vj = &v[0*vl+j];
                                {
                                    double* vj = ptr_vj;
                                    double* vj1 = ptr_vj + 1;

                                    for (i = 0; i < n; i++)
                                    {
                                        /*t = cs * v[i*vl+j] + sn * v[i*vl+j + 1];
                                        v[i*vl+j + 1] = -sn * v[i*vl+j] + cs * v[i*vl+j + 1];
                                        v[i*vl+j] = t;*/

                                        double vij = *vj;
                                        double vij1 = *vj1;

                                        t = cs * vij + sn * vij1;
                                        *vj1 = -sn * vij + cs * vij1;
                                        *vj = t;

                                        vj += n; vj1 += n;
                                    }
                                }
                            }
                        }

                        t = Hypotenuse(f, g);
                        cs = f / t;
                        sn = g / t;
                        s[j] = t;
                        f = cs * e[j] + sn * s[j + 1];
                        s[j + 1] = -sn * e[j] + cs * s[j + 1];
                        g = sn * e[j + 1];
                        e[j + 1] = cs * e[j + 1];

                        if (wantu && (j < m - 1))
                        {
                            
                            {
                                double* ptr_uj = &u[0*ul+j];
                                {
                                    double* uj = ptr_uj;
                                    double* uj1 = ptr_uj + 1;

                                    for (i = 0; i < m; i++)
                                    {
                                        /* t = cs * u[i*ul+j] + sn * u[i*ul+j + 1];
                                         u[i*ul+j + 1] = -sn * u[i*ul+j] + cs * u[i*ul+j + 1];
                                         u[i*ul+j] = t;*/

                                        double uij = *uj;
                                        double uij1 = *uj1;

                                        t = cs * uij + sn * uij1;
                                        *uj1 = -sn * uij + cs * uij1;
                                        *uj = t;

                                        uj += nu; uj1 += nu;
                                    }
                                }
                            }
                        }

                    }

                    e[p - 2] = f;
                    iter = iter + 1;
                }
                break;

            // Convergence.
            case 4:
                {
                    // Make the singular values positive.
                    if (s[k] <= 0.0)
                    {
                        s[k] = (s[k] < 0.0 ? -s[k] : 0.0);
                        if (wantv)
                        {
                            for (i = 0; i <= pp; i++)
                                v[i*vl+k] = -v[i*vl+k];
                        }
                    }

                    // Order the singular values.
                    while (k < pp)
                    {
                        double t;
                        if (s[k] >= s[k + 1])
                            break;

                        t = s[k];
                        s[k] = s[k + 1];
                        s[k + 1] = t;
                        if (wantv && (k < n - 1)) {
                            for (i = 0; i < n; i++)
                            {
                                t = v[i*vl+k + 1];
                                v[i*vl+k + 1] = v[i*vl+k];
                                v[i*vl+k] = t;
                            }
                        }

                        if (wantu && (k < m - 1)) {
                            for (i = 0; i < m; i++)
                            {
                                t = u[i*ul+k + 1];
                                u[i*ul+k + 1] = u[i*ul+k];
                                u[i*ul+k] = t;
                            }
                        }

                        k++;
                    }

                    iter = 0;
                    p--;
                }
                break;
        }
    }

    // If we are violating JAMA's assumption about 
    // the input dimension, we need to swap u and v.
    if (swapped)
    {
        //double[,] temp = this.u; this.u = this.v; this.v = temp;
    }
    return swapped;
}

#if 0
/// <summary>
///   Solves a linear equation system of the form AX = B.
/// </summary>
/// <param name="value">Parameter B from the equation AX = B.</param>
/// <returns>The solution X from equation AX = B.</returns>
int svd_solve2(const double* value, int ml, double* r, int rl)
{
    // Additionally an important property is that if there does not exists a solution
    // when the matrix A is singular but replacing 1/Li with 0 will provide a solution
    // that minimizes the residue |AX -Y|. SVD finds the least squares best compromise
    // solution of the linear equation system. Interestingly SVD can be also used in an
    // over-determined system where the number of equations exceeds that of the parameters.

    // L is a diagonal matrix with non-negative matrix elements having the same
    // dimension as A, Wi ? 0. The diagonal elements of L are the singular values of matrix A.

    double[,] Y = value;

    // Create L*, which is a diagonal matrix with elements
    //    L*[i] = 1/L[i]  if L[i] < e, else 0, 
    // where e is the so-called singularity threshold.

    // In other words, if L[i] is zero or close to zero (smaller than e),
    // one must replace 1/L[i] with 0. The value of e depends on the precision
    // of the hardware. This method can be used to solve linear equations
    // systems even if the matrices are singular or close to singular.

    //singularity threshold
    double e = this.Threshold;


    int scols = s.Length;
    double[,] Ls = new double[scols, scols];
    for (i = 0; i < s.Length; i++)
    {
        if (fabs(s[i]) <= e)
            Ls[i, i] = 0.0;
        else Ls[i, i] = 1.0 / s[i];
    }

    //(V x L*) x Ut x Y
    double[,] VL = v.Multiply(Ls);

    //(V x L* x Ut) x Y
    int vrows = v.GetLength(0);
    int urows = u.GetLength(0);
    double[,] VLU = new double[vrows, urows];
    for (i = 0; i < vrows; i++)
    {
        for (j = 0; j < urows; j++)
        {
            double sum = 0;
            for (k = 0; k < urows; k++)
                sum += VL[i, k] * u[j*ul+k];
            VLU[i, j] = sum;
        }
    }

    //(V x L* x Ut x Y)
    VLU.Multiply(Y);
    return 0;
}

/// <summary>
///   Solves a linear equation system of the form Ax = b.
/// </summary>
/// <param name="value">The b from the equation Ax = b.</param>
/// <returns>The x from equation Ax = b.</returns>
int svd_solve(double[] value, double* Y)
{
    // Additionally an important property is that if there does not exists a solution
    // when the matrix A is singular but replacing 1/Li with 0 will provide a solution
    // that minimizes the residue |AX -Y|. SVD finds the least squares best compromise
    // solution of the linear equation system. Interestingly SVD can be also used in an
    // over-determined system where the number of equations exceeds that of the parameters.

    // L is a diagonal matrix with non-negative matrix elements having the same
    // dimension as A, Wi ? 0. The diagonal elements of L are the singular values of matrix A.

    //singularity threshold
    double e = this.Threshold;

    double[] Y = value;

    // Create L*, which is a diagonal matrix with elements
    //    L*i = 1/Li  if Li = e, else 0, 
    // where e is the so-called singularity threshold.

    // In other words, if Li is zero or close to zero (smaller than e),
    // one must replace 1/Li with 0. The value of e depends on the precision
    // of the hardware. This method can be used to solve linear equations
    // systems even if the matrices are singular or close to singular.


    int scols = s.Length;

    double[,] Ls = new double[scols, scols];
    for (i = 0; i < s.Length; i++)
    {
        if (fabs(s[i]) <= e)
            Ls[i, i] = 0;
        else Ls[i, i] = 1.0 / s[i];
    }

    //(V x L*) x Ut x Y
    double[,] VL = v.Multiply(Ls);

    //(V x L* x Ut) x Y
    int urows = u.GetLength(0);
    int vrows = v.GetLength(0);
    double[,] VLU = new double[vrows, urows];
    for (i = 0; i < vrows; i++)
    {
        for (j = 0; j < urows; j++)
        {
            double sum = 0;
            for (k = 0; k < scols; k++)
                sum += VL[i, k] * u[j*ul+k];
            VLU[i, j] = sum;
        }
    }

    //(V x L* x Ut x Y)
    return VLU.Multiply(Y);
}

/// <summary>
///   Computes the (pseudo-)inverse of the matrix given to the Singular value decomposition.
/// </summary>
double[,] svd_inverse()
{
    double e = this.Threshold;

    // X = V*S^-1
    int vrows = v.GetLength(0);
    int vcols = v.GetLength(1);
    double[,] X = new double[vrows, s.Length];
    for (i = 0; i < vrows; i++)
    {
        for (j = 0; j < vcols; j++)
        {
            if (fabs(s[j]) > e)
                X[i*xl+j] = v[i*vl+j] / s[j];
        }
    }

    // Y = X*U'
    int urows = u.GetLength(0);
    int ucols = u.GetLength(1);
    double[,] Y = new double[vrows, urows];
    for (i = 0; i < vrows; i++)
    {
        for (j = 0; j < urows; j++)
        {
            double sum = 0;
            for (k = 0; k < ucols; k++)
                sum += X[i*xl+k] * u[j*ul+k];
            Y[i*yl+j] = sum;
        }
    }

    return Y;
}

#endif
