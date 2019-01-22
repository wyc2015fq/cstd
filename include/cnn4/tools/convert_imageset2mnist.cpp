

#include <stdint.h>
#include <sys/stat.h>

#include <fstream>  // NOLINT(readability/streams)
#include <string>

//#include "boost/scoped_ptr.hpp"

#include "std/log_c.h"
#include "std/flags_c.h"
#include "std/fileio_c.h"
//#include "../types.h"
#include "wstd/string.hpp"
#include "wstd/filesystem.hpp"
#include "imgio/imgio.h"
#include "../blobdata.h"
#ifdef _DEBUG
#include "std/gui_c.h"
#endif


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

  FILE* pf = fopen(argv[1], "rb");
  if (NULL==pf) {
    printf("failed to open %s\n", argv[2]);
  }
  //const char* typelist = "ifff";
  const char* db_fn = argv[2];
  const char* typelist = FLAGS_typelist;
  const char* root_folder = FLAGS_root_folder;
  // Create new DB
  int h = FLAGS_resize_height;
  int w = FLAGS_resize_width;
  bool is_color = !FLAGS_gray;
  int cn = is_color ? 3 : 1;
  // Storing to db
  int count = 0;
  int i, n = strlen(typelist);
  img_t im[1] = { 0 };
  char buf[1024];
  int inited = 0;
  DataInfo info[1];
  int img_size = h*w*cn;
  memset(info, 0, sizeof(info));
  info->shape.set(1, cn, h, w);
  const char* outfilename = argv[2];
  FILE* out_pf = NULL;
  if (CheckFileExist(outfilename)) {
    DataInfo info2[1];
    int64_t db_file_size = 0;
    {
      FILE* pf2 = fopen(outfilename, "rb");
      int64_t off = 0;
      db_file_size = fsize(pf2);
      if (db_file_size > sizeof(DataInfo)) {
        int t = mnist_read_head(pf2, info2);
        bool t1 = info2->shape == info->shape;
        bool t2 = info2->label_num == info2->label_num;
        int size1 = img_size;
        for (i = 0; i < n - 1; ++i) {
          size1 += info2->label_dim[i] * sizeof(float);
        }
        if (info2->shape==info->shape && info2->label_num== info2->label_num) {
          int num = (db_file_size - t) / size1;
          off = t + (num - 1)*size1;
#ifdef _DEBUG
          if (1) {
            _fseeki64(pf2, off, SEEK_SET);
            imsetsize(im, h, w, cn, 1);
            int tt = fread(im->data, 1, img_size, pf2);
            imshow_(im); waitkey(-1);
          }
#endif
          for (; fgets(buf, 1024, pf) && count<num; ) {
            ++count;
          }
        }
      }
      fclose(pf2);
      if (off > 0) {
        out_pf = fopen(outfilename, "wb+");
        _fseeki64(out_pf, off, SEEK_SET);
      }
    }
  }
  if (NULL== out_pf) {
    out_pf = fopen(outfilename, "wb");
  }
  for (; fgets(buf, 1024, pf); ) {
    bool status;
    vector<vector<float> > vecs;
    vector<string> strs;
    split(strs, buf, ";");
    if (1 == strs.size()) {
      char* p = strchr(buf, ' ');
      strs.resize(2);
      strs[0].assign(buf, p);
      strs[1].assign(p+1);
    }
    LOG_IF(INFO, strs.size() != n) << "strs.size()!=n";
    for (i = 0; i<n; ++i) {
      string fn = strs[i];
      char c = typelist[i];
      if ('i' == c) {
        //printf("%s\n", fn.c_str());
        string fullfn = root_folder + fn;
        if (imread(fullfn.c_str(), cn, 1, im)) {
          if (im->h != h || im->w != w) {
            imresize(im, h, w, im);
          }
        }
        else {
          continue;
        }
      }
      else if ('f' == c) {
        vector<float> vec;
        str2vec(fn.c_str(), vec);
        vecs.push_back(vec);
      }
    }
    ++count;
    LOG_IF(INFO, vecs.size() != (n-1)) << "vecs.size()!=n-1";
    if (!inited) {
      inited = 1;
      info->dimtype_ = NCHW;
      info->type_ = TF_U8;
      info->label_num = n-1;
      info->shape.n = count;
      for (i = 0; i < (n - 1); ++i) {
        info->label_dim[i] = vecs[i].size();
      }
      mnist_write_head(out_pf, info);
    }
    fwrite(im->data, 1, img_size, out_pf);
    for (i = 0; i < (n - 1); ++i) {
      LOG_IF(INFO, vecs[i].size() != info->label_dim[i]) << "vecs[i].size() != info->label_dim[i]";
      fwrite(vecs[i].data(), sizeof(float), info->label_dim[i], out_pf);
    }
    if (count % 1000 == 0) {
      // Commit db
      //fseek(out_pf, 0, SEEK_SET);
      //mnist_write_head(out_pf, info);
      fflush(out_pf);
      LOG(INFO) << "Processed " << count << " files.";
    }
    // sequential
    // set(NCHW, TF_U8, im->data, w, h, cn);
    // set(NCHW, TF_F32, vec.data(), 1, 1, (int)vec.size(), 1);
  }
  fclose(pf);
  fclose(out_pf);
  // write the last batch
  return 0;
}

int test_convert_imageset2mnist() {
  //test_dbread();
  char a = ENDIANNESS;
  char b = (char)*(int*)"l??b";
  char c = b;
#ifdef _DEBUG
  _chdir("E:/OCR_Line/chars/");
  _chdir("E:/OCR_Line/hans/");
  _chdir("E:/OCR_Line/lines/");
  _chdir("D:/OCR_Line/lines/han200w/");
#endif
  char* test[] = { "",
    "test.txt", "./dbtest.mnist","--typelist=if", "--resize_width=280", "--resize_height=32", "--gray=true"
  };
  char* train[] = { "",
    "train.txt", "./dbtrain.mnist","--typelist=if", "--resize_width=280", "--resize_height=32", "--gray=true"
  };
  convert_imageset2mnist(countof(test), test);
  convert_imageset2mnist(countof(train), train);
  return 0;
}


