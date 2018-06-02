#include "strmap.inl"
#include "cstd.h"
#include "cmath.h"
#include <stdio.h>
static void iter(const char* key, const char* value, const void* obj)
{
    printf("key: %s value: %s\n", key, value);
}
int test_strmap()
{
    StrMap sm[1] = {0};
    char buf[255];
    int result;
    sm_new(sm, 1000);
    if (sm == NULL)
    {
        /* Handle allocation failure... */
    }
    /* Insert a couple of string associations */
    sm_put(sm, "application name", "Test Application");
    sm_put(sm, "application version", "1.0.0");
    {
        uint32 seed = 12;
        char key[32] = {0};
        char value[32] = {0};
        int i;
        for (i = 0; i < 100; ++i)
        {
            randstr(&seed, key, 5, "123");
            randstr(&seed, value, 31, "123456789abcdefghijklmnopqrstuvwxyz");
            sm_put(sm, key, value);
            result = sm_get(sm, key, buf, sizeof(buf));
            ASSERT(result && 0 == strcmp(value, buf));
        }
    }
    /* Retrieve a value */
    result = sm_get(sm, "application name", buf, sizeof(buf));
    if (result == 0)
    {
        ASSERT(0);
    }
    printf("value: %s\n", buf);
    sm_enum(sm, iter, NULL);
    /* When done, destroy the StrMap object */
    sm_delete(sm);
    return 0;
}
typedef struct  bucket_t_ bucket_t;
struct bucket_t_
{
    char* key;
    void* value;
    bucket_t* next;
    bucket_t* prev;
};
static unsigned long strhash(const char* str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
int has_loop(bucket_t* head)
{
    bucket_t* pf = head; /* fast pointer */
    bucket_t* ps = head; /* slow pointer */
    while (true)
    {
        if (pf && pf->next)
        {
            pf = pf->next->next;
        }
        else
        {
            return FALSE;
        }
        ps = ps->next;
        if (ps == pf)
        {
            return TRUE;
        }
    }
    return FALSE;
}
bucket_t* bucket_find(bucket_t* bucket, const char* key)
{
    for (; bucket; bucket = bucket->next)
    {
        if (0 == strcmp(bucket->key, key))
        {
            return bucket;
        }
    }
    return NULL;
}
bucket_t* shm_get(bucket_t** buckets, int nbuckets, const char* key, void* pvalue)
{
    unsigned int index = strhash(key) % nbuckets;
    bucket_t* bucket = buckets[index];
    bucket = bucket_find(bucket, key);
    if (pvalue)
    {
        *(void**)pvalue = bucket ? bucket->value : NULL;
    }
    return bucket;
}
bucket_t* shm_next(bucket_t** buckets, int nbuckets, bucket_t* cur, int* iter, void* pvalue)
{
    bucket_t* bucket = NULL;
    if (cur)
    {
        if (cur->next)
        {
            bucket = cur->next;
        }
        else
        {
            (*iter)++;
        }
    }
    if (NULL == bucket && *iter >= 0)
    {
        for (; *iter < nbuckets && NULL == (bucket = buckets[*iter]); (*iter)++);
    }
    if (pvalue)
    {
        *(void**)pvalue = bucket ? bucket->value : NULL;
    }
    return bucket;
}
int shm_del(bucket_t** buckets, int nbuckets, const char* key)
{
    unsigned int index = strhash(key) % nbuckets;
    bucket_t* bucket = buckets[index];
    bucket = bucket_find(bucket, key);
    if (bucket)
    {
        if (bucket->prev)
        {
            bucket->prev->next = bucket->next;
        }
        if (bucket->next)
        {
            bucket->next->prev = bucket->prev;
        }
        if (bucket == buckets[index])
        {
            buckets[index] = bucket->next;
        }
        bucket->next = NULL;
        bucket->prev = NULL;
    }
    return 0;
}
int shm_put(bucket_t** buckets, int nbuckets, char* key, void* value, bucket_t* new_bucket)
{
    unsigned int index = strhash(key) % nbuckets;
    bucket_t* bucket = buckets[index];
    bucket = bucket_find(bucket, key);
    if (bucket)
    {
        bucket->key = key;
        bucket->value = value;
    }
    else
    {
        new_bucket->key = key;
        new_bucket->value = value;
        new_bucket->prev = NULL;
        new_bucket->next = buckets[index];
        if (buckets[index])
        {
            buckets[index]->prev = new_bucket;
        }
        buckets[index] = new_bucket;
    }
    return index;
}
int test_shm()
{
    bucket_t* shm[1000] = {0};
    bucket_t* bk = 0;
    typedef struct
    {
        bucket_t bk[1];
        char key[32];
        char value[32];
    } strpair_t;
    strpair_t* sp = NULL;
    strpair_t* sp2 = NULL;
    int n = 1000;
    uint32 seed = 12;
    int i, j;
    MYREALLOC(sp, n);
    for (i = 0; i < n; ++i)
    {
        randstr(&seed, sp[i].key, 5, "012");
        randstr(&seed, sp[i].value, 31, "0123456789abcdefghijklmnopqrstuvwxyz");
        shm_put(shm, countof(shm), sp[i].key, sp + i, sp[i].bk);
        shm_get(shm, countof(shm), sp[i].key, &sp2);
        if (has_loop(sp[i].bk))
        {
            int asdf = 0;
        }
        if (!(sp2 && 0 == strcmp(sp2->value, sp[i].value)))
        {
            int asdf = 0;
        }
        ASSERT(sp2 && 0 == strcmp(sp2->value, sp[i].value));
    }
    bk = NULL;
    for (i = 0, j = 0; bk = shm_next(shm, countof(shm), bk, &j, &sp2); ++i)
    {
        printf("i:%d j:%d key: %s value: %s\n", i, j, sp2->key, sp2->value);
    }
    FREE(sp);
    return 0;
}

