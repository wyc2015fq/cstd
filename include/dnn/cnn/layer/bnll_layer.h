
class BNLLLayer {
};



const float kBNLL_THRESHOLD = 50.;


void BNLLLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  const int count = Data_count(prev->size);
  for (i = 0; i < count; ++i) {
    nextData[i] = prevData[i] > 0 ?
        prevData[i] + log(1. + exp(-prevData[i])) :
        log(1. + exp(prevData[i]));
  }
}


void BNLLLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* prevData = prevData;
    const Dtype* nextDiff = nextDiff;
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const int count = Data_count(prev->size);
    Dtype expval;
    for (i = 0; i < count; ++i) {
      expval = exp(std::min(prevData[i], (Dtype)(kBNLL_THRESHOLD)));
      prevDiff[i] = nextDiff[i] * expval / (expval + 1.);
    }
  }
}


