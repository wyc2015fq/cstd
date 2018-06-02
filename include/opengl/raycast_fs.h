//VTK::System::Dec

//////////////////////////////////////////////////////////////////////////////
///
/// Inputs
///
//////////////////////////////////////////////////////////////////////////////

/// 3D texture coordinates form vertex shader
varying vec3 ip_textureCoords;
varying vec3 ip_vertexPos;

//////////////////////////////////////////////////////////////////////////////
///
/// Outputs
///
//////////////////////////////////////////////////////////////////////////////

vec4 g_fragColor = vec4(0.0);

//////////////////////////////////////////////////////////////////////////////
///
/// Uniforms, attributes, and globals
///
//////////////////////////////////////////////////////////////////////////////
vec3 g_dataPos;
vec3 g_dirStep;
vec4 g_srcColor;
vec4 g_eyePosObj;
bool g_exit;

uniform vec4 in_volume_scale;
uniform vec4 in_volume_bias;

//VTK::Output::Dec


// Volume dataset
uniform sampler3D in_volume;
uniform int in_noOfComponents;
uniform int in_independentComponents;

uniform sampler2D in_noiseSampler;
#ifndef GL_ES
uniform sampler2D in_depthSampler;
#endif

// Camera position
uniform vec3 in_cameraPos;

// view and model matrices
uniform mat4 in_volumeMatrix;
uniform mat4 in_inverseVolumeMatrix;
uniform mat4 in_projectionMatrix;
uniform mat4 in_inverseProjectionMatrix;
uniform mat4 in_modelViewMatrix;
uniform mat4 in_inverseModelViewMatrix;
uniform mat4 in_textureDatasetMatrix;
uniform mat4 in_inverseTextureDatasetMatrix;
varying mat4 ip_inverseTextureDataAdjusted;
uniform vec3 in_texMin;
uniform vec3 in_texMax;
uniform mat4 in_texureToEyeIt;

// Ray step size
uniform vec3 in_cellStep;
uniform vec2 in_scalarsRange[4];
uniform vec3 in_cellSpacing;

// Sample distance
uniform float in_sampleDistance;

// Scales
uniform vec3 in_cellScale;
uniform vec2 in_windowLowerLeftCorner;
uniform vec2 in_inverseOriginalWindowSize;
uniform vec2 in_inverseWindowSize;
uniform vec3 in_textureExtentsMax;
uniform vec3 in_textureExtentsMin;

// Material and lighting
uniform vec3 in_diffuse[4];
uniform vec3 in_ambient[4];
uniform vec3 in_specular[4];
uniform float in_shininess[4];

// Others
uniform bool in_cellFlag;
uniform bool in_useJittering;
uniform bool in_clampDepthToBackface;

uniform vec2 in_averageIPRange;
uniform vec3 in_lightAmbientColor[1];
uniform vec3 in_lightDiffuseColor[1];
uniform vec3 in_lightSpecularColor[1];
vec4 g_lightPosObj;
vec3 g_ldir;
vec3 g_vdir;
vec3 g_h;

//VTK::Termination::Dec

//VTK::Cropping::Dec

//VTK::Shading::Dec

//VTK::BinaryMask::Dec

//VTK::CompositeMask::Dec


uniform sampler2D in_opacityTransferFunc;
float computeOpacity(vec4 scalar)
{
  return texture2D(in_opacityTransferFunc, vec2(scalar.w, 0)).r;
}


vec4 computeGradient(int component)
{
  return vec4(0.0);
}


vec4 computeLighting(vec4 color, int component)
{
  vec4 finalColor = vec4(0.0);
  finalColor = vec4(color.rgb, 0.0);
  finalColor.a = color.a;
  return finalColor;
}


uniform sampler2D in_colorTransferFunc;
vec4 computeColor(vec4 scalar, float opacity)
{
  return computeLighting(vec4(texture2D(in_colorTransferFunc,
      vec2(scalar.w, 0.0)).xyz, opacity), 0);
}


vec3 computeRayDirection()
{
  return normalize(ip_vertexPos.xyz - g_eyePosObj.xyz);
}

//VTK::Picking::Dec

/// We support only 8 clipping planes for now
/// The first value is the size of the data array for clipping
/// planes (origin, normal)
uniform float in_clippingPlanes[49];
uniform float in_scale;
uniform float in_bias;

