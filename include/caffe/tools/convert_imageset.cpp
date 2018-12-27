// This program converts a set of images to a lmdb/leveldb by storing them
// as Datum proto buffers.
// Usage:
//   convert_imageset [FLAGS] ROOTFOLDER/ LISTFILE DB_NAME
//
// where ROOTFOLDER is the root folder that holds all the images, and LISTFILE
// should be a list of files as well as their labels, in the format as
//   subfolder1/file1.JPEG 7
//   ....

#define countof(arr) sizeof(arr)/sizeof(arr[0])

#include <algorithm>
#include <fstream>  // NOLINT(readability/streams)
#include <string>
#include <utility>
#include <vector>
#include <direct.h>

#include "caffe/proto/caffe_proto.h"
#include "caffe/util/db.hpp"
#include "caffe/util/io.hpp"
#include "caffe/util/rng.hpp"
//#include "caffe/libcaffe.cpp"

#include "caffe/common.cpp"
//#include "caffe/proto/caffe_proto.cc"
#include "caffe/util/db.cpp"
#include "../util/io.cpp"
//#include "caffe/util/db_leveldb.cpp"
#include "caffe/util/db_lmdb.cpp"

#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#endif  // USE_OPENCV

#include "std/flags_c.h"
#include "std/log_c.h"
#include "wstd/string.hpp"
#include "wstd/filesystem.hpp"

using namespace std;
using namespace caffe;  // NOLINT(build/namespaces)
using std::pair;


DEFINE_bool(gray, false, "When this option is on, treat images as grayscale ones");
DEFINE_bool(shuffle, true, "Randomly shuffle the order of images and their labels");
DEFINE_string(backend, "lmdb", "The backend (lmdb, leveldb) for storing the result");
DEFINE_string(typelist, "ifff", "typelist");
DEFINE_int32(resize_width, 0, "Width images are resized to");
DEFINE_int32(resize_height, 0, "Height images are resized to");
DEFINE_bool(check_size, false, "When this option is on, check that all the datum have the same size");
DEFINE_bool(encoded, true, "When this option is on, the encoded image will be save in datum");
DEFINE_string(encode_type, "", "Optional: What type should we encode the image as ('png','jpg',...).");


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

int save_db(const char* db_fn, const char* encode_type, bool is_color, const char* root_folder, const char* typelist, const vector<string>& lines) {
  int resize_height = FLAGS_resize_height;
  int resize_width = FLAGS_resize_width;
  // Create new DB
  shared_ptr<db::DB> db(db::GetDB(FLAGS_backend));
  db->Open(db_fn, db::NEW);
  shared_ptr<db::Transaction> txn(db->NewTransaction());
  // Storing to db
  Datum datum;
  int count = 0;
  int data_size = 0;
  bool data_size_initialized = false;
  bool encoded = encode_type!=NULL;
  bool check_size = true;
  int i, n = strlen(typelist);
  for (i=0; typelist[i]; ++i) {
    datum.add_blob();
  }
  for (int line_id = 0; line_id < lines.size(); ++line_id) {
    bool status;
    vector<string> strs;
    split(strs, lines[line_id], ";");
    LOG_IF(INFO, strs.size() != n) << "strs.size()!=n";
    for (i=0; i<n; ++i) {
      string fn = strs[i];
      BlobData* blob = datum.mutable_blob(i);
      char c = typelist[i];
      if ('i' == c) {
        std::string enc = encode_type ? encode_type : "";
        printf("%s\n", fn.c_str());
        if (encoded && !enc.size()) {
          // Guess the encoding type from the file name
          size_t p = fn.rfind('.');
          if (p == fn.npos) {
            LOG(WARNING) << "Failed to guess the encoding of '" << fn << "'";
          }
          enc = fn.substr(p);
          std::transform(enc.begin(), enc.end(), enc.begin(), ::tolower);
        }
        status = ReadImageToBlob(root_folder + fn, resize_height, resize_width, is_color, enc, blob);
        if (status == false) {
          LOG(WARNING) << "Failed to ReadImageToBlob '" << fn << "'";
          continue;
        }
        if (check_size) {
          if (!data_size_initialized) {
            data_size = blob_channels(*blob) * blob_height(*blob) * blob_width(*blob);
            data_size_initialized = true;
          }
          else {
            const std::string & data = blob->data();
            CHECK_EQ(data.size(), data_size) << "Incorrect data field size "
              << data.size();
          }
        }
      } else if ('f' == c) {
        vector<float> vec;
        str2vec(fn.c_str(), vec);
        Blob_NCHW(blob, false, vec.data(), (int)vec.size());
      }
    }
    // sequential
    string key_str = format_int(line_id, 8);
    // Put in db
    string out;
    CHECK(datum.SerializeToString(&out));
    txn->Put(key_str, out);
    if (++count % 1000 == 0) {
      // Commit db
      txn->Commit();
      txn.reset(db->NewTransaction());
      LOG(INFO) << "Processed " << count << " files.";
    }
  }
  // write the last batch
  if (count % 1000 != 0) {
    txn->Commit();
    LOG(INFO) << "Processed " << count << " files.";
  }
  return 0;
}

