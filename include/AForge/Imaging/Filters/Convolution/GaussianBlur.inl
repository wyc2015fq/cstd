

#include "math1\Gaussian.inl"
/// <summary>
/// Gaussian blur filter.
/// </summary>
/// 
/// <remarks><para>The filter performs <see cref="Convolution">convolution filter</see> using
/// the kernel, which is calculate with the help of <see cref="AForge.Math.Gaussian.Kernel2D"/>
/// method and then converted to integer kernel by dividing all elements by the element with the
/// smallest value. Using the kernel the convolution filter is known as Gaussian blur.</para>
/// 
/// <para>Using <see cref="Sigma"/> property it is possible to configure
/// <see cref="AForge.Math.Gaussian.Sigma">sigma value of Gaussian function</see>.</para>
/// 
/// <para>For the list of supported pixel formats, see the documentation to <see cref="Convolution"/>
/// filter.</para>
/// 
/// <para><note>By default this filter sets <see cref="Convolution.ProcessAlpha"/> property to
/// <see langword="true"/>, so the alpha channel of 32 bpp and 64 bpp images is blurred as well.
/// </note></para>
/// 
/// <para>Sample usage:</para>
/// <code>
/// // create filter with kernel size equal to 11
/// // and Gaussia sigma value equal to 4.0
/// GaussianBlur filter = new GaussianBlur( 4, 11 );
/// // apply the filter
/// filter.ApplyInPlace( image );
/// </code>
///
/// <para><b>Initial image:</b></para>
/// <img src="img/imaging/sample1.jpg" width="480" height="361" />
/// <para><b>Result image:</b></para>
/// <img src="img/imaging/gaussian_blur.jpg" width="480" height="361" />
/// </remarks>
/// 
/// <seealso cref="Convolution"/>
/// 
/// <summary>
/// Initializes a new instance of the <see cref="GaussianBlur"/> class.
/// </summary>
/// 
/// <param name="sigma">Gaussian sigma value.</param>
/// <param name="size">Kernel size.</param>
/// 

int GaussianBlur(double sigma, int size, int* intKernel)
{
    int i, j;
    // create Gaussian kernel
    double kernel[21*21]={0};
    double min;
    // integer kernel
    //int intKernel[21*21]={0};
    int divisor = 0;
    int kl=size;

    //double sigma = 1.4;
    //int    size = 5;
    bool ProcessAlpha = true;

    /// <summary>
    /// Gaussian sigma value, [0.5, 5.0].
    /// </summary>
    /// 
    /// <remarks><para>Sigma value for Gaussian function used to calculate
    /// the kernel.</para>
    /// 
    /// <para>Default value is set to <b>1.4</b>.</para>
    /// </remarks>
    /// 
    // get new sigma value
    sigma = MAX( 0.5, MIN( 5.0, sigma ) );

    /// <summary>
    /// Kernel size, [3, 21].
    /// </summary>
    /// 
    /// <remarks><para>Size of Gaussian kernel.</para>
    /// 
    /// <para>Default value is set to <b>5</b>.</para>
    /// </remarks>
    /// 
    size = MAX( 3, MIN( 21, size | 1 ) );

    // Create Gaussian filter
    
    Gaussian_Kernel2D( sigma, size, kernel, kl );
    min = kernel[0];

    for (i = 0; i < size; i++ )
    {
        for (j = 0; j < size; j++ )
        {
            double v = kernel[i*kl+j] / min;

            if ( v > USHRT_MAX )
            {
                v = USHRT_MAX;
            }
            intKernel[i*kl+j] = (int) v;

            // collect divisor
            divisor += intKernel[i*kl+j];
        }
    }

    // update filter
    //this.Kernel = intKernel;
    //this.Divisor = divisor;

    return divisor;
}

