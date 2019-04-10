# caffe批量分类

2016年01月16日 13:09:08 [鱼蛋蛋哥](https://me.csdn.net/u010327085) 阅读数：3281



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/u010327085/article/details/50527776

caffe的官方文档中只给出了对一张图片进行分类的demo，也即cpp_classification。由于一些需求，我们需要对大量图像进行分类，这就需要批量操作。参考了一些资料来完成这一工作，主要是使用MemoryDatalayer这一数据输入层来进行操作。 
MemortDatalayer能够使得caffe支持从内存中将数据导入网络。而且可以批量进行。

## 进入主题

**第一步** 
修改网络参数，这里为只列出data层，其他层基本上是按照caffenet中的deploy.prototxt来的，最后的层为prob层。如果只想要最后的分类结果，也可以在最后面加上argmax层。

```protobuf
layer {
  name: "mydata"
  type: "MemoryData"
  top: "data"
  top: "label"
  memory_data_param {
    batch_size: 50
    channels: 3
    height: 227
    width: 227
    }
  transform_param{
    crop_size: 227
    mirror: false
    mean_file:"../../model/imagenet_mean.binaryproto"
    }
  }
```

想要说的就一点，就是caffe中的layers与layer。之前一直没注意，这次在使用中在网上也查了一些资料，基本上都是用layers定义的data层。导致了我找到好久都没解决问题。这里说几点： 
1.统一使用layers或者layer，不要混用。 
2.在type中的参数，layer为字符串，需要双引号，以及首字母大写。不清楚的可以看caffe文档，不会查文档可以随便写一个，然后看错误输出，会将所有可用的层列举出来；layers的type不带双引号，且都为大写。

**进入主题** 
首先说明一点，我的输出层为prob，因此为了取出输出结果，在代码中可能有点繁琐的操作，如果不需要可以进行修改。 
MultiClassifier.hpp

```cpp
#ifndef MULTICLASSFIER_H
#define MULTICLASSFIER_H

#include<string>
#include<vector>
#include"caffe/net.hpp"
#include"caffe/data_layers.hpp"
#include<opencv2/core.hpp>

using cv::Mat;
using std::string;
using std::vector;

namespace caffe{

    template<typename Dtype>
    class MultiClassifier{
    public:
        explicit MultiClassifier(const string&param_file, const string& weights_file);
        Dtype test(vector<Mat> &images, vector<int> &labels, int iter_num);
        inline shared_ptr<Net<Dtype>>net(){ return net_; }
        void predict(vector<Mat> &images, vector<vector<Dtype>> &labels);
        void predict(vector<Dtype> &data, vector<int> *labels, int num);
        void extract_feature(vector<Mat> &images, vector<vector<Dtype>> *out);
    protected:
        shared_ptr<Net<Dtype>>net_;
        MemoryDataLayer<Dtype>*m_layer_;
        int batch_size_;
        int channels_;
        int height_;
        int width_;
        DISABLE_COPY_AND_ASSIGN(MultiClassifier);
    };
}
#endif

```

MultiClassifier.cpp

```cpp
#include<cstdio>
#include<algorithm>
#include"caffe/proto/caffe.pb.h"
#include"caffe/util/io.hpp"
#include"caffe/util/math_functions.hpp"
#include"caffe/util/upgrade_proto.hpp"

#include "MultiClassifier.h"
//#define CPU_ONLY
namespace caffe{
    template<typename Dtype>
    MultiClassifier<Dtype>::MultiClassifier(const string&param_file, const string &weights_file) :net_()
    {
#ifdef CPU_ONLY
        Caffe::set_mode(Caffe::CPU);
#else
        Caffe::set_mode(Caffe::GPU);
#endif
        net_.reset(new Net<Dtype>(param_file, TEST));
        net_->CopyTrainedLayersFrom(weights_file);
        m_layer_ = (MemoryDataLayer<Dtype>*)net_->layers()[0].get();
        batch_size_ = m_layer_->batch_size();
        channels_ = m_layer_->channels();
        height_ = m_layer_->height();
        width_ = m_layer_->width();
    }

    template<typename Dtype>
    Dtype MultiClassifier<Dtype>::test(vector<Mat>&images, vector<int>&labels, int iter_num)
    {
        m_layer_->AddMatVector(images, labels);
        int iterations = iter_num;
        vector<Blob<Dtype>*>bottom_vec;
        vector<int> test_score_output_id;
        vector<Dtype> test_score;
        Dtype loss = 0;
        for (int i = 0; i < iterations; ++i) {
            Dtype iter_loss;
            const vector<Blob<Dtype>*>& result =
                net_->Forward(bottom_vec, &iter_loss);
            loss += iter_loss;
            int idx = 0;
            for (int j = 0; j < result.size(); ++j) {
                const Dtype* result_vec = result[j]->cpu_data();
                for (int k = 0; k < result[j]->count(); ++k, ++idx) {
                    const Dtype score = result_vec[k];
                    if (i == 0) {
                        test_score.push_back(score);
                        test_score_output_id.push_back(j);
                    }
                    else {
                        test_score[idx] += score;
                    }
                    const std::string& output_name = net_->blob_names()[
                        net_->output_blob_indices()[j]];
                        LOG(INFO) << "Batch " << i << ", " << output_name << " = " << score;
                }
            }
        }
        loss /= iterations;
        LOG(INFO) << "Loss: " << loss;
        return loss;
    }
    template <typename Dtype>
    void MultiClassifier<Dtype>::predict(vector<Mat> &images, vector<vector<Dtype>>&labels)
    {
        int original_length = images.size();
        if (original_length == 0)
            return;
        int valid_length = original_length / batch_size_ * batch_size_;
        if (original_length != valid_length)
        {
            valid_length += batch_size_;
            for (int i = original_length; i < valid_length; i++)
            {
                images.push_back(images[0].clone());
            }
        }
        vector<int>valid_labels;
        valid_labels.resize(valid_length, 0);
        m_layer_->AddMatVector(images, valid_labels);
        vector<Blob<Dtype>* > bottom_vec;
        for (int i = 0; i < valid_length / batch_size_; i++)
        {
            const vector<Blob<Dtype>*>& result = net_->Forward(bottom_vec);
            const Dtype * result_vec = result[1]->cpu_data();
            int nums = result[1]->shape()[0];
            int channels = result[1]->shape()[1];
            for (int k = 0; k < nums; k++)
            {
                vector<Dtype>temp;
                for (int j = 0; j < channels; j++)
                {
                    temp.push_back(result_vec[k*channels + j]);
                }
                labels.push_back(temp);
            }
        }
        if (original_length != valid_length)
        {
            images.erase(images.begin() + original_length, images.end());
            labels.erase(labels.begin() + original_length, labels.end());
        }
    }
    template <typename Dtype>
    void MultiClassifier<Dtype>::extract_feature(vector<Mat> &images, vector<vector<Dtype>> *out)
    {
        int original_length = images.size();
        if (original_length == 0)
            return;
        int valid_length = original_length / batch_size_ * batch_size_;
        if (original_length != valid_length)
        {
            valid_length += batch_size_;
            for (int i = original_length; i < valid_length; i++)
            {
                images.push_back(images[0].clone());
            }
        }
        vector<int> valid_labels;
        valid_labels.resize(valid_length, 0);
        m_layer_->AddMatVector(images, valid_labels);
        vector<Blob<Dtype>* > bottom_vec;
        out->clear();
        for (int i = 0; i < valid_length / batch_size_; i++)
        {
            const vector<Blob<Dtype>*>& result = net_->Forward(bottom_vec);
            const Dtype * result_vec = result[0]->cpu_data();
            const int dim = result[0]->count(1);
            for (int j = 0; j < result[0]->num(); j++)
            {
                const Dtype * ptr = result_vec + j * dim;
                vector<Dtype> one_;
                for (int k = 0; k < dim; ++k)
                    one_.push_back(ptr[k]);
                out->push_back(one_);
            }
        }
        if (original_length != valid_length)
        {
            images.erase(images.begin() + original_length, images.end());
            out->erase(out->begin() + original_length, out->end());
        }
    }
    INSTANTIATE_CLASS(MultiClassifier);
}// namespace caffe

```

main.cpp

```cpp
#include"MultiClassifier.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<io.h>
#include<time.h>
using namespace caffe;
typedef std::pair<string, float> Prediction;


void getFiles(string path, vector<string>& files)
{
    //文件句柄
    long   hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}

int main(int argc, char** argv) {
    //caffe的准备工作
    clock_t start, finish;
    double inittime,getfiletime,pretime,getpairtime,writetime;
    start = clock();
    ::google::InitGoogleLogging(argv[0]);

    string model_file = "..\\..\\model\\deploy_Test.prototxt";
    string trained_file = "..\\..\\model\\forvideo_iter_60000.caffemodel";
    string label_file = "D:\\dataset\\representSnap\\labels.txt";
    //load lables;
    vector<string>labels_;//存放所有标签名称
    vector<int>labels_idx_;//存放分类结果标签索引
    vector<cv::Mat> input;
    vector<vector<float>> labels;//存放分类结果，每张图片，每个类别的概率
    std::ifstream flabels(label_file.c_str());
    CHECK(flabels) << "Unable to open labels file " << label_file;
    string line;
    while (std::getline(flabels, line))
        labels_.push_back(string(line));

    //init Multiclassifier
    MultiClassifier<float> classifier(model_file, trained_file);
    finish = clock();
    inittime = (double)(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    vector<string> files;
    getFiles("D:\\dataset\\representSnap\\total\\test", files);
    for (int i = 0; i < files.size(); i++)
    {
        Mat reimg;
        Mat img = cv::imread(files[i], -1);
        cv::resize(img, reimg, CvSize(256, 256));
        input.push_back(reimg);
    }
    finish = clock();
    getfiletime = (double)(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    classifier.predict(input,labels);
    finish = clock();
    pretime = (double)(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    for (int i = 0; i < labels.size(); i++)
    {
        auto max = std::max_element(labels[i].begin(), labels[i].end());
        labels_idx_.push_back(std::distance(labels[i].begin(), max));
    }
    finish = clock();
    getpairtime = (double)(finish - start) / CLOCKS_PER_SEC;

    start = clock();
    std::ofstream outfile("output.txt");
    for (int i = 0; i < labels_idx_.size(); i++)
    {
        int idx = labels_idx_[i];
        outfile << labels_[idx] << ":" << labels[i][idx] << std::endl;
    }
    outfile.close();
    finish = clock();
    writetime = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << "init:" << inittime << std::endl;
    std::cout << "getfile:" << getfiletime << std::endl;
    std::cout << "pre:" << pretime << std::endl;
    std::cout << "getpair:" << getpairtime << std::endl;
    std::cout << "writetime:" << writetime << std::endl;
    return 1;
}
```

结果，我的test文件夹下游342张图片，batch_size的大小是50。在win8，intel i7-4770K 3.5GHz，NV gtx770下的测试结果如下： 
![图一](https://img-blog.csdn.net/20160116172123671)