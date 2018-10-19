#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"
using namespace std;
using namespace cv;
#define DEBUG FALSE

Point2f GetPointAfterRotate(Point2f inputpoint, Point2f center, double angle) {
  Point2d preturn;
  preturn.x = (inputpoint.x - center.x)*cos(-angle) - (inputpoint.y - center.y)*sin(-angle) + center.x;
  preturn.y = (inputpoint.x - center.x)*sin(-angle) + (inputpoint.y - center.y)*cos(-angle) + center.y;
  return preturn;
}
Point GetPointAfterRotate(Point inputpoint, Point center, double angle) {
  Point preturn;
  preturn.x = (inputpoint.x - center.x)*cos(-1 * angle) - (inputpoint.y - center.y)*sin(-1 * angle) + center.x;
  preturn.y = (inputpoint.x - center.x)*sin(-1 * angle) + (inputpoint.y - center.y)*cos(-1 * angle) + center.y;
  return preturn;
}

double getOrientation(vector<Point> &pts, Point2f& pos, Mat& img)
{
  //Construct a buffer used by the pca analysis
  Mat data_pts = Mat(pts.size(), 2, CV_64FC1);
  for (int i = 0; i < data_pts.rows; ++i)
  {
    data_pts.at<double>(i, 0) = pts[i].x;
    data_pts.at<double>(i, 1) = pts[i].y;
  }

  //Perform PCA analysis
  PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);

  //Store the position of the object
  pos = Point2f(pca_analysis.mean.at<double>(0, 0),
    pca_analysis.mean.at<double>(0, 1));

  //Store the eigenvalues and eigenvectors
  vector<Point2d> eigen_vecs(2);
  vector<double> eigen_val(2);
  for (int i = 0; i < 2; ++i)
  {
    eigen_vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
      pca_analysis.eigenvectors.at<double>(i, 1));

    eigen_val[i] = pca_analysis.eigenvalues.at<double>(i, 0);
  }


  return atan2(eigen_vecs[0].y, eigen_vecs[0].x);
}


int test_leaf(int argc, char** argv)
{

  Mat img = imread("E:/data/testpic/leaf.jpg");
  pyrDown(img, img);
  pyrDown(img, img);

  Mat bw;
  bool dRet;
  cvtColor(img, bw, COLOR_BGR2GRAY);

  threshold(bw, bw, 150, 255, CV_THRESH_BINARY);

  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);


  for (size_t i = 0; i < contours.size(); ++i)
  {

    double area = contourArea(contours[i]);

    if (area < 1e2 || 1e5 < area) continue;

    Point2f* pos = new Point2f();
    double dOrient = getOrientation(contours[i], *pos, img);

    int xmin = 99999;
    int xmax = 0;
    int ymin = 99999;
    int ymax = 0;
    for (size_t j = 0; j<contours[i].size(); j++)
    {
      contours[i][j] = GetPointAfterRotate(contours[i][j], (Point)*pos, dOrient);
      if (contours[i][j].x < xmin)
        xmin = contours[i][j].x;
      if (contours[i][j].x > xmax)
        xmax = contours[i][j].x;
      if (contours[i][j].y < ymin)
        ymin = contours[i][j].y;
      if (contours[i][j].y > ymax)
        ymax = contours[i][j].y;
    }
    Point lt = Point(xmin, ymin);
    Point ld = Point(xmin, ymax);
    Point rd = Point(xmax, ymax);
    Point rt = Point(xmax, ymin);

    drawContours(img, contours, i, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0);

    lt = GetPointAfterRotate((Point)lt, (Point)*pos, -dOrient);
    ld = GetPointAfterRotate((Point)ld, (Point)*pos, -dOrient);
    rd = GetPointAfterRotate((Point)rd, (Point)*pos, -dOrient);
    rt = GetPointAfterRotate((Point)rt, (Point)*pos, -dOrient);

    line(img, lt, ld, Scalar(0, 255, 255), 2);
    line(img, lt, rt, Scalar(0, 255, 255), 2);
    line(img, rd, ld, Scalar(0, 255, 255), 2);
    line(img, rd, rt, Scalar(0, 255, 255), 2);

  }
  imshow("img", img); waitKey(-1);
  return 0;
}