/* This is sample from the OpenCV book. The copyright notice is below */

/* *************** License:**************************
   Oct. 3, 2008
   Right to use this code in any way you want without warranty, support or any guarantee of it working.

   BOOK: It would be nice if you cited it:
   Learning OpenCV: Computer Vision with the OpenCV Library
     by Gary Bradski and Adrian Kaehler
     Published by O'Reilly Media, October 3, 2008

   AVAILABLE AT:
     http://www.amazon.com/Learning-OpenCV-Computer-Vision-Library/dp/0596516134
     Or: http://oreilly.com/catalog/9780596516130/
     ISBN-10: 0596516134 or: ISBN-13: 978-0596516130

   OPENCV WEBSITES:
     Homepage:      http://opencv.org
     Online docs:   http://docs.opencv.org
     Q&A forum:     http://answers.opencv.org
     Issue tracker: http://code.opencv.org
     GitHub:        https://github.com/opencv/opencv/
   ************************************************** */

#include "cstd.h"
#include "str.h"
#include "cfile.h"
#include "cv/cv.inl"

static int print_help()
{
    printf(
            " Given a list of chessboard images, the number of corners (nx, ny)\n"
            " on the chessboards, and a flag: useCalibrated for \n"
            "   calibrated (0) or\n"
            "   uncalibrated \n"
            "     (1: use cvStereoCalibrate(), 2: compute fundamental\n"
            "         matrix separately) stereo. \n"
            " Calibrate the cameras and display the\n"
            " rectified results along with the computed disparity images.   \n\n");
    printf( "Usage:\n ./stereo_calib -w=<board_width default=9> -h=<board_height default=6> -s=<square_size default=1.0> <image list XML/YML file default=../data/stereo_calib.xml>\n\n");
    return 0;
}

#include "img/imgio.inl"

static void
StereoCalib(const vstr_t* imagelist, CSize boardSize, float squareSize, bool displayCorners = false, bool useCalibrated=true, bool showRectified=true)
{

    if( imagelist->n % 2 != 0 )
    {
        printf("Error: the image list contains odd (non-even) number of elements\n");
        return;
    }

    const int maxScale = 2;
    // ARRAY AND VECTOR STORAGE:
    enum {MAXIMAGE = 100};

    img_t imagePoints[2][MAXIMAGE] = {0};// FPOINT2
    CSize imageSize = {0};

    int i, j, k, nimages = (int)imagelist->n/2;
    nimages = MIN(nimages, MAXIMAGE);
    vstr_t goodImageList[1] = {0};
    i = 0;
    img_t im[10] = {0};
    img_t* img = im+i++;
    img_t* timg = im+i++;

    for( i = j = 0; i < nimages; i++ )
    {
        for( k = 0; k < 2; k++ )
        {
            const char* filename = imagelist->v[i*2+k].s;
            imread(filename, 1, 1, img);
            if(imempty(img))
                break;
            if( imageSize.h==0 || imageSize.w==0 ) {
                imageSize.h = img->h;
                imageSize.w = img->w;
            } else if( img->h != imageSize.h || img->w != imageSize.w )
            {
                printf("The image %s has the size different from the first image size. Skipping the pair\n", filename);
                break;
            }

#if 0
            bool found = false;
            img_t* corners = &imagePoints[k][j];
            for( int scale = 1; scale <= maxScale; scale++ )
            {
                if( scale == 1 ) {
                    cvCloneMat(timg, img);
                } else {
                    cvSetMat(timg, scale*img->h, scale*img->w, CC_MAT_TYPECN(img));
                    cvResize(img, timg);
                }
                found = findChessboardCorners(timg, boardSize, corners,
                    CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
                if( found )
                {
                    if( scale > 1 )
                    {
                        //img_t cornersMat(corners);                        cornersMat *= 1./scale;
                        cvConvertScale(corners, corners, 1./scale);
                    }
                    break;
                }
            }
            if( displayCorners )
            {
                cout << filename << endl;
                img_t cimg, cimg1;
                cvtColor(img, cimg, COLOR_GRAY2BGR);
                drawChessboardCorners(cimg, boardSize, corners, found);
                double sf = 640./MAX(img->rows, img->cols);
                resize(cimg, cimg1, CSize(), sf, sf);
                imshow("corners", cimg1);
                char c = (char)waitKey(500);
                if( c == 27 || c == 'q' || c == 'Q' ) //Allow ESC to quit
                    exit(-1);
            }
            else
                putchar('.');
            if( !found )
                break;
            cornerSubPix(img, corners, CSize(11,11), CSize(-1,-1),
                         TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,
                                      30, 0.01));
#endif
        }
#if 0
        if( k == 2 )
        {
            goodImageList.push_back(imagelist[i*2]);
            goodImageList.push_back(imagelist[i*2+1]);
            j++;
        }
#endif
    }
#if 0
    cout << j << " pairs have been successfully detected.\n";
    nimages = j;
    if( nimages < 2 )
    {
        cout << "Error: too little pairs to run the calibration\n";
        return;
    }

    imagePoints[0].resize(nimages);
    imagePoints[1].resize(nimages);
    vector<vector<FPOINT3> > objectPoints;
    objectPoints.resize(nimages);

    for( i = 0; i < nimages; i++ )
    {
        for( j = 0; j < boardSize.height; j++ )
            for( k = 0; k < boardSize.width; k++ )
                objectPoints[i].push_back(CPoint3f(k*squareSize, j*squareSize, 0));
    }

    cout << "Running stereo calibration ...\n";

    img_t cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = initCameraMatrix2D(objectPoints,imagePoints[0],imageSize,0);
    cameraMatrix[1] = initCameraMatrix2D(objectPoints,imagePoints[1],imageSize,0);
    img_t R, T, E, F;

    double rms = stereoCalibrate(objectPoints, imagePoints[0], imagePoints[1],
                    cameraMatrix[0], distCoeffs[0],
                    cameraMatrix[1], distCoeffs[1],
                    imageSize, R, T, E, F,
                    CALIB_FIX_ASPECT_RATIO +
                    CALIB_ZERO_TANGENT_DIST +
                    CALIB_USE_INTRINSIC_GUESS +
                    CALIB_SAME_FOCAL_LENGTH +
                    CALIB_RATIONAL_MODEL +
                    CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5,
                    TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5) );
    cout << "done with RMS error=" << rms << endl;

