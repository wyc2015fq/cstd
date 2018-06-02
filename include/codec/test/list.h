/** \file
* <pre><b>C list</b></pre> 
* <pre><b>(C) 2007 GEC All rights reserved.</b></pre> 
* \author deng yangjun
* \date 2007-1-28
*/
#ifndef LIST_H_
#define LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

  //! An node in list
  typedef struct node {
    void *data; //!< user data
    struct node *next; //!< point to next link
  }
  node,       //! stuct  node struct
  *nodePtr; //! struct node pointer
  
  //!  list struct
  typedef struct list {
    nodePtr first;  //!< first node in list
    nodePtr last;   //!< last node in list
  }
  list,               //! list struct
  *listPtr;         //! list struct pointer
  
  //!  free user data handler
  typedef void ( *freeUserData ) ( void * data );
  
  listPtr createList();
  nodePtr addLink( listPtr list, void * data );
  void destroyList( listPtr * listPtrPtr, freeUserData onFreeData );
  
#ifdef __cplusplus
} //end of extern "C" {
#endif

#endif

