/*
 * gdal_image.cpp -- Load GIS data into OpenCV Containers using the Geospatial Data Abstraction Library
*/

// OpenCV Headers
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

// C++ Standard Libraries
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

// define the corner points
//    Note that GDAL library can natively determine this
Point2d tl( -122.441017, 37.815664 );
Point2d tr( -122.370919, 37.815311 );
Point2d bl( -122.441533, 37.747167 );
Point2d br( -122.3715,   37.746814 );

// determine dem corners
Point2d dem_bl( -122.0, 38);
Point2d dem_tr( -123.0, 37);

// range of the heat map colors
std::vector<std::pair<Vec3b,double> > color_range;


// List of all function prototypes
Point2d lerp( const Point2d&, const Point2d&, const double& );

Vec3b get_dem_color( const double& );

Point2d world2dem( const Point2d&, const CvSize&);

Point2d pixel2world( const int&, const int&, const CvSize& );

void add_color( Vec3b& pix, const uchar& b, const uchar& g, const uchar& r );



/*
 * Linear Interpolation
 * p1 - Point 1
 * p2 - Point 2
 * t  - Ratio from Point 1 to Point 2
*/
Point2d lerp( Point2d const& p1, Point2d const& p2, const double& t ){
    return Point2d( ((1-t)*p1.x) + (t*p2.x),
                        ((1-t)*p1.y) + (t*p2.y));
}

/*
 * Interpolate Colors
*/
template <typename DATATYPE, int N>
Vec<DATATYPE,N> lerp( Vec<DATATYPE,N> const& minColor,
                          Vec<DATATYPE,N> const& maxColor,
                          double const& t ){

    Vec<DATATYPE,N> output;
    for( int i=0; i<N; i++ ){
        output[i] = (uchar)(((1-t)*minColor[i]) + (t * maxColor[i]));
    }
    return output;
}

/*
 * Compute the dem color
*/
Vec3b get_dem_color( const double& elevation ){

    // if the elevation is below the minimum, return the minimum
    if( elevation < color_range[0].second ){
        return color_range[0].first;
    }
    // if the elevation is above the maximum, return the maximum
    if( elevation > color_range.back().second ){
        return color_range.back().first;
    }

    // otherwise, find the proper starting index
    int idx=0;
    double t = 0;
    for( int x=0; x<(int)(color_range.size()-1); x++ ){

        // if the current elevation is below the next item, then use the current
        // two colors as our range
        if( elevation < color_range[x+1].second ){
            idx=x;
            t = (color_range[x+1].second - elevation)/
                (color_range[x+1].second - color_range[x].second);

            break;
        }
    }

    // interpolate the color
    return lerp( color_range[idx].first, color_range[idx+1].first, t);
}

/*
 * Given a pixel coordinate and the size of the input image, compute the pixel location
 * on the DEM image.
*/
Point2d world2dem( Point2d const& coordinate, const CvSize& dem_size   ){


    // relate this to the dem points
    // ASSUMING THAT DEM DATA IS ORTHORECTIFIED
    double demRatioX = ((dem_tr.x - coordinate.x)/(dem_tr.x - dem_bl.x));
    double demRatioY = 1-((dem_tr.y - coordinate.y)/(dem_tr.y - dem_bl.y));

    Point2d output;
    output.x = demRatioX * dem_size.width;
    output.y = demRatioY * dem_size.height;

    return output;
}

/*
 * Convert a pixel coordinate to world coordinates
*/
Point2d pixel2world( const int& x, const int& y, const CvSize& size ){

    // compute the ratio of the pixel location to its dimension
    double rx = (double)x / size.width;
    double ry = (double)y / size.height;

    // compute LERP of each coordinate
    Point2d rightSide = lerp(tr, br, ry);
    Point2d leftSide  = lerp(tl, bl, ry);

    // compute the actual Lat/Lon coordinate of the interpolated coordinate
    return lerp( leftSide, rightSide, rx );
}