// CALIBRATION QUALITY CHECK
// because the output fundamental matrix implicitly
// includes all the output information,
// we can check the quality of calibration using the
// epipolar geometry constraint: m2^t*F*m1=0
    double err = 0;
    int npoints = 0;
    vector<Vec3f> lines[2];
    for( i = 0; i < nimages; i++ )
    {
        int npt = (int)imagePoints[0][i].size();
        img_t imgpt[2];
        for( k = 0; k < 2; k++ )
        {
            imgpt[k] = img_t(imagePoints[k][i]);
            undistortPoints(imgpt[k], imgpt[k], cameraMatrix[k], distCoeffs[k], img_t(), cameraMatrix[k]);
            computeCorrespondEpilines(imgpt[k], k+1, F, lines[k]);
        }
        for( j = 0; j < npt; j++ )
        {
            double errij = fabs(imagePoints[0][i][j].x*lines[1][j][0] +
                                imagePoints[0][i][j].y*lines[1][j][1] + lines[1][j][2]) +
                           fabs(imagePoints[1][i][j].x*lines[0][j][0] +
                                imagePoints[1][i][j].y*lines[0][j][1] + lines[0][j][2]);
            err += errij;
        }
        npoints += npt;
    }
    cout << "average epipolar err = " <<  err/npoints << endl;

    // save intrinsic parameters
    FileStorage fs("intrinsics.yml", FileStorage::WRITE);
    if( fs.isOpened() )
    {
        fs << "M1" << cameraMatrix[0] << "D1" << distCoeffs[0] <<
            "M2" << cameraMatrix[1] << "D2" << distCoeffs[1];
        fs.release();
    }
    else
        cout << "Error: can not save the intrinsic parameters\n";

    img_t R1, R2, P1, P2, Q;
    Rect validRoi[2];

    stereoRectify(cameraMatrix[0], distCoeffs[0],
                  cameraMatrix[1], distCoeffs[1],
                  imageSize, R, T, R1, R2, P1, P2, Q,
                  CALIB_ZERO_DISPARITY, 1, imageSize, &validRoi[0], &validRoi[1]);

    fs.open("extrinsics.yml", FileStorage::WRITE);
    if( fs.isOpened() )
    {
        fs << "R" << R << "T" << T << "R1" << R1 << "R2" << R2 << "P1" << P1 << "P2" << P2 << "Q" << Q;
        fs.release();
    }
    else
        cout << "Error: can not save the extrinsic parameters\n";

    // OpenCV can handle left-right
    // or up-down camera arrangements
    bool isVerticalStereo = fabs(P2.at<double>(1, 3)) > fabs(P2.at<double>(0, 3));

// COMPUTE AND DISPLAY RECTIFICATION
    if( !showRectified )
        return;

    img_t rmap[2][2];
// IF BY CALIBRATED (BOUGUET'S METHOD)
    if( useCalibrated )
    {
        // we already computed everything
    }
