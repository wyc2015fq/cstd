
static int Input_load_param(Layer* layer, const char* param)
{
  int* size = (int*)layer->x;
  return 3==sscanf(param, "%d %d %d", size, size+1, size+2);
}

static int Input_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blob, img_t* top_blob)
{
  return 0;
}

static int Input_forward_inplace(buf_t* bf, const Layer* layer, img_t* bottom_top_blob)
{
  return 0;
}
