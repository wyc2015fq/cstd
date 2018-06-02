
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POOL_SIZE 1024 * 1024
#define BLOCK_SIZE 64

#define allocator_realloc(x, p, size)  (x)->_realloc((x), p, size)
#define allocator_alloc(x, size)  (x)->_realloc((x), NULL, size)
#define allocator_free(x, p)  (x)->_free((x), p)

struct allocator {
  void* (*_realloc)(struct allocator* s, void* p, size_t size);
  void (*_free)(struct allocator* s, void* p);
  void (*freeAll)(struct allocator* s);
  void* x;
};

typedef struct memory_map_talbe
{
    char *p_block;
    int index;
    int used;
} Memory_Map_Table;

typedef struct memory_alloc_table
{
    char *p_start;
    int used;
    int block_start_index;
    int block_cnt;
}Memory_Alloc_Table;

typedef struct memory_pool
{
    char *memory_start;//内存池起始地址, free整个内存池时使用
    Memory_Alloc_Table *alloc_table;
    Memory_Map_Table *map_table;
    int total_size;
    int internal_total_size;
    int increment;
    int used_size;
    int block_size;
    int block_cnt;
    int alloc_cnt;
} Memory_Pool;




//获取内存映射表的位置
Memory_Map_Table *map_table_pos(Memory_Pool *pool)
{
    Memory_Map_Table *p = (Memory_Map_Table *)(pool->memory_start + sizeof(Memory_Pool));
    return p;
}

//获取内存分配表的位置
Memory_Alloc_Table *alloc_talbe_pos(Memory_Pool *pool)
{
    Memory_Alloc_Table *p = (Memory_Alloc_Table *)(pool->memory_start + sizeof(Memory_Pool) +
            sizeof(Memory_Map_Table) * (pool->block_cnt));
    return p;
}

//获得memory在位置
char *memory_pos(struct allocator* s)
{
    char *p = (char *)(pool->memory_start + sizeof(Memory_Pool) +
            (sizeof(Memory_Map_Table) + sizeof(Memory_Alloc_Table))* pool->block_cnt);
    return p;
}

void *Memory_malloc(struct allocator* s, int size)
{
    char *p_start = NULL;
    int need_block_cnt = 0;
    Memory_Map_Table *map_table = NULL;
    Memory_Map_Table *p_map_table = NULL;
    Memory_Alloc_Table *alloc_table = NULL;
    Memory_Alloc_Table *p_alloc_table = NULL;
    int block_cnt = 0;
    int start_index = -1;
    int i = 0;

    if (size <= 0) {
        printf("Invalid size(%d)\n", size);
        return p_start;
    }

    if (size > pool->total_size) {
        printf("%d is more than total size\n", size);
        return p_start;
    }

    if (size > pool->total_size - pool->used_size) {
        printf("Free memory(%d) is less than allocate(%d)\n",
        pool->total_size - pool->used_size, size);
        return NULL;
    }

    need_block_cnt = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    map_table = map_table_pos(pool);

    start_index = -1;
    for (i = 0; i < pool->block_cnt; i++) {
        p_map_table = (Memory_Map_Table *)((char *)map_table + i * sizeof(Memory_Map_Table));
        if (p_map_table->used) {
            //printf("before alloc: map index: %d is used\n", i);
            block_cnt = 0;
            start_index = -1;
            continue;
        }

        if (start_index == -1) {
            start_index = i;
            //printf("start_index: %d\n", start_index);
        }
        block_cnt++;

        if (block_cnt == need_block_cnt) {
            break;
        }
    }

    if (start_index == -1) {
        printf("No available memory to used\n");
        return NULL;
    }

    alloc_table = alloc_talbe_pos(pool);

    for (i = 0; i < pool->block_cnt; i++) {
        p_alloc_table = (Memory_Alloc_Table *)((char *)alloc_table + i * sizeof(Memory_Alloc_Table));
        if (p_alloc_table->used == 0) {
            break;
        }
        p_alloc_table = NULL;
    }

    if (p_alloc_table == NULL) {
        return NULL;
    }
    p_map_table = (Memory_Map_Table *)((char *)map_table + sizeof(Memory_Map_Table) * start_index);
    p_alloc_table->p_start = p_map_table->p_block;
    p_alloc_table->block_start_index = p_map_table->index;
    p_alloc_table->block_cnt = block_cnt;
    p_alloc_table->used = 1;

    //printf("block cnt is %d\n", block_cnt);
    for (i = start_index; i < start_index + block_cnt; i++) {
        p_map_table = (Memory_Map_Table *)((char *)map_table + i * sizeof(Memory_Map_Table));
        //printf("map index: %d is used\n", i);
        p_map_table->used = 1;
    }

    printf("Alloc size: %d, Block: (start: %d, end: %d, cnt: %d)\n", size,
            start_index, start_index + block_cnt - 1, block_cnt);
    pool->alloc_cnt++;
    pool->used_size += size;
    return p_alloc_table->p_start;
}