//////////////////////////////////////////////////////////////////////////////
///
/// Main
///
//////////////////////////////////////////////////////////////////////////////
void main()
{
  /// Initialize g_fragColor (output) to 0
  g_fragColor = vec4(0.0);
  g_dirStep = vec3(0.0);
  g_srcColor = vec4(0.0);
  g_exit = false;


  bool l_adjustTextureExtents =  !in_cellFlag;
  // Get the 3D texture coordinates for lookup into the in_volume dataset
  g_dataPos = ip_textureCoords.xyz;

  // Eye position in dataset space
  g_eyePosObj = (in_inverseVolumeMatrix * vec4(in_cameraPos, 1.0));

  if (g_eyePosObj.w != 0.0) {
    g_eyePosObj.x /= g_eyePosObj.w;
    g_eyePosObj.y /= g_eyePosObj.w;
    g_eyePosObj.z /= g_eyePosObj.w;
    g_eyePosObj.w = 1.0;
  }

  // Getting the ray marching direction (in dataset space);
  vec3 rayDir = computeRayDirection();

  // Multiply the raymarching direction with the step size to get the
  // sub-step size we need to take at each raymarching step
  g_dirStep = (ip_inverseTextureDataAdjusted *
      vec4(rayDir, 0.0)).xyz * in_sampleDistance;

  // 2D Texture fragment coordinates [0,1] from fragment coordinates.
  // The frame buffer texture has the size of the plain buffer but
  // we use a fraction of it. The texture coordinate is less than 1 if
  // the reduction factor is less than 1.
  // Device coordinates are between -1 and 1. We need texture
  // coordinates between 0 and 1. The in_noiseSampler and in_depthSampler
  // buffers have the original size buffer.
  vec2 fragTexCoord = (gl_FragCoord.xy - in_windowLowerLeftCorner) *
      in_inverseWindowSize;

  float jitterValue = 0;

  if (in_useJittering) {
    jitterValue = texture2D(in_noiseSampler, fragTexCoord).x;
    g_dataPos += g_dirStep * jitterValue;
  }
  else {
    g_dataPos += g_dirStep;
  }

  // Flag to deternmine if voxel should be considered for the rendering
  bool l_skip = false;


  // Flag to indicate if the raymarch loop should terminate
  bool stop = false;

  float l_terminatePointMax = 0.0;

#ifdef GL_ES
  vec4 l_depthValue = vec4(1.0, 1.0, 1.0, 1.0);
#else
  vec4 l_depthValue = texture2D(in_depthSampler, fragTexCoord);
#endif

  // Depth test
  if (gl_FragCoord.z >= l_depthValue.x) {
    discard;
  }

  // color buffer or max scalar buffer have a reduced size.
  fragTexCoord = (gl_FragCoord.xy - in_windowLowerLeftCorner) *
      in_inverseOriginalWindowSize;

  // Compute max number of iterations it will take before we hit
  // the termination point

  // Abscissa of the point on the depth buffer along the ray.
  // point in texture coordinates
  vec4 terminatePoint;
  terminatePoint.x = (gl_FragCoord.x - in_windowLowerLeftCorner.x) * 2.0 *
      in_inverseWindowSize.x - 1.0;
  terminatePoint.y = (gl_FragCoord.y - in_windowLowerLeftCorner.y) * 2.0 *
      in_inverseWindowSize.y - 1.0;
  terminatePoint.z = (2.0 * l_depthValue.x - (gl_DepthRange.near +
      gl_DepthRange.far)) / gl_DepthRange.diff;
  terminatePoint.w = 1.0;

  // From normalized device coordinates to eye coordinates.
  // in_projectionMatrix is inversed because of way VT
  // From eye coordinates to texture coordinates
  terminatePoint = ip_inverseTextureDataAdjusted *
      in_inverseVolumeMatrix *
      in_inverseModelViewMatrix *
      in_inverseProjectionMatrix *
      terminatePoint;
  terminatePoint /= terminatePoint.w;

  l_terminatePointMax = length(terminatePoint.xyz - g_dataPos.xyz) /
      length(g_dirStep);
  float l_currentT = 0.0;


  // We get data between 0.0 - 1.0 range
  bool l_firstValue = true;
  vec4 l_maxValue = vec4(0.0);

  //VTK::Cropping::Init

  //VTK::Clipping::Init

  //VTK::RenderToImage::Init

  //VTK::DepthPass::Init

  /// For all samples along the ray
  while (!g_exit) {

    l_skip = false;

    //VTK::Cropping::Impl

    //VTK::Clipping::Impl

    //VTK::BinaryMask::Impl

    //VTK::CompositeMask::Impl


    if (!l_skip) {
      vec4 scalar = texture3D(in_volume, g_dataPos);
      scalar.r = scalar.r * in_volume_scale.r + in_volume_bias.r;
      scalar = vec4(scalar.r, scalar.r, scalar.r, scalar.r);

      if (l_maxValue.w < scalar.x || l_firstValue) {
        l_maxValue.w = scalar.x;
      }

      if (l_firstValue) {
        l_firstValue = false;
      }
    }

    //VTK::RenderToImage::Impl

    //VTK::DepthPass::Impl

    /// Advance ray
    g_dataPos += g_dirStep;


    if (any(greaterThan(g_dataPos, in_texMax)) ||
        any(lessThan(g_dataPos, in_texMin))) {
      break;
    }

    // Early ray termination
    // if the currently composited colour alpha is already fully saturated
    // we terminated the loop or if we have hit an obstacle in the
    // direction of they ray (using depth buffer) we terminate as well.
    if ((g_fragColor.a > (1.0 - 1.0 / 255.0)) ||
        l_currentT >= l_terminatePointMax) {
      break;
    }

    ++l_currentT;
  }

  //VTK::Base::Exit

  //VTK::Terminate::Exit

  //VTK::Cropping::Exit

  //VTK::Clipping::Exit


  g_srcColor = computeColor(l_maxValue,
      computeOpacity(l_maxValue));
  g_fragColor.rgb = g_srcColor.rgb * g_srcColor.a;
  g_fragColor.a = g_srcColor.a;

  //VTK::Picking::Exit

  g_fragColor.r = g_fragColor.r * in_scale + in_bias * g_fragColor.a;
  g_fragColor.g = g_fragColor.g * in_scale + in_bias * g_fragColor.a;
  g_fragColor.b = g_fragColor.b * in_scale + in_bias * g_fragColor.a;
  gl_FragData[0] = g_fragColor;

  //VTK::RenderToImage::Exit

  //VTK::DepthPass::Exit
}

