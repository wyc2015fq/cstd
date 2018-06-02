/**\file
* \brief Mini XML lib
*
* (C) 2007 GEC. All rights reserved.
*
* \version 0.1
* \author Deng Yangjun 
* \author dyj057@gmail.com
* \date 2007-1-28
* \warning Because the string pointer  of  'name', 'value', 'prop' will be used in printfDoc function, 
* so you can't release it before invoke printfDoc function
* \bug 
* - Encoding
* -# can't support chinese language translate into UTF-8 
* -# only support ASCII
* - Need more strong check (be careful)
* -# not check tag name
* -# not set transferred meaning letter for text value
* - Not support namespace
* \todo 
* - Add chinese support
* - Check name and value validity
* - Xml Parser
*/
#ifndef MINIXML_H_
#define MINIXML_H_

#include <stdio.h>
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

  //! XML property node struct
  typedef struct xmlPropNode {
    char *name;          //!< name of property
    char *value;         //!< value of property
  }
  xmlPropNode,             //! XML property node struct
  *xmlPropNodePtr;       //! XML property node struct pointer
  
  //! XML text node struct
  typedef struct xmlTextNode {
    char *value;         //!<value of the text node
  }
  xmlTextNode,             //! XML text node struct
  *xmlTextNodePtr;       //! XML text node struct pointer
  
  //! XML node struct
  // it must be a text node or element node, can't contain both
  typedef struct xmlNode {
    listPtr children;    //!< children element node list in the element node
    xmlTextNodePtr text; //!< text node in the element node
    char *name;          //!< tag name of element node
    listPtr props;       //!< property nodes list
  }
  xmlNode,                 //! XML node struct
  *xmlNodePtr;           //! XML node struct pointer
  
  //! XML document struct
  typedef struct xmlDoc {
    char *version;       //!< XML version default 1.0
    char *encoding;      //!< XML doc encoding, default UTF-8
    xmlNodePtr root;     //!< root node in XML document
  }
  xmlDoc,                  //! xmlDoc struct
  *xmlDocPtr;            //! xmlDoc struct pointer
  
  
  xmlDocPtr xmlNewDoc();
  void xmlFreeDoc( xmlDocPtr doc );
  xmlNodePtr xmlNewNode( const char * name );
  xmlNodePtr xmlAddChild( xmlNodePtr father, xmlNodePtr child );
  xmlTextNodePtr xmlNewText( const char * text );
  void xmlAddText( xmlNodePtr node, xmlTextNodePtr text );
  void xmlAddProp( xmlNodePtr node, const char * propName, const char * propValue );
  void printfXmlDoc( FILE * fc, xmlDocPtr doc );
  
#ifdef __cplusplus
} //end of extern "C" {
#endif

#endif
