# faster rcnn windows 下c++版本

2017年01月11日 11:37:51 [鱼蛋蛋哥](https://me.csdn.net/u010327085) 阅读数：6168



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/u010327085/article/details/54342070

# faster rcnn windows 下c++版本

参考了<http://www.cnblogs.com/louyihang-loves-baiyan/p/5485955.html>，和<http://blog.csdn.net/oYangZi12/article/details/53290426>。 
上面两个分别是参考在linux下参考python版本的faster rcnn改写成c++版本的。不过其中用到了python的rpn layer，以及其中的 nms库，不能完全脱离python。 
第二篇是改写的matlab版的faster rcnn。把rpn的操作在代码中间实现，中间有一层转接，感觉不太统一。 
我主要是参考这两篇，用c++实现rpn层，并融入到caffe中，使用起来比较方便。其中主要流程参考的python版的faster rcnn。具体一些细节参考了<http://blog.csdn.net/oYangZi12/article/details/53290426>这里的内容。

环境：win8 + cuda8 +opencv2.4.11+微软的windows版本caffe。可在github上下载。

**1.添加rpn层，直接上代码。**

- rpn_layer.hpp

```
#ifndef CAFFE_RPN_LAYER_HPP_
#define CAFFE_RPN_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include"opencv2/opencv.hpp"
#define mymax(a,b) ((a)>(b))?(a):(b)
#define mymin(a,b) ((a)>(b))?(b):(a)
namespace caffe {

    /**
    * @brief implement RPN layer for faster rcnn
    */

    template <typename Dtype>
    class RPNLayer : public Layer<Dtype> {
    public:
        explicit RPNLayer(const LayerParameter& param)
            : Layer<Dtype>(param) {}
        virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
            const vector<Blob<Dtype>*>& top);
        virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
            const vector<Blob<Dtype>*>& top){}
        virtual inline const char* type() const { return "RPN"; }

    protected:
        virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
            const vector<Blob<Dtype>*>& top);
        virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
            const vector<Blob<Dtype>*>& top);
        virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
            const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom){};

        private:

        int feat_stride_;
        int base_size_;
        vector<int> anchor_scales_;
        vector<float> ratios_;
        vector<vector<float>> gen_anchors_;
        int anchors_[9][4];
        int anchors_nums_;
        int min_size_;
        int pre_nms_topN_;
        int post_nms_topN_;
        float nms_thresh_;
        int src_height_;
        int src_width_;
        float src_scale_;
        private:
        void generate_anchors();
        vector<vector<float>> ratio_enum(vector<float>);
        vector<float> whctrs(vector<float>);
        vector<float> mkanchor(float w,float h,float x_ctr,float y_ctr);
        vector<vector<float>> scale_enum(vector<float>);
        cv::Mat proposal_local_anchor(int width, int height);

        void filter_boxs(cv::Mat& pre_box, cv::Mat& score, vector<RPN::abox>& aboxes);

    };
}  // namespace caffe
#endif  // CAFFE_RPN_LAYER_HPP_1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465
```

- rpn_layer.cpp

```
#include <algorithm>
#include <vector>

#include "caffe/layers/rpn_layer.hpp"
#include "caffe/util/math_functions.hpp"

int debug = 0;
int  tmp[9][4] = {
    { -83, -39, 100, 56 },
    { -175, -87, 192, 104 },
    { -359, -183, 376, 200 },
    { -55, -55, 72, 72 },
    { -119, -119, 136, 136 },
    { -247, -247, 264, 264 },
    { -35, -79, 52, 96 },
    { -79, -167, 96, 184 },
    { -167, -343, 184, 360 }
};
namespace caffe {

    template <typename Dtype>
    void RPNLayer<Dtype>::LayerSetUp(
        const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
        feat_stride_ = layer_param_.rpn_param().feat_stride();
        base_size_ = layer_param_.rpn_param().basesize();
        min_size_ = layer_param_.rpn_param().boxminsize();
        pre_nms_topN_ = layer_param_.rpn_param().per_nms_topn();
        post_nms_topN_ = layer_param_.rpn_param().post_nms_topn();
        nms_thresh_ = layer_param_.rpn_param().nms_thresh();
        int scales_num = layer_param_.rpn_param().scale_size();
        for (int i = 0; i < scales_num; ++i)
        {
            anchor_scales_.push_back(layer_param_.rpn_param().scale(i));
        }
        int ratios_num = layer_param_.rpn_param().ratio_size();
        for (int i = 0; i < ratios_num; ++i)
        {
            ratios_.push_back(layer_param_.rpn_param().ratio(i));
        }
        generate_anchors();

        //memcpy(anchors_, tmp, 9 * 4 * sizeof(int));
        //anchors_nums_ = 9;
        for (int i = 0; i<gen_anchors_.size(); ++i)
        {
            for (int j = 0; j<gen_anchors_[i].size(); ++j)
            {
                anchors_[i][j] = gen_anchors_[i][j];
                if (debug)
                    LOG(INFO) << anchors_[i][j] << std::endl;;
            }
        }
        anchors_nums_ = gen_anchors_.size();

        top[0]->Reshape(1, 5, 1, 1);
        if (top.size() > 1)
        {
            top[1]->Reshape(1, 1, 1, 1);
        }
    }

    template <typename Dtype>
    void RPNLayer<Dtype>::generate_anchors(){
        //generate base anchor
        vector<float> base_anchor;
        base_anchor.push_back(0);
        base_anchor.push_back(0);
        base_anchor.push_back(base_size_ - 1);
        base_anchor.push_back(base_size_ - 1);
        //enum ratio anchors
        vector<vector<float>>ratio_anchors = ratio_enum(base_anchor);
        for (int i = 0; i < ratio_anchors.size(); ++i)
        {
            vector<vector<float>> tmp = scale_enum(ratio_anchors[i]);
            gen_anchors_.insert(gen_anchors_.end(), tmp.begin(), tmp.end());
        }
    }

    template <typename Dtype>
    vector<vector<float>> RPNLayer<Dtype>::scale_enum(vector<float> anchor){
        vector<vector<float>> result;
        vector<float> reform_anchor = whctrs(anchor);
        float x_ctr = reform_anchor[2];
        float y_ctr = reform_anchor[3];
        float w = reform_anchor[0];
        float h = reform_anchor[1];
        for (int i = 0; i < anchor_scales_.size(); ++i)
        {
            float ws = w * anchor_scales_[i];
            float hs = h *  anchor_scales_[i];
            vector<float> tmp = mkanchor(ws, hs, x_ctr, y_ctr);
            result.push_back(tmp);
        }
        return result;
    }


    template <typename Dtype>
    vector<vector<float> > RPNLayer<Dtype>::ratio_enum(vector<float> anchor){
        vector<vector<float>> result;
        vector<float> reform_anchor = whctrs(anchor);
        float x_ctr = reform_anchor[2];
        float y_ctr = reform_anchor[3];
        float size = reform_anchor[0] * reform_anchor[1];
        for (int i = 0; i < ratios_.size(); ++i)
        {
            float size_ratios = size / ratios_[i];
            float ws = round(sqrt(size_ratios));
            float hs = round(ws*ratios_[i]);
            vector<float> tmp = mkanchor(ws, hs, x_ctr, y_ctr);
            result.push_back(tmp);
        }
        return result;
    }

    template <typename Dtype>
    vector<float> RPNLayer<Dtype>::mkanchor(float w, float h, float x_ctr, float y_ctr){
        vector<float> tmp;
        tmp.push_back(x_ctr - 0.5*(w - 1));
        tmp.push_back(y_ctr - 0.5*(h - 1));
        tmp.push_back(x_ctr + 0.5*(w - 1));
        tmp.push_back(y_ctr + 0.5*(h - 1));
        return tmp;
    }
    template <typename Dtype>
    vector<float> RPNLayer<Dtype>::whctrs(vector<float> anchor){
        vector<float> result;
        result.push_back(anchor[2] - anchor[0] + 1); //w
        result.push_back(anchor[3] - anchor[1] + 1); //h
        result.push_back((anchor[2] + anchor[0]) / 2); //ctrx
        result.push_back((anchor[3] + anchor[1]) / 2); //ctry
        return result;
    }


    template <typename Dtype>
    cv::Mat RPNLayer<Dtype>::proposal_local_anchor(int width, int height)
    {
        Blob<float> shift;
        cv::Mat shitf_x(height, width, CV_32SC1);
        cv::Mat shitf_y(height, width, CV_32SC1);
        for (size_t i = 0; i < width; i++)
        {
            for (size_t j = 0; j < height; j++)
            {
                shitf_x.at<int>(j, i) = i * feat_stride_;
                shitf_y.at<int>(j, i) = j * feat_stride_;
            }
        }
        shift.Reshape(anchors_nums_, width*height, 4,  1);
        float *p = shift.mutable_cpu_diff(), *a = shift.mutable_cpu_data();
        for (int i = 0; i < height*width; i++)
        {
            for (int j = 0; j < anchors_nums_; j++)
            {
                size_t num = i * 4 + j * 4 * height*width;
                p[num + 0] = -shitf_x.at<int>(i / shitf_x.cols, i % shitf_x.cols);
                p[num + 2] = -shitf_x.at<int>(i / shitf_x.cols, i % shitf_x.cols);
                p[num + 1] = -shitf_y.at<int>(i / shitf_y.cols, i % shitf_y.cols);
                p[num + 3] = -shitf_y.at<int>(i / shitf_y.cols, i % shitf_y.cols);
                a[num + 0] = anchors_[j][0];
                a[num + 1] = anchors_[j][1];
                a[num + 2] = anchors_[j][2];
                a[num + 3] = anchors_[j][3];
            }
        }
        shift.Update();
        cv::Mat loacl_anchors(anchors_nums_ * height*width, 4, CV_32FC1);
        size_t num = 0;
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                for (int c = 0; c < anchors_nums_; ++c)
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        loacl_anchors.at<float>((i*width + j)*anchors_nums_+c, k)= shift.data_at(c, i*width + j, k, 0);
                    }
                }
            }
        }
        return loacl_anchors;
    }



    template<typename Dtype>
    void RPNLayer<Dtype>::filter_boxs(cv::Mat& pre_box, cv::Mat& score, vector<RPN::abox>& aboxes)
    {
        float localMinSize=min_size_*src_scale_;
        aboxes.clear();

        for (int i = 0; i < pre_box.rows; i++)
        {
            int widths = pre_box.at<float>(i, 2) - pre_box.at<float>(i, 0) + 1;
            int heights = pre_box.at<float>(i, 3) - pre_box.at<float>(i, 1) + 1;
            if (widths >= localMinSize || heights >= localMinSize)
            {
                RPN::abox tmp;
                tmp.x1 = pre_box.at<float>(i, 0);
                tmp.y1 = pre_box.at<float>(i, 1);
                tmp.x2 = pre_box.at<float>(i, 2);
                tmp.y2 = pre_box.at<float>(i, 3);
                tmp.score = score.at<float>(i, 0);
                aboxes.push_back(tmp);
            }
        }
    }



    template <typename Dtype>
    void RPNLayer<Dtype>::Forward_gpu(
        const vector<Blob<Dtype>*>& bottom,
        const vector<Blob<Dtype>*>& top) {
        this->Forward_cpu(bottom, top);
    }

    template <typename Dtype>
    void RPNLayer<Dtype>::Forward_cpu(
        const vector<Blob<Dtype>*>& bottom,
        const vector<Blob<Dtype>*>& top) {

        int width = bottom[1]->width();
        int height = bottom[1]->height();
        //int channels = bottom[1]->channels();


        //get boxs_delta,向右。
        cv::Mat boxs_delta(height*width*anchors_nums_, 4, CV_32FC1);
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                for (int k = 0; k < anchors_nums_; ++k)
                {
                    for (int c = 0; c < 4; ++c)
                    {
                        boxs_delta.at<float>((i*width + j)*anchors_nums_ + k, c) = bottom[1]->data_at(0, k*4 + c, i, j);
                    }
                }
            }
        }

        //get sores 向右，前面anchors_nums_个位bg的得分，后面anchors_nums_为fg得分，我们需要的是后面的。
        cv::Mat scores(height*width*anchors_nums_, 1, CV_32FC1);
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                for (int k = 0; k < anchors_nums_; ++k)
                {
                    scores.at<float>((i*width + j)*anchors_nums_+k, 0) = bottom[0]->data_at(0, k + anchors_nums_, i, j);
                }
            }
        }

        //get im_info

        src_height_ = bottom[2]->data_at(0, 0,0,0);
        src_width_ = bottom[2]->data_at(0, 1,0,0);
        src_scale_ = bottom[2]->data_at(0, 2, 0, 0);

        //gen local anchors 向右
        cv::Mat local_anchors = proposal_local_anchor(width, height);

        //Convert anchors into proposals via bbox transformations
        cv::Mat pre_box = RPN::bbox_tranform_inv(local_anchors, boxs_delta);

        for (int i = 0; i < pre_box.rows; ++i)
        {
            if (pre_box.at<float>(i, 0) < 0)    pre_box.at<float>(i, 0) = 0;
            if (pre_box.at<float>(i, 0) > (src_width_ - 1)) pre_box.at<float>(i, 0) = src_width_ - 1;
            if (pre_box.at<float>(i, 2) < 0)    pre_box.at<float>(i, 2) = 0;
            if (pre_box.at<float>(i, 2) > (src_width_ - 1)) pre_box.at<float>(i, 2) = src_width_ - 1;

            if (pre_box.at<float>(i, 1) < 0)    pre_box.at<float>(i, 1) = 0;
            if (pre_box.at<float>(i, 1) > (src_height_ - 1))    pre_box.at<float>(i, 1) = src_height_ - 1;
            if (pre_box.at<float>(i, 3) < 0)    pre_box.at<float>(i, 3) = 0;
            if (pre_box.at<float>(i, 3) > (src_height_ - 1))    pre_box.at<float>(i, 3) = src_height_ - 1;
        }
        vector<RPN::abox>aboxes;
        filter_boxs(pre_box, scores, aboxes);
        std::sort(aboxes.rbegin(), aboxes.rend()); //降序
        if (pre_nms_topN_ > 0)
        {
            int tmp = mymin(pre_nms_topN_, aboxes.size());
            aboxes.erase(aboxes.begin() + tmp - 1, aboxes.end());
        }
        RPN::nms(aboxes,nms_thresh_);

        if (post_nms_topN_ > 0)
        {
            int tmp = mymin(post_nms_topN_, aboxes.size());
            aboxes.erase(aboxes.begin() + tmp - 1, aboxes.end());
        }
        top[0]->Reshape(aboxes.size(),5,1,1);
        Dtype *top0 = top[0]->mutable_cpu_data();
        for (int i = 0; i < aboxes.size(); ++i)
        {
            top0[0] = 0;
            top0[1] = aboxes[i].x1;
            top0[2] = aboxes[i].y1; 
            top0[3] = aboxes[i].x2;
            top0[4] = aboxes[i].y2;
            top0 += top[0]->offset(1);
        }
        if (top.size()>1)
        {
            top[1]->Reshape(aboxes.size(), 1,1,1);
            Dtype *top1 = top[1]->mutable_cpu_data();
            for (int i = 0; i < aboxes.size(); ++i)
            {
                top1[0] = aboxes[i].score;
                top1 += top[1]->offset(1);
            }
        }
    }

#ifdef CPU_ONLY
        STUB_GPU(RPNLayer);
#endif

    INSTANTIATE_CLASS(RPNLayer);
    REGISTER_LAYER_CLASS(RPN);

}  // namespace caffe123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198199200201202203204205206207208209210211212213214215216217218219220221222223224225226227228229230231232233234235236237238239240241242243244245246247248249250251252253254255256257258259260261262263264265266267268269270271272273274275276277278279280281282283284285286287288289290291292293294295296297298299300301302303304305306307308309310311312313314315316317318319320321322323324325326327328
```

- 修改caffe.proto文件。主要是添加rpn param

```
optional RPNParameter rpn_param = 99;


message RPNParameter {

  optional uint32 feat_stride = 1;
  optional uint32 basesize = 2;
  repeated uint32 scale = 3;
  repeated float ratio = 4;
  optional uint32 boxminsize =5;
  optional uint32 per_nms_topn = 9;
  optional uint32 post_nms_topn = 11;
  optional float nms_thresh = 8;
}1234567891011121314
```

**2 添加abox结构体和nms等函数** 
主要是在common.hpp和common.cpp中。

- common.hpp

```
namespace RPN{
    struct abox
    {
        float x1;
        float y1;
        float x2;
        float y2;
        float score;
        bool operator <(const abox&tmp) const{
            return score < tmp.score;
        }
    };
    void nms(std::vector<abox>& input_boxes,float nms_thresh);
    cv::Mat bbox_tranform_inv(cv::Mat, cv::Mat);
}123456789101112131415
```

- common.cpp

```
namespace RPN{
    cv::Mat bbox_tranform_inv(cv::Mat local_anchors, cv::Mat boxs_delta){
        cv::Mat pre_box(local_anchors.rows, local_anchors.cols, CV_32FC1);
        for (int i = 0; i < local_anchors.rows; i++)
        {
            double pred_ctr_x, pred_ctr_y, src_ctr_x, src_ctr_y;
            double dst_ctr_x, dst_ctr_y, dst_scl_x, dst_scl_y;
            double src_w, src_h, pred_w, pred_h;
            src_w = local_anchors.at<float>(i, 2) - local_anchors.at<float>(i, 0) + 1;
            src_h = local_anchors.at<float>(i, 3) - local_anchors.at<float>(i, 1) + 1;
            src_ctr_x = local_anchors.at<float>(i, 0) + 0.5 * src_w;
            src_ctr_y = local_anchors.at<float>(i, 1) + 0.5 * src_h;

            dst_ctr_x = boxs_delta.at<float>(i, 0);
            dst_ctr_y = boxs_delta.at<float>(i, 1);
            dst_scl_x = boxs_delta.at<float>(i, 2);
            dst_scl_y = boxs_delta.at<float>(i, 3);
            pred_ctr_x = dst_ctr_x*src_w + src_ctr_x;
            pred_ctr_y = dst_ctr_y*src_h + src_ctr_y;
            pred_w = exp(dst_scl_x) * src_w;
            pred_h = exp(dst_scl_y) * src_h;

            pre_box.at<float>(i, 0) = pred_ctr_x - 0.5*pred_w;
            pre_box.at<float>(i, 1) = pred_ctr_y - 0.5*pred_h;
            pre_box.at<float>(i, 2) = pred_ctr_x + 0.5*pred_w;
            pre_box.at<float>(i, 3) = pred_ctr_y + 0.5*pred_h;
        }
        return pre_box;
    }


    void nms(std::vector<abox> &input_boxes, float nms_thresh){
        std::vector<float>vArea(input_boxes.size());
        for (int i = 0; i < input_boxes.size(); ++i)
        {
            vArea[i] = (input_boxes.at(i).x2 - input_boxes.at(i).x1 + 1)
                * (input_boxes.at(i).y2 - input_boxes.at(i).y1 + 1);
        }
        for (int i = 0; i < input_boxes.size(); ++i)
        {
            for (int j = i + 1; j < input_boxes.size();)
            {
                float xx1 = std::max(input_boxes[i].x1, input_boxes[j].x1);
                float yy1 = std::max(input_boxes[i].y1, input_boxes[j].y1);
                float xx2 = std::min(input_boxes[i].x2, input_boxes[j].x2);
                float yy2 = std::min(input_boxes[i].y2, input_boxes[j].y2);
                float w = std::max(float(0), xx2 - xx1 + 1);
                float   h = std::max(float(0), yy2 - yy1 + 1);
                float   inter = w * h;
                float ovr = inter / (vArea[i] + vArea[j] - inter);
                if (ovr >= nms_thresh)
                {
                    input_boxes.erase(input_boxes.begin() + j);
                    vArea.erase(vArea.begin() + j);
                }
                else
                {
                    j++;
                }
            }
        }
    }
}123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263
```

**3. 在libcaffe中添加层**

在libcaffe中rpn_layer.hpp,rpn_layer.cpp, 
roi_pooling.hpp,roi_pooling.cpp,roi_pooling.cu。

上面部分上传到了github。<https://github.com/CCHOT/caffe>

**4.测试加输入输出**

- caffe_detector.h

```
#ifndef DETECTORPRIVATE_H
#define DETECTORPRIVATE_H

//#include "detectresult.h"
#define INPUT_SIZE_NARROW  600
#define INPUT_SIZE_LONG  1000


#include <string>
#include <caffe/net.hpp>
#include <opencv2/core/core.hpp>

struct Result
{
    float arry[6];
};

class Caffe_DetectorPrivate
{
public:
    Caffe_DetectorPrivate();
    bool loadModel(const std::string &model_file, const std::string &weights_file, const std::string &mean_file,const std::string &label_file, bool encrypt_flag = 0);
    /*DetectResult*/void detect(cv::Mat &image);

private:


private:
    std::shared_ptr< caffe::Net<float> > net_;
    std::vector<Result> m_Result;
    int class_num_;
};

#endif // DETECTORPRIVATE_H
1234567891011121314151617181920212223242526272829303132333435
```

- caffe_detector.cpp

```
#include "caffe_detectorprivate.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include <fstream>

using std::string;
using std::vector;
using namespace caffe;
using  std::max;
using std::min;

//Using for box sort


Caffe_DetectorPrivate::Caffe_DetectorPrivate()
{

}

bool Caffe_DetectorPrivate::loadModel(const std::string &model_file, 
    const std::string &weights_file,const std::string &mean_file,
    const std::string &label_file, bool encrypt_flag)
{
#ifdef CPU_ONLY
    Caffe::set_mode(Caffe::CPU);
#else
    Caffe::set_mode(Caffe::GPU);
#endif //#ifdef CPU_ONLY 
    net_.reset(new Net<float>(model_file, TEST));
    net_->CopyTrainedLayersFrom(weights_file);  
    class_num_ = net_->blob_by_name("cls_prob")->channels();
    return true;
}


void Caffe_DetectorPrivate::detect(cv::Mat &image){

    float CONF_THRESH = 0.8;
    float NMS_THRESH = 0.3;

    int max_side = max(image.rows, image.cols);
    int min_side = min(image.rows, image.cols);

    float max_side_scale = float(max_side) / float(INPUT_SIZE_LONG);
    float min_side_scale = float(min_side) / float(INPUT_SIZE_NARROW);
    float max_scale = max(max_side_scale, min_side_scale);

    float img_scale = float(1) / max_scale;
    int height = int(image.rows * img_scale);
    int width = int(image.cols * img_scale);
    //printf("%d,%d", height, width);
    int num_out;
    cv::Mat cv_resized;
    image.convertTo(cv_resized, CV_32FC3);
    cv::resize(cv_resized, cv_resized, cv::Size(width, height));
    cv::Mat mean(height, width, cv_resized.type(), cv::Scalar(102.9801, 115.9465, 122.7717));
    cv::Mat normalized;
    subtract(cv_resized, mean, normalized);




    float im_info[3];
    im_info[0] = height;
    im_info[1] = width;
    im_info[2] = img_scale;
    shared_ptr<Blob<float>> input_layer = net_->blob_by_name("data");
    input_layer->Reshape(1, normalized.channels(), height, width);
    net_->Reshape();
    float* input_data = input_layer->mutable_cpu_data();
    vector<cv::Mat> input_channels;
    for (int i = 0; i < input_layer->channels(); ++i) {
        cv::Mat channel(height, width, CV_32FC1, input_data);
        input_channels.push_back(channel);
        input_data += height * width;
    }
    cv::split(normalized, input_channels);
    net_->blob_by_name("im_info")->set_cpu_data(im_info);
    net_->Forward();


    int num = net_->blob_by_name("rois")->num();
    const float *rois_data = net_->blob_by_name("rois")->cpu_data();
    int num1 = net_->blob_by_name("bbox_pred")->num();
    cv::Mat rois_box(num, 4, CV_32FC1);
    for (int i = 0; i < num; ++i)
    {
        rois_box.at<float>(i, 0) = rois_data[i * 5 + 1] / img_scale;
        rois_box.at<float>(i, 1) = rois_data[i * 5 + 2] / img_scale;
        rois_box.at<float>(i, 2) = rois_data[i * 5 + 3] / img_scale;
        rois_box.at<float>(i, 3) = rois_data[i * 5 + 4] / img_scale;
    }

    shared_ptr<Blob<float>> bbox_delt_data = net_->blob_by_name("bbox_pred");
    shared_ptr<Blob<float>>score = net_->blob_by_name("cls_prob");

    vector<RPN::abox> result;
    for (int i = 1; i < class_num_; ++i)
    {
        cv::Mat bbox_delt(num, 4, CV_32FC1);
        for (int j = 0; j < num; ++j)
        {
            bbox_delt.at<float>(j, 0) = bbox_delt_data->data_at(j, i-1 * 4, 0, 0);
            bbox_delt.at<float>(j, 1) = bbox_delt_data->data_at(j, i * 4+1, 0, 0);
            bbox_delt.at<float>(j, 2) = bbox_delt_data->data_at(j, i * 4+2, 0, 0);
            bbox_delt.at<float>(j, 3) = bbox_delt_data->data_at(j, i * 4+3, 0, 0);
        }
        cv::Mat box_class =RPN::bbox_tranform_inv(rois_box, bbox_delt);

        vector<RPN::abox>aboxes;
        for (int j = 0; j < box_class.rows; ++j)
        {
            if (box_class.at<float>(j, 0) < 0)  box_class.at<float>(j, 0) = 0;
            if (box_class.at<float>(j, 0) > (image.cols - 1))   box_class.at<float>(j, 0) = image.cols - 1;
            if (box_class.at<float>(j, 2) < 0)  box_class.at<float>(j, 2) = 0;
            if (box_class.at<float>(j, 2) > (image.cols - 1))   box_class.at<float>(j, 2) = image.cols - 1;

            if (box_class.at<float>(j, 1) < 0)  box_class.at<float>(j, 1) = 0;
            if (box_class.at<float>(j, 1) > (image.rows - 1))   box_class.at<float>(j, 1) = image.rows - 1;
            if (box_class.at<float>(j, 3) < 0)  box_class.at<float>(j, 3) = 0;          
            if (box_class.at<float>(j, 3) > (image.rows - 1))   box_class.at<float>(j, 3) = image.rows - 1;
            RPN::abox tmp;
            tmp.x1 = box_class.at<float>(j, 0);
            tmp.y1 = box_class.at<float>(j, 1);
            tmp.x2 = box_class.at<float>(j, 2);
            tmp.y2 = box_class.at<float>(j, 3);
            tmp.score = score->data_at(j,i,0,0);
            aboxes.push_back(tmp);
        }
        std::sort(aboxes.rbegin(), aboxes.rend());
        RPN::nms(aboxes, NMS_THRESH);
        for (int k = 0; k < aboxes.size();)
        {
            if (aboxes[k].score < CONF_THRESH)
            {
                aboxes.erase(aboxes.begin() + k);
            }
            else
            {
                k++;
            }
        }
        result.insert(result.end(), aboxes.begin(), aboxes.end());
    }
    return;
}123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149
```

**5.修改python faster rcnn中的pt文件**

主要是rpn层。

```
layer {
  name: 'proposal'
  type: 'RPN'
  bottom: 'rpn_cls_prob_reshape'
  bottom: 'rpn_bbox_pred'
  bottom: 'im_info'
  top: 'rois'
  rpn_param{
   feat_stride : 16
   basesize : 16
   scale : 8
   scale : 16
   scale : 32
   ratio : 0.5
   ratio : 1
   ratio : 2
   boxminsize :16
   per_nms_topn : 0;
   post_nms_topn : 0;
   nms_thresh : 0.7;
  }
}
```