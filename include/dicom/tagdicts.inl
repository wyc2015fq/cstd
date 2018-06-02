
// GENERATED FILE DO NOT EDIT
// $ xsltproc DefaultDicts.xsl Part6.xml > gdcmDefaultDicts.cxx

/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library

  Copyright (c) 2006-2011 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef GDCMDEFAULTDICTS_CXX
#define GDCMDEFAULTDICTS_CXX

#include "gdcmDicts.h"
#include "gdcmVR.h"
#include "gdcmDict.h"
#include "gdcmDictEntry.h"

namespace gdcm
{
typedef struct {
  uint16_t group;
  uint16_t element;
  VR::VRType vr;
  VM::VMType vm;
  const char* name;
  const char* keyword;
  bool ret;
} DICT_ENTRY;

static const DICT_ENTRY DICOMV3DataDict [] = ;

void Dict::LoadDefault()
{
  unsigned int i = 0;
  DICT_ENTRY n = DICOMV3DataDict[i];

  while (n.name != 0) {
    Tag t(n.group, n.element);
    DictEntry e(n.name, n.keyword, n.vr, n.vm, n.ret);
    assert(DictEntry::CheckKeywordAgainstName(n.name, n.keyword));
    AddDictEntry(t, e);
    n = DICOMV3DataDict[++i];
  }
}

/*
void PrivateDict::LoadDefault()
{
  // TODO
}
*/

} // end namespace gdcm
#endif // GDCMDEFAULTDICTS_CXX
