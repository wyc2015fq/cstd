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
    /// Wood texture.
    /// </summary>
    /// 
    /// <remarks><para>The texture generator creates textures with effect of
    /// rings on trunk's shear. The <see cref="Rings"/> property allows to specify the
    /// desired amount of wood rings.</para>
    /// 
    /// <para>The generator is based on the <see cref="AForge.Math.PerlinNoise">Perlin noise function</see>.</para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // create texture generator
    /// WoodTexture textureGenerator = new WoodTexture( );
    /// // generate new texture
    /// float[,] texture = textureGenerator.Generate( 320, 240 );
    /// // convert it to image to visualize
    /// Bitmap textureImage = TextureTools.ToBitmap( texture );
    /// </code>
    ///
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/wood_texture.jpg" width="320" height="240" />
    /// </remarks>
    /// 
    public class WoodTexture : ITextureGenerator
    {
        // Perlin noise function used for texture generation
        private AForge.Math.PerlinNoise	noise = new AForge.Math.PerlinNoise( 8, 0.5, 1.0 / 32, 0.05 );

        // randmom number generator
        private Random	rand = new Random( );
        private int		r;

        // rings amount
        private double	rings = 12;

        /// <summary>
        /// Wood rings amount, ¡Ý 3.
        /// </summary>
        /// 
        /// <remarks><para>The property sets the amount of wood rings, which make effect of
        /// rings on trunk's shear.</para>
        /// 
        /// <para>Default value is set to <b>12</b>.</para></remarks>
        /// 
        public double Rings
        {
            get { return rings; }
            set { rings = MAX( 3, value ); }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="WoodTexture"/> class.
        /// </summary>
        /// 
        public WoodTexture( )
        {
            Reset( );
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="WoodTexture"/> class.
        /// </summary>
        /// 
        /// <param name="rings">Wood rings amount.</param>
        /// 
        public WoodTexture( double rings )
        {
            this.rings = rings;
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
            int			w2 = width / 2;
            int			h2 = height / 2;

            for (y = 0; y < height; y++ )
            {
                for (x = 0; x < width; x++ )
                {
                    double xv = (double) ( x - w2 ) / width;
                    double yv = (double) ( y - h2 ) / height;

                    texture[y, x] =
                        MIN( 1.0f, (float)
                        fabs( Math.Sin(
                            ( sqrt( xv * xv + yv * yv ) + noise.Function2D( x + r, y + r ) )
                                * CV_PI * 2 * rings
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
