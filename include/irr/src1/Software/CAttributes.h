// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_ATTRIBUTES_H_INCLUDED__
#define __C_ATTRIBUTES_H_INCLUDED__
//#include "IAttributes.h"
//#include "IAttribute.h"
class ITexture;
class IVideoDriver;
#include "CAttributes_impl.h"
//! Implementation of the IAttributes interface
class CAttributes : public IAttributes
{
public:
  CAttributes(IVideoDriver* driver = 0);
  ~CAttributes();
  //! Returns amount of attributes in this collection of attributes.
  u32 getAttributeCount() const;
  //! Returns attribute name by index.
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  const c8* getAttributeName(s32 index);
  //! Returns the type of an attribute
  //! \param attributeName: Name for the attribute
  E_ATTRIBUTE_TYPE getAttributeType(const c8* attributeName);
  //! Returns attribute type by index.
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  E_ATTRIBUTE_TYPE getAttributeType(s32 index);
  //! Returns the type string of the attribute
  //! \param attributeName: String for the attribute type
  const char* getAttributeTypeString(const c8* attributeName);
  //! Returns the type string of the attribute by index.
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  const char* getAttributeTypeString(s32 index);
  //! Returns if an attribute with a name exists
  bool existsAttribute(const c8* attributeName);
  //! Returns attribute index from name, -1 if not found
  s32 findAttribute(const c8* attributeName) const;
  //! Removes all attributes
  void clear();
  //! Reads attributes from a xml file.
  //! \param readCurrentElementOnly: If set to true, reading only works if current element has the name 'attributes'.
  //! IF set to false, the first appearing list attributes are read.
  bool read(IXMLReader* reader, bool readCurrentElementOnly = false,
      const char* nonDefaultElementName = 0);
  //! Write these attributes into a xml file
  bool write(IXMLWriter* writer, bool writeXMLHeader = false, const char* nonDefaultElementName = 0);
  /*
    Integer Attribute
  */
  //! Adds an attribute as integer
  void addInt(const c8* attributeName, s32 value);
  //! Sets an attribute as integer value
  void setAttribute(const c8* attributeName, s32 value);
  //! Gets an attribute as integer value
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  s32 getAttributeAsInt(const c8* attributeName) const;
  //! Gets an attribute as integer value
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  s32 getAttributeAsInt(s32 index) const;
  //! Sets an attribute as integer value
  void setAttribute(s32 index, s32 value);
  /*
    Float Attribute
  */
  //! Adds an attribute as float
  void addFloat(const c8* attributeName, f32 value);
  //! Sets a attribute as float value
  void setAttribute(const c8* attributeName, f32 value);
  //! Gets an attribute as float value
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  f32 getAttributeAsFloat(const c8* attributeName);
  //! Gets an attribute as float value
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  f32 getAttributeAsFloat(s32 index);
  //! Sets an attribute as float value
  void setAttribute(s32 index, f32 value);
  /*
    String Attribute
  */
  //! Adds an attribute as string
  void addString(const c8* attributeName, const c8* value);
  //! Sets an attribute value as string.
  //! \param attributeName: Name for the attribute
  //! \param value: Value for the attribute. Set this to 0 to delete the attribute
  void setAttribute(const c8* attributeName, const c8* value);
  //! Gets an attribute as string.
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  //! or 0 if attribute is not set.
  char* getAttributeAsString(const c8* attributeName);
  //! Gets an attribute as string.
  //! \param attributeName: Name of the attribute to get.
  //! \param target: Buffer where the string is copied to.
  void getAttributeAsString(const c8* attributeName, c8* target);
  //! Returns attribute value as string by index.
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  char* getAttributeAsString(s32 index);
  //! Sets an attribute value as string.
  //! \param attributeName: Name for the attribute
  void setAttribute(s32 index, const c8* value);
  //! Sets an attribute value as string.
  //! \param attributeName: Name for the attribute
  /*
    Binary Data Attribute
  */
  //! Adds an attribute as binary data
  void addBinary(const c8* attributeName, void* data, s32 dataSizeInBytes);
  //! Sets an attribute as binary data
  void setAttribute(const c8* attributeName, void* data, s32 dataSizeInBytes);
  //! Gets an attribute as binary data
  //! \param attributeName: Name of the attribute to get.
  void getAttributeAsBinaryData(const c8* attributeName, void* outData, s32 maxSizeInBytes);
  //! Gets an attribute as binary data
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  void getAttributeAsBinaryData(s32 index, void* outData, s32 maxSizeInBytes);
  //! Sets an attribute as binary data
  void setAttribute(s32 index, void* data, s32 dataSizeInBytes);
  /*
    Array Attribute
  */
  //! Adds an attribute as wide string array
  void addArray(const c8* attributeName, const strv_t value);
  //! Sets an attribute value as a wide string array.
  //! \param attributeName: Name for the attribute
  //! \param value: Value for the attribute. Set this to 0 to delete the attribute
  void setAttribute(const c8* attributeName, const strv_t value);
  //! Gets an attribute as an array of wide strings.
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  //! or 0 if attribute is not set.
  strv_t getAttributeAsArray(const c8* attributeName);
  //! Returns attribute value as an array of wide strings by index.
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  strv_t getAttributeAsArray(s32 index);
  //! Sets an attribute as an array of wide strings
  void setAttribute(s32 index, const strv_t value);
  /*
    Bool Attribute
  */
  //! Adds an attribute as bool
  void addBool(const c8* attributeName, bool value);
  //! Sets an attribute as boolean value
  void setAttribute(const c8* attributeName, bool value);
  //! Gets an attribute as boolean value
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  bool getAttributeAsBool(const c8* attributeName);
  //! Gets an attribute as boolean value
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  bool getAttributeAsBool(s32 index);
  //! Sets an attribute as boolean value
  void setAttribute(s32 index, bool value);
  /*
    Enumeration Attribute
  */
  //! Adds an attribute as enum
  void addEnum(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals);
  //! Adds an attribute as enum
  void addEnum(const c8* attributeName, s32 enumValue, const c8* const* enumerationLiterals);
  //! Sets an attribute as enumeration
  void setAttribute(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals);
  //! Gets an attribute as enumeration
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  const c8* getAttributeAsEnumeration(const c8* attributeName);
  //! Gets an attribute as enumeration
  //! \param attributeName: Name of the attribute to get.
  //! \param enumerationLiteralsToUse: Use these enumeration literals to get the index value instead of the set ones.
  //! This is useful when the attribute list maybe was read from an xml file, and only contains the enumeration string, but
  //! no information about its index.
  //! \return Returns value of the attribute previously set by setAttribute()
  s32 getAttributeAsEnumeration(const c8* attributeName, const c8* const* enumerationLiteralsToUse);
  //! Gets an attribute as enumeration
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  s32 getAttributeAsEnumeration(s32 index, const c8* const* enumerationLiteralsToUse);
  //! Gets an attribute as enumeration
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  const c8* getAttributeAsEnumeration(s32 index);
  //! Gets the list of enumeration literals of an enumeration attribute
  //! \param attributeName: Name of the attribute to get.
  void getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName, strv_t& outLiterals);
  //! Gets the list of enumeration literals of an enumeration attribute
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  void getAttributeEnumerationLiteralsOfEnumeration(s32 index, strv_t& outLiterals);
  //! Sets an attribute as enumeration
  void setAttribute(s32 index, const c8* enumValue, const c8* const* enumerationLiterals);
  /*
    SColor Attribute
  */
  //! Adds an attribute as color
  void addColor(const c8* attributeName, SColor value);
  //! Sets a attribute as color
  void setAttribute(const c8* attributeName, SColor color);
  //! Gets an attribute as color
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  SColor getAttributeAsColor(const c8* attributeName);
  //! Gets an attribute as color
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  SColor getAttributeAsColor(s32 index);
  //! Sets an attribute as color
  void setAttribute(s32 index, SColor color);
  /*
    SColorf Attribute
  */
  //! Adds an attribute as floating point color
  void addColorf(const c8* attributeName, SColorf value);
  //! Sets a attribute as floating point color
  void setAttribute(const c8* attributeName, SColorf color);
  //! Gets an attribute as floating point color
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  SColorf getAttributeAsColorf(const c8* attributeName);
  //! Gets an attribute as floating point color
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  SColorf getAttributeAsColorf(s32 index);
  //! Sets an attribute as floating point color
  void setAttribute(s32 index, SColorf color);
  /*
    Vector3d Attribute
  */
  //! Adds an attribute as 3d vector
  void addVector3d(const c8* attributeName, float3 value);
  //! Sets a attribute as 3d vector
  void setAttribute(const c8* attributeName, float3 v);
  //! Gets an attribute as 3d vector
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  float3 getAttributeAsVector3d(const c8* attributeName);
  //! Gets an attribute as 3d vector
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  float3 getAttributeAsVector3d(s32 index);
  //! Sets an attribute as vector
  void setAttribute(s32 index, float3 v);
  /*
    Vector2d Attribute
  */
  //! Adds an attribute as 2d vector
  void addVector2d(const c8* attributeName, vector2df value);
  //! Sets a attribute as 2d vector
  void setAttribute(const c8* attributeName, vector2df v);
  //! Gets an attribute as 2d vector
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  vector2df getAttributeAsVector2d(const c8* attributeName);
  //! Gets an attribute as 3d vector
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  vector2df getAttributeAsVector2d(s32 index);
  //! Sets an attribute as vector
  void setAttribute(s32 index, vector2df v);
  /*
    Position2d Attribute
  */
  //! Adds an attribute as 2d position
  void addPosition2d(const c8* attributeName, position2di value);
  //! Sets a attribute as 2d position
  void setAttribute(const c8* attributeName, position2di v);
  //! Gets an attribute as position
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  position2di getAttributeAsPosition2d(const c8* attributeName);
  //! Gets an attribute as position
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  position2di getAttributeAsPosition2d(s32 index);
  //! Sets an attribute as 2d position
  void setAttribute(s32 index, position2di v);
  /*
    Rectangle Attribute
  */
  //! Adds an attribute as rectangle
  void addRect(const c8* attributeName, IRECT value);
  //! Sets an attribute as rectangle
  void setAttribute(const c8* attributeName, IRECT v);
  //! Gets an attribute as rectangle
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  IRECT getAttributeAsRect(const c8* attributeName);
  //! Gets an attribute as rectangle
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  IRECT getAttributeAsRect(s32 index);
  //! Sets an attribute as rectangle
  void setAttribute(s32 index, IRECT v);
  /*
    Dimension2d Attribute
  */
  //! Adds an attribute as dimension2d
  void addDimension2d(const c8* attributeName, dimension2du value);
  //! Sets an attribute as dimension2d
  void setAttribute(const c8* attributeName, dimension2du v);
  //! Gets an attribute as dimension2d
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  dimension2du getAttributeAsDimension2d(const c8* attributeName);
  //! Gets an attribute as dimension2d
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  dimension2du getAttributeAsDimension2d(s32 index);
  //! Sets an attribute as dimension2d
  void setAttribute(s32 index, dimension2du v);
  /*
    matrix attribute
  */
  //! Adds an attribute as matrix
  void addMatrix(const c8* attributeName, const matrix4& v);
  //! Sets an attribute as matrix
  void setAttribute(const c8* attributeName, const matrix4& v);
  //! Gets an attribute as a matrix4
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  matrix4 getAttributeAsMatrix(const c8* attributeName);
  //! Gets an attribute as matrix
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  matrix4 getAttributeAsMatrix(s32 index);
  //! Sets an attribute as matrix
  void setAttribute(s32 index, const matrix4& v);
  /*
    quaternion attribute
  */
  //! Adds an attribute as quaternion
  void addQuaternion(const c8* attributeName, quaternion v);
  //! Sets an attribute as quaternion
  void setAttribute(const c8* attributeName, quaternion v);
  //! Gets an attribute as a quaternion
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  quaternion getAttributeAsQuaternion(const c8* attributeName);
  //! Gets an attribute as quaternion
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  quaternion getAttributeAsQuaternion(s32 index);
  //! Sets an attribute as quaternion
  void setAttribute(s32 index, quaternion v);
  /*
    3d bounding box
  */
  //! Adds an attribute as axis aligned bounding box
  void addBox3d(const c8* attributeName, aabbox3df v);
  //! Sets an attribute as axis aligned bounding box
  void setAttribute(const c8* attributeName, aabbox3df v);
  //! Gets an attribute as a axis aligned bounding box
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  aabbox3df getAttributeAsBox3d(const c8* attributeName);
  //! Gets an attribute as axis aligned bounding box
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  aabbox3df getAttributeAsBox3d(s32 index);
  //! Sets an attribute as axis aligned bounding box
  void setAttribute(s32 index, aabbox3df v);
  /*
    plane
  */
  //! Adds an attribute as 3d plane
  void addPlane3d(const c8* attributeName, plane3df v);
  //! Sets an attribute as 3d plane
  void setAttribute(const c8* attributeName, plane3df v);
  //! Gets an attribute as a 3d plane
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  plane3df getAttributeAsPlane3d(const c8* attributeName);
  //! Gets an attribute as 3d plane
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  plane3df getAttributeAsPlane3d(s32 index);
  //! Sets an attribute as 3d plane
  void setAttribute(s32 index, plane3df v);
  /*
    3d triangle
  */
  //! Adds an attribute as 3d triangle
  void addTriangle3d(const c8* attributeName, triangle3df v);
  //! Sets an attribute as 3d trianle
  void setAttribute(const c8* attributeName, triangle3df v);
  //! Gets an attribute as a 3d triangle
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  triangle3df getAttributeAsTriangle3d(const c8* attributeName);
  //! Gets an attribute as 3d triangle
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  triangle3df getAttributeAsTriangle3d(s32 index);
  //! Sets an attribute as 3d triangle
  void setAttribute(s32 index, triangle3df v);
  /*
    line 2d
  */
  //! Adds an attribute as a 2d line
  void addLine2d(const c8* attributeName, line2df v);
  //! Sets an attribute as a 2d line
  void setAttribute(const c8* attributeName, line2df v);
  //! Gets an attribute as a 2d line
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  line2df getAttributeAsLine2d(const c8* attributeName);
  //! Gets an attribute as a 2d line
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  line2df getAttributeAsLine2d(s32 index);
  //! Sets an attribute as a 2d line
  void setAttribute(s32 index, line2df v);
  /*
    line 3d
  */
  //! Adds an attribute as a 3d line
  void addLine3d(const c8* attributeName, line3df v);
  //! Sets an attribute as a 3d line
  void setAttribute(const c8* attributeName, line3df v);
  //! Gets an attribute as a 3d line
  //! \param attributeName: Name of the attribute to get.
  //! \return Returns value of the attribute previously set by setAttribute()
  line3df getAttributeAsLine3d(const c8* attributeName);
  //! Gets an attribute as a 3d line
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  line3df getAttributeAsLine3d(s32 index);
  //! Sets an attribute as a 3d line
  void setAttribute(s32 index, line3df v);
  /*
    Texture Attribute
  */
  //! Adds an attribute as texture reference
  void addTexture(const c8* attributeName, ITexture* texture, const char* filename = "");
  //! Sets an attribute as texture reference
  void setAttribute(const c8* attributeName, ITexture* texture, const char* filename = "");
  //! Gets an attribute as texture reference
  //! \param attributeName: Name of the attribute to get.
  ITexture* getAttributeAsTexture(const c8* attributeName);
  //! Gets an attribute as texture reference
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  ITexture* getAttributeAsTexture(s32 index);
  //! Sets an attribute as texture reference
  void setAttribute(s32 index, ITexture* texture, const char* filename = "");
  /*
    User Pointer Attribute
  */
  //! Adds an attribute as user pointner
  void addUserPointer(const c8* attributeName, void* userPointer);
  //! Sets an attribute as user pointer
  void setAttribute(const c8* attributeName, void* userPointer);
  //! Gets an attribute as user pointer
  //! \param attributeName: Name of the attribute to get.
  void* getAttributeAsUserPointer(const c8* attributeName);
  //! Gets an attribute as user pointer
  //! \param index: Index value, must be between 0 and getAttributeCount()-1.
  void* getAttributeAsUserPointer(s32 index);
  //! Sets an attribute as user pointer
  void setAttribute(s32 index, void* userPointer);
protected:
  void readAttributeFromXML(IXMLReader* reader);
  IAttribute* > Attributes;
  IAttribute* getAttributeP(const c8* attributeName) const;
  IVideoDriver* Driver;
};
#endif

