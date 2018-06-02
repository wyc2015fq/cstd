
#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

struct Slot_ {
  Slot_*next;
  size_t size;
};

#define allocator_realloc(x, p, size)  (x)->_realloc((x), p, size)
#define allocator_alloc(x, size)  (x)->_realloc((x), NULL, size)
#define allocator_free(x, p)  (x)->_free((x), p)
struct allocator {
  void* (*_realloc)(struct allocator* s, void* p, size_t size);
  void (*_free)(struct allocator* s, void* p);
  void* x;
};

struct MemoryPool
{
  struct allocator base;
  Slot_* currentBlock_;
  Slot_* currentSlot_;
  Slot_* lastSlot_;
  Slot_* freeSlots_;
};

size_t padPointer(void* p, size_t align) {
  size_t result = (size_t)(p);
  return ((align - result) % align);
}

void MemoryPool_free(struct allocator* s)
{
  MemoryPool* x = (MemoryPool*)(s->x);
  struct allocator* a = &x->base;
  Slot_* curr = x->currentBlock_;
  while (curr != 0) {
    Slot_* prev = curr->next;
    allocator_free(a, curr);
    curr = prev;
  }
  allocator_free(a, x);
  memset(s, 0, sizeof(*s));
}

void MemoryPool_allocateBlock(struct allocator* s, size_t size)
{
  enum {MinBlockSize = 4096};
  int BlockSize = MAX(MinBlockSize, (size+sizeof(Slot_)*4));
  MemoryPool* x = (MemoryPool*)(s->x);
  struct allocator* a = &x->base;
  // Allocate space for the new block and store a pointer to the previous one
  char* newBlock = (char*)allocator_alloc(a, BlockSize);
  ((Slot_*)newBlock)->next = x->currentBlock_;
  x->currentBlock_ = (Slot_*)(newBlock);
  // Pad block body to staisfy the alignment requirements for elements
  char* body = newBlock + sizeof(Slot_*);
  size_t bodyPadding = padPointer(body, sizeof(Slot_));
  x->currentSlot_ = (Slot_*)(body + bodyPadding);
  x->lastSlot_ = (Slot_*)(newBlock + BlockSize - sizeof(Slot_) + 1);
}

void* MemoryPool_realloc(struct allocator* s, void* p, size_t size)
{
  MemoryPool* x = (MemoryPool*)(s->x);
  struct allocator* a = &x->base;
  if (x->freeSlots_ != 0) {
    char* result = (char*)(x->freeSlots_);
    x->freeSlots_ = x->freeSlots_->next;
    return result;
  }
  else {
    if (x->currentSlot_ >= x->lastSlot_) {
      MemoryPool_allocateBlock(x, size);
    }
    return (x->currentSlot_++);
  }
}

void MemoryPool_free(struct allocator* s, void* p);
{
  MemoryPool* x = (MemoryPool*)(s->x);
  struct allocator* a = &x->base;
  if (p != 0) {
    (Slot_*)(p)->next = x->freeSlots_;
    x->freeSlots_ = (Slot_*)(p);
  }
}



#if 0




inline void
construct(pointer p, const_reference val)
{
  new (p) value_type (val);
}




inline void
destroy(pointer p)
{
  p->~value_type();
}




inline typename pointer
newElement(const_reference val)
{
  pointer result = allocate();
  construct(result, val);
  return result;
}




inline void
deleteElement(pointer p)
{
  if (p != 0) {
    p->~value_type();
    deallocate(p);
  }
}

#endif


#endif // MEMORY_POOL_H
