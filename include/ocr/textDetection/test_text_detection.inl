
#if 1
#define FN_TEXT_LINE_CLASSIFIER  "./model/swt/TextLineClassifier"
#define FN_CHARACTER_CLASSIFIER  "./model/swt/CharacterClassifier"
#define FN_PMEANFFT  "./model/swt/pMeanFFT"
#define FN_PEIGVECSFFT  "./model/swt/pEigVecsFFT"

#include "FilterBox.cpp"
#include "Fmeasure.cpp"
#include "GetCandidate.cpp"
#include "GroundDetect.cpp"
#include "LinkCandidate.cpp"
#include "LinkCandidateUtil.cpp"
#include "PolyGroundTruth.cpp"
#include "swt.cpp"
#include "swtUtil.cpp"
#include "TextDetection.cpp"
#include "Tools.cpp"
#include "VericaficationTextLine.cpp"

#endif

#include"TextDetection.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;
using namespace cv;

#ifdef _DEBUG
#pragma comment(lib,"opencv_world310d.lib")
#else
#pragma comment(lib,"opencv_world310.lib")
#endif
#include <unistd.h>

int test_text_detection()
{
	chdir("C:/OCR_Line");
	vector<string> gtset; string line;
	ifstream fin("./model/swt/gt_demo");
	while (getline(fin, line))
	{
		gtset.push_back(line);
	}
	const char* fn = "./model/swt/demo.jpg";
	fn = "./model/swt/demo.jpg";
	fn = "./demo_images/004.jpg";
	cv::Mat image = cv::imread(fn);
	TextDetection td;
	cout << td.detectText(image, gtset) << endl;
	cv::waitKey();
}
