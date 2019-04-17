# caffe blob 数据存储 blob.ccp - 家家的专栏 - CSDN博客





2015年03月23日 14:59:05[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3775








**blob。h函数修改：**

void save_to_file(const std::string& file_name);





**blob。cpp 的修改：**

**头文件**

**#include <fstream>#include <iostream>using namespace std;**



template <typename Dtype>

void Blob<Dtype>::save_to_file( const std::string& file_name)

{

        FILE* fp = fopen(file_name.c_str(), "wb");

        CHECK(fp);

        fwrite(&num_, sizeof(int), 1, fp);

        fwrite(&channels_, sizeof(int), 1, fp);

        fwrite(&height_, sizeof(int), 1, fp);

        fwrite(&width_, sizeof(int), 1, fp);

        fwrite(data_->cpu_data(), sizeof(Dtype), count_, fp); //for save the forward data

       //fwrite(diff_->cpu_data(), sizeof(Dtype), count_, fp); //for save the backward gradient data

//      fwrite(mydata, sizeof(Dtype), 10000, fp); //cpudata

        fclose(fp);



 //   ofstream fout(file_name.c_str());

        //fout << num_ <<"  "  ;

        //fout << channels_ <<"  "  ;

        //fout << height_ <<"  "  ;

        //fout << width_ <<"  "  ;

        //const Dtype* data = this->cpu_data();

        //for (int i = 0; i < count_; ++i)      {

        //      fout << data[i] <<"  "  ;

        //      //LOG(INFO) << "CPU_data"<<data_->cpu_data()[i];

        //}

        ////fout.write(data_->cpu_data(), strlen(data_->cpu_data));

 //   fout.close();

}




**caffe中如何调用并存储数据？**

**(*top)[0]->save_to_file("top" + this->layer_param_.name());**

** bottom[0]->save_to_file("bottom" + this->layer_param_.name());**




**matlab中如何查看数据？***********************************************************************************************************

function y = read_blob_data(file_name)



fp = fopen(file_name);



num_images = fread(fp, 1, 'int32');

num_channels = fread(fp, 1, 'int32');

num_height = fread(fp, 1, 'int32');

num_width = fread(fp, 1, 'int32');



num_count = num_images * num_channels * num_height * num_width;



data = fread(fp, num_count, 'float');



data = reshape(data, num_width, num_height, num_channels, num_images);

y = zeros(num_height, num_width, num_channels, num_images);

for i = 1 : num_images

    for j = 1 : num_channels

        z = data(:, :, j, i);

        y(:, :, j, i) = z';

    end

end








如何调用？

y = read_blob_data(“zhangsan”);





The above code is from wjf~~~~



