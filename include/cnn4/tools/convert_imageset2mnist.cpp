

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
#include "wstd/filesystem.hpp"
#include "imgio/imgio.h"



using namespace std;
using namespace wstd;

DEFINE_bool(gray, false, "When this option is on, treat images as grayscale ones");
DEFINE_bool(shuffle, true, "Randomly shuffle the order of images and their labels");
DEFINE_string(backend, "lmdb", "The backend (lmdb, leveldb) for storing the result");
DEFINE_string(typelist, "if", "typelist");
DEFINE_int32(resize_width, 32, "Width images are resized to");
DEFINE_int32(resize_height, 32, "Height images are resized to");
//DEFINE_bool(check_size, false, "When this option is on, check that all the datum have the same size");
//DEFINE_bool(encoded, true, "When this option is on, the encoded image will be save in datum");
//DEFINE_string(encode_type, "", "Optional: What type should we encode the image as ('png','jpg',...).");
DEFINE_string(root_folder, "", "root_folder");


size_t rfind_splash(const std::string& line, size_t pos) {
  size_t pos1 = line.rfind('/', pos);
  if (pos1>line.size()) {
    pos1 = line.rfind('\\', pos);
  }
  if (pos1 > line.size()) {
    pos1 = 0;
  }
  return pos1;
}

//int str2Datum(const char* str, char type, )

int str2vec(const char* str, vector<float>& vec) {
  vector<string> strs;
  split(strs, str, " ");
  vec.resize(strs.size());
  int i;
  for (i = 0; i < vec.size(); ++i) {
    vec[i] = atof(strs[i].c_str());
  }
  return vec.size();
}

int convert_imageset2mnist(int argc, char** argv)
{
  SetUsageMessage("Convert a set of images to the leveldb/lmdb\n"
    "format used as input for Caffe.\n"
    "Usage:\n"
    "    convert_imageset [FLAGS] ROOTFOLDER/ LISTFILE DB_NAME\n"
    "The ImageNet dataset for the training demo is at\n"
    "    http://www.image-net.org/download-images\n");
  ParseCommandLineFlags(argc, argv, true);

  std::vector<string> strs;
  int len = readlines(argv[1], strs);
  if (len<=0) {
    printf("failed to open %s\n", argv[2]);
  }
  //const char* typelist = "ifff";
  save_db(argv[2], strs);
  const char* db_fn = argv[2];
  vector<string>& lines = strs;

  const char* typelist = FLAGS_typelist;
  const char* root_folder = FLAGS_root_folder;
  // Create new DB
  int h = FLAGS_resize_height;
  int w = FLAGS_resize_width;
  bool is_color = !FLAGS_gray;
  int cn = is_color ? 3 : 1;
  DB* db = GetDB(FLAGS_backend);
  db->Open(db_fn, NEW);
  Transaction* txn = db->NewTransaction();
  // Storing to db
  Datum datum;
  int count = 0;
  int i, n = strlen(typelist);
  datum.resize(n);
  img_t im[1] = { 0 };
  for (int line_id = 0; line_id < lines.size(); ++line_id) {
    bool status;
    vector<string> strs;
    split(strs, lines[line_id], ";");
    LOG_IF(INFO, strs.size() != n) << "strs.size()!=n";
    for (i = 0; i<n; ++i) {
      string fn = strs[i];
      BlobData* blob = &datum[i];
      char c = typelist[i];
      if ('i' == c) {
        printf("%s\n", fn.c_str());
        if (1) {
          string fullfn = root_folder + fn;
          if (imread(fullfn.c_str(), cn, 1, im)) {
            if (im->h != h || im->w != w) {
              imresize(im, h, w, im);
            }
            blob->set(NCHW, TF_U8, im->data, w, h, cn);
          }
        }
      }
      else if ('f' == c) {
        vector<float> vec;
        str2vec(fn.c_str(), vec);
        blob->set(NCHW, TF_F32, vec.data(), 1, 1, (int)vec.size(), 1);
      }
    }
    // sequential
    string key_str = format_int(line_id, 8);
    string value;
    str_append_datum(&value, datum);
    txn->Put(key_str, value);

    if (++count % 1000 == 0) {
      txn->Commit();
    }
  }
  // write the last batch
  if (count % 1000 != 0) {
    txn->Commit();
    LOG(INFO) << "Processed " << count << " files.";
  }
  return 0;
}

int test_convert_imageset2mnist() {
  //test_dbread();
#ifdef _DEBUG
  _chdir("E:/OCR_Line/chars/");
  _chdir("E:/OCR_Line/hans/");
  _chdir("E:/OCR_Line/lines/");
#endif
  char* test[] = { "",
    "test.txt", "./dbtest","--backend=lmdb", "--typelist=if", "--resize_width=280", "--resize_height=32", "--gray=false"
  };
  char* train[] = { "",
    "train.txt", "./dbtrain","--backend=lmdb", "--typelist=if", "--resize_width=280", "--resize_height=32", "--gray=false"
  };
  convert_imageset2mnist(countof(test), test);
  convert_imageset2mnist(countof(train), train);
  return 0;
}


