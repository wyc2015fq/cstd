/* ../../../dependencies/lapack/src/zgetf2.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/




/* Subroutine */ static int zgetf2_(integer *m, integer *n, dcomplex *a, 
        integer *lda, integer *ipiv, integer *info)
        {
          /* Table of constant values */
          
          static dcomplex c_b1 = {1.,0.};
static integer c__1 = 1;

    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    dcomplex z__1;

    /* Builtin functions */
    void z_div(dcomplex *, dcomplex *, dcomplex *);

    /* Local variables */
    static integer j, jp;
     /* Subroutine */ int zscal_(integer *, dcomplex *, 
            dcomplex *, integer *), zgeru_(integer *, integer *, 
            dcomplex *, dcomplex *, integer *, dcomplex *, 
            integer *, dcomplex *, integer *), zswap_(integer *, 
            dcomplex *, integer *, dcomplex *, integer *), xerbla_(
            char *, integer *, ftnlen);
     integer izamax_(integer *, dcomplex *, integer *);


/*  -- LAPACK routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     September 30, 1994 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  ZGETF2 computes an LU factorization of a general m-by-n matrix A */
/*  using partial pivoting with row interchanges. */

/*  The factorization has the form */
/*     A = P * L * U */
/*  where P is a permutation matrix, L is lower triangular with unit */
/*  diagonal elements (lower trapezoidal if m > n), and U is upper */
/*  triangular (upper trapezoidal if m < n). */

/*  This is the right-looking Level 2 BLAS version of the algorithm. */

/*  Arguments */
/*  ========= */

/*  M       (input) INTEGER */
/*          The number of rows of the matrix A.  M >= 0. */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix A.  N >= 0. */

/*  A       (input/output) COMPLEX*16 array, dimension (LDA,N) */
/*          On entry, the m by n matrix to be factored. */
/*          On exit, the factors L and U from the factorization */
/*          A = P*L*U; the unit diagonal elements of L are not stored. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= MAX(1,M). */

/*  IPIV    (output) INTEGER array, dimension (MIN(M,N)) */
/*          The pivot indices; for 1 <= i <= MIN(M,N), row i of the */
/*          matrix was interchanged with row IPIV(i). */

/*  INFO    (output) INTEGER */
/*          = 0: successful exit */
/*          < 0: if INFO = -k, the k-th argument had an illegal value */
/*          > 0: if INFO = k, U(k,k) is exactly zero. The factorization */
/*               has been completed, but the factor U is exactly */
/*               singular, and division by zero will occur if it is used */
/*               to solve a system of equations. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --ipiv;

    /* Function Body */
    *info = 0;
    if (*m < 0) {
        *info = -1;
    } else if (*n < 0) {
        *info = -2;
    } else if (*lda < MAX(1,*m)) {
        *info = -4;
    }
    if (*info != 0) {
        i__1 = -(*info);
        xerbla_("ZGETF2", &i__1, (ftnlen)6);
        return 0;
    }

/*     Quick return if possible */

    if (*m == 0 || *n == 0) {
        return 0;
    }

    i__1 = MIN(*m,*n);
    for (j = 1; j <= i__1; ++j) {

/*        Find pivot and test for singularity. */

        i__2 = *m - j + 1;
        jp = j - 1 + izamax_(&i__2, &a[j + j * a_dim1], &c__1);
        ipiv[j] = jp;
        i__2 = jp + j * a_dim1;
        if (a[i__2].r != 0. || a[i__2].i != 0.) {

/*           Apply the interchange to columns 1:N. */

            if (jp != j) {
                zswap_(n, &a[j + a_dim1], lda, &a[jp + a_dim1], lda);
            }

/*           Compute elements J+1:M of J-th column. */

            if (j < *m) {
                i__2 = *m - j;
                z_div(&z__1, &c_b1, &a[j + j * a_dim1]);
                zscal_(&i__2, &z__1, &a[j + 1 + j * a_dim1], &c__1);
            }

        } else if (*info == 0) {

            *info = j;
        }

        if (j < MIN(*m,*n)) {

/*           Update trailing submatrix. */

            i__2 = *m - j;
            i__3 = *n - j;
            z__1.r = -1., z__1.i = -0.;
            zgeru_(&i__2, &i__3, &z__1, &a[j + 1 + j * a_dim1], &c__1, &a[j + 
                    (j + 1) * a_dim1], lda, &a[j + 1 + (j + 1) * a_dim1], lda)
                    ;
        }
/* L10: */
    }
    return 0;

/*     End of ZGETF2 */

} /* zgetf2_ */

