# caffe 运行过程中如何显示更新梯度信息 solver.cpp - 家家的专栏 - CSDN博客





2015年03月09日 20:36:59[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3547








在solver.cpp中。。。

**包含文件变动如下：**

#include <cstdio>



#include <algorithm>

#include <string>

#include <vector>
#include <iomanip>



#include "caffe/net.hpp"

#include "caffe/proto/caffe.pb.h"

#include "caffe/solver.hpp"
#include "caffe/util/io.hpp"
#include "caffe/util/math_functions.hpp"

#include "caffe/util/upgrade_proto.hpp"


using std::max;

using std::min;

using std::setw;

using std::scientific;

using std::left;





**正文cpp文件中，void Solver<Dtype>::Solve(const char* resume_file) {**

**中变动如下：**

if (display) {

      LOG(INFO) << "Iteration " << iter_ << ", loss = " << loss;

      const vector<Blob<Dtype>*>& result = net_->output_blobs();

      int score_index = 0;

      for (int j = 0; j < result.size(); ++j) {

        const Dtype* result_vec = result[j]->cpu_data();

        const string& output_name =

            net_->blob_names()[net_->output_blob_indices()[j]];

        const Dtype loss_weight =

            net_->blob_loss_weights()[net_->output_blob_indices()[j]];

        for (int k = 0; k < result[j]->count(); ++k) {

          ostringstream loss_msg_stream;

          if (loss_weight) {

            loss_msg_stream << " (* " << loss_weight

                            << " = " << loss_weight * result_vec[k] << " loss)";

          }

          LOG(INFO) << "    Train net output #"

              << score_index++ << ": " << output_name << " = "

              << result_vec[k] << loss_msg_stream.str();

        }

      }


    //add by me

     int max_len = 0;

      for (int l = 0; l < net_->layers().size(); ++l) {

        Layer<Dtype>& layer = *net_->layers()[l].get();

        if (layer.blobs().size() > 0 && layer.layer_param().name().length()

            > max_len)

          max_len = layer.layer_param().name().length();

      }

     for (int l = 0; l < net_->layers().size(); ++l) {

        Layer<Dtype>& layer = *net_->layers()[l].get();

        for (int b = 0; b < layer.blobs().size(); ++b) {

          Blob<Dtype>& blob = *layer.blobs()[b].get();

          const Dtype* blob_cpu_data = blob.cpu_data();

          const Dtype* blob_cpu_diff = blob.cpu_diff();

          Dtype data_sum = 0;

          Dtype diff_sum = 0;

          for (int i = 0; i < blob.count(); ++i) {

            data_sum += (blob_cpu_data[i] > Dtype(0.)) ? blob_cpu_data[i]

                : - blob_cpu_data[i];

            diff_sum += (blob_cpu_diff[i] > Dtype(0.)) ? blob_cpu_diff[i]

                : - blob_cpu_diff[i];

          }

          data_sum /= blob.count();

          diff_sum /= blob.count();

          LOG(INFO) << std::left << std::setw(max_len + 1) << std::setfill(' ')

                    << layer.layer_param().name()

                    << " blob" << b << ": " << std::scientific

                   << data_sum << " [" << diff_sum << "]";

        }

      }


     //end revised by me

}






