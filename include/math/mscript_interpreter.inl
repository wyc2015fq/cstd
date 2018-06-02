// Брвы
int mscript_comp(mscript_engine_t* me)
{
  int i = 0, tok;
  str_t s1[1] = {0};
  char optstack[256];
  char pretop = 0, top = 0;
  optstack[top++] = '#';
  for (i = 0; ;) {
    tok = mscript_next_token(me, s1);
  }
  return 0;
}