int read_db(const char* db_fn)
{
  shared_ptr<db::DB> db(db::GetDB(FLAGS_backend));
  db->Open(db_fn, db::READ);
  SHARED_PTR<db::Cursor> cursor(db->NewCursor());
  for (int i = 0; cursor->valid(); i++) {
    Datum datum;
    printf("=== %d ===\n", i);
    // TODO deserialize in-place instead of copy?
    datum.ParseFromString(cursor->value());
    for (int j = 0; j < datum.blob_size(); ++j) {
      const BlobData& blob = datum.blob(j);
      int num = blob_size(blob);
      printf("[");
      for (int k = 0; k < num; k++) {
        double lk = blob_getValue(blob, k);
        printf("%s%lg", (k > 0)? " ":"", lk);
      }
      printf("]\n");
    }
    cursor->Next();
  }
  return 0;
}

int convert_db_mutil(int argc, char** argv)
{
  SetUsageMessage("Convert a set of images to the leveldb/lmdb\n"
    "format used as input for Caffe.\n"
    "Usage:\n"
    "    convert_imageset [FLAGS] ROOTFOLDER/ LISTFILE DB_NAME\n"
    "The ImageNet dataset for the training demo is at\n"
    "    http://www.image-net.org/download-images\n");
  ParseCommandLineFlags(argc, argv, true);
  if (argc < 4) {
    ShowUsageWithFlagsRestrict(argv[0], "tools/convert_imageset");
    return 1;
  }
  const bool is_color = !FLAGS_gray;
  const bool check_size = FLAGS_check_size;
  const bool encoded = FLAGS_encoded;
  const string encode_type = FLAGS_encode_type;
  printf("shuffle=%d,color=%d, check_size=%d, encoded=%d, resize_w=%d,resize_h=%d\n",
    FLAGS_shuffle, is_color, check_size, encoded,
    FLAGS_resize_width, FLAGS_resize_height);
  std::vector<string> strs;
  int len = readlines(argv[2], strs);
  if (len<=0) {
    printf("failed to open %s\n", argv[2]);
  }
  //const char* typelist = "ifff";
  save_db(argv[3], NULL, is_color, argv[1], FLAGS_typelist, strs);
  return 0;
}

int convert_db(int argc, char** argv)
{
  SetUsageMessage("Convert a set of images to the leveldb/lmdb\n"
                          "format used as input for Caffe.\n"
                          "Usage:\n"
                          "    convert_imageset [FLAGS] ROOTFOLDER/ LISTFILE DB_NAME\n"
                          "The ImageNet dataset for the training demo is at\n"
                          "    http://www.image-net.org/download-images\n");
  ParseCommandLineFlags(argc, argv, true);
  if (argc < 4) {
    ShowUsageWithFlagsRestrict(argv[0], "tools/convert_imageset");
    return 1;
  }
  const bool is_color = !FLAGS_gray;
  const bool check_size = FLAGS_check_size;
  const bool encoded = FLAGS_encoded;
  const string encode_type = FLAGS_encode_type;
  printf("shuffle=%d,color=%d, check_size=%d, encoded=%d, resize_w=%d,resize_h=%d\n",
         FLAGS_shuffle, is_color, check_size, encoded,
         FLAGS_resize_width, FLAGS_resize_height);
  std::ifstream infile(argv[2]);
  if (!infile.is_open()) {
    printf("failed to open %s\n", argv[2]);
  }
  std::vector<std::pair<std::string, vector<int> > > lines;
  std::string filename;
  int label;
  string line;
  while (getline(infile, line)) {
    size_t postfixpos = line.rfind(".");
    if (postfixpos == string::npos) {
      LOG(INFO) << "wrong format:" << line;
      continue;
    }
    size_t firstblank = line.find(' ', postfixpos + 1);
    filename = line.substr(0, firstblank);
    string strlabels = line.substr(firstblank + 1);
    if (firstblank > line.size()) {
      size_t pos1 = rfind_splash(line, -1);
      size_t pos2 = rfind_splash(line, pos1 - 1);
      if (pos2 > 0) {
        ++pos2;
      }
      strlabels = line.substr(pos2, pos1- pos2);
    }
    std::istringstream iss(strlabels);
    vector<int> labels;
    while (iss >> label) {
      labels.push_back(label);
    }
    lines.push_back(std::make_pair(filename, labels));
  }
  std::cout << "found " << lines.size() << " images" << std::endl;
  if (FLAGS_shuffle) {
    // randomly shuffle data
    LOG(INFO) << "Shuffling data";
    shuffle(lines.begin(), lines.end());
  }
  LOG(INFO) << "A total of " << lines.size() << " images.";
  if (encode_type.size() && !encoded) {
    LOG(INFO) << "encode_type specified, assuming encoded=true.";
  }
  //int resize_height = std::max<int>(0, FLAGS_resize_height);
  // int resize_width = std::max<int>(0, FLAGS_resize_width);
  int resize_height = FLAGS_resize_height;
  int resize_width = FLAGS_resize_width;
  // Create new DB
  shared_ptr<db::DB> db(db::GetDB(FLAGS_backend));
  db->Open(argv[3], db::NEW);
  shared_ptr<db::Transaction> txn(db->NewTransaction());
  // Storing to db
  std::string root_folder(argv[1]);
  Datum datum;
  int count = 0;
  int data_size = 0;
  bool data_size_initialized = false;
  BlobData* blob_img = datum.add_blob();
  BlobData* blob_labels = datum.add_blob();
  for (int line_id = 0; line_id < lines.size(); ++line_id) {
    bool status;
    std::string enc = encode_type;
    if (encoded && !enc.size()) {
      // Guess the encoding type from the file name
      string fn = lines[line_id].first;
      size_t p = fn.rfind('.');
      if (p == fn.npos) {
        LOG(WARNING) << "Failed to guess the encoding of '" << fn << "'";
      }
      enc = fn.substr(p);
      std::transform(enc.begin(), enc.end(), enc.begin(), ::tolower);
    }
    status = ReadImageToBlob(root_folder + lines[line_id].first, resize_height, resize_width, is_color, enc, blob_img);
    LabelsToBlob(lines[line_id].second, blob_labels);
    if (status == false) { continue; }
    if (check_size) {
      if (!data_size_initialized) {
        data_size = blob_channels(*blob_img) * blob_height(*blob_img) * blob_width(*blob_img);
        data_size_initialized = true;
      } else {
        const std::string & data = blob_img->data();
        CHECK_EQ(data.size(), data_size) << "Incorrect data field size "
                                         << data.size();
      }
    }
    // sequential
    string key_str = format_int(line_id, 8) + "_" + lines[line_id].first;
    // Put in db
    string out;
    CHECK(datum.SerializeToString(&out));
    txn->Put(key_str, out);
    if (++count % 1000 == 0) {
      // Commit db
      txn->Commit();
      txn.reset(db->NewTransaction());
      LOG(INFO) << "Processed " << count << " files.";
    }
  }
  // write the last batch
  if (count % 1000 != 0) {
    txn->Commit();
    LOG(INFO) << "Processed " << count << " files.";
  }
  return 0;
}

