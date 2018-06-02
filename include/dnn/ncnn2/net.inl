
//#include "paramdict.inl"
#include "ncnn.inl"
struct Blob
{
  // blob name
  char name[MAX_NAME];
  // layer index which produce this blob as output
  int producer;
  // layer index which need this blob as input
  int consumers[MAX_BLOB];
  int consumers_size;
};

struct Net
{
  Blob* blobs;
  int blobs_size;
  Layer *layers;
  int layers_size;
};

int forward_inplace(buf_t* bf, const Layer* layer, img_t* bottom_top_blobs)
{
  img_t bottom_blobs[1] = {0};
  imclone2(bottom_top_blobs, bottom_blobs);
  bf_free(bf, bottom_top_blobs);
  int ret = layer_registry[layer->index].forward(bf, layer, bottom_blobs, bottom_top_blobs);
  imfree(bottom_blobs);
  return ret;
}

int layer_to_index(const char* type)
{
  for (int i = 0; i < layer_registry_entry_count; i++) {
    if (strcmp(type, layer_registry[i].name) == 0) {
      return i;
    }
  }

  return -1;
}

int find_blob_index_by_name(Net* net, const char* name)
{
  for (size_t i = 0; i < net->blobs_size; i++) {
    if (0==strcmp(net->blobs[i].name, name)) {
      return i;
    }
  }

  fprintf(stderr, "find_blob_index_by_name %s failed\n", name);
  return -1;
}

int net_load_param(Net* net, FILE* fp)
{
  int layer_index = 0;
  int blob_index = 0;
  char line[1024];
#if 0
  int magic = 0;
  fscanf(fp, "%d", &magic);

  if (magic != 7767517) {
    fprintf(stderr, "param is too old, please regenerate\n");
    return -1;
  }
#endif

  // parse
  net->layers_size = 0;
  net->blobs_size = 0;
  fgets(line, countof(line), fp);
  sscanf(line, "%d %d", &net->layers_size, &net->blobs_size);

  net->blobs_size += 100;
  MYREALLOC(net->layers, net->layers_size);
  MEMZERO(net->layers, net->layers_size);
  MYREALLOC(net->blobs, net->blobs_size);
  MEMZERO(net->blobs, net->blobs_size);

  while (fgets(line, countof(line), fp)) {
    Layer *layer = net->layers + layer_index;
    int i, nscan = 0;
    char layer_type[256];
    char* p = line;
    BZERO(layer);
    nscan = str_getstr(p, &p, layer_type, countof(layer_type), NULL) &&
    str_getstr(p, &p, layer->name, countof(layer->name), NULL) &&
    str_getint(p, &p, &layer->bottoms_size, 0) &&
    str_getint(p, &p, &layer->tops_size, 0);

    if (!nscan) {
      printf("warning! fscanf\n");
      continue;
    }

    ASSERT(layer->bottoms_size <= MAX_BLOB);
    ASSERT(layer->tops_size <= MAX_BLOB);

    for (i = 0; i < layer->bottoms_size; i++) {
      Blob* blob = net->blobs+blob_index;
      if (!str_getstr(p, &p, blob->name, countof(blob->name), NULL)) {
        continue;
      }

      int bottom_blob_index = find_blob_index_by_name(net, blob->name);

      if (bottom_blob_index == -1) {
        bottom_blob_index = blob_index++;
        if (bottom_blob_index >= net->blobs_size); {
          int asdf=0;
        }
        printf("new blob %s\n", blob->name);
      } else {
        int adsf=0;
      }

      blob = net->blobs + bottom_blob_index;
      blob->consumers[blob->consumers_size++] = layer_index;
      layer->bottoms[i] = bottom_blob_index;
    }

    for (i = 0; i < layer->tops_size; i++) {
      Blob* blob = net->blobs+blob_index;
      if (!str_getstr(p, &p, blob->name, countof(blob->name), NULL)) {
        continue;
      }

      int top_blob_index = find_blob_index_by_name(net, blob->name);

      if (top_blob_index == -1) {
        top_blob_index = blob_index++;
        printf("new blob %s\n", blob->name);
        if (top_blob_index >= net->blobs_size); {
          int asdf=0;
        }
      } else {
        int adsf=0;
      }

      // fprintf(stderr, "new blob %s\n", blob_name);
      blob->producer = layer_index;
      layer->tops[i] = top_blob_index;
      blob_index++;
    }

    int typeindex = layer_to_index(layer_type);
    if (typeindex < 0) {
      int adsf=0;
    }
    ASSERT(0<=typeindex);
    layer_registry[typeindex].load_param(layer, p);
    layer_index++;
  }

  return 0;
}

#if 0

int Net::load_param(const char* protopath)
{
  FILE* fp = fopen(protopath, "rb");

  if (!fp) {
    fprintf(stderr, "fopen %s failed\n", protopath);
    return -1;
  }

  int ret = load_param(fp);

  fclose(fp);

  return ret;
}

