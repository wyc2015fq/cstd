
void FUN(pooling_forward)(PoolMethod pool, Phase phase, const Dtype* bottom_data,
  const int num, const int channels, const int height, const int width, const int pooled_height, const int pooled_width,
  const int kernel_h, const int kernel_w, const int stride_h, const int stride_w, const int pad_h, const int pad_w,
  Dtype* rand_idx, Dtype* top_data, int* mask, Dtype* top_mask) {
  int top_count = num*channels*pooled_height * pooled_width;
  int bottom_offset = height * width;
  int top_offset = pooled_height * pooled_width;
  bool use_top_mask = top_mask!=NULL;
  double aa = 0;
  switch (pool) {
  case PoolMethod_MAX:
    // Initialize
    if (use_top_mask) {
      FUN(caffe_set)(top_count, Dtype(-1), top_mask);
    }
    else {
      icaffe_set(top_count, -1, mask);
    }
    FUN(caffe_set)(top_count, Dtype(-FLT_MAX), top_data);
    // The main loop
    for (int n = 0; n < num; ++n) {
      for (int c = 0; c < channels; ++c) {
        for (int ph = 0; ph < pooled_height; ++ph) {
          for (int pw = 0; pw < pooled_width; ++pw) {
            int hstart = ph * stride_h - pad_h;
            int wstart = pw * stride_w - pad_w;
            int hend = min(hstart + kernel_h, height);
            int wend = min(wstart + kernel_w, width);
            hstart = max(hstart, 0);
            wstart = max(wstart, 0);
            const int poolindex = ph * pooled_width + pw;
            for (int h = hstart; h < hend; ++h) {
              for (int w = wstart; w < wend; ++w) {
                const int index = h * width + w;
                if (bottom_data[index] > top_data[poolindex]) {
                  top_data[poolindex] = bottom_data[index];
                  aa += fabs(top_data[poolindex]);
                  if (aa > 0) {
                    int asdf = 0;
                  }
                  if (top_mask) {
                    top_mask[poolindex] = static_cast<Dtype>(index);
                  }
                  else {
                    mask[poolindex] = index;
                  }
                }
              }
            }
          }
        }
        // compute offset
        bottom_data += bottom_offset;
        top_data += top_offset;
        if (top_mask) {
          top_mask += top_offset;
        }
        else {
          mask += top_offset;
        }
      }
    }
    break;
  case PoolMethod_AVE:
    for (int i = 0; i < top_count; ++i) {
      top_data[i] = 0;
    }
    // The main loop
    for (int n = 0; n < num; ++n) {
      for (int c = 0; c < channels; ++c) {
        for (int ph = 0; ph < pooled_height; ++ph) {
          for (int pw = 0; pw < pooled_width; ++pw) {
            int hstart = ph * stride_h - pad_h;
            int wstart = pw * stride_w - pad_w;
            int hend = min(hstart + kernel_h, height + pad_h);
            int wend = min(wstart + kernel_w, width + pad_w);
            int poolsize = (hend - hstart) * (wend - wstart);
            hstart = max(hstart, 0);
            wstart = max(wstart, 0);
            hend = min(hend, height);
            wend = min(wend, width);
            for (int h = hstart; h < hend; ++h) {
              for (int w = wstart; w < wend; ++w) {
                top_data[ph * pooled_width + pw] +=
                  bottom_data[h * width + w];
              }
            }
            top_data[ph * pooled_width + pw] /= poolsize;
          }
        }
        // compute offset
        bottom_data += bottom_offset;
        top_data += top_offset;
      }
    }
    break;
  case PoolMethod_STOCHASTIC:
    NOT_IMPLEMENTED;
    break;
  default:
    LOG(FATAL) << "Unknown pooling method.";
  }
}

void FUN(pooling_backward)(PoolMethod pool, const Dtype* const rand_idx,
  const Dtype* top_diff, const int* mask, const Dtype* top_mask, const int num,
  const int channels, const int height, const int width, const int pooled_height, const int pooled_width, const int kernel_h,
  const int kernel_w, const int stride_h, const int stride_w, const int pad_h, const int pad_w, Dtype* bottom_diff) {
#if 0
  if (!bottom[0]->propagate_down_) {
    return;
  }
  const Dtype* top_diff = top[0]->diff();
  Dtype* bottom_diff = bottom[0]->mutable_diff();
  // Different pooling methods. We explicitly do the switch outside the for
  // loop to save time, although this results in more codes.
  //caffe_set(bottom[0]->count(), Dtype(0), bottom_diff);
  // We'll output the mask to top[1] if it's of size >1.
  const bool use_top_mask = top.size() > 1;
  const int* mask = NULL;  // suppress warnings about uninitialized variables
  const Dtype* top_mask = NULL;
  if (use_top_mask) {
    top_mask = top[1]->data();
  }
  else {
    mask = max_idx_.data();
  }
#endif
  const int count = num*channels*height * width;
  //const bool use_top_mask = top_mask != NULL;

  FUN(caffe_set)(count, Dtype(0), bottom_diff);
  int bottom_offset = height*width;
  int top_offset = pooled_height*pooled_width;
  switch (pool) {
  case PoolMethod_MAX:
    // The main loop
    for (int n = 0; n < num; ++n) {
      for (int c = 0; c < channels; ++c) {
        for (int ph = 0; ph < pooled_height; ++ph) {
          for (int pw = 0; pw < pooled_width; ++pw) {
            const int index = ph * pooled_width + pw;
            const int bottom_index = top_mask ? top_mask[index] : mask[index];
            bottom_diff[bottom_index] += top_diff[index];
          }
        }
        bottom_diff += bottom_offset;
        top_diff += top_offset;
        if (top_mask) {
          top_mask += top_offset;
        }
        else {
          mask += top_offset;
        }
      }
    }
    break;
  case PoolMethod_AVE:
    // The main loop
    for (int n = 0; n < num; ++n) {
      for (int c = 0; c < channels; ++c) {
        for (int ph = 0; ph < pooled_height; ++ph) {
          for (int pw = 0; pw < pooled_width; ++pw) {
            int hstart = ph * stride_h - pad_h;
            int wstart = pw * stride_w - pad_w;
            int hend = min(hstart + kernel_h, height + pad_h);
            int wend = min(wstart + kernel_w, width + pad_w);
            int poolsize = (hend - hstart) * (wend - wstart);
            hstart = max(hstart, 0);
            wstart = max(wstart, 0);
            hend = min(hend, height);
            wend = min(wend, width);
            for (int h = hstart; h < hend; ++h) {
              for (int w = wstart; w < wend; ++w) {
                bottom_diff[h * width + w] += top_diff[ph * pooled_width + pw] / poolsize;
              }
            }
          }
        }
        // offset
        bottom_diff += bottom_offset;
        top_diff += top_offset;
      }
    }
    break;
  case PoolMethod_STOCHASTIC:
    NOT_IMPLEMENTED;
    break;
  default:
    LOG(FATAL) << "Unknown pooling method.";
  }
}