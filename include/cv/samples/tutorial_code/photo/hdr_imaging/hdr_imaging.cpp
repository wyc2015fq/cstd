#include <opencv2/photo.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

void loadExposureSeq(String, vector<CvMat>&, vector<float>&);

int main(int, char**argv)
{
    vector<CvMat> images;
    vector<float> times;
    loadExposureSeq(argv[1], images, times);

    CvMat response;
    Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
    calibrate->process(images, response, times);

    CvMat hdr;
    Ptr<MergeDebevec> merge_debevec = createMergeDebevec();
    merge_debevec->process(images, hdr, times, response);

    CvMat ldr;
    Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
    tonemap->process(hdr, ldr);

    CvMat fusion;
    Ptr<MergeMertens> merge_mertens = createMergeMertens();
    merge_mertens->process(images, fusion);

    imwrite("fusion.png", fusion * 255);
    imwrite("ldr.png", ldr * 255);
    imwrite("hdr.hdr", hdr);

    return 0;
}

void loadExposureSeq(String path, vector<CvMat>& images, vector<float>& times)
{
    path = path + std::string("/");
    ifstream list_file((path + "list.txt").c_str());
    string name;
    float val;
    while(list_file >> name >> val) {
        CvMat img = imread(path + name);
        images.push_back(img);
        times.push_back(1 / val);
    }
    list_file.close();
}
