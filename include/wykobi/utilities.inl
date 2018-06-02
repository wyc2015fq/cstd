/*
(***********************************************************************)
(* *)
(* Wykobi Computational Geometry Library *)
(* Release Version 0.0.4 *)
(* http://www.wykobi.com *)
(* Copyright (c) 2005-2009 Arash Partow, All Rights Reserved. *)
(* *)
(* The Wykobi computational geometry library and its components are *)
(* supplied under the terms of the General Wykobi License agreement. *)
(* The contents of the Wykobi computational geometry library and its *)
(* components may not be copied or disclosed except in accordance with *)
(* the terms of that agreement. *)
(* *)
(* URL: http://www.wykobi.com/license.html *)
(* *)
(***********************************************************************)
*/

#ifndef INCLUDE_WYKOBI_UTILTIIES
#define INCLUDE_WYKOBI_UTILTIIES

#include <iostream>
#include <iomanip>
#include <vector>

#include "wykobi.hpp"
#include "wykobi_matrix.hpp"

namespace wykobi
{
template<typename Type>
CC_INLINE ostream& operator<<(ostream& os, const point2d<Type>& point)
{
  os << scientific
      << showpoint
      << setprecision(6)
      << "(" << point.x << "," << point.y << ")";
  return os;
}

template<typename Type>
CC_INLINE ostream& operator<<(ostream& os, const point3d<Type>& point)
{
  os << scientific
      << showpoint
      << setprecision(6)
      << "(" << point.x << "," << point.y << "," << point.z << ")";
  return os;
}

template<typename Type>
CC_INLINE ostream& operator<<(ostream& os, const vector2d<Type>& v)
{
  os << scientific
      << showpoint
      << setprecision(6)
      << "(" << v.x << "," << v.y << ")";
  return os;
}

template<typename Type>
CC_INLINE ostream& operator<<(ostream& os, const vector3d<Type>& v)
{
  os << scientific
      << showpoint
      << setprecision(6)
      << "(" << v.x << "," << v.y << "," << v.z << ")";
  return os;
}

template<typename Type>
CC_INLINE ostream& operator<<(ostream& os, const ray<Type, 2>& ray)
{
  os << scientific
      << showpoint
      << setprecision(6)
      << "(" << ray.origin.x << "," << ray.origin.y << "," << ray.direction.x << "," << ray.direction.y << ")";
  return os;
}

template<typename Type>
CC_INLINE ostream& operator<<(ostream& os, const ray<Type, 3>& ray)
{
  os << scientific
      << showpoint
      << setprecision(6)
      << "(" << ray.origin.x << "," << ray.origin.y << "," << ray.origin.z << ","
      << ray.direction.x << "," << ray.direction.y << "," << ray.direction.z << ")";
  return os;
}

template<typename Type, int Dimension>
CC_INLINE ostream& operator<<(ostream& os, const pointnd<Type, Dimension>& point)
{
  os << "(";

  for (int i = 0; i < Dimension - 1; ++i) {
    os << scientific
        << showpoint
        << setprecision(6)
        << point[i] << ",";
  }

  os << scientific
      << showpoint
      << setprecision(6)
      << point[Dimension - 1] << ")";

  return os;
}

template<typename Type, int Dimension>
CC_INLINE ostream& operator<<(ostream& os, const segment<Type, Dimension>& segment)
{
  for (unsigned int i = 0; i < segment<Type, Dimension>::PointCount; ++i) {
    os << segment[i];
  }

  return os;
}

template<typename Type, int Dimension>
CC_INLINE ostream& operator<<(ostream& os, const line<Type, Dimension>& line)
{
  for (unsigned int i = 0; i < line<Type, Dimension>::PointCount; ++i) {
    os << line[i];
  }

  return os;
}

template<typename Type, int Dimension>
CC_INLINE ostream& operator<<(ostream& os, const triangle<Type, Dimension>& triangle)
{
  for (unsigned int i = 0; i < triangle<Type, Dimension>::PointCount; ++i) {
    os << triangle[i];
  }

  return os;
}

template<typename Type>
CC_INLINE ostream& operator<<(ostream& os, const rectangle<Type>& rectangle)
{
  for (unsigned int i = 0; i < rectangle<Type>::PointCount; ++i) {
    os << rectangle[i];
  }

  return os;
}

template<typename Type, int Dimension>
CC_INLINE ostream& operator<<(ostream& os, const box<Type, Dimension>& box)
{
  for (unsigned int i = 0; i < box<Type, Dimension>::PointCount; ++i) {
    os << box[i];
  }

  return os;
}

template<typename Type, int Dimension>
CC_INLINE ostream& operator<<(ostream& os, const quadix<Type, Dimension>& quadix)
{
  for (unsigned int i = 0; i < quadix<Type, Dimension>::PointCount; ++i) {
    os << quadix[i];
  }

  return os;
}

template<typename Type>
CC_INLINE ostream& operator<<(ostream& os, const circle<Type>& circle)
{
  os << scientific
      << showpoint
      << setprecision(10)
      << "(" << circle.x << "," << circle.y << "," << circle.radius << ")";
  return os;
}

template<typename Type>
CC_INLINE ostream& operator<<(ostream& os, const sphere<Type>& sphere)
{
  os << scientific
      << showpoint
      << setprecision(6)
      << "(" << sphere.x << "," << sphere.y << "," << sphere.z << "," << sphere.radius << ")";
  return os;
}

template<typename Type, int M, int N>
CC_INLINE ostream& operator<<(ostream& os, const matrix<Type, M, N>& matrix)
{
  for (int x = 0; x < M; x++) {
    for (int y = 0; y < N; y++) {
      os << matrix(x, y) << "\t";
    }

    os << endl;
  }

  return os;
}

CC_INLINE void finalize_trigonometry_tables()
{
  delete[] sin_table;
  delete[] cos_table;
  delete[] tan_table;
}

CC_INLINE void initialize_trigonometry_tables()
{
  finalize_trigonometry_tables();

  sin_table = new Float[TrigTableSize];
  cos_table = new Float[TrigTableSize];
  tan_table = new Float[TrigTableSize];

  for (unsigned int i = 0; i < TrigTableSize; ++i) {
    sin_table[i] = Float(sin((1.0 * i) * PIDiv180));
    cos_table[i] = Float(cos((1.0 * i) * PIDiv180));
    tan_table[i] = Float(tan((1.0 * i) * PIDiv180));
  }
}

} // namespace wykobi

#endif

