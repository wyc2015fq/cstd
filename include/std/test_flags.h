
#include "flags_c.h"
#include "log_c.h"

DEFINE_string(confPath, "../conf/setup.ini", "program configure file.");
DEFINE_int32(port, 9090, "program listen port");
DEFINE_bool(daemon, true, "run daemon mode");

int test_flags()
{
  char* argv[] = {
    "test_flags",
    "-confPath",  "C:/conf/setup.ini",
    "-port = 888",
    "-daemon",  "false",
  };
  int argc = countof(argv);
  ParseCommandLineFlags(argc, argv, true);
  printf("confPath = %s\n", FLAGS_confPath);
  printf("port = %d\n", FLAGS_port);
  LOG(INFO) << "asdfasf" << 123 << 345.2345;
  LOG(INFO) << "asdfasf" << 123 << 345.2345;
  return 0;
}