int read2img_db(int argc, char** argv)
{
#ifdef _DEBUG
  argc = 4;
  argv[1] = "I:\\OCR_Line\\synth_english\\dbtest\\";//db folder
  argv[2] = "I:\\OCR_Line\\synth_english\\db_read_test\\";//save folder
  argv[3] = "100";//record num
  argv[4] = "I:\\OCR_Line\\synth_english\\db_read_test_list.txt";
#endif
  if (argc < 5) {
    printf("exe dbfolder dstfolder readnum outputlist\n");
    return 0;
  }
  string dbfolder = argv[1];
  string dstfolder = argv[2];
  int num = atoi(argv[3]);
  _mkdir(dstfolder.c_str());
  shared_ptr<db::DB> db(db::GetDB("leveldb"));
  db->Open(dbfolder.c_str(), db::READ);
  SHARED_PTR<db::Cursor> cursor(db->NewCursor());
  std::ofstream ofs(argv[4]);
  for (int i = 0; i < num; i++) {
    Datum datum;
    // TODO deserialize in-place instead of copy?
    datum.ParseFromString(cursor->value());
    cv::Mat img = DecodeDatumToCVMat(datum.blob(0), true);
    char name[100];
    sprintf(name, "%05d.png", i);
    ofs << name;
    const BlobData& blob_label = datum.blob(1);
    int label_size = blob_size(blob_label);
    for (int j = 0; j < label_size; j++) {
      int lj = (int)blob_getValue(blob_label, j);
      ofs << " " << lj;
    }
    ofs << std::endl;
    string dstfile = dstfolder + name;
    cv::imwrite(dstfile, img);
    // go to the next iter
    cursor->Next();
    if (!cursor->valid()) {
      break;
    }
  }
  return 0;
}

int convert_imageset(int argc, char** argv)
{
#ifdef USE_OPENCV
  InitGoogleLogging(argv[0]);
  // Print output to stderr (while still logging)
  FLAGS_alsologtostderr = 1;
  //FLAGS_alsologtostderr
#ifdef _DEBUG
  const char* path;
  path = "C:/caffe_train/ocr";
  path = "E:/data/ew_id/mtcnn/48";
  _chdir(path);
  char* imdir = "E:/data/ew_id/mtcnn/48/";
  char* argv_[] = {"<bin>", imdir, "images/train.txt", "lmdb/train1", "--gray=false", "--resize_width=48", "--resize_height=48", "--typelist=ifff" };
    argc = countof(argv_);
    argv = argv_;
#endif
  convert_db_mutil(argc, argv);
  //read_db(argv_[3]);
  //convert_db(argc, argv);
#else
  LOG(FATAL) << "This tool requires OpenCV; compile with USE_OPENCV.";
#endif  // USE_OPENCV
  return 0;
}
