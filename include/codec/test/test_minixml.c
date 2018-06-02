#include <stdio.h>
#include <assert.h>

#include "minixml.h"

void testList();

int main() {
  int i = 0;
  
  //for(i =0; i<10000; i++)
  {
    //testList();
    xmlNodePtr child;
    xmlNodePtr nameNode;
    
    xmlDocPtr doc = xmlNewDoc();
    doc->root = xmlNewNode( "employees" );
    
    child = xmlNewNode( "employee" );
    child = xmlAddChild( doc->root, child );
    
    xmlAddProp( child, "devtype", "mdacout" );
    xmlAddProp( child, "age", "28" );
    xmlAddProp( child, "sexy", "male" );
    
    //add name node
    nameNode = xmlAddChild( child, xmlNewNode( "name" ) );
    xmlAddText( nameNode, xmlNewText( "Nicholas C. Zakas" ) );
    
    child = xmlAddChild( doc->root, xmlNewNode( "employee" ) );
    xmlAddProp( child, "age", "22" );
    xmlAddProp( child, "sexy", "male" );
    nameNode = xmlAddChild( child, xmlNewNode( "name" ) );
    xmlAddText( nameNode, xmlNewText( "Jim Smith" ) );
    
    printfXmlDoc( stdout, doc );
    xmlFreeDoc( doc );
  }
  return 0;
}

