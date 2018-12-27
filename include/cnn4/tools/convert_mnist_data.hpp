// This script converts the MNIST dataset to a lmdb (default) or
// leveldb (--backend=leveldb) format used by caffe to load data.
// Usage:
//    convert_mnist_data [FLAGS] input_image_file input_label_file
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
#include "parser/cJSON.hpp"
#include "../types.h"
#include "../db.hpp"
#include "../data_transformer.hpp"
#include "wstd/string.hpp"
using namespace wstd;


uint32_t swap_endian(uint32_t val)
{
  val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
  return (val << 16) | (val >> 16);
}

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
  DB* db(GetDB(db_backend));
  db->Open(db_path, NEW);
  Transaction* txn(db->NewTransaction());
  // Storing to db
  char label;
  char* pixels = new char[rows * cols];
  int count = 0;
  Datum datum(2);
  BlobData* blob_img = &datum[0];
  BlobData* blob_labels = &datum[1];
  //blob_img->set(cols);
  //blob_img->add_dim(rows);
  //blob_labels->add_dim(1);
  LOG(INFO) << "A total of " << num_items << " items.";
  LOG(INFO) << "Rows: " << rows << " Cols: " << cols;
  if (0) {
    Cursor* cursor = db->NewCursor();
    while (cursor->valid()) {
      Datum datum2;
      printf("%s\n", cursor->key().c_str());
      string value = cursor->value();
      ParseFromString(value.c_str(), datum2);
      cursor->Next();
    }
  }
  for (int item_id = 0; item_id < (int)num_items; ++item_id) {
    printf("item_id=%d\n", item_id);
    image_file.read(pixels, rows * cols);
    label_file.read(&label, 1);
    blob_img->set(NCHW, TF_U8, pixels, cols, rows);
    blob_labels->set(NCHW, TF_U8, &label, 1);
    //datum.set_label(label);
    string key_str = wstd::format_int(item_id, 8);
    string value;
    str_append_datum(&value, datum);
    txn->Put(key_str, value);
    if (1) {
      Datum datum2;
      ParseFromString(value.c_str(), datum2);
      int ret = Datum_cmp(datum2, datum);
      assert(ret == 0);
    }
    if (++count % 1000 == 0) {
      txn->Commit();
    }
  }
  // write the last batch
  if (count % 1000 != 0) {
    txn->Commit();
  }
  blob_img->Free();
  blob_labels->Free();
  LOG(INFO) << "Processed " << count << " files.";
  delete[] pixels;
  db->Close();
  delete db;
  delete txn;
  return 0;
}

DEFINE_string(backend, "lmdb", "The backend for storing the result");

int convert_mnist_data(int argc, char** argv)
{
  if (argc < 4) {
    printf("This script converts the MNIST dataset to\n"
           "the lmdb/leveldb format used by Caffe to load data.\n"
           "Usage:\n"
           "    convert_mnist_data [FLAGS] input_image_file input_label_file "
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

int test_convert_mnist_data() {
  _chdir("E:/caffe_train/mnist");
  char* train[] = { "",
    "train-images-idx3-ubyte","train-labels-idx1-ubyte","./mnist_train_lmdb","--backend=lmdb"
  };
  char* test[] = { "",
    "t10k-images-idx3-ubyte","t10k-labels-idx1-ubyte","./mnist_test_lmdb","--backend=lmdb"
  };
  convert_mnist_data(countof(test), test);
  convert_mnist_data(countof(train), train);
  return 0;
}
