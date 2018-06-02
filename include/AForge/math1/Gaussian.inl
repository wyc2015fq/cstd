// AForge Math Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © Andrew Kirillov, 2005-2009
// andrew.kirillov@aforgenet.com
//

/// <summary>
/// Gaussian function.
/// </summary>
/// 
/// <remarks><para>The class is used to calculate 1D and 2D Gaussian functions for
/// specified <see cref="Sigma"/> (s) value:</para>
/// 
/// <code lang="none">
/// 1-D: f(x) = exp( x * x / ( -2 * s * s ) ) / ( s * sqrt( 2 * PI ) )
/// 
/// 2-D: f(x, y) = exp( x * x + y * y / ( -2 * s * s ) ) / ( s * s * 2 * PI )
/// </code>
/// 
/// </remarks>
/// 

// sigma value
//double sigma = 1.0;
// squared sigma
//double sqrSigma = 1.0;

/// <summary>
/// Sigma value.
/// </summary>
/// 
/// <remarks><para>Sigma property of Gaussian function.</para>
/// 
/// <para>Default value is set to <b>1</b>. Minimum allowed value is <b>0.00000001</b>.</para>
/// </remarks>
/// 

/// <summary>
/// 1-D Gaussian function.
/// </summary>
/// 
/// <param name="x">x value.</param>
/// 
/// <returns>Returns function's value at point <paramref name="x"/>.</returns>
/// 
/// <remarks><para>The function calculates 1-D Gaussian function:</para>
/// 
/// <code lang="none">
/// f(x) = exp( x * x / ( -2 * s * s ) ) / ( s * sqrt( 2 * PI ) )
/// </code>
/// </remarks>
/// CV_PI
#define GAUSSIAN_FUNCTION( x )  (exp( x * x / ( -2 * sqrSigma ) ) / ( sqrt( 2 * CV_PI ) * sigma ))


/// <summary>
/// 2-D Gaussian function.
/// </summary>
/// 
/// <param name="x">x value.</param>
/// <param name="y">y value.</param>
/// 
/// <returns>Returns function's value at point (<paramref name="x"/>, <paramref name="y"/>).</returns>
/// 
/// <remarks><para>The function calculates 2-D Gaussian function:</para>
/// 
/// <code lang="none">
/// f(x, y) = exp( x * x + y * y / ( -2 * s * s ) ) / ( s * s * 2 * PI )
/// </code>
/// </remarks>
/// 
#define GAUSSIAN_FUNCTION2D( x, y )   (exp( ( x * x + y * y ) / ( -2 * sqrSigma ) ) / ( 2 * CV_PI * sqrSigma ))

/// <summary>
/// 1-D Gaussian kernel.
/// </summary>
/// 
/// <param name="size">Kernel size (should be odd), [3, 101].</param>
/// 
/// <returns>Returns 1-D Gaussian kernel of the specified size.</returns>
/// 
/// <remarks><para>The function calculates 1-D Gaussian kernel, which is array
/// of Gaussian function's values in the [-r, r] range of x value, where
/// r=floor(<paramref name="size"/>/2).
/// </para></remarks>
/// 
/// <exception cref="ArgumentException">Wrong kernel size.</exception>
/// 
int Gaussian_Kernel( double sigma, int size, double* kernel )
{
    int r, x, i;
    double sqrSigma;
    // check for evem size and for out of range
    if ( ( ( size % 2 ) == 0 ) || ( size < 3 ) || ( size > 101 ) )
    {
        ASSERT( 0 && "Wrong kernal size." );
    }

    // raduis
    r = size / 2;
    // kernel
    sigma = MAX( 0.00000001, sigma );
    sqrSigma = sigma * sigma;

    // compute kernel
    for (x = -r, i = 0; i < size; x++, i++ )
    {
        kernel[i] = GAUSSIAN_FUNCTION( x );
    }

    return size;
}

/// <summary>
/// 2-D Gaussian kernel.
/// </summary>
/// 
/// <param name="size">Kernel size (should be odd), [3, 101].</param>
/// 
/// <returns>Returns 2-D Gaussian kernel of specified size.</returns>
/// 
/// <remarks><para>The function calculates 2-D Gaussian kernel, which is array
/// of Gaussian function's values in the [-r, r] range of x,y values, where
/// r=floor(<paramref name="size"/>/2).
/// </para></remarks>
/// 
/// <exception cref="ArgumentException">Wrong kernel size.</exception>
/// 
int Gaussian_Kernel2D( double sigma, int size, double* kernel, int kl )
{
    int r, x, y, i, j;
    double sqrSigma;
    // check for evem size and for out of range
    if ( ( ( size % 2 ) == 0 ) || ( size < 3 ) || ( size > 101 ) )
    {
        ASSERT( 0 && "Wrong kernal size." );
    }

    // raduis
    r = size / 2;
    // kernel
    sigma = MAX( 0.00000001, sigma );
    sqrSigma = sigma * sigma;

    // compute kernel
    for (y = -r, i = 0; i < size; y++, i++ )
    {
        for (x = -r, j = 0; j < size; x++, j++ )
        {
            kernel[i*kl+j] = GAUSSIAN_FUNCTION2D( x, y );
        }
    }

    return size*size;
}


