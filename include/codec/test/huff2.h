
#include <string.h>

#define NUM_CHARS 256   // number of characters in the character set
#define MAX_SIZE 100000   // maximal buffer size for input from file

#ifndef TRUE
#define TRUE 1
#endif // TRUE
#ifndef FALSE
#define FALSE 0
#endif // FALSE

//Huffman 编码
typedef struct HuffCode {
  int used;
  unsigned char bits[2];
  unsigned int size;
} HuffCode;

//Huffman Tree 节点
typedef struct HuffNode {
  char c;             // for leaves, the corresponding character from the character set
  unsigned int freq;  // frequency of the node (for non-leaves sum of frequencies of child nodes)
  struct HuffNode *left, *right;    // 孩子结点
} HuffNode;


// This class is used in the construction of the Huffman tree.
// 优先级队列
  HuffNode g_huffNode[NUM_CHARS*2];
  HuffNode* g_HuffNodes[NUM_CHARS*2];
  unsigned int g_size;
  int g_Nodeused;

  HuffNode* new_HuffNode() {
    return g_huffNode + g_Nodeused++;
  }
  void init() {
    g_size=0;
    g_Nodeused=0;
  }

  void heapify(int i)
  {
    int l,r,smallest;
    HuffNode* tmp;
    
    l=2*i; /*left child*/
    r=2*i+1; /*right child*/
    
    if ((l < g_size)&&(g_HuffNodes[l]->freq < g_HuffNodes[i]->freq))
      smallest=l;
    else
      smallest=i;
    if ((r < g_size)&&(g_HuffNodes[r]->freq < g_HuffNodes[smallest]->freq))
      smallest=r;
    
    if (smallest!=i) 
    {
      /*exchange to maintain heap property*/
      tmp=g_HuffNodes[smallest];
      g_HuffNodes[smallest]=g_HuffNodes[i];
      g_HuffNodes[i]=tmp;
      heapify(smallest);
    }
  }

  void addItem(HuffNode* node) 
  {
    unsigned int i,parent;  
    g_size = g_size+1;
    i = g_size-1;
    parent = i/2;

    /*find the correct place to insert*/
    while ( (i > 0) && (g_HuffNodes[parent]->freq > node->freq) )
    {
      g_HuffNodes[i] = g_HuffNodes[parent];
      i = parent;
      parent = i/2;
    }
    g_HuffNodes[i]=node;
  }
  
  int isEmpty(void) {
    return g_size==0;
  }
  
  int isFull(void) {
    return g_size >= NUM_CHARS;
  }

  HuffNode* extractMin(void) 
  {
    HuffNode* max;
    if (isEmpty())
      return 0;
    max=g_HuffNodes[0];
    g_HuffNodes[0]=g_HuffNodes[g_size-1];
    g_size=g_size-1;
    heapify(0);
    return max;
  }


// set the bit at position pos in the array bits to the value state
void set_bit(unsigned char* bits, unsigned int pos, unsigned int state)
{
  unsigned int i;
  unsigned char mask = 0x80;  // = 128 dec = 10000000 bin
  for (i = 0; i < (pos % 8); i++)
    mask = mask >> 1;  // shift bitmask to right
  
  if (state)
    bits[pos/8] = bits[pos/8] | mask;
  else
    bits[pos/8] = bits[pos/8] & (~mask);
  
  return;
}

// get the state of the bit at position pos in the array bits
unsigned int get_bit(unsigned char* bits, unsigned int pos) {
  unsigned int i;
  unsigned char mask = 0x80;  // = 128 dec = 10000000 bin
  for (i = 0; i < (pos % 8); i++)
    mask = mask >> 1;  // shift bitmask to right
  
  return (((mask & bits[(int)(pos/8)]) == mask) ? 1 : 0);
}


// create the Huffman tree from the array of frequencies
// returns a pointer to the root node of the Huffman tree
// 根据字符频率数组，创建一个huffman树。返回根节点。
HuffNode* build_Huffman_tree(unsigned int freqs[NUM_CHARS])
{
  unsigned int i;
  // create priority queue
  //HuffNodePriorityQueue priority_queue;
  init();

  for (i = 0; i < NUM_CHARS; i++)
  {
    if (freqs[i] > 0)
    {
      HuffNode* node = new_HuffNode();
      node->c = i;
      node->freq = freqs[i];
      node->left = NULL;
      node->right = NULL;
      addItem(node);
    }
  }

  //printf("number of characters: %d\n", size);

  // create the Huffman tree
  while (g_size > 1)
  {
    HuffNode* left = extractMin();
    HuffNode* right = extractMin();

    HuffNode* root = new_HuffNode();
    root->freq = left->freq + right->freq;
    root->left = left;
    root->right = right;
    addItem(root);
  }

  // return pointer to the root of the Huffman tree
  return extractMin();
}


