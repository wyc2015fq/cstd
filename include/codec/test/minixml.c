/**\file
* \brief Mini XML lib
*
* (C) 2007 GEC. All rights reserved.
*
* \version 0.1
* \author Deng Yangjun
* \date 2007-1-28
*/
#include <memory.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string.h>

#include "minixml.h"

void xmlFreeNode( xmlNodePtr node );

char *copyString( const char *s ) {
  size_t strLen = strlen( s );
  char * r = ( char * ) malloc( strLen + 1 );
  memcpy( r, s, strLen );
  r[ strLen ] = '\0';
  return r;
}

void freeString( char *s ) {
  if ( s != NULL ) {
    free( s );
  }
}

/*!
*\brief create a new XML document
*\return new XML document pointer, you MUST release it by xmlFreeDoc
*/
xmlDocPtr xmlNewDoc() {
  xmlDocPtr doc = ( xmlDocPtr ) malloc( sizeof( xmlDoc ) );
  assert( doc != NULL );
  
  doc->encoding = "utf-8";
  doc->version = "1.0";
  doc->root = NULL;
  
  return doc;
}

/*!
*\brief free XML document struct
*\param doc [in] doc that want to destroy
*/
void xmlFreeDoc( xmlDocPtr doc ) {
  assert( doc != NULL );
  
  if ( NULL != doc->root ) {
    xmlFreeNode( doc->root );
  }
  
  free( doc );
}

/*!
*\brief free a property node
*\param data [in] property node pointer
*/
void onFreePropNode( void *data ) {
  xmlPropNode * prop = ( xmlPropNode * ) data;
  assert( data != NULL );
  freeString( prop->name );
  freeString( prop->value );
  free( data );
}

/*!
*\brief free a element node
*\param data [in] element node pointer
*/
void onFreeNode( void *data ) {
  xmlFreeNode( ( xmlNodePtr ) data );
}


/*!
*\brief free XML document struct
*\param node [in] XML node that want to destroy
*/
void xmlFreeNode( xmlNodePtr node ) {
  assert( node != NULL );
  
  freeString( node->name );
  
  if ( node->text != NULL ) {
    freeString( node->text->value );
    free( node->text );
  }
  
  if ( node->props != NULL ) {
    destroyList( &node->props, onFreePropNode );
  }
  
  if ( node->children != NULL ) {
    destroyList( &node->children, onFreeNode );
  }
  
  free( node );
}


/*!
*\brief add a text node to a element
*\param node [in] XML element node
*\param  text [in] text node
*/
void xmlAddText( xmlNodePtr node, xmlTextNodePtr text ) {
  assert( node != NULL );
  assert( node->children == NULL );
  node->text = text;
}

/*!
*\brief create a new element node
*\param name [in] tag name of the new element node
*\return new element node
*/
xmlNodePtr xmlNewNode( const char *name ) {
  xmlNodePtr node = ( xmlNodePtr ) malloc( sizeof( xmlNode ) );
  assert( node != NULL );
  assert( name != NULL );
  assert( strcmp( name, "" ) != 0 );
  
  memset( node, 0, sizeof( xmlNode ) );
  node->name = copyString( name );
  return node;
}

/*!
*\brief create a new text node
*\param text [in] valve of the text node
*\return new text node
*/
xmlTextNodePtr xmlNewText( const char *text ) {
  xmlTextNodePtr textNode = ( xmlTextNodePtr ) malloc( sizeof( xmlTextNode ) );
  assert( text != NULL );
  assert( textNode != NULL );
  
  //copy the text value
  textNode->value = copyString( text );
  
  return textNode;
}

/*!
*\brief add child node to the father node
*\param father [in] father node
*\param child [in] child node
*\return child node
*/
xmlNodePtr xmlAddChild( xmlNodePtr father, xmlNodePtr child ) {
  assert( father != NULL );
  assert( child != NULL );
  assert( father->text == NULL ); //it's not a text node
  
  if ( father->children == NULL ) {
    father->children = createList();
  }
  
  addLink( father->children, child );
  
  return child;
}

static int tabs = -1;

/*!
*\brief print node property nodes's XML to a file
*\param f [in] file pointer
*\param node [in] print node
*/
void printfProps( FILE *f, xmlNodePtr node ) {
  if ( node->props != NULL ) {
    nodePtr next = node->props->first;
    while ( next ) {
      xmlPropNodePtr prop = ( xmlPropNodePtr ) next->data;
      
      fprintf( f, " %s=\"%s\"", prop->name, prop->value );
      next = next->next;
    }
  }
}

/*!
*\brief print node's XML to a file
*\param f [in] file pointer
*\param node [in] print node
*/
void printfXmlNode( FILE *f, xmlNodePtr node ) {
  int i;
  if ( node == NULL ) {
    return ;
  }
  tabs ++;
  for ( i = 0; i < tabs; ++i ) {
    fprintf( f, "\t" );
  }
  
  if ( node->text != NULL ) {
    fprintf( f, "<%s", node->name );
    printfProps( f, node );
    fprintf( f, ">" );
    
    //text node
    fprintf( f, "%s</%s>\n", node->text->value, node->name );
  } else if ( node->children != NULL ) {
    nodePtr next = node->children->first;
    
    fprintf( f, "<%s", node->name );
    printfProps( f, node );
    fprintf( f, ">\n" );
    
    while ( next ) {
      printfXmlNode( f, ( xmlNodePtr ) next->data );
      next = next->next;
    }
    
    for ( i = 0; i < tabs; ++i ) {
      fprintf( f, "\t" );
    }
    
    fprintf( f, "</%s>\n", node->name ); //
  } else {
    fprintf( f, "<%s", node->name );
    printfProps( f, node );
    fprintf( f, ">" );
    fprintf( f, "</%s>\n", node->name ); // empty node
  }
  
  tabs--;
}

/*!
*\brief add property to a node
*\param node [in] file pointer
*\param propName [in] name of the property
*\param propValue [in] value of the property
*/
void xmlAddProp( xmlNodePtr node, const char *propName, const char *propValue ) {
  xmlPropNodePtr prop = ( xmlPropNodePtr ) malloc( sizeof( xmlPropNode ) );
  assert( prop != NULL );
  assert( node != NULL );
  assert( propName != NULL );
  assert( propValue != NULL );
  if ( NULL == node->props ) {
    node->props = createList();
  }
  
  prop->name = copyString( propName );
  prop->value = copyString( propValue );
  
  addLink( node->props, prop );
}

/*!
*\brief print document's XML to a file
*\param f [in] file pointer
*\param doc [in] doc pointer
*/
void printfXmlDoc( FILE* f, xmlDocPtr doc ) {
  fprintf( f, "<?xml version=\"%s\" encoding=\"%s\"?>\n", doc->version, doc->encoding );
  printfXmlNode( f, doc->root );
}
