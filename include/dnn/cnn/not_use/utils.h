
#pragma once
#include "dirent.h"
#include <iterator>
#include <string>
#include <vector>
#include <sstream>

template<typename SRC_TYPE, typename DST_TYPE>
DST_TYPE convert(const SRC_TYPE& val)
{
  stringstream ss;
  ss << val;
  DST_TYPE result;
  ss >> result;
  return result;
}

#undef min
#undef max
vector<string> get_files_in_dir(const string& dir_path)
{
  vector<string> result;
  DIR* dir;
  struct dirent* ent;

  if ((dir = opendir(dir_path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL) {
      if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
        continue;
      }

      if (ent->d_type == DT_REG) {
        result.push_back(dir_path + ent->d_name);
      }
      else if (ent->d_type == DT_DIR) {
        vector<string> sub_dir_files = get_files_in_dir(dir_path + ent->d_name + "\\");
        copy(sub_dir_files.begin(), sub_dir_files.end(), back_inserter(result));
      }
    }

    closedir(dir);
  }

  return result;
}
