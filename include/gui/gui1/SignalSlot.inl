
#define INIT_LIST_HEAD(ptr) do { \
  (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

template<typename T>
static inline void __list_add(T* ptr, T* prev, T* next)
{
  ptr->next = next;
  ptr->prev = prev;
  if (next) {
    next->prev = ptr;
  }
  if (prev) {
    prev->next = ptr;
  }
}
template<typename T>
static inline void list_add_head(T* ptr, T** head)
{
  if (*head) {
    __list_add(ptr, (*head)->prev, (*head));
  } else {
    ptr->next = NULL;
    ptr->prev = NULL;
  }
  *head = ptr;
}
template<typename T>
static inline T** list_get_tail(T** head)
{
  if (*head) {
    while ((*head)->next) {
      head = &((*head)->next);
    }
  }
  return head;
}

template<typename T>
static inline void list_add_tail(T* ptr, T** head)
{
  T** tail = list_get_tail(head);
  if (*tail) {
    __list_add(ptr, (*tail), (*tail)->next);
  } else {
    *tail = ptr;
    ptr->next = NULL;
    ptr->prev = NULL;
  }
}
template<typename T>
static inline void __list_del(T* prev, T* next)
{
  if (next) {
    next->prev = prev;
  }
  if (prev) {
    prev->next = next;
  }
}
template<typename T>
static inline void list_del(T* ptr)
{
  if (ptr) {
    __list_del(ptr->prev, ptr->next);
    ptr->next = (T*)0;
    ptr->prev = (T*)0;
  }
}

template<typename T>
static inline void list_destroy(T** head)
{
  if (*head) {
    T* iter = *head;
    for (; iter; ) {
      T* iter_next = iter->next;
      list_del(iter);
      delete iter;
      iter = iter_next;
    }
    *head = NULL;
  }
}

struct Slot {
  virtual ~Slot() {}
};

template<typename Arg0>
struct Signal;

template<typename Arg0>
struct _Slot : public Slot {
  virtual void exec(Arg0 args) = 0;
  _Slot* next;
  _Slot* prev;
  Signal<Arg0>* _signal;
  _Slot(): _signal(NULL), prev(NULL), next(NULL) {}
  virtual ~_Slot() {
    unconnect();
  }
  virtual void unconnect() {
    if (this == _signal->_slot) {
      _signal->_slot = _signal->_slot->next;
    }
    list_del(this);
    //list_destroy(&_signal);
  }
};

template<typename Arg0, typename FUN>
struct aSlot : public _Slot<Arg0> {
  aSlot(FUN func) : m_func(func) {}
  FUN m_func;
  virtual void exec(Arg0 args) {
    m_func(args);
  }
};
template<typename Arg0, typename RET, typename T, typename Y>
struct bSlot : public _Slot<Arg0> {
  RET (T::*m_func) (Arg0 x);
  Y* m_t;
  bSlot(RET(T::*func) (Arg0 x), Y* t) : m_func(func) : m_t(t) {}
  virtual void exec(Arg0 args) {
    (m_t->*m_func) (args);
  }
};
template<typename Arg0, typename FUN, typename Y>
struct cSlot : public _Slot<Arg0> {
  FUN m_func;
  Y* m_t;
  cSlot(FUN func, Y* t) : m_func(func), m_t(t) {}
  virtual void exec(Arg0 args) {
    (m_t->*m_func) (args);
  }
};

template<typename Arg0>
struct Signal {
  //Signal* next;
  //Signal* prev;
  _Slot<Arg0>* _slot;
  Signal() : _slot(NULL) //, prev(NULL), next(NULL)
  {}
  ~Signal() {
    //list_del(this);
    list_destroy(&_slot);
  }
  void push_back(_Slot<Arg0>* slot) {
    slot->_signal = this;
    list_add_tail(slot, &_slot);
  }
  template<typename RET, typename T, typename Y>
  Slot* connect1(RET(T::*func) (Arg0 x), Y* t) {
    bSlot<Arg0, RET, T, Y>* s = new bSlot<Arg0, RET, T, Y>(func, t);
    push_back(s);
    return s;
  }
  template<typename FUN, typename Y>
  Slot* connect2(FUN func, Y* t) {
    cSlot<Arg0, FUN, Y>* s = new cSlot<Arg0, FUN, Y>(func, t);
    push_back(s);
    return s;
  }
  template<typename FUN>
  Slot* connect(FUN func) {
    aSlot<Arg0, FUN>* s = new aSlot<Arg0, FUN>(func);
    push_back(s);
    return s;
  }
  void emit(Arg0 args) {
    _Slot<Arg0>* iter = _slot;
    for (; iter; iter = iter->next) {
      iter->exec(args);
    }
  }
};

//////////////////////////////////////////////////////////////////////////


template<typename Arg0, typename Arg1>
struct Signal2;

template<typename Arg0, typename Arg1>
struct _Slot2 : public Slot {
  virtual void exec(Arg0 arg0, Arg1 arg1) = 0;
  _Slot2* next;
  _Slot2* prev;
  Signal2<Arg0, Arg1>* _signal;
  _Slot2(): _signal(NULL), prev(NULL), next(NULL) {}
  virtual ~_Slot2() {
    unconnect();
  }
  virtual void unconnect() {
    if (this == _signal->_slot) {
      _signal->_slot = _signal->_slot->next;
    }
    list_del(this);
    //list_destroy(&_signal);
  }
};

template<typename Arg0, typename Arg1, typename FUN>
struct aSlot2 : public _Slot2<Arg0, Arg1> {
  aSlot2(FUN func) : m_func(func) {}
  FUN m_func;
  virtual void exec(Arg0 arg0, Arg1 arg1) {
    m_func(arg0, arg1);
  }
};
template<typename Arg0, typename Arg1, typename RET, typename T, typename Y>
struct bSlot2 : public _Slot2<Arg0, Arg1> {
  RET (T::*m_func) (Arg0, Arg1 x);
  Y* m_t;
  bSlot2(RET(T::*func) (Arg0, Arg1 & x), Y* t) : m_func(func) : m_t(t) {}
  virtual void exec(Arg0 arg0, Arg1 arg1) {
    (m_t->*m_func) (arg0, arg1);
  }
};
template<typename Arg0, typename Arg1, typename FUN, typename Y>
struct cSlot2 : public _Slot2<Arg0, Arg1> {
  FUN m_func;
  Y* m_t;
  cSlot2(FUN func, Y* t) : m_func(func), m_t(t) {}
  virtual void exec(Arg0 arg0, Arg1 arg1) {
    (m_t->*m_func) (arg0, arg1);
  }
};

template<typename Arg0, typename Arg1>
struct Signal2 {
  //Signal2* next;
  //Signal2* prev;
  _Slot2<Arg0, Arg1>* _slot;
  Signal2() : _slot(NULL) //, prev(NULL), next(NULL)
  {}
  ~Signal2() {
    //list_del(this);
    list_destroy(&_slot);
  }
  void push_back(_Slot2<Arg0, Arg1>* slot) {
    slot->_signal = this;
    list_add_tail(slot, &_slot);
  }
  template<typename FUN, typename Y>
    Slot* connect2(FUN func, Y* t) {
    cSlot2<Arg0, Arg1, FUN, Y>* s = new cSlot2<Arg0, Arg1, FUN, Y>(func, t);
    push_back(s);
    return s;
  }
  template<typename FUN>
    Slot* connect(FUN func) {
    aSlot2<Arg0, Arg1, FUN>* s = new aSlot2<Arg0, Arg1, FUN>(func);
    push_back(s);
    return s;
  }
  void emit(Arg0 arg0, Arg1 arg1) {
    _Slot2<Arg0, Arg1>* iter = _slot;
    for (; iter; iter = iter->next) {
      iter->exec(arg0, arg1);
    }
  }
};


