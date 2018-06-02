
  
  class ProposalLayer : public Layer {
  public:
    explicit ProposalLayer(const LayerParameter& param)
      : Layer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top) {
      //LOG(FATAL) << "Reshaping happens during the call to forward.";
    }

    virtual inline const char* type() const { return "ProposalLayer"; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    int base_size_;
    int feat_stride_;
    int pre_nms_topn_;
    int post_nms_topn_;
    Dtype nms_thresh_;
    int min_size_;
    Blob anchors_;
    Blob proposals_;
    Blob<int> roi_indices_;
    Blob<int> nms_mask_;
  };


#define ROUND(x) ((int)((x) + (Dtype)0.5))


static int transform_box(Dtype box[],
                         const Dtype dx, const Dtype dy,
                         const Dtype d_log_w, const Dtype d_log_h,
                         const Dtype img_W, const Dtype img_H,
                         const Dtype min_box_W, const Dtype min_box_H)
{
  // width & height of box
  const Dtype w = box[2] - box[0] + (Dtype)1;
  const Dtype h = box[3] - box[1] + (Dtype)1;
  // center location of box
  const Dtype ctr_x = box[0] + (Dtype)0.5 * w;
  const Dtype ctr_y = box[1] + (Dtype)0.5 * h;

  // new center location according to gradient (dx, dy)
  const Dtype pred_ctr_x = dx * w + ctr_x;
  const Dtype pred_ctr_y = dy * h + ctr_y;
  // new width & height according to gradient d(log w), d(log h)
  const Dtype pred_w = exp(d_log_w) * w;
  const Dtype pred_h = exp(d_log_h) * h;

  // update upper-left corner location
  box[0] = pred_ctr_x - (Dtype)0.5 * pred_w;
  box[1] = pred_ctr_y - (Dtype)0.5 * pred_h;
  // update lower-right corner location
  box[2] = pred_ctr_x + (Dtype)0.5 * pred_w;
  box[3] = pred_ctr_y + (Dtype)0.5 * pred_h;

  // adjust new corner locations to be within the image region,
  box[0] = MAX((Dtype)0, std::min(box[0], img_W - (Dtype)1));
  box[1] = MAX((Dtype)0, std::min(box[1], img_H - (Dtype)1));
  box[2] = MAX((Dtype)0, std::min(box[2], img_W - (Dtype)1));
  box[3] = MAX((Dtype)0, std::min(box[3], img_H - (Dtype)1));

  // recompute new width & height
  const Dtype box_w = box[2] - box[0] + (Dtype)1;
  const Dtype box_h = box[3] - box[1] + (Dtype)1;

  // check if new box's size >= threshold
  return (box_w >= min_box_W) * (box_h >= min_box_H);
}


static void sort_box(Dtype list[], const int start, const int end,
                     const int num_top)
{
  const Dtype pivot_score = list[start * 5 + 4];
  int left = start + 1, right = end;
  Dtype temp[5];
  while (left <= right)
  {
    while (left <= end && list[left * 5 + 4] >= pivot_score)
      ++left;
    while (right > start && list[right * 5 + 4] <= pivot_score)
      --right;
    if (left <= right)
    {
      for (i = 0; i < 5; ++i)
      {
        temp[i] = list[left * 5 + i];
      }
      for (i = 0; i < 5; ++i)
      {
        list[left * 5 + i] = list[right * 5 + i];
      }
      for (i = 0; i < 5; ++i)
      {
        list[right * 5 + i] = temp[i];
      }
      ++left;
      --right;
    }
  }

  if (right > start)
  {
    for (i = 0; i < 5; ++i)
    {
      temp[i] = list[start * 5 + i];
    }
    for (i = 0; i < 5; ++i)
    {
      list[start * 5 + i] = list[right * 5 + i];
    }
    for (i = 0; i < 5; ++i)
    {
      list[right * 5 + i] = temp[i];
    }
  }

  if (start < right - 1)
  {
    sort_box(list, start, right - 1, num_top);
  }
  if (right + 1 < num_top && right + 1 < end)
  {
    sort_box(list, right + 1, end, num_top);
  }
}


