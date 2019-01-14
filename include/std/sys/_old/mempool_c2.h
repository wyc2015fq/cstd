
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MP_OK			0
#define	MP_MALLOC_FIAL	-1
#define MP_NOT_INIT		-2

struct memblock_t {
  unsigned int column;
  unsigned char * data;
  memblock_t * next;
};

typedef struct {
  unsigned int total_count;
  unsigned int used_count;
  unsigned int block_len;
  memblock_t  * free_header;
  memblock_t  * used_header;
} mempool_t;


int buffer_pool_init(unsigned int colume_no, unsigned int block_len[], unsigned int block_count[]);
memblock_t * buffer_malloc(unsigned int size);
int buffer_free(memblock_t * buffer);
int buffer_pool_destory(void);
int buffer_runtime_print(void);

mempool_t* b_p = NULL;
int buffer_pool_no = 0;

int buffer_pool_init(unsigned int colume_no, unsigned int block_len[] , unsigned int block_count[])
{
  b_p = (mempool_t *)malloc(sizeof(mempool_t) * colume_no);
  if (b_p == NULL)
    return MP_MALLOC_FIAL;
  memset(b_p , 0 , sizeof(mempool_t) * colume_no);
  buffer_pool_no = colume_no;
  
  
  memblock_t * curr_node = NULL;
  memblock_t * new_node = NULL;
  for (int i = 0; i < colume_no; i++)
  {
    b_p[i].block_len = block_len[i];
    b_p[i].total_count = block_count[i];
    for (int j = 0; j < block_count[i]; j++)
    {
      new_node =(memblock_t *)malloc(sizeof(memblock_t));
      new_node->column = i;
      new_node->data =(unsigned char *) malloc(block_len[i]);
      memset(new_node->data , 0 ,block_len[i]);
      if (new_node == NULL || new_node->data == NULL)
        return MP_MALLOC_FIAL;
      new_node->next = NULL;
      if (j == 0)
      {
        b_p[i].free_header = new_node;
        curr_node = b_p[i].free_header;
      }
      else
      {
        curr_node->next = new_node;
        curr_node = curr_node->next;
      }
    }
  }
  return MP_OK;
}

memblock_t * buffer_malloc(unsigned int size)
{
  memblock_t * node = NULL;
  if (size > b_p[buffer_pool_no - 1].block_len)
  {
    printf("malloc size[%d] so big ,need new from stack!!\n" , size);
a:		node = (memblock_t *)malloc(sizeof(memblock_t));
      node->column = 9999;
      node->data = (unsigned char *)malloc(size);
      memset(node->data, 0, size);
      if (node == NULL || node->data == NULL)
        return NULL;
      node->next = NULL;
      return node;
  }
  for (int i = 0 ; i < buffer_pool_no ; i++)
  {
    if (size > b_p[i].block_len)
      continue;
    if (b_p[i].total_count - b_p[i].used_count == 0)
    {
      printf("warning!!!!  size[%d]pool use up!!!! \n" , b_p[i].block_len);
      continue;
    }
    node = b_p[i].free_header;
    b_p[i].free_header = b_p[i].free_header->next;
    b_p[i].used_count++;
    node->next = b_p[i].used_header;
    b_p[i].used_header = node;
    return node;
  }
  printf("warning!!!!  all of pool used up!!!! \n");
  goto a;
}

int buffer_free(memblock_t * buffer)
{
  memblock_t * node_cur = b_p[buffer->column].used_header;
  memblock_t * node_pre = NULL;
  if (buffer->column == 9999)
  {
    free(buffer->data);
    free(buffer);
    buffer = NULL;
    return MP_OK;
  }
  while(node_cur != NULL)
  {
    if (node_cur != buffer)
    {
      node_pre = node_cur;
      node_cur = node_cur->next;
      continue;
    }
    if (node_pre == NULL)
    {
      b_p[buffer->column].used_header = b_p[buffer->column].used_header->next;
    }
    else
    {
      node_pre->next = node_cur->next;
    }
    b_p[buffer->column].used_count--;
    node_cur->next = b_p[buffer->column].free_header;
    b_p[buffer->column].free_header = node_cur;
    break;
  }
  return MP_OK;
}

int buffer_pool_destory(void)
{
  memblock_t * node_cur = NULL;
  memblock_t * node_del = NULL;
  if (b_p == NULL)
    return MP_NOT_INIT;
  for (int i = 0; i < buffer_pool_no; i++)
  {
    node_cur = b_p[i].used_header;
    while (node_cur != NULL)
    {
      node_del = node_cur;
      node_cur = node_cur->next;
      free(node_del->data);
      free(node_del);
    }
    node_cur = b_p[i].free_header;
    while (node_cur != NULL)
    {
      node_del = node_cur;
      node_cur = node_cur->next;
      free(node_del->data);
      free(node_del);
    }
  }
  free(b_p);
  b_p = NULL;
  buffer_pool_no = 0;
  return MP_OK;
}

int buffer_runtime_print(void)
{
  if (b_p == NULL)
  {
    printf("buffer pool not init yet!!!\n");
    return MP_NOT_INIT;
  }
  printf("\n*********************** memory pool runtime report start************************\n");
  for (int i = 0; i < buffer_pool_no; i++)
  {
    printf("pool no[%d] blocksize[%d] blockTotalCount[%d] usedBlock[%d] used percentage[%d%%]\n" \
      , i , b_p[i].block_len , b_p[i].total_count , b_p[i].used_count , b_p[i].used_count*100/ b_p[i].total_count);
  }
  printf("*********************** memory pool runtime report end**************************\n");
  return MP_OK;
}


int test_mempool() {
  
  //printf("press any key to start init and malloc memory pool\n");
  
  unsigned int a[10] = { 8 ,16 ,32 ,64 ,128 , 256 , 512 , 1024 ,2028 , 4096};
  unsigned int b[10] = { 100 ,500 ,1000 ,1500 ,2000 , 3000 , 4000 , 5000 ,4000 , 2000};
  buffer_pool_init(10, a , b);
  int i = 455;
  memblock_t * node = NULL;
  
  memblock_t * node1 = buffer_malloc(6);
  memblock_t * node2 = buffer_malloc(172);
  memblock_t * node3 = buffer_malloc(186);
  memblock_t * node4 = buffer_malloc(1300);
  memblock_t * node5 = buffer_malloc(2100);
  buffer_malloc(40);
  buffer_malloc(60);
  buffer_malloc(80);
  buffer_malloc(100);
  buffer_malloc(120);
  buffer_malloc(130);
  buffer_malloc(150);
  buffer_malloc(180);
  buffer_malloc(700);
  buffer_malloc(900);
  buffer_runtime_print();
  printf("press any key to free memory pool\n");
  //getchar();
  buffer_free(node5);
  buffer_free(node4);
  buffer_free(node3);
  buffer_runtime_print();
  buffer_free(node2);
  buffer_free(node1);
  buffer_runtime_print();
  printf("press any key to destory memory pool\n");
  //getchar();
  buffer_pool_destory();
  printf("press any key to quit\n");
  return 0;
}