// OR ELSE HARTLEY'S METHOD
    else
 // use intrinsic parameters of each camera, but
 // compute the rectification transformation directly
 // from the fundamental matrix
    {
        vector<CPoint2f> allimgpt[2];
        for( k = 0; k < 2; k++ )
        {
            for( i = 0; i < nimages; i++ )
                std::copy(imagePoints[k][i].begin(), imagePoints[k][i].end(), back_inserter(allimgpt[k]));
        }
        F = findFundamentalMat(img_t(allimgpt[0]), img_t(allimgpt[1]), FM_8POINT, 0, 0);
        img_t H1, H2;
        stereoRectifyUncalibrated(img_t(allimgpt[0]), img_t(allimgpt[1]), F, imageSize, H1, H2, 3);

        R1 = cameraMatrix[0].inv()*H1*cameraMatrix[0];
        R2 = cameraMatrix[1].inv()*H2*cameraMatrix[1];
        P1 = cameraMatrix[0];
        P2 = cameraMatrix[1];
    }

    //Precompute maps for remap()
    initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CC_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CC_16SC2, rmap[1][0], rmap[1][1]);

    img_t canvas;
    double sf;
    int w, h;
    if( !isVerticalStereo )
    {
        sf = 600./MAX(imageSize.width, imageSize.height);
        w = cRound(imageSize.width*sf);
        h = cRound(imageSize.height*sf);
        canvas cvSetMat(h, w*2, CC_8UC3);
    }
    else
    {
        sf = 300./MAX(imageSize.width, imageSize.height);
        w = cRound(imageSize.width*sf);
        h = cRound(imageSize.height*sf);
        canvas cvSetMat(h*2, w, CC_8UC3);
    }

    for( i = 0; i < nimages; i++ )
    {
        for( k = 0; k < 2; k++ )
        {
            img_t img = imread(goodImageList[i*2+k], 0), rimg, cimg;
            remap(img, rimg, rmap[k][0], rmap[k][1], CC_INTER_LINEAR);
            cvtColor(rimg, cimg, COLOR_GRAY2BGR);
            img_t canvasPart = !isVerticalStereo ? canvas(Rect(w*k, 0, w, h)) : canvas(Rect(0, h*k, w, h));
            resize(cimg, canvasPart, canvasPart.size(), 0, 0, CC_INTER_AREA);
            if( useCalibrated )
            {
                Rect vroi(cRound(validRoi[k].x*sf), cRound(validRoi[k].y*sf),
                          cRound(validRoi[k].width*sf), cRound(validRoi[k].height*sf));
                rectangle(canvasPart, vroi, Scalar(0,0,255), 3, 8);
            }
        }

        if( !isVerticalStereo )
            for( j = 0; j < canvas->rows; j += 16 )
                line(canvas, Point(0, j), Point(canvas->cols, j), Scalar(0, 255, 0), 1, 8);
        else
            for( j = 0; j < canvas->cols; j += 16 )
                line(canvas, Point(j, 0), Point(j, canvas->rows), Scalar(0, 255, 0), 1, 8);
        imshow("rectified", canvas);
        char c = (char)waitKey();
        if( c == 27 || c == 'q' || c == 'Q' )
            break;
    }
#endif
    return ;
}

#if 0
static bool readStringList( const string& filename, vector<string>& l )
{
    l.resize(0);
    CvFileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
        return false;
    FileNode n = fs.getFirstTopLevelNode();
    if( n->tid != FileNode::SEQ )
        return false;
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
        l.push_back((string)*it);
    return true;
}

#endif

int stereo_calib(int argc, char** argv)
{
    CSize boardSize;
    char imagelistfn[256];
    BOOL showRectified, help;
    double squareSize;
    int i=1;
    char* argv2[] = {"-h","6","-w","9","-s","1.1","-nr","-@input","../data/stereo_calib.xml","-help"};
    argc = countof(argv2), argv = argv2;
    sys_chdir("C:\code\lib\opencv-3.2.0\samples\data");
    cvCommandLineParser(argc, argv, "{w|d|9|}{h|d|6|}{s|f|1.0|}{nr|d||}{help|h||}{@input|s|../data/stereo_calib.xml|}",
      &boardSize.width, &boardSize.height, &squareSize, &showRectified, &help, imagelistfn);
    if (help) {
      //return 0;
    }

    vstr_t imagelist[1] = {0};
    vstr_load(imagelistfn, imagelist);
#if 0
    bool ok = readStringList(imagelistfn, imagelist);
    if(!ok || imagelist.empty())
    {
        cout << "can not open " << imagelistfn << " or the string list is empty" << endl;
        return print_help();
    }

#endif
    StereoCalib(imagelist, boardSize, squareSize, false, true, showRectified);
    vstr_free(imagelist);
    return 0;
}