static void generate_anchors(int base_size,
                             const Dtype ratios[],
                             const Dtype scales[],
                             const int num_ratios,
                             const int num_scales,
                             Dtype anchors[])
{
  // base box's width & height & center location
  const Dtype base_area = (Dtype)(base_size * base_size);
  const Dtype center = (Dtype)0.5 * (base_size - (Dtype)1);

  // enumerate all transformed boxes
  Dtype *p_anchors = anchors;
  for (i = 0; i < num_ratios; ++i)
  {
    // transformed width & height for given ratio factors
    const Dtype ratio_w = (Dtype)(sqrt(base_area / ratios[i]));
    const Dtype ratio_h = (Dtype)(ratio_w * ratios[i]);

    for (j = 0; j < num_scales; ++j)
    {
      // transformed width & height for given scale factors
      const Dtype scale_w = (Dtype)0.5 * ROUND(ratio_w * scales[j] * 1.0 / base_size - (Dtype)1);
      const Dtype scale_h = (Dtype)0.5 * ROUND(ratio_h * scales[j] * 1.0 / base_size - (Dtype)1);

      // (x1, y1, x2, y2) for transformed box
      p_anchors[0] = center - scale_w;
      p_anchors[1] = center - scale_h;
      p_anchors[2] = center + scale_w;
      p_anchors[3] = center + scale_h;
      p_anchors += 4;
    } // endfor j
  }
}


static void enumerate_proposals(const Dtype bottom4d[],
                                    const Dtype d_anchor4d[],
                                    const Dtype anchors[],
                                    Dtype proposals[],
                                    const int num_anchors,
                                    const int bottom_H, const int bottom_W,
                                    const Dtype img_H, const Dtype img_W,
                                    const Dtype min_box_H, const Dtype min_box_W,
                                    const int feat_stride)
{
  Dtype *p_proposal = proposals;
  const int bottom_area = bottom_H * bottom_W;

  for (h = 0; h < bottom_H; ++h)
  {
    for (w = 0; w < bottom_W; ++w)
    {
      const Dtype x = w * feat_stride;
      const Dtype y = h * feat_stride;
      const Dtype *p_box = d_anchor4d + h * bottom_W + w;
      const Dtype *p_score = bottom4d + h * bottom_W + w;
      for (k = 0; k < num_anchors; ++k)
      {
        const Dtype dx = p_box[(k * 4 + 0) * bottom_area];
        const Dtype dy = p_box[(k * 4 + 1) * bottom_area];
        const Dtype d_log_w = p_box[(k * 4 + 2) * bottom_area];
        const Dtype d_log_h = p_box[(k * 4 + 3) * bottom_area];

        p_proposal[0] = x + anchors[k * 4 + 0];
        p_proposal[1] = y + anchors[k * 4 + 1];
        p_proposal[2] = x + anchors[k * 4 + 2];
        p_proposal[3] = y + anchors[k * 4 + 3];
        p_proposal[4] = transform_box(p_proposal,
                                      dx, dy, d_log_w, d_log_h,
                                      img_W, img_H, min_box_W, min_box_H) *
                        p_score[k * bottom_area];
        p_proposal += 5;
      } // endfor k
    }   // endfor w
  }     // endfor h
}


static void retrieve_rois(const int num_rois,
                              const int item_index,
                              const Dtype proposals[],
                              const int roi_indices[],
                              Dtype rois[],
                              Dtype roi_scores[])
{
  for (i = 0; i < num_rois; ++i)
  {
    const Dtype *const proposals_index = proposals + roi_indices[i] * 5;
    rois[i * 5 + 0] = item_index;
    rois[i * 5 + 1] = proposals_index[0];
    rois[i * 5 + 2] = proposals_index[1];
    rois[i * 5 + 3] = proposals_index[2];
    rois[i * 5 + 4] = proposals_index[3];
    if (roi_scores)
    {
      roi_scores[i] = proposals_index[4];
    }
  }
}