void memory_free(struct allocator* s, void *memory)
{
    Memory_Map_Table *map_table = NULL;
    Memory_Map_Table *p_map_table = NULL;
    Memory_Alloc_Table *alloc_table = NULL;
    Memory_Alloc_Table *p_alloc_table = NULL;
    int i = 0;
    int block_start_index = 0;
    int block_cnt = 0;

    if (memory == NULL) {
        printf("memory_free(): memory is NULL\n");
        return;
    }

    if (pool == NULL) {
        printf("Pool is NULL\n");
        return;
    }

    alloc_table = alloc_talbe_pos(pool);

    for (i = 0; i < pool->alloc_cnt; i++) {
        p_alloc_table = (Memory_Alloc_Table *)((char *)(alloc_table) + i * sizeof(Memory_Alloc_Table));
        if (p_alloc_table->p_start == memory) {
            block_start_index = p_alloc_table->block_start_index;
            block_cnt = p_alloc_table->block_cnt;
        }
    }

    if (block_cnt == 0) {
        return;
    }

    map_table = map_table_pos(pool);

    printf("Block: free: start: %d, end: %d, cnt: %d\n", block_start_index,
            block_start_index + block_cnt -1, block_cnt);
    for (i = block_start_index; i < block_start_index + block_cnt; i++) {
        p_map_table = (Memory_Map_Table *)((char *)map_table + i * sizeof(Memory_Map_Table));
        p_map_table->used = 0;
    }

    p_alloc_table->used = 0;
    pool->used_size -= block_cnt * BLOCK_SIZE;
}

void memory_pool_freeAll(struct allocator* s)
{
    if (pool == NULL) {
        printf("memory_pool_destroy: pool is NULL\n");
        return;
    }

    free(pool);
    pool = NULL;
}

struct allocator* memory_pool_init(struct allocator* s, int size, int increment)
{
    char *p = NULL;
    char *p_memory = NULL;
    Memory_Pool *pool = NULL;
    Memory_Alloc_Table *alloc_table = NULL;
    Memory_Alloc_Table *p_alloc_table = NULL;
    Memory_Map_Table *map_table = NULL;
    Memory_Map_Table *p_map_table = NULL;
    int block_cnt = 0;
    int all_size = 0;
    int i = 0;

    if (size < 0 || size > MAX_POOL_SIZE) {
        printf("memory_pool_init(): Invalid size(%d)\n", size);
        return pool;
    }

    block_cnt = ((size + BLOCK_SIZE - 1) / BLOCK_SIZE);
    all_size = sizeof(Memory_Pool) + (sizeof(Memory_Map_Table) +
            sizeof(Memory_Alloc_Table)) * block_cnt + size;
    p = (char *)malloc(all_size);
    if (p == NULL) {
        perror("Malloc failed\n");
        return pool;
    }

    memset(p, 0, all_size);

    pool = (Memory_Pool *)p;
    pool->block_cnt = block_cnt;
    pool->block_size = BLOCK_SIZE;
    pool->increment = increment;
    pool->internal_total_size = BLOCK_SIZE * block_cnt;
    pool->total_size = size;
    pool->used_size = 0;
    pool->alloc_cnt = 0;
    pool->memory_start = p;

    p_memory = memory_pos(pool);
    map_table = map_table_pos(pool);
    for (i = 0; i < block_cnt; i++) {
        p_map_table = (Memory_Map_Table *)((char *)map_table + i * sizeof(Memory_Map_Table));
        p_map_table->index = 0;
        p_map_table->p_block = p_memory + i * BLOCK_SIZE;
        p_map_table->used = 0;
    }

    alloc_table = alloc_talbe_pos(pool);
    for (i = 0; i < block_cnt; i++) {
        p_alloc_table = (Memory_Alloc_Table *)((char *)alloc_table + i * sizeof(Memory_Alloc_Table));
        p_alloc_table->block_cnt = 0;
        p_alloc_table->block_start_index = -1;
        p_alloc_table->p_start = NULL;
        p_alloc_table->used = 0;
    }

    printf("memory_pool_init: total size: %d, block cnt: %d, block size: %d\n",
            pool->total_size, pool->block_cnt, BLOCK_SIZE);
    s->x = pool;
    return s;
}

#define LOOP 5
#define ALLOC_SIZE 8

int test_mempool()
{
    Memory_Pool *pool = NULL;
    char *p1 = NULL;
    char *p2 = NULL;
    int i = 0;



    pool = memory_pool_init(1024, 512);
    if (pool == NULL)
        printf("memory pool init failed\n");

    for (i = 0; i < 2; i++) {
        p1 = (char *)Memory_malloc(pool, ALLOC_SIZE);
        if (p1 == NULL)
            printf("Malloc failed\n");
        else
            printf("Malloc success\n");

        memory_free(pool, p1);
    }


    p1 = (char *)Memory_malloc(pool, 256);
    if (p1 == NULL)
        printf("Malloc failed\n");
    else
        printf("Malloc success\n");

    p2 = (char *)Memory_malloc(pool, 512);
    if (p1 == NULL)
        printf("Malloc failed\n");
    else
        printf("Malloc success\n");

    memory_free(pool, p1);

    p1 = (char *)Memory_malloc(pool, 256);
    if (p1 == NULL)
        printf("Malloc failed\n");
    else
        printf("Malloc success\n");

    memory_pool_destroy(pool);

    return 0;
}
