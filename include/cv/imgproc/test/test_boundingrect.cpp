

#include "test_precomp.hpp"
#include <time.h>

#define IMGPROC_BOUNDINGRECT_ERROR_DIFF 1

#define MESSAGE_ERROR_DIFF "Bounding rectangle found by boundingRect function is incorrect."

using namespace cv;
using namespace std;

class CC_BoundingRectTest: public cvtest::ArrayTest
{
public:
    CC_BoundingRectTest();
    ~CC_BoundingRectTest();

protected:
    void run (int);

private:
    template <typename T> void generate_src_points(vector <Point_<T> >& src, int n);
    template <typename T> Rect get_bounding_rect(const vector <Point_<T> > src);
    template <typename T> bool checking_function_work(vector <Point_<T> >& src, int type);
};

CC_BoundingRectTest::CC_BoundingRectTest() {}
CC_BoundingRectTest::~CC_BoundingRectTest() {}

template <typename T> void CC_BoundingRectTest::generate_src_points(vector <Point_<T> >& src, int n)
{
    src.clear();
    for (int i = 0; i < n; ++i)
        src.push_back(Point_<T>(randu<T>(), randu<T>()));
}

template <typename T> Rect CC_BoundingRectTest::get_bounding_rect(const vector <Point_<T> > src)
{
    int n = (int)src.size();
    T min_w = std::numeric_limits<T>::max(), max_w = std::numeric_limits<T>::min();
    T min_h = min_w, max_h = max_w;

    for (int i = 0; i < n; ++i)
    {
        min_w = MIN<T>(src.at(i).x, min_w);
        max_w = MAX<T>(src.at(i).x, max_w);
        min_h = MIN<T>(src.at(i).y, min_h);
        max_h = MAX<T>(src.at(i).y, max_h);
    }

    return Rect((int)min_w, (int)min_h, (int)max_w-(int)min_w + 1, (int)max_h-(int)min_h + 1);
}

template <typename T> bool CC_BoundingRectTest::checking_function_work(vector <Point_<T> >& src, int type)
{
    const int MAX_COUNT_OF_POINTS = 1000;
    const int N = 10000;

    for (int k = 0; k < N; ++k)
    {

        RNG& rng = ts->get_rng();

        int n = rng.next()%MAX_COUNT_OF_POINTS + 1;

        generate_src_points <T> (src, n);

        Rect right = get_bounding_rect <T> (src);

        Rect rect[2] = { boundingRect(src), boundingRect(CvMat(src)) };

        for (int i = 0; i < 2; ++i) if (rect[i] != right)
        {
            cout << endl; cout << "Checking for the work of boundingRect function..." << endl;
            cout << "Type of src points: ";
            switch (type)
            {
            case 0: {cout << "INT"; break;}
            case 1: {cout << "FLOAT"; break;}
            default: break;
            }
            cout << endl;
            cout << "Src points are stored as "; if (i == 0) cout << "VECTOR" << endl; else cout << "MAT" << endl;
            cout << "Number of points: " << n << endl;
            cout << "Right rect (x, y, w, h): [" << right.x << ", " << right.y << ", " << right.width << ", " << right.height << "]" << endl;
            cout << "Result rect (x, y, w, h): [" << rect[i].x << ", " << rect[i].y << ", " << rect[i].width << ", " << rect[i].height << "]" << endl;
            cout << endl;
            CC_Error(IMGPROC_BOUNDINGRECT_ERROR_DIFF, MESSAGE_ERROR_DIFF);
            return false;
        }

    }

    return true;
}

void CC_BoundingRectTest::run(int)
{
    vector <Point> src_veci; if (!checking_function_work(src_veci, 0)) return;
    vector <Point2f> src_vecf; checking_function_work(src_vecf, 1);
}

TEST (Imgproc_BoundingRect, accuracy) { CC_BoundingRectTest test; test.safe_run(); }
