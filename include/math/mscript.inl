typedef struct mscript_engine_t {
  char* cur;
  char* str;
  char* send;
  int line, col;
} mscript_engine_t;
#include "mscript_token.inl"
#include "mscript_interpreter.inl"