int Net::load_param_bin(FILE* fp)
{
  int magic = 0;
  fread(&magic, sizeof(int), 1, fp);

  if (magic != 7767517) {
    fprintf(stderr, "param is too old, please regenerate\n");
    return -1;
  }

  int layers_size = 0;
  fread(&layers_size, sizeof(int), 1, fp);

  int blobs_size = 0;
  fread(&blobs_size, sizeof(int), 1, fp);

  layers.resize(layers_size);
  blobs.resize(blobs_size);

  ParamDict pd;

  for (int i = 0; i < layers_size; i++) {
    int typeindex;
    fread(&typeindex, sizeof(int), 1, fp);

    int bottoms_size;
    fread(&bottoms_size, sizeof(int), 1, fp);

    int tops_size;
    fread(&tops_size, sizeof(int), 1, fp);

    Layer* layer = create_layer(typeindex);

    if (!layer) {
      int custom_index = typeindex & ~LayerType::CustomBit;
      layer = create_custom_layer(custom_index);
    }

    if (!layer) {
      fprintf(stderr, "layer %d not exists or registered\n", typeindex);
      clear();
      return -1;
    }

    //         layer->type = std::string(layer_type);
    //         layer->name = std::string(layer_name);
    //         fprintf(stderr, "new layer %d\n", typeindex);

    layer->bottoms.resize(bottoms_size);

    for (int j = 0; j < bottoms_size; j++) {
      int bottom_blob_index;
      fread(&bottom_blob_index, sizeof(int), 1, fp);

      Blob& blob = blobs[bottom_blob_index];

      blob.consumers.push_back(i);

      layer->bottoms[j] = bottom_blob_index;
    }

    layer->tops.resize(tops_size);

    for (int j = 0; j < tops_size; j++) {
      int top_blob_index;
      fread(&top_blob_index, sizeof(int), 1, fp);

      Blob& blob = blobs[top_blob_index];

      //             blob.name = std::string(blob_name);
      //             fprintf(stderr, "new blob %s\n", blob_name);

      blob.producer = i;

      layer->tops[j] = top_blob_index;
    }

    // layer specific params
    int pdlr = pd.load_param_bin(fp);

    if (pdlr != 0) {
      fprintf(stderr, "ParamDict load_param failed\n");
      continue;
    }

    //         int lr = layer->load_param_bin(fp);
    int lr = layer->load_param(pd);

    if (lr != 0) {
      fprintf(stderr, "layer load_param failed\n");
      continue;
    }

    layers[i] = layer;
  }

  return 0;
}

int Net::load_param_bin(const char* protopath)
{
  FILE* fp = fopen(protopath, "rb");

  if (!fp) {
    fprintf(stderr, "fopen %s failed\n", protopath);
    return -1;
  }

  int ret = load_param_bin(fp);

  fclose(fp);

  return ret;
}

int Net::load_model(FILE* fp)
{
  // load file
  int ret = 0;

  for (size_t i = 0; i < layers_size; i++) {
    Layer* layer = layers[i];

    int lret = layer->load_model(fp);

    if (lret != 0) {
      fprintf(stderr, "layer load_model %d failed\n", (int)i);
      ret = -1;
      break;
    }
  }

  return ret;
}

int Net::load_model(const char* modelpath)
{
  FILE* fp = fopen(modelpath, "rb");

  if (!fp) {
    fprintf(stderr, "fopen %s failed\n", modelpath);
    return -1;
  }

  int ret = load_model(fp);

  fclose(fp);

  return ret;
}

