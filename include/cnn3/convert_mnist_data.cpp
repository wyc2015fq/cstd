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
#include "io.hpp"
#include "db.hpp"
#include "proto.h"
#include "wstd/string.hpp"
#include "wstd/flags.hpp"


uint32_t swap_endian(uint32_t val)
{
  val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
  return (val << 16) | (val >> 16);
}

int convert_dataset(const char* image_filename, const char* label_filename,
                    const char* db_path, const string & db_backend)
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
  db::DB* db(db::GetDB(db_backend));
  db->Open(db_path, db::NEW);
  db::Transaction* txn(db->NewTransaction());
  // Storing to db
  char label;
  char* pixels = new char[rows * cols];
  int count = 0;
  string value;
  Datum datum(2);
  BlobData* blob_img = &datum[0];
  BlobData* blob_labels = &datum[1];
  blob_img->add_dim(cols);
  blob_img->add_dim(rows);
  blob_labels->add_dim(1);
  LOG(INFO) << "A total of " << num_items << " items.";
  LOG(INFO) << "Rows: " << rows << " Cols: " << cols;
  for (int item_id = 0; item_id < (int)num_items; ++item_id) {
    printf("item_id=%d\n", item_id);
    image_file.read(pixels, rows * cols);
    label_file.read(&label, 1);
    Blob_NCHW(blob_img, false, (unsigned char*)pixels, cols, rows);
    Blob_NCHW(blob_labels, false, (unsigned char*)&label, 1);
    //datum.set_label(label);
    string key_str = wstd::format_int(item_id, 8);
    value.resize(0);
    SerializeToString(&value, datum);
    txn->Put(key_str, value);
    if (++count % 1000 == 0) {
      txn->Commit();
    }
  }
  // write the last batch
  if (count % 1000 != 0) {
    txn->Commit();
  }
  LOG(INFO) << "Processed " << count << " files.";
  delete[] pixels;
  db->Close();
  delete db;
  delete txn;
  return 0;
}

int convert_mnist_data(int argc, char** argv)
{
  if (1) {
    _chdir("C:/caffe_train/mnist");
    char* aa[] = { "",
    "train-images-idx3-ubyte","train-labels-idx1-ubyte","./mnist_train_lmdb","--backend=lmdb"
    };
    argc = 5;
    argv = aa;
  }
  
  static const char* const keys3 =
    "{ backend | lmdb  | The backend for storing the result }"
    "{ s5 |       | five values scalar }";
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
  wstd::CommandLineParser parser(argc, argv, keys3);
  const string & db_backend = parser.get<string>("backend");
  convert_dataset(argv[1], argv[2], argv[3], db_backend);
  return 0;
}

