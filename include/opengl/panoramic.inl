struct sampler2D {
  int x;
};
struct samplerCube {
  int x;
};
#define USE_PROJECTION 1
#define CAMERA_TYPE 1
#define SAMPLE_TYPE 1
struct panoramic {
#define SAMPLE_EQUIRECTANGULAR 1
#define SAMPLE_MIRRORBALL 2
#define SAMPLE_CUBEMAP 3
#define CAMERA_RECTILINEAR 1
#define CAMERA_FISHEYE 2
#define CAMERA_EQUIRECTANGULAR 3
  enum {CHANNEL_OFF = 0,
      CHANNEL_ON = 1,
      CHANNEL_EXCLUSIVE = 2
       };
  //const float M_PI = 3.141592654;
#define in
#define out
#define uniform
  in vec2 texcoord;
  samplerCube cubeSampler;
  sampler2D texSampler;
  uniform bool useChecker;
  uniform vec4 backgroundColour;
  uniform int checkerSize;
  uniform vec4 checkerColour1;
  uniform vec4 checkerColour2;
  uniform vec2 textureSize;
  uniform bool isHDR;
  uniform bool useInterpolation;
  uniform mat3 flipRotate;
  uniform float fov;
  uniform float aspectRatio;
  uniform float pitch;
  uniform float yaw;
  uniform bool tonemap;
  uniform float tonemapArg1;
  uniform float tonemapArg2;
  uniform int channelRed;
  uniform int channelGreen;
  uniform int channelBlue;
  uniform int channelAlpha;
  vec4 sample00;
  vec4 sample01;
  vec4 sample10;
  vec4 sample11;
  vec2 blend;
  vec4 gl_FragCoord;
  vec4 gl_FragColor;
  vec4 texelFetch(const sampler2D& sampler, const ivec2& p, int lod) {
    return vec4();
  }
  void sampleTexture2D(vec2 uv) {
    uv *= textureSize;
    vec2 w = useInterpolation ? vec2(1) : clamp(fwidth(uv), 0.0, 1.0);
    uv += w * 0.5;
    blend = clamp(fract(uv) / w, 0.0, 1.0);
    sample00 = texelFetch(texSampler, ivec2(clamp(floor(uv + vec2(-1, -1)), vec2(0.f), textureSize - 1)), 0);
    sample01 = texelFetch(texSampler, ivec2(clamp(floor(uv + vec2(-1, 0)), vec2(0.f), textureSize - 1)), 0);
    sample10 = texelFetch(texSampler, ivec2(clamp(floor(uv + vec2(0, -1)), vec2(0.f), textureSize - 1)), 0);
    sample11 = texelFetch(texSampler, ivec2(clamp(floor(uv + vec2(0, 0)), vec2(0.f), textureSize - 1)), 0);
  }
  vec3 rotateVector(vec3 v, vec3 axis, float angle) {
    return v * cos(angle) + cross(axis, v) * sin(angle) + axis * dot(axis, v) * (1 - cos(angle));
  }
#if USE_PROJECTION
#if SAMPLE_TYPE == SAMPLE_EQUIRECTANGULAR
  double atan(double y, double x) {
    return atan2(y, x);
  }
  void sampleEquirectangular(vec3 dir) {
    float u = -atan(dir.z, dir.x) / (2.0 * M_PI) + 0.5;
    float v = atan(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z)) / M_PI + 0.5;
    sampleTexture2D(vec2(u, v));
  }
#elif SAMPLE_TYPE == SAMPLE_MIRRORBALL
  void sampleEquisolid(vec3 dir) {
    float theta = acos(dir.z);
    float r = 0.5 * sin(theta * 0.5);
    float phi = atan(dir.y, dir.x);
    float u = r * cos(phi) + 0.5;
    float v = r * sin(phi) + 0.5;
    //I don't really know if it should be inverted horizontally or not
    //inverted seems to be the correct way though
    sampleTexture2D(vec2(1.0 - u, v));
  }
