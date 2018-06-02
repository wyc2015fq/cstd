// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_ATTRIBUTE_H_INCLUDED__
#define __I_ATTRIBUTE_H_INCLUDED__
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
//#include "EAttributes.h"
class IAttribute
{
public:
  ~IAttribute() {};
  s32 getInt()        {
    return 0;
  }
  f32 getFloat()        {
    return 0;
  }
  SColorf getColorf()    {
    return SColorf(1.0f, 1.0f, 1.0f, 1.0f);
  }
  SColor getColor()    {
    return SColor(255, 255, 255, 255);
  }
  char* getString()   {
    return char * (getString());
  }
  strv_t getArray() {
    return strv_t();
  };
  bool getBool()        {
    return false;
  }
  void getBinary(void* outdata, s32 maxLength) {};
  float3 getVector()   {
    return float3();
  }
  position2di getPosition() {
    return position2di();
  }
  IRECT getRect()   {
    return IRECT();
  }
  quaternion getQuaternion() {
    return quaternion();
  }
  matrix4 getMatrix()   {
    return matrix4();
  }
  triangle3df getTriangle()   {
    return triangle3df();
  }
  vector2df getVector2d()   {
    return vector2df();
  }
  vector2di getVector2di()    {
    return vector2di();
  }
  line2df getLine2d()   {
    return line2df();
  }
  line2di getLine2di()    {
    return line2di();
  }
  line3df getLine3d()   {
    return line3df();
  }
  line3di getLine3di()    {
    return line3di();
  }
  dimension2du getDimension2d() {
    return dimension2du();
  }
  F3AABBOX getBBox()   {
    return F3AABBOX();
  }
  plane3df getPlane()   {
    return plane3df();
  }
  ITexture* getTexture()   {
    return 0;
  }
  const char* getEnum()     {
    return 0;
  }
  void* getUserPointer()      {
    return 0;
  }
  void setInt(s32 intValue)   {};
  void setFloat(f32 floatValue)   {};
  void setString(const char* text)  {};
  void setArray(const strv_t arr) {};
  void setColor(SColorf color) {};
  void setColor(SColor color)  {};
  void setBool(bool boolValue)    {};
  void setBinary(void* data, s32 maxLenght) {};
  void setVector(float3 v) {};
  void setPosition(position2di v) {};
  void setRect(IRECT v)   {};
  void setQuaternion(quaternion v) {};
  void setMatrix(matrix4 v) {};
  void setTriangle(triangle3df v) {};
  void setVector2d(vector2df v) {};
  void setVector2d(vector2di v) {};
  void setLine2d(line2df v) {};
  void setLine2d(line2di v) {};
  void setLine3d(line3df v) {};
  void setLine3d(line3di v) {};
  void setDimension2d(dimension2du v) {};
  void setBBox(F3AABBOX v) {};
  void setPlane(plane3df v) {};
  void setUserPointer(void* v)  {};
  void setEnum(const char* enumValue, const char* const* enumerationLiterals) {};
  void setTexture(ITexture*, const char* filename) {};
  char* Name;
  E_ATTRIBUTE_TYPE getType() = 0;
  const char* getTypeString() = 0;
};
#endif

