// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// This file was originally written by Salvatore Russo.
// I (Nikolaus Gebhardt) did some minor modifications changes to it and integrated
// it into Irrlicht:
// - removed STL dependency
// - removed log file and replaced it with irrlicht logging
// - adapted code formatting a bit to Irrlicht style
// - removed memory leaks
// Thanks a lot to Salvatore for his work on this and that he gave me
// his permission to add it into Irrlicht.
// This support library has been made by Salvatore Russo and is released under GNU public license for general uses.
// For uses in Irrlicht core and only for Irrlicht related uses I release this library under zlib license.
#ifndef __DMF_SUPPORT_H_INCLUDED__
#define __DMF_SUPPORT_H_INCLUDED__
//#include "irrString.h"
/** A structure representing some DeleD infos.
This structure contains data about DeleD level file like: version, ambient color, number of objects etc...*/
struct dmfHeader {
  //main file header
  char* dmfName; //!<Scene name
  f32 dmfVersion;     //!<File version
  SColor dmfAmbient; //!<Ambient color
  f32 dmfShadow;     //!<Shadow intensity
  u32 numObjects;    //!<Number of objects in this scene
  u32 numMaterials;  //!<Number of materials in this scene
  u32 numVertices;   //!<Total number of vertices faces*(vertices for each face)
  u32 numFaces;      //!<Total number of faces
  u32 numLights;     //!<Number of dynamic lights in this scene
  u32 numWatVertices; //!<Total number of vertices of water plains watfaces*(vertices for each face)
  u32 numWatFaces;   //!<Total number of faces for water plains.Note that each water plane is a rectangle with one face only.
};
/** A structure representing a DeleD material.
This structure contains texture names, an ID and some flags.*/
struct dmfMaterial {
  u32 materialID;//!<This material unique ID.
  u32 textureLayers;//!<First texture Flag (0=Normal, 1=Color).
  u32 textureFlag;//!<First texture Flag (0=Normal, 1=Color).
  u32 lightmapFlag;//!<Lightmap Flag (0=Normal, others not considered).
  u32 textureBlend;//!<Texture Blend mode used to support alpha maps (4=Alpha map, others not implemented yet).
  char* pathName;//!<Name of path defined in path element.
  char* textureName;//!<Name of first texture (only file name, no path).
  char* lightmapName;//!<Name of lightmap (only file name, no path).
  u32 lightmapBlend;//!<Blend mode used to support alpha maps (not implemented yet).
};
/** A structure representing a single face.
This structure contains first vertice index, number of vertices and the material used.*/
struct dmfFace {
  u32 firstVert;//!<First vertex index.
  u32 numVerts;//!<Number of vertices for this face.
  u32 materialID;//!<Material used for this face.
};
/** A structure representing a single vertice.
This structure contains vertice position coordinates and texture an lightmap UV.*/
struct dmfVert {
  float3 pos;//!<Position of vertex
  vector2df tc;//!<Texture UV coords
  vector2df lc;//!<Lightmap UV coords
};
/** A structure representing a single dynamic light.
This structure contains light position coordinates, diffuse color, specular color and maximum radius of light.*/
struct dmfLight {
  float3 pos;//!<Position of this light.
  SColorf diffuseColor;//!<Diffuse color.
  SColorf specularColor;//!<Specular color.
  f32 radius;//!<Maximum radius of light.
};
/** A structure representing a single water plane.
This structure contains light position coordinates, diffuse color, specular color and maximum radius of light.*/
struct dmfWaterPlane {
  u32 waterID;//!<ID of specified water plane.
  u32 numFaces;//!<number of faces that make this plain.Owing to the fact that this is a rectangle you'll have 1 every time.
  u32 firstFace;//!<first face of this plain.
  dimension2du tileNum;//!<number of tiles of this water plain.
  f32 waveHeight;//!<height of waves.
  f32 waveSpeed;//!<waves speed.
  f32 waveLength;//!<waves length.
};
/** A function to convert a hexstring to a int.
This function converts an hex string (i.e. FF) to its int value (i.e. 255).
\return An int representing the hex input value.*/
int axtoi(const char* hexStg)
{
  unsigned int intValue = 0;  // integer value of hex string
  sscanf(hexStg, "%x", &intValue);
  return (intValue);
}
typedef strv_t StringList;
//Loads a stringlist from a file
//note that each String added to StringList
//is separated by a \\n character and it's present
//at the end of line.
/** Loads a StringList from a file.
This function loads a StringList from a file where each string is divided by a \\n char.*/
void LoadFromFile(IReadFile* file, StringList& strlist)
{
  const long sz = file->getSize();
  char* buf = new char[sz + 1];
  file->read(buf, sz);
  buf[sz] = 0;
  char* p = buf;
  char* start = p;
  while (*p) {
    if (*p == '\n') {
      char* str(start, (u32)(p - start - 1));
      str.trim();
      strlist.push_back(str);
      start = p + 1;
    }
    ++p;
  }
  if (p - start > 1) {
    char* str(start, (u32)(p - start - 1));
    str.trim();
    strlist.push_back(str);
  }
  delete [] buf;
};
//This function subdivides a string in a list of strings
/** This function subdivides strings divided by divider in a list of strings.
\return A StringList made of all strings divided by divider.*/
StringList SubdivideString(const char* str, const char* divider)
{
  StringList strings; //returned StringList
  strings.clear();    //clear returned stringlist
  int c = 0;
  int l = str_size;
  //process entire string
  while (c < l) {
    char* resultstr;
    resultstr = "";
    //read characters until divider is encountered
    while ((str[c] != divider[0]) && c < l) {
      resultstr += str[c];
      ++c;
    }
    //Remove spaces \t and \n from string in my implementation...
    //pay attention or change it in dll.h if you don't want to remove
    //a particular char.
    resultstr.trim();//trims string resultstr
    strings.push_back(resultstr);//add trimmed string
    ++c;
  }
  return strings;
}
//Get DeleD informations and convert in dmfHeader
/**This function extract a dmfHeader from a DMF file.
You must give in input a StringList representing a DMF file loaded with LoadFromFile.
\return true if function succeed or false on fail.*/
bool GetDMFHeader(const StringList& RawFile, dmfHeader& header)
{
  StringList temp;
  RawFile[0].split(temp, ";"); //file info
  //  StringList temp=SubdivideString(RawFile[0],";"); //file info
  if (temp[0] != "DeleD Map File") {
    return false;  //not a deled file
  }
  temp.clear();
  temp = SubdivideString(RawFile[1], " "); //get version
  StringList temp1 = SubdivideString(temp[1], ";");
  header.dmfVersion = (float)atof(temp1[0]);//save version
  if (header.dmfVersion < 0.91) {
    return false;  //not correct version
  }
  temp.clear();
  temp = SubdivideString(RawFile[2], ";"); //get name,ambient color and shadow opacity
  header.dmfName = temp[0]; //save name
  //set ambient color
  header.dmfAmbient.set(axtoi(temp[1]));
  //set Shadow intensity
  header.dmfShadow = (float)atof(temp[2]);
  //set current position
  int offs = 3;
  //set Materials Number
  header.numMaterials = atoi(RawFile[offs]);
  offs += header.numMaterials;
  ++offs;
  //set Object Number
  header.numObjects = atoi(RawFile[offs]);
  //retrieve face and vertices number
  header.numVertices = 0;
  header.numFaces = 0;
  header.numWatFaces = 0;
  header.numWatVertices = 0;
  offs++;
  s32 fac;
  int i;
  for (i = 0; i < (int)header.numObjects; i++) {
    StringList wat = SubdivideString(RawFile[offs], ";");
    StringList wat1 = SubdivideString(wat[0], "_");
    ++offs;
    offs += atoi(RawFile[offs]);
    ++offs;
    fac = atoi(RawFile[offs]);
    if (!(wat1[0] == "water" && wat[2] == "0")) {
      header.numFaces = header.numFaces + fac;
    }
    else {
      header.numWatFaces = header.numWatFaces + fac;
    }
    offs++;
    for (int j = 0; j < fac; j++) {
      if (!(wat1[0] == "water" && wat[2] == "0")) {
        header.numVertices = header.numVertices + atoi(RawFile[offs + j]);
      }
      else {
        header.numWatVertices = header.numWatVertices + atoi(RawFile[offs + j]);
      }
    }
    offs = offs + fac;
  }
  //retrieve number of dynamic lights
  header.numLights = 0;
  temp.clear();
  temp1.clear();
  s32 lit = atoi(RawFile[offs]);
  for (i = 0; i < lit; i++) {
    offs++;
    temp = SubdivideString(RawFile[offs], ";");
    if (atoi(temp[0]) == 1) {
      temp1 = SubdivideString(temp[18], "_");
      if (temp1[0] == "dynamic") {
        header.numLights++;
      }
    }
    temp.clear();
    temp1.clear();
  }
  return true; //everything is OK so loading is correct
}
/**This function extract an array of dmfMaterial from a DMF file.
You must give in input a StringList representing a DMF file loaded with LoadFromFile.
\param RawFile StringList representing a DMF file.
\param materials Materials returned.
\param num_material Number of materials contained in DMF file.
\param use_material_dirs Here you can choose to use default DeleD structure for material dirs.
\return true if function succeed or false on fail.*/
bool GetDMFMaterials(const StringList& RawFile,
    dmfMaterial > & materials,
    int num_material)
{
  // offset for already handled lines
  const int offs = 4;
  StringList temp;
  StringList temp1;
  // The number of materials is predetermined
  materials.reallocate(num_material);
  int i;
  for (i = 0; i < num_material; ++i) {
    materials.push_back(dmfMaterial());
    // get all tokens
    temp = SubdivideString(RawFile[offs + i], ";");
    // should be equal to first token
    materials[i].materialID = i;
    // The path used for the texture
    materials[i].pathName = temp[2];
    materials[i].pathName.replace('\\', '/');
    materials[i].pathName += "/";
    // temp[3] is reserved, temp[4] is the number of texture layers
    materials[i].textureLayers = strtoul10(temp[4]);
    // Three values are separated by commas
    temp1 = SubdivideString(temp[5], ",");
    materials[i].textureFlag = atoi(temp1[0]);
    materials[i].textureName = temp1[1];
    materials[i].textureName.replace('\\', '/');
    materials[i].textureBlend = atoi(temp1[2]);
    if (temp_size >= 9) {
      temp1 = SubdivideString(temp[temp_size - 1], ",");
      materials[i].lightmapFlag = atoi(temp1[0]);
      materials[i].lightmapName = temp1[1];
      materials[i].lightmapName.replace('\\', '/');
      materials[i].lightmapBlend = atoi(temp1[2]);
    }
    else {
      materials[i].lightmapFlag = 1;
      materials[i].lightmapName = "";
    }
  }
  return true;
}
/**This function extract an array of dmfMaterial from a DMF file considering 1st an 2nd layer for water plains.
You must give in input a StringList representing a DMF file loaded with LoadFromFile.
\return true if function succeed or false on fail.*/
bool GetDMFWaterMaterials(const StringList& RawFile /**<StringList representing a DMF file.*/,
    dmfMaterial > & materials/**<Materials returned.*/,
    int num_material/**<Number of materials contained in DMF file.*/
                         )
{
  int offs = 4;
  StringList temp;
  StringList temp1;
  StringList temp2;
  //Checking if this is a DeleD map File of version >= 0.91
  temp = SubdivideString(RawFile[0], ";"); //file info
  if (temp[0] != "DeleD Map File") {
    return false;  //not a deled file
  }
  temp.clear();
  temp = SubdivideString(RawFile[1], " "); //get version
  temp1 = SubdivideString(temp[1], ";");
  if (atof(temp1[0]) < 0.91) {
    return false;  //not correct version
  }
  //end checking
  temp.clear();
  temp1.clear();
  int i;
  for (i = 0; i < num_material; i++) {
    temp = SubdivideString(RawFile[offs + i], ";");
    materials[i].materialID = i;
    temp1 = SubdivideString(temp[5], ",");
    materials[i].textureFlag = atoi(temp1[0]);
    temp2 = SubdivideString(temp1[1], "\\");
    materials[i].textureName = temp2.getLast();
    temp1.clear();
    temp2.clear();
    int a = temp_size;
    if (a == 7) {
      temp1 = SubdivideString(temp[6], ",");
      materials[i].lightmapFlag = atoi(temp1[0]);
      temp2 = SubdivideString(temp1[1], "\\");
      materials[i].lightmapName = temp2.getLast();
    }
    else {
      materials[i].lightmapFlag = 1;
      materials[i].lightmapName = "FFFFFFFF";
    }
    temp1.clear();
    temp2.clear();
  }
  return true;
}
/**This function extract an array of dmfVert and dmfFace from a DMF file.
You must give in input a StringList representing a DMF file loaded with LoadFromFile and two arrays long enough.
Please use GetDMFHeader() before this function to know number of vertices and faces.
\return true if function succeed or false on fail.*/
bool GetDMFVerticesFaces(const StringList& RawFile/**<StringList representing a DMF file.*/,
    dmfVert vertices[]/**<Vertices returned*/,
    dmfFace faces[]/**Faces returned*/
                        )
{
  StringList temp, temp1;
  // skip materials
  s32 offs = 4 + atoi(RawFile[3]);
  const s32 objs = atoi(RawFile[offs]);
  offs++;
#ifdef _IRR_DMF_DEBUG_
  os::Printer::log("Reading objects", char * (objs));
#endif
  s32 vert_cnt = 0, face_cnt = 0;
  int i;
  for (i = 0; i < objs; ++i) {
    StringList wat = SubdivideString(RawFile[offs], ";");
    StringList wat1 = SubdivideString(wat[0], "_");
#ifdef _IRR_DMF_DEBUG_
    os::Printer::log("Reading object", wat[0]);
#endif
    offs++;
    // load vertices
    float3 > pos;
    const u32 posCount = strtoul10(RawFile[offs]);
    ++offs;
    pos.reallocate(posCount);
    int i;
    for (i = 0; i < posCount; ++i) {
      temp1 = SubdivideString(RawFile[offs], ";");
      pos.push_back(f3VECTOR(fast_atof(temp1[0]),
          fast_atof(temp1[1]),
          -fast_atof(temp1[2])));
      ++offs;
    }
    const u32 numFaces = strtoul10(RawFile[offs]);
    offs++;
    if (!(wat1[0] == "water" && wat[2] == "0")) {
      for (int j = 0; j < numFaces; ++j) {
        temp = SubdivideString(RawFile[offs + j], ";");
        //first value is vertices number for this face
        const u32 vert = strtoul10(temp[0]);
        faces[face_cnt].numVerts = vert;
        //second is material ID
        faces[face_cnt].materialID = strtoul10(temp[1]);
        //vertices are ordined
        faces[face_cnt].firstVert = vert_cnt;
        //now we'll create vertices structure
        for (int k = 0; k < vert; ++k) {
          //copy position
          vertices[vert_cnt].pos.set(pos[strtoul10(temp[2 + k])]);
          //get uv coords for tex and light if any
          vertices[vert_cnt].tc.set(fast_atof(temp[2 + vert + (2 * k)]),
              fast_atof(temp[2 + vert + (2 * k) + 1]));
          const u32 tmp_sz = temp_size;
          vertices[vert_cnt].lc.set(fast_atof(temp[tmp_sz - (2 * vert) + (2 * k)]),
              fast_atof(temp[tmp_sz - (2 * vert) + (2 * k) + 1]));
          vert_cnt++;
        }
        face_cnt++;
      }
    }
    offs += numFaces;
  }
  return true;
}
/**This function extract an array of dmfLights from a DMF file.
You must give in input a StringList representing a DMF file loaded with
LoadFromFile and one array long enough.  Please use GetDMFHeader() before this
function to know number of dynamic lights.
\return true if function succeed or false on fail.*/
bool GetDMFLights(const StringList& RawFile/**<StringList representing a DMF file.*/,
    dmfLight lights[]/**<Lights returned.*/
                 )
{
  int offs = 3;
  StringList temp, temp1;
  //Checking if this is a DeleD map File of version >= 0.91
  temp = SubdivideString(RawFile[0], ";"); //file info
  if (temp[0] != "DeleD Map File") {
    return false;  //not a deled file
  }
  temp.clear();
  temp = SubdivideString(RawFile[1], " "); //get version
  temp1 = SubdivideString(temp[1], ";");
  if (atof(temp1[0]) < 0.91) {
    return false;  //not correct version
  }
  //end checking
  temp.clear();
  temp1.clear();
  offs = offs + atoi(RawFile[offs]);
  offs++;
  s32 objs = atoi(RawFile[offs]);
  s32 lit = 0;
  s32 d_lit = 0;
  offs++;
  //let's get position of lights in file
  int i;
  for (i = 0; i < objs; i++) {
    offs++;
    offs = offs + atoi(RawFile[offs]);
    offs++;
    offs = offs + atoi(RawFile[offs]);
    offs++;
  }
  //let's find dynamic lights
  lit = atoi(RawFile[offs]);
  for (i = 0; i < lit; i++) {
    offs++;
    temp = SubdivideString(RawFile[offs], ";");
    if (atoi(temp[0]) == 1) {
      temp1 = SubdivideString(temp[18], "_");
      if (temp1[0] == "dynamic") {
        lights[d_lit].radius = (float)atof(temp[4]);
        lights[d_lit].pos.set((float)atof(temp[5]),
            (float)atof(temp[6]),
            (float) - atof(temp[7]));
        lights[d_lit].diffuseColor = SColorf(
            SColor(255, atoi(temp[10]), atoi(temp[11]),
                atoi(temp[12])));
        lights[d_lit].specularColor = SColorf(
            SColor(255, atoi(temp[13]), atoi(temp[14]),
                atoi(temp[15])));
        d_lit++;
      }
    }
    temp.clear();
    temp1.clear();
  }
  return true;
}
/**This function extracts an array of dmfWaterPlane,dmfVert and dmfFace from a DMF file.
You must give in input a StringList representing a DMF file loaded with LoadFromFile and three arrays long enough.
Please use GetDMFHeader() before this function to know number of water plains and water faces as well as water vertices.
\return true if function succeed or false on fail.*/
bool GetDMFWaterPlanes(const StringList& RawFile/**<StringList representing a DMF file.*/,
    dmfWaterPlane wat_planes[]/**<Water planes returned.*/,
    dmfVert vertices[]/**<Vertices returned*/,
    dmfFace faces[]/**Faces returned*/
                      )
{
  int offs = 3;
  int offs1 = 0;
  StringList temp, temp1;
  //Checking if this is a DeleD map File of version >= 0.91
  temp = SubdivideString(RawFile[0], ";"); //file info
  if (temp[0] != "DeleD Map File") {
    return false;  //not a deled file
  }
  temp.clear();
  temp = SubdivideString(RawFile[1], " "); //get version
  temp1 = SubdivideString(temp[1], ";");
  if (atof(temp1[0]) < 0.91) {
    return false;  //not correct version
  }
  //end checking
  temp.clear();
  temp1.clear();
  offs = offs + atoi(RawFile[offs]);
  offs++;
  s32 objs = atoi(RawFile[offs]);
  s32 fac = 0, vert = 0, tmp_sz = 0, vert_cnt = 0, face_cnt = 0, wat_id = 0;
  dimension2du tilenum(40, 40);
  f32 waveheight = 3.0f;
  f32 wavespeed = 300.0f;
  f32 wavelength = 80.0f;
  offs++;
  int i;
  for (i = 0; i < objs; i++) {
    StringList wat = SubdivideString(RawFile[offs], ";");
    StringList wat1 = SubdivideString(wat[0], "_");
    offs++;
    offs1 = offs;
    offs = offs + atoi(RawFile[offs]);
    offs++;
    offs1++;
    fac = atoi(RawFile[offs]);
    offs++;
    if (wat1[0] == "water" && wat[2] == "0") {
      StringList userinfo = SubdivideString(wat[7], ",");
      int j;
      for (j = 0; j < (int)userinfo_size; j++) {
        switch (j) {
        case 0:
          if (atoi(userinfo[0])) {
            tilenum.Width = atoi(userinfo[0]);
          }
          break;
        case 1:
          if (atoi(userinfo[1])) {
            tilenum.Height = atoi(userinfo[1]);
          }
          break;
        case 2:
          if (atof(userinfo[2])) {
            waveheight = (float)atof(userinfo[2]);
          }
          break;
        case 3:
          if (atof(userinfo[3])) {
            wavespeed = (float)atof(userinfo[3]);
          }
          break;
        case 4:
          if (atof(userinfo[4])) {
            wavelength = (float)atof(userinfo[4]);
          }
          break;
        }
      }
      wat_planes[wat_id].waterID = wat_id;
      wat_planes[wat_id].numFaces = fac;
      wat_planes[wat_id].firstFace = face_cnt;
      wat_planes[wat_id].tileNum = tilenum;
      wat_planes[wat_id].waveHeight = waveheight;
      wat_planes[wat_id].waveSpeed = wavespeed;
      wat_planes[wat_id].waveLength = wavelength;
      for (j = 0; j < fac; j++) {
        temp = SubdivideString(RawFile[offs + j], ";");
        //first value is vertices number for this face
        faces[face_cnt].numVerts = atoi(temp[0]);
        vert = faces[face_cnt].numVerts;
        //second is material ID
        faces[face_cnt].materialID = atoi(temp[1]);
        //vertices are ordined
        faces[face_cnt].firstVert = vert_cnt;
        //now we'll create vertices structure
        for (int k = 0; k < vert; k++) {
          //get vertex position
          temp1 = SubdivideString(RawFile[offs1 + atoi(temp[2 + k])], ";");
          //copy x,y,z values
          vertices[vert_cnt].pos.set((float)atof(temp1[0]),
              (float)atof(temp1[1]),
              (float) - atof(temp1[2]));
          //get uv coords for tex and light if any
          vertices[vert_cnt].tc.set((float)atof(temp[2 + vert + (2 * k)]),
              (float)atof(temp[2 + vert + (2 * k) + 1]));
          tmp_sz = temp_size;
          vertices[vert_cnt].lc.set((float)atof(temp[tmp_sz - (2 * vert) + (2 * k)]),
              (float)atof(temp[tmp_sz - (2 * vert) + (2 * k) + 1]));
          ++vert_cnt;
          temp1.clear();
        }
        ++face_cnt;
        temp.clear();
      }
    }
    offs = offs + fac;
  }
  return true;
}
// end
#endif /* __DMF_SUPPORT_H__ */

