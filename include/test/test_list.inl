
//  * Simple doubly linked list implementation.
//  *
//  * Some of the internal functions (※__xxx§) are useful when
//  * manipulating whole lists rather than single entries, as
//  * sometimes we already know the next/prev entries and we can
//  * generate better code by using them directly rather than
//  * using the generic single-entry routines.
//  

struct list_head {
  struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
  list->next = list;
  list->prev = list;
}
// Insert a new entry between two known consecutive entries.
// 
// This is only for internal list manipulation where we know
// the prev/next entries already!

static inline void __list_add(struct list_head *new1, struct list_head *prev, struct list_head *next)
{
  next->prev = new1;
  new1->next = next;
  new1->prev = prev;
  prev->next = new1;
}

// list_add 每 add a new entry
// @new: new entry to be added
// @head: list head to add it after
// 
// Insert a new entry after the specified head.
// This is good for implementing stacks.
//   
static inline void list_add(struct list_head *new1, struct list_head *head)
{
  __list_add(new1, head, head->next);
}

// 
// list_add_tail 每 add a new entry
// @new: new entry to be added
// @head: list head to add it before
// 
// Insert a new entry before the specified head.
// This is useful for implementing queues.

static inline void list_add_tail(struct list_head *new1, struct list_head *head)
{
  __list_add(new1, head->prev, head);
}

// Delete a list entry by making the prev/next entries
// point to each other.
// 
// This is only for internal list manipulation where we know
// the prev/next entries already!

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
  next->prev = prev;
  prev->next = next;
}

// 
// list_del 每 deletes entry from list.
// @entry: the element to delete from the list.
// Note: list_empty on entry does not return true after this, the entry is in an undefined state.

static inline void list_del(struct list_head *entry)
{
  __list_del(entry->prev, entry->next);
  entry->next = (struct list_head *) 0;
  entry->prev = (struct list_head *) 0;
}

// 
// list_del_init 每 deletes entry from list and reinitialize it.
// @entry: the element to delete from the list.

static inline void list_del_init(struct list_head *entry)
{
  __list_del(entry->prev, entry->next);
  INIT_LIST_HEAD(entry);
}

// 
// list_move 每 delete from one list and add as another＊s head
// @list: the entry to move
// @head: the head that will precede our entry

static inline void list_move(struct list_head *list, struct list_head *head)
{
  __list_del(list->prev, list->next);
  list_add(list, head);
}

// 
// list_move_tail 每 delete from one list and add as another＊s tail
// @list: the entry to move
// @head: the head that will follow our entry

static inline void list_move_tail(struct list_head *list,
                                  struct list_head *head)
{
  __list_del(list->prev, list->next);
  list_add_tail(list, head);
}

// 
// list_empty 每 tests whether a list is empty
// @head: the list to test.

static inline int list_empty(struct list_head *head)
{
  return head->next == head;
}

static inline void __list_splice(struct list_head *list,
                                 struct list_head *head)
{
  struct list_head *first = list->next;
  struct list_head *last = list->prev;
  struct list_head *at = head->next;
  
  first->prev = head;
  head->next = first;
  
  last->next = at;
  at->prev = last;
}

// 
// list_splice 每 join two lists
// @list: the new list to add.
// @head: the place to add it in the first list.

static inline void list_splice(struct list_head *list, struct list_head *head)
{
  if (!list_empty(list))
    __list_splice(list, head);
}

// 
// list_splice_init 每 join two lists and reinitialise the emptied list.
// @list: the new list to add.
// @head: the place to add it in the first list.
// 
// The list at @list is reinitialised

static inline void list_splice_init(struct list_head *list,
                                    struct list_head *head)
{
  if (!list_empty(list)) {
    __list_splice(list, head);
    INIT_LIST_HEAD(list);
  }
}

// 
// list_entry 每 get the struct for this entry
// @ptr:    the &struct list_head pointer.
// @type:    the type of the struct this is embedded in.
// @member:    the name of the list_struct within the struct.
#define list_entry(ptr, type, member) ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

// 
// list_for_each    -    iterate over a list
// @pos:    the &struct list_head to use as a loop counter.
// @head:    the head for your list.

#define list_for_each(pos, head) \
  for (pos = (head)->next; pos != (head); \
pos = pos->next)
// 
// list_for_each_prev    -    iterate over a list backwards
// @pos:    the &struct list_head to use as a loop counter.
// @head:    the head for your list.

#define list_for_each_prev(pos, head) \
  for (pos = (head)->prev; pos != (head); \
pos = pos->prev)

// 
// list_for_each_safe    -    iterate over a list safe against removal of list entry
// @pos:    the &struct list_head to use as a loop counter.
// @n:        another &struct list_head to use as temporary storage
// @head:    the head for your list.

