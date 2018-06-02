#include "arithn.inl"
#ifndef SEEK_END
#define SEEK_END 2
#endif
long file_size(const char* name)
{
  long eof_ftell;
  FILE* file;
  file = fopen(name, "rb");
  if (file == NULL) {
    return 0L;
  }
  fseek(file, 0L, SEEK_END);
  eof_ftell = ftell(file);
  fclose(file);
  return eof_ftell;
}
void print_ratios(const char* input, const char* output)
{
  long input_size;
  long output_size;
  int ratio;
  input_size = file_size(input);
  if (input_size == 0) {
    input_size = 1;
  }
  output_size = file_size(output);
  ratio = 100 - (int)(output_size * 100L / input_size);
  printf("\nInput bytes: %ld\n", input_size);
  printf("Output bytes: %ld\n", output_size);
  printf("Compression ratio: %d%%\n", ratio);
}
#include <direct.h>
int test_arithn()
{
  char* CompressionName = "Adaptive order n moder with arithmetic coding";
  char* Usage = "in-file out-file [-o order]\n\n";
  {
    BIT_FILE* output;
    FILE* input;
    const char* infile = "W0104.rtf";
    const char* oufile = "W0104.rtf.ari";
    _chdir("E:/pub/bin/codec");
    setbuf(stdout, NULL);
    input = fopen(infile, "rb");
    if (input == NULL) {
      fatal_error("Error opening %s for input\n", infile);
    }
    output = OpenOutputBitFile(oufile);
    if (output == NULL) {
      fatal_error("Error opening %s for output\n", oufile);
    }
    printf("\nCompressing %s to %s\n", infile, oufile);
    printf("Using %s\n", CompressionName);
    CompressFile(input, output);
    CloseOutputBitFile(output);
    fclose(input);
    print_ratios(infile, oufile);
  }
  {
    FILE* output;
    BIT_FILE* input;
    const char* infile = "W0104.rtf.ari";
    const char* oufile = "W01042.rtf";
    setbuf(stdout, NULL);
    _chdir("E:/pub/bin/codec");
    input = OpenInputBitFile(infile);
    if (input == NULL) {
      fatal_error("Error opening %s for input\n", infile);
    }
    output = fopen(oufile, "wb");
    if (output == NULL) {
      fatal_error("Error opening %s for output\n", oufile);
    }
    printf("\nExpanding %s to %s\n", infile, oufile);
    printf("Using %s\n", CompressionName);
    ExpandFile(input, output);
    CloseInputBitFile(input);
    fclose(output);
    putc('\n', stdout);
  }
  return 0;
}