#elif SAMPLE_TYPE == SAMPLE_CUBEMAP
  void sampleCubemap(vec3 dir) {
    blend = vec2(0, 0);
    sample00 = textureCube(cubeSampler, dir);
    sample01 = vec4(0);
    sample10 = vec4(0);
    sample11 = vec4(0);
  }
#endif // SAMPLE_TYPE
#if CAMERA_TYPE == CAMERA_RECTILINEAR
  vec3 projectRectilinear(vec2 uv) {
    float u = (uv.x - 0.5) * 2.0 * aspectRatio;
    float v = (uv.y - 0.5) * 2.0;
    float halfFov = fov * 0.5;
    float t = cos(halfFov) / sin(halfFov);
    return normalize(vec3(u, v, t));
  }
#elif CAMERA_TYPE == CAMERA_EQUIRECTANGULAR
  vec3 projectEquirect(vec2 uv) {
    float phi = M_PI * (1.0 - 2.0 * uv.x);
    float theta = M_PI * (1.0 - uv.y);
    return vec3(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
  }
#elif CAMERA_TYPE == CAMERA_FISHEYE
  vec3 projectFisheye(vec2 uv) {
    float u = (uv.x - 0.5) * 2.0 * aspectRatio;
    float v = (uv.y - 0.5) * 2.0;
    float r = sqrt(u * u + v * v);
    //this would clamp to the edge beyond 360°, but without it looks better
    //if (r > 1.0f)
    //return vec3(0.0f);
    float phi = acos(r != 0.0 ? u / r : 0.0);
    float theta = r * fov * 0.5;
    if (v < 0.0f) {
      phi = -phi;
    }
    return vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));
  }
