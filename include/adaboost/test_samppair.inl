

int test_samppair()
{
  SAMPPAIR_T sp[1] = {0};
  samppair_init(sp, 300, 4);
  samppair_free(sp);
  return 0;
}
