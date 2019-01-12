
#ifndef _LIST_H_
#define _LIST_H_

////////////////////////////////////////////////////////////////////////////////
#define LIST_PUSHBACK(list, item, tmp) if (list) { tmp = list; while(tmp->next) { tmp = tmp->next; } tmp->next = item; } else { list = item; }
#define LIST_PUSHBACK_T(list, item, T) if (list) { T* tmp = list; while(tmp->next) { tmp = tmp->next; } tmp->next = item; } else { list = item; }
#define LIST_POPFRONT(list, item) { item = list; if (list) { list = list->next; } }
#define LIST_ADDFRONT1(list, item) if (list) { item->next = list; list = item; } else { item->next = NULL; list = item; }
#define LIST_ADDFRONT(list, item) if (list) { item->prev = NULL; item->next = list; list->prev = item; list = item; } else { item->prev = item->next = NULL; list = item; }
#define LIST_DEL1(item)   if (item) { item = (item)->next; }
#define LIST_DEL(list, item) if (list==item) { list->prev = NULL; list = item->next; } else { if (item->prev) { item->prev->next = item->next; } if (item->next) { item->next->prev = item->prev; } }


#endif // _LIST_H_
