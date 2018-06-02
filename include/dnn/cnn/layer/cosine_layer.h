


class CosineLayer {
 public:

};




void CosineLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  const int count = Data_count(prev->size);
  
  for (i = 0; i < count; ++i) {
    nextData[i] = cos(prevData[i]);
  }
}


void CosineLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  const Dtype* prevData = prevData;
  const Dtype* nextDiff = nextDiff;
  const int count = Data_count(prev->size);

  // Propagate to bottom
  if (propagate_down[0]) {
    Dtype* prevDiff = bottom[0]->mutable_diff();
    for (i = 0; i < count; ++i) {
      prevDiff[i] = -sin(prevData[i]);
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(CosineLayer);
#endif

INSTANTIATE_CLASS(CosineLayer);
REGISTER_LAYER_CLASS(Cosine);


