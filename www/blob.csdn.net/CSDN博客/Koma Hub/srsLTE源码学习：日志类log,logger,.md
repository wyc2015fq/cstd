# srsLTE源码学习：日志类log,logger, - Koma Hub - CSDN博客
2019年04月08日 21:52:51[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：17
**Table of Contents**
[log.h    lib\include\srslte\common    4658    2/28/2019    53](#log.h%C2%A0%C2%A0%20%C2%A0lib%5Cinclude%5Csrslte%5Ccommon%C2%A0%C2%A0%20%C2%A04658%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A053)
[logger.h    lib\include\srslte\common    1301    2/28/2019](#logger.h%C2%A0%C2%A0%20%C2%A0lib%5Cinclude%5Csrslte%5Ccommon%C2%A0%C2%A0%20%C2%A01301%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A0)
[logger_file.cc    lib\src\common    3329    2/28/2019    80](#logger_file.cc%C2%A0%C2%A0%20%C2%A0lib%5Csrc%5Ccommon%C2%A0%C2%A0%20%C2%A03329%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A080)
[logger_file.h    lib\include\srslte\common    2253    2/28/2019](#logger_file.h%C2%A0%C2%A0%20%C2%A0lib%5Cinclude%5Csrslte%5Ccommon%C2%A0%C2%A0%20%C2%A02253%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A0)
[logger_stdout.h    lib\include\srslte\common    1481    2/28/2019    6](#logger_stdout.h%C2%A0%C2%A0%20%C2%A0lib%5Cinclude%5Csrslte%5Ccommon%C2%A0%C2%A0%20%C2%A01481%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A06)
[logger_test.cc    lib\test\common    2591    2/28/2019    70](#logger_test.cc%C2%A0%C2%A0%20%C2%A0lib%5Ctest%5Ccommon%C2%A0%C2%A0%20%C2%A02591%C2%A0%C2%A0%20%C2%A02%2F28%2F2019%C2%A0%C2%A0%20%C2%A070)
### log.h    lib\include\srslte\common    4658    2/28/2019    53
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
/******************************************************************************
 *  File:         log.h
 *
 *  Description:  Abstract logging service
 *
 *  Reference:
 *****************************************************************************/
#ifndef SRSLTE_LOG_H
#define SRSLTE_LOG_H
#include <stdint.h>
#include <string>
namespace srslte {
typedef enum {
  LOG_LEVEL_NONE = 0,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_N_ITEMS
} LOG_LEVEL_ENUM;
static const char log_level_text[LOG_LEVEL_N_ITEMS][16] = {"None   ",
                                                           "Error  ",
                                                           "Warning",
                                                           "Info   ",
                                                           "Debug  "};
static const char log_level_text_short[LOG_LEVEL_N_ITEMS][16] = {"[-]",
                                                                 "[E]",
                                                                 "[W]",
                                                                 "[I]",
                                                                 "[D]"};
class log
{
public:
  log() {
    service_name = "";
    tti = 0;
    level = LOG_LEVEL_NONE;
    hex_limit = 0;
    show_layer_en = true;
    add_string_en = false;
    level_text_short = true;
  }
  log(std::string service_name_) {
    service_name = service_name_;
    tti = 0;
    level = LOG_LEVEL_NONE;
    hex_limit = 0;
    show_layer_en = true;
    add_string_en = false;
    level_text_short = true;
  }
  virtual ~log() {};
  // This function shall be called at the start of every tti for printing tti
  void step(uint32_t tti_) {
    tti = tti_;
    add_string_en  = false;
  }
  void prepend_string(std::string s) {
    add_string_en  = true;
    add_string_val = s;
  }
  uint32_t get_tti() {
    return tti;
  }
  void set_level(LOG_LEVEL_ENUM l) {
    level = l;
  }
  LOG_LEVEL_ENUM get_level() {
    return level;
  }
  void set_hex_limit(int limit) {
    hex_limit = limit;
  }
  int get_hex_limit() {
    return hex_limit;
  }
  void set_log_level_short(bool enable) {
    level_text_short = enable;
  }
  void show_layer(bool enable) {
    show_layer_en = enable;
  }
  // Pure virtual methods for logging
  virtual void console(const char * message, ...) __attribute__ ((format (printf, 2, 3))) = 0;
  virtual void error(const char * message, ...)   __attribute__ ((format (printf, 2, 3))) = 0;
  virtual void warning(const char * message, ...) __attribute__ ((format (printf, 2, 3))) = 0;
  virtual void info(const char * message, ...)    __attribute__ ((format (printf, 2, 3))) = 0;
  virtual void debug(const char * message, ...)   __attribute__ ((format (printf, 2, 3))) = 0;
  // Same with hex dump
  virtual void error_hex(const uint8_t *, int, const char *, ...)   __attribute__((format (printf, 4, 5)))
    {error("error_hex not implemented.\n");}
  virtual void warning_hex(const uint8_t *, int, const char *, ...) __attribute__((format (printf, 4, 5)))
    {error("warning_hex not implemented.\n");}
  virtual void info_hex(const uint8_t *, int, const char *, ...)    __attribute__((format (printf, 4, 5)))
    {error("info_hex not implemented.\n");}
  virtual void debug_hex(const uint8_t *, int, const char *, ...)   __attribute__((format (printf, 4, 5)))
    {error("debug_hex not implemented.\n");}
protected:
  std::string get_service_name() { return service_name; }
  uint32_t        tti;
  LOG_LEVEL_ENUM  level;
  int             hex_limit;
  std::string     service_name;
  bool        show_layer_en;
  bool        level_text_short;
  bool        add_string_en;
  std::string add_string_val;
};
} // namespace srslte
#endif // SRSLTE_LOG_H
```
### logger.h    lib\include\srslte\common    1301    2/28/2019    
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
/******************************************************************************
 * File:        logger.h
 * Description: Interface for logging output
 *****************************************************************************/
#ifndef SRSLTE_LOGGER_H
#define SRSLTE_LOGGER_H
#include <stdio.h>
#include <string>
namespace srslte {
class logger
{
public:
  virtual void log(std::string *msg) = 0;
};
} // namespace srslte
#endif // SRSLTE_LOGGER_H
```
### logger_file.cc    lib\src\common    3329    2/28/2019    80
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
#define LOG_BUFFER_SIZE 1024*32
#include "srslte/common/logger_file.h"
using namespace std;
namespace srslte{
logger_file::logger_file()
  :logfile(NULL)
  ,is_running(false)
  ,cur_length(0)
  ,max_length(0)
{}
logger_file::~logger_file() {
  if(is_running) {
    log(new std::string("Closing log\n"));
    pthread_mutex_lock(&mutex);
    is_running = false;
    pthread_cond_signal(¬_empty);  // wakeup thread and let it terminate
    pthread_mutex_unlock(&mutex);
    wait_thread_finish();
    flush();
    if (logfile) {
      fclose(logfile);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(¬_empty);
  }
}
void logger_file::init(std::string file, int max_length_) {
  pthread_mutex_init(&mutex, NULL); 
  pthread_cond_init(¬_empty, NULL);
  max_length = (int64_t)max_length_*1024;
  name_idx = 0;
  filename = file;
  logfile = fopen(filename.c_str(), "w");
  if(logfile == NULL) {
    printf("Error: could not create log file, no messages will be logged!\n");
  }
  is_running = true;
  start(-2);
}
void logger_file::log(const char *msg) {
  log(new std::string(msg));
}
void logger_file::log(str_ptr msg) {
  pthread_mutex_lock(&mutex);
  buffer.push_back(msg);
  pthread_cond_signal(¬_empty);
  pthread_mutex_unlock(&mutex);
}
void logger_file::run_thread() {
  while(is_running) {
    pthread_mutex_lock(&mutex);
    while(buffer.empty()) {
      pthread_cond_wait(¬_empty, &mutex);
      if(!is_running) return; // Thread done. Messages in buffer will be handled in flush.
    }
    str_ptr s = buffer.front();
    int n = 0;
    if(logfile)
      n = fprintf(logfile, "%s", s->c_str());
    delete s; 
    buffer.pop_front();
    pthread_mutex_unlock(&mutex);
    if (n > 0) {
      cur_length += (int64_t) n;
      if (cur_length >= max_length && max_length > 0) {
        fclose(logfile);
        name_idx++;
        char numstr[21]; // enough to hold all numbers up to 64-bits
        sprintf(numstr, ".%d", name_idx);
        string newfilename = filename + numstr ;
        logfile = fopen(newfilename.c_str(), "w");
        if(logfile==NULL) {
          printf("Error: could not create log file, no messages will be logged!\n");
        }
        cur_length = 0;
      }
    }
  }
}
void logger_file::flush() {
  std::deque<str_ptr>::iterator it;
  for(it=buffer.begin();it!=buffer.end();it++)
  {
    str_ptr s = *it; 
    if(logfile)
      fprintf(logfile, "%s", s->c_str());
    delete s; 
  }
}
} // namespace srsue
```
### logger_file.h    lib\include\srslte\common    2253    2/28/2019    
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
/******************************************************************************
 * File:        logger_file.h
 * Description: Common log object. Maintains a queue of log messages
 *              and runs a thread to read messages and write to file.
 *              Multiple producers, single consumer. If full, producers
 *              increase queue size. If empty, consumer blocks.
 *****************************************************************************/
#ifndef SRSLTE_LOGGER_FILE_H
#define SRSLTE_LOGGER_FILE_H
#include <stdio.h>
#include <deque>
#include <string>
#include "srslte/common/logger.h"
#include "srslte/common/threads.h"
namespace srslte {
typedef std::string* str_ptr;
class logger_file : public thread, public logger
{
public:
  logger_file();
  logger_file(std::string file);
  ~logger_file();
  void init(std::string file, int max_length = -1);
  // Implementation of log_out
  void log(str_ptr msg);
  void log(const char *msg);
private:
  void run_thread(); 
  void flush();
  uint32_t              name_idx;
  int64_t               max_length;
  int64_t               cur_length;
  FILE*                 logfile;
  bool                  is_running;
  std::string           filename;
  pthread_cond_t        not_empty;
  pthread_mutex_t       mutex;
  pthread_t             thread;
  std::deque<str_ptr> buffer;
};
} // namespace srslte
#endif // SRSLTE_LOGGER_FILE_H
```
### logger_stdout.h    lib\include\srslte\common    1481    2/28/2019    6
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
/******************************************************************************
 * File:        logger_stdout.h
 * Description: Interface for logging output
 *****************************************************************************/
#ifndef SRSLTE_LOGGER_STDOUT_H
#define SRSLTE_LOGGER_STDOUT_H
#include <stdio.h>
#include <string>
#include "srslte/common/logger.h"
namespace srslte {
  class logger_stdout : public logger
  {
  public:
    void log(std::string *msg) {
      if (msg) {
        fprintf(stdout, "%s", msg->c_str());
        delete msg;
      }
    }
  };
} // namespace srslte
#endif // SRSLTE_LOGGER_STDOUT_H
```
### logger_test.cc    lib\test\common    2591    2/28/2019    70
```cpp
/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
#define NTHREADS 100
#define NMSGS    100
#include <stdlib.h>
#include <stdio.h>
#include "srslte/common/logger_file.h"
using namespace srslte;
typedef struct {
  logger_file *l;
  int thread_id;
}args_t;
void* thread_loop(void *a) {
  args_t *args = (args_t*)a;
  char buf[100];
  for(int i=0;i<NMSGS;i++)
  {
    sprintf(buf, "Thread %d: %d", args->thread_id, i);
    args->l->log(new std::string(buf));
  }
  return NULL;
}
void write(std::string filename) {
  logger_file l;
  l.init(filename);
  pthread_t threads[NTHREADS];
  args_t    args[NTHREADS];
  for(int i=0;i<NTHREADS;i++) {
    args[i].l = &l;
    args[i].thread_id = i;
    pthread_create(&threads[i], NULL, &thread_loop, &args[i]);
  }
  for(int i=0;i<NTHREADS;i++) {
    pthread_join(threads[i], NULL);
  }
}
bool read(std::string filename) {
  bool pass = true;
  bool written[NTHREADS][NMSGS];
  int thread = 0, msg = 0;
  int r;
  for(int i=0;i<NTHREADS;i++) {
    for(int j=0;j<NMSGS;j++) {
      written[i][j] = false;
    }
  }
  FILE *f = fopen(filename.c_str(), "r");
  if(f!=NULL) {
    while(fscanf(f, "Thread %d: %d\n", &thread, &msg)) {
      if (thread < NTHREADS && msg < NMSGS) {
        written[thread][msg] = true;
      } else {
        perror("Wrong thread and/or msg");
        fclose(f);
        return false;
      }
    }
    fclose(f);
  }
  for(int i=0;i<NTHREADS;i++) {
    for(int j=0;j<NMSGS;j++) {
      if(!written[i][j]) pass = false;
    }
  }
  return pass;
}
int main(int argc, char **argv) {
  bool result;
  std::string f("log.txt");
  write(f);
  result = read(f);
  if (remove(f.c_str())) {
    perror("Removing file");
  }
  if(result) {
    printf("Passed\n");
    exit(0);
  }else{
    printf("Failed\n;");
    exit(1);
  }
}
```

