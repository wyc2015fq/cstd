// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_ATTRIBUTES_H_INCLUDED__
#define __I_ATTRIBUTES_H_INCLUDED__
//#include "line2d.h"
//#include "line3d.h"
//#include "triangle3d.h"
//#include "dimension2d.h"
//#include "quaternion.h"
//#include "plane3d.h"
//#include "triangle3d.h"
//#include "line2d.h"
//#include "line3d.h"
//#include "irrString.h"
//#include "irrArray.h"
//#include "IXMLReader.h"
//#include "EAttributes.h"
class ITexture;
class IXMLWriter;
//! Provides a generic interface for attributes and their values and the possiblity to serialize them
class IAttributes
{
public:
  //! Returns amount of attributes in this collection of attributes.
  u32 getAttributeCount() = 0;
  //! Returns attribute name by index.
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  const c8* getAttributeName(s32 index) = 0;
  //! Returns the type of an attribute
  //! \param attributeName: Name for the attribute
  E_ATTRIBUTE_TYPE getAttributeType(const c8* attributeName) = 0;
  //! Returns attribute type by index.
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  E_ATTRIBUTE_TYPE getAttributeType(s32 index) = 0;
  //! Returns the type string of the attribute
  //! \param attributeName: String for the attribute type
  const char* getAttributeTypeString(const c8* attributeName) = 0;
  //! Returns the type string of the attribute by index.
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  const char* getAttributeTypeString(s32 index) = 0;
  //! Returns if an attribute with a name exists
  bool existsAttribute(const c8* attributeName) = 0;
  //! Returns attribute index from name, -1 if not found
  s32 findAttribute(const c8* attributeName) = 0;
  //! Removes all attributes
  void clear() = 0;
  //! Reads attributes from a xml file.
  //! \param reader The XML reader to read from
  //! \param readCurrentElementOnly If set to true, reading only works if current element has the name 'attributes' or
  //! the name specified using elementName.
  //! \param elementName The surrounding element name. If it is null, the default one, "attributes" will be taken.
  //! If set to false, the first appearing list of attributes are read.
  bool read(IXMLReader* reader, bool readCurrentElementOnly = false, const char* elementName = 0) = 0;
  //! Write these attributes into a xml file
  //! \param writer: The XML writer to write to
  //! \param writeXMLHeader: Writes a header to the XML file, required if at the beginning of the file
  //! \param elementName: The surrounding element name. If it is null, the default one, "attributes" will be taken.
  bool write(IXMLWriter* writer, bool writeXMLHeader = false, const char* elementName = 0) = 0;
  /*
    Integer Attribute
  */
  //! Adds an attribute as integer
  void addInt(const c8* attributeName, s32 value) = 0;
  //! Sets an attribute as integer value
  void setAttribute(const c8* attributeName, s32 value) = 0;
  //! Gets an attribute as integer value
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  s32 getAttributeAsInt(const c8* attributeName) = 0;
  //! Gets an attribute as integer value
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  s32 getAttributeAsInt(s32 index) = 0;
  //! Sets an attribute as integer value
  void setAttribute(s32 index, s32 value) = 0;
  /*
    Float Attribute
  */
  //! Adds an attribute as float
  void addFloat(const c8* attributeName, f32 value) = 0;
  //! Sets a attribute as float value
  void setAttribute(const c8* attributeName, f32 value) = 0;
  //! Gets an attribute as float value
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  f32 getAttributeAsFloat(const c8* attributeName) = 0;
  //! Gets an attribute as float value
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  f32 getAttributeAsFloat(s32 index) = 0;
  //! Sets an attribute as float value
  void setAttribute(s32 index, f32 value) = 0;
  /*
    String Attribute
  */
  //! Adds an attribute as string
  void addString(const c8* attributeName, const c8* value) = 0;
  //! Sets an attribute value as string.
  //! \param attributeName: Name for the attribute
  //! \param value: Value for the attribute. Set this to 0 to delete the attribute
  void setAttribute(const c8* attributeName, const c8* value) = 0;
  //! Gets an attribute as string.
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  //! or 0 if attribute is not set.
  char* getAttributeAsString(const c8* attributeName) = 0;
  //! Gets an attribute as string.
  //! \param attributeName Name of the attribute to get.
  //! \param target Buffer where the string is copied to.
  void getAttributeAsString(const c8* attributeName, c8* target) = 0;
  //! Returns attribute value as string by index.
  //! \param index Index value, must be between 0 and getAttributeCount()-1.
  char* getAttributeAsString(s32 index) = 0;
  //! Sets an attribute value as string.
  //! \param index Index value, must be between 0 and getAttributeCount()-1.
  //! \param value String to which the attribute is set.
  void setAttribute(s32 index, const c8* value) = 0;
  /*
    Binary Data Attribute
  */
  //! Adds an attribute as binary data
  void addBinary(const c8* attributeName, void* data, s32 dataSizeInBytes) = 0;
  //! Sets an attribute as binary data
  void setAttribute(const c8* attributeName, void* data, s32 dataSizeInBytes) = 0;
  //! Gets an attribute as binary data
  /** \param attributeName: Name of the attribute to get.
  \param outData Pointer to buffer where data shall be stored.
  \param maxSizeInBytes Maximum number of bytes to write into outData.
  */
  void getAttributeAsBinaryData(const c8* attributeName, void* outData, s32 maxSizeInBytes) = 0;
  //! Gets an attribute as binary data
  /** \param index: Index value, must be between 0 and getAttributeCount()-1.
  \param outData Pointer to buffer where data shall be stored.
  \param maxSizeInBytes Maximum number of bytes to write into outData.
  */
  void getAttributeAsBinaryData(s32 index, void* outData, s32 maxSizeInBytes) = 0;
  //! Sets an attribute as binary data
  void setAttribute(s32 index, void* data, s32 dataSizeInBytes) = 0;
  /*
    Array Attribute
  */
  //! Adds an attribute as wide string array
  void addArray(const c8* attributeName, const strv_t value) = 0;
  //! Sets an attribute value as a wide string array.
  //! \param attributeName: Name for the attribute
  //! \param value: Value for the attribute. Set this to 0 to delete the attribute
  void setAttribute(const c8* attributeName, const strv_t value) = 0;
  //! Gets an attribute as an array of wide strings.
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  //! or 0 if attribute is not set.
  strv_t getAttributeAsArray(const c8* attributeName) = 0;
  //! Returns attribute value as an array of wide strings by index.
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  strv_t getAttributeAsArray(s32 index) = 0;
  //! Sets an attribute as an array of wide strings
  void setAttribute(s32 index, const strv_t value) = 0;
  /*
    Bool Attribute
  */
  //! Adds an attribute as bool
  void addBool(const c8* attributeName, bool value) = 0;
  //! Sets an attribute as boolean value
  void setAttribute(const c8* attributeName, bool value) = 0;
  //! Gets an attribute as boolean value
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  bool getAttributeAsBool(const c8* attributeName) = 0;
  //! Gets an attribute as boolean value
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  bool getAttributeAsBool(s32 index) = 0;
  //! Sets an attribute as boolean value
  void setAttribute(s32 index, bool value) = 0;
  /*
    Enumeration Attribute
  */
  //! Adds an attribute as enum
  void addEnum(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals) = 0;
  //! Adds an attribute as enum
  void addEnum(const c8* attributeName, s32 enumValue, const c8* const* enumerationLiterals) = 0;
  //! Sets an attribute as enumeration
  void setAttribute(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals) = 0;
  //! Gets an attribute as enumeration
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  const c8* getAttributeAsEnumeration(const c8* attributeName) = 0;
  //! Gets an attribute as enumeration
  /** \param attributeName: Name of the attribute to get.
  \param enumerationLiteralsToUse: Use these enumeration literals to get
  the index value instead of the set ones. This is useful when the
  attribute list maybe was read from an xml file, and only contains the
  enumeration string, but no information about its index.
  \return Returns value of the attribute previously set by setAttribute()
  */
  s32 getAttributeAsEnumeration(const c8* attributeName, const c8* const* enumerationLiteralsToUse) = 0;
  //! Gets an attribute as enumeration
  /** \param index: Index value, must be between 0 and getAttributeCount()-1.
  \param enumerationLiteralsToUse: Use these enumeration literals to get
  the index value instead of the set ones. This is useful when the
  attribute list maybe was read from an xml file, and only contains the
  enumeration string, but no information about its index.
  \return Returns value of the attribute previously set by setAttribute()
  */
  s32 getAttributeAsEnumeration(s32 index, const c8* const* enumerationLiteralsToUse) = 0;
  //! Gets an attribute as enumeration
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  const c8* getAttributeAsEnumeration(s32 index) = 0;
  //! Gets the list of enumeration literals of an enumeration attribute
  //! \param attributeName Name of the attribute to get.
  //! \param outLiterals Set of strings to choose the enum name from.
  void getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName, strv_t& outLiterals) = 0;
  //! Gets the list of enumeration literals of an enumeration attribute
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  //! \param outLiterals Set of strings to choose the enum name from.
  void getAttributeEnumerationLiteralsOfEnumeration(s32 index, strv_t& outLiterals) = 0;
  //! Sets an attribute as enumeration
  void setAttribute(s32 index, const c8* enumValue, const c8* const* enumerationLiterals) = 0;
  /*
    SColor Attribute
  */
  //! Adds an attribute as color
  void addColor(const c8* attributeName, SColor value) = 0;
  //! Sets a attribute as color
  void setAttribute(const c8* attributeName, SColor color) = 0;
  //! Gets an attribute as color
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  SColor getAttributeAsColor(const c8* attributeName) = 0;
  //! Gets an attribute as color
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  SColor getAttributeAsColor(s32 index) = 0;
  //! Sets an attribute as color
  void setAttribute(s32 index, SColor color) = 0;
  /*
    SColorf Attribute
  */
  //! Adds an attribute as floating point color
  void addColorf(const c8* attributeName, SColorf value) = 0;
  //! Sets a attribute as floating point color
  void setAttribute(const c8* attributeName, SColorf color) = 0;
  //! Gets an attribute as floating point color
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  SColorf getAttributeAsColorf(const c8* attributeName) = 0;
  //! Gets an attribute as floating point color
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  SColorf getAttributeAsColorf(s32 index) = 0;
  //! Sets an attribute as floating point color
  void setAttribute(s32 index, SColorf color) = 0;
  /*
    Vector3d Attribute
  */
  //! Adds an attribute as 3d vector
  void addVector3d(const c8* attributeName, float3 value) = 0;
  //! Sets a attribute as 3d vector
  void setAttribute(const c8* attributeName, float3 v) = 0;
  //! Gets an attribute as 3d vector
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  float3 getAttributeAsVector3d(const c8* attributeName) = 0;
  //! Gets an attribute as 3d vector
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  float3 getAttributeAsVector3d(s32 index) = 0;
  //! Sets an attribute as vector
  void setAttribute(s32 index, float3 v) = 0;
  /*
    Vector2d Attribute
  */
  //! Adds an attribute as 2d vector
  void addVector2d(const c8* attributeName, vector2df value) = 0;
  //! Sets a attribute as 2d vector
  void setAttribute(const c8* attributeName, vector2df v) = 0;
  //! Gets an attribute as vector
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  vector2df getAttributeAsVector2d(const c8* attributeName) = 0;
  //! Gets an attribute as position
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  vector2df getAttributeAsVector2d(s32 index) = 0;
  //! Sets an attribute as 2d vector
  void setAttribute(s32 index, vector2df v) = 0;
  /*
    Position2d Attribute
  */
  //! Adds an attribute as 2d position
  void addPosition2d(const c8* attributeName, position2di value) = 0;
  //! Sets a attribute as 2d position
  void setAttribute(const c8* attributeName, position2di v) = 0;
  //! Gets an attribute as position
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  position2di getAttributeAsPosition2d(const c8* attributeName) = 0;
  //! Gets an attribute as position
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  position2di getAttributeAsPosition2d(s32 index) = 0;
  //! Sets an attribute as 2d position
  void setAttribute(s32 index, position2di v) = 0;
  /*
    Rectangle Attribute
  */
  //! Adds an attribute as rectangle
  void addRect(const c8* attributeName, IRECT value) = 0;
  //! Sets an attribute as rectangle
  void setAttribute(const c8* attributeName, IRECT v) = 0;
  //! Gets an attribute as rectangle
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  IRECT getAttributeAsRect(const c8* attributeName) = 0;
  //! Gets an attribute as rectangle
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  IRECT getAttributeAsRect(s32 index) = 0;
  //! Sets an attribute as rectangle
  void setAttribute(s32 index, IRECT v) = 0;
  /*
    Dimension2d Attribute
  */
  //! Adds an attribute as dimension2d
  void addDimension2d(const c8* attributeName, dimension2du value) = 0;
  //! Sets an attribute as dimension2d
  void setAttribute(const c8* attributeName, dimension2du v) = 0;
  //! Gets an attribute as dimension2d
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  dimension2du getAttributeAsDimension2d(const c8* attributeName) = 0;
  //! Gets an attribute as dimension2d
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  dimension2du getAttributeAsDimension2d(s32 index) = 0;
  //! Sets an attribute as dimension2d
  void setAttribute(s32 index, dimension2du v) = 0;
  /*
    matrix attribute
  */
  //! Adds an attribute as matrix
  void addMatrix(const c8* attributeName, const matrix4& v) = 0;
  //! Sets an attribute as matrix
  void setAttribute(const c8* attributeName, const matrix4& v) = 0;
  //! Gets an attribute as a matrix4
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  matrix4 getAttributeAsMatrix(const c8* attributeName) = 0;
  //! Gets an attribute as matrix
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  matrix4 getAttributeAsMatrix(s32 index) = 0;
  //! Sets an attribute as matrix
  void setAttribute(s32 index, const matrix4& v) = 0;
  /*
    quaternion attribute
  */
  //! Adds an attribute as quaternion
  void addQuaternion(const c8* attributeName, quaternion v) = 0;
  //! Sets an attribute as quaternion
  void setAttribute(const c8* attributeName, quaternion v) = 0;
  //! Gets an attribute as a quaternion
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  quaternion getAttributeAsQuaternion(const c8* attributeName) = 0;
  //! Gets an attribute as quaternion
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  quaternion getAttributeAsQuaternion(s32 index) = 0;
  //! Sets an attribute as quaternion
  void setAttribute(s32 index, quaternion v) = 0;
  /*
    3d bounding box
  */
  //! Adds an attribute as axis aligned bounding box
  void addBox3d(const c8* attributeName, aabbox3df v) = 0;
  //! Sets an attribute as axis aligned bounding box
  void setAttribute(const c8* attributeName, aabbox3df v) = 0;
  //! Gets an attribute as a axis aligned bounding box
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  aabbox3df getAttributeAsBox3d(const c8* attributeName) = 0;
  //! Gets an attribute as axis aligned bounding box
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  aabbox3df getAttributeAsBox3d(s32 index) = 0;
  //! Sets an attribute as axis aligned bounding box
  void setAttribute(s32 index, aabbox3df v) = 0;
  /*
    plane
  */
  //! Adds an attribute as 3d plane
  void addPlane3d(const c8* attributeName, plane3df v) = 0;
  //! Sets an attribute as 3d plane
  void setAttribute(const c8* attributeName, plane3df v) = 0;
  //! Gets an attribute as a 3d plane
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  plane3df getAttributeAsPlane3d(const c8* attributeName) = 0;
  //! Gets an attribute as 3d plane
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  plane3df getAttributeAsPlane3d(s32 index) = 0;
  //! Sets an attribute as 3d plane
  void setAttribute(s32 index, plane3df v) = 0;
  /*
    3d triangle
  */
  //! Adds an attribute as 3d triangle
  void addTriangle3d(const c8* attributeName, triangle3df v) = 0;
  //! Sets an attribute as 3d trianle
  void setAttribute(const c8* attributeName, triangle3df v) = 0;
  //! Gets an attribute as a 3d triangle
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  triangle3df getAttributeAsTriangle3d(const c8* attributeName) = 0;
  //! Gets an attribute as 3d triangle
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  triangle3df getAttributeAsTriangle3d(s32 index) = 0;
  //! Sets an attribute as 3d triangle
  void setAttribute(s32 index, triangle3df v) = 0;
  /*
    line 2d
  */
  //! Adds an attribute as a 2d line
  void addLine2d(const c8* attributeName, line2df v) = 0;
  //! Sets an attribute as a 2d line
  void setAttribute(const c8* attributeName, line2df v) = 0;
  //! Gets an attribute as a 2d line
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  line2df getAttributeAsLine2d(const c8* attributeName) = 0;
  //! Gets an attribute as a 2d line
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  line2df getAttributeAsLine2d(s32 index) = 0;
  //! Sets an attribute as a 2d line
  void setAttribute(s32 index, line2df v) = 0;
  /*
    line 3d
  */
  //! Adds an attribute as a 3d line
  void addLine3d(const c8* attributeName, line3df v) = 0;
  //! Sets an attribute as a 3d line
  void setAttribute(const c8* attributeName, line3df v) = 0;
  //! Gets an attribute as a 3d line
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  line3df getAttributeAsLine3d(const c8* attributeName) = 0;
  //! Gets an attribute as a 3d line
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  line3df getAttributeAsLine3d(s32 index) = 0;
  //! Sets an attribute as a 3d line
  void setAttribute(s32 index, line3df v) = 0;
  /*
    Texture Attribute
  */
  //! Adds an attribute as texture reference
  void addTexture(const c8* attributeName, ITexture* texture, const char* filename = "") = 0;
  //! Sets an attribute as texture reference
  void setAttribute(const c8* attributeName, ITexture* texture, const char* filename = "") = 0;
  //! Gets an attribute as texture reference
  //! \param attributeName: Name of the attribute to get.
  ITexture* getAttributeAsTexture(const c8* attributeName) = 0;
  //! Gets an attribute as texture reference
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  ITexture* getAttributeAsTexture(s32 index) = 0;
  //! Sets an attribute as texture reference
  void setAttribute(s32 index, ITexture* texture, const char* filename = "") = 0;
  /*
    User Pointer Attribute
  */
  //! Adds an attribute as user pointner
  void addUserPointer(const c8* attributeName, void* userPointer) = 0;
  //! Sets an attribute as user pointer
  void setAttribute(const c8* attributeName, void* userPointer) = 0;
  //! Gets an attribute as user pointer
  //! \param attributeName: Name of the attribute to get.
  void* getAttributeAsUserPointer(const c8* attributeName) = 0;
  //! Gets an attribute as user pointer
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  void* getAttributeAsUserPointer(s32 index) = 0;
  //! Sets an attribute as user pointer
  void setAttribute(s32 index, void* userPointer) = 0;
};
#endif