void ProposalLayer::LayerSetUp(const vector<Blob *> &bottom,
                                      const vector<Blob *> &top)
{

  ProposalParameter param = this->layer_param_.proposal_param();

  base_size_ = param.base_size();
  feat_stride_ = param.feat_stride();
  pre_nms_topn_ = param.pre_nms_topn();
  post_nms_topn_ = param.post_nms_topn();
  nms_thresh_ = param.nms_thresh();
  min_size_ = param.min_size();

  vector ratios(param.ratio_size());
  for (i = 0; i < param.ratio_size(); ++i)
  {
    ratios[i] = param.ratio(i);
  }
  vector scales(param.scale_size());
  for (i = 0; i < param.scale_size(); ++i)
  {
    scales[i] = param.scale(i);
  }

  vector<int> anchors_shape(2);
  anchors_shape[0] = ratios.size() * scales.size();
  anchors_shape[1] = 4;
  anchors_.Reshape(anchors_shape);
  generate_anchors(base_size_, &ratios[0], &scales[0],
                   ratios.size(), scales.size(),
                   anchors_);

  vector<int> roi_indices_shape(1);
  roi_indices_shape[0] = post_nms_topn_;
  roi_indices_.Reshape(roi_indices_shape);

  // rois blob : holds R regions of interest, each is a 5 - tuple
  // (n, x1, y1, x2, y2) specifying an image batch index n and a
  // rectangle(x1, y1, x2, y2)
  vector<int> top_shape(2);
  top_shape[0] = prevSize.n * post_nms_topn_;
  top_shape[1] = 5;
  top[0]->Reshape(top_shape);

  // scores blob : holds scores for R regions of interest
  if (layer->o_size > 1)
  {
    top_shape.pop_back();
    top[1]->Reshape(top_shape);
  }
}


void ProposalLayer::Forward(const vector<Blob *> &bottom,
                                       const vector<Blob *> &top)
{

  const Dtype *p_bottom_item = prevData;
  const Dtype *p_d_anchor_item = nextData;
  const Dtype *p_img_info = getData(prev2);
  Dtype *p_roi_item = nextData;
  Dtype *p_score_item = (layer->o_size > 1) ? top[1]->mutable_data() : NULL;

  vector<int> proposals_shape(2);
  vector<int> top_shape(2);
  proposals_shape[0] = 0;
  proposals_shape[1] = 5;
  top_shape[0] = 0;
  top_shape[1] = 5;

  for (n = 0; n < prevSize.n; ++n)
  {
    // bottom shape: (2 x num_anchors) x H x W
    const int bottom_H = prev->size.h;
    const int bottom_W = prev->size.w;
    // input image height & width
    const Dtype img_H = p_img_info[0];
    const Dtype img_W = p_img_info[1];
    // scale factor for height & width
    const Dtype scale_H = p_img_info[2];
    const Dtype scale_W = p_img_info[3];
    // minimum box width & height
    const Dtype min_box_H = min_size_ * scale_H;
    const Dtype min_box_W = min_size_ * scale_W;
    // number of all proposals = num_anchors * H * W
    const int num_proposals = anchors_.shape(0) * bottom_H * bottom_W;
    // number of top-n proposals before NMS
    const int pre_nms_topn = std::min(num_proposals, pre_nms_topn_);
    // number of final RoIs
    int num_rois = 0;

    // enumerate all proposals
    //   num_proposals = num_anchors * H * W
    //   (x1, y1, x2, y2, score) for each proposal
    // NOTE: for bottom, only foreground scores are passed
    proposals_shape[0] = num_proposals;
    proposals_.Reshape(proposals_shape);
    enumerate_proposals(
        p_bottom_item + num_proposals, p_d_anchor_item,
        anchors_.cpu_data(), proposals_, anchors_.shape(0),
        bottom_H, bottom_W, img_H, img_W, min_box_H, min_box_W,
        feat_stride_);

    sort_box(proposals_, 0, num_proposals - 1, pre_nms_topn_);

    nms(pre_nms_topn, proposals_.cpu_data(),
            roi_indices_, &num_rois,
            0, nms_thresh_, post_nms_topn_);

    retrieve_rois(
        num_rois, n, proposals_.cpu_data(), roi_indices_.cpu_data(),
        p_roi_item, p_score_item);

    top_shape[0] += num_rois;

    p_bottom_item += bottom[0]->offset(1);
    p_d_anchor_item += bottom[1]->offset(1);
    p_roi_item += num_rois * 5;
    p_score_item += num_rois * 1;
  }

  top[0]->Reshape(top_shape);
  if (layer->o_size > 1)
  {
    top_shape.pop_back();
    top[1]->Reshape(top_shape);
  }
}


void ProposalLayer::Backward(const vector<Blob*>& top,
                          const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  NOT_IMPLEMENTED;
}
