{
typedef sVec4 vec4;
typedef sVec3 vec3;
typedef sVec2 vec2;
#define in
#define uniform
#define attribute
#define varying
#ifdef _MSC_VER
#pragma warning(disable:4244)
#endif
struct mat4 {
  float m[4][4];
  vec4 operator* (const vec4& in) const {
    vec4 out;
    return out;
  }
};
struct mat3 {
  float m[3][3];
  vec3 operator* (const vec3& in) const {
    vec3 out;
    return out;
  }
};
const int gl_MaxLights = 8;
CV_INLINE float dot(float x, float y)
{
  return x * y;
}
CV_INLINE float dot(const vec2& x, const vec2& y)
{
  return x.x * y.x + x.y * y.y;
}
CV_INLINE float dot(const vec3& x, const vec3& y)
{
  return x.x * y.x + x.y * y.y + x.z * y.z;
}
CV_INLINE float dot(const vec4& x, const vec4& y)
{
  return x.x * y.x + x.y * y.y + x.z * y.z + x.w * y.w;
}
CV_INLINE float reflect(float I, float N)
{
  return I - 2.0 * dot(N, I) * N;
}
CV_INLINE vec2 reflect(const vec2& I, const vec2& N)
{
  return I - N * 2.0 * dot(N, I);
}
CV_INLINE vec3 reflect(const vec3& I, const vec3& N)
{
  return I - N * 2.0 * dot(N, I);
}
CV_INLINE vec4 reflect(const vec4& I, const vec4& N)
{
  return I - N * 2.0 * dot(N, I);
}
CV_INLINE float refract(float I, float N, float eta)
{
  const float k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
  if (k < 0.0) {
    return 0.0;
  }
  return eta * I - (eta * dot(N, I) + sqrt(k)) * N;
}
CV_INLINE vec2 refract(const vec2& I, const vec2& N, float eta)
{
  const float k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
  if (k < 0.0) {
    return vec2(0.0);
  }
  return I * eta - N * (eta * dot(N, I) + sqrt(k));
}
CV_INLINE vec3 refract(const vec3& I, const vec3& N, float eta)
{
  const float k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
  if (k < 0.0) {
    return vec3(0.0);
  }
  return I * eta - N * (eta * dot(N, I) + sqrt(k));
}
CV_INLINE vec4 refract(const vec4& I, const vec4& N, float eta)
{
  const float k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
  if (k < 0.0) {
    return vec4(0.0);
  }
  return I * eta - N * (eta * dot(N, I) + sqrt(k));
}
#undef max
#undef min
CV_INLINE float length(const vec3& v)
{
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
vec3 normalize(const vec3& v)
{
  float l = 1.f / length(v);
  return vec3(v.x * l, v.y * l, v.z * l);
}
float max(float a, float b)
{
  return a > b ? a : b;
}
float min(float a, float b)
{
  return a < b ? a : b;
}
vec4 clamp(const vec4& a, f32 low, f32 high)
{
  return vec4(min(max(a.x, low), high), min(max(a.y, low), high), min(max(a.z, low), high), min(max(a.w, low), high));
}
typedef int sampler2D;
sampler2D texUnit0;
vec4 texture2D(sampler2D sampler, const vec2& coord)
{
  return vec4(0.0);
}
struct gl_LightSourceParameters {
  vec4 ambient;              // Acli
  vec4 diffuse;              // Dcli
  vec4 specular;             // Scli
  vec4 position;             // Ppli
  vec4 halfVector;           // Derived: Hi
  vec3 spotDirection;        // Sdli
  float spotExponent;        // Srli
  float spotCutoff;          // Crli
  // (range: [0.0,90.0], 180.0)
  float spotCosCutoff;       // Derived: cos(Crli)
  // (range: [1.0,0.0],-1.0)
  float constantAttenuation; // K0
  float linearAttenuation;   // K1
  float quadraticAttenuation;// K2
};
uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];
struct gl_LightModelParameters {
  vec4 ambient;
};
uniform gl_LightModelParameters gl_LightModel;
struct gl_LightModelProducts {
  vec4 sceneColor;
};
uniform gl_LightModelProducts gl_FrontLightModelProduct;
uniform gl_LightModelProducts gl_BackLightModelProduct;
struct gl_LightProducts {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
uniform gl_LightProducts gl_FrontLightProduct[gl_MaxLights];
uniform gl_LightProducts gl_BackLightProduct[gl_MaxLights];
struct gl_MaterialParameters {
  vec4 emission;    // Ecm
  vec4 ambient;     // Acm
  vec4 diffuse;     // Dcm
  vec4 specular;    // Scm
  float shininess;  // Srm
};
uniform gl_MaterialParameters gl_FrontMaterial;
uniform gl_MaterialParameters gl_BackMaterial;
// GLSL has some built-in attributes in a vertex shader:
attribute vec4 gl_Vertex;     // 4D vector representing the vertex position
attribute vec3 gl_Normal;     // 3D vector representing the vertex normal
attribute vec4 gl_Color;      // 4D vector representing the vertex color
attribute vec4 gl_MultiTexCoord0; // 4D vector representing the texture coordinate of texture unit X
attribute vec4 gl_MultiTexCoord1; // 4D vector representing the texture coordinate of texture unit X
uniform mat4 gl_ModelViewMatrix;      //4x4 Matrix representing the model-view matrix.
uniform mat4 gl_ModelViewProjectionMatrix;  //4x4 Matrix representing the model-view-projection matrix.
uniform mat3 gl_NormalMatrix;       //3x3 Matrix representing the inverse transpose model-view matrix. This matrix is used for normal transformation.
varying vec4 gl_FrontColor;       // 4D vector representing the primitives front color
varying vec4 gl_FrontSecondaryColor;  // 4D vector representing the primitives second front color
varying vec4 gl_BackColor;        // 4D vector representing the primitives back color
varying vec4 gl_TexCoord[4];      // 4D vector representing the Xth texture coordinate
// shader output
varying vec4 gl_Position;       // 4D vector representing the final processed vertex position. Only  available in vertex shader.
varying vec4 gl_FragColor;        // 4D vector representing the final color which is written in the frame buffer. Only available in fragment shader.
varying float gl_FragDepth;       // float representing the depth which is written in the depth buffer. Only available in fragment shader.
varying vec4 gl_SecondaryColor;
varying float gl_FogFragCoord;
vec4 ftransform(void)
{
  return gl_ModelViewProjectionMatrix * gl_Vertex;
}
vec3 fnormal(void)
{
  //Compute the normal
  vec3 normal = gl_NormalMatrix * gl_Normal;
  normal = normalize(normal);
  return normal;
}
struct program1 {
  vec4 Ambient;
  vec4 Diffuse;
  vec4 Specular;
  void pointLight(in int i, in vec3 normal, in vec3 eye, in vec3 ecPosition3) {
    float nDotVP;       // normal . light direction
    float nDotHV;       // normal . light half vector
    float pf;           // power factor
    float attenuation;  // computed attenuation factor
    float d;            // distance from surface to light source
    vec3  VP;           // direction from surface to light position
    vec3  halfVector;   // direction of maximum highlights
    // Compute vector from surface to light position
    VP = vec3(gl_LightSource[i].position) - ecPosition3;
    // Compute distance between surface and light position
    d = length(VP);
    // Normalize the vector from surface to light position
    VP = normalize(VP);
    // Compute attenuation
    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
        gl_LightSource[i].linearAttenuation * d +
        gl_LightSource[i].quadraticAttenuation * d * d);
    halfVector = normalize(VP + eye);
    nDotVP = max(0.0, dot(normal, VP));
    nDotHV = max(0.0, dot(normal, halfVector));
    if (nDotVP == 0.0) {
      pf = 0.0;
    }
    else {
      pf = pow(nDotHV, gl_FrontMaterial.shininess);
    }
    Ambient  += gl_LightSource[i].ambient * attenuation;
    Diffuse  += gl_LightSource[i].diffuse * nDotVP * attenuation;
    Specular += gl_LightSource[i].specular * pf * attenuation;
  }
  vec3 fnormal(void) {
    //Compute the normal
    vec3 normal = gl_NormalMatrix * gl_Normal;
    normal = normalize(normal);
    return normal;
  }
  void ftexgen(in vec3 normal, in vec4 ecPosition) {
    gl_TexCoord[0] = gl_MultiTexCoord0;
  }
  void flight(in vec3 normal, in vec4 ecPosition, float alphaFade) {
    vec4 color;
    vec3 ecPosition3;
    vec3 eye;
    ecPosition3 = (vec3(ecPosition.x, ecPosition.y, ecPosition.z)) / ecPosition.w;
    eye = vec3(0.0, 0.0, 1.0);
    // Clear the light intensity accumulators
    Ambient  = vec4(0.0);
    Diffuse  = vec4(0.0);
    Specular = vec4(0.0);
    pointLight(0, normal, eye, ecPosition3);
    pointLight(1, normal, eye, ecPosition3);
    color = gl_FrontLightModelProduct.sceneColor +
        Ambient  * gl_FrontMaterial.ambient +
        Diffuse  * gl_FrontMaterial.diffuse;
    gl_FrontSecondaryColor = Specular * gl_FrontMaterial.specular;
    color = clamp(color, 0.0, 1.0);
    gl_FrontColor = color;
    gl_FrontColor.a *= alphaFade;
  }
  void vertexshader_main(void) {
    vec3  transformedNormal;
    float alphaFade = 1.0;
    // Eye-coordinate position of vertex, needed in various calculations
    vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
    // Do fixed functionality vertex transform
    gl_Position = ftransform();
    transformedNormal = fnormal();
    flight(transformedNormal, ecPosition, alphaFade);
    ftexgen(transformedNormal, ecPosition);
  }
  void fragmentshader_main(void) {
    vec4 color;
    color = gl_Color;
    color *= texture2D(texUnit0, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y));
    color += gl_SecondaryColor;
    color = clamp(color, 0.0, 1.0);
    gl_FragColor = color;
  }
};
}