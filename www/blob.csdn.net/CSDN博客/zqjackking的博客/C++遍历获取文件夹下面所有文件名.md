# C++遍历获取文件夹下面所有文件名 - zqjackking的博客 - CSDN博客





2017年11月20日 09:13:38[zqjackking](https://me.csdn.net/zqjackking)阅读数：607








```cpp
#include <iostream>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#ifdef linux  
#include <unistd.h>  
#include <dirent.h>  
#endif  
#ifdef WIN32  
#include <direct.h>  
#include <io.h>  
#endif  
using namespace std;
```

```cpp
vector<string> getFiles(string cate_dir)  
{  
    vector<string> files;//存放文件名  

#ifdef WIN32  
    _finddata_t file;  
    long lf;  
    //输入文件夹路径  
    if ((lf=_findfirst(cate_dir.c_str(), &file)) == -1) {  
        cout<<cate_dir<<" not found!!!"<<endl;  
    } else {  
        while(_findnext(lf, &file) == 0) {  
            //输出文件名  
            //cout<<file.name<<endl;  
            if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)  
                continue;  
            files.push_back(file.name);  
        }  
    }  
    _findclose(lf);  
#endif  

#ifdef linux  
    DIR *dir;  
    struct dirent *ptr;  
    char base[1000];  

    if ((dir=opendir(cate_dir.c_str())) == NULL)  
        {  
        perror("Open dir error...");  
                exit(1);  
        }  

    while ((ptr=readdir(dir)) != NULL)  
    {  
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir  
                continue;  
        else if(ptr->d_type == 8)    ///file  
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
            files.push_back(ptr->d_name);  
        else if(ptr->d_type == 10)    ///link file  
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
            continue;  
        else if(ptr->d_type == 4)    ///dir  
        {  
            files.push_back(ptr->d_name);  
            /* 
                memset(base,'\0',sizeof(base)); 
                strcpy(base,basePath); 
                strcat(base,"/"); 
                strcat(base,ptr->d_nSame); 
                readFileList(base); 
            */  
        }  
    }  
    closedir(dir);  
#endif  

    //排序，按从小到大排序  
    sort(files.begin(), files.end());  
    return files;  
}
```

返回的是一个vector，调用的话就是files[i] 

比如zqh文件夹下面有1.jpg,2.jpg,那么files[0]=”1.jpg”,files[1]=”2.jpg” 

以下是做图像分类的时候，调用caffe的classification.bin（改动以后的），遍历文件夹下的所有图片，并将每张图片属于每一类的概率写入csv文件。
```cpp
#include <caffe/caffe.hpp>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif  // USE_OPENCV
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <fstream>  
#include <unistd.h>  
#include <dirent.h>
#include <stdlib.h>      
#ifdef USE_OPENCV
using namespace caffe;  // NOLINT(build/namespaces)
using std::string;
using namespace std;
ofstream outFile;

/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class Classifier {
 public:
  Classifier(const string& model_file,
             const string& trained_file,
             const string& mean_file,
             const string& label_file);

  std::vector<Prediction> Classify(const cv::Mat& img, const string file,int N = 5);

 private:
  void SetMean(const string& mean_file);

  std::vector<float> Predict(const cv::Mat& img);

  void WrapInputLayer(std::vector<cv::Mat>* input_channels);

  void Preprocess(const cv::Mat& img,
                  std::vector<cv::Mat>* input_channels);

 private:
  shared_ptr<Net<float> > net_;
  cv::Size input_geometry_;
  int num_channels_;
  cv::Mat mean_;
  std::vector<string> labels_;
};

Classifier::Classifier(const string& model_file,
                       const string& trained_file,
                       const string& mean_file,
                       const string& label_file) {
#ifdef CPU_ONLY
  Caffe::set_mode(Caffe::CPU);
#else
  Caffe::set_mode(Caffe::GPU);
#endif

  /* Load the network. */
  net_.reset(new Net<float>(model_file, TEST));
  net_->CopyTrainedLayersFrom(trained_file);

  CHECK_EQ(net_->num_inputs(), 1) << "Network should have exactly one input.";
  CHECK_EQ(net_->num_outputs(), 1) << "Network should have exactly one output.";

  Blob<float>* input_layer = net_->input_blobs()[0];
  num_channels_ = input_layer->channels();
  CHECK(num_channels_ == 3 || num_channels_ == 1)
    << "Input layer should have 1 or 3 channels.";
  input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

  /* Load the binaryproto mean file. */
  SetMean(mean_file);

  /* Load labels. */
  std::ifstream labels(label_file.c_str());
  CHECK(labels) << "Unable to open labels file " << label_file;
  string line;
  while (std::getline(labels, line))
    labels_.push_back(string(line));

  Blob<float>* output_layer = net_->output_blobs()[0];
  CHECK_EQ(labels_.size(), output_layer->channels())
    << "Number of labels is different from the output layer dimension.";
}

static bool PairCompare(const std::pair<float, int>& lhs,
                        const std::pair<float, int>& rhs) {
  return lhs.first > rhs.first;
}

/* Return the indices of the top N values of vector v. */
static std::vector<int> Argmax(const std::vector<float>& v, int N) {
  std::vector<std::pair<float, int> > pairs;
  for (size_t i = 0; i < v.size(); ++i)
    pairs.push_back(std::make_pair(v[i], i));
  std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);

  std::vector<int> result;
  for (int i = 0; i < N; ++i)
    result.push_back(pairs[i].second);
  return result;
}

/* Return the top N predictions. */
std::vector<Prediction> Classifier::Classify(const cv::Mat& img, const string file,int N) {
  std::vector<float> output = Predict(img);
  int i;
  double sum=0;
  int cnt=0;
  int b=file.find('.'); 
  for(i=0;i<output.size();i++)
  {
    sum+=output[i];
    if(i)
    {
        outFile << file.substr(0,b) << ',' << i << ',' << output[i] << endl;  
    }
    if(i==29)
    {
        outFile << file.substr(0,b) << ',' << 30 << ',' << output[0] << endl;
    }
  }
  if(sum > 1)
  {
      cnt++;
      cout << cnt << endl;
  }
  N = std::min<int>(labels_.size(), N);
  std::vector<int> maxN = Argmax(output, N);
  std::vector<Prediction> predictions;
  for (int i = 0; i < N; ++i) {
    int idx = maxN[i];
    predictions.push_back(std::make_pair(labels_[idx], output[idx]));
  }

  return predictions;
}

/* Load the mean file in binaryproto format. */
void Classifier::SetMean(const string& mean_file) {
  BlobProto blob_proto;
  ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);

  /* Convert from BlobProto to Blob<float> */
  Blob<float> mean_blob;
  mean_blob.FromProto(blob_proto);
  CHECK_EQ(mean_blob.channels(), num_channels_)
    << "Number of channels of mean file doesn't match input layer.";

  /* The format of the mean file is planar 32-bit float BGR or grayscale. */
  std::vector<cv::Mat> channels;
  float* data = mean_blob.mutable_cpu_data();
  for (int i = 0; i < num_channels_; ++i) {
    /* Extract an individual channel. */
    cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
    channels.push_back(channel);
    data += mean_blob.height() * mean_blob.width();
  }

  /* Merge the separate channels into a single image. */
  cv::Mat mean;
  cv::merge(channels, mean);

  /* Compute the global mean pixel value and create a mean image
   * filled with this value. */
  cv::Scalar channel_mean = cv::mean(mean);
  mean_ = cv::Mat(input_geometry_, mean.type(), channel_mean);
}

std::vector<float> Classifier::Predict(const cv::Mat& img) {
  Blob<float>* input_layer = net_->input_blobs()[0];
  input_layer->Reshape(1, num_channels_,
                       input_geometry_.height, input_geometry_.width);
  /* Forward dimension change to all layers. */
  net_->Reshape();

  std::vector<cv::Mat> input_channels;
  WrapInputLayer(&input_channels);
  double mul=0.00390625;
  cv::Mat out_img=img*mul;
  Preprocess(out_img, &input_channels);

  net_->Forward();

  /* Copy the output layer to a std::vector */
  Blob<float>* output_layer = net_->output_blobs()[0];
  const float* begin = output_layer->cpu_data();
  const float* end = begin + output_layer->channels();
  return std::vector<float>(begin, end);
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void Classifier::WrapInputLayer(std::vector<cv::Mat>* input_channels) {
  Blob<float>* input_layer = net_->input_blobs()[0];

  int width = input_layer->width();
  int height = input_layer->height();
  float* input_data = input_layer->mutable_cpu_data();
  for (int i = 0; i < input_layer->channels(); ++i) {
    cv::Mat channel(height, width, CV_32FC1, input_data);
    input_channels->push_back(channel);
    input_data += width * height;
  }
}

void Classifier::Preprocess(const cv::Mat& img,
                            std::vector<cv::Mat>* input_channels) {
  /* Convert the input image to the input image format of the network. */
  cv::Mat sample;
  if (img.channels() == 3 && num_channels_ == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
  else if (img.channels() == 4 && num_channels_ == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
  else if (img.channels() == 4 && num_channels_ == 3)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
  else if (img.channels() == 1 && num_channels_ == 3)
    cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
  else
    sample = img;

  cv::Mat sample_resized;
  if (sample.size() != input_geometry_)
    cv::resize(sample, sample_resized, input_geometry_);
  else
    sample_resized = sample;

  cv::Mat sample_float;
  if (num_channels_ == 3)
    sample_resized.convertTo(sample_float, CV_32FC3);
  else
    sample_resized.convertTo(sample_float, CV_32FC1);

  cv::Mat sample_normalized;
  cv::subtract(sample_float, mean_, sample_normalized);

  /* This operation will write the separate BGR planes directly to the
   * input layer of the network because it is wrapped by the cv::Mat
   * objects in input_channels. */
  cv::split(sample_normalized, *input_channels);

  CHECK(reinterpret_cast<float*>(input_channels->at(0).data)
        == net_->input_blobs()[0]->cpu_data())
    << "Input channels are not wrapping the input layer of the network.";
}
vector<string> getFiles(string cate_dir)  
{
    vector<string> files;
    DIR *dir;  
    struct dirent *ptr;   

    if ((dir=opendir(cate_dir.c_str())) == NULL)  
        {  
        perror("Open dir error...");  
                exit(1);  
        }  

    while ((ptr=readdir(dir)) != NULL)  
    {  
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir  
                continue;  
        else if(ptr->d_type == 8)    ///file  
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
            files.push_back(ptr->d_name);  
        else if(ptr->d_type == 10)    ///link file  
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
            continue;  
        else if(ptr->d_type == 4)    ///dir  
        {  
            files.push_back(ptr->d_name);  
            /* 
                memset(base,'\0',sizeof(base)); 
                strcpy(base,basePath); 
                strcat(base,"/"); 
                strcat(base,ptr->d_nSame); 
                readFileList(base); 
            */  
        }  
    }  
    closedir(dir);    
    sort(files.begin(), files.end());  
    return files;  
}  
int main(int argc, char** argv) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0]
              << " deploy.prototxt network.caffemodel"
              << " mean.binaryproto labels.txt img.jpg" << std::endl;
    return 1;
  }
  outFile.open("/home/zq/test.csv", ios::out); 
  ::google::InitGoogleLogging(argv[0]);

  string model_file   = argv[1];
  string trained_file = argv[2];
  string mean_file    = argv[3];
  string label_file   = argv[4];
  Classifier classifier(model_file, trained_file, mean_file, label_file);
  string filepath = "data/pigface/test/";
  vector<string> files; 
  files=getFiles(filepath);
  int size = files.size();
  cout << size << endl;  
  int j;
  for(j=0;j<size;j++)
  {
  cv::Mat img = cv::imread(filepath+files[j], -1);
  CHECK(!img.empty()) << "Unable to decode image " << files[j];
  std::vector<Prediction> predictions = classifier.Classify(img,files[j]);

  /* Print the top N predictions. 
  for (size_t i = 0; i < predictions.size(); ++i) {
    Prediction p = predictions[i];
    std::cout << std::fixed << std::setprecision(4) << p.second << " - \""
              << p.first << "\"" << std::endl;
  }
  */
  }
  outFile.close();
}
#else
int main(int argc, char** argv) {
  LOG(FATAL) << "This example requires OpenCV; compile with USE_OPENCV.";
}
#endif  // USE_OPENCV
```