int Net::load_param(const unsigned char* _mem)
{
  if ((unsigned long)_mem & 0x3) {
    // reject unaligned memory
    fprintf(stderr, "memory not 32-bit aligned at %p\n", _mem);
    return 0;
  }

  const unsigned char* mem = _mem;

  int magic = *(int*)(mem);
  mem += 4;

  if (magic != 7767517) {
    fprintf(stderr, "param is too old, please regenerate\n");
    return 0;
  }

  int layers_size = *(int*)(mem);
  mem += 4;

  int blobs_size = *(int*)(mem);
  mem += 4;

  layers.resize(layers_size);
  blobs.resize(blobs_size);

  ParamDict pd;

  for (int i = 0; i < layers_size; i++) {
    int typeindex = *(int*)mem;
    mem += 4;

    int bottoms_size = *(int*)mem;
    mem += 4;

    int tops_size = *(int*)mem;
    mem += 4;

    Layer* layer = create_layer(typeindex);

    if (!layer) {
      int custom_index = typeindex & ~LayerType::CustomBit;
      layer = create_custom_layer(custom_index);
    }

    if (!layer) {
      fprintf(stderr, "layer %d not exists or registered\n", typeindex);
      clear();
      return 0;
    }

    //         layer->type = std::string(layer_type);
    //         layer->name = std::string(layer_name);
    //         fprintf(stderr, "new layer %d\n", typeindex);

    layer->bottoms.resize(bottoms_size);

    for (int j = 0; j < bottoms_size; j++) {
      int bottom_blob_index = *(int*)mem;
      mem += 4;

      Blob& blob = blobs[bottom_blob_index];

      blob.consumers.push_back(i);

      layer->bottoms[j] = bottom_blob_index;
    }

    layer->tops.resize(tops_size);

    for (int j = 0; j < tops_size; j++) {
      int top_blob_index = *(int*)mem;
      mem += 4;

      Blob& blob = blobs[top_blob_index];

      //             blob.name = std::string(blob_name);
      //             fprintf(stderr, "new blob %s\n", blob_name);

      blob.producer = i;

      layer->tops[j] = top_blob_index;
    }

    // layer specific params
    int pdlr = pd.load_param(mem);

    if (pdlr != 0) {
      fprintf(stderr, "ParamDict load_param failed\n");
      continue;
    }

    //         int lr = layer->load_param(mem);
    int lr = layer->load_param(pd);

    if (lr != 0) {
      fprintf(stderr, "layer load_param failed\n");
      continue;
    }

    layers[i] = layer;
  }

  return mem - _mem;
}

int Net::load_model(const unsigned char* _mem)
{
  if ((unsigned long)_mem & 0x3) {
    // reject unaligned memory
    fprintf(stderr, "memory not 32-bit aligned at %p\n", _mem);
    return 0;
  }

  const unsigned char* mem = _mem;

  for (size_t i = 0; i < layers_size; i++) {
    Layer* layer = layers[i];

    int lret = layer->load_model(mem);

    if (lret != 0) {
      fprintf(stderr, "layer load_model failed\n");
      return -1;
    }
  }

  return mem - _mem;
}

void Net::clear()
{
  blobs.clear();

  for (size_t i = 0; i < layers_size; i++) {
    delete layers[i];
  }

  layers.clear();
}

Extractor Net::create_extractor() const
{
  return Extractor(this, blobs_size);
}

int Net::find_layer_index_by_name(const char* name) const
{
  for (size_t i = 0; i < layers_size; i++) {
    const Layer* layer = layers[i];

    if (layer->name == name) {
      return i;
    }
  }

  fprintf(stderr, "find_layer_index_by_name %s failed\n", name);
  return -1;
}

int Net::custom_layer_to_index(const char* type)
{
  const int custom_layer_registry_entry_count = custom_layer_registry_size;

  for (int i = 0; i < custom_layer_registry_entry_count; i++) {
    if (strcmp(type, custom_layer_registry[i].name) == 0) {
      return i;
    }
  }

  return -1;
}

Layer* Net::create_custom_layer(int index)
{
  const int custom_layer_registry_entry_count = custom_layer_registry_size;

  if (index < 0 || index >= custom_layer_registry_entry_count) {
    return 0;
  }

  layer_creator_func layer_creator = custom_layer_registry[index].creator;

  if (!layer_creator) {
    return 0;
  }

  return layer_creator();
}

