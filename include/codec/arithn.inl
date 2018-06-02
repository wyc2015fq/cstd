#ifndef _ARITHN_INL_
#define _ARITHN_INL_
// arith-n - order-n arithmetic coding module
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitio.inl"
// low_count 和 high_count 唯一地定义了在 0 到 1 范围中符号的所在位置
// scale 指 0 到 1 范围内的总量程，即有多少字符要记数
typedef struct {
  unsigned short int low_count;
  unsigned short int high_count;
  unsigned short int scale;
} SYMBOL;
#define MAXIMUM_SCALE 16383 // maximum allowed frequency count 
#define ESCAPE 256 // the escape symbol
#define DONE ( -1 ) // the output stream empty symbol
#define FLUSH ( -2 ) // the symbol to flush the model
int max_order = 3;
int check_compression(FILE* input, BIT_FILE* output)
{
  static long local_input_marker = 0L;
  static long local_output_marker = 0L;
  long total_input_bytes;
  long total_output_bytes;
  int local_ratio;
  total_input_bytes = ftell(input) - local_input_marker;
  total_output_bytes = ftell(output->file);
  total_output_bytes -= local_output_marker;
  if (total_output_bytes == 0) {
    total_output_bytes = 1;
  }
  local_ratio = (int)((total_output_bytes * 100) / total_input_bytes);
  local_input_marker = ftell(input);
  local_output_marker = ftell(output->file);
  return (local_ratio > 90);
}
//----------------------------------------------------------
// the next few routines contain all of the code and data used to
// perfoem modeling for thes program.
// a context table contains a list of the counts for all symbols
// that have been seen in the defined context. for example, a
// context of "Zor" might have only had 2 different characters
// appear. 't' might have appeared 10 times, and 'l' might have
// appeared once. these two counts are stored in and array of STATS.
// as new characters are added to a particular contexts, the STATS
// array will grow. sometimes, the STATS array will shrink
// after flushing the model.
typedef struct {
  unsigned char symbol;
  unsigned char counts;
} STATS;
// each context has to have links to higher order contexts. These
// links are used to navigate through the context tables. For example,
// to find the context table for "ABC", I start at the roder 0 table.
// then find the pointer to the "A" context table by looking through
// the LINKS array. At that table, we find the "B" link and go to
// that table. The process continues until the destination table is
// found. The table pointed to by the LINKS array corresponds to the
// symbol found at the same offset in the STATS table. The reason that
// LINKS is in a separate structure instead of being combined with
// STATS is to save space. All of the leaf context nodes don't need
// next pointers, since they are in the highest order context. In the
// leaf nodes, the LINKS array is a NULL pointers.
typedef struct {
  struct context* next;
} LINKS;
// The CONTEXT structure holds all of the known information about
// a particular context. The links and stats pointers are discussed
// immediately above here. The max_index element gives the maximum
// index that can be applied to the stats or link array. When the
// table is first created, and stats is set to NULL, max_index si set
// to -1. As soon as single element is added to stats, max_index is
// incremented to 0.
//
// The lesser context pointer is a navigational aid. It points to
// the context that is one less than the current order. For example
// if the current is "ABC", the lesser_context pointer will
// point to "BC". The reason for maintaining this pointer is that
// this particular bit of table searching is done frequently, but
// the pointer only needs to be built once, when the context is
// created.
//
typedef struct context {
  int max_index;
  LINKS* links;
  STATS* stats;
  struct context* lesser_context;
} CONTEXT;
CONTEXT** contexts;
// current_order contains the current order of the model. it starts
// at max_order, and is decremented every time an ESCAPE is sent. it
// will only go down to -1 for normal symbols, but can go to -2 for
// EOF and FLUSH
int current_order;
// this table contains the cumulative totals for the current context.
// because this program is using exclusion, totals has to be calculated
// every time a context is used. the scoreboard array keeps track of
// symbols that have appeared in higher order models, so that they
// can be excluded from lower order context total calculations.
short int totals[258];
char scoreboard[256];
CONTEXT* allocate_next_order_table(CONTEXT* table, int symbol, CONTEXT* lesser_context)
{
  CONTEXT* new_table;
  int i;
  unsigned int new_size;
  for (i = 0; i <= table->max_index; i++) {
    if (table->stats[i].symbol == (unsigned char)symbol) {
      break;
    }
  }
  if (i > table->max_index) {
    table->max_index++;
    new_size = sizeof(LINKS);
    new_size *= table->max_index + 1;
    if (table->links == NULL) {
      table->links = (LINKS*)calloc(new_size, 1);
    }
    else {
      table->links = (LINKS*)realloc((char*)table->links, new_size);
    }
    new_size = sizeof(STATS);
    new_size *= table->max_index + 1;
    if (table->stats == NULL) {
      table->stats = (STATS*)calloc(new_size, 1);
    }
    else {
      table->stats = (STATS*)realloc((char*)table->stats, new_size);
    }
    if (table->links == NULL) {
      fatal_error("Failure #6: allocating new talbe");
    }
    if (table->stats == NULL) {
      fatal_error("Failure #7: allocating new table");
    }
    table->stats[i].symbol = (unsigned char)symbol;
    table->stats[i].counts = 0;
  }
  new_table = (CONTEXT*)calloc(sizeof(CONTEXT), 1);
  if (new_table == NULL) {
    fatal_error("Failure #8: allocating new table");
  }
  new_table->max_index = -1;
  table->links[i].next = new_table;
  new_table->lesser_context = lesser_context;
  return (new_table);
}
void initialize_model()
{
  int i;
  CONTEXT* null_table;
  CONTEXT* control_table;
  current_order = max_order;
  contexts = (CONTEXT**)calloc(sizeof(CONTEXT*), 10);
  if (contexts == NULL) {
    fatal_error("Failure #1: allocating context table!");
  }
  contexts += 2;
  null_table = (CONTEXT*)calloc(sizeof(CONTEXT), 1);
  if (null_table == NULL) {
    fatal_error("Failure #2: allocating null table!");
  }
  null_table->max_index = -1;
  contexts[-1] = null_table;
  for (i = 0; i <= max_order; i++) {
    contexts[i] = allocate_next_order_table(contexts[i - 1], 0, contexts[i - 1]);
  }
  free((char*)null_table->stats);
  null_table->stats = (STATS*)calloc(sizeof(STATS), 256);
  if (null_table->stats == NULL) {
    fatal_error("Failure #3: allocating null table!");
  }
  null_table->max_index = 255;
  for (i = 0; i < 256; i ++) {
    null_table->stats[i].symbol = (unsigned char)i;
    null_table->stats[i].counts = 1;
  }
  control_table = (CONTEXT*)calloc(sizeof(CONTEXT), 1);
  if (control_table == NULL) {
    fatal_error("Failure #4: allocating control table!");
  }
  control_table->stats = (STATS*)calloc(sizeof(STATS), 2);
  if (control_table->stats == NULL) {
    fatal_error("Failure #5: allocating control table!");
  }
  contexts[-2] = control_table;
  control_table->max_index = 1;
  control_table->stats[0].symbol = -FLUSH;
  control_table->stats[0].counts = 1;
  control_table->stats[1].symbol = -DONE;
  control_table->stats[1].counts = 1;
  for (i = 0; i < 256; i++) {
    scoreboard[i] = 0;
  }
}
// rexdaling the table needs to be done for one of three reasons.
// first, if the maximum count for the table has exceeded 16383, it
// means that arithmetic coding using 16 and 32 bit registers might
// no longer work. secondly, if an individual symbol count has
// reached 255, it will no longer fit in a byte. third, if the
// current model isn't compressing well, the compressor program may
// want to rescale all tables in order to give more weight to newer
// statistics.
// all this routine does is divide each count by 2. if any counts
// drop to 0, the counters can be removed from the stats table, but
// only if this is a leaf context. Otherwise, we might cut a link to
// a higher order table.
void rescale_table(CONTEXT* table)
{
  int i;
  if (table->max_index == -1) {
    return;
  }
  for (i = 0; i <= table->max_index; i++) {
    table->stats[i].counts /= 2;
  }
  if (table->stats[table->max_index].counts == 0 &&
      table->links == NULL) {
    while (table->stats[table->max_index].counts == 0 &&
        table->max_index >= 0) {
      table->max_index--;
    }
    if (table->max_index == -1) {
      free((char*) table->stats);
      table->stats = NULL;
    }
    else {
      table->stats = (STATS*)realloc((char*)table->stats,
          sizeof(STATS) * (table->max_index + 1));
      if (table->stats == NULL) {
        fatal_error("Error #11: reallocating stats space!");
      }
    }
  }
}
void update_table(CONTEXT* table, int symbol)
{
  int i;
  int index;
  unsigned char temp;
  CONTEXT* temp_ptr;
  unsigned int new_size;
  // first, find the symbol in the apropriate context table. The first
  // symbol in the table is the most active. so start there.
  index = 0;
  while (index <= table->max_index &&
      table->stats[index].symbol != (unsigned char)symbol) {
    index++;
  }
  if (index > table->max_index) {
    table->max_index++;
    new_size = sizeof(LINKS);
    new_size *= table->max_index + 1;
    if (current_order < max_order) {
      if (table->max_index == 0) {
        table->links = (LINKS*)calloc(new_size, 1);
      }
      else {
        table->links = (LINKS*)realloc((char*)table->links, new_size);
      }
      if (table->links == NULL) {
        fatal_error("Error #9: reallocating table space!");
      }
      table->links[index].next = NULL;
    }
    new_size = sizeof(STATS);
    new_size *= table->max_index + 1;
    if (table->max_index == 0) {
      table->stats = (STATS*)calloc(new_size, 1);
    }
    else {
      table->stats = (STATS*)realloc((char*)table->stats, new_size);
    }
    if (table->stats == NULL) {
      fatal_error("Error #10: reallocating table space!");
    }
    table->stats[index].symbol = (unsigned char)symbol;
    table->stats[index].counts = 0;
  }
  // now I move the symbol to the front of its list
  i = index;
  while (i > 0 && table->stats[index].counts == table->stats[i - 1].counts) {
    i--;
  }
  if (i != index) {
    temp = table->stats[index].symbol;
    table->stats[index].symbol = table->stats[i].symbol;
    table->stats[i].symbol = temp;
    if (table->links != NULL) {
      temp_ptr = table->links[index].next;
      table->links[index].next = table->links[i].next;
      table->links[i].next = temp_ptr;
    }
    index = i;
  }
  // the switch has been performed. now I can update the counts
  table->stats[index].counts++;
  if (table->stats[index].counts == 255) {
    rescale_table(table);
  }
}
// the routine is called to increment the counts for the current
// contexts. It is called after a character has been encoded or
// decoded. All it does is call update_table for each of the
// current contexts, which does the work of incrementing the count.
// This particular version of update_model() practices update exclusion.
// which means that if lower order models weren't used to encode
// or decode the character, they don't get their counts updated.
// this seems to improve compression performance quite a bit.
// to disable update exclusion, the loop would be changed to run
// from 0 to max_order, instead of current_order to max_order
void update_model(int symbol)
{
  int i;
  int local_order;
  if (current_order < 0) {
    local_order = 0;
  }
  else {
    local_order = current_order;
  }
  if (symbol >= 0) {
    while (local_order <= max_order) {
      if (symbol >= 0) {
        update_table(contexts[local_order], symbol);
      }
      local_order++;
    }
  }
  current_order = max_order;
  for (i = 0; i < 256; i++) {
    scoreboard[i] = 0;
  }
}
// this routine has the job of creating a cumulative totals table for
// a given context. the cumulative low and high for symbol c are going to
// be shored in totals[c + 2] and totals[c + 1]. Locations 0 and 1 are
// reserved for the special ESCAPE symbol.
void totalize_table(CONTEXT* table)
{
  int i;
  unsigned char max;
  for (;;) {
    max = 0;
    i = table->max_index + 2;
    totals[i] = 0;
    for (; i > 1; i--) {
      totals[i - 1] = totals[i];
      if (table->stats[i - 2].counts) {
        if ((current_order == -2) ||
            scoreboard[table->stats[i - 2].symbol] == 0) {
          totals[i - 1] += table->stats[i - 2].counts;
        }
      }
      if (table->stats[i - 2].counts > max) {
        max = table->stats[i - 2].counts;
      }
    }
    // here is where the escape calulation needs to take place.
    if (max == 0) {
      totals[0] = 1;
    }
    else {
      totals[0] = (short int)(256 - table->max_index);
      totals[0] *= table->max_index;
      totals[0] /= 256;
      totals[0] /= max;
      totals[0]++;
      totals[0] += totals[1];
    }
    if (totals[0] < MAXIMUM_SCALE) {
      break;
    }
    rescale_table(table);
  }
  for (i = 0; i < table->max_index; i++) {
    if (table->stats[i].counts != 0) {
      scoreboard[table->stats[i].symbol] = 1;
    }
  }
}
int convert_int_to_symbol(int c, SYMBOL* s)
{
  int i;
  CONTEXT* table;
  table = contexts[current_order];
  totalize_table(table);
  s->scale = totals[0];
  if (current_order == -2) {
    c = -c;
  }
  for (i = 0; i <= table->max_index; i++) {
    if (c == (int)table->stats[i].symbol) {
      if (table->stats[i].counts == 0) {
        break;
      }
      s->low_count = totals[i + 2];
      s->high_count = totals[i + 1];
      return (0);
    }
  }
  s->low_count = totals[1];
  s->high_count = totals[0];
  current_order--;
  return (1);
}
void get_symbol_scale(SYMBOL* s)
{
  CONTEXT* table;
  table = contexts[current_order];
  totalize_table(table);
  s->scale = totals[0];
}
int convert_symbol_to_int(int count, SYMBOL* s)
{
  int c;
  CONTEXT* table;
  table = contexts[current_order];
  for (c = 0; count < totals[c]; c++)
    ;
  s->high_count = totals[c - 1];
  s->low_count = totals[c];
  if (c == 1) {
    current_order--;
    return (ESCAPE);
  }
  if (current_order < -1) {
    return ((int) - table->stats[c - 2].symbol);
  }
  else {
    return (table->stats[c - 2].symbol);
  }
}
CONTEXT* shift_to_next_context(CONTEXT* table, int c, int order)
{
  int i;
  CONTEXT* new_lesser;
  // first, try to find the new context by backing up the lesser
  // context and searching its link table. if I find the link, we take
  // a quick and easy exit, returning the link. note that their is a
  // special Kludge for context order 0. we hnow for a fact that
  // that lesser context pointer at order 0 points to the null table.
  // order -1, and we know that the -1 table only has a single link
  // pointer. which points back to the order 0 table.
  table = table->lesser_context;
  if (order == 0) {
    return (table->links[0].next);
  }
  for (i = 0; i <= table->max_index; i++)
    if (table->stats[i].symbol == (unsigned char)c)
      if (table->links[i].next != NULL) {
        return (table->links[i].next);
      }
      else {
        break;
      }
  // if I get here, it means the new context did not exist. I have to
  // create the new context, add a link to it here, and add the backwards
  // link to *his* previous context. Creating the table and adding it to
  // this table is pretty easy. but adding the back pointer isn't. Since
  // creating the new back pointer isn't easy, I duck my responsibility
  // and recurse to myself in order to pick it up.
  new_lesser = shift_to_next_context(table, c, order - 1);
  // Now that I have the back pointer for this table, I can make a call
  // to a utility to allocate the new table
  table = allocate_next_order_table(table, c, new_lesser);
  return (table);
}
void add_character_to_model(int c)
{
  int i;
  if (max_order < 0 || c < 0) {
    return;
  }
  contexts[max_order] = shift_to_next_context(contexts[max_order], c, max_order);
  for (i = max_order - 1; i > 0; i--) {
    contexts[i] = contexts[i + 1]->lesser_context;
  }
}
void recursive_flush(CONTEXT* table)
{
  int i;
  if (table->links != NULL) {
    for (i = 0; i <= table->max_index; i++) {
      if (table->links[i].next != NULL) {
        recursive_flush(table->links[i].next);
      }
    }
  }
  rescale_table(table);
}
void flush_model()
{
  putc('F', stdout);
  recursive_flush(contexts[0]);
}
//---------------------------------------------------------------
// everything from here down define the arithmetic coder section
// of the program
static unsigned short int code; // the present input code value
static unsigned short int low; // start of the current code range
static unsigned short int high; // end of the current code range
long underflow_bits; // number of underflow bits pending
void initialize_arithmetic_encoder()
{
  low = 0;
  high = 0xffff;
  underflow_bits = 0;
}
void flush_arithmetic_encoder(BIT_FILE* stream)
{
  OutputBit(stream, low & 0x4000);
  underflow_bits++;
  while (underflow_bits-- > 0) {
    OutputBit(stream, ~low & 0x4000);
  }
  OutputBits(stream, 0L, 16);
}
void encode_symbol(BIT_FILE* stream, SYMBOL* s)
{
  long range;
  // these three lines rescale high and low for the new symbol.
  range = (long)(high - low) + 1;
  high = low + (unsigned short int)
      ((range * s->high_count) / s->scale - 1);
  low = low + (unsigned short int)
      ((range * s->low_count) / s->scale);
  // this loop turns out new bits until high and low are far enough
  // apart to have stabilized.
  for (;;) {
    // if this test passer, it means that the MSDigits match, and can
    // be sent to the output stream.
    if ((high & 0x8000) == (low & 0x8000)) {
      OutputBit(stream, high & 0x8000);
      while (underflow_bits > 0) {
        OutputBit(stream, ~high & 0x8000);
        underflow_bits--;
      }
    }
    // if this test passes, the numbers are in danger of underflow, because
    // the MSDigits don't match, and the 2nd digits are just one apart.
    else if ((low & 0x4000) && !(high & 0x4000)) {
      underflow_bits += 1;
      low &= 0x3fff;
      high |= 0x4000;
    }
    else {
      return;
    }
    low <<= 1;
    high <<= 1;
    high |= 1;
  }
}
short int get_current_count(SYMBOL* s)
{
  long range;
  short int count;
  range = (long)(high - low) + 1;
  count = (short int)
      ((((long)(code - low) + 1) * s->scale - 1) / range);
  return (count);
}
void initialize_arithmetic_decoder(BIT_FILE* stream)
{
  int i;
  code = 0;
  for (i = 0; i < 16; i++) {
    code <<= 1;
    code += InputBit(stream);
  }
  low = 0;
  high = 0xffff;
}
void remove_symbol_from_stream(BIT_FILE* stream, SYMBOL* s)
{
  long range;
  // first, the range is expanded to account for the symbol removal
  range = (long)(high - low) + 1;
  high = low + (unsigned short int)
      ((range * s->high_count) / s->scale - 1);
  low = low + (unsigned short int)
      ((range * s->low_count) / s->scale);
  // next, any possible bits are shipped out
  for (;;) {
    // if the MSDigits match, the ibts will be shifted out.
    if ((high & 0x8000) == (low & 0x8000)) {
    }
    // else, if underflow is threatening, shift out the 2nd MSDigit.
    else if ((low & 0x4000) == 0x4000 && (high & 0x4000) == 0) {
      code ^= 0x4000;
      low &= 0x3fff;
      high |= 0x4000;
    }
    // otherwise, nothing can be shifted out, so I return.
    else {
      return;
    }
    low <<= 1;
    high <<= 1;
    high |= 1;
    code <<= 1;
    code += InputBit(stream);
  }
}
void CompressFile(FILE* input, BIT_FILE* output)
{
  SYMBOL s;
  int c;
  int escaped;
  int flush = 0;
  long int text_count = 0;
  initialize_model();
  initialize_arithmetic_encoder();
  for (;;) {
    if ((++ text_count & 0xff) == 0) {
      flush = check_compression(input, output);
    }
    if (!flush) {
      c = getc(input);
    }
    else {
      c = FLUSH;
    }
    if (c == EOF) {
      c = DONE;
    }
    do {
      escaped = convert_int_to_symbol(c, &s);
      encode_symbol(output, &s);
    }
    while (escaped);
    if (c == DONE) {
      break;
    }
    if (c == FLUSH) {
      flush_model();
      flush = 0;
    }
    update_model(c);
    add_character_to_model(c);
  }
  flush_arithmetic_encoder(output);
}
void ExpandFile(BIT_FILE* input, FILE* output)
{
  SYMBOL s;
  int c;
  int count;
  initialize_model();
  initialize_arithmetic_decoder(input);
  for (;;) {
    do {
      get_symbol_scale(&s);
      count = get_current_count(&s);
      c = convert_symbol_to_int(count, &s);
      remove_symbol_from_stream(input, &s);
    }
    while (c == ESCAPE);
    if (c == DONE) {
      break;
    }
    if (c != FLUSH) {
      putc((char) c, output);
    }
    else {
      flush_model();
    }
    update_model(c);
    add_character_to_model(c);
  }
}
#endif // _ARITHN_INL_