// recursively free memory of the Huffman tree
// 递归函数：释放huffman树（所有节点）占用的内存

void delete_Huffman_tree(HuffNode* root)
{
  HuffNode* node = root;

  // base case at leaf
  if (node->left == NULL && node->right == NULL)
  {
    //delete node;
    return;
  }

  if (node->left)
    delete_Huffman_tree(node->left);
  if (node->right)
    delete_Huffman_tree(node->right);

  //delete node;
  return;
}



//递归函数：深度优先遍历huffman树，并把每个叶子节点的编码设置到响应的 HuffCode 表中相应对象。
//code: 追踪记录当前的HuffCode。（到叶子节点的路径）
//pos: code的当前长度

void create_table_rec(HuffCode table[NUM_CHARS], HuffNode* node, unsigned char* code, unsigned int pos) {
  if(node->left) {
    set_bit(code, pos, 0);
    create_table_rec(table, node->left, code, pos+1);
  }
  
  if(node->right) {
    set_bit(code, pos, 1);
    create_table_rec(table, node->right, code, pos+1);
  }
  
  //到达叶子节点？则存储当前节点的HuffCode
  if(node->left == 0 && node->right == 0) {
    table[node->c].used = TRUE;
    table[node->c].bits[0] = code[0];
    table[node->c].bits[1] = code[1];
    table[node->c].size = pos;
  }
}

HuffNode* ntable[NUM_CHARS];
unsigned char npos[NUM_CHARS];
unsigned char ncode0[NUM_CHARS];
unsigned char ncode1[NUM_CHARS];
int create_table_rec1(HuffCode table[NUM_CHARS], HuffNode* node, unsigned char* code, unsigned int pos) {
  int nlen=0;
  ncode0[nlen] = code[0], ncode1[nlen] = code[1], npos[nlen]=pos, ntable[nlen++]=node;
  while(nlen>0 && nlen<NUM_CHARS) {
    code[0] = ncode0[--nlen], code[1] = ncode1[nlen], pos = npos[nlen], node = ntable[nlen];
    if(node->left) {
      set_bit(code, pos, 0);
      //create_table_rec(table, node->left, code, pos+1);
      ncode0[nlen] = code[0], ncode1[nlen] = code[1], npos[nlen]=pos+1, ntable[nlen++]=node->left;
    }
    
    if(node->right) {
      set_bit(code, pos, 1);
      //create_table_rec(table, node->right, code, pos+1);
      ncode0[nlen] = code[0], ncode1[nlen] = code[1], npos[nlen]=pos+1, ntable[nlen++]=node->right;
    }
    
    //到达叶子节点？则存储当前节点的HuffCode
    if(node->left == 0 && node->right == 0) {
      table[node->c].used = TRUE;
      table[node->c].bits[0] = code[0];
      table[node->c].bits[1] = code[1];
      table[node->c].size = pos;
    }
  }
  return 0;
}


// create table of Huffman codes
// calls create_table_rec to do the actual work
int create_table(HuffCode table[NUM_CHARS], HuffNode* tree) {
  unsigned int i;
  unsigned char code[2];
  code[0]=0, code[1]=0;
  // initialize table data so that there are no undefined values
  for (i = 0; i < NUM_CHARS; i++) {
    table[i].used = FALSE;
    table[i].size = 0;
    table[i].bits[0] = 0;
    table[i].bits[1] = 0;
  }

  //code中保存节点的huffman编码。
  create_table_rec(table, tree, code, 0);
  //for (i = 0; i < NUM_CHARS; i++) {
  //  printf("%d, %d, %d, %d,\n", table[i].used, table[i].size, table[i].bits[0], table[i].bits[1]);
  //}
  return 0;
}

// 创建文件头
void create_header(unsigned int freqs[NUM_CHARS], HuffCode table[NUM_CHARS], unsigned char* header, unsigned int size)
{
  unsigned int i;
  // store the size of the string at the beginning of the header
  memcpy(header, &size, sizeof(int));

  // store the scaled frequencies for all symbols in the character set
  for (i = 0; i < NUM_CHARS; i++)
    header[sizeof(int) + i] = (unsigned char)freqs[i];

  return;
}

//给定一个字符串，把字符的出现频率保存到freqs数组中
//Hint: Be carefull that you don’t scale any frequencies to zero for symbols that do appear in the string!
void create_freq_array(unsigned int freqs[NUM_CHARS], unsigned char* string, unsigned int size)
{
  int i, maxfreq = 0;
  
  //初始化成0
  memset(freqs, 0, sizeof(unsigned int) * NUM_CHARS);
  
  for(i=0; i<size; i++)
  {
    freqs[string[i]]++;
    
    if(freqs[string[i]] > maxfreq)
      maxfreq = freqs[string[i]];
  }
  
  //把字符频率压缩到一个字节。 scaled freqs to (0~255)
  if(maxfreq > 0xff) {
    for(i=0; i<NUM_CHARS; i++) {
      if(freqs[i]) {
        freqs[i] = (int)(freqs[i] * 255.0 / maxfreq + 0.5);
        
        //要确保不会被缩小成0！
        if(freqs[i] == 0) {
          freqs[i] = 1;
        }
      }
    }
  }
}