/*
 * Add color to a specific pixel color value
*/
void add_color( Vec3b& pix, const uchar& b, const uchar& g, const uchar& r ){

    if( pix[0] + b < 255 && pix[0] + b >= 0 ){ pix[0] += b; }
    if( pix[1] + g < 255 && pix[1] + g >= 0 ){ pix[1] += g; }
    if( pix[2] + r < 255 && pix[2] + r >= 0 ){ pix[2] += r; }
}


/*
 * Main Function
*/
int main( int argc, char* argv[] ){

    /*
     * Check input arguments
    */
    if( argc < 3 ){
        cout << "usage: " << argv[0] << " <image_name> <dem_model_name>" << endl;
        return -1;
    }

    // load the image (note that we don't have the projection information.  You will
    // need to load that yourself or use the full GDAL driver.  The values are pre-defined
    // at the top of this file
    //![load1]
    CvMat image = imread(argv[1], IMREAD_LOAD_GDAL | IMREAD_COLOR );
    //![load1]

    //![load2]
    // load the dem model
    CvMat dem = imread(argv[2], IMREAD_LOAD_GDAL | IMREAD_ANYDEPTH );
    //![load2]

    // create our output products
    CvMat output_dem(   image.size(), CC_8UC3 );
    CvMat output_dem_flood(   image.size(), CC_8UC3 );

    // for sanity sake, make sure GDAL Loads it as a signed short
    if( dem->tid != CC_16SC1 ){ throw std::runtime_error("DEM image type must be CC_16SC1"); }

    // define the color range to create our output DEM heat map
    //  Pair format ( Color, elevation );  Push from low to high
    //  Note:  This would be perfect for a configuration file, but is here for a working demo.
    color_range.push_back( std::pair<Vec3b,double>(Vec3b( 188, 154,  46),   -1));
    color_range.push_back( std::pair<Vec3b,double>(Vec3b( 110, 220, 110), 0.25));
    color_range.push_back( std::pair<Vec3b,double>(Vec3b( 150, 250, 230),   20));
    color_range.push_back( std::pair<Vec3b,double>(Vec3b( 160, 220, 200),   75));
    color_range.push_back( std::pair<Vec3b,double>(Vec3b( 220, 190, 170),  100));
    color_range.push_back( std::pair<Vec3b,double>(Vec3b( 250, 180, 140),  200));

    // define a minimum elevation
    double minElevation = -10;

    // iterate over each pixel in the image, computing the dem point
    for( int y=0; y<image.rows; y++ ){
    for( int x=0; x<image.cols; x++ ){

        // convert the pixel coordinate to lat/lon coordinates
        Point2d coordinate = pixel2world( x, y, image.size() );

        // compute the dem image pixel coordinate from lat/lon
        Point2d dem_coordinate = world2dem( coordinate, dem.size() );

        // extract the elevation
        double dz;
        if( dem_coordinate.x >=    0    && dem_coordinate.y >=    0     &&
            dem_coordinate.x < dem.cols && dem_coordinate.y < dem.rows ){
            dz = dem.at<short>(dem_coordinate);
        }else{
            dz = minElevation;
        }

        // write the pixel value to the file
        output_dem_flood.at<Vec3b>(y,x) = image.at<Vec3b>(y,x);

        // compute the color for the heat map output
        Vec3b actualColor = get_dem_color(dz);
        output_dem.at<Vec3b>(y,x) = actualColor;

        // show effect of a 10 meter increase in ocean levels
        if( dz < 10 ){
            add_color( output_dem_flood.at<Vec3b>(y,x), 90, 0, 0 );
        }
        // show effect of a 50 meter increase in ocean levels
        else if( dz < 50 ){
            add_color( output_dem_flood.at<Vec3b>(y,x), 0, 90, 0 );
        }
        // show effect of a 100 meter increase in ocean levels
        else if( dz < 100 ){
            add_color( output_dem_flood.at<Vec3b>(y,x), 0, 0, 90 );
        }

    }}

    // print our heat map
    imwrite( "heat-map.jpg"   ,  output_dem );

    // print the flooding effect image
    imwrite( "flooded.jpg",  output_dem_flood);

    return 0;
}
