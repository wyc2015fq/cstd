// This script converts the MNIST dataset to a lmdb (default) or
// leveldb (--backend=leveldb) format used by caffe to load data.
// Usage:
//    convert_mnist2mnist255 [FLAGS] input_image_file input_label_file
//                        output_db_file
// The MNIST dataset could be downloaded at
//    http://yann.lecun.com/exdb/mnist/

//#include <gflags/gflags.h>
//#include "logging.hpp"
//#include <google/protobuf/text_format.h>

#include <stdint.h>
#include <sys/stat.h>

#include <fstream>  // NOLINT(readability/streams)
#include <string>

//#include "boost/scoped_ptr.hpp"

#include "std/flags_c.h"
#include "std/log_c.h"
#include "std/dir_c.h"
#include "../blobdata.h"
//using namespace wstd;

int convert_dataset(const char* image_filename, const char* label_filename,
                    const char* db_path, const char* db_backend)
{
  // Open files
  std::ifstream image_file(image_filename, std::ios::in | std::ios::binary);
  std::ifstream label_file(label_filename, std::ios::in | std::ios::binary);
  CHECK(image_file) << "Unable to open file " << image_filename;
  CHECK(label_file) << "Unable to open file " << label_filename;
  // Read the magic and the meta data
  uint32_t magic;
  uint32_t num_items;
  uint32_t num_labels;
  uint32_t rows;
  uint32_t cols;
  image_file.read(reinterpret_cast<char*>(&magic), 4);
  magic = swap_endian(magic);
  CHECK_EQ(magic, 2051) << "Incorrect image file magic.";
  label_file.read(reinterpret_cast<char*>(&magic), 4);
  magic = swap_endian(magic);
  CHECK_EQ(magic, 2049) << "Incorrect label file magic.";
  image_file.read(reinterpret_cast<char*>(&num_items), 4);
  num_items = swap_endian(num_items);
  label_file.read(reinterpret_cast<char*>(&num_labels), 4);
  num_labels = swap_endian(num_labels);
  CHECK_EQ(num_items, num_labels);
  image_file.read(reinterpret_cast<char*>(&rows), 4);
  rows = swap_endian(rows);
  image_file.read(reinterpret_cast<char*>(&cols), 4);
  cols = swap_endian(cols);
  DataInfo info[1];
  info->dimtype_ = NCHW;
  info->type_ = TF_U8;
  info->shape.set(num_items, 1, rows, cols);
  info->label_num = 1;
  info->label_dim[0] = 1;
  FILE* db = fopen(db_path, "wb");
  mnist_write_head(db, info);
  // Storing to db
  char label;
  char* pixels = (char*)malloc(rows * cols);
  int count = 0;
  //blob_img->set(cols);
  //blob_img->add_dim(rows);
  //blob_labels->add_dim(1);
  LOG(INFO) << "A total of " << num_items << " items.";
  LOG(INFO) << "Rows: " << rows << " Cols: " << cols;
  for (int item_id = 0; item_id < (int)num_items; ++item_id) {
    image_file.read(pixels, rows * cols);
    label_file.read(&label, 1);
    if (0) {
      char buf[256];
      _snprintf(buf, 256, "E:/OCR_Line/chars/mnist/%d/", label);
      mkdirs(buf);
      _snprintf(buf, 256, "E:/OCR_Line/chars/mnist/%d/%d.jpg", label, item_id);
      //imwrite4(buf, rows, cols, (uchar*)pixels, cols, 1);
    }
    //blob_img->set(NCHW, TF_U8, pixels, cols, rows);
    //blob_labels->set(NCHW, TF_U8, &label, 1);
    float flabel = label;
    fwrite(pixels, 1, rows*cols, db);
    fwrite(&flabel, sizeof(float), 1, db);
    //datum.set_label(label);
    if (++count % 1000 == 0) {
      fflush(db);
      printf("item_id=%d\n", item_id);
    }
  }
  // write the last batch
  LOG(INFO) << "Processed " << count << " files.";
  free(pixels);
  fclose(db);
  return 0;
}

DEFINE_string(backend, "lmdb", "The backend for storing the result");

int convert_mnist2mnist255(int argc, char** argv)
{
  if (argc < 4) {
    printf("This script converts the MNIST dataset to\n"
           "the lmdb/leveldb format used by Caffe to load data.\n"
           "Usage:\n"
           "    convert_mnist2mnist255 [FLAGS] input_image_file input_label_file "
           "output_db_file\n"
           "The MNIST dataset could be downloaded at\n"
           "    http://yann.lecun.com/exdb/mnist/\n"
           "You should gunzip them after downloading,"
           "or directly use data/mnist/get_mnist.sh\n");
    return 0;
  }
  ParseCommandLineFlags(argc, argv, 0);
  convert_dataset(argv[1], argv[2], argv[3], FLAGS_backend);
  return 0;
}

int test_convert_mnist2mnist255() {
  _chdir("E:/OCR_Line/mnist");
  char* train[] = { "",
    "train-images-idx3-ubyte","train-labels-idx1-ubyte","./dbtrain.mnist","--backend=lmdb"
  };
  char* test[] = { "",
    "t10k-images-idx3-ubyte","t10k-labels-idx1-ubyte","./dbtest.mnist","--backend=lmdb"
  };
  convert_mnist2mnist255(countof(test), test);
  convert_mnist2mnist255(countof(train), train);
  return 0;
}