#define list_for_each_safe(pos, n, head) \
  for (pos = (head)->next, n = pos->next; pos != (head);\
pos = n, n = pos->next)


// 
// list_for_each_entry    -    iterate over list of given type
// @pos:    the type * to use as a loop counter.
// @head:    the head for your list.
// @member:    the name of the list_struct within the struct.
#define list_for_each_entry(pos, head, type, member)                \
  for (pos = list_entry((head)->next, type, member); &pos->member != (head); \
pos = list_entry(pos->member.next, type, member))

// list_for_each_entry_safe 每 iterate over list of given type safe against removal of list entry
// @pos:    the type * to use as a loop counter.
// @n:        another type * to use as temporary storage
// @head:    the head for your list.
// @member:    the name of the list_struct within the struct.

#define list_for_each_entry_safe(pos, n, head, member)            \
  for (pos = list_entry((head)->next, typeof(*pos), member),    \
  n = list_entry(pos->member.next, typeof(*pos), member);    \
  &pos->member != (head);                     \
pos = n, n = list_entry(n->member.next, typeof(*n), member))







struct kool_list{
  int to;
  struct list_head list;
  int from;
};

#include "cstd.h"

int main(int argc, char **argv){
  
  struct kool_list *tmp;
  struct list_head *pos, *q;
  unsigned int i;
  
  struct kool_list mylist;
  INIT_LIST_HEAD(&mylist.list);
  //     or you could have declared this with the following macro
  //        * LIST_HEAD(mylist); which declares and initializes the list
  //        
  
  /* adding elements to mylist */
  for(i=5; i!=0; --i){
    tmp= (struct kool_list *)malloc(sizeof(struct kool_list));
    
    //  INIT_LIST_HEAD(&tmp->list); 
    // 
    // this initializes a dynamically allocated list_head. we
    // you can omit this if subsequent call is add_list() or 
    // anything along that line because the next, prev
    // fields get initialized in those functions.
    
    printf("enter to and from:");
    scanf("%d %d", &tmp->to, &tmp->from);
    
    /* add the new item 'tmp' to the list of items in mylist */
    list_add(&(tmp->list), &(mylist.list));
    /* you can also use list_add_tail() which adds new items to
    * the tail end of the list
    */
  }
  printf("\n");
  
  
  //  now you have a circularly linked list of items of type struct kool_list.
  // now let us go through the items and print them out
  
  
  
  //  list_for_each() is a macro for a for loop. 
  // first parameter is used as the counter in for loop. in other words, inside the
  // loop it points to the current item's list_head.
  // second parameter is the pointer to the list. it is not manipulated by the macro.
  
  printf("traversing the list using list_for_each()\n");
  list_for_each(pos, &mylist.list){
    
    //  at this point: pos->next points to the next item's 'list' variable and 
    // pos->prev points to the previous item's 'list' variable. Here item is 
    // of type struct kool_list. But we need to access the item itself not the 
    // variable 'list' in the item! macro list_entry() does just that. See "How
    // does this work?" below for an explanation of how this is done.
    
    tmp= list_entry(pos, struct kool_list, list);
    
    //  given a pointer to struct list_head, type of data structure it is part of,
    //  * and it's name (struct list_head's name in the data structure) it returns a
    //  * pointer to the data structure in which the pointer is part of.
    //  * For example, in the above line list_entry() will return a pointer to the
    //  * struct kool_list item it is embedded in!
    //  
    
    printf("to= %d from= %d\n", tmp->to, tmp->from);
    
  }
  printf("\n");
  //  since this is a circularly linked list. you can traverse the list in reverse order
  // as well. all you need to do is replace 'list_for_each' with 'list_for_each_prev'
  // everything else remain the same!
  // 
  // Also you can traverse the list using list_for_each_entry() to iterate over a given
  // type of entries. For example:
  
  printf("traversing the list using list_for_each_entry()\n");
  list_for_each_entry(tmp, &mylist.list, struct kool_list, list) {
    printf("to= %d from= %d\n", tmp->to, tmp->from);
  }
  list_for_each_entry(tmp,&mylist.list,struct kool_list, list){
    if(tmp->to == 2)
      break;
  }
  printf("to = %d from %d\n",tmp->to,tmp->from);
  printf("\n");
  
  //  now let's be good and free the kool_list items. since we will be removing items
  // off the list using list_del() we need to use a safer version of the list_for_each() 
  // macro aptly named list_for_each_safe(). Note that you MUST use this macro if the loop 
  // involves deletions of items (or moving items from one list to another).
  
  printf("deleting the list using list_for_each_safe()\n");
  list_for_each_safe(pos, q, &mylist.list){
    tmp= list_entry(pos, struct kool_list, list);
    printf("freeing item to= %d from= %d\n", tmp->to, tmp->from);
    list_del(pos);
    free(tmp);
  }
  
  return 0;
}