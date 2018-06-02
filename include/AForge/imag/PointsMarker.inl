
/// <summary>
///   Filter to mark (highlight) points in a image.
/// </summary>
/// 
/// <remarks>
/// <para>The filter highlights points on the image using a given set of points.</para>
/// 
/// <para>The filter accepts 8 bpp grayscale and 24 color images for processing.</para>
/// </remarks>
/// 
int PointsMarker(img_t* im, int n, const IPOINT* points)
{
    int i;
    // mark all points
    for (i=0; i<n; ++i)
    {
        imdrawaa_circle(im, points[i].x, points[i].y, 3, White, 0, 0);
    }
    return 0;
}
