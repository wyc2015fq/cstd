// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_XML_WRITER_H_INCLUDED__
#define __C_XML_WRITER_H_INCLUDED__
//#include <wchar.h>
//#include "IXMLWriter.h"
//#include "IWriteFile.h"
//! Interface providing methods for making it easier to write XML files.
class CXMLWriter : public IXMLWriter
{
public:
  //! Constructor
  CXMLWriter(IWriteFile* file);
  //! Destructor
  ~CXMLWriter();
  //! Writes a xml 1.0 header like <?xml version="1.0"?>
  void writeXMLHeader();
  //! Writes an xml element with maximal 5 attributes
  void writeElement(const char* name, bool empty = false,
      const char* attr1Name = 0, const char* attr1Value = 0,
      const char* attr2Name = 0, const char* attr2Value = 0,
      const char* attr3Name = 0, const char* attr3Value = 0,
      const char* attr4Name = 0, const char* attr4Value = 0,
      const char* attr5Name = 0, const char* attr5Value = 0);
  //! Writes an xml element with any number of attributes
  void writeElement(const char* name, bool empty,
      strv_t names, strv_t values);
  //! Writes a comment into the xml file
  void writeComment(const char* comment);
  //! Writes the closing tag for an element. Like </foo>
  void writeClosingTag(const char* name);
  //! Writes a text into the file. All occurrences of special characters like
  //! & (&amp;), < (&lt;), > (&gt;), and " (&quot;) are automaticly replaced.
  void writeText(const char* text);
  //! Writes a line break
  void writeLineBreak();
  struct XMLSpecialCharacters {
    char Character;
    const char* Symbol;
  };
private:
  void writeAttribute(const char* att, const char* name);
  IWriteFile* File;
  s32 Tabs;
  bool TextWrittenLast;
};
#endif

