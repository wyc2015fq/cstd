#ifndef __CLASSIFICATION__
#define __CLASSIFICATION__

#include "ICNNPredict.h"
#include "wstd/filesystem.hpp"
#include "cnn4/cnn4.h"

using namespace std;

static bool PairCompare(const std::pair<float, int>& lhs,
  const std::pair<float, int>& rhs) {
  return lhs.first > rhs.first;
}

/* Return the indices of the top N values of vector v. */
static std::vector<int> Argmax(const std::vector<float>& v, int N) {
  std::vector<std::pair<float, int> > pairs;
  for (size_t i = 0; i < v.size(); ++i)
    pairs.push_back(std::make_pair(v[i], (int)i));
  std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);

  std::vector<int> result;
  for (int i = 0; i < N; ++i)
    result.push_back(pairs[i].second);
  return result;
}

class Classifier //: public ICNNPredict
{
public:
  CnnNet* net_;
  cv::Size input_geometry_;
  int num_channels_;
  cv::Mat mean_;
  vector<float> channel_mean_;
  vector<string> labels_;
  //std::vector< std::set<string> > synsetwords1;
  int FindMaxChannelLayer();
  int FindLayerIndex(const string& strLayerName);
public:
	Classifier();

	bool Init(const string& model_path, bool gpu_mode = true);
	bool Init(const string& trained_file, const string& model_file,
		const string&mean_file, const string& label_file,
		bool gpu_mode);
	void Release() { delete this; }

	bool IsCPUMode();

	std::vector<Prediction> Classify(const string& file, int N = 5);
	std::vector<Prediction> Classify(const unsigned char* pJPGBuffer, int len, int N = 5);

	std::vector<Prediction> Classify(const cv::Mat& img, int N = 5);
	std::vector< std::vector<PredictionIdx> > BatchClassify(const std::vector<cv::Mat>& imgs, int N = 5);
	std::vector<Prediction> CropClassify(const cv::Mat& img, int std_size, int crop_num = 1, int N = 5);

	std::vector<PredictionIdx> ClassifyRtnIdx(const cv::Mat& img, int N = 5);

	//std::vector<float> ExtractFeature(const cv::Mat& img,const string& strLayerName="");
	std::vector<float> GetLayerFeatureMaps(const string& strLayerName, std::vector<int>& outshape);
	int GetFeatureDim();
	std::vector<std::string> GetLabels() { return labels_; }
	std::vector< std::vector<float> > GetLastBlockFeature(const cv::Mat& img);
	std::vector<float> GetOutputFeatureMap(const cv::Mat& img, std::vector<int>& outshape);
	
	void SetMean(const string& mean_file);

	std::vector<float> Predict(const cv::Mat& img);
	std::vector<std::vector<float> > BatchPredict(const std::vector<cv::Mat>& img);

	void WrapInputLayer(std::vector<cv::Mat>* input_channels);

	void Preprocess(const cv::Mat& img,
		std::vector<cv::Mat>* input_channels,bool resize_img=true);

	void GetInputImageSize(int &w, int &h);

	//advanced operations
	float Pruning(float weight_t, const char* saveas_name = 0);

	//∏– ‹“∞π¿º∆
	cv::Mat EstimateReceptiveField(const cv::Mat& img, const string& layerName,int x,int y, int idxNeuron = -1, bool islstm = false, int* width_parts = 0);
	void GetLayerFeatureMapSize(int w, int h, const std::string& layerName, int& w1, int& h1);
private:
	void Forward(const cv::Mat& img, const string& lastLayerName);
	void BatchForward(const vector<cv::Mat>& imgs, const string& lastLayerName);
	void PrepareInput(const cv::Mat& img);
	void PrepareBatchInputs(const vector<cv::Mat>& imgs);
private:
};

Classifier* CreatePredictInstance(const char* model_folder, bool use_gpu)
{
  Classifier* p = new Classifier();
  if (!p->Init(model_folder, use_gpu))
  {
    delete p;
    p = NULL;
  }
  return p;
}
#endif
