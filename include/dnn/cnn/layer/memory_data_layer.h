

/**
 * @brief Provides data to the Net from memory.
 *
 * TODO(dox): thorough documentation for Forward and proto params.
 */

class MemoryDataLayer : public BaseDataLayer {
 public:
  explicit MemoryDataLayer(const LayerParameter& param)
      : BaseDataLayer(param), has_new_data_(false) {}
  virtual void DataLayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "MemoryData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int ExactNumTopBlobs() const { return 2; }

  virtual void AddDatumVector(const vector<Datum>& datum_vector);
#ifdef USE_OPENCV
  virtual void AddMatVector(const vector<cv::Mat>& mat_vector,
      const vector<int>& labels);
#endif  // USE_OPENCV

  // Reset should accept const pointers, but can't, because the memory
  //  will be given to Blob, which is mutable
  void Reset(Dtype* data, Dtype* label, int n);
  void set_batch_size(int new_size);
  void set_spatial_size(int new_height, int new_width);

  int batch_size() { return batch_size_; }
  int channels() { return channels_; }
  int height() { return height_; }
  int width() { return width_; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  int batch_size_, channels_, height_, width_, size_;
  Dtype* data_;
  Dtype* labels_;
  int n_;
  size_t pos_;
  Blob added_data_;
  Blob added_label_;
  bool has_new_data_;
  bool transpose_;
};


void MemoryDataLayer::DataLayerSetUp(const vector<Blob*>& bottom,
     const vector<Blob*>& top) {
  batch_size_ = this->layer_param_.memory_data_param().batch_size();
  channels_ = this->layer_param_.memory_data_param().channels();
  height_ = this->layer_param_.memory_data_param().height();
  width_ = this->layer_param_.memory_data_param().width();
  transpose_ = this->layer_param_.memory_data_param().transpose();
  size_ = channels_ * height_ * width_;
  CHECK_GT(batch_size_ * size_, 0) <<
      "batch_size, channels, height, and width must be specified and"
      " positive in memory_data_param";
  int crop_size = this->transform_param_.crop_size();
  if (crop_size > 0) {
    top[0]->Reshape(batch_size_, channels_, crop_size, crop_size);
    added_data_.Reshape(batch_size_, channels_, crop_size, crop_size);
  } else {
    top[0]->Reshape(batch_size_, channels_, height_, width_);
    added_data_.Reshape(batch_size_, channels_, height_, width_);
  }
  vector<int> label_shape(1, batch_size_);
  top[1]->Reshape(label_shape);
  added_label_.Reshape(label_shape);
  data_ = NULL;
  labels_ = NULL;
  added_data_.cpu_data();
  added_label_.cpu_data();
}


void MemoryDataLayer::AddDatumVector(const vector<Datum>& datum_vector) {
  CHECK(!has_new_data_) <<
      "Can't add data until current data has been consumed.";
  size_t num = datum_vector.size();
  channels_ = num;
  CHECK_GT(num, 0) << "There is no datum to add.";
  int crop_size = this->transform_param_.crop_size();
  if (crop_size > 0)
    added_data_.Reshape(num, channels_, crop_size, crop_size);
  else
    added_data_.Reshape(num, channels_, height_, width_);
  added_label_.Reshape(num, 1, 1, 1);
  // Apply data transformations (mirror, scale, crop...)
  this->data_transformer_->Transform(datum_vector, &added_data_);
  // Copy Labels
  Dtype* top_label = added_label_;
  for (item_id = 0; item_id < num; ++item_id) {
    top_label[item_id] = datum_vector[item_id].label();
  }
  // num_images == batch_size_
  Dtype* nextData = added_data_;
  Reset(nextData, top_label, num);
  has_new_data_ = true;
}

#ifdef USE_OPENCV

void MemoryDataLayer::AddMatVector(const vector<cv::Mat>& mat_vector,
    const vector<int>& labels) {
  size_t num = mat_vector.size();
  CHECK(!has_new_data_) <<
      "Can't add mat until current data has been consumed.";
  CHECK_GT(num, 0) << "There is no mat to add";
  batch_size_ = num;
  height_ = mat_vector[0].rows;
  width_ = mat_vector[0].cols;
  if (transpose_) {
    std::swap(height_, width_);
  }
  int crop_size = this->transform_param_.crop_size();
  if (crop_size > 0)
    added_data_.Reshape(num, channels_, crop_size, crop_size);
  else {
    added_data_.Reshape(num, channels_, height_, width_);
  }
    
  added_label_.Reshape(num, 1, 1, 1);
  // Apply data transformations (mirror, scale, crop...)
  this->data_transformer_->Transform(mat_vector, &added_data_, transpose_);
  // Copy Labels
  Dtype* top_label = added_label_;
  for (item_id = 0; item_id < num && item_id < labels.size(); ++item_id) {
    top_label[item_id] = labels[item_id];
  }
  // num_images == batch_size_
  Dtype* nextData = added_data_;
  Reset(nextData, top_label, num);
  has_new_data_ = true;
}
#endif  // USE_OPENCV


void MemoryDataLayer::Reset(Dtype* data, Dtype* labels, int n) {
  CHECK(data);
  CHECK(labels);
  // Warn with transformation parameters since a memory array is meant to
  // be generic and no transformations are done with Reset().
  if (this->layer_param_.has_transform_param()) {
    // suppress this warning as we have applied transformation before calling
    // Reset
    // LOG(WARNING) << this->type() << " does not transform array data on
    // Reset()";
  }
  data_ = data;
  labels_ = labels;
  n_ = n;
  pos_ = 0;
}


void MemoryDataLayer::set_batch_size(int new_size) {
  CHECK(!has_new_data_) <<
      "Can't change batch_size until current data has been consumed.";
  batch_size_ = new_size;
  added_data_.Reshape(batch_size_, channels_, height_, width_);
  added_label_.Reshape(batch_size_, 1, 1, 1);
}


void MemoryDataLayer::set_spatial_size(int new_height, int new_width) {
  CHECK(!has_new_data_) <<
    "Can't change batch_size until current data has been consumed.";
  height_ = new_height;
  width_ = new_width;
  added_data_.Reshape(batch_size_, channels_, height_, width_);
}


void MemoryDataLayer::Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK(data_) << "MemoryDataLayer needs to be initalized by calling Reset";
  int crop_size = this->transform_param_.crop_size();
  if (crop_size > 0)
    top[0]->Reshape(batch_size_, channels_, crop_size, crop_size);
  else
    top[0]->Reshape(batch_size_, channels_, height_, width_);
  top[1]->Reshape(batch_size_, 1, 1, 1);
  top[0]->set_data(data_ + pos_ * size_);
  top[1]->set_data(labels_ + pos_);
  pos_ = (pos_ + batch_size_) % n_;
  if (pos_ == 0)
    has_new_data_ = false;
}

INSTANTIATE_CLASS(MemoryDataLayer);
REGISTER_LAYER_CLASS(MemoryData);