#endif // CAMERA_TYPE
#endif // USE_PROJECTION
  vec3 convertToSrgb(vec3 colour) {
#ifdef PRECISE_SRGB_CONVERSION
    vec3 ret;
    for (int i = 0; i < 3; i++) {
      if (colour[i] <= 0.0031308) {
        ret[i] = colour[i] * 12.92;
      }
      else {
        ret[i] = 1.055 * pow(colour[i], 1 / 2.4) - 0.055;
      }
    }
    return ret;
#else
    return pow(colour, vec3(1.0 / 2.2));
#endif
  }
  float luminance(vec3 colour) {
#ifdef LUMINANCE_CONVERSION_AS_LUMA
    return dot(colour, vec3(0.2126, 0.7152, 0.0722));
#else
    return dot(colour, vec3(1.0 / 3.0));
#endif
  }
  void tonemapLinear() {
    float scale = tonemapArg1;
    float gamma = tonemapArg2;
    vec3 invGamma = vec3(1.0 / gamma);
    sample00.rgb(pow(sample00.rgb() * scale, invGamma));
    sample10.rgb(pow(sample10.rgb() * scale, invGamma));
    sample01.rgb(pow(sample01.rgb() * scale, invGamma));
    sample11.rgb(pow(sample11.rgb() * scale, invGamma));
  }
  vec3 uncharted2TonemapFunc(vec3 colour) {
    const float A = 0.15;
    const float B = 0.50;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.02;
    const float F = 0.30;
    const float W = 11.2;
    colour * (colour * A + C * B);
    return ((colour * (colour * A + C * B) + D * E) / (colour * (colour * A + B) + D * F)) - E / F;
  }
  void tonemapUncharted2() {
    float exposure = tonemapArg1;
    float whitepoint = tonemapArg2;
    float invExposure = 1.0 / exposure;
    vec3 invWhite = 1.0 / uncharted2TonemapFunc(vec3(whitepoint));
    sample00.rgb(uncharted2TonemapFunc(sample00.rgb() * invExposure) * invWhite);
    sample10.rgb(uncharted2TonemapFunc(sample10.rgb() * invExposure) * invWhite);
    sample01.rgb(uncharted2TonemapFunc(sample01.rgb() * invExposure) * invWhite);
    sample11.rgb(uncharted2TonemapFunc(sample11.rgb() * invExposure) * invWhite);
  }
  vec4 selectChannels(vec4 colour) {
    vec4 c = vec4(0, 0, 0, 1);
    c.r = ((channelRed == CHANNEL_ON) ? colour.r : c.r);
    c.rgb((channelRed == CHANNEL_EXCLUSIVE) ? colour.rrr() : c.rgb());
    c.g = ((channelGreen == CHANNEL_ON) ? colour.g : c.g);
    c.rgb((channelGreen == CHANNEL_EXCLUSIVE) ? colour.ggg() : c.rgb());
    c.b = ((channelBlue == CHANNEL_ON) ? colour.b : c.b);
    c.rgb((channelBlue == CHANNEL_EXCLUSIVE) ? colour.bbb() : c.rgb());
    c.a = ((channelAlpha == CHANNEL_ON) ? colour.a : c.a);
    c.rgb((channelAlpha == CHANNEL_EXCLUSIVE) ? colour.aaa() : c.rgb());
    return c;
  }
  void run(void) {
    vec4 colour = vec4(0.f);
#if USE_PROJECTION
    vec3 dir = vec3(0.f);
    //get viewing direction from different projection types
#if CAMERA_TYPE == CAMERA_RECTILINEAR
    dir = projectRectilinear(texcoord);
#elif CAMERA_TYPE == CAMERA_FISHEYE
    dir = projectFisheye(texcoord);
#elif CAMERA_TYPE == CAMERA_EQUIRECTANGULAR
    dir = projectEquirect(texcoord);
#else
#error Invalid Camera Type
#endif
    //apply rotation to the viewing direction
#if CAMERA_TYPE != CAMERA_EQUIRECTANGULAR
    dir = rotateVector(dir, vec3(1, 0, 0), pitch);
#endif
    dir = rotateVector(dir, vec3(0, 1, 0), yaw);
    dir = flipRotate * dir;
    //use the viewing direction to sample from the texture
    //according to its projection
#if SAMPLE_TYPE == SAMPLE_EQUIRECTANGULAR
    sampleEquirectangular(dir);
#elif SAMPLE_TYPE == SAMPLE_MIRRORBALL
    sampleEquisolid(dir);
#elif SAMPLE_TYPE == SAMPLE_CUBEMAP
#error Cubemaps currently not implemented
    sampleCubemap(dir);
#else
#error Invalid Projection Type
#endif
#else // USE_PROJECTION
    sampleTexture2D(texcoord);
#endif // USE_PROJECTION
    if (!isHDR) {
      // convert samples to linear
      sample00.rgb(pow(sample00.rgb(), vec3(2.2)));
      sample10.rgb(pow(sample10.rgb(), vec3(2.2)));
      sample01.rgb(pow(sample01.rgb(), vec3(2.2)));
      sample11.rgb(pow(sample11.rgb(), vec3(2.2)));
    }
    if (tonemap) {
      tonemapUncharted2();
    }
    else {
      tonemapLinear();
    }
    // premultiply alpha
    sample00.rgb(sample00.rgb() * sample00.a);
    sample10.rgb(sample10.rgb() * sample10.a);
    sample01.rgb(sample01.rgb() * sample01.a);
    sample11.rgb(sample11.rgb() * sample11.a);
    // blend the samples
    vec4 mix0 = mix(sample00, sample10, blend.x);
    vec4 mix1 = mix(sample01, sample11, blend.x);
    colour = mix(mix0, mix1, blend.y);
    colour = selectChannels(colour);
    vec2 coord = gl_FragCoord.xy() / checkerSize;
    float checker = fmod(floor(coord.x) + floor(coord.y), 2.0);
    vec4 baseColour = mix(backgroundColour, mix(checkerColour1, checkerColour2, checker), float(useChecker));
    colour = baseColour * (1.0 - colour.a) + colour;
    // only convert back to srgb for ldr images and tonemapped hdr images
    // so one can still watch an hdr image in linear space
    if (!isHDR || tonemap) {
      colour.rgb(pow(colour.rgb(), vec3(1.0 / 2.2)));
    }
    gl_FragColor = colour;
  }
};


#undef in
#undef out
#undef uniform