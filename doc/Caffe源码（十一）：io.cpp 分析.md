# Caffe源码（十一）：io.cpp 分析

2015年08月21日 18:51:24 [seven_first](https://me.csdn.net/seven_first) 阅读数：8656







# 简单介绍

io.cpp 主要定义了一些读取图像或者文件，以及它们之间的一些转化的函数。

# 主要函数

### 1.ReadProtoFromTextFile 函数：

从prototxt文件中读取message参数

```
bool ReadProtoFromTextFile(const char* filename, Message* proto) {
  int fd = open(filename, O_RDONLY);//打开文件
  CHECK_NE(fd, -1) << "File not found: " << filename;
  FileInputStream* input = new FileInputStream(fd);//新建一个FileInputStream对象 input
  bool success = google::protobuf::TextFormat::Parse(input, proto);
  //解析input文件中的Message， 即使文件中参数定义顺序与Message中的参数定义顺序不一致，也可以解析。
  delete input;
  close(fd);
  return success;
}12345678910
```

### 2.WriteProtoToTextFile 函数：

```
void WriteProtoToTextFile(const Message& proto, const char* filename) {
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  FileOutputStream* output = new FileOutputStream(fd);
  CHECK(google::protobuf::TextFormat::Print(proto, output));
  delete output;
  close(fd);// 和ReadProtoFromTextFile功能相反
}1234567
```

### 3.ReadProtoFromBinaryFile 函数：

从二进制文件中读取message 参数

```
bool ReadProtoFromBinaryFile(const char* filename, Message* proto) {//读取二进制文件
  int fd = open(filename, O_RDONLY);
  CHECK_NE(fd, -1) << "File not found: " << filename;
  ZeroCopyInputStream* raw_input = new FileInputStream(fd);
  CodedInputStream* coded_input = new CodedInputStream(raw_input);
  // 建立CodedInputStream类的对象coded_input
  coded_input->SetTotalBytesLimit(kProtoReadBytesLimit, 536870912);
  //折设置最大字节限制
  bool success = proto->ParseFromCodedStream(coded_input);//解析message

  delete coded_input;
  delete raw_input;
  close(fd);
  return success;
}123456789101112131415
```

### 4.WriteProtoToBinaryFile 函数 ：

```
void WriteProtoToBinaryFile(const Message& proto, const char* filename) {
  fstream output(filename, ios::out | ios::trunc | ios::binary);
  CHECK(proto.SerializeToOstream(&output));
}//和ReadProtoFromBinaryFile功能相反1234
```

### 5.ReadImageToCVMat 函数：

以cvMat格式读入图像

```
cv::Mat ReadImageToCVMat(const string& filename,//is_color 为1读入彩色图像，0灰度图
    const int height, const int width, const bool is_color) {
    //height，width都不为0则把图像resize 到height*width
  cv::Mat cv_img;
  int cv_read_flag = (is_color ? CV_LOAD_IMAGE_COLOR :
    CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat cv_img_origin = cv::imread(filename, cv_read_flag);//读入图像
  if (!cv_img_origin.data) {
    LOG(ERROR) << "Could not open or find file " << filename;
    return cv_img_origin;
  }
  if (height > 0 && width > 0) {
    cv::resize(cv_img_origin, cv_img, cv::Size(width, height));
  } else {
    cv_img = cv_img_origin;
  }
  return cv_img;
}123456789101112131415161718
```

重载函数，提供各种不同的功能

```
cv::Mat ReadImageToCVMat(const string& filename,
    const int height, const int width) {
  return ReadImageToCVMat(filename, height, width, true);
}//重载函数，读入彩色图
cv::Mat ReadImageToCVMat(const string& filename,
    const bool is_color) {
  return ReadImageToCVMat(filename, 0, 0, is_color);
}//重载函数，读入图像但不resize
cv::Mat ReadImageToCVMat(const string& filename) {
  return ReadImageToCVMat(filename, 0, 0, true);
}//重载函数，读入彩色图像且不resize1234567891011
```

### 6.matchExt 函数：

匹配文件后缀名

```
static bool matchExt(const std::string & fn,
                     std::string en) {
  size_t p = fn.rfind('.');//p 为文件名中“.”所在位置的索引
  std::string ext = p != fn.npos ? fn.substr(p) : fn;//ext为文件后缀名".xxx"
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  //把ext中的大写字母转化小写字母
  std::transform(en.begin(), en.end(), en.begin(), ::tolower);
  if ( ext == en )
    return true;
  if ( en == "jpg" && ext == "jpeg" )
    return true;
  return false;
}12345678910111213
```

### 7.CVMatToDatum 函数：

cvMat 格式数据转化为Datum格式

```
void CVMatToDatum(const cv::Mat& cv_img, Datum* datum) {
  CHECK(cv_img.depth() == CV_8U) << "Image data type must be unsigned byte";
  datum->set_channels(cv_img.channels());
  datum->set_height(cv_img.rows);
  datum->set_width(cv_img.cols);//分别设置channel， height，width
  datum->clear_data();
  datum->clear_float_data();
  datum->set_encoded(false);
  int datum_channels = datum->channels();
  int datum_height = datum->height();
  int datum_width = datum->width();
  int datum_size = datum_channels * datum_height * datum_width;
  std::string buffer(datum_size, ' ');//将buffer初始化为字符''的datum_size个副本 
  for (int h = 0; h < datum_height; ++h) {
    const uchar* ptr = cv_img.ptr<uchar>(h);//指向图像第h行的指针
    int img_index = 0;
    for (int w = 0; w < datum_width; ++w) {
      for (int c = 0; c < datum_channels; ++c) {
        int datum_index = (c * datum_height + h) * datum_width + w;
        buffer[datum_index] = static_cast<char>(ptr[img_index++]);
      }
    }
  }
  datum->set_data(buffer);
}12345678910111213141516171819202122232425
```

### 8.ReadFileToDatum 函数：

```
bool ReadFileToDatum(const string& filename, const int label,
    Datum* datum) {
  std::streampos size;//获取文件指针位置 size

  fstream file(filename.c_str(), ios::in|ios::binary|ios::ate);
  if (file.is_open()) {
    size = file.tellg();//代表当前get 流指针的位置
    std::string buffer(size, ' ');
    file.seekg(0, ios::beg);//设置0输入文件流的起始位置
    file.read(&buffer[0], size);
    file.close();
    datum->set_data(buffer);//data_->assign(buffer)
    datum->set_label(label);//label_ = label
    datum->set_encoded(true);
    return true;
  } else {
    return false;
  }
}12345678910111213141516171819
```

### 9.ReadImageToDatum 函数：

读入图像到Datum

```
bool ReadImageToDatum(const string& filename, const int label,
    const int height, const int width, const bool is_color,
    const std::string & encoding, Datum* datum) {
  cv::Mat cv_img = ReadImageToCVMat(filename, height, width, is_color);
  if (cv_img.data) {
    if (encoding.size()) {
      if ( (cv_img.channels() == 3) == is_color && !height && !width &&
          matchExt(filename, encoding) )
        return ReadFileToDatum(filename, label, datum);
      std::vector<uchar> buf;
      cv::imencode("."+encoding, cv_img, buf);
      datum->set_data(std::string(reinterpret_cast<char*>(&buf[0]),
                      buf.size()));
      datum->set_label(label);
      datum->set_encoded(true);
      return true;
    }
    CVMatToDatum(cv_img, datum);//cvmat转为Datum格式
    datum->set_label(label);
    return true;
  } else {
    return false;
  }
}123456789101112131415161718192021222324
```

### 10.DecodeDatumToCVMat 函数：

```
cv::Mat DecodeDatumToCVMat(const Datum& datum, bool is_color) {
  cv::Mat cv_img;
  CHECK(datum.encoded()) << "Datum not encoded";
  const string& data = datum.data();
  std::vector<char> vec_data(data.c_str(), data.c_str() + data.size());
  int cv_read_flag = (is_color ? CV_LOAD_IMAGE_COLOR :
    CV_LOAD_IMAGE_GRAYSCALE);
  cv_img = cv::imdecode(vec_data, cv_read_flag);//从内存都入图片
  if (!cv_img.data) {
    LOG(ERROR) << "Could not decode datum ";
  }
  return cv_img;//将encode 的Datum转化为cvMat
}12345678910111213
```

### 11.DecodeDatum 函数：

```
bool DecodeDatum(Datum* datum, bool is_color) {
  if (datum->encoded()) {
    cv::Mat cv_img = DecodeDatumToCVMat((*datum), is_color);
    CVMatToDatum(cv_img, datum);
    return true;
  } else {
    return false;
  }
}//将encodedDatum转化为没有encode的Datum
```