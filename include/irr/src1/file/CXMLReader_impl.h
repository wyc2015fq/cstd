// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CXMLReaderImpl.h"
//#include "CXMLReader.h"
//#include "IReadFile.h"
//! Irrlicht implementation of the file read callback for the xml parser
class CIrrXMLFileReadCallBack : public IFileReadCallBack
{
public:
  //! construct from FILE pointer
  CIrrXMLFileReadCallBack(IReadFile* file)
    : ReadFile(file) {
    ReadFile->grab();
  }
  //! destructor
  ~CIrrXMLFileReadCallBack() {
    ReadFile->drop();
  }
  //! Reads an amount of bytes from the file.
  int read(void* buffer, int sizeToRead) {
    return ReadFile->read(buffer, sizeToRead);
  }
  //! Returns size of file in bytes
  long getSize() const {
    return ReadFile->getSize();
  }
private:
  IReadFile* ReadFile;
}; // end class CMyXMLFileReadCallBack
// now create an implementation for IXMLReader using irrXML.
//! Creates an instance of a wide character xml parser.
IXMLReader* createIXMLReader(IReadFile* file)
{
  if (!file) {
    return 0;
  }
  return new CXMLReaderImpl<char, IReferenceCounted>(new CIrrXMLFileReadCallBack(file));
}
//! Creates an instance of an UFT-8 or ASCII character xml parser.
IXMLReaderUTF8* createIXMLReaderUTF8(IReadFile* file)
{
  if (!file) {
    return 0;
  }
  return new CXMLReaderImpl<char, IReferenceCounted>(new CIrrXMLFileReadCallBack(file));
}
// end
// end

