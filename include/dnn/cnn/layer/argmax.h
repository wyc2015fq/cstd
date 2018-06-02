

//#include "test/test_partial_sort.inl"

static int ArgMax(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  const Blob* prevData = IBLOB(0)->data;
  Blob* next = net->blobs[layer->blobs[1]].data;
  const float* ptr = NULL;
  int* vec = NULL;
  float* outptr = NULL;
  int i, size = Blob_total(prevData);
  int out_max_val = LARG(out_max_val);
  int topk = LARG(topk);

  if (out_max_val) {
    pBoxsetsize(next, 2, topk, 1, 1);
  }
  else {
    pBoxsetsize(next, 1, topk, 1, 1);
  }

  if (Blob_empty(next)) {
    return -100;
  }

  ptr = prevData;

  // partial sort topk with index
  // optional value
  MYREALLOC(vec, size);

  for (i = 0; i < size; i++) {
    vec[i] = i;
  }

#define GREATER(a, b)  ptr[a]>ptr[b]
  PARTIAL_SORT(size, vec, topk, GREATER, int);

  outptr = Blob_datap(next);
  if (out_max_val) {
    float* valptr = outptr + topk;

    for (i = 0; i < topk; i++) {
      outptr[i] = ptr[vec[i]];
      valptr[i] = vec[i];
    }
  }
  else {
    for (i = 0; i < topk; i++) {
      outptr[i] = vec[i];
    }
  }
  FREE(vec);

  return 0;
}
