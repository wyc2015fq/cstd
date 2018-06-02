// This script converts the MNIST dataset to a lmdb (default) or
// leveldb (--backend=leveldb) format used by caffe to load data.
// Usage:
//    convert_mnist_data [FLAGS] input_image_file input_label_file
//                        output_db_file
// The MNIST dataset could be downloaded at
//    http://yann.lecun.com/exdb/mnist/

#include "../util/db_lmdb.inl"

// DEFINE_string(backend, "lmdb", "The backend for storing the result");

uint32_t swap_endian(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

void convert_mnist_data(const char* image_filename, const char* label_filename, const char* db_path) {
  // Open files
  FILE* image_file = fopen(image_filename, "rb");
  FILE* label_file = fopen(label_filename, "rb");
  if (!image_file) printf("Unable to open file %s\n", image_filename);
  if (!label_file) printf("Unable to open file %s\n", label_filename);
  // Read the magic and the meta data
  uint32_t magic;
  uint32_t num_items;
  uint32_t num_labels;
  uint32_t rows;
  uint32_t cols;
  fread((&magic), 4, 1, image_file);
  magic = swap_endian(magic);
  if (magic != 2051) { printf("Incorrect image file magic."); }
  fread((char*)(&magic), 4, 1, label_file);
  magic = swap_endian(magic);
  if (magic != 2049) { printf("Incorrect label file magic.\n"); }
  fread((char*)(&num_items), 4, 1, image_file);
  num_items = swap_endian(num_items);
  fread((char*)(&num_labels), 4, 1, label_file);
  num_labels = swap_endian(num_labels);
  CHECK_EQ(num_items, num_labels);
  fread((char*)(&rows), 4, 1, image_file);
  rows = swap_endian(rows);
  fread((char*)(&cols), 4, 1, image_file);
  cols = swap_endian(cols);


  DB db[1] = {0};
  LMDB_Open(db, db_path, DB_NEW);

  // Storing to db
  int count = 0, i;
  enum {MAXKVS = 1000};
  buf_t keys[MAXKVS] = {0};
  buf_t values[MAXKVS] = {0};

  printf("A total of %d items.\n", num_items);
  printf("Rows: %d Cols: \n", rows, cols);
  for (i = 0; i < num_items; ++i) {
    int pos = i%MAXKVS;
    uchar label=0;
    bfsetsize(keys+pos, 4);
    *((uint*)(keys[pos].data)) = i;
    bfsetsize(values+pos, rows * cols+4);
    fread(values[pos].data+4, rows * cols, 1, image_file);
    fread(&label, 1, 1, label_file);
    *((uint*)(values[pos].data)) = label;
    lz4_compress(values+pos, values+pos);
    //lz4_uncompress(values+pos, values+pos);

    if (++count % MAXKVS == 0) {
      db_Commit(db, keys, values, MAXKVS);
    }
  }
  // write the last batch
  if (count % 1000 != 0) {
    db_Commit(db, keys, values, count);
  }
  printf("Processed %d files.", count);
  db_Close(db);
  for (i = 0; i < MAXKVS; ++i) {
    bffree(keys+i);
    bffree(values+i);
  }
  return ;
}
