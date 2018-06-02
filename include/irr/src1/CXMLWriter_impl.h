// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CXMLWriter.h"
//#include <wchar.h>
//#include "irrString.h"
//! Constructor
CXMLWriter::CXMLWriter(IWriteFile* file)
  : File(file), Tabs(0), TextWrittenLast(false)
{
#ifdef _DEBUG
  setDebugName("CXMLWriter");
#endif
  if (File) {
    File->grab();
  }
}
//! Destructor
CXMLWriter::~CXMLWriter()
{
  if (File) {
    File->drop();
  }
}
//! Writes a xml 1.0 header like <?xml version="1.0"?>
void CXMLWriter::writeXMLHeader()
{
  if (!File) {
    return;
  }
  if (sizeof(char) == 2) {
    const u16 h = 0xFEFF;
    File->write(&h, 2);
  }
  else {
    const u32 h = 0x0000FEFF;
    File->write(&h, sizeof(char));
  }
  const char* const p = "<?xml version=\"1.0\"?>";
  File->write(p, cstr_len(p)*sizeof(char));
  writeLineBreak();
  TextWrittenLast = false;
}
//! Writes an xml element with maximal 5 attributes
void CXMLWriter::writeElement(const char* name, bool empty,
    const char* attr1Name, const char* attr1Value,
    const char* attr2Name, const char* attr2Value,
    const char* attr3Name, const char* attr3Value,
    const char* attr4Name, const char* attr4Value,
    const char* attr5Name, const char* attr5Value)
{
  if (!File || !name) {
    return;
  }
  if (Tabs > 0) {
    int i;
    for (i = 0; i < Tabs; ++i) {
      File->write("\t", sizeof(char));
    }
  }
  // write name
  File->write("<", sizeof(char));
  File->write(name, cstr_len(name)*sizeof(char));
  // write attributes
  writeAttribute(attr1Name, attr1Value);
  writeAttribute(attr2Name, attr2Value);
  writeAttribute(attr3Name, attr3Value);
  writeAttribute(attr4Name, attr4Value);
  writeAttribute(attr5Name, attr5Value);
  // write closing tag
  if (empty) {
    File->write(" />", 3 * sizeof(char));
  }
  else {
    File->write(">", sizeof(char));
    ++Tabs;
  }
  TextWrittenLast = false;
}
//! Writes an xml element with any number of attributes
void CXMLWriter::writeElement(const char* name, bool empty,
    strv_t names,
    strv_t values)
{
  if (!File || !name) {
    return;
  }
  if (Tabs > 0) {
    int i;
    for (i = 0; i < Tabs; ++i) {
      File->write("\t", sizeof(char));
    }
  }
  // write name
  File->write("<", sizeof(char));
  File->write(name, cstr_len(name)*sizeof(char));
  // write attributes
  u32 i = 0;
  for (; i < names_size && i < values_size; ++i) {
    writeAttribute(names[i], values[i]);
  }
  // write closing tag
  if (empty) {
    File->write(" />", 3 * sizeof(char));
  }
  else {
    File->write(">", sizeof(char));
    ++Tabs;
  }
  TextWrittenLast = false;
}
void CXMLWriter::writeAttribute(const char* name, const char* value)
{
  if (!name || !value) {
    return;
  }
  File->write(" ", sizeof(char));
  File->write(name, cstr_len(name)*sizeof(char));
  File->write("=\"", 2 * sizeof(char));
  writeText(value);
  File->write("\"", sizeof(char));
}
//! Writes a comment into the xml file
void CXMLWriter::writeComment(const char* comment)
{
  if (!File || !comment) {
    return;
  }
  File->write("<!--", 4 * sizeof(char));
  writeText(comment);
  File->write("-->", 3 * sizeof(char));
}
//! Writes the closing tag for an element. Like </foo>
void CXMLWriter::writeClosingTag(const char* name)
{
  if (!File || !name) {
    return;
  }
  --Tabs;
  if (Tabs > 0 && !TextWrittenLast) {
    int i;
    for (i = 0; i < Tabs; ++i) {
      File->write("\t", sizeof(char));
    }
  }
  File->write("</", 2 * sizeof(char));
  File->write(name, cstr_len(name)*sizeof(char));
  File->write(">", sizeof(char));
  TextWrittenLast = false;
}
const CXMLWriter::XMLSpecialCharacters XMLWSChar[] = {
  { '&', "&amp;" },
  { '<', "&lt;" },
  { '>', "&gt;" },
  { '"', "&quot;" },
  { '\0', 0 }
};
//! Writes a text into the file. All occurrences of special characters like
//! & (&amp;), < (&lt;), > (&gt;), and " (&quot;) are automaticly replaced.
void CXMLWriter::writeText(const char* text)
{
  if (!File || !text) {
    return;
  }
  // TODO: we have to get rid of that reserve call as well as it slows down xml-writing seriously.
  // Making a member-variable would work, but a lot of memory would stay around after writing.
  // So the correct solution is probably using fixed block here and always write when that is full.
  char* s;
  s.reserve(cstr_len(text) + 1);
  const char* p = text;
  while (*p) {
    // check if it is matching
    bool found = false;
    for (s32 i = 0; XMLWSChar[i].Character != '\0'; ++i)
      if (*p == XMLWSChar[i].Character) {
        s.append(XMLWSChar[i].Symbol);
        found = true;
        break;
      }
    if (!found) {
      s.append(*p);
    }
    ++p;
  }
  // write new string
  File->write(s, s_size * sizeof(char));
  TextWrittenLast = true;
}
//! Writes a line break
void CXMLWriter::writeLineBreak()
{
  if (!File) {
    return;
  }
#if defined(_IRR_OSX_PLATFORM_)
  File->write("\r", sizeof(char));
#elif defined(_IRR_WINDOWS_API_)
  File->write("\r\n", 2 * sizeof(char));
#else
  File->write("\n", sizeof(char));
#endif
}

