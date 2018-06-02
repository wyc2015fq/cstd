

typedef struct ArgMax
{
  int out_max_val;
  int topk;
} ArgMax;

static int ArgMax_load_param(Layer* layer, const char* param) {
  ArgMax* s = (ArgMax*)layer->x;
  char* p = (char*)param;
  str_getint(p, &p, &s->out_max_val, 0);
  str_getint(p, &p, &s->topk, 1);
  return 1;
}

//#include "test/test_partial_sort.inl"

static int ArgMax_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blob, img_t* top_blob)
{
  ArgMax* s = (ArgMax*)layer->x;
  const float* ptr = NULL;
  int* vec = NULL;
  float* outptr = NULL;
  int i, size = pBox_total(bottom_blob);

  if (s->out_max_val) {
    bf_pBoxsetsize(bf, top_blob, 2, s->topk, 1, 1);
  }
  else {
    bf_pBoxsetsize(bf, top_blob, 1, s->topk, 1, 1);
  }

  if (pBox_empty(top_blob)) {
    return -100;
  }

  ptr = pBox_datap(bottom_blob);

  // partial sort topk with index
  // optional value
  MYREALLOC(vec, size);

  for (i = 0; i < size; i++) {
    vec[i] = i;
  }

#define GREATER(a, b)  ptr[a]>ptr[b]
  PARTIAL_SORT(size, vec, s->topk, GREATER, int);

  outptr = pBox_datap(top_blob);
  if (s->out_max_val) {
    float* valptr = outptr + s->topk;

    for (i = 0; i < s->topk; i++) {
      outptr[i] = ptr[vec[i]];
      valptr[i] = vec[i];
    }
  }
  else {
    for (i = 0; i < s->topk; i++) {
      outptr[i] = vec[i];
    }
  }
  FREE(vec);

  return 0;
}
