int test_str2f()
{
  float aaaa = 0;
  aaaa = str2f("12.34");
  aaaa = str2f("a23");
  aaaa = str2f("1234");
  aaaa = str2f("12.34");
  aaaa = str2f("12.34.56");
  aaaa = str2f(".34");
  aaaa = str2f("34a");
  aaaa = str2f("34a.456");
  aaaa = str2f("-34");
  aaaa = str2f("-56.34");
  aaaa = str2f("-3.45.67");
  aaaa = str2f("-.45.6a");
  aaaa = str2f("-.");
  aaaa = str2f("-0");
  return 0;
}

