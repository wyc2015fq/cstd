// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright ? Andrew Kirillov, 2005-2010
// andrew.kirillov@aforgenet.com
//

namespace AForge.Imaging.Textures
{
    using System;

    /// <summary>
    /// Marble texture.
    /// </summary>
    /// 
    /// <remarks><para>The texture generator creates textures with effect of marble.
    /// The <see cref="XPeriod"/> and <see cref="YPeriod"/> properties allow to control the look
    /// of marble texture in X/Y directions.</para>
    /// 
    /// <para>The generator is based on the <see cref="AForge.Math.PerlinNoise">Perlin noise function</see>.</para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // create texture generator
    /// MarbleTexture textureGenerator = new MarbleTexture( );
    /// // generate new texture
    /// float[,] texture = textureGenerator.Generate( 320, 240 );
    /// // convert it to image to visualize
    /// Bitmap textureImage = TextureTools.ToBitmap( texture );
    /// </code>
    ///
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/marble_texture.jpg" width="320" height="240" />
    /// </remarks>
    /// 
    public class MarbleTexture : ITextureGenerator
    {
        // Perlin noise function used for texture generation
        private AForge.Math.PerlinNoise noise = new AForge.Math.PerlinNoise( 2, 0.65, 1.0 / 32, 1.0 );

        // randmom number generator
        private Random rand = new Random( );
        private int		r;

        private double	xPeriod = 5.0;
        private double	yPeriod = 10.0;

        /// <summary>
        /// X period value, ¡Ý 2.
        /// </summary>
        /// 
        /// <remarks>Default value is set to <b>5</b>.</remarks>
        /// 
        public double XPeriod
        {
            get { return xPeriod; }
            set { xPeriod = MAX( 2.0, value ); }
        }

        /// <summary>
        /// Y period value, ¡Ý 2.
        /// </summary>
        /// 
        /// <remarks>Default value is set to <b>10</b>.</remarks>
        /// 
        public double YPeriod
        {
            get { return yPeriod; }
            set { yPeriod = MAX( 2.0, value ); }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MarbleTexture"/> class.
        /// </summary>
        /// 
        public MarbleTexture( )
        {
            Reset( );
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MarbleTexture"/> class.
        /// </summary>
        /// 
        /// <param name="xPeriod">X period value.</param>
        /// <param name="yPeriod">Y period value.</param>
        /// 
        public MarbleTexture( double xPeriod, double yPeriod )
        {
            this.xPeriod = xPeriod;
            this.yPeriod = yPeriod;
            Reset( );
        }

        /// <summary>
        /// Generate texture.
        /// </summary>
        /// 
        /// <param name="width">Texture's width.</param>
        /// <param name="height">Texture's height.</param>
        /// 
        /// <returns>Two dimensional array of intensities.</returns>
        /// 
        /// <remarks>Generates new texture of the specified size.</remarks>
        /// 
        public float[,] Generate( int width, int height )
        {
            float[,]	texture = new float[height, width];
            double		xFact = xPeriod / width;
            double		yFact = yPeriod / height;

            for (y = 0; y < height; y++ )
            {
                for (x = 0; x < width; x++ )
                {
                    texture[y, x] =
                        MIN( 1.0f, (float)
                            fabs( Math.Sin(
                                ( x * xFact + y * yFact + noise.Function2D( x + r, y + r ) ) * CV_PI
                            ) )
                        );

                }
            }
            return texture;
        }

        /// <summary>
        /// Reset generator.
        /// </summary>
        /// 
        /// <remarks>Regenerates internal random numbers.</remarks>
        ///  
        public void Reset( )
        {
            r = rand.Next( 5000 );
        }
    }
}