//将一个指定长度的字符串进行Huffman编码
//compressed: 编码结果
//string: 输入的字符串
//size: 字符串长度
//返回 compressed 的字节长度。
unsigned int compress(HuffCode table[NUM_CHARS], unsigned char* compressed, unsigned char* string, unsigned int size)
{
  unsigned int i, j, pos = 0;
  unsigned char c;
  
  for(i = 0; i < size; i++) {
    c=string[i];
    for(j = 0; j < table[c].size; j++) {
      set_bit(compressed, pos+j, get_bit(table[c].bits, j));
    }
    pos += table[c].size;
  }
  
  //bits = (pos+1)
  return (pos/8 + 1);
}

//huffman解码：
//root: 已经重建好的Huffman树的根节点
//comprerssed: huffman编码后的bits
//string: 解码后的字符串
//size: 原始字符串（编码前的明文字符串）长度，它是从header中读取的
void uncompress(HuffNode* root, unsigned char* compressed, unsigned char* string, unsigned int size)
{
  unsigned int length = 0;   //已经解码的字符数
  unsigned int pos = 0;     //记录在bits中的位置
  unsigned int bit;
  HuffNode *node = NULL; //当前节点
  
  while(length < size)
  {
    //置到根节点
    node = root; 
    
    //寻找叶子节点
    while(node->left != 0 || node->right != 0)
    {
      bit = get_bit(compressed, pos++);
    
      if(bit) node = node->right;  //goto right child
      else node = node->left;     //goto left child
    }
    
    string[length++] = node->c;    
  }
}

#define HSIZE (sizeof(int) + NUM_CHARS)

HuffCode table[NUM_CHARS];
unsigned char header[HSIZE];
unsigned int freqs[NUM_CHARS];
// compress the size-many symbols in string into *compressed
// returns the size of the compressed data (in bytes)
unsigned int HUFF_Encode(unsigned char* string, unsigned int size, unsigned char* compressed)
{
  HuffNode* huff_tree;
  unsigned char* comp = compressed+HSIZE;
  unsigned int codesize;
  // create array of frequencies for all ascii characters
  create_freq_array(freqs, string, size);

  //打印字符频率
  //print_freqs(freqs);

  // create Huffman tree
  huff_tree = build_Huffman_tree(freqs);

  // create symbol table
  create_table(table, huff_tree);

  //print_table(table);

  //create header
  //unsigned int HSIZE = sizeof(int) + NUM_CHARS;  // size of the header
  create_header(freqs, table, header, size);
  
  // 将Header 和 编码结果 合并到一起(compressed_tmp)。
  memcpy(compressed, header, HSIZE);

  // create compressed text
  codesize = compress(table, comp, string, size);

  //printf("compressed string: (size: %d bit)\n", 8*codesize);
  //for (unsigned int i = 0; i < 8*codesize; i++)
  //  printf("%d", get_bit(comp, i));
  //printf("\n");

  // free Huffman tree
  delete_Huffman_tree(huff_tree);

  return HSIZE + codesize;
}

// uncompress the data in compressed into *string
// returns the size of the uncompressed text (in bytes)
unsigned int HUFF_Decode(unsigned char* compressed, unsigned int size0, unsigned char* str)
{
  unsigned int size = 0;  // size of the string
  unsigned int freqs[NUM_CHARS];  // frequencies of the ascii characters
  //unsigned int HSIZE = sizeof(int) + NUM_CHARS;  // size of the header
  unsigned int i;
  HuffNode* huff_tree;

  // get number of symbols in compressed
  // 读取原始字符串的长度（存储在前4个字节中）
  memcpy(&size, compressed, sizeof(int));

  //printf("size of compressed string: %d\n", size);

  // restore frequency table
  for (i = 0; i < NUM_CHARS; i++)
    freqs[i] = compressed[sizeof(int) + i];


  // create Huffman tree
  huff_tree = build_Huffman_tree(freqs);

  /*
  HuffCode table[NUM_CHARS];
  create_table(table, huff_tree);
  print_table(table);
  */

  // uncompress the data
  uncompress(huff_tree, &compressed[HSIZE], str, size);

  // free Huffman tree
  delete_Huffman_tree(huff_tree);

  //printf("uncompressed string: (size: %d bit)\n", 8*sizeof(char)*size);
  //for(i = 0; i < size; i++)
  //  printf("%c", (*string)[i]);
  //printf("\n");

  return size;
}

