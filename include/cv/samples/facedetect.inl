
#include "cfile.h"
#include "zlib/libzlib.inl"

static int zstream_eof(stream_t* s)
{
  return gzeof((gzFile)s->x);
}
static int zstream_close(stream_t* s)
{
  gzFile f = (gzFile)s->x;
  bzero(s, sizeof(stream_t));
  return gzclose(f);
}
static int zstream_tell(stream_t* s)
{
  return gztell((gzFile)s->x);
}
static int zstream_get8(stream_t* s)
{
  return gzgetc((gzFile)s->x);
}
static char* zstream_gets(stream_t* s, char* buf, int bufsize)
{
  return gzgets((gzFile)s->x, buf, bufsize);
}
static int zstream_unget8(int c, stream_t* s)
{
  return gzungetc(c, (gzFile)s->x);
}
static int zstream_put8(stream_t* s, int c)
{
  return gzputc((gzFile)s->x, c);
}
static int zstream_seek(stream_t* s, int offset, int origin)
{
  return gzseek((gzFile)s->x, offset, origin);
}
static int zstream_read(stream_t* s, void* buf, int len)
{
  return gzread((gzFile)s->x, buf, len);
}
static int zstream_write(stream_t* s, const void* buf, int len)
{
  return gzwrite((gzFile)s->x, buf, len);
}
// fstream_init
int zstreams_init(stream_t* s, const char* filename, bool write_mode, bool compression) {
  char mode[] = { write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0' };
  s->x = gzopen(filename, mode);
  stream_init_func(s, z);
  return s->x!=NULL;
}

#include "test/test_mem_block.inl"

int facedetect() {
  int i=1, j=1;
  double a[10] = {1.0, 1.1, 1.2, 1.3};
  int aa = i + j[a];
test_mem_block();

  return 0;
}
#if 0
void detectAndDraw( img_t& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip );

string cascadeName;
string nestedCascadeName;

int facedetect( int argc, const char** argv )
{
    VideoCapture capture;
    img_t frame, image;
    string inputName;
    bool tryflip;
    CascadeClassifier cascade, nestedCascade;
    double scale;

    CommandLineParser parser(argc, argv,
        "{help h||}"
        "{cascade|../../data/haarcascades/haarcascade_frontalface_alt.xml|}"
        "{nested-cascade|../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|}"
        "{scale|1|}{try-flip||}{@filename||}"
    );
    if (parser.has("help"))
    {
    cout << "\nThis program demonstrates the cascade recognizer. Now you can use Haar or LBP features.\n"
            "This classifier can recognize many kinds of rigid objects, once the appropriate classifier is trained.\n"
            "It's most known use is for faces.\n"
            "Usage:\n"
            "./facedetect [--cascade=<cascade_path> this is the primary trained classifier such as frontal face]\n"
               "   [--nested-cascade[=nested_cascade_path this an optional secondary classifier such as eyes]]\n"
               "   [--scale=<image scale greater or equal to 1, try 1.3 for example>]\n"
               "   [--try-flip]\n"
               "   [filename|camera_index]\n\n"
            "see facedetect.cmd for one call:\n"
            "./facedetect --cascade=\"../../data/haarcascades/haarcascade_frontalface_alt.xml\" --nested-cascade=\"../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml\" --scale=1.3\n\n"
            "During execution:\n\tHit any key to quit.\n"
            "\tUsing OpenCV version " << CC_VERSION << "\n" << endl;
        return 0;
    }
    cascadeName = parser.get<string>("cascade");
    nestedCascadeName = parser.get<string>("nested-cascade");
    scale = parser.get<double>("scale");
    if (scale < 1)
        scale = 1;
    tryflip = parser.has("try-flip");
    inputName = parser.get<string>("@filename");
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }
    if ( !nestedCascade.load( nestedCascadeName ) )
        cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        help();
        return -1;
    }
    if( inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1) )
    {
        int camera = inputName.empty() ? 0 : inputName[0] - '0';
        if(!capture.open(camera))
            cout << "Capture from camera #" <<  camera << " didn't work" << endl;
    }
    else if( inputName.size() )
    {
        image = imread( inputName, 1 );
        if( image.empty() )
        {
            if(!capture.open( inputName ))
                cout << "Could not read " << inputName << endl;
        }
    }
    else
    {
        image = imread( "../data/lena.jpg", 1 );
        if(image.empty()) cout << "Couldn't read ../data/lena.jpg" << endl;
    }

    if( capture.isOpened() )
    {
        cout << "Video capturing has been started ..." << endl;

        for(;;)
        {
            capture >> frame;
            if( frame.empty() )
                break;

            img_t frame1 = frame.clone();
            detectAndDraw( frame1, cascade, nestedCascade, scale, tryflip );

            char c = (char)waitKey(10);
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
        }
    }
    else
    {
        cout << "Detecting face(s) in " << inputName << endl;
        if( !image.empty() )
        {
            detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
            waitKey(0);
        }
        else if( !inputName.empty() )
        {
            /* assume it is a text file containing the
            list of the image filenames to be processed - one per line */
            FILE* f = fopen( inputName.c_str(), "rt" );
            if( f )
            {
                char buf[1000+1];
                while( fgets( buf, 1000, f ) )
                {
                    int len = (int)strlen(buf);
                    while( len > 0 && isspace(buf[len-1]) )
                        len--;
                    buf[len] = '\0';
                    cout << "file " << buf << endl;
                    image = imread( buf, 1 );
                    if( !image.empty() )
                    {
                        detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
                        char c = (char)waitKey(0);
                        if( c == 27 || c == 'q' || c == 'Q' )
                            break;
                    }
                    else
                    {
                        cerr << "Aw snap, couldn't read image " << buf << endl;
                    }
                }
                fclose(f);
            }
        }
    }

    return 0;
}

void detectAndDraw( img_t& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip )
{
    double t = 0;
    vector<Rect> faces, faces2;
    const static Scalar colors[] =
    {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    img_t gray, smallImg;

    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, CSize(), fx, fx, CC_INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)cGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        CSize(30, 30) );
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CASCADE_FIND_BIGGEST_OBJECT
                                 //|CASCADE_DO_ROUGH_SEARCH
                                 |CASCADE_SCALE_IMAGE,
                                 CSize(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r )
        {
            faces.push_back(Rect(smallImg->cols - r->x - r->width, r->y, r->width, r->height));
        }
    }
    t = (double)cGetTickCount() - t;
    printf( "detection time = %g ms\n", t*1000/getTickFrequency());
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        img_t smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;

        double aspect_ratio = (double)r.width/r.height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cRound((r.x + r.width*0.5)*scale);
            center.y = cRound((r.y + r.height*0.5)*scale);
            radius = cRound((r.width + r.height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        else
            rectangle( img, cPoint(cRound(r.x*scale), cRound(r.y*scale)),
                       cPoint(cRound((r.x + r.width-1)*scale), cRound((r.y + r.height-1)*scale)),
                       color, 3, 8, 0);
        if( nestedCascade.empty() )
            continue;
        smallImgROI = smallImg( r );
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            //|CASCADE_DO_CANNY_PRUNING
            |CASCADE_SCALE_IMAGE,
            CSize(30, 30) );
        for ( size_t j = 0; j < nestedObjects.size(); j++ )
        {
            Rect nr = nestedObjects[j];
            center.x = cRound((r.x + nr.x + nr.width*0.5)*scale);
            center.y = cRound((r.y + nr.y + nr.height*0.5)*scale);
            radius = cRound((nr.width + nr.height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
    }
    imshow( "result", img );
}

#endif
