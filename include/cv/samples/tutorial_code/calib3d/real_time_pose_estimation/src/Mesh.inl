/*
 * Mesh.h
 *
 *  Created on: Apr 9, 2014
 *      Author: edgar
 */

#ifndef MESH_H_
#define MESH_H_

#include <iostream>
#include <opencv2/core/core.hpp>


// --------------------------------------------------- //
//                 TRIANGLE CLASS                      //
// --------------------------------------------------- //

class Triangle {
public:

  explicit Triangle(int id, CPoint3f V0, CPoint3f V1, CPoint3f V2);
  virtual ~Triangle();

  CPoint3f getV0() const { return v0_; }
  CPoint3f getV1() const { return v1_; }
  CPoint3f getV2() const { return v2_; }

private:
  /** The identifier number of the triangle */
  int id_;
  /** The three vertices that defines the triangle */
  CPoint3f v0_, v1_, v2_;
};


// --------------------------------------------------- //
//                     RAY CLASS                       //
// --------------------------------------------------- //

class Ray {
public:

  explicit Ray(CPoint3f P0, CPoint3f P1);
  virtual ~Ray();

  CPoint3f getP0() { return p0_; }
  CPoint3f getP1() { return p1_; }

private:
  /** The two points that defines the ray */
  CPoint3f p0_, p1_;
};


// --------------------------------------------------- //
//                OBJECT MESH CLASS                    //
// --------------------------------------------------- //

class Mesh
{
public:

  Mesh();
  virtual ~Mesh();

  std::vector<std::vector<int> > getTrianglesList() const { return list_triangles_; }
  CPoint3f getVertex(int pos) const { return list_vertex_[pos]; }
  int getNumVertices() const { return num_vertexs_; }

  void load(const std::string path_file);

private:
  /** The identification number of the mesh */
  int id_;
  /** The current number of vertices in the mesh */
  int num_vertexs_;
  /** The current number of triangles in the mesh */
  int num_triangles_;
  /* The list of triangles of the mesh */
  std::vector<CPoint3f> list_vertex_;
  /* The list of triangles of the mesh */
  std::vector<std::vector<int> > list_triangles_;
};

#endif /* OBJECTMESH_H_ */
