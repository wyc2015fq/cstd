// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CAttributes.h"
//#include "ITexture.h"
//#include "IVideoDriver.h"
/*
  basic types
*/
// Attribute implemented for boolean values
class CBoolAttribute : public IAttribute
{
public:
  CBoolAttribute(const char* name, bool value) {
    Name = name;
    setBool(value);
  }
  s32 getInt() {
    return BoolValue ? 1 : 0;
  }
  f32 getFloat() {
    return BoolValue ? 1.0f : 0.0f;
  }
  bool getBool() {
    return BoolValue;
  }
  char* getString() {
    return char * (BoolValue ? "true" : "false");
  }
  void setInt(s32 intValue) {
    BoolValue = (intValue != 0);
  }
  void setFloat(f32 floatValue) {
    BoolValue = (floatValue != 0);
  }
  void setBool(bool boolValue) {
    BoolValue = boolValue;
  }
  void setString(const char* string) {
    BoolValue = strcmp(string, "true") == 0;
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_BOOL;
  }
  const char* getTypeString() const {
    return "bool";
  }
  bool BoolValue;
};
// Attribute implemented for integers
class CIntAttribute : public IAttribute
{
public:
  CIntAttribute(const char* name, s32 value) {
    Name = name;
    setInt(value);
  }
  s32 getInt() {
    return Value;
  }
  f32 getFloat() {
    return (f32)Value;
  }
  bool getBool() {
    return (Value != 0);
  }
  char* getString() {
    return char * (Value);
  }
  void setInt(s32 intValue) {
    Value = intValue;
  }
  void setFloat(f32 floatValue) {
    Value = (s32)floatValue;
  };
  void setString(const char* text) {
    Value = atoi(text);
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_INT;
  }
  const char* getTypeString() const {
    return "int";
  }
  s32 Value;
};
// Attribute implemented for floats
class CFloatAttribute : public IAttribute
{
public:
  CFloatAttribute(const char* name, f32 value) {
    Name = name;
    setFloat(value);
  }
  s32 getInt() {
    return (s32)Value;
  }
  f32 getFloat() {
    return Value;
  }
  bool getBool() {
    return (Value != 0);
  }
  char* getString() {
    return char * ((double)Value);
  }
  void setInt(s32 intValue) {
    Value = (f32)intValue;
  }
  void setFloat(f32 floatValue) {
    Value = floatValue;
  }
  void setString(const char* text) {
    Value = fast_atof(text);
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_FLOAT;
  }
  const char* getTypeString() const {
    return "float";
  }
  f32 Value;
};
/*
  Types which can be represented as a list of numbers
*/
// Base class for all attributes which are a list of numbers-
// vectors, colors, positions, triangles, etc
class CNumbersAttribute : public IAttribute
{
public:
  CNumbersAttribute(const char* name, SColorf value) :
    ValueI(), ValueF(), Count(4), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.r);
    ValueF.push_back(value.g);
    ValueF.push_back(value.b);
    ValueF.push_back(value.a);
  }
  CNumbersAttribute(const char* name, SColor value) :
    ValueI(), ValueF(), Count(4), IsFloat(false) {
    Name = name;
    ValueI.push_back(value.getRed());
    ValueI.push_back(value.getGreen());
    ValueI.push_back(value.getBlue());
    ValueI.push_back(value.getAlpha());
  }
  CNumbersAttribute(const char* name, float3 value) :
    ValueI(), ValueF(), Count(3), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.X);
    ValueF.push_back(value.Y);
    ValueF.push_back(value.Z);
  }
  CNumbersAttribute(const char* name, IRECT value) :
    ValueI(), ValueF(), Count(4), IsFloat(false) {
    Name = name;
    ValueI.push_back(value.UpperLeftCorner.X);
    ValueI.push_back(value.UpperLeftCorner.Y);
    ValueI.push_back(value.LowerRightCorner.X);
    ValueI.push_back(value.LowerRightCorner.Y);
  }
  CNumbersAttribute(const char* name, rect<f32> value) :
    ValueI(), ValueF(), Count(4), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.UpperLeftCorner.X);
    ValueF.push_back(value.UpperLeftCorner.Y);
    ValueF.push_back(value.LowerRightCorner.X);
    ValueF.push_back(value.LowerRightCorner.Y);
  }
  CNumbersAttribute(const char* name, matrix4 value) :
    ValueI(), ValueF(), Count(16), IsFloat(true) {
    Name = name;
    for (s32 r = 0; r < 4; ++r)
      for (s32 c = 0; c < 4; ++c) {
        ValueF.push_back(value(r, c));
      }
  }
  CNumbersAttribute(const char* name, quaternion value) :
    ValueI(), ValueF(), Count(4), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.X);
    ValueF.push_back(value.Y);
    ValueF.push_back(value.Z);
    ValueF.push_back(value.W);
  }
  CNumbersAttribute(const char* name, F3AABBOX value) :
    ValueI(), ValueF(), Count(6), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.MinEdge.X);
    ValueF.push_back(value.MinEdge.Y);
    ValueF.push_back(value.MinEdge.Z);
    ValueF.push_back(value.MaxEdge.X);
    ValueF.push_back(value.MaxEdge.Y);
    ValueF.push_back(value.MaxEdge.Z);
  }
  CNumbersAttribute(const char* name, plane3df value) :
    ValueI(), ValueF(), Count(4), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.Normal.X);
    ValueF.push_back(value.Normal.Y);
    ValueF.push_back(value.Normal.Z);
    ValueF.push_back(value.D);
  }
  CNumbersAttribute(const char* name, triangle3df value) :
    ValueI(), ValueF(), Count(9), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.pointA.X);
    ValueF.push_back(value.pointA.Y);
    ValueF.push_back(value.pointA.Z);
    ValueF.push_back(value.pointB.X);
    ValueF.push_back(value.pointB.Y);
    ValueF.push_back(value.pointB.Z);
    ValueF.push_back(value.pointC.X);
    ValueF.push_back(value.pointC.Y);
    ValueF.push_back(value.pointC.Z);
  }
  CNumbersAttribute(const char* name, vector2df value) :
    ValueI(), ValueF(), Count(2), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.X);
    ValueF.push_back(value.Y);
  }
  CNumbersAttribute(const char* name, vector2di value) :
    ValueI(), ValueF(), Count(2), IsFloat(false) {
    Name = name;
    ValueI.push_back(value.X);
    ValueI.push_back(value.Y);
  }
  CNumbersAttribute(const char* name, line2di value) :
    ValueI(), ValueF(), Count(4), IsFloat(false) {
    Name = name;
    ValueI.push_back(value.start.X);
    ValueI.push_back(value.start.Y);
    ValueI.push_back(value.end.X);
    ValueI.push_back(value.end.Y);
  }
  CNumbersAttribute(const char* name, line2df value) :
    ValueI(), ValueF(), Count(4), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.start.X);
    ValueF.push_back(value.start.Y);
    ValueF.push_back(value.end.X);
    ValueF.push_back(value.end.Y);
  }
  CNumbersAttribute(const char* name, line3df value) :
    ValueI(), ValueF(), Count(6), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.start.X);
    ValueF.push_back(value.start.Y);
    ValueF.push_back(value.start.Z);
    ValueF.push_back(value.end.X);
    ValueF.push_back(value.end.Y);
    ValueF.push_back(value.end.Z);
  }
  CNumbersAttribute(const char* name, dimension2du value) :
    ValueI(), ValueF(), Count(2), IsFloat(false) {
    Name = name;
    ValueI.push_back(value.Width);
    ValueI.push_back(value.Height);
  }
  CNumbersAttribute(const char* name, dimension2df value) :
    ValueI(), ValueF(), Count(2), IsFloat(true) {
    Name = name;
    ValueF.push_back(value.Width);
    ValueF.push_back(value.Height);
  }
  // getting values
  s32 getInt() {
    if (Count == 0) {
      return 0;
    }
    if (IsFloat) {
      return (s32)ValueF[0];
    }
    else {
      return ValueI[0];
    }
  }
  f32 getFloat() {
    if (Count == 0) {
      return 0.0f;
    }
    if (IsFloat) {
      return ValueF[0];
    }
    else {
      return (f32)ValueI[0];
    }
  }
  bool getBool() {
    // return true if any number is nonzero
    bool ret = false;
    int i;
    for (i = 0; i < Count; ++i)
      if (IsFloat ? (ValueF[i] != 0) : (ValueI[i] != 0)) {
        ret = true;
        break;
      }
    return ret;
  }
  char* getString() {
    char* outstr;
    int i;
    for (i = 0; i < Count; ++i) {
      if (IsFloat) {
        outstr += ValueF[i];
      }
      else {
        outstr += ValueI[i];
      }
      if (i < Count - 1) {
        outstr += ", ";
      }
    }
    return outstr;
  }
  position2di getPosition() {
    position2di p;
    if (IsFloat) {
      p.X = (s32)(Count > 0 ? ValueF[0] : 0);
      p.Y = (s32)(Count > 1 ? ValueF[1] : 0);
    }
    else {
      p.X = Count > 0 ? ValueI[0] : 0;
      p.Y = Count > 1 ? ValueI[1] : 0;
    }
    return p;
  }
  float3 getVector() {
    float3 v;
    if (IsFloat) {
      v.X = Count > 0 ? ValueF[0] : 0;
      v.Y = Count > 1 ? ValueF[1] : 0;
      v.Z = Count > 2 ? ValueF[2] : 0;
    }
    else {
      v.X = (f32)(Count > 0 ? ValueI[0] : 0);
      v.Y = (f32)(Count > 1 ? ValueI[1] : 0);
      v.Z = (f32)(Count > 2 ? ValueI[2] : 0);
    }
    return v;
  }
  vector2df getVector2d() {
    vector2df v;
    if (IsFloat) {
      v.X = Count > 0 ? ValueF[0] : 0;
      v.Y = Count > 1 ? ValueF[1] : 0;
    }
    else {
      v.X = (f32)(Count > 0 ? ValueI[0] : 0);
      v.Y = (f32)(Count > 1 ? ValueI[1] : 0);
    }
    return v;
  }
  SColorf getColorf() {
    SColorf c;
    if (IsFloat) {
      c.setColorComponentValue(0, Count > 0 ? ValueF[0] : 0);
      c.setColorComponentValue(1, Count > 1 ? ValueF[1] : 0);
      c.setColorComponentValue(2, Count > 2 ? ValueF[2] : 0);
      c.setColorComponentValue(3, Count > 3 ? ValueF[3] : 0);
    }
    else {
      c.setColorComponentValue(0, Count > 0 ? (f32)(ValueI[0]) / 255.0f : 0);
      c.setColorComponentValue(1, Count > 1 ? (f32)(ValueI[1]) / 255.0f : 0);
      c.setColorComponentValue(2, Count > 2 ? (f32)(ValueI[2]) / 255.0f : 0);
      c.setColorComponentValue(3, Count > 3 ? (f32)(ValueI[3]) / 255.0f : 0);
    }
    return c;
  }
  SColor getColor() {
    return getColorf().toSColor();
  }
  IRECT getRect() {
    IRECT r;
    if (IsFloat) {
      r.UpperLeftCorner.X  = (s32)(Count > 0 ? ValueF[0] : 0);
      r.UpperLeftCorner.Y  = (s32)(Count > 1 ? ValueF[1] : 0);
      r.LowerRightCorner.X = (s32)(Count > 2 ? ValueF[2] : r.UpperLeftCorner.X);
      r.LowerRightCorner.Y = (s32)(Count > 3 ? ValueF[3] : r.UpperLeftCorner.Y);
    }
    else {
      r.UpperLeftCorner.X  = Count > 0 ? ValueI[0] : 0;
      r.UpperLeftCorner.Y  = Count > 1 ? ValueI[1] : 0;
      r.LowerRightCorner.X = Count > 2 ? ValueI[2] : r.UpperLeftCorner.X;
      r.LowerRightCorner.Y = Count > 3 ? ValueI[3] : r.UpperLeftCorner.Y;
    }
    return r;
  }
  dimension2du getDimension2d() {
    dimension2du dim;
    if (IsFloat) {
      dim.Width = (u32)(Count > 0 ? ValueF[0] : 0);
      dim.Height = (u32)(Count > 1 ? ValueF[1] : 0);
    }
    else {
      dim.Width = (u32)(Count > 0 ? ValueI[0] : 0);
      dim.Height = (u32)(Count > 1 ? ValueI[1] : 0);
    }
    return dim;
  }
  matrix4 getMatrix() {
    matrix4 ret;
    if (IsFloat) {
      for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
          if (Count > c + r * 4) {
            ret(r, c) = ValueF[c + r * 4];
          }
    }
    else {
      for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
          if (Count > c + r * 4) {
            ret(r, c) = (f32)ValueI[c + r * 4];
          }
    }
    return ret;
  }
  quaternion getQuaternion() {
    quaternion ret;
    if (IsFloat) {
      ret.X = Count > 0 ? ValueF[0] : 0.0f;
      ret.Y = Count > 1 ? ValueF[1] : 0.0f;
      ret.Z = Count > 2 ? ValueF[2] : 0.0f;
      ret.W = Count > 3 ? ValueF[3] : 0.0f;
    }
    else {
      ret.X = Count > 0 ? (f32)ValueI[0] : 0.0f;
      ret.Y = Count > 1 ? (f32)ValueI[1] : 0.0f;
      ret.Z = Count > 2 ? (f32)ValueI[2] : 0.0f;
      ret.W = Count > 3 ? (f32)ValueI[3] : 0.0f;
    }
    return ret;
  }
  triangle3df getTriangle() {
    triangle3df ret;
    if (IsFloat) {
      ret.pointA.X = Count > 0 ? ValueF[0] : 0.0f;
      ret.pointA.Y = Count > 1 ? ValueF[1] : 0.0f;
      ret.pointA.Z = Count > 2 ? ValueF[2] : 0.0f;
      ret.pointB.X = Count > 3 ? ValueF[3] : 0.0f;
      ret.pointB.Y = Count > 4 ? ValueF[4] : 0.0f;
      ret.pointB.Z = Count > 5 ? ValueF[5] : 0.0f;
      ret.pointC.X = Count > 6 ? ValueF[6] : 0.0f;
      ret.pointC.Y = Count > 7 ? ValueF[7] : 0.0f;
      ret.pointC.Z = Count > 8 ? ValueF[8] : 0.0f;
    }
    else {
      ret.pointA.X = Count > 0 ? (f32)ValueI[0] : 0.0f;
      ret.pointA.Y = Count > 1 ? (f32)ValueI[1] : 0.0f;
      ret.pointA.Z = Count > 2 ? (f32)ValueI[2] : 0.0f;
      ret.pointB.X = Count > 3 ? (f32)ValueI[3] : 0.0f;
      ret.pointB.Y = Count > 4 ? (f32)ValueI[4] : 0.0f;
      ret.pointB.Z = Count > 5 ? (f32)ValueI[5] : 0.0f;
      ret.pointC.X = Count > 6 ? (f32)ValueI[6] : 0.0f;
      ret.pointC.Y = Count > 7 ? (f32)ValueI[7] : 0.0f;
      ret.pointC.Z = Count > 8 ? (f32)ValueI[8] : 0.0f;
    }
    return ret;
  }
  plane3df getPlane() {
    plane3df ret;
    if (IsFloat) {
      ret.Normal.X = Count > 0 ? ValueF[0] : 0.0f;
      ret.Normal.Y = Count > 1 ? ValueF[1] : 0.0f;
      ret.Normal.Z = Count > 2 ? ValueF[2] : 0.0f;
      ret.D    = Count > 3 ? ValueF[3] : 0.0f;
    }
    else {
      ret.Normal.X = Count > 0 ? (f32)ValueI[0] : 0.0f;
      ret.Normal.Y = Count > 1 ? (f32)ValueI[1] : 0.0f;
      ret.Normal.Z = Count > 2 ? (f32)ValueI[2] : 0.0f;
      ret.D    = Count > 3 ? (f32)ValueI[3] : 0.0f;
    }
    return ret;
  }
  aabbox3df getBBox() {
    aabbox3df ret;
    if (IsFloat) {
      ret.MinEdge.X = Count > 0 ? ValueF[0] : 0.0f;
      ret.MinEdge.Y = Count > 1 ? ValueF[1] : 0.0f;
      ret.MinEdge.Z = Count > 2 ? ValueF[2] : 0.0f;
      ret.MaxEdge.X = Count > 3 ? ValueF[3] : 0.0f;
      ret.MaxEdge.Y = Count > 4 ? ValueF[4] : 0.0f;
      ret.MaxEdge.Z = Count > 5 ? ValueF[5] : 0.0f;
    }
    else {
      ret.MinEdge.X = Count > 0 ? (f32)ValueI[0] : 0.0f;
      ret.MinEdge.Y = Count > 1 ? (f32)ValueI[1] : 0.0f;
      ret.MinEdge.Z = Count > 2 ? (f32)ValueI[2] : 0.0f;
      ret.MaxEdge.X = Count > 3 ? (f32)ValueI[3] : 0.0f;
      ret.MaxEdge.Y = Count > 4 ? (f32)ValueI[4] : 0.0f;
      ret.MaxEdge.Z = Count > 5 ? (f32)ValueI[5] : 0.0f;
    }
    return ret;
  }
  line2df getLine2d() {
    line2df ret;
    if (IsFloat) {
      ret.start.X = Count > 0 ? ValueF[0] : 0.0f;
      ret.start.Y = Count > 1 ? ValueF[1] : 0.0f;
      ret.end.X   = Count > 2 ? ValueF[2] : 0.0f;
      ret.end.Y   = Count > 3 ? ValueF[3] : 0.0f;
    }
    else {
      ret.start.X = Count > 0 ? (f32)ValueI[0] : 0.0f;
      ret.start.Y = Count > 1 ? (f32)ValueI[1] : 0.0f;
      ret.end.X   = Count > 2 ? (f32)ValueI[2] : 0.0f;
      ret.end.Y   = Count > 3 ? (f32)ValueI[3] : 0.0f;
    }
    return ret;
  }
  line3df getLine3d() {
    line3df ret;
    if (IsFloat) {
      ret.start.X = Count > 0 ? ValueF[0] : 0.0f;
      ret.start.Y = Count > 1 ? ValueF[1] : 0.0f;
      ret.start.Z = Count > 2 ? ValueF[2] : 0.0f;
      ret.end.X   = Count > 3 ? ValueF[3] : 0.0f;
      ret.end.Y   = Count > 4 ? ValueF[4] : 0.0f;
      ret.end.Z   = Count > 5 ? ValueF[5] : 0.0f;
    }
    else {
      ret.start.X = Count > 0 ? (f32)ValueI[0] : 0.0f;
      ret.start.Y = Count > 1 ? (f32)ValueI[1] : 0.0f;
      ret.start.Z = Count > 2 ? (f32)ValueI[2] : 0.0f;
      ret.end.X   = Count > 3 ? (f32)ValueI[3] : 0.0f;
      ret.end.Y   = Count > 4 ? (f32)ValueI[4] : 0.0f;
      ret.end.Z   = Count > 5 ? (f32)ValueI[5] : 0.0f;
    }
    return ret;
  }
  //! get float array
  f32 > getFloatArray() {
    if (!IsFloat) {
      ValueF.clear();
      int i;
      for (i = 0; i < Count; ++i) {
        ValueF.push_back((f32) ValueI[i]);
      }
    }
    return ValueF;
  }
  //! get int array
  s32 > getIntArray() {
    if (IsFloat) {
      ValueI.clear();
      int i;
      for (i = 0; i < Count; ++i) {
        ValueI.push_back((s32) ValueF[i]);
      }
    }
    return ValueI;
  }
  // setting values
  void setInt(s32 intValue) {
    // set all values
    int i;
    for (i = 0; i < Count; ++i)
      if (IsFloat) {
        ValueF[i] = (f32)intValue;
      }
      else {
        ValueI[i] = intValue;
      }
  }
  void setFloat(f32 floatValue) {
    // set all values
    int i;
    for (i = 0; i < Count; ++i)
      if (IsFloat) {
        ValueF[i] = floatValue;
      }
      else {
        ValueI[i] = (s32)floatValue;
      }
  }
  void setBool(bool boolValue) {
    setInt(boolValue ? 1 : 0);
  }
  void setString(const char* text) {
    // parse text
    const char* P = (const char*)text;
    reset();
    u32 i = 0;
    for (i = 0; i < Count && *P; ++i) {
      while (*P && P[0] != '-' && (P[0] == ' ' || (P[0] < '0' || P[0] > '9'))) {
        ++P;
      }
      // set value
      if (*P) {
        if (IsFloat) {
          f32 c = 0;
          P = fast_atof_move(P, c);
          ValueF[i] = c;
        }
        else {
          // todo: fix this to read ints properly
          f32 c = 0;
          P = fast_atof_move(P, c);
          ValueI[i] = (s32)c;
        }
      }
    }
    // todo: warning message
    //if (i < Count-1)
    //{
    //
    //}
  }
  void setPosition(position2di v) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = (f32)v.X;
      }
      if (Count > 1) {
        ValueF[1] = (f32)v.Y;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = v.X;
      }
      if (Count > 1) {
        ValueI[1] = v.Y;
      }
    }
  }
  void setVector(float3 v) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = v.X;
      }
      if (Count > 1) {
        ValueF[1] = v.Y;
      }
      if (Count > 2) {
        ValueF[2] = v.Z;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = (s32)v.X;
      }
      if (Count > 1) {
        ValueI[1] = (s32)v.Y;
      }
      if (Count > 2) {
        ValueI[2] = (s32)v.Z;
      }
    }
  }
  void setColor(SColorf color) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = color.r;
      }
      if (Count > 1) {
        ValueF[1] = color.g;
      }
      if (Count > 2) {
        ValueF[2] = color.b;
      }
      if (Count > 3) {
        ValueF[3] = color.a;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = (s32)(color.r * 255);
      }
      if (Count > 1) {
        ValueI[1] = (s32)(color.g * 255);
      }
      if (Count > 2) {
        ValueI[2] = (s32)(color.b * 255);
      }
      if (Count > 3) {
        ValueI[3] = (s32)(color.a * 255);
      }
    }
  }
  void setColor(SColor color) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = (f32)color.getRed() / 255.0f;
      }
      if (Count > 1) {
        ValueF[1] = (f32)color.getGreen() / 255.0f;
      }
      if (Count > 2) {
        ValueF[2] = (f32)color.getBlue() / 255.0f;
      }
      if (Count > 3) {
        ValueF[3] = (f32)color.getAlpha() / 255.0f;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = color.getRed();
      }
      if (Count > 1) {
        ValueI[1] = color.getGreen();
      }
      if (Count > 2) {
        ValueI[2] = color.getBlue();
      }
      if (Count > 3) {
        ValueI[3] = color.getAlpha();
      }
    }
  }
  void setRect(IRECT value) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = (f32)value.UpperLeftCorner.X;
      }
      if (Count > 1) {
        ValueF[1] = (f32)value.UpperLeftCorner.Y;
      }
      if (Count > 2) {
        ValueF[2] = (f32)value.LowerRightCorner.X;
      }
      if (Count > 3) {
        ValueF[3] = (f32)value.LowerRightCorner.Y;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = value.UpperLeftCorner.X;
      }
      if (Count > 1) {
        ValueI[1] = value.UpperLeftCorner.Y;
      }
      if (Count > 2) {
        ValueI[2] = value.LowerRightCorner.X;
      }
      if (Count > 3) {
        ValueI[3] = value.LowerRightCorner.Y;
      }
    }
  }
  void setMatrix(matrix4 value) {
    reset();
    if (IsFloat) {
      for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
          if (Count > c + r * 4) {
            ValueF[c + r * 4] = value(r, c);
          }
    }
    else {
      for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
          if (Count > c + r * 4) {
            ValueI[c + r * 4] = (s32)value(r, c);
          }
    }
  }
  void setQuaternion(quaternion value) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = value.X;
      }
      if (Count > 1) {
        ValueF[1] = value.Y;
      }
      if (Count > 2) {
        ValueF[2] = value.Z;
      }
      if (Count > 3) {
        ValueF[3] = value.W;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = (s32)value.X;
      }
      if (Count > 1) {
        ValueI[1] = (s32)value.Y;
      }
      if (Count > 2) {
        ValueI[2] = (s32)value.Z;
      }
      if (Count > 3) {
        ValueI[3] = (s32)value.W;
      }
    }
  }
  void setBoundingBox(F3AABBOX value) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = value.MinEdge.X;
      }
      if (Count > 1) {
        ValueF[1] = value.MinEdge.Y;
      }
      if (Count > 2) {
        ValueF[2] = value.MinEdge.Z;
      }
      if (Count > 3) {
        ValueF[3] = value.MaxEdge.X;
      }
      if (Count > 4) {
        ValueF[4] = value.MaxEdge.Y;
      }
      if (Count > 5) {
        ValueF[5] = value.MaxEdge.Z;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = (s32)value.MinEdge.X;
      }
      if (Count > 1) {
        ValueI[1] = (s32)value.MinEdge.Y;
      }
      if (Count > 2) {
        ValueI[2] = (s32)value.MinEdge.Z;
      }
      if (Count > 3) {
        ValueI[3] = (s32)value.MaxEdge.X;
      }
      if (Count > 4) {
        ValueI[4] = (s32)value.MaxEdge.Y;
      }
      if (Count > 5) {
        ValueI[5] = (s32)value.MaxEdge.Z;
      }
    }
  }
  void setPlane(plane3df value) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = value.Normal.X;
      }
      if (Count > 1) {
        ValueF[1] = value.Normal.Y;
      }
      if (Count > 2) {
        ValueF[2] = value.Normal.Z;
      }
      if (Count > 3) {
        ValueF[3] = value.D;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = (s32)value.Normal.X;
      }
      if (Count > 1) {
        ValueI[1] = (s32)value.Normal.Y;
      }
      if (Count > 2) {
        ValueI[2] = (s32)value.Normal.Z;
      }
      if (Count > 3) {
        ValueI[3] = (s32)value.D;
      }
    }
  }
  void setTriangle3d(triangle3df value) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = value.pointA.X;
      }
      if (Count > 1) {
        ValueF[1] = value.pointA.Y;
      }
      if (Count > 2) {
        ValueF[2] = value.pointA.Z;
      }
      if (Count > 3) {
        ValueF[3] = value.pointB.X;
      }
      if (Count > 4) {
        ValueF[4] = value.pointB.Y;
      }
      if (Count > 5) {
        ValueF[5] = value.pointB.Z;
      }
      if (Count > 6) {
        ValueF[6] = value.pointC.X;
      }
      if (Count > 7) {
        ValueF[7] = value.pointC.Y;
      }
      if (Count > 8) {
        ValueF[8] = value.pointC.Z;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = (s32)value.pointA.X;
      }
      if (Count > 1) {
        ValueI[1] = (s32)value.pointA.Y;
      }
      if (Count > 2) {
        ValueI[2] = (s32)value.pointA.Z;
      }
      if (Count > 3) {
        ValueI[3] = (s32)value.pointB.X;
      }
      if (Count > 4) {
        ValueI[4] = (s32)value.pointB.Y;
      }
      if (Count > 5) {
        ValueI[5] = (s32)value.pointB.Z;
      }
      if (Count > 6) {
        ValueI[6] = (s32)value.pointC.X;
      }
      if (Count > 7) {
        ValueI[7] = (s32)value.pointC.Y;
      }
      if (Count > 8) {
        ValueI[8] = (s32)value.pointC.Z;
      }
    }
  }
  void setVector2d(vector2df v) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = v.X;
      }
      if (Count > 1) {
        ValueF[1] = v.Y;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = (s32)v.X;
      }
      if (Count > 1) {
        ValueI[1] = (s32)v.Y;
      }
    }
  }
  void setVector2d(vector2di v) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = (f32)v.X;
      }
      if (Count > 1) {
        ValueF[1] = (f32)v.Y;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = v.X;
      }
      if (Count > 1) {
        ValueI[1] = v.Y;
      }
    }
  }
  void setLine2d(line2di v) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = (f32)v.start.X;
      }
      if (Count > 1) {
        ValueF[1] = (f32)v.start.Y;
      }
      if (Count > 2) {
        ValueF[2] = (f32)v.end.X;
      }
      if (Count > 3) {
        ValueF[3] = (f32)v.end.Y;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = v.start.X;
      }
      if (Count > 1) {
        ValueI[1] = v.start.Y;
      }
      if (Count > 2) {
        ValueI[2] = v.end.X;
      }
      if (Count > 3) {
        ValueI[3] = v.end.Y;
      }
    }
  }
  void setLine2d(line2df v) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = v.start.X;
      }
      if (Count > 1) {
        ValueF[1] = v.start.Y;
      }
      if (Count > 2) {
        ValueF[2] = v.end.X;
      }
      if (Count > 3) {
        ValueF[3] = v.end.Y;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = (s32)v.start.X;
      }
      if (Count > 1) {
        ValueI[1] = (s32)v.start.Y;
      }
      if (Count > 2) {
        ValueI[2] = (s32)v.end.X;
      }
      if (Count > 3) {
        ValueI[3] = (s32)v.end.Y;
      }
    }
  }
  void setDimension2d(dimension2du v) {
    reset();
    if (IsFloat) {
      if (Count > 0) {
        ValueF[0] = (f32)v.Width;
      }
      if (Count > 1) {
        ValueF[1] = (f32)v.Height;
      }
    }
    else {
      if (Count > 0) {
        ValueI[0] = (s32)v.Width;
      }
      if (Count > 1) {
        ValueI[1] = (s32)v.Height;
      }
    }
  }
  //! set float array
  void setFloatArray(f32 > & vals) {
    reset();
    int i;
    for (i = 0; i < vals_size && i < Count; ++i) {
      if (IsFloat) {
        ValueF[i] = vals[i];
      }
      else {
        ValueI[i] = (s32)vals[i];
      }
    }
  }
  //! set int array
  void setIntArray(s32* vals) {
    reset();
    int i;
    for (i = 0; i < vals_size && i < Count; ++i) {
      if (IsFloat) {
        ValueF[i] = (f32)vals[i];
      }
      else {
        ValueI[i] = vals[i];
      }
    }
  }
  //! is it a number list?
  bool isNumberList() {
    return true;
  }
  //! is it a float list?
  bool isFloat() {
    return IsFloat;
  }
  E_ATTRIBUTE_TYPE getType() const {
    if (IsFloat) {
      return EAT_FLOATARRAY;
    }
    else {
      return EAT_INTARRAY;
    }
  }
  const char* getTypeString() const {
    if (IsFloat) {
      return "floatlist";
    }
    else {
      return "intlist";
    }
  }
