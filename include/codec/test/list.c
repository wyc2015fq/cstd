/** \file
* \brief C list implementation
*
* (C) 2007 GEC All rights reserved.
*
* \author Deng Yangjun
* \date 2006-1-28
*/
#include <memory.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "list.h"

/*!
*\brief create a new list strut
*\return new list pointer, you MUST release it by destroyList(listPtr list)
*/
listPtr createList() {
  listPtr newList = malloc( sizeof( list ) );
  assert( NULL != newList );
  
  memset( newList, 0, sizeof( list ) );
  return newList;
}

/*!
*\brief create a new list strut
*\param list [in,out]list that will be add the new link
*\param data [in]value of the new link
*\return the new link 's point, can't free it, please free it by destroyList or removeLink 
*/
nodePtr addLink( listPtr list, void *data ) {
  nodePtr newLink = malloc( sizeof( node ) );
  
  assert( newLink != NULL );
  assert( list != NULL );
  
  //printf("new link\n");
  
  memset( newLink, 0, sizeof( node ) );
  
  if ( NULL == list->first )   // empty list, add the link as first link
  {
    list->first = list->last = newLink;
  } else {
    assert( list->last != NULL );
    
    list->last->next = newLink;
    list->last = newLink;
  }
  
  newLink->data = data;
  
  return newLink;
}

/**
* \brief destroy a list
* \param listPtrPtr [in,out] list that will be destroyed, after the function, the list's pointer will be NULL
* \param onFreeData [in] register user data free hander function. when destroy link, it will be invoked.
* if you don't need to free user data, you can set it NULL
*/
void destroyList( listPtr *listPtrPtr, freeUserData onFreeData ) {

  nodePtr next;
  listPtr list;
  assert( listPtrPtr != NULL );
  
  list = *listPtrPtr;
  assert( list != NULL );
  
  while ( list->first ) {
    next = list->first->next;
    if ( onFreeData != NULL ) {
      onFreeData( list->first->data );
    }
    
    free( list->first );
    //printf("free link\n");
    list->first = next;
  }
  
  free( list );
  *listPtrPtr = NULL;
}