int Net::forward_layer(int layer_index, std::vector<Mat>& blob_mats, bool lightmode) const
{
  const Layer* layer = layers[layer_index];

  //     fprintf(stderr, "forward_layer %d %s\n", layer_index, layer->name.c_str());

  if (layer->one_blob_only) {
    // load bottom blob
    int bottom_blob_index = layer->bottoms[0];
    int top_blob_index = layer->tops[0];

    if (blob_mats[bottom_blob_index].dims == 0) {
      int ret = forward_layer(blobs[bottom_blob_index].producer, blob_mats, lightmode);

      if (ret != 0) {
        return ret;
      }
    }

    Mat bottom_blob = blob_mats[bottom_blob_index];

    if (lightmode) {
      // delete after taken in light mode
      blob_mats[bottom_blob_index].release();

      // deep copy for inplace forward if data is shared
      if (layer->support_inplace && *bottom_blob.refcount != 1) {
        bottom_blob = bottom_blob.clone();
      }
    }

    // forward
    if (lightmode && layer->support_inplace) {
      Mat& bottom_top_blob = bottom_blob;
      int ret = layer->forward_inplace(bottom_top_blob);

      if (ret != 0) {
        return ret;
      }

      // store top blob
      blob_mats[top_blob_index] = bottom_top_blob;
    }
    else {
      Mat top_blob;
      int ret = layer->forward(bottom_blob, top_blob);

      if (ret != 0) {
        return ret;
      }

      // store top blob
      blob_mats[top_blob_index] = top_blob;
    }

  }
  else {
    // load bottom blobs
    std::vector<Mat> bottom_blobs;
    bottom_blobs.resize(layer->bottoms_size);

    for (size_t i = 0; i < layer->bottoms_size; i++) {
      int bottom_blob_index = layer->bottoms[i];

      if (blob_mats[bottom_blob_index].dims == 0) {
        int ret = forward_layer(blobs[bottom_blob_index].producer, blob_mats, lightmode);

        if (ret != 0) {
          return ret;
        }
      }

      bottom_blobs[i] = blob_mats[bottom_blob_index];

      if (lightmode) {
        // delete after taken in light mode
        blob_mats[bottom_blob_index].release();

        // deep copy for inplace forward if data is shared
        if (layer->support_inplace && *bottom_blobs[i].refcount != 1) {
          bottom_blobs[i] = bottom_blobs[i].clone();
        }
      }
    }

    // forward
    if (lightmode && layer->support_inplace) {
      std::vector<Mat>& bottom_top_blobs = bottom_blobs;
      int ret = layer->forward_inplace(bottom_top_blobs);

      if (ret != 0) {
        return ret;
      }

      // store top blobs
      for (size_t i = 0; i < layer->tops_size; i++) {
        int top_blob_index = layer->tops[i];

        blob_mats[top_blob_index] = bottom_top_blobs[i];
      }
    }
    else {
      std::vector<Mat> top_blobs;
      top_blobs.resize(layer->tops_size);
      int ret = layer->forward(bottom_blobs, top_blobs);

      if (ret != 0) {
        return ret;
      }

      // store top blobs
      for (size_t i = 0; i < layer->tops_size; i++) {
        int top_blob_index = layer->tops[i];

        blob_mats[top_blob_index] = top_blobs[i];
      }
    }
  }

  //     fprintf(stderr, "forward_layer %d %s done\n", layer_index, layer->name.c_str());
  //     const Mat& blob = blob_mats[layer->tops[0]];
  //     fprintf(stderr, "[%-2d %-16s %-16s]  %d    blobs count = %-3d   size = %-3d x %-3d\n", layer_index, layer->type.c_str(), layer->name.c_str(), layer->tops[0], blob.c, blob.h, blob.w);

  return 0;
}

Extractor::Extractor(const Net* _net, int blobs_size) : net(_net)
{
  blob_mats.resize(blobs_size);
  lightmode = false;
  num_threads = 0;
}

void Extractor::set_light_mode(bool enable)
{
  lightmode = enable;
}

void Extractor::set_num_threads(int _num_threads)
{
  num_threads = _num_threads;
}

int Extractor::input(int blob_index, const Mat& in)
{
  if (blob_index < 0 || blob_index >= (int)blob_mats_size) {
    return -1;
  }

  blob_mats[blob_index] = in;

  return 0;
}

int Extractor::extract(int blob_index, Mat& feat)
{
  if (blob_index < 0 || blob_index >= (int)blob_mats_size) {
    return -1;
  }

  int ret = 0;

  if (blob_mats[blob_index].dims == 0) {
    int layer_index = net->blobs[blob_index].producer;

#ifdef _OPENMP
    int dynamic_current = 0;
    int num_threads_current = 1;

    if (num_threads) {
      dynamic_current = omp_get_dynamic();
      num_threads_current = omp_get_num_threads();
      omp_set_dynamic(0);
      omp_set_num_threads(num_threads);
    }

#endif

    ret = net->forward_layer(layer_index, blob_mats, lightmode);

#ifdef _OPENMP

    if (num_threads) {
      omp_set_dynamic(dynamic_current);
      omp_set_num_threads(num_threads_current);
    }

#endif
  }

  feat = blob_mats[blob_index];

  return ret;
}

int Extractor::input(const char* blob_name, const Mat& in)
{
  int blob_index = net->find_blob_index_by_name(blob_name);

  if (blob_index == -1) {
    return -1;
  }

  blob_mats[blob_index] = in;

  return 0;
}

int Extractor::extract(const char* blob_name, Mat& feat)
{
  int blob_index = net->find_blob_index_by_name(blob_name);

  if (blob_index == -1) {
    return -1;
  }

  int ret = 0;

  if (blob_mats[blob_index].dims == 0) {
    int layer_index = net->blobs[blob_index].producer;

#ifdef _OPENMP
    int dynamic_current = 0;
    int num_threads_current = 1;

    if (num_threads) {
      dynamic_current = omp_get_dynamic();
      num_threads_current = omp_get_num_threads();
      omp_set_dynamic(0);
      omp_set_num_threads(num_threads);
    }

#endif

    ret = net->forward_layer(layer_index, blob_mats, lightmode);

#ifdef _OPENMP

    if (num_threads) {
      omp_set_dynamic(dynamic_current);
      omp_set_num_threads(num_threads_current);
    }

#endif
  }

  feat = blob_mats[blob_index];

  return ret;
}

#endif

