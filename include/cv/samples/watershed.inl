
#include "img/imgio.inl"

static void help()
{
    printf("\nThis program demonstrates the famous watershed segmentation algorithm in OpenCV: watershed()\n"
            "Usage:\n"
            "./watershed [image_name -- default is ../data/fruits.jpg]\n\n");


    printf("Hot keys: \n"
        "\tESC - quit the program\n"
        "\tr - restore the original image\n"
        "\tw or SPACE - run watershed segmentation algorithm\n"
        "\t\t(before running it, *roughly* mark the areas to segment on the image)\n"
        "\t  (before that, roughly outline several markers on the image)\n");
}

img_t markerMask[1] = {0}, img[1] = {0};
CPoint prevPt = {-1, -1};

static void onMouse( int event, int x, int y, int flags, void* )
{
    if( x < 0 || x >= img->w || y < 0 || y >= img->h )
        return;
    if( event == MSG_LBUTTONUP || !(flags & MSG_FLAG_LBUTTON) )
        prevPt = cPoint(-1,-1);
    else if( event == MSG_LBUTTONDOWN )
        prevPt = cPoint(x,y);
    else if( event == MSG_MOUSEMOVE && (flags & MSG_FLAG_LBUTTON) )
    {
        CPoint pt = {x, y};
        if( prevPt.x < 0 )
            prevPt = pt;
        cvLine( markerMask, prevPt, pt, cScalarAll(255), 5, 8, 0 );
        cvLine( img, prevPt, pt, cScalarAll(255), 5, 8, 0 );
        prevPt = pt;
        imshow2("image", img);
    }
}

int watershed( int argc, char** argv )
{
    int hashelp = 0;
    char filename[256] = "";
    cvCommandLineParser(argc, argv, "{help h |d| | }{ @input|s | ../data/fruits.jpg | }", &hashelp, filename);
    if (hashelp)
    {
        help();
        return 0;
    }
    int i = 0;
    img_t im[10] = {0};
    img_t* img0 = im+i++;
    img_t* imgGray = im+i++;
    
    imread(filename, 3, 1, img0);

    if( imempty(img0) )
    {
        printf("Couldn'g open image %s. Usage: watershed <image_name>\n", filename);
        return 0;
    }
    help();
    cvNamedWindow( "image", 1 );

    cvCloneMat(img, img0);
    //cvCvtColor(img, markerMask, COLOR_BGR2GRAY);
    //cvtColor(markerMask, imgGray, COLOR_GRAY2BGR);
    imcolorcvt(img, markerMask, T_RGB, T_GRAY);
    imcolorcvt(markerMask, imgGray, T_RGB, T_GRAY);
    //markerMask = Scalar::all(0);
    cvZero(markerMask);
    imshow2( "image", img );
    cvSetMouseCallback( "image", onMouse, 0 );

#if 0
    for(;;)
    {
        char c = (char)waitKey(0);

        if( c == 27 )
            break;

        if( c == 'r' )
        {
            markerMask = Scalar::all(0);
            img0.copyTo(img);
            imshow( "image", img );
        }

        if( c == 'w' || c == ' ' )
        {
            int i, j, compCount = 0;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            findContours(markerMask, contours, hierarchy, CC_RETR_CCOMP, CC_CHAIN_APPROX_SIMPLE);

            if( contours.empty() )
                continue;
            img_t markers(markerMask.size(), CC_32S);
            markers = Scalar::all(0);
            int idx = 0;
            for( ; idx >= 0; idx = hierarchy[idx][0], compCount++ )
                drawContours(markers, contours, idx, Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);

            if( compCount == 0 )
                continue;

            vector<Vec3b> colorTab;
            for( i = 0; i < compCount; i++ )
            {
                int b = theRNG().uniform(0, 255);
                int g = theRNG().uniform(0, 255);
                int r = theRNG().uniform(0, 255);

                colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
            }

            double t = (double)cGetTickCount();
            watershed( img0, markers );
            t = (double)cGetTickCount() - t;
            printf( "execution time = %gms\n", t*1000./getTickFrequency() );

            img_t wshed(markers.size(), CC_8UC3);

            // paint the watershed image
            for( i = 0; i < markers->h; i++ )
                for( j = 0; j < markers->w; j++ )
                {
                    int index = markers.at<int>(i,j);
                    if( index == -1 )
                        wshed.at<Vec3b>(i,j) = Vec3b(255,255,255);
                    else if( index <= 0 || index > compCount )
                        wshed.at<Vec3b>(i,j) = Vec3b(0,0,0);
                    else
                        wshed.at<Vec3b>(i,j) = colorTab[index - 1];
                }

            wshed = wshed*0.5 + imgGray*0.5;
            imshow( "watershed transform", wshed );
        }
    }

#endif
    return 0;
}
