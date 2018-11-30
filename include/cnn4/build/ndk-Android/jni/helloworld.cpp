
#include <stdio.h>
#include "test_neon.hpp"
#include "test_conv.hpp"
#include "cnn4/myblas/test_gemm.h"


int main() {
  test_gemm();
	printf("%s\n", "hello");
	return 0;
}