protected:
  //! clear all values
  void reset() {
    int i;
    if (IsFloat) {
      for (i = 0; i < Count ; ++i) {
        ValueF[i] = 0.0f;
      }
    }
    else {
      for (i = 0; i < Count ; ++i) {
        ValueI[i] = 0;
      }
    }
  }
  s32 > ValueI;
  f32 > ValueF;
  u32 Count;
  bool IsFloat;
};
// Attribute implemented for floating point colors
class CColorfAttribute : public CNumbersAttribute
{
public:
  CColorfAttribute(const char* name, SColorf value) : CNumbersAttribute(name, value) {}
  s32 getInt() {
    return getColor().color;
  }
  f32 getFloat() {
    return (f32)getColor().color;
  }
  void setInt(s32 intValue) {
    SColorf c = SColor(intValue);
    ValueF[0] = c.r;
    ValueF[1] = c.g;
    ValueF[2] = c.b;
    ValueF[3] = c.a;
  }
  void setFloat(f32 floatValue) {
    setInt((s32)floatValue);
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_COLORF;
  }
  const char* getTypeString() const {
    return "colorf";
  }
};
// Attribute implemented for colors
class CColorAttribute : public CNumbersAttribute
{
public:
  CColorAttribute(const char* name, const SColorf& value) : CNumbersAttribute(name, value) {}
  CColorAttribute(const char* name, const SColor& value) : CNumbersAttribute(name, value) {}
  s32 getInt() {
    return getColor().color;
  }
  f32 getFloat() {
    return (f32)getColor().color;
  }
  void setInt(s32 intValue) {
    SColorf c = SColor(intValue);
    ValueF[0] = c.r;
    ValueF[1] = c.g;
    ValueF[2] = c.b;
    ValueF[3] = c.a;
  }
  void setFloat(f32 floatValue) {
    setInt((s32)floatValue);
  }
  char* getString() {
    char tmp[10];
    const SColor c = getColor();
    sprintf(tmp, "%02x%02x%02x%02x", c.getAlpha(), c.getRed(), c.getGreen(), c.getBlue());
    return char * (tmp);
  }
  void setString(const char* text) {
    u32 c;
    if (sscanf(text, "%08x", &c) != 1) {
      CNumbersAttribute::setString(text);
    }
    else {
      setColor(c);
    }
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_COLOR;
  }
  const char* getTypeString() const {
    return "color";
  }
};
// Attribute implemented for 3d vectors
class CVector3DAttribute : public CNumbersAttribute
{
public:
  CVector3DAttribute(const char* name, float3 value) : CNumbersAttribute(name, value) {}
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_VECTOR3D;
  }
  matrix4 getMatrix() {
    matrix4 ret;
    ret.makeIdentity();
    ret.setTranslation(float3(ValueF[0], ValueF[1], ValueF[2]));
    return ret;
  }
  const char* getTypeString() const {
    return "vector3d";
  }
};
// Attribute implemented for 2d vectors
class CVector2DAttribute : public CNumbersAttribute
{
public:
  CVector2DAttribute(const char* name, vector2df value) : CNumbersAttribute(name, value) {}
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_VECTOR2D;
  }
  const char* getTypeString() const {
    return "vector2d";
  }
};
// Attribute implemented for 2d vectors
class CPosition2DAttribute : public CNumbersAttribute
{
public:
  CPosition2DAttribute(const char* name, position2di value) : CNumbersAttribute(name, value) {}
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_POSITION2D;
  }
  const char* getTypeString() const {
    return "position";
  }
};
// Attribute implemented for rectangles
class CRectAttribute : public CNumbersAttribute
{
public:
  CRectAttribute(const char* name, IRECT value) : CNumbersAttribute(name, value) { }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_RECT;
  }
  const char* getTypeString() const {
    return "rect";
  }
};
// Attribute implemented for dimension
class CDimension2dAttribute : public CNumbersAttribute
{
public:
  CDimension2dAttribute(const char* name, dimension2du value) : CNumbersAttribute(name, value) { }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_DIMENSION2D;
  }
  const char* getTypeString() const {
    return "dimension2d";
  }
};
// Attribute implemented for matrices
class CMatrixAttribute : public CNumbersAttribute
{
public:
  CMatrixAttribute(const char* name, matrix4 value) : CNumbersAttribute(name, value) { }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_MATRIX;
  }
  quaternion getQuaternion() {
    return quaternion(getMatrix());
  }
  const char* getTypeString() const {
    return "matrix";
  }
};
// Attribute implemented for quaternions
class CQuaternionAttribute : public CNumbersAttribute
{
public:
  CQuaternionAttribute(const char* name, quaternion value) : CNumbersAttribute(name, value) { }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_QUATERNION;
  }
  matrix4 getMatrix() {
    return getQuaternion().getMatrix();
  }
  const char* getTypeString() const {
    return "quaternion";
  }
};
// Attribute implemented for bounding boxes
class CBBoxAttribute : public CNumbersAttribute
{
public:
  CBBoxAttribute(const char* name, aabbox3df value) : CNumbersAttribute(name, value) { }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_BBOX;
  }
  const char* getTypeString() const {
    return "box3d";
  }
};
// Attribute implemented for planes
class CPlaneAttribute : public CNumbersAttribute
{
public:
  CPlaneAttribute(const char* name, plane3df value) : CNumbersAttribute(name, value) { }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_PLANE;
  }
  const char* getTypeString() const {
    return "plane";
  }
};
// Attribute implemented for triangles
class CTriangleAttribute : public CNumbersAttribute
{
public:
  CTriangleAttribute(const char* name, triangle3df value) : CNumbersAttribute(name, value) { }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_TRIANGLE3D;
  }
  plane3df getPlane() {
    return getTriangle().getPlane();
  }
  const char* getTypeString() const {
    return "triangle";
  }
};
// Attribute implemented for 2d lines
class CLine2dAttribute : public CNumbersAttribute
{
public:
  CLine2dAttribute(const char* name, line2df value) : CNumbersAttribute(name, value) { }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_LINE2D;
  }
  const char* getTypeString() const {
    return "line2d";
  }
};
// Attribute implemented for 3d lines
class CLine3dAttribute : public CNumbersAttribute
{
public:
  CLine3dAttribute(const char* name, line3df value) : CNumbersAttribute(name, value) { }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_LINE3D;
  }
  const char* getTypeString() const {
    return "line3d";
  }
};
// vector2df
// dimension2du
/*
  Special attributes
*/
// Attribute implemented for enumeration literals
class CEnumAttribute : public IAttribute
{
public:
  CEnumAttribute(const char* name, const char* value, const char* const* literals) {
    Name = name;
    setEnum(value, literals);
  }
  void setEnum(const char* enumValue, const char* const* enumerationLiterals) {
    int literalCount = 0;
    if (enumerationLiterals) {
      s32 i;
      for (i = 0; enumerationLiterals[i]; ++i) {
        ++literalCount;
      }
      EnumLiterals.reallocate(literalCount);
      for (i = 0; enumerationLiterals[i]; ++i) {
        EnumLiterals.push_back(enumerationLiterals[i]);
      }
    }
    setString(enumValue);
  }
  s32 getInt() {
    for (s32 i = 0; EnumLiterals_size; ++i)
      if (Value.equals_ignore_case(EnumLiterals[i])) {
        return i;
      }
    return -1;
  }
  f32 getFloat() {
    return (f32)getInt();
  }
  bool getBool() {
    return (getInt() != 0); // does not make a lot of sense, I know
  }
  char* getString() {
    return Value;
  }
  void setInt(s32 intValue) {
    if (intValue >= 0 && intValue < (s32)EnumLiterals_size) {
      Value = EnumLiterals[intValue];
    }
    else {
      Value = "";
    }
  }
  void setFloat(f32 floatValue) {
    setInt((s32)floatValue);
  };
  void setString(const char* text) {
    Value = text;
  }
  const char* getEnum() {
    return Value;
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_ENUM;
  }
  const char* getTypeString() const {
    return "enum";
  }
  char* Value;
  strv_t EnumLiterals;
};
// Attribute implemented for strings
class CStringAttribute : public IAttribute
{
public:
  CStringAttribute(const char* name, const char* value) {
    IsStringW = false;
    Name = name;
    setString(value);
  }
  CStringAttribute(const char* name, void* binaryData, s32 lenghtInBytes) {
    IsStringW = false;
    Name = name;
    setBinary(binaryData, lenghtInBytes);
  }
  s32 getInt() {
    if (IsStringW) {
      return atoi(char * (ValueW));
    }
    else {
      return atoi(Value);
    }
  }
  f32 getFloat() {
    if (IsStringW) {
      return fast_atof(char * (ValueW));
    }
    else {
      return fast_atof(Value);
    }
  }
  bool getBool() {
    if (IsStringW) {
      return ValueW.equals_ignore_case("true");
    }
    else {
      return Value.equals_ignore_case("true");
    }
  }
  char* getString() {
    if (IsStringW) {
      return char * (ValueW);
    }
    else {
      return Value;
    }
  }
  void setInt(s32 intValue) {
    if (IsStringW) {
      ValueW = char * (intValue);
    }
    else {
      Value = char * (intValue);
    }
  }
  void setFloat(f32 floatValue) {
    if (IsStringW) {
      ValueW = char * ((double)floatValue);
    }
    else {
      Value = char * ((double)floatValue);
    }
  };
  void setString(const char* text) {
    if (IsStringW) {
      ValueW = char * (text);
    }
    else {
      Value = text;
    }
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_STRING;
  }
  const char* getTypeString() const {
    return "string";
  }
  void getBinary(void* outdata, s32 maxLength) {
    s32 dataSize = maxLength;
    c8* datac8 = (c8*)(outdata);
    s32 p = 0;
    const c8* dataString = Value;
    for (s32 i = 0; i < dataSize; ++i) {
      datac8[i] = 0;
    }
    while (dataString[p] && p < dataSize) {
      s32 v = getByteFromHex((c8)dataString[p * 2]) * 16;
      if (dataString[(p * 2) + 1]) {
        v += getByteFromHex((c8)dataString[(p * 2) + 1]);
      }
      datac8[p] = v;
      ++p;
    }
  };
  void setBinary(void* data, s32 maxLength) {
    s32 dataSize = maxLength;
    c8* datac8 = (c8*)(data);
    char tmp[3];
    tmp[2] = 0;
    Value = "";
    for (s32 b = 0; b < dataSize; ++b) {
      getHexStrFromByte(datac8[b], tmp);
      Value.append(tmp);
    }
  };
  bool IsStringW;
  char* Value;
  char* ValueW;
protected:
  static CV_INLINE s32 getByteFromHex(c8 h) {
    if (h >= '0' && h <= '9') {
      return h - '0';
    }
    if (h >= 'a' && h <= 'f') {
      return h - 'a' + 10;
    }
    return 0;
  }
  static CV_INLINE void getHexStrFromByte(c8 byte, c8* out) {
    s32 b = (byte & 0xf0) >> 4;
    for (s32 i = 0; i < 2; ++i) {
      if (b >= 0 && b <= 9) {
        out[i] = b + '0';
      }
      if (b >= 10 && b <= 15) {
        out[i] = (b - 10) + 'a';
      }
      b = byte & 0x0f;
    }
  }
};
// Attribute implemented for binary data
class CBinaryAttribute : public CStringAttribute
{
public:
  CBinaryAttribute(const char* name, void* binaryData, s32 lenghtInBytes)
    : CStringAttribute(name, binaryData, lenghtInBytes) {
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_BINARY;
  }
  const char* getTypeString() const {
    return "binary";
  }
};
// Attribute implemented for texture references
class CTextureAttribute : public IAttribute
{
public:
  CTextureAttribute(const char* name, ITexture* value, IVideoDriver* driver, const char* filename)
    : Value(0), Driver(driver), OverrideName(filename) {
    if (Driver) {
      Driver->grab();
    }
    Name = name;
    setTexture(value);
  }
  ~CTextureAttribute() {
    if (Driver) {
      Driver->drop();
    }
    if (Value) {
      Value->drop();
    }
  }
  ITexture* getTexture() {
    return Value;
  }
  bool getBool() {
    return (Value != 0);
  }
  char* getString() {
    // since texture names can be stringc we are careful with the types
    if (OverrideName_size) {
      return char * (OverrideName);
    }
    if (Value) {
      return char * (Value->getName().getPath());
    }
    return char * (0);
  }
  void setString(const char* text) {
    if (Driver) {
      if (text && *text) {
        setTexture(Driver->getTexture(text));
        OverrideName = text;
      }
      else {
        setTexture(0);
      }
    }
  }
  void setTexture(ITexture* value) {
    if (value == Value) {
      return;
    }
    if (Value) {
      Value->drop();
    }
    Value = value;
    if (Value) {
      Value->grab();
    }
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_TEXTURE;
  }
  const char* getTypeString() const {
    return "texture";
  }
  ITexture* Value;
  IVideoDriver* Driver;
  char* OverrideName;
};
// Attribute implemented for array of stringc
class CStringWArrayAttribute : public IAttribute
{
public:
  CStringWArrayAttribute(const char* name, const strv_t value) {
    Name = name;
    setArray(value);
  }
  strv_t getArray() {
    return Value;
  }
  void setArray(const strv_t value) {
    Value = value;
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_STRINGWARRAY;
  }
  const char* getTypeString() const {
    return "stringwarray";
  }
  strv_t Value;
};
// Attribute implemented for user pointers
class CUserPointerAttribute : public IAttribute
{
public:
  CUserPointerAttribute(const char* name, void* value) {
    Name = name;
    Value = value;
  }
  s32 getInt() {
    return *static_cast<s32*>(Value);
  }
  bool getBool() {
    return (Value != 0);
  }
  char* getString() {
    char buf[32];
    snprintf(buf, 32, "%p", Value);
    return char * (buf);
  }
  void setString(const char* text) {
    u32 tmp;
    sscanf(text, "0x%x", &tmp);
    Value = (void*) tmp;
  }
  E_ATTRIBUTE_TYPE getType() const {
    return EAT_USER_POINTER;
  }
  void setUserPointer(void* v) {
    Value = v;
  }
  void* getUserPointer() {
    return Value;
  }
  const char* getTypeString() const {
    return "userPointer";
  }
  void* Value;
};
// todo: CGUIFontAttribute

