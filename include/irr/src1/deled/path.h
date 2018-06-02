// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine" and the "irrXML" project.
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __IRR_PATH_H_INCLUDED__
#define __IRR_PATH_H_INCLUDED__
//#include "irrString.h"


//! Type used for all file system related strings.
/** This type will transparently handle different file system encodings. */
//typedef string<fschar_t> path;
//! Used in places where we identify objects by a filename, but don't actually work with the real filename
/** Irrlicht is internally not case-sensitive when it comes to names.
    Also this class is a first step towards support for correctly serializing renamed objects.
*/
#if 0
struct SNamedPath {
  //! Constructor
  SNamedPath() {}
  //! Constructor
  SNamedPath(const char* p) : Path(p), InternalName(PathToName(p)) {
  }
  //! Is smaller comparator
  bool operator <(const SNamedPath& other) const {
    return InternalName < other.InternalName;
  }
  //! Set the path.
  void setPath(const char* p) {
    Path = p;
    InternalName = PathToName(p);
  }
  //! Get the path.
  const char* getPath() const {
    return Path;
  };
  //! Get the name which is used to identify the file.
  //! This string is similar to the names and filenames used before Irrlicht 1.7
  const char* getInternalName() const {
    return InternalName;
  }
  //! Implicit cast to char*
  operator char*() const {
    return char*(getPath());
  }
protected:
  // convert the given path string to a name string.
  int PathToName(const char* p, char* name, int len) const {
    strncpy(name, p, len);
    name.replace('\\', '/');
    name.make_lower();
    return name;
  }
private:
  path Path;
  path InternalName;
};
#else
#endif

#endif // __IRR_PATH_H_INCLUDED__

