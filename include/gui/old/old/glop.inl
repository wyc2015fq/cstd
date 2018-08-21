
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define gl_malloc malloc
#define gl_free free
enum {
  /* Boolean values */
  GL_FALSE = 0,
  GL_TRUE = 1,
  /* Data types */
  GL_BYTE = 0x1400,
  GL_UNSIGNED_BYTE = 0x1401,
  GL_SHORT = 0x1402,
  GL_UNSIGNED_SHORT = 0x1403,
  GL_INT = 0x1404,
  GL_UNSIGNED_INT = 0x1405,
  GL_FLOAT = 0x1406,
  GL_DOUBLE = 0x140A,
  GL_2_BYTES = 0x1407,
  GL_3_BYTES = 0x1408,
  GL_4_BYTES = 0x1409,
  /* Primitives */
  GL_LINES = 0x0001,
  GL_POINTS = 0x0000,
  GL_LINE_STRIP = 0x0003,
  GL_LINE_LOOP = 0x0002,
  GL_TRIANGLES = 0x0004,
  GL_TRIANGLE_STRIP = 0x0005,
  GL_TRIANGLE_FAN = 0x0006,
  GL_QUADS = 0x0007,
  GL_QUAD_STRIP = 0x0008,
  GL_POLYGON = 0x0009,
  GL_EDGE_FLAG = 0x0B43,
  /* Vertex Arrays */
  GL_VERTEX_ARRAY = 0x8074,
  GL_NORMAL_ARRAY = 0x8075,
  GL_COLOR_ARRAY = 0x8076,
  GL_INDEX_ARRAY = 0x8077,
  GL_TEXTURE_COORD_ARRAY = 0x8078,
  GL_EDGE_FLAG_ARRAY = 0x8079,
  GL_VERTEX_ARRAY_SIZE = 0x807A,
  GL_VERTEX_ARRAY_TYPE = 0x807B,
  GL_VERTEX_ARRAY_STRIDE = 0x807C,
  GL_VERTEX_ARRAY_COUNT = 0x807D,
  GL_NORMAL_ARRAY_TYPE = 0x807E,
  GL_NORMAL_ARRAY_STRIDE = 0x807F,
  GL_NORMAL_ARRAY_COUNT = 0x8080,
  GL_COLOR_ARRAY_SIZE = 0x8081,
  GL_COLOR_ARRAY_TYPE = 0x8082,
  GL_COLOR_ARRAY_STRIDE = 0x8083,
  GL_COLOR_ARRAY_COUNT = 0x8084,
  GL_INDEX_ARRAY_TYPE = 0x8085,
  GL_INDEX_ARRAY_STRIDE = 0x8086,
  GL_INDEX_ARRAY_COUNT = 0x8087,
  GL_TEXTURE_COORD_ARRAY_SIZE = 0x8088,
  GL_TEXTURE_COORD_ARRAY_TYPE = 0x8089,
  GL_TEXTURE_COORD_ARRAY_STRIDE = 0x808A,
  GL_TEXTURE_COORD_ARRAY_COUNT = 0x808B,
  GL_EDGE_FLAG_ARRAY_STRIDE = 0x808C,
  GL_EDGE_FLAG_ARRAY_COUNT = 0x808D,
  GL_VERTEX_ARRAY_POINTER = 0x808E,
  GL_NORMAL_ARRAY_POINTER = 0x808F,
  GL_COLOR_ARRAY_POINTER = 0x8090,
  GL_INDEX_ARRAY_POINTER = 0x8091,
  GL_TEXTURE_COORD_ARRAY_POINTER = 0x8092,
  GL_EDGE_FLAG_ARRAY_POINTER = 0x8093,
  GL_V2F = 0x2A20,
  GL_V3F = 0x2A21,
  GL_C4UB_V2F = 0x2A22,
  GL_C4UB_V3F = 0x2A23,
  GL_C3F_V3F = 0x2A24,
  GL_N3F_V3F = 0x2A25,
  GL_C4F_N3F_V3F = 0x2A26,
  GL_T2F_V3F = 0x2A27,
  GL_T4F_V4F = 0x2A28,
  GL_T2F_C4UB_V3F = 0x2A29,
  GL_T2F_C3F_V3F = 0x2A2A,
  GL_T2F_N3F_V3F = 0x2A2B,
  GL_T2F_C4F_N3F_V3F = 0x2A2C,
  GL_T4F_C4F_N3F_V4F = 0x2A2D,
  /* Matrix Mode */
  GL_MATRIX_MODE = 0x0BA0,
  GL_MODELVIEW = 0x1700,
  GL_PROJECTION = 0x1701,
  GL_TEXTURE = 0x1702,
  /* Points */
  GL_POINT_SMOOTH = 0x0B10,
  GL_POINT_SIZE = 0x0B11,
  GL_POINT_SIZE_GRANULARITY = 0x0B13,
  GL_POINT_SIZE_RANGE = 0x0B12,
  /* Lines */
  GL_LINE_SMOOTH = 0x0B20,
  GL_LINE_STIPPLE = 0x0B24,
  GL_LINE_STIPPLE_PATTERN = 0x0B25,
  GL_LINE_STIPPLE_REPEAT = 0x0B26,
  GL_LINE_WIDTH = 0x0B21,
  GL_LINE_WIDTH_GRANULARITY = 0x0B23,
  GL_LINE_WIDTH_RANGE = 0x0B22,
  /* Polygons */
  GL_POINT = 0x1B00,
  GL_LINE = 0x1B01,
  GL_FILL = 0x1B02,
  GL_CCW = 0x0901,
  GL_CW = 0x0900,
  GL_FRONT = 0x0404,
  GL_BACK = 0x0405,
  GL_CULL_FACE = 0x0B44,
  GL_CULL_FACE_MODE = 0x0B45,
  GL_POLYGON_SMOOTH = 0x0B41,
  GL_POLYGON_STIPPLE = 0x0B42,
  GL_FRONT_FACE = 0x0B46,
  GL_POLYGON_MODE = 0x0B40,
  GL_POLYGON_OFFSET_FACTOR = 0x3038,
  GL_POLYGON_OFFSET_UNITS = 0x2A00,
  GL_POLYGON_OFFSET_POINT = 0x2A01,
  GL_POLYGON_OFFSET_LINE = 0x2A02,
  GL_POLYGON_OFFSET_FILL = 0x8037,
  /* Display Lists */
  GL_COMPILE = 0x1300,
  GL_COMPILE_AND_EXECUTE = 0x1301,
  GL_LIST_BASE = 0x0B32,
  GL_LIST_INDEX = 0x0B33,
  GL_LIST_MODE = 0x0B30,
  /* Depth buffer */
  GL_NEVER = 0x0200,
  GL_LESS = 0x0201,
  GL_GEQUAL = 0x0206,
  GL_LEQUAL = 0x0203,
  GL_GREATER = 0x0204,
  GL_NOTEQUAL = 0x0205,
  GL_EQUAL = 0x0202,
  GL_ALWAYS = 0x0207,
  GL_DEPTH_TEST = 0x0B71,
  GL_DEPTH_BITS = 0x0D56,
  GL_DEPTH_CLEAR_VALUE = 0x0B73,
  GL_DEPTH_FUNC = 0x0B74,
  GL_DEPTH_RANGE = 0x0B70,
  GL_DEPTH_WRITEMASK = 0x0B72,
  GL_DEPTH_COMPONENT = 0x1902,
  /* Lighting */
  GL_LIGHTING = 0x0B50,
  GL_LIGHT0 = 0x4000,
  GL_LIGHT1 = 0x4001,
  GL_LIGHT2 = 0x4002,
  GL_LIGHT3 = 0x4003,
  GL_LIGHT4 = 0x4004,
  GL_LIGHT5 = 0x4005,
  GL_LIGHT6 = 0x4006,
  GL_LIGHT7 = 0x4007,
  GL_SPOT_EXPONENT = 0x1205,
  GL_SPOT_CUTOFF = 0x1206,
  GL_CONSTANT_ATTENUATION = 0x1207,
  GL_LINEAR_ATTENUATION = 0x1208,
  GL_QUADRATIC_ATTENUATION = 0x1209,
  GL_AMBIENT = 0x1200,
  GL_DIFFUSE = 0x1201,
  GL_SPECULAR = 0x1202,
  GL_SHININESS = 0x1601,
  GL_EMISSION = 0x1600,
  GL_POSITION = 0x1203,
  GL_SPOT_DIRECTION = 0x1204,
  GL_AMBIENT_AND_DIFFUSE = 0x1602,
  GL_COLOR_INDEXES = 0x1603,
  GL_LIGHT_MODEL_TWO_SIDE = 0x0B52,
  GL_LIGHT_MODEL_LOCAL_VIEWER = 0x0B51,
  GL_LIGHT_MODEL_AMBIENT = 0x0B53,
  GL_FRONT_AND_BACK = 0x0408,
  GL_SHADE_MODEL = 0x0B54,
  GL_FLAT = 0x1D00,
  GL_SMOOTH = 0x1D01,
  GL_COLOR_MATERIAL = 0x0B57,
  GL_COLOR_MATERIAL_FACE = 0x0B55,
  GL_COLOR_MATERIAL_PARAMETER = 0x0B56,
  GL_NORMALIZE = 0x0BA1,
  /* User clipping planes */
  GL_CLIP_PLANE0 = 0x3000,
  GL_CLIP_PLANE1 = 0x3001,
  GL_CLIP_PLANE2 = 0x3002,
  GL_CLIP_PLANE3 = 0x3003,
  GL_CLIP_PLANE4 = 0x3004,
  GL_CLIP_PLANE5 = 0x3005,
  /* Accumulation buffer */
  GL_ACCUM_RED_BITS = 0x0D58,
  GL_ACCUM_GREEN_BITS = 0x0D59,
  GL_ACCUM_BLUE_BITS = 0x0D5A,
  GL_ACCUM_ALPHA_BITS = 0x0D5B,
  GL_ACCUM_CLEAR_VALUE = 0x0B80,
  GL_ACCUM = 0x0100,
  GL_ADD = 0x0104,
  GL_LOAD = 0x0101,
  GL_MULT = 0x0103,
  GL_RETURN = 0x0102,
  /* Alpha testing */
  GL_ALPHA_TEST = 0x0BC0,
  GL_ALPHA_TEST_REF = 0x0BC2,
  GL_ALPHA_TEST_FUNC = 0x0BC1,
  /* Blending */
  GL_BLEND = 0x0BE2,
  GL_BLEND_SRC = 0x0BE1,
  GL_BLEND_DST = 0x0BE0,
  GL_ZERO = 0,
  GL_ONE = 1,
  GL_SRC_COLOR = 0x0300,
  GL_ONE_MINUS_SRC_COLOR = 0x0301,
  GL_DST_COLOR = 0x0306,
  GL_ONE_MINUS_DST_COLOR = 0x0307,
  GL_SRC_ALPHA = 0x0302,
  GL_ONE_MINUS_SRC_ALPHA = 0x0303,
  GL_DST_ALPHA = 0x0304,
  GL_ONE_MINUS_DST_ALPHA = 0x0305,
  GL_SRC_ALPHA_SATURATE = 0x0308,
  GL_CONSTANT_COLOR = 0x8001,
  GL_ONE_MINUS_CONSTANT_COLOR = 0x8002,
  GL_CONSTANT_ALPHA = 0x8003,
  GL_ONE_MINUS_CONSTANT_ALPHA = 0x8004,
  /* Render Mode */
  GL_FEEDBACK = 0x1C01,
  GL_RENDER = 0x1C00,
  GL_SELECT = 0x1C02,
  /* Feedback */
  GL_2D = 0x0600,
  GL_3D = 0x0601,
  GL_3D_COLOR = 0x0602,
  GL_3D_COLOR_TEXTURE = 0x0603,
  GL_4D_COLOR_TEXTURE = 0x0604,
  GL_POINT_TOKEN = 0x0701,
  GL_LINE_TOKEN = 0x0702,
  GL_LINE_RESET_TOKEN = 0x0707,
  GL_POLYGON_TOKEN = 0x0703,
  GL_BITMAP_TOKEN = 0x0704,
  GL_DRAW_PIXEL_TOKEN = 0x0705,
  GL_COPY_PIXEL_TOKEN = 0x0706,
  GL_PASS_THROUGH_TOKEN = 0x0700,
  /* Fog */
  GL_FOG = 0x0B60,
  GL_FOG_MODE = 0x0B65,
  GL_FOG_DENSITY = 0x0B62,
  GL_FOG_COLOR = 0x0B66,
  GL_FOG_INDEX = 0x0B61,
  GL_FOG_START = 0x0B63,
  GL_FOG_END = 0x0B64,
  GL_LINEAR = 0x2601,
  GL_EXP = 0x0800,
  GL_EXP2 = 0x0801,
  /* Logic Ops */
  GL_LOGIC_OP = 0x0BF1,
  GL_LOGIC_OP_MODE = 0x0BF0,
  GL_CLEAR = 0x1500,
  GL_SET = 0x150F,
  GL_COPY = 0x1503,
  GL_COPY_INVERTED = 0x150C,
  GL_NOOP = 0x1505,
  GL_INVERT = 0x150A,
  GL_AND = 0x1501,
  GL_NAND = 0x150E,
  GL_OR = 0x1507,
  GL_NOR = 0x1508,
  GL_XOR = 0x1506,
  GL_EQUIV = 0x1509,
  GL_AND_REVERSE = 0x1502,
  GL_AND_INVERTED = 0x1504,
  GL_OR_REVERSE = 0x150B,
  GL_OR_INVERTED = 0x150D,
  /* Stencil */
  GL_STENCIL_TEST = 0x0B90,
  GL_STENCIL_WRITEMASK = 0x0B98,
  GL_STENCIL_BITS = 0x0D57,
  GL_STENCIL_FUNC = 0x0B92,
  GL_STENCIL_VALUE_MASK = 0x0B93,
  GL_STENCIL_REF = 0x0B97,
  GL_STENCIL_FAIL = 0x0B94,
  GL_STENCIL_PASS_DEPTH_PASS = 0x0B96,
  GL_STENCIL_PASS_DEPTH_FAIL = 0x0B95,
  GL_STENCIL_CLEAR_VALUE = 0x0B91,
  GL_STENCIL_INDEX = 0x1901,
  GL_KEEP = 0x1E00,
  GL_REPLACE = 0x1E01,
  GL_INCR = 0x1E02,
  GL_DECR = 0x1E03,
  /* Buffers, Pixel Drawing/Reading */
  GL_NONE = 0,
  GL_LEFT = 0x0406,
  GL_RIGHT = 0x0407,
  /*GL_FRONT = 0x0404, */
  /*GL_BACK = 0x0405, */
  /*GL_FRONT_AND_BACK = 0x0408, */
  GL_FRONT_LEFT = 0x0400,
  GL_FRONT_RIGHT = 0x0401,
  GL_BACK_LEFT = 0x0402,
  GL_BACK_RIGHT = 0x0403,
  GL_AUX0 = 0x0409,
  GL_AUX1 = 0x040A,
  GL_AUX2 = 0x040B,
  GL_AUX3 = 0x040C,
  GL_COLOR_INDEX = 0x1900,
  GL_RED = 0x1903,
  GL_GREEN = 0x1904,
  GL_BLUE = 0x1905,
  GL_ALPHA = 0x1906,
  GL_LUMINANCE = 0x1909,
  GL_LUMINANCE_ALPHA = 0x190A,
  GL_ALPHA_BITS = 0x0D55,
  GL_RED_BITS = 0x0D52,
  GL_GREEN_BITS = 0x0D53,
  GL_BLUE_BITS = 0x0D54,
  GL_INDEX_BITS = 0x0D51,
  GL_SUBPIXEL_BITS = 0x0D50,
  GL_AUX_BUFFERS = 0x0C00,
  GL_READ_BUFFER = 0x0C02,
  GL_DRAW_BUFFER = 0x0C01,
  GL_DOUBLEBUFFER = 0x0C32,
  GL_STEREO = 0x0C33,
  GL_BITMAP = 0x1A00,
  GL_COLOR = 0x1800,
  GL_DEPTH = 0x1801,
  GL_STENCIL = 0x1802,
  GL_DITHER = 0x0BD0,
  GL_RGB = 0x1907,
  GL_RGBA = 0x1908,
  /* Implementation limits */
  GL_MAX_LIST_NESTING = 0x0B31,
  GL_MAX_ATTRIB_STACK_DEPTH = 0x0D35,
  GL_MAX_MODELVIEW_STACK_DEPTH = 0x0D36,
  GL_MAX_NAME_STACK_DEPTH = 0x0D37,
  GL_MAX_PROJECTION_STACK_DEPTH = 0x0D38,
  GL_MAX_TEXTURE_STACK_DEPTH = 0x0D39,
  GL_MAX_EVAL_ORDER = 0x0D30,
  GL_MAX_LIGHTS = 0x0D31,
  GL_MAX_CLIP_PLANES = 0x0D32,
  GL_MAX_TEXTURE_SIZE = 0x0D33,
  GL_MAX_PIXEL_MAP_TABLE = 0x0D34,
  GL_MAX_VIEWPORT_DIMS = 0x0D3A,
  GL_MAX_CLIENT_ATTRIB_STACK_DEPTH = 0x0D3B,
  /* Gets */
  GL_ATTRIB_STACK_DEPTH = 0x0BB0,
  GL_COLOR_CLEAR_VALUE = 0x0C22,
  GL_COLOR_WRITEMASK = 0x0C23,
  GL_CURRENT_INDEX = 0x0B01,
  GL_CURRENT_COLOR = 0x0B00,
  GL_CURRENT_NORMAL = 0x0B02,
  GL_CURRENT_RASTER_COLOR = 0x0B04,
  GL_CURRENT_RASTER_DISTANCE = 0x0B09,
  GL_CURRENT_RASTER_INDEX = 0x0B05,
  GL_CURRENT_RASTER_POSITION = 0x0B07,
  GL_CURRENT_RASTER_TEXTURE_COORDS = 0x0B06,
  GL_CURRENT_RASTER_POSITION_VALID = 0x0B08,
  GL_CURRENT_TEXTURE_COORDS = 0x0B03,
  GL_INDEX_CLEAR_VALUE = 0x0C20,
  GL_INDEX_MODE = 0x0C30,
  GL_INDEX_WRITEMASK = 0x0C21,
  GL_MODELVIEW_MATRIX = 0x0BA6,
  GL_MODELVIEW_STACK_DEPTH = 0x0BA3,
  GL_NAME_STACK_DEPTH = 0x0D70,
  GL_PROJECTION_MATRIX = 0x0BA7,
  GL_PROJECTION_STACK_DEPTH = 0x0BA4,
  GL_RENDER_MODE = 0x0C40,
  GL_RGBA_MODE = 0x0C31,
  GL_TEXTURE_MATRIX = 0x0BA8,
  GL_TEXTURE_STACK_DEPTH = 0x0BA5,
  GL_VIEWPORT = 0x0BA2,
  /* Evaluators */
  GL_AUTO_NORMAL = 0x0D80,
  GL_MAP1_COLOR_4 = 0x0D90,
  GL_MAP1_GRID_DOMAIN = 0x0DD0,
  GL_MAP1_GRID_SEGMENTS = 0x0DD1,
  GL_MAP1_INDEX = 0x0D91,
  GL_MAP1_NORMAL = 0x0D92,
  GL_MAP1_TEXTURE_COORD_1 = 0x0D93,
  GL_MAP1_TEXTURE_COORD_2 = 0x0D94,
  GL_MAP1_TEXTURE_COORD_3 = 0x0D95,
  GL_MAP1_TEXTURE_COORD_4 = 0x0D96,
  GL_MAP1_VERTEX_3 = 0x0D97,
  GL_MAP1_VERTEX_4 = 0x0D98,
  GL_MAP2_COLOR_4 = 0x0DB0,
  GL_MAP2_GRID_DOMAIN = 0x0DD2,
  GL_MAP2_GRID_SEGMENTS = 0x0DD3,
  GL_MAP2_INDEX = 0x0DB1,
  GL_MAP2_NORMAL = 0x0DB2,
  GL_MAP2_TEXTURE_COORD_1 = 0x0DB3,
  GL_MAP2_TEXTURE_COORD_2 = 0x0DB4,
  GL_MAP2_TEXTURE_COORD_3 = 0x0DB5,
  GL_MAP2_TEXTURE_COORD_4 = 0x0DB6,
  GL_MAP2_VERTEX_3 = 0x0DB7,
  GL_MAP2_VERTEX_4 = 0x0DB8,
  GL_COEFF = 0x0A00,
  GL_DOMAIN = 0x0A02,
  GL_ORDER = 0x0A01,
  /* Hints */
  GL_FOG_HINT = 0x0C54,
  GL_LINE_SMOOTH_HINT = 0x0C52,
  GL_PERSPECTIVE_CORRECTION_HINT = 0x0C50,
  GL_POINT_SMOOTH_HINT = 0x0C51,
  GL_POLYGON_SMOOTH_HINT = 0x0C53,
  GL_DONT_CARE = 0x1100,
  GL_FASTEST = 0x1101,
  GL_NICEST = 0x1102,
  /* Scissor box */
  GL_SCISSOR_TEST = 0x0C11,
  GL_SCISSOR_BOX = 0x0C10,
  /* Pixel Mode / Transfer */
  GL_MAP_COLOR = 0x0D10,
  GL_MAP_STENCIL = 0x0D11,
  GL_INDEX_SHIFT = 0x0D12,
  GL_INDEX_OFFSET = 0x0D13,
  GL_RED_SCALE = 0x0D14,
  GL_RED_BIAS = 0x0D15,
  GL_GREEN_SCALE = 0x0D18,
  GL_GREEN_BIAS = 0x0D19,
  GL_BLUE_SCALE = 0x0D1A,
  GL_BLUE_BIAS = 0x0D1B,
  GL_ALPHA_SCALE = 0x0D1C,
  GL_ALPHA_BIAS = 0x0D1D,
  GL_DEPTH_SCALE = 0x0D1E,
  GL_DEPTH_BIAS = 0x0D1F,
  GL_PIXEL_MAP_S_TO_S_SIZE = 0x0CB1,
  GL_PIXEL_MAP_I_TO_I_SIZE = 0x0CB0,
  GL_PIXEL_MAP_I_TO_R_SIZE = 0x0CB2,
  GL_PIXEL_MAP_I_TO_G_SIZE = 0x0CB3,
  GL_PIXEL_MAP_I_TO_B_SIZE = 0x0CB4,
  GL_PIXEL_MAP_I_TO_A_SIZE = 0x0CB5,
  GL_PIXEL_MAP_R_TO_R_SIZE = 0x0CB6,
  GL_PIXEL_MAP_G_TO_G_SIZE = 0x0CB7,
  GL_PIXEL_MAP_B_TO_B_SIZE = 0x0CB8,
  GL_PIXEL_MAP_A_TO_A_SIZE = 0x0CB9,
  GL_PIXEL_MAP_S_TO_S = 0x0C71,
  GL_PIXEL_MAP_I_TO_I = 0x0C70,
  GL_PIXEL_MAP_I_TO_R = 0x0C72,
  GL_PIXEL_MAP_I_TO_G = 0x0C73,
  GL_PIXEL_MAP_I_TO_B = 0x0C74,
  GL_PIXEL_MAP_I_TO_A = 0x0C75,
  GL_PIXEL_MAP_R_TO_R = 0x0C76,
  GL_PIXEL_MAP_G_TO_G = 0x0C77,
  GL_PIXEL_MAP_B_TO_B = 0x0C78,
  GL_PIXEL_MAP_A_TO_A = 0x0C79,
  GL_PACK_ALIGNMENT = 0x0D05,
  GL_PACK_LSB_FIRST = 0x0D01,
  GL_PACK_ROW_LENGTH = 0x0D02,
  GL_PACK_SKIP_PIXELS = 0x0D04,
  GL_PACK_SKIP_ROWS = 0x0D03,
  GL_PACK_SWAP_BYTES = 0x0D00,
  GL_UNPACK_ALIGNMENT = 0x0CF5,
  GL_UNPACK_LSB_FIRST = 0x0CF1,
  GL_UNPACK_ROW_LENGTH = 0x0CF2,
  GL_UNPACK_SKIP_PIXELS = 0x0CF4,
  GL_UNPACK_SKIP_ROWS = 0x0CF3,
  GL_UNPACK_SWAP_BYTES = 0x0CF0,
  GL_ZOOM_X = 0x0D16,
  GL_ZOOM_Y = 0x0D17,
  /* Texture mapping */
  GL_TEXTURE_ENV = 0x2300,
  GL_TEXTURE_ENV_MODE = 0x2200,
  GL_TEXTURE_1D = 0x0DE0,
  GL_TEXTURE_2D = 0x0DE1,
  GL_TEXTURE_WRAP_S = 0x2802,
  GL_TEXTURE_WRAP_T = 0x2803,
  GL_TEXTURE_MAG_FILTER = 0x2800,
  GL_TEXTURE_MIN_FILTER = 0x2801,
  GL_TEXTURE_ENV_COLOR = 0x2201,
  GL_TEXTURE_GEN_S = 0x0C60,
  GL_TEXTURE_GEN_T = 0x0C61,
  GL_TEXTURE_GEN_MODE = 0x2500,
  GL_TEXTURE_BORDER_COLOR = 0x1004,
  GL_TEXTURE_WIDTH = 0x1000,
  GL_TEXTURE_HEIGHT = 0x1001,
  GL_TEXTURE_BORDER = 0x1005,
  GL_TEXTURE_COMPONENTS = 0x1003,
  GL_NEAREST_MIPMAP_NEAREST = 0x2700,
  GL_NEAREST_MIPMAP_LINEAR = 0x2702,
  GL_LINEAR_MIPMAP_NEAREST = 0x2701,
  GL_LINEAR_MIPMAP_LINEAR = 0x2703,
  GL_OBJECT_LINEAR = 0x2401,
  GL_OBJECT_PLANE = 0x2501,
  GL_EYE_LINEAR = 0x2400,
  GL_EYE_PLANE = 0x2502,
  GL_SPHERE_MAP = 0x2402,
  GL_DECAL = 0x2101,
  GL_MODULATE = 0x2100,
  GL_NEAREST = 0x2600,
  GL_REPEAT = 0x2901,
  GL_CLAMP = 0x2900,
  GL_S = 0x2000,
  GL_T = 0x2001,
  GL_R = 0x2002,
  GL_Q = 0x2003,
  GL_TEXTURE_GEN_R = 0x0C62,
  GL_TEXTURE_GEN_Q = 0x0C63,
  GL_PROXY_TEXTURE_1D = 0x8063,
  GL_PROXY_TEXTURE_2D = 0x8064,
  GL_TEXTURE_PRIORITY = 0x8066,
  GL_TEXTURE_RESIDENT = 0x8067,
  GL_TEXTURE_1D_BINDING = 0x8068,
  GL_TEXTURE_2D_BINDING = 0x8069,
  /* Internal texture formats */
  GL_ALPHA4 = 0x803B,
  GL_ALPHA8 = 0x803C,
  GL_ALPHA12 = 0x803D,
  GL_ALPHA16 = 0x803E,
  GL_LUMINANCE4 = 0x803F,
  GL_LUMINANCE8 = 0x8040,
  GL_LUMINANCE12 = 0x8041,
  GL_LUMINANCE16 = 0x8042,
  GL_LUMINANCE4_ALPHA4 = 0x8043,
  GL_LUMINANCE6_ALPHA2 = 0x8044,
  GL_LUMINANCE8_ALPHA8 = 0x8045,
  GL_LUMINANCE12_ALPHA4 = 0x8046,
  GL_LUMINANCE12_ALPHA12 = 0x8047,
  GL_LUMINANCE16_ALPHA16 = 0x8048,
  GL_INTENSITY = 0x8049,
  GL_INTENSITY4 = 0x804A,
  GL_INTENSITY8 = 0x804B,
  GL_INTENSITY12 = 0x804C,
  GL_INTENSITY16 = 0x804D,
  GL_R3_G3_B2 = 0x2A10,
  GL_RGB4 = 0x804F,
  GL_RGB5 = 0x8050,
  GL_RGB8 = 0x8051,
  GL_RGB10 = 0x8052,
  GL_RGB12 = 0x8053,
  GL_RGB16 = 0x8054,
  GL_RGBA2 = 0x8055,
  GL_RGBA4 = 0x8056,
  GL_RGB5_A1 = 0x8057,
  GL_RGBA8 = 0x8058,
  GL_RGB10_A2 = 0x8059,
  GL_RGBA12 = 0x805A,
  GL_RGBA16 = 0x805B,
  /* Utility */
  GL_VENDOR = 0x1F00,
  GL_RENDERER = 0x1F01,
  GL_VERSION = 0x1F02,
  GL_EXTENSIONS = 0x1F03,
  /* Errors */
  GL_INVALID_VALUE = 0x0501,
  GL_INVALID_ENUM = 0x0500,
  GL_INVALID_OPERATION = 0x0502,
  GL_STACK_OVERFLOW = 0x0503,
  GL_STACK_UNDERFLOW = 0x0504,
  GL_OUT_OF_MEMORY = 0x0505,
  /*
  * 1.0 Extensions
  */
  /* GL_EXT_blend_minmax and GL_EXT_blend_color */
  GL_CONSTANT_COLOR_EXT = 0x8001,
  GL_ONE_MINUS_CONSTANT_COLOR_EXT = 0x8002,
  GL_CONSTANT_ALPHA_EXT = 0x8003,
  GL_ONE_MINUS_CONSTANT_ALPHA_EXT = 0x8004,
  GL_BLEND_EQUATION_EXT = 0x8009,
  GL_MIN_EXT = 0x8007,
  GL_MAX_EXT = 0x8008,
  GL_FUNC_ADD_EXT = 0x8006,
  GL_FUNC_SUBTRACT_EXT = 0x800A,
  GL_FUNC_REVERSE_SUBTRACT_EXT = 0x800B,
  GL_BLEND_COLOR_EXT = 0x8005,
  /* GL_EXT_polygon_offset */
  GL_POLYGON_OFFSET_EXT = 0x8037,
  GL_POLYGON_OFFSET_FACTOR_EXT = 0x8038,
  GL_POLYGON_OFFSET_BIAS_EXT = 0x8039,
  /* GL_EXT_vertex_array */
  GL_VERTEX_ARRAY_EXT = 0x8074,
  GL_NORMAL_ARRAY_EXT = 0x8075,
  GL_COLOR_ARRAY_EXT = 0x8076,
  GL_INDEX_ARRAY_EXT = 0x8077,
  GL_TEXTURE_COORD_ARRAY_EXT = 0x8078,
  GL_EDGE_FLAG_ARRAY_EXT = 0x8079,
  GL_VERTEX_ARRAY_SIZE_EXT = 0x807A,
  GL_VERTEX_ARRAY_TYPE_EXT = 0x807B,
  GL_VERTEX_ARRAY_STRIDE_EXT = 0x807C,
  GL_VERTEX_ARRAY_COUNT_EXT = 0x807D,
  GL_NORMAL_ARRAY_TYPE_EXT = 0x807E,
  GL_NORMAL_ARRAY_STRIDE_EXT = 0x807F,
  GL_NORMAL_ARRAY_COUNT_EXT = 0x8080,
  GL_COLOR_ARRAY_SIZE_EXT = 0x8081,
  GL_COLOR_ARRAY_TYPE_EXT = 0x8082,
  GL_COLOR_ARRAY_STRIDE_EXT = 0x8083,
  GL_COLOR_ARRAY_COUNT_EXT = 0x8084,
  GL_INDEX_ARRAY_TYPE_EXT = 0x8085,
  GL_INDEX_ARRAY_STRIDE_EXT = 0x8086,
  GL_INDEX_ARRAY_COUNT_EXT = 0x8087,
  GL_TEXTURE_COORD_ARRAY_SIZE_EXT = 0x8088,
  GL_TEXTURE_COORD_ARRAY_TYPE_EXT = 0x8089,
  GL_TEXTURE_COORD_ARRAY_STRIDE_EXT = 0x808A,
  GL_TEXTURE_COORD_ARRAY_COUNT_EXT = 0x808B,
  GL_EDGE_FLAG_ARRAY_STRIDE_EXT = 0x808C,
  GL_EDGE_FLAG_ARRAY_COUNT_EXT = 0x808D,
  GL_VERTEX_ARRAY_POINTER_EXT = 0x808E,
  GL_NORMAL_ARRAY_POINTER_EXT = 0x808F,
  GL_COLOR_ARRAY_POINTER_EXT = 0x8090,
  GL_INDEX_ARRAY_POINTER_EXT = 0x8091,
  GL_TEXTURE_COORD_ARRAY_POINTER_EXT = 0x8092,
  GL_EDGE_FLAG_ARRAY_POINTER_EXT = 0x8093
};
enum {
  GL_CURRENT_BIT = 0x00000001,
  GL_POINT_BIT = 0x00000002,
  GL_LINE_BIT = 0x00000004,
  GL_POLYGON_BIT = 0x00000008,
  GL_POLYGON_STIPPLE_BIT = 0x00000010,
  GL_PIXEL_MODE_BIT = 0x00000020,
  GL_LIGHTING_BIT = 0x00000040,
  GL_FOG_BIT = 0x00000080,
  GL_DEPTH_BUFFER_BIT = 0x00000100,
  GL_ACCUM_BUFFER_BIT = 0x00000200,
  GL_STENCIL_BUFFER_BIT = 0x00000400,
  GL_VIEWPORT_BIT = 0x00000800,
  GL_TRANSFORM_BIT = 0x00001000,
  GL_ENABLE_BIT = 0x00002000,
  GL_COLOR_BUFFER_BIT = 0x00004000,
  GL_HINT_BIT = 0x00008000,
  GL_EVAL_BIT = 0x00010000,
  GL_LIST_BIT = 0x00020000,
  GL_TEXTURE_BIT = 0x00040000,
  GL_SCISSOR_BIT = 0x00080000,
  GL_ALL_ATTRIB_BITS = 0x000fffff
};
/* some types */
typedef int GLenum;
typedef void GLvoid;
typedef unsigned char GLboolean;
typedef signed char GLbyte; /* 1-byte signed */
typedef short GLshort; /* 2-byte signed */
typedef int GLint; /* 4-byte signed */
typedef unsigned char GLubyte; /* 1-byte unsigned */
typedef unsigned short GLushort; /* 2-byte unsigned */
typedef unsigned int GLuint; /* 4-byte unsigned */
typedef float GLfloat; /* single precision float */
typedef double GLdouble; /* double precision float */
typedef int GLsizei;
/* Some simple mathematical functions. Don't look for some logic in the function names :-) */
typedef struct {
  float m[4][4];
} M4;
typedef struct {
  float m[3][3];
} M3;
typedef struct {
  float m[3][4];
} M34;
typedef float V3[3];
typedef float V4[4];
#define V3_Set(v3, x, y, z) ((v3)[0] = (x), (v3)[1] = (y), (v3)[2] = (z))
/* ******* Gestion des matrices 4x4 ****** */
void gl_M4_Id(M4* a)
{
  int i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (i == j) {
        a->m[i][j] = 1.0;
      }
      else {
        a->m[i][j] = 0.0;
      }
    }
  }
}
int gl_M4_IsId(M4* a)
{
  int i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (i == j) {
        if (a->m[i][j] != 1.0) {
          return 0;
        }
      }
      else if (a->m[i][j] != 0.0) {
        return 0;
      }
    }
  }

  return 1;
}
void gl_M4_Mul(M4* c, M4* a, M4* b)
{
  int i, j, k;
  GLfloat s;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) {
      s = 0.0;

      for (k = 0; k < 4; k++) {
        s += a->m[i][k] * b->m[k][j];
      }

      c->m[i][j] = s;
    }
}
/* c=c*a */
void gl_M4_MulLeft(M4* c, M4* b)
{
  int i, j, k;
  GLfloat s;
  M4 a;
  /*memcpy(&a, c, 16*sizeof(GLfloat));
  */
  a = *c;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      s = 0.0;

      for (k = 0; k < 4; k++) {
        s += a.m[i][k] * b->m[k][j];
      }

      c->m[i][j] = s;
    }
  }
}
void gl_M4_Move(M4* a, M4* b)
{
  memcpy(a, b, sizeof(M4));
}
void gl_MoveV3(V3 a, V3 b)
{
  memcpy(a, b, sizeof(V3));
}
void gl_MulM4V3(V3 a, M4* b, V3 c)
{
  a[0] = b->m[0][0] * c[0] + b->m[0][1] * c[1] + b->m[0][2] * c[2] + b->m[0][3];
  a[1] = b->m[1][0] * c[0] + b->m[1][1] * c[1] + b->m[1][2] * c[2] + b->m[1][3];
  a[2] = b->m[2][0] * c[0] + b->m[2][1] * c[1] + b->m[2][2] * c[2] + b->m[2][3];
}
void gl_MulM3V3(V3 a, M4* b, V3 c)
{
  a[0] = b->m[0][0] * c[0] + b->m[0][1] * c[1] + b->m[0][2] * c[2];
  a[1] = b->m[1][0] * c[0] + b->m[1][1] * c[1] + b->m[1][2] * c[2];
  a[2] = b->m[2][0] * c[0] + b->m[2][1] * c[1] + b->m[2][2] * c[2];
}
void gl_M4_MulV4(V4 a, M4* b, V4 c)
{
  a[0] = b->m[0][0] * c[0] + b->m[0][1] * c[1] + b->m[0][2] * c[2] + b->m[0][3] * c[3];
  a[1] = b->m[1][0] * c[0] + b->m[1][1] * c[1] + b->m[1][2] * c[2] + b->m[1][3] * c[3];
  a[2] = b->m[2][0] * c[0] + b->m[2][1] * c[1] + b->m[2][2] * c[2] + b->m[2][3] * c[3];
  a[3] = b->m[3][0] * c[0] + b->m[3][1] * c[1] + b->m[3][2] * c[2] + b->m[3][3] * c[3];
}
/* transposition of a 4x4 matrix */
void gl_M4_Transpose(M4* a, M4* b)
{
  a->m[0][0] = b->m[0][0];
  a->m[0][1] = b->m[1][0];
  a->m[0][2] = b->m[2][0];
  a->m[0][3] = b->m[3][0];
  a->m[1][0] = b->m[0][1];
  a->m[1][1] = b->m[1][1];
  a->m[1][2] = b->m[2][1];
  a->m[1][3] = b->m[3][1];
  a->m[2][0] = b->m[0][2];
  a->m[2][1] = b->m[1][2];
  a->m[2][2] = b->m[2][2];
  a->m[2][3] = b->m[3][2];
  a->m[3][0] = b->m[0][3];
  a->m[3][1] = b->m[1][3];
  a->m[3][2] = b->m[2][3];
  a->m[3][3] = b->m[3][3];
}
/* inversion of an orthogonal matrix of type Y=M[0]+P */
void gl_M4_InvOrtho(M4* a, M4 b)
{
  int i, j;
  GLfloat s;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      a->m[i][j] = b.m[j][i];
    }
  }

  a->m[3][0] = 0.0;
  a->m[3][1] = 0.0;
  a->m[3][2] = 0.0;
  a->m[3][3] = 1.0;

  for (i = 0; i < 3; i++) {
    s = 0;

    for (j = 0; j < 3; j++) {
      s -= b.m[j][i] * b.m[j][3];
    }

    a->m[i][3] = s;
  }
}
/* Inversion of a general nxn matrix.
 Note : m is destroyed */
int Matrix_Inv(GLfloat* r, GLfloat* m, int n)
{
  int i, j, k, l;
  GLfloat max, tmp, t;

  /* identit閑 dans r */
  for (i = 0; i < n * n; i++) {
    r[i] = 0;
  }

  for (i = 0; i < n; i++) {
    r[i * n + i] = 1;
  }

  for (j = 0; j < n; j++) {
    /* recherche du nombre de plus grand module sur la colonne j */
    max = m[j * n + j];
    k = j;

    for (i = j + 1; i < n; i++)
      if (fabs(m[i * n + j]) > fabs(max)) {
        k = i;
        max = m[i * n + j];
      }

    /* non intersible matrix */
    if (max == 0) {
      return 1;
    }

    /* permutation des lignes j et k */
    if (k != j) {
      for (i = 0; i < n; i++) {
        tmp = m[j * n + i];
        m[j * n + i] = m[k * n + i];
        m[k * n + i] = tmp;
        tmp = r[j * n + i];
        r[j * n + i] = r[k * n + i];
        r[k * n + i] = tmp;
      }
    }

    /* multiplication de la ligne j par 1/max */
    max = 1 / max;

    for (i = 0; i < n; i++) {
      m[j * n + i] *= max;
      r[j * n + i] *= max;
    }

    for (l = 0; l < n; l++) if (l != j) {
        t = m[l * n + j];

        for (i = 0; i < n; i++) {
          m[l * n + i] -= m[j * n + i] * t;
          r[l * n + i] -= r[j * n + i] * t;
        }
      }
  }

  return 0;
}
/* inversion of a 4x4 matrix */
void gl_M4_Inv(M4* a, M4* b)
{
  M4 tmp;
  memcpy(&tmp, b, 16 * sizeof(GLfloat));
  /*tmp=*b;*/
  Matrix_Inv(&a->m[0][0], &tmp.m[0][0], 4);
}
void gl_M4_Rotate(M4* a, GLfloat t, int u)
{
  GLfloat s, c;
  int v, w;

  if ((v = u + 1) > 2) {
    v = 0;
  }

  if ((w = v + 1) > 2) {
    w = 0;
  }

  s = (float)sin(t);
  c = (float)cos(t);
  gl_M4_Id(a);
  a->m[v][v] = c;
  a->m[v][w] = -s;
  a->m[w][v] = s;
  a->m[w][w] = c;
}
/* inverse of a 3x3 matrix */
void gl_M3_Inv(M3* a, M3* m)
{
  GLfloat det;
  det = m->m[0][0] * m->m[1][1] * m->m[2][2] - m->m[0][0] * m->m[1][2] * m->m[2][1] -
      m->m[1][0] * m->m[0][1] * m->m[2][2] + m->m[1][0] * m->m[0][2] * m->m[2][1] +
      m->m[2][0] * m->m[0][1] * m->m[1][2] - m->m[2][0] * m->m[0][2] * m->m[1][1];
  a->m[0][0] = (m->m[1][1] * m->m[2][2] - m->m[1][2] * m->m[2][1]) / det;
  a->m[0][1] = -(m->m[0][1] * m->m[2][2] - m->m[0][2] * m->m[2][1]) / det;
  a->m[0][2] = -(-m->m[0][1] * m->m[1][2] + m->m[0][2] * m->m[1][1]) / det;
  a->m[1][0] = -(m->m[1][0] * m->m[2][2] - m->m[1][2] * m->m[2][0]) / det;
  a->m[1][1] = (m->m[0][0] * m->m[2][2] - m->m[0][2] * m->m[2][0]) / det;
  a->m[1][2] = -(m->m[0][0] * m->m[1][2] - m->m[0][2] * m->m[1][0]) / det;
  a->m[2][0] = (m->m[1][0] * m->m[2][1] - m->m[1][1] * m->m[2][0]) / det;
  a->m[2][1] = -(m->m[0][0] * m->m[2][1] - m->m[0][1] * m->m[2][0]) / det;
  a->m[2][2] = (m->m[0][0] * m->m[1][1] - m->m[0][1] * m->m[1][0]) / det;
}
/* vector arithmetic */
int gl_V3_Norm(V3 a)
{
  GLfloat n;
  n = (float)sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);

  if (n == 0) {
    return 1;
  }

  a[0] /= n;
  a[1] /= n;
  a[2] /= n;
  return 0;
}
void gl_V3_New(V3 a, GLfloat x, GLfloat y, GLfloat z)
{
  a[0] = x;
  a[1] = y;
  a[2] = z;
}
void gl_V4_New(V4 a, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  a[0] = x;
  a[1] = y;
  a[2] = z;
  a[3] = w;
}
void gl_V3_Cross(V3 c, V3 a, V3 b)
{
  gl_V3_New(c, a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

//typedef unsigned int PIXEL;
#define BPP2CN(bpp) (((bpp)+1)>>3)
#if 0
static void ZB_copyBuffer(GLContext* c, void* buf, int step, int cn)
{
  unsigned char* p1 = (unsigned char*)buf;
  unsigned char* q = c->pbuf;
  int y, n = c->w * cn;

  for (y = 0; y < c->h; y++) {
    memcpy(p1, q, n);
    p1 += step;
    q += c->step;
  }
}
/* 32 bpp copy */
#define RGB16_TO_RGB32(p0,p1,v)\
  {\
    unsigned int g,b,gb;\
    g = (v & 0x07E007E0) << 5;\
    b = (v & 0x001F001F) << 3;\
    gb = g | b;\
    p0 = (gb & 0x0000FFFF) | ((v & 0x0000F800) << 8);\
    p1 = (gb >> 16) | ((v & 0xF8000000) >> 8);\
  }
static void ZB_copyFrameBufferRGB32(GLContext* c, void* buf, int step)
{
  unsigned uchar* q;
  unsigned int* p, *p1, v, w0, w1;
  int y, n;
  q = c->pbuf;
  p1 = (unsigned int*) buf;

  for (y = 0; y < c->h; y++) {
    p = p1;
    n = c->w >> 2;

    if (ENDIANNESS == 'b') {
      do {
        v = *(unsigned int*) q;
        RGB16_TO_RGB32(w1, w0, v);
        p[0] = w0;
        p[1] = w1;
        v = *(unsigned int*)(q + 2);
        RGB16_TO_RGB32(w1, w0, v);
        p[2] = w0;
        p[3] = w1;
        q += 4;
        p += 4;
      }
      while (--n > 0);
    }
    else {
      do {
        v = *(unsigned int*) q;
        RGB16_TO_RGB32(w0, w1, v);
        p[0] = w0;
        p[1] = w1;
        v = *(unsigned int*)(q + 2);
        RGB16_TO_RGB32(w0, w1, v);
        p[2] = w0;
        p[3] = w1;
        q += 4;
        p += 4;
      }
      while (--n > 0);
    }

    p1 += step;
  }
}
/* 24 bit packed pixel handling */
/* order: RGBR GBRG BRGB */
/* XXX: packed pixel 24 bit support not tested */
/* XXX: big endian case not optimised */
#if BYTE_ORDER == BIG_ENDIAN
#define RGB16_TO_RGB24(p0,p1,p2,v1,v2)\
  {\
    unsigned int r1,g1,b1,gb1,g2,b2,gb2;\
    v1 = (v1 << 16) | (v1 >> 16);\
    v2 = (v2 << 16) | (v2 >> 16);\
    r1 = (v1 & 0xF800F800);\
    g1 = (v1 & 0x07E007E0) << 5;\
    b1 = (v1 & 0x001F001F) << 3;\
    gb1 = g1 | b1;\
    p0 = ((gb1 & 0x0000FFFF) << 8) | (r1 << 16) | (r1 >> 24);\
    g2 = (v2 & 0x07E007E0) << 5;\
    b2 = (v2 & 0x001F001F) << 3;\
    gb2 = g2 | b2;\
    p1 = (gb1 & 0xFFFF0000) | (v2 & 0xF800) | ((gb2 >> 8) & 0xff);\
    p2 = (gb2 << 24) | ((v2 & 0xF8000000) >> 8) | (gb2 >> 16);\
  }
#else
#define RGB16_TO_RGB24(p0,p1,p2,v1,v2)\
  {\
    unsigned int r1,g1,b1,gb1,g2,b2,gb2;\
    r1 = (v1 & 0xF800F800);\
    g1 = (v1 & 0x07E007E0) << 5;\
    b1 = (v1 & 0x001F001F) << 3;\
    gb1 = g1 | b1;\
    p0 = ((gb1 & 0x0000FFFF) << 8) | (r1 << 16) | (r1 >> 24);\
    g2 = (v2 & 0x07E007E0) << 5;\
    b2 = (v2 & 0x001F001F) << 3;\
    gb2 = g2 | b2;\
    p1 = (gb1 & 0xFFFF0000) | (v2 & 0xF800) | ((gb2 >> 8) & 0xff);\
    p2 = (gb2 << 24) | ((v2 & 0xF8000000) >> 8) | (gb2 >> 16);\
  }
#endif
static void ZB_copyFrameBufferRGB24(GLContext* c, void* buf, int step)
{
  unsigned short* q;
  unsigned int* p, *p1, w0, w1, w2, v0, v1;
  int y, n;
  q = c->pbuf;
  p1 = (unsigned int*) buf;
  step = step * 3;

  for (y = 0; y < c->h; y++) {
    p = p1;
    n = c->w >> 2;

    do {
      v0 = *(unsigned int*) q;
      v1 = *(unsigned int*)(q + 2);
      RGB16_TO_RGB24(w0, w1, w2, v0, v1);
      p[0] = w0;
      p[1] = w1;
      p[2] = w2;
      q += 4;
      p += 3;
    }
    while (--n > 0);

    p1 = (unsigned int*)(((char*) p1) + step);
  }
}
void ZB_copyFrameBuffer(GLContext* c, void* buf,
    int step)
{
  switch (c->mode) {
#ifdef TGL_FEATURE_8_BITS

  case ZB_MODE_INDEX:
    ZB_ditherFrameBuffer(c, (unsigned char*)buf, step >> 1);
    break;
#endif
#ifdef TGL_FEATURE_16_BITS

  case ZB_MODE_5R6G5B:
    ZB_copyBuffer(c, buf, step);
    break;
#endif
#ifdef TGL_FEATURE_32_BITS

  case ZB_MODE_RGBA:
    ZB_copyFrameBufferRGB32(c, buf, step >> 1);
    break;
#endif
#ifdef TGL_FEATURE_24_BITS

  case ZB_MODE_RGB24:
    ZB_copyFrameBufferRGB24(c, buf, step >> 1);
    break;
#endif

  default:
    assert(0);
  }
}
#define RGB24_TO_RGB16(r, g, b) \
  ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
/* XXX: not optimized */
static void ZB_copyFrameBuffer5R6G5B(GLContext* c,
    void* buf, int step)
{
  PIXEL* q;
  unsigned short* p, *p1;
  int y, n;
  q = c->pbuf;
  p1 = (unsigned short*) buf;

  for (y = 0; y < c->h; y++) {
    p = p1;
    n = c->w >> 2;

    do {
      p[0] = RGB24_TO_RGB16(q[0], q[1], q[2]);
      p[1] = RGB24_TO_RGB16(q[3], q[4], q[5]);
      p[2] = RGB24_TO_RGB16(q[6], q[7], q[8]);
      p[3] = RGB24_TO_RGB16(q[9], q[10], q[11]);
      q = (PIXEL*)((char*)q + 4 * PSZB);
      p += 4;
    }
    while (--n > 0);

    p1 = (unsigned short*)((char*)p1 + step);
  }
}
void ZB_copyFrameBuffer(GLContext* c, void* buf, int step)
{
  switch (c->mode) {
#ifdef TGL_FEATURE_16_BITS

  case ZB_MODE_5R6G5B:
    ZB_copyFrameBuffer5R6G5B(c, buf, step);
    break;
#endif
#ifdef TGL_FEATURE_24_BITS

  case ZB_MODE_RGB24:
    ZB_copyBuffer(c, buf, step);
    break;
#endif

  default:
    assert(0);
  }
}
#define RGB32_TO_RGB16(v) \
  (((v >> 8) & 0xf800) | (((v) >> 5) & 0x07e0) | (((v) & 0xff) >> 3))
/* XXX: not optimized */
static void ZB_copyFrameBuffer5R6G5B(GLContext* c,
    void* buf, int step)
{
  PIXEL* q;
  unsigned short* p, *p1;
  int y, n;
  q = c->pbuf;
  p1 = (unsigned short*) buf;

  for (y = 0; y < c->h; y++) {
    p = p1;
    n = c->w >> 2;

    do {
      p[0] = RGB32_TO_RGB16(q[0]);
      p[1] = RGB32_TO_RGB16(q[1]);
      p[2] = RGB32_TO_RGB16(q[2]);
      p[3] = RGB32_TO_RGB16(q[3]);
      q += 4;
      p += 4;
    }
    while (--n > 0);

    p1 = (unsigned short*)((char*)p1 + step);
  }
}
void ZB_copyFrameBuffer(GLContext* c, void* buf, int step)
{
  switch (c->mode) {
#ifdef TGL_FEATURE_16_BITS

  case ZB_MODE_5R6G5B:
    ZB_copyFrameBuffer5R6G5B(c, buf, step);
    break;
#endif
#ifdef TGL_FEATURE_32_BITS

  case ZB_MODE_RGBA:
    ZB_copyBuffer(c, buf, step);
    break;
#endif

  default:
    assert(0);
  }
}
#endif
/*
 * adr must be aligned on an 'int'
 */
void memset_s(void* adr, int val, int count)
{
  int i, n, v;
  unsigned int* p;
  unsigned short* q;
  p = (unsigned int*)adr;
  v = val | (val << 16);
  n = count >> 3;

  for (i = 0; i < n; i++) {
    p[0] = v;
    p[1] = v;
    p[2] = v;
    p[3] = v;
    p += 4;
  }

  q = (unsigned short*) p;
  n = count & 7;

  for (i = 0; i < n; i++) {
    *q++ = val;
  }
}
void memset_l(void* adr, int val, int count)
{
  int i, n, v;
  unsigned int* p;
  p = (unsigned int*)adr;
  v = val;
  n = count >> 2;

  for (i = 0; i < n; i++) {
    p[0] = v;
    p[1] = v;
    p[2] = v;
    p[3] = v;
    p += 4;
  }

  n = count & 3;

  for (i = 0; i < n; i++) {
    *p++ = val;
  }
}
/* count must be a multiple of 4 and >= 4 */
void memset_RGB24(void* adr, int r, int v, int b, long count)
{
  long i, n;
  register long v1, v2, v3, *pt = (long*)(adr);
  unsigned char* p, R = (unsigned char)r, V = (unsigned char)v, B = (unsigned char)b;
  p = (unsigned char*)adr;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  v1 = *pt++;
  v2 = *pt++;
  v3 = *pt++;
  n = count >> 2;

  for (i = 1; i < n; i++) {
    *pt++ = v1;
    *pt++ = v2;
    *pt++ = v3;
  }
}

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "cstd.h"
#define DEBUG
/* #define NDEBUG */
enum {
#define ADD_OP(a,b,c) OP_ ## a ,
#include "opinfo.txt"
};
/* initially # of allocated GLVertexes (will grow when necessary) */
#define POLYGON_MAX_VERTEX 16
/* Max # of specular light pow buffers */
#define MAX_SPECULAR_BUFFERS 8
/* # of entries in specular buffer */
#define SPECULAR_BUFFER_SIZE 1024
/* specular buffer granularity */
#define SPECULAR_BUFFER_RESOLUTION 1024
#define MAX_MODELVIEW_STACK_DEPTH 32
#define MAX_PROJECTION_STACK_DEPTH 8
#define MAX_TEXTURE_STACK_DEPTH 8
#define MAX_NAME_STACK_DEPTH  64
#define MAX_TEXTURE_LEVELS   11
#define MAX_LIGHTS     16
#define VERTEX_HASH_SIZE 1031
#define MAX_DISPLAY_LISTS 1024
#define OP_BUFFER_MAX_SIZE 512
#define TGL_OFFSET_FILL 0x1
#define TGL_OFFSET_LINE 0x2
#define TGL_OFFSET_POINT 0x4
#define ZB_Z_BITS 16
#define ZB_POINT_Z_FRAC_BITS 14
#define ZB_POINT_S_MIN ( (1<<13) )
#define ZB_POINT_S_MAX ( (1<<22)-(1<<13) )
#define ZB_POINT_T_MIN ( (1<<21) )
#define ZB_POINT_T_MAX ( (1<<30)-(1<<21) )
#define ZB_POINT_RED_MIN ( (1<<10) )
#define ZB_POINT_RED_MAX ( (1<<16)-(1<<10) )
#define ZB_POINT_GREEN_MIN ( (1<<9) )
#define ZB_POINT_GREEN_MAX ( (1<<16)-(1<<9) )
#define ZB_POINT_BLUE_MIN ( (1<<10) )
#define ZB_POINT_BLUE_MAX ( (1<<16)-(1<<10) )
/* display modes */
#define ZB_MODE_5R6G5B 1 /* true color 16 bits */
#define ZB_MODE_INDEX 2 /* color index 8 bits */
#define ZB_MODE_RGBA 3 /* 32 bit rgba mode */
#define ZB_MODE_RGB24 4 /* 24 bit rgb mode */
#define ZB_NB_COLORS 225 /* number of colors for 8 bit display */
#if 0
#define RGB_TO_PIXEL15(pp, r,g,b) (*(unsigned short*)pp = (((r) >> 1) & 0x7c00) | (((g) >> 6) & 0x03e0) | ((b) >> 11))
#define RGB_TO_PIXEL16(pp, r,g,b) (*(unsigned short*)pp = ((r) & 0xF800) | (((g) >> 5) & 0x07E0) | ((b) >> 11))
#define RGB_TO_PIXEL24(pp, r,g,b) ((pp)[0] = r, (pp)[1] = g, (pp)[2] = b)
#define RGB_TO_PIXEL32(pp, r,g,b) (*(unsigned int*)pp = (((r) << 8) & 0xff0000) | ((g) & 0xff00) | ((b) >> 8))
#else
#define RGB_TO_PIXEL15(pp, r,g,b) (*(unsigned short*)pp = (((b) >> 1) & 0x7c00) | (((g) >> 6) & 0x03e0) | ((r) >> 11))
#define RGB_TO_PIXEL16(pp, r,g,b) (*(unsigned short*)pp = ((b) & 0xF800) | (((g) >> 5) & 0x07E0) | ((r) >> 11))
#define RGB_TO_PIXEL24(pp, r,g,b) ((pp)[0] = b, (pp)[1] = g, (pp)[2] = r)
#define RGB_TO_PIXEL32(pp, r,g,b) (*(unsigned int*)pp = (((b) << 8) & 0xff0000) | ((g) & 0xff00) | ((r) >> 8))
#endif
typedef struct GLSpecBuf {
  int shininess_i;
  int last_used;
  float buf[SPECULAR_BUFFER_SIZE + 1];
  struct GLSpecBuf* next;
} GLSpecBuf;
typedef struct GLLight {
  V4 ambient;
  V4 diffuse;
  V4 specular;
  V4 position;
  V3 spot_direction;
  float spot_exponent;
  float spot_cutoff;
  float attenuation[3];
  /* precomputed values */
  float cos_spot_cutoff;
  V3 norm_spot_direction;
  V3 norm_position;
  /* we use a linked list to know which are the enabled lights */
  int enabled;
  struct GLLight* next, *prev;
} GLLight;
typedef struct GLMaterial {
  V4 emission;
  V4 ambient;
  V4 diffuse;
  V4 specular;
  float shininess;
  /* computed values */
  int shininess_i;
  int do_specular;
} GLMaterial;
typedef struct GLViewport {
  int xmin, ymin, w, h;
  V3 scale;
  V3 trans;
  int updated;
} GLViewport;
typedef union {
  int op;
  float f;
  int i;
  unsigned int ui;
  void* p;
} GLParam;
typedef struct GLParamBuffer {
  GLParam ops[OP_BUFFER_MAX_SIZE];
  struct GLParamBuffer* next;
} GLParamBuffer;
typedef struct GLList {
  GLParamBuffer* first_op_buffer;
  /* TODO: extensions for an hash table or a better allocating scheme */
} GLList;
typedef struct {
  int x, y, z; /* integer coordinates in the zbuffer */
  int s, t;  /* coordinates for the mapping */
  int r, g, b; /* color indexes */
  int sz, tz; /* temporary coordinates for mapping */
} ZBufferPoint;
typedef struct GLVertex {
  int edge_flag;
  V3 normal;
  V4 coord;
  V4 tex_coord;
  V4 color;
  /* computed values */
  V4 ec;    /* eye coordinates */
  V4 pc;    /* coordinates in the normalized volume */
  int clip_code;  /* clip code */
  ZBufferPoint zp;  /* integer coordinates for the rasterization */
} GLVertex;
typedef struct GLImage {
  void* pixmap;
  int w, h;
} GLImage;
/* textures */
#define TEXTURE_HASH_TABLE_SIZE 256
typedef struct GLTexture {
  GLImage images[MAX_TEXTURE_LEVELS];
  int handle;
  struct GLTexture* next, *prev;
} GLTexture;
/* shared state */
typedef struct GLSharedState {
  GLList** lists;
  GLTexture** texture_hash_table;
} GLSharedState;
typedef void (*gl_draw_triangle_func)(struct GLContext* c, GLVertex* p0, GLVertex* p1, GLVertex* p2);
typedef struct ZBuffer {
  int h, w, step, cn, bpp;
  uchar* pbuf;
  uchar* current_texture;
  unsigned short* zbuf;
} ZBuffer;
typedef struct GLContext {
  /* buffer */
  ZBuffer zb[1];
  /* lights */
  GLLight lights[MAX_LIGHTS];
  GLLight* first_light;
  V4 ambient_light_model;
  int local_light_model;
  int lighting_enabled;
  int light_model_two_side;
  /* materials */
  GLMaterial materials[2];
  int color_material_enabled;
  int current_color_material_mode;
  int current_color_material_type;
  /* textures */
  GLTexture* current_texture;
  int texture_2d_enabled;
  /* shared state */
  GLSharedState shared_state;
  /* current list */
  GLParamBuffer* current_op_buffer;
  int current_op_buffer_index;
  int exec_flag, compile_flag, print_flag;
  /* matrix */
  int matrix_mode;
  M4* matrix_stack[3];
  M4* matrix_stack_ptr[3];
  int matrix_stack_depth_max[3];
  M4 matrix_model_view_inv;
  M4 matrix_model_projection;
  int matrix_model_projection_updated;
  int matrix_model_projection_no_w_transform;
  int apply_texture_matrix;
  /* viewport */
  GLViewport viewport;
  /* current state */
  int polygon_mode_back;
  int polygon_mode_front;
  int current_front_face;
  int current_shade_model;
  int current_cull_face;
  int cull_face_enabled;
  int normalize_enabled;
  gl_draw_triangle_func draw_triangle_front, draw_triangle_back;
  /* selection */
  int render_mode;
  unsigned int* select_buffer;
  int select_size;
  unsigned int* select_ptr, *select_hit;
  int select_overflow;
  int select_hits;
  /* names */
  unsigned int name_stack[MAX_NAME_STACK_DEPTH];
  int name_stack_size;
  /* clear */
  float clear_depth;
  V4 clear_color;
  /* current vertex state */
  V4 current_color;
  unsigned int longcurrent_color[3]; /* precomputed integer color */
  V4 current_normal;
  V4 current_tex_coord;
  int current_edge_flag;
  /* glBegin / glEnd */
  int in_begin;
  int begin_type;
  int vertex_n, vertex_cnt;
  int vertex_max;
  GLVertex* vertex;
  /* opengl 1.1 arrays */
  float* vertex_array;
  int vertex_array_size;
  int vertex_array_stride;
  float* normal_array;
  int normal_array_stride;
  float* color_array;
  int color_array_size;
  int color_array_stride;
  float* texcoord_array;
  int texcoord_array_size;
  int texcoord_array_stride;
  int client_states;
  /* opengl 1.1 polygon offset */
  float offset_factor;
  float offset_units;
  int offset_states;
  /* specular buffer. could probably be shared between contexts,
  but that wouldn't be 100% thread safe */
  GLSpecBuf* specbuf_first;
  int specbuf_used_counter;
  int specbuf_num_buffers;
  /* opaque structure for user's use */
  void* opaque;
  /* resize viewport function */
  //int (*gl_resize_viewport)(struct GLContext* c, int* w, int* h);
  /* depth test */
  int depth_test;
  int point_size;
} GLContext;
static int dprintf(const char* fmt, ...)
{
  va_list ARGLIST;
  va_start(ARGLIST, fmt);
  vfprintf(stderr, fmt, ARGLIST);
  va_end(ARGLIST);
  //vfprintf(stderr,"In '%s': " format "\n",__FUNCTION__, ##args);
  return 0;
}
/* this clip epsilon is needed to avoid some rounding errors after
 several clipping stages */
#define CLIP_EPSILON (1E-5)
static CC_INLINE int gl_clipcode(float x, float y, float z, float w1)
{
  float w;
  w = (float)(w1 * (1.0 + CLIP_EPSILON));
  return (x < -w) | ((x > w) << 1) | ((y < -w) << 2) | ((y > w) << 3) | ((z < -w) << 4) | ((z > w) << 5);
}
void gl_fatal_error(char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  fprintf(stderr, "TinyGL: fatal error: ");
  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  exit(1);
  va_end(ap);
}
#define NDEBUG
#ifdef NDEBUG
#define NO_DEBUG_OUTPUT
#endif
/* Use this function to output messages when something unexpected
 happens (which might be an indication of an error). *Don't* use it
 when there's internal errors in the code - these should be handled
 by asserts. */
void tgl_warning(const char* format, ...)
{
#ifndef NO_DEBUG_OUTPUT
  va_list args;
  va_start(args, format);
  fprintf(stderr, "*WARNING* ");
  vfprintf(stderr, format, args);
  va_end(args);
#endif /* !NO_DEBUG_OUTPUT */
}
/* This function should be used for debug output only. */
void tgl_trace(const char* format, ...)
{
#ifndef NO_DEBUG_OUTPUT
  va_list args;
  va_start(args, format);
  fprintf(stderr, "*DEBUG* ");
  vfprintf(stderr, format, args);
  va_end(args);
#endif /* !NO_DEBUG_OUTPUT */
}
/* Use this function to output info about things in the code which
 should be fixed (missing handling of special cases, important
 features not implemented, known bugs/buglets, ...). */
void tgl_fixme(const char* format, ...)
{
#ifndef NO_DEBUG_OUTPUT
  va_list args;
  va_start(args, format);
  fprintf(stderr, "*FIXME* ");
  vfprintf(stderr, format, args);
  va_end(args);
#endif /* !NO_DEBUG_OUTPUT */
}
void glopTexCoord(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopEdgeFlag(GLContext* c, GLParam* p)
{
  c->current_edge_flag = p[1].i;
}
void glopNormal(GLContext* c, GLParam* p)
{
  V3 v;
  v[0] = p[1].f;
  v[1] = p[2].f;
  v[2] = p[3].f;
  c->current_normal[0] = v[0];
  c->current_normal[1] = v[1];
  c->current_normal[2] = v[2];
  c->current_normal[3] = 0;
}
void gl_eval_viewport(GLContext* c)
{
  GLViewport* v;
  GLfloat zsize = (1 << (ZB_Z_BITS + ZB_POINT_Z_FRAC_BITS));
  v = &c->viewport;
  v->trans[0] = (float)((v->w - 0.5) / 2.0) + v->xmin;
  v->trans[1] = (float)((v->h - 0.5) / 2.0) + v->ymin;
  v->trans[2] = (float)((zsize - 0.5) / 2.0) + ((1 << ZB_POINT_Z_FRAC_BITS)) / 2;
  v->scale[0] = (float)(v->w - 0.5f) / 2.0f;
  v->scale[1] = -(v->h - 0.5f) / 2.0f;
  v->scale[2] = -((zsize - 0.5f) / 2.0f);
}
#define CLIP_XMIN (1<<0)
#define CLIP_XMAX (1<<1)
#define CLIP_YMIN (1<<2)
#define CLIP_YMAX (1<<3)
#define CLIP_ZMIN (1<<4)
#define CLIP_ZMAX (1<<5)
void gl_transform_to_viewport(GLContext* c, GLVertex* v)
{
  GLfloat winv;
  /* coordinates */
  winv = 1.0f / v->pc[3];
  v->zp.x = (int)(v->pc[0] * winv * c->viewport.scale[0] + c->viewport.trans[0]);
  v->zp.y = (int)(v->pc[1] * winv * c->viewport.scale[1] + c->viewport.trans[1]);
  v->zp.z = (int)(v->pc[2] * winv * c->viewport.scale[2] + c->viewport.trans[2]);

  /* color */
  if (c->lighting_enabled) {
    v->zp.r = (int)(v->color[0] * (ZB_POINT_RED_MAX - ZB_POINT_RED_MIN) + ZB_POINT_RED_MIN);
    v->zp.g = (int)(v->color[1] * (ZB_POINT_GREEN_MAX - ZB_POINT_GREEN_MIN) + ZB_POINT_GREEN_MIN);
    v->zp.b = (int)(v->color[2] * (ZB_POINT_BLUE_MAX - ZB_POINT_BLUE_MIN) + ZB_POINT_BLUE_MIN);
  }
  else {
    /* no need to convert to integer if no lighting : take current color */
    v->zp.r = c->longcurrent_color[0];
    v->zp.g = c->longcurrent_color[1];
    v->zp.b = c->longcurrent_color[2];
  }

  /* texture */
  if (c->texture_2d_enabled) {
    v->zp.s = (int)(v->tex_coord[0] * (ZB_POINT_S_MAX - ZB_POINT_S_MIN) + ZB_POINT_S_MIN);
    v->zp.t = (int)(v->tex_coord[1] * (ZB_POINT_T_MAX - ZB_POINT_T_MIN) + ZB_POINT_T_MIN);
  }
}
int glopRenderMode(GLContext* c, int mode)
{
  int result = 0;

  switch (c->render_mode) {
  case GL_RENDER:
    break;

  case GL_SELECT:
    if (c->select_overflow) {
      result = -c->select_hits;
    }
    else {
      result = c->select_hits;
    }

    c->select_overflow = 0;
    c->select_ptr = c->select_buffer;
    c->name_stack_size = 0;
    break;

  default:
    assert(0);
  }

  switch (mode) {
  case GL_RENDER:
    c->render_mode = GL_RENDER;
    break;

  case GL_SELECT:
    c->render_mode = GL_SELECT;
    assert(c->select_buffer != NULL);
    c->select_ptr = c->select_buffer;
    c->select_hits = 0;
    c->select_overflow = 0;
    c->select_hit = NULL;
    break;

  default:
    assert(0);
  }

  return result;
}
void APIENTRY glopSelectBuffer(GLContext* c, int size, unsigned int* buf)
{
  assert(c->render_mode != GL_SELECT);
  c->select_buffer = buf;
  c->select_size = size;
}
void glopInitNames(GLContext* c, GLParam* p)
{
  if (c->render_mode == GL_SELECT) {
    c->name_stack_size = 0;
    c->select_hit = NULL;
  }
}
void glopPushName(GLContext* c, GLParam* p)
{
  if (c->render_mode == GL_SELECT) {
    assert(c->name_stack_size < MAX_NAME_STACK_DEPTH);
    c->name_stack[c->name_stack_size++] = p[1].i;
    c->select_hit = NULL;
  }
}
void glopPopName(GLContext* c, GLParam* p)
{
  if (c->render_mode == GL_SELECT) {
    assert(c->name_stack_size > 0);
    c->name_stack_size--;
    c->select_hit = NULL;
  }
}
void glopLoadName(GLContext* c, GLParam* p)
{
  if (c->render_mode == GL_SELECT) {
    assert(c->name_stack_size > 0);
    c->name_stack[c->name_stack_size - 1] = p[1].i;
    c->select_hit = NULL;
  }
}
void gl_add_select(GLContext* c, unsigned int zmin, unsigned int zmax)
{
  unsigned int* ptr;
  int n, i;

  if (!c->select_overflow) {
    if (c->select_hit == NULL) {
      n = c->name_stack_size;

      if ((c->select_ptr - c->select_buffer + 3 + n) >
          c->select_size) {
        c->select_overflow = 1;
      }
      else {
        ptr = c->select_ptr;
        c->select_hit = ptr;
        *ptr++ = c->name_stack_size;
        *ptr++ = zmin;
        *ptr++ = zmax;

        for (i = 0; i < n; i++) {
          *ptr++ = c->name_stack[i];
        }

        c->select_ptr = ptr;
        c->select_hits++;
      }
    }
    else {
      if (zmin < c->select_hit[1]) {
        c->select_hit[1] = zmin;
      }

      if (zmax > c->select_hit[2]) {
        c->select_hit[2] = zmax;
      }
    }
  }
}
static void gl_add_select1(GLContext* c, int z1, int z2, int z3)
{
  int min, max;
  min = max = z1;

  if (z2 < min) {
    min = z2;
  }

  if (z3 < min) {
    min = z3;
  }

  if (z2 > max) {
    max = z2;
  }

  if (z3 > max) {
    max = z3;
  }

  gl_add_select(c, 0xffffffff - min, 0xffffffff - max);
}
#define ZCMP(z,zpix) ((z) >= (zpix))
void ZB_point(ZBuffer* zb, ZBufferPoint* p, int point_size)
{
  int zz;

  switch (zb->cn) {
  case 4: {
#define PUT_PIX() RGB_TO_PIXEL32(pp, p->r, p->g, p->b);
#include "zpoint_impl.inl"
  }
  break;

  case 3: {
    int r = p->r >> 8, g = p->g >> 8, b = p->b >> 8;
#define PUT_PIX() RGB_TO_PIXEL24(pp, r, g, b)
#include "zpoint_impl.inl"
  }
  break;

  case 2:
#define PUT_PIX() RGB_TO_PIXEL16(pp, p->r, p->g, p->b);
#include "zpoint_impl.inl"
    break;
  }
}
static void ZB_line_flat_z(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2, int color)
{
#define INTERP_Z
#include "zline_impl_cn.inl"
}
/* line with color interpolation */
static void ZB_line_interp_z(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2)
{
#define INTERP_Z
#define INTERP_RGB
#include "zline_impl_cn.inl"
}
/* no Z interpolation */
static void ZB_line_flat(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2, int color)
{
#include "zline_impl_cn.inl"
}
#define INTERP_RGB
static void ZB_line_interp(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2)
{
#include "zline_impl_cn.inl"
}
void ZB_line_z(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2)
{
  int color1, color2;
  RGB_TO_PIXEL32(&color1, p1->r, p1->g, p1->b);
  RGB_TO_PIXEL32(&color2, p2->r, p2->g, p2->b);

  /* choose if the line should have its color interpolated or not */
  if (color1 == color2) {
    ZB_line_flat_z(zb, p1, p2, color1);
  }
  else {
    ZB_line_interp_z(zb, p1, p2);
  }
}
void ZB_line(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2)
{
  int color1, color2;
  RGB_TO_PIXEL32(&color1, p1->r, p1->g, p1->b);
  RGB_TO_PIXEL32(&color2, p2->r, p2->g, p2->b);

  /* choose if the line should have its color interpolated or not */
  if (color1 == color2) {
    ZB_line_flat(zb, p1, p2, color1);
  }
  else {
    ZB_line_interp(zb, p1, p2);
  }
}
/* point */
void gl_draw_point(GLContext* c, GLVertex* p0)
{
  if (p0->clip_code == 0) {
    if (c->render_mode == GL_SELECT) {
      gl_add_select(c, p0->zp.z, p0->zp.z);
    }
    else {
      ZB_point(c->zb, &p0->zp, c->point_size);
    }
  }
}
/* line */
static CC_INLINE void interpolate(GLVertex* q, GLVertex* p0, GLVertex* p1, GLfloat t)
{
  q->pc[0] = p0->pc[0] + (p1->pc[0] - p0->pc[0]) * t;
  q->pc[1] = p0->pc[1] + (p1->pc[1] - p0->pc[1]) * t;
  q->pc[2] = p0->pc[2] + (p1->pc[2] - p0->pc[2]) * t;
  q->pc[3] = p0->pc[3] + (p1->pc[3] - p0->pc[3]) * t;
  q->color[0] = p0->color[0] + (p1->color[0] - p0->color[0]) * t;
  q->color[1] = p0->color[1] + (p1->color[1] - p0->color[1]) * t;
  q->color[2] = p0->color[2] + (p1->color[2] - p0->color[2]) * t;
}
/*
 * Line Clipping
 */
/* Line Clipping algorithm from 'Computer Graphics', Principles and
 Practice */
static CC_INLINE int ClipLine1(GLfloat denom, GLfloat num, GLfloat* tmin, GLfloat* tmax)
{
  GLfloat t;

  if (denom > 0) {
    t = num / denom;

    if (t > *tmax) {
      return 0;
    }

    if (t > *tmin) {
      *tmin = t;
    }
  }
  else if (denom < 0) {
    t = num / denom;

    if (t < *tmin) {
      return 0;
    }

    if (t < *tmax) {
      *tmax = t;
    }
  }
  else if (num > 0) {
    return 0;
  }

  return 1;
}
void gl_draw_line(GLContext* c, GLVertex* p1, GLVertex* p2)
{
  GLfloat dx, dy, dz, dw, x1, y1, z1, w1;
  GLfloat tmin, tmax;
  GLVertex q1, q2;
  int cc1, cc2;
  cc1 = p1->clip_code;
  cc2 = p2->clip_code;

  if ((cc1 | cc2) == 0) {
    if (c->render_mode == GL_SELECT) {
      gl_add_select1(c, p1->zp.z, p2->zp.z, p2->zp.z);
    }
    else {
      if (c->depth_test) {
        ZB_line_z(c->zb, &p1->zp, &p2->zp);
      }
      else {
        ZB_line(c->zb, &p1->zp, &p2->zp);
      }
    }
  }
  else if ((cc1 & cc2) != 0) {
    return;
  }
  else {
    dx = p2->pc[0] - p1->pc[0];
    dy = p2->pc[1] - p1->pc[1];
    dz = p2->pc[2] - p1->pc[2];
    dw = p2->pc[3] - p1->pc[3];
    x1 = p1->pc[0];
    y1 = p1->pc[1];
    z1 = p1->pc[2];
    w1 = p1->pc[3];
    tmin = 0;
    tmax = 1;

    if (ClipLine1(dx + dw, -x1 - w1, &tmin, &tmax) &&
        ClipLine1(-dx + dw, x1 - w1, &tmin, &tmax) &&
        ClipLine1(dy + dw, -y1 - w1, &tmin, &tmax) &&
        ClipLine1(-dy + dw, y1 - w1, &tmin, &tmax) &&
        ClipLine1(dz + dw, -z1 - w1, &tmin, &tmax) &&
        ClipLine1(-dz + dw, z1 - w1, &tmin, &tmax)) {
      interpolate(&q1, p1, p2, tmin);
      interpolate(&q2, p1, p2, tmax);
      gl_transform_to_viewport(c, &q1);
      gl_transform_to_viewport(c, &q2);

      if (c->depth_test) {
        ZB_line_z(c->zb, &q1.zp, &q2.zp);
      }
      else {
        ZB_line(c->zb, &q1.zp, &q2.zp);
      }
    }
  }
}
/* triangle */
/*
 * Clipping
 */
/* We clip the segment [a,b] against the 6 planes of the normal volume.
 * We compute the point 'c' of intersection and the value of the parameter 't'
 * of the intersection if x=a+t(b-a).
 */
#define clip_func(name,sign,dir,dir1,dir2) \
  static GLfloat name(V4 c,V4 a,V4 b) \
  {\
    GLfloat t,d[4],den;\
    d[0] = (b[0] - a[0]);\
    d[1] = (b[1] - a[1]);\
    d[2] = (b[2] - a[2]);\
    d[3] = (b[3] - a[3]);\
    den = -(sign d[dir]) + d[3];\
    if (den == 0) t=0;\
    else t = ( sign a[dir] - a[3]) / den;\
    c[dir1] = a[dir1] + t * d[dir1];\
    c[dir2] = a[dir2] + t * d[dir2];\
    c[3] = a[3] + t * d[3];\
    c[dir] = sign c[3];\
    return t;\
  }
clip_func(clip_xmin, -, 0, 1, 2)
clip_func(clip_xmax, +, 0, 1, 2)
clip_func(clip_ymin, -, 1, 0, 2)
clip_func(clip_ymax, +, 1, 0, 2)
clip_func(clip_zmin, -, 2, 0, 1)
clip_func(clip_zmax, +, 2, 0, 1)
GLfloat(*clip_proc[6])(V4, V4, V4) = {
  clip_xmin, clip_xmax,
  clip_ymin, clip_ymax,
  clip_zmin, clip_zmax
};
static CC_INLINE void updateTmp(GLContext* c, GLVertex* q, GLVertex* p0, GLVertex* p1, GLfloat t)
{
  if (c->current_shade_model == GL_SMOOTH) {
    q->color[0] = p0->color[0] + (p1->color[0] - p0->color[0]) * t;
    q->color[1] = p0->color[1] + (p1->color[1] - p0->color[1]) * t;
    q->color[2] = p0->color[2] + (p1->color[2] - p0->color[2]) * t;
  }
  else {
    q->color[0] = p0->color[0];
    q->color[1] = p0->color[1];
    q->color[2] = p0->color[2];
  }

  if (c->texture_2d_enabled) {
    q->tex_coord[0] = p0->tex_coord[0] + (p1->tex_coord[0] - p0->tex_coord[0]) * t;
    q->tex_coord[1] = p0->tex_coord[1] + (p1->tex_coord[1] - p0->tex_coord[1]) * t;
  }

  q->clip_code = gl_clipcode(q->pc[0], q->pc[1], q->pc[2], q->pc[3]);

  if (q->clip_code == 0) {
    gl_transform_to_viewport(c, q);
  }
}
static void gl_draw_triangle_clip(GLContext* c,
    GLVertex* p0, GLVertex* p1, GLVertex* p2, int clip_bit);
void gl_draw_triangle(GLContext* c, GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
  int co, c_and, cc[3], front;
  GLfloat norm;
  cc[0] = p0->clip_code;
  cc[1] = p1->clip_code;
  cc[2] = p2->clip_code;
  co = cc[0] | cc[1] | cc[2];

  /* we handle the non clipped case here to go faster */
  if (co == 0) {
    norm = (GLfloat)(p1->zp.x - p0->zp.x) * (GLfloat)(p2->zp.y - p0->zp.y) -
        (GLfloat)(p2->zp.x - p0->zp.x) * (GLfloat)(p1->zp.y - p0->zp.y);

    if (norm == 0) {
      return;
    }

    front = norm < 0.0;
    front = front ^ c->current_front_face;

    /* back face culling */
    if (c->cull_face_enabled) {
      /* most used case first */
      if (c->current_cull_face == GL_BACK) {
        if (front == 0) {
          return;
        }

        c->draw_triangle_front(c, p0, p1, p2);
      }
      else if (c->current_cull_face == GL_FRONT) {
        if (front != 0) {
          return;
        }

        c->draw_triangle_back(c, p0, p1, p2);
      }
      else {
        return;
      }
    }
    else {
      /* no culling */
      if (front) {
        c->draw_triangle_front(c, p0, p1, p2);
      }
      else {
        c->draw_triangle_back(c, p0, p1, p2);
      }
    }
  }
  else {
    c_and = cc[0] & cc[1] & cc[2];

    if (c_and == 0) {
      gl_draw_triangle_clip(c, p0, p1, p2, 0);
    }
  }
}
static void gl_draw_triangle_clip(GLContext* c, GLVertex* p0, GLVertex* p1, GLVertex* p2, int clip_bit)
{
  int co, c_and, co1, cc[3], edge_flag_tmp, clip_mask;
  GLVertex tmp1, tmp2, *q[3];
  GLfloat tt;
  cc[0] = p0->clip_code;
  cc[1] = p1->clip_code;
  cc[2] = p2->clip_code;
  co = cc[0] | cc[1] | cc[2];

  if (co == 0) {
    gl_draw_triangle(c, p0, p1, p2);
  }
  else {
    c_and = cc[0] & cc[1] & cc[2];

    /* the triangle is completely outside */
    if (c_and != 0) {
      return;
    }

    /* find the next direction to clip */
    while (clip_bit < 6 && (co & (1 << clip_bit)) == 0) {
      clip_bit++;
    }

    /* this test can be true only in case of rounding errors */
    if (clip_bit == 6) {
#if 0
      printf("Error:\n");
      printf("%f %f %f %f\n", p0->pc[0], p0->pc[1], p0->pc[2], p0->pc[3]);
      printf("%f %f %f %f\n", p1->pc[0], p1->pc[1], p1->pc[2], p1->pc[3]);
      printf("%f %f %f %f\n", p2->pc[0], p2->pc[1], p2->pc[2], p2->pc[3]);
#endif
      return;
    }

    clip_mask = 1 << clip_bit;
    co1 = (cc[0] ^ cc[1] ^ cc[2]) & clip_mask;

    if (co1) {
      /* one point outside */
      if (cc[0] & clip_mask) {
        q[0] = p0;
        q[1] = p1;
        q[2] = p2;
      }
      else if (cc[1] & clip_mask) {
        q[0] = p1;
        q[1] = p2;
        q[2] = p0;
      }
      else {
        q[0] = p2;
        q[1] = p0;
        q[2] = p1;
      }

      tt = clip_proc[clip_bit](tmp1.pc, q[0]->pc, q[1]->pc);
      updateTmp(c, &tmp1, q[0], q[1], tt);
      tt = clip_proc[clip_bit](tmp2.pc, q[0]->pc, q[2]->pc);
      updateTmp(c, &tmp2, q[0], q[2], tt);
      tmp1.edge_flag = q[0]->edge_flag;
      edge_flag_tmp = q[2]->edge_flag;
      q[2]->edge_flag = 0;
      gl_draw_triangle_clip(c, &tmp1, q[1], q[2], clip_bit + 1);
      tmp2.edge_flag = 1;
      tmp1.edge_flag = 0;
      q[2]->edge_flag = edge_flag_tmp;
      gl_draw_triangle_clip(c, &tmp2, &tmp1, q[2], clip_bit + 1);
    }
    else {
      /* two points outside */
      if ((cc[0] & clip_mask) == 0) {
        q[0] = p0;
        q[1] = p1;
        q[2] = p2;
      }
      else if ((cc[1] & clip_mask) == 0) {
        q[0] = p1;
        q[1] = p2;
        q[2] = p0;
      }
      else {
        q[0] = p2;
        q[1] = p0;
        q[2] = p1;
      }

      tt = clip_proc[clip_bit](tmp1.pc, q[0]->pc, q[1]->pc);
      updateTmp(c, &tmp1, q[0], q[1], tt);
      tt = clip_proc[clip_bit](tmp2.pc, q[0]->pc, q[2]->pc);
      updateTmp(c, &tmp2, q[0], q[2], tt);
      tmp1.edge_flag = 1;
      tmp2.edge_flag = q[2]->edge_flag;
      gl_draw_triangle_clip(c, q[0], &tmp1, &tmp2, clip_bit + 1);
    }
  }
}
void gl_draw_triangle_select(GLContext* c,
    GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
  gl_add_select1(c, p0->zp.z, p1->zp.z, p2->zp.z);
}
#ifdef PROFILE
int count_triangles, count_triangles_textured, count_pixels;
#endif
#define ZCMP(z,zpix) ((z) >= (zpix))
void ZB_fillTriangleFlat(ZBuffer* zb, ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
  switch (zb->cn) {
  case 3: {
    enum {PSZB = 3};
    typedef uchar PIXEL;
    unsigned char colorR, colorG, colorB;
#define INTERP_Z
#define DRAW_INIT() { colorR=p2->r>>8; colorG=p2->g>>8; colorB=p2->b>>8; }
#define PUT_PIXEL(_a) \
{   \
  zz=z >> ZB_POINT_Z_FRAC_BITS; \
  if (ZCMP(zz,pz[_a])) { uchar* pp1 = pp+3 * _a; \
    RGB_TO_PIXEL24(pp1, colorR, colorG, colorB); \
    pz[_a]=zz;  \
  }\
  z+=dzdx;   \
}
#include "ztriangle_impl.1inl"
  }
  break;

  case 2: {
    enum {PSZB = 2};
    typedef ushort PIXEL;
    PIXEL color;
#define INTERP_Z
#define DRAW_INIT() { RGB_TO_PIXEL16(&color, p2->r,p2->g,p2->b); }
#define PUT_PIXEL(_a) { zz=z >> ZB_POINT_Z_FRAC_BITS; if (ZCMP(zz,pz[_a])) { pp[_a]=color; pz[_a]=zz; } z+=dzdx; }
#include "ztriangle_impl.1inl"
  }
  break;
  }
}
/*
 * Smooth filled triangle.
 * The code below is very tricky :)
 */
void ZB_fillTriangleSmooth(ZBuffer* zb, ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
#define SAR_RND_TO_ZERO(v,n) (v / (1<<n))

  switch (zb->cn) {
  case 3: {
    enum {PSZB = 3};
    typedef uchar PIXEL;
#define INTERP_Z
#define INTERP_RGB
#define DRAW_INIT() {}
#define PUT_PIXEL(_a)  \
{   \
  zz=z >> ZB_POINT_Z_FRAC_BITS; \
  if (ZCMP(zz,pz[_a])) { uchar* pp1 = pp+3 * _a; \
    RGB_TO_PIXEL24(pp1, or1 >> 8, og1 >> 8, ob1 >> 8); \
    pz[_a]=zz;  \
  }\
  z+=dzdx;   \
  og1+=dgdx;   \
  or1+=drdx;   \
  ob1+=dbdx;   \
}
#include "ztriangle_impl.1inl"
  }
  break;

  case 2: {
    int _drgbdx;
    enum {PSZB = 2};
    typedef ushort PIXEL;
#define INTERP_Z
#define INTERP_RGB
#define DRAW_INIT() {   \
  _drgbdx=(SAR_RND_TO_ZERO(drdx,6) << 22) & 0xFFC00000; \
  _drgbdx|=SAR_RND_TO_ZERO(dgdx,5) & 0x000007FF; \
  _drgbdx|=(SAR_RND_TO_ZERO(dbdx,7) << 12) & 0x001FF000; \
}
#define PUT_PIXEL(_a)  \
{   \
  zz=z >> ZB_POINT_Z_FRAC_BITS; \
  if (ZCMP(zz,pz[_a])) {  \
    tmp=rgb & 0xF81F07E0;  \
    pp[_a]=tmp | (tmp >> 16);  \
    pz[_a]=zz;  \
  }   \
  z+=dzdx;   \
  rgb=(rgb+drgbdx) & ( ~ 0x00200800); \
}
#define DRAW_LINE()    \
{     \
  register unsigned short *pz;    \
  register PIXEL *pp;   \
  register unsigned int tmp,z,zz,rgb,drgbdx;   \
  register int n;    \
  n=(x2 >> 16) - x1;     \
  pp=pp1+x1;     \
  pz=pz1+x1;     \
  z=z1;     \
  rgb=(r1 << 16) & 0xFFC00000;    \
  rgb|=(g1 >> 5) & 0x000007FF;    \
  rgb|=(b1 << 5) & 0x001FF000;    \
  drgbdx=_drgbdx;    \
  while (n>=3) {     \
    PUT_PIXEL(0);    \
    PUT_PIXEL(1);    \
    PUT_PIXEL(2);    \
    PUT_PIXEL(3);    \
    pz+=4;     \
    pp+=4;     \
    n-=4;     \
  }     \
  while (n>=0) {     \
    PUT_PIXEL(0);    \
    pz+=1;     \
    pp+=1;     \
    n-=1;     \
  }     \
}
#include "ztriangle_impl.1inl"
  }
  break;

  case 4: {
    enum {PSZB = 4};
    typedef uint PIXEL;
#define INTERP_Z
#define INTERP_RGB
#define DRAW_INIT()   \
{   \
}
#define PUT_PIXEL(_a)  \
{   \
  zz=z >> ZB_POINT_Z_FRAC_BITS; \
  if (ZCMP(zz,pz[_a])) {  \
    RGB_TO_PIXEL32((pp+_a), or1, og1, ob1);\
    pz[_a]=zz;  \
  }\
  z+=dzdx;   \
  og1+=dgdx;   \
  or1+=drdx;   \
  ob1+=dbdx;   \
}
#include "ztriangle_impl.1inl"
  }
  break;
  }
}
void ZB_setTexture(ZBuffer* zb, uchar* texture)
{
  zb->current_texture = texture;
}
void ZB_fillTriangleMapping(ZBuffer* zb,
    ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
  switch (zb->cn) {
  case 3: {
    enum {PSZB = 3};
    typedef uchar PIXEL;
    PIXEL* texture;
#define INTERP_Z
#define INTERP_ST
#define DRAW_INIT() { texture=(PIXEL*)zb->current_texture; }
#define PUT_PIXEL(_a) {   \
  unsigned char *ptr;\
  zz=z >> ZB_POINT_Z_FRAC_BITS; \
  if (ZCMP(zz,pz[_a])) { uchar* pp1 = pp + 3*_a;  \
    ptr = texture + (((t & 0x3FC00000) | s) >> 14) * 3; \
    pp1[0]= ptr[0]; pp1[1]= ptr[1]; pp1[2]= ptr[2];\
    pz[_a]=zz;  \
  }   \
  z+=dzdx;   \
  s+=dsdx;   \
  t+=dtdx;   \
}
#include "ztriangle_impl.1inl"
  }
  break;

  case 2: {
    enum {PSZB = 2};
    typedef ushort PIXEL;
    PIXEL* texture;
#define INTERP_Z
#define INTERP_ST
#define DRAW_INIT() { texture=(PIXEL*)zb->current_texture; }
#define PUT_PIXEL(_a) {   \
  zz=z >> ZB_POINT_Z_FRAC_BITS; \
  if (ZCMP(zz,pz[_a])) {  \
    pp[_a]=texture[((t & 0x3FC00000) | s) >> 14]; \
    pz[_a]=zz;  \
  }   \
  z+=dzdx;   \
  s+=dsdx;   \
  t+=dtdx;   \
}
#include "ztriangle_impl.1inl"
  }
  break;
  }
}
/*
 * Texture mapping with perspective correction.
 * We use the gradient method to make less divisions.
 * TODO: pipeline the division
 */
void ZB_fillTriangleMappingPerspective(ZBuffer* zb, ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
  GLfloat fdzdx, fndzdx, ndszdx, ndtzdx;
#define NB_INTERP 8
#define DRAW_INIT1()  \
  {   \
    texture=(PIXEL*)zb->current_texture;\
    fdzdx=(GLfloat)dzdx;\
    fndzdx=NB_INTERP * fdzdx;\
    ndszdx=NB_INTERP * dszdx;\
    ndtzdx=NB_INTERP * dtzdx;\
  }
#define DRAW_LINE1()  \
  {   \
    register unsigned short *pz; \
    register PIXEL *pp; \
    register unsigned int s,t,z,zz; \
    register int n,dsdx,dtdx; \
    GLfloat sz,tz,fz,zinv; \
    n=(x2>>16)-x1;        \
    fz=(GLfloat)z1;\
    zinv=1.0f / fz;\
    pp=(PIXEL *)((char *)pp1 + x1 * PSZB); \
    pz=pz1+x1;   \
    z=z1;   \
    sz=sz1;\
    tz=tz1;\
    while (n>=(NB_INTERP-1)) {    \
      {\
        GLfloat ss,tt;\
        ss=(sz * zinv);\
        tt=(tz * zinv);\
        s=(int) ss;\
        t=(int) tt;\
        dsdx= (int)( (dszdx - ss*fdzdx)*zinv );\
        dtdx= (int)( (dtzdx - tt*fdzdx)*zinv );\
        fz+=fndzdx;\
        zinv=1.0f / fz;\
      }\
      PUT_PIXEL(0);    \
      PUT_PIXEL(1);    \
      PUT_PIXEL(2);    \
      PUT_PIXEL(3);    \
      PUT_PIXEL(4);    \
      PUT_PIXEL(5);    \
      PUT_PIXEL(6);    \
      PUT_PIXEL(7);    \
      pz+=NB_INTERP;     \
      pp=(PIXEL *)((char *)pp + NB_INTERP * PSZB);\
      n-=NB_INTERP;    \
      sz+=ndszdx;\
      tz+=ndtzdx;\
    }     \
    {\
      GLfloat ss,tt;\
      ss=(sz * zinv);\
      tt=(tz * zinv);\
      s=(int) ss;\
      t=(int) tt;\
      dsdx= (int)( (dszdx - ss*fdzdx)*zinv );\
      dtdx= (int)( (dtzdx - tt*fdzdx)*zinv );\
    }\
    while (n>=0) {     \
      PUT_PIXEL(0);    \
      pz+=1;     \
      pp=(PIXEL *)((char *)pp + PSZB);\
      n-=1;     \
    }     \
  }

  switch (zb->cn) {
  case 3: {
    enum {PSZB = 3, PSZSH = 5};
    typedef uchar PIXEL;
    PIXEL* texture;
#define INTERP_Z
#define INTERP_STZ
#define DRAW_INIT DRAW_INIT1
#define DRAW_LINE DRAW_LINE1
#define PUT_PIXEL(_a)  \
{   \
  unsigned char *ptr;\
  zz=z >> ZB_POINT_Z_FRAC_BITS; \
  if (ZCMP(zz,pz[_a])) {  \
    ptr = texture + (((t & 0x3FC00000) | (s & 0x003FC000)) >> 14) * 3;\
    pp[3 * _a]= ptr[0];\
    pp[3 * _a + 1]= ptr[1];\
    pp[3 * _a + 2]= ptr[2];\
    pz[_a]=zz;  \
  }   \
  z+=dzdx;   \
  s+=dsdx;   \
  t+=dtdx;   \
}
#include "ztriangle_impl.1inl"
  }
  break;

  case 2: {
    enum {PSZB = 2, PSZSH = 4};
    typedef ushort PIXEL;
    PIXEL* texture;
#define INTERP_Z
#define INTERP_STZ
#define DRAW_INIT DRAW_INIT1
#define DRAW_LINE DRAW_LINE1
#define PUT_PIXEL(_a)  \
{   \
  zz=z >> ZB_POINT_Z_FRAC_BITS; \
  if (ZCMP(zz,pz[_a])) {  \
    pp[_a]=*(PIXEL *)((char *)texture+ \
        (((t & 0x3FC00000) | (s & 0x003FC000)) >> (17 - PSZSH)));\
    pz[_a]=zz;  \
  }   \
  z+=dzdx;   \
  s+=dsdx;   \
  t+=dtdx;   \
}
#include "ztriangle_impl.1inl"
  }
  break;
  }

#undef DRAW_LINE1
#undef DRAW_INIT1
}
#if 0
/* slow but exact version (only there for reference, incorrect for 24 bits) */
void ZB_fillTriangleMappingPerspective(ZBuffer* zb, ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
  PIXEL* texture;
#define INTERP_Z
#define INTERP_STZ
#define DRAW_INIT()  \
  {   \
    texture=zb->current_texture;  \
  }
#define PUT_PIXEL(_a)  \
  {   \
    GLfloat zinv; \
    int s,t; \
    zz=z >> ZB_POINT_Z_FRAC_BITS; \
    if (ZCMP(zz,pz[_a])) {  \
      zinv= 1.0 / (GLfloat) z; \
      s= (int) (sz * zinv); \
      t= (int) (tz * zinv); \
      pp[_a]=texture[((t & 0x3FC00000) | s) >> 14]; \
      pz[_a]=zz;  \
    }   \
    z+=dzdx;   \
    sz+=dszdx;   \
    tz+=dtzdx;   \
  }
#include "ztriangle_impl_cn.inl"
}
#endif
void gl_draw_triangle_fill(GLContext* c, GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
#ifdef PROFILE
  {
    int norm;
    assert(p0->zp.x >= 0 && p0->zp.x < c->w);
    assert(p0->zp.y >= 0 && p0->zp.y < c->h);
    assert(p1->zp.x >= 0 && p1->zp.x < c->w);
    assert(p1->zp.y >= 0 && p1->zp.y < c->h);
    assert(p2->zp.x >= 0 && p2->zp.x < c->w);
    assert(p2->zp.y >= 0 && p2->zp.y < c->h);
    norm = (p1->zp.x - p0->zp.x) * (p2->zp.y - p0->zp.y) -
    (p2->zp.x - p0->zp.x) * (p1->zp.y - p0->zp.y);
    count_pixels += abs(norm) / 2;
    count_triangles++;
  }
#endif

  if (c->texture_2d_enabled) {
#ifdef PROFILE
    count_triangles_textured++;
#endif
    ZB_setTexture(c->zb, (unsigned char*)c->current_texture->images[0].pixmap);
    ZB_fillTriangleMappingPerspective(c->zb, &p0->zp, &p1->zp, &p2->zp);
  }
  else if (c->current_shade_model == GL_SMOOTH) {
    ZB_fillTriangleSmooth(c->zb, &p0->zp, &p1->zp, &p2->zp);
  }
  else {
    ZB_fillTriangleFlat(c->zb, &p0->zp, &p1->zp, &p2->zp);
  }
}
/* Render a clipped triangle in line mode */
void gl_draw_triangle_line(GLContext* c, GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
  if (c->depth_test) {
    if (p0->edge_flag) {
      ZB_line_z(c->zb, &p0->zp, &p1->zp);
    }

    if (p1->edge_flag) {
      ZB_line_z(c->zb, &p1->zp, &p2->zp);
    }

    if (p2->edge_flag) {
      ZB_line_z(c->zb, &p2->zp, &p0->zp);
    }
  }
  else {
    if (p0->edge_flag) {
      ZB_line(c->zb, &p0->zp, &p1->zp);
    }

    if (p1->edge_flag) {
      ZB_line(c->zb, &p1->zp, &p2->zp);
    }

    if (p2->edge_flag) {
      ZB_line(c->zb, &p2->zp, &p0->zp);
    }
  }
}
/* Render a clipped triangle in point mode */
void gl_draw_triangle_point(GLContext* c,
    GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
  if (p0->edge_flag) {
    ZB_point(c->zb, &p0->zp, c->point_size);
  }

  if (p1->edge_flag) {
    ZB_point(c->zb, &p1->zp, c->point_size);
  }

  if (p2->edge_flag) {
    ZB_point(c->zb, &p2->zp, c->point_size);
  }
}
void glopBegin(GLContext* c, GLParam* p)
{
  int type;
  M4 tmp;
  assert(c->in_begin == 0);
  type = p[1].i;
  c->begin_type = type;
  c->in_begin = 1;
  c->vertex_n = 0;
  c->vertex_cnt = 0;

  if (c->matrix_model_projection_updated) {
    if (c->lighting_enabled) {
      /* precompute inverse modelview */
      gl_M4_Inv(&tmp, c->matrix_stack_ptr[0]);
      gl_M4_Transpose(&c->matrix_model_view_inv, &tmp);
    }
    else {
      GLfloat* m = &c->matrix_model_projection.m[0][0];
      /* precompute projection matrix */
      gl_M4_Mul(&c->matrix_model_projection, c->matrix_stack_ptr[1], c->matrix_stack_ptr[0]);
      /* test to accelerate computation */
      c->matrix_model_projection_no_w_transform = 0;

      if (m[12] == 0.0 && m[13] == 0.0 && m[14] == 0.0) {
        c->matrix_model_projection_no_w_transform = 1;
      }
    }

    /* test if the texture matrix is not Identity */
    c->apply_texture_matrix = !gl_M4_IsId(c->matrix_stack_ptr[2]);
    c->matrix_model_projection_updated = 0;
  }

  /* viewport */
  if (c->viewport.updated) {
    gl_eval_viewport(c);
    c->viewport.updated = 0;
  }

  /* triangle drawing functions */
  if (c->render_mode == GL_SELECT) {
    c->draw_triangle_front = gl_draw_triangle_select;
    c->draw_triangle_back = gl_draw_triangle_select;
  }
  else {
    switch (c->polygon_mode_front) {
    case GL_POINT:
      c->draw_triangle_front = gl_draw_triangle_point;
      break;

    case GL_LINE:
      c->draw_triangle_front = gl_draw_triangle_line;
      break;

    default:
      c->draw_triangle_front = gl_draw_triangle_fill;
      break;
    }

    switch (c->polygon_mode_back) {
    case GL_POINT:
      c->draw_triangle_back = gl_draw_triangle_point;
      break;

    case GL_LINE:
      c->draw_triangle_back = gl_draw_triangle_line;
      break;

    default:
      c->draw_triangle_back = gl_draw_triangle_fill;
      break;
    }
  }
}
/* coords, tranformation , clip code and projection */
/* TODO : handle all cases */
static CC_INLINE void gl_vertex_transform(GLContext* c, GLVertex* v)
{
  GLfloat* m;
  const GLfloat* n;

  if (c->lighting_enabled) {
    /* eye coordinates needed for lighting */
    m = &c->matrix_stack_ptr[0]->m[0][0];
    v->ec[0] = (v->coord[0] * m[0] + v->coord[1] * m[1] + v->coord[2] * m[2] + m[3]);
    v->ec[1] = (v->coord[0] * m[4] + v->coord[1] * m[5] + v->coord[2] * m[6] + m[7]);
    v->ec[2] = (v->coord[0] * m[8] + v->coord[1] * m[9] + v->coord[2] * m[10] + m[11]);
    v->ec[3] = (v->coord[0] * m[12] + v->coord[1] * m[13] + v->coord[2] * m[14] + m[15]);
    /* projection coordinates */
    m = &c->matrix_stack_ptr[1]->m[0][0];
    v->pc[0] = (v->ec[0] * m[0] + v->ec[1] * m[1] + v->ec[2] * m[2] + v->ec[3] * m[3]);
    v->pc[1] = (v->ec[0] * m[4] + v->ec[1] * m[5] + v->ec[2] * m[6] + v->ec[3] * m[7]);
    v->pc[2] = (v->ec[0] * m[8] + v->ec[1] * m[9] + v->ec[2] * m[10] + v->ec[3] * m[11]);
    v->pc[3] = (v->ec[0] * m[12] + v->ec[1] * m[13] + v->ec[2] * m[14] + v->ec[3] * m[15]);
    m = &c->matrix_model_view_inv.m[0][0];
    n = c->current_normal;
    v->normal[0] = (n[0] * m[0] + n[1] * m[1] + n[2] * m[2]);
    v->normal[1] = (n[0] * m[4] + n[1] * m[5] + n[2] * m[6]);
    v->normal[2] = (n[0] * m[8] + n[1] * m[9] + n[2] * m[10]);

    if (c->normalize_enabled) {
      gl_V3_Norm(v->normal);
    }
  }
  else {
    /* no eye coordinates needed, no normal */
    /* NOTE: W = 1 is assumed */
    m = &c->matrix_model_projection.m[0][0];
    v->pc[0] = (v->coord[0] * m[0] + v->coord[1] * m[1] + v->coord[2] * m[2] + m[3]);
    v->pc[1] = (v->coord[0] * m[4] + v->coord[1] * m[5] + v->coord[2] * m[6] + m[7]);
    v->pc[2] = (v->coord[0] * m[8] + v->coord[1] * m[9] + v->coord[2] * m[10] + m[11]);

    if (c->matrix_model_projection_no_w_transform) {
      v->pc[3] = m[15];
    }
    else {
      v->pc[3] = (v->coord[0] * m[12] + v->coord[1] * m[13] + v->coord[2] * m[14] + m[15]);
    }
  }

  v->clip_code = gl_clipcode(v->pc[0], v->pc[1], v->pc[2], v->pc[3]);
}
static CC_INLINE GLfloat clampf(GLfloat a, GLfloat min, GLfloat max)
{
  if (a < min) {
    return min;
  }
  else if (a > max) {
    return max;
  }
  else {
    return a;
  }
}
static void calc_buf(GLSpecBuf* buf, const GLfloat shininess)
{
  int i;
  GLfloat val, inc;
  val = 0.0f;
  inc = 1.0f / SPECULAR_BUFFER_SIZE;

  for (i = 0; i <= SPECULAR_BUFFER_SIZE; i++) {
    buf->buf[i] = (float)pow(val, shininess);
    val += inc;
  }
}
GLSpecBuf* specbuf_get_buffer(GLContext* c, const int shininess_i, const GLfloat shininess)
{
  GLSpecBuf* found, *oldest;
  found = oldest = c->specbuf_first;

  while (found && found->shininess_i != shininess_i) {
    if (found->last_used < oldest->last_used) {
      oldest = found;
    }

    found = found->next;
  }

  if (found) { /* hey, found one! */
    found->last_used = c->specbuf_used_counter++;
    return found;
  }

  if (oldest == NULL || c->specbuf_num_buffers < MAX_SPECULAR_BUFFERS) {
    /* create new buffer */
    GLSpecBuf* buf = (GLSpecBuf*)gl_malloc(sizeof(GLSpecBuf));

    if (!buf) {
      gl_fatal_error("could not allocate specular buffer");
    }

    c->specbuf_num_buffers++;
    buf->next = c->specbuf_first;
    c->specbuf_first = buf;
    buf->last_used = c->specbuf_used_counter++;
    buf->shininess_i = shininess_i;
    calc_buf(buf, shininess);
    return buf;
  }

  /* overwrite the lru buffer */
  /*tgl_trace("overwriting spec buffer :(\n");*/
  oldest->shininess_i = shininess_i;
  oldest->last_used = c->specbuf_used_counter++;
  calc_buf(oldest, shininess);
  return oldest;
}
/* non optimized lightening model */
void gl_shade_vertex(GLContext* c, GLVertex* v)
{
  GLfloat R, G, B, A;
  GLMaterial* m;
  GLLight* l;
  V3 n, s, d;
  GLfloat dist, tmp, att, dot, dot_spot, dot_spec;
  int twoside = c->light_model_two_side;
  m = &c->materials[0];
  n[0] = v->normal[0];
  n[1] = v->normal[1];
  n[2] = v->normal[2];
  R = m->emission[0] + m->ambient[0] * c->ambient_light_model[0];
  G = m->emission[1] + m->ambient[1] * c->ambient_light_model[1];
  B = m->emission[2] + m->ambient[2] * c->ambient_light_model[2];
  A = clampf(m->diffuse[3], 0, 1);

  for (l = c->first_light; l != NULL; l = l->next) {
    GLfloat lR, lB, lG;
    /* ambient */
    lR = l->ambient[0] * m->ambient[0];
    lG = l->ambient[1] * m->ambient[1];
    lB = l->ambient[2] * m->ambient[2];

    if (l->position[3] == 0) {
      /* light at infinity */
      d[0] = l->position[0];
      d[1] = l->position[1];
      d[2] = l->position[2];
      att = 1;
    }
    else {
      /* distance attenuation */
      d[0] = l->position[0] - v->ec[0];
      d[1] = l->position[1] - v->ec[1];
      d[2] = l->position[2] - v->ec[2];
      dist = (float)sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);

      if (dist > 1E-3) {
        tmp = 1 / dist;
        d[0] *= tmp;
        d[1] *= tmp;
        d[2] *= tmp;
      }

      att = 1.0f / (l->attenuation[0] + dist * (l->attenuation[1] + dist * l->attenuation[2]));
    }

    dot = d[0] * n[0] + d[1] * n[1] + d[2] * n[2];

    if (twoside && dot < 0) {
      dot = -dot;
    }

    if (dot > 0) {
      /* diffuse light */
      lR += dot * l->diffuse[0] * m->diffuse[0];
      lG += dot * l->diffuse[1] * m->diffuse[1];
      lB += dot * l->diffuse[2] * m->diffuse[2];

      /* spot light */
      if (l->spot_cutoff != 180) {
        dot_spot = -(d[0] * l->norm_spot_direction[0] + d[1] * l->norm_spot_direction[1] + d[2] * l->norm_spot_direction[2]);

        if (twoside && dot_spot < 0) {
          dot_spot = -dot_spot;
        }

        if (dot_spot < l->cos_spot_cutoff) {
          /* no contribution */
          continue;
        }
        else {
          /* TODO: optimize */
          if (l->spot_exponent > 0) {
            att = (float)(att * pow(dot_spot, l->spot_exponent));
          }
        }
      }

      /* specular light */
      if (c->local_light_model) {
        V3 vcoord;
        vcoord[0] = v->ec[0];
        vcoord[1] = v->ec[1];
        vcoord[2] = v->ec[2];
        gl_V3_Norm(vcoord);
        s[0] = d[0] - vcoord[0];
        s[1] = d[1] - vcoord[0];
        s[2] = d[2] - vcoord[0];
      }
      else {
        s[0] = d[0];
        s[1] = d[1];
        s[2] = (float)(d[2] + 1.0);
      }

      dot_spec = n[0] * s[0] + n[1] * s[1] + n[2] * s[2];

      if (twoside && dot_spec < 0) {
        dot_spec = -dot_spec;
      }

      if (dot_spec > 0) {
        GLSpecBuf* specbuf;
        int idx;
        tmp = (float)sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);

        if (tmp > 1E-3) {
          dot_spec = dot_spec / tmp;
        }

        /* TODO: optimize */
        /* testing specular buffer code */
        /* dot_spec= pow(dot_spec,m->shininess);*/
        specbuf = specbuf_get_buffer(c, m->shininess_i, m->shininess);
        idx = (int)(dot_spec * SPECULAR_BUFFER_SIZE);

        if (idx > SPECULAR_BUFFER_SIZE) {
          idx = SPECULAR_BUFFER_SIZE;
        }

        dot_spec = specbuf->buf[idx];
        lR += dot_spec * l->specular[0] * m->specular[0];
        lG += dot_spec * l->specular[1] * m->specular[1];
        lB += dot_spec * l->specular[2] * m->specular[2];
      }
    }

    R += att * lR;
    G += att * lG;
    B += att * lB;
  }

  v->color[0] = clampf(R, 0, 1);
  v->color[1] = clampf(G, 0, 1);
  v->color[2] = clampf(B, 0, 1);
  v->color[3] = A;
}
void glopVertex(GLContext* c, GLParam* p)
{
  GLVertex* v;
  int n, i, cnt;
  assert(c->in_begin != 0);
  n = c->vertex_n;
  cnt = c->vertex_cnt;
  cnt++;
  c->vertex_cnt = cnt;

  /* quick fix to avoid crashes on large polygons */
  if (n >= c->vertex_max) {
    GLVertex* newarray;
    c->vertex_max <<= 1; /* just double size */
    newarray = (GLVertex*)gl_malloc(sizeof(GLVertex) * c->vertex_max);

    if (!newarray) {
      gl_fatal_error("unable to allocate GLVertex array.\n");
    }

    memcpy(newarray, c->vertex, n * sizeof(GLVertex));
    gl_free(c->vertex);
    c->vertex = newarray;
  }

  /* new vertex entry */
  v = &c->vertex[n];
  n++;
  v->coord[0] = p[1].f;
  v->coord[1] = p[2].f;
  v->coord[2] = p[3].f;
  v->coord[3] = p[4].f;
  gl_vertex_transform(c, v);

  /* color */
  if (c->lighting_enabled) {
    gl_shade_vertex(c, v);
  }
  else {
    V4SETV(v->color, c->current_color);
  }

  /* tex coords */
  if (c->texture_2d_enabled) {
    if (c->apply_texture_matrix) {
      gl_M4_MulV4(v->tex_coord, c->matrix_stack_ptr[2], c->current_tex_coord);
    }
    else {
      V4SETV(v->tex_coord, c->current_tex_coord);
    }
  }

  /* precompute the mapping to the viewport */
  if (v->clip_code == 0) {
    gl_transform_to_viewport(c, v);
  }

  /* edge flag */
  v->edge_flag = c->current_edge_flag;

  switch (c->begin_type) {
  case GL_POINTS:
    gl_draw_point(c, &c->vertex[0]);
    n = 0;
    break;

  case GL_LINES:
    if (n == 2) {
      gl_draw_line(c, &c->vertex[0], &c->vertex[1]);
      n = 0;
    }

    break;

  case GL_LINE_STRIP:
  case GL_LINE_LOOP:
    if (n == 1) {
      c->vertex[2] = c->vertex[0];
    }
    else if (n == 2) {
      gl_draw_line(c, &c->vertex[0], &c->vertex[1]);
      c->vertex[0] = c->vertex[1];
      n = 1;
    }

    break;

  case GL_TRIANGLES:
    if (n == 3) {
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
      n = 0;
    }

    break;

  case GL_TRIANGLE_STRIP:
    if (cnt >= 3) {
      if (n == 3) {
        n = 0;
      }

      /* needed to respect triangle orientation */
      switch (cnt & 1) {
      case 0:
        gl_draw_triangle(c, &c->vertex[2], &c->vertex[1], &c->vertex[0]);
        break;

      default:
      case 1:
        gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
        break;
      }
    }

    break;

  case GL_TRIANGLE_FAN:
    if (n == 3) {
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
      c->vertex[1] = c->vertex[2];
      n = 2;
    }

    break;

  case GL_QUADS:
    if (n == 4) {
      c->vertex[2].edge_flag = 0;
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
      c->vertex[2].edge_flag = 1;
      c->vertex[0].edge_flag = 0;
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[2], &c->vertex[3]);
      n = 0;
    }

    break;

  case GL_QUAD_STRIP:
    if (n == 4) {
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
      gl_draw_triangle(c, &c->vertex[1], &c->vertex[3], &c->vertex[2]);

      for (i = 0; i < 2; i++) {
        c->vertex[i] = c->vertex[i + 2];
      }

      n = 2;
    }

    break;

  case GL_POLYGON:
    break;

  default:
    gl_fatal_error("glBegin: type %x not handled\n", c->begin_type);
  }

  c->vertex_n = n;
}
void glopEnd(GLContext* c, GLParam* p)
{
  assert(c->in_begin == 1);

  if (c->begin_type == GL_LINE_LOOP) {
    if (c->vertex_cnt >= 3) {
      gl_draw_line(c, &c->vertex[0], &c->vertex[2]);
    }
  }
  else if (c->begin_type == GL_POLYGON) {
    int i = c->vertex_cnt;

    while (i >= 3) {
      i--;
      gl_draw_triangle(c, &c->vertex[i], &c->vertex[0], &c->vertex[i - 1]);
    }
  }

  c->in_begin = 0;
}
void gl_enable_disable_light(GLContext* c, int light, int v)
{
  GLLight* l = &c->lights[light];

  if (v && !l->enabled) {
    l->enabled = 1;
    l->next = c->first_light;
    c->first_light = l;
    l->prev = NULL;
  }
  else if (!v && l->enabled) {
    l->enabled = 0;

    if (l->prev == NULL) {
      c->first_light = l->next;
    }
    else {
      l->prev->next = l->next;
    }

    if (l->next != NULL) {
      l->next->prev = l->prev;
    }
  }
}
void glopEnableDisable(GLContext* c, GLParam* p)
{
  int code = p[1].i;
  int v = p[2].i;

  switch (code) {
  case GL_CULL_FACE:
    c->cull_face_enabled = v;
    break;

  case GL_LIGHTING:
    c->lighting_enabled = v;
    break;

  case GL_COLOR_MATERIAL:
    c->color_material_enabled = v;
    break;

  case GL_TEXTURE_2D:
    c->texture_2d_enabled = v;
    break;

  case GL_NORMALIZE:
    c->normalize_enabled = v;
    break;

  case GL_DEPTH_TEST:
    c->depth_test = v;
    break;

  case GL_POLYGON_OFFSET_FILL:
    if (v) {
      c->offset_states |= TGL_OFFSET_FILL;
    }
    else {
      c->offset_states &= ~TGL_OFFSET_FILL;
    }

    break;

  case GL_POLYGON_OFFSET_POINT:
    if (v) {
      c->offset_states |= TGL_OFFSET_POINT;
    }
    else {
      c->offset_states &= ~TGL_OFFSET_POINT;
    }

    break;

  case GL_POLYGON_OFFSET_LINE:
    if (v) {
      c->offset_states |= TGL_OFFSET_LINE;
    }
    else {
      c->offset_states &= ~TGL_OFFSET_LINE;
    }

    break;

  default:
    if (code >= GL_LIGHT0 && code < GL_LIGHT0 + MAX_LIGHTS) {
      gl_enable_disable_light(c, code - GL_LIGHT0, v);
    }
    else {
      /*
      fprintf(stderr,"glEnableDisable: 0x%X not supported.\n",code);
      */
    }

    break;
  }
}
void gl_print_matrix(const GLfloat* m)
{
  int i;

  for (i = 0; i < 4; i++) {
    fprintf(stderr, "%f %f %f %f\n", m[i], m[4 + i], m[8 + i], m[12 + i]);
  }
}
static CC_INLINE void gl_matrix_update(GLContext* c)
{
  c->matrix_model_projection_updated = (c->matrix_mode <= 1);
}
void glopMatrixMode(GLContext* c, GLParam* p)
{
  int mode = p[1].i;

  switch (mode) {
  case GL_MODELVIEW:
    c->matrix_mode = 0;
    break;

  case GL_PROJECTION:
    c->matrix_mode = 1;
    break;

  case GL_TEXTURE:
    c->matrix_mode = 2;
    break;

  default:
    ASSERT(0);
  }
}
void glopLoadMatrix(GLContext* c, GLParam* p)
{
  M4* m;
  int i;
  GLParam* q;
  m = c->matrix_stack_ptr[c->matrix_mode];
  q = p + 1;

  for (i = 0; i < 4; i++) {
    m->m[0][i] = q[0].f;
    m->m[1][i] = q[1].f;
    m->m[2][i] = q[2].f;
    m->m[3][i] = q[3].f;
    q += 4;
  }

  gl_matrix_update(c);
}
void glopLoadIdentity(GLContext* c, GLParam* p)
{
  gl_M4_Id(c->matrix_stack_ptr[c->matrix_mode]);
  gl_matrix_update(c);
}
void glopMultMatrix(GLContext* c, GLParam* p)
{
  M4 m;
  int i;
  GLParam* q;
  q = p + 1;

  for (i = 0; i < 4; i++) {
    m.m[0][i] = q[0].f;
    m.m[1][i] = q[1].f;
    m.m[2][i] = q[2].f;
    m.m[3][i] = q[3].f;
    q += 4;
  }

  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
}
void glopPushMatrix(GLContext* c, GLParam* p)
{
  int n = c->matrix_mode;
  M4* m;
  ASSERT((c->matrix_stack_ptr[n] - c->matrix_stack[n] + 1) < c->matrix_stack_depth_max[n]);
  m = ++c->matrix_stack_ptr[n];
  gl_M4_Move(&m[0], &m[-1]);
  gl_matrix_update(c);
}
void glopPopMatrix(GLContext* c, GLParam* p)
{
  int n = c->matrix_mode;
  ASSERT(c->matrix_stack_ptr[n] > c->matrix_stack[n]);
  c->matrix_stack_ptr[n]--;
  gl_matrix_update(c);
}
void glopRotate(GLContext* c, GLParam* p)
{
  M4 m;
  GLfloat u[3];
  GLfloat angle;
  int dir_code;
  angle = (float)(p[1].f * M_PI / 180.0);
  u[0] = p[2].f;
  u[1] = p[3].f;
  u[2] = p[4].f;
  /* simple case detection */
  dir_code = ((u[0] != 0) << 2) | ((u[1] != 0) << 1) | (u[2] != 0);

  switch (dir_code) {
  case 0:
    gl_M4_Id(&m);
    break;

  case 4:
    if (u[0] < 0) {
      angle = -angle;
    }

    gl_M4_Rotate(&m, angle, 0);
    break;

  case 2:
    if (u[1] < 0) {
      angle = -angle;
    }

    gl_M4_Rotate(&m, angle, 1);
    break;

  case 1:
    if (u[2] < 0) {
      angle = -angle;
    }

    gl_M4_Rotate(&m, angle, 2);
    break;

  default: {
    GLfloat cost, sint;
    /* normalize vector */
    GLfloat len = u[0] * u[0] + u[1] * u[1] + u[2] * u[2];

    if (len == 0.0f) {
      return;
    }

    len = (float)(1.0f / sqrt(len));
    u[0] *= len;
    u[1] *= len;
    u[2] *= len;
    /* store cos and sin values */
    cost = (float)cos(angle);
    sint = (float)sin(angle);
    /* fill in the values */
    m.m[3][0] = m.m[3][1] = m.m[3][2] =
        m.m[0][3] = m.m[1][3] = m.m[2][3] = 0.0f;
    m.m[3][3] = 1.0f;
    /* do the math */
    m.m[0][0] = u[0] * u[0] + cost * (1 - u[0] * u[0]);
    m.m[1][0] = u[0] * u[1] * (1 - cost) - u[2] * sint;
    m.m[2][0] = u[2] * u[0] * (1 - cost) + u[1] * sint;
    m.m[0][1] = u[0] * u[1] * (1 - cost) + u[2] * sint;
    m.m[1][1] = u[1] * u[1] + cost * (1 - u[1] * u[1]);
    m.m[2][1] = u[1] * u[2] * (1 - cost) - u[0] * sint;
    m.m[0][2] = u[2] * u[0] * (1 - cost) - u[1] * sint;
    m.m[1][2] = u[1] * u[2] * (1 - cost) + u[0] * sint;
    m.m[2][2] = u[2] * u[2] + cost * (1 - u[2] * u[2]);
  }
  }

  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
}
void glopTranslate(GLContext* c, GLParam* p)
{
  GLfloat* m;
  GLfloat x = p[1].f, y = p[2].f, z = p[3].f;
  m = &c->matrix_stack_ptr[c->matrix_mode]->m[0][0];
  m[3] = m[0] * x + m[1] * y + m[2] * z + m[3];
  m[7] = m[4] * x + m[5] * y + m[6] * z + m[7];
  m[11] = m[8] * x + m[9] * y + m[10] * z + m[11];
  m[15] = m[12] * x + m[13] * y + m[14] * z + m[15];
  gl_matrix_update(c);
}
void glopScale(GLContext* c, GLParam* p)
{
  GLfloat* m;
  GLfloat x = p[1].f, y = p[2].f, z = p[3].f;
  m = &c->matrix_stack_ptr[c->matrix_mode]->m[0][0];
  m[0] *= x;
  m[1] *= y;
  m[2] *= z;
  m[4] *= x;
  m[5] *= y;
  m[6] *= z;
  m[8] *= x;
  m[9] *= y;
  m[10] *= z;
  m[12] *= x;
  m[13] *= y;
  m[14] *= z;
  gl_matrix_update(c);
}
void glopViewport(GLContext* c, GLParam* p)
{
  int w, h, xmin, ymin, xsize_req, ysize_req;
  xmin = p[1].i;
  ymin = p[2].i;
  w = p[3].i;
  h = p[4].i;

  /* we may need to resize the zbuffer */
  if (c->viewport.xmin != xmin ||
      c->viewport.ymin != ymin ||
      c->viewport.w != w ||
      c->viewport.h != h) {
    xsize_req = xmin + w;
    ysize_req = ymin + h;
#if 0

    if (c->gl_resize_viewport && c->gl_resize_viewport(c, &xsize_req, &ysize_req) != 0) {
      gl_fatal_error("glViewport: error while resizing display");
    }

#endif
    w = xsize_req - xmin;
    h = ysize_req - ymin;

    if (w <= 0 || h <= 0) {
      gl_fatal_error("glViewport: size too small");
    }

    tgl_trace("glViewport: %d %d %d %d\n", xmin, ymin, w, h);
    c->viewport.xmin = xmin;
    c->viewport.ymin = ymin;
    c->viewport.w = w;
    c->viewport.h = h;
    c->viewport.updated = 1;
  }
}
void setFrustum(float* m, float l, float r, float b, float t, float n, float f)
{
  m[0] = 2 * n / (r - l);
  m[2] = (r + l) / (r - l);
  m[5] = 2 * n / (t - b);
  m[6] = (t + b) / (t - b);
  m[10] = -(f + n) / (f - n);
  m[11] = -(2 * f * n) / (f - n);
  m[14] = -1;
}
void glopFrustum(GLContext* c, GLParam* p)
{
#if 1
  M4 m = {0};
  setFrustum(&m.m[0][0], p[1].f, p[2].f, p[3].f, p[4].f, p[5].f, p[6].f);
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
#else
  GLfloat* r;
  M4 m;
  GLfloat left = p[1].f;
  GLfloat right = p[2].f;
  GLfloat bottom = p[3].f;
  GLfloat top = p[4].f;
  GLfloat nearv = p[5].f;
  GLfloat farp = p[6].f;
  GLfloat x, y, A, B, C, D;
  x = (float)((2.0 * nearv) / (right - left));
  y = (float)((2.0 * nearv) / (top - bottom));
  A = (right + left) / (right - left);
  B = (top + bottom) / (top - bottom);
  C = -(farp + nearv) / (farp - nearv);
  D = -(2.0f * farp * nearv) / (farp - nearv);
  r = &m.m[0][0];
  r[0] = x;
  r[1] = 0;
  r[2] = A;
  r[3] = 0;
  r[4] = 0;
  r[5] = y;
  r[6] = B;
  r[7] = 0;
  r[8] = 0;
  r[9] = 0;
  r[10] = C;
  r[11] = D;
  r[12] = 0;
  r[13] = 0;
  r[14] = -1;
  r[15] = 0;
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
#endif
}
void setOrtho(float* m, float l, float r, float b, float t, float n, float f)
{
  m[0] = 2 / (r - l);
  m[3] = -(r + l) / (r - l);
  m[5] = 2 / (t - b);
  m[7] = -(t + b) / (t - b);
  m[10] = -2 / (f - n);
  m[11] = -(f + n) / (f - n);
  m[15] = 1;
}
void glopOrtho(GLContext* c, GLParam* p)
{
#if 1
  M4 m = {0};
  setOrtho(&m.m[0][0], p[1].f, p[2].f, p[3].f, p[4].f, p[5].f, p[6].f);
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
#else
  GLfloat* r;
  M4 m = {0};
  GLfloat left = p[1].f;
  GLfloat right = p[2].f;
  GLfloat bottom = p[3].f;
  GLfloat top = p[4].f;
  GLfloat nearv = p[5].f;
  GLfloat farp = p[6].f;
  GLfloat x, y, z;
  x = (float)(2.0 / (right - left));
  y = (float)(2.0 / (top - bottom));
  z = (float)(2.0 / (farp - nearv));
  r = &m.m[0][0];
  r[0] = x;
  r[1] = 0;
  r[2] = 0;
  r[3] = -(right + left) * 0.5 * x;
  r[4] = 0;
  r[5] = y;
  r[6] = 0;
  r[7] = -(top + bottom) * 0.5 * y;
  r[8] = 0;
  r[9] = 0;
  r[10] = z;
  r[11] = -(farp + nearv) * 0.5 * z;
  r[12] = 0;
  r[13] = 0;
  r[14] = 0;
  r[15] = 1;
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
#endif
}
void glopMaterial(GLContext* c, GLParam* p)
{
  int mode = p[1].i;
  int type = p[2].i;
  GLfloat* v = &p[3].f;
  int i;
  GLMaterial* m;

  if (mode == GL_FRONT_AND_BACK) {
    p[1].i = GL_FRONT;
    glopMaterial(c, p);
    mode = GL_BACK;
  }

  if (mode == GL_FRONT) {
    m = &c->materials[0];
  }
  else {
    m = &c->materials[1];
  }

  switch (type) {
  case GL_EMISSION:
    for (i = 0; i < 4; i++) {
      m->emission[i] = v[i];
    }

    break;

  case GL_AMBIENT:
    for (i = 0; i < 4; i++) {
      m->ambient[i] = v[i];
    }

    break;

  case GL_DIFFUSE:
    for (i = 0; i < 4; i++) {
      m->diffuse[i] = v[i];
    }

    break;

  case GL_SPECULAR:
    for (i = 0; i < 4; i++) {
      m->specular[i] = v[i];
    }

    break;

  case GL_SHININESS:
    m->shininess = v[0];
    m->shininess_i = (int)((v[0] / 128.0f) * SPECULAR_BUFFER_RESOLUTION);
    break;

  case GL_AMBIENT_AND_DIFFUSE:
    for (i = 0; i < 4; i++) {
      m->diffuse[i] = v[i];
    }

    for (i = 0; i < 4; i++) {
      m->ambient[i] = v[i];
    }

    break;

  default:
    ASSERT(0);
  }
}
void glopColor(GLContext* c, GLParam* p)
{
  c->current_color[0] = p[1].f;
  c->current_color[1] = p[2].f;
  c->current_color[2] = p[3].f;
  c->current_color[3] = p[4].f;
  c->longcurrent_color[0] = p[5].ui;
  c->longcurrent_color[1] = p[6].ui;
  c->longcurrent_color[2] = p[7].ui;

  if (c->color_material_enabled) {
    GLParam q[7];
    q[0].op = OP_Material;
    q[1].i = c->current_color_material_mode;
    q[2].i = c->current_color_material_type;
    q[3].f = p[1].f;
    q[4].f = p[2].f;
    q[5].f = p[3].f;
    q[6].f = p[4].f;
    glopMaterial(c, q);
  }
}
void glopColorMaterial(GLContext* c, GLParam* p)
{
  int mode = p[1].i;
  int type = p[2].i;
  c->current_color_material_mode = mode;
  c->current_color_material_type = type;
}
void glopLight(GLContext* c, GLParam* p)
{
  int light = p[1].i;
  int type = p[2].i;
  V4 v;
  GLLight* l;
  int i;
  ASSERT(light >= GL_LIGHT0 && light < GL_LIGHT0 + MAX_LIGHTS);
  l = &c->lights[light - GL_LIGHT0];

  for (i = 0; i < 4; i++) {
    v[i] = p[3 + i].f;
  }

  switch (type) {
  case GL_AMBIENT:
    V4SETV(l->ambient, v);
    break;

  case GL_DIFFUSE:
    V4SETV(l->diffuse, v);
    break;

  case GL_SPECULAR:
    V4SETV(l->specular, v);
    break;

  case GL_POSITION: {
    V4 pos;
    gl_M4_MulV4(pos, c->matrix_stack_ptr[0], v);
    V4SETV(l->position, pos);

    if (l->position[3] == 0) {
      l->norm_position[0] = pos[0];
      l->norm_position[1] = pos[1];
      l->norm_position[2] = pos[2];
      gl_V3_Norm(l->norm_position);
    }
  }
  break;

  case GL_SPOT_DIRECTION:
    for (i = 0; i < 3; i++) {
      l->spot_direction[i] = v[i];
      l->norm_spot_direction[i] = v[i];
    }

    gl_V3_Norm(l->norm_spot_direction);
    break;

  case GL_SPOT_EXPONENT:
    l->spot_exponent = v[0];
    break;

  case GL_SPOT_CUTOFF: {
    GLfloat a = v[0];
    ASSERT(a == 180 || (a >= 0 && a <= 90));
    l->spot_cutoff = a;

    if (a != 180) {
      l->cos_spot_cutoff = (float)cos(a * M_PI / 180.0);
    }
  }
  break;

  case GL_CONSTANT_ATTENUATION:
    l->attenuation[0] = v[0];
    break;

  case GL_LINEAR_ATTENUATION:
    l->attenuation[1] = v[0];
    break;

  case GL_QUADRATIC_ATTENUATION:
    l->attenuation[2] = v[0];
    break;

  default:
    ASSERT(0);
  }
}
void glopLightModel(GLContext* c, GLParam* p)
{
  int pname = p[1].i;
  GLfloat* v = &p[2].f;
  int i;

  switch (pname) {
  case GL_LIGHT_MODEL_AMBIENT:
    for (i = 0; i < 4; i++) {
      c->ambient_light_model[i] = v[i];
    }

    break;

  case GL_LIGHT_MODEL_LOCAL_VIEWER:
    c->local_light_model = (int)v[0];
    break;

  case GL_LIGHT_MODEL_TWO_SIDE:
    c->light_model_two_side = (int)v[0];
    break;

  default:
    tgl_warning("glopLightModel: illegal pname: 0x%x\n", pname);
    //ASSERT(0);
    break;
  }
}
void glopClear(GLContext* c, GLParam* p)
{
  int mask = p[1].i;
  int z = 0;
  int r = (int)(c->clear_color[0] * 65535);
  int g = (int)(c->clear_color[1] * 65535);
  int b = (int)(c->clear_color[2] * 65535);
  int clear_z = mask & GL_DEPTH_BUFFER_BIT;
  int clear_color = mask & GL_COLOR_BUFFER_BIT;
  int color, y;
  ushort color_s;
  ZBuffer* zb = c->zb;

  if (clear_z) {
    memset_s(zb->zbuf, z, zb->w * zb->h);
  }

  if (clear_color) {
    switch (zb->bpp) {
    case 15:
      RGB_TO_PIXEL15(&color_s, r, g, b);

      for (y = 0; y < zb->h; y++) {
        memset_s(zb->pbuf + y * zb->step, color_s, zb->w);
      }

      break;

    case 16:
      RGB_TO_PIXEL16(&color_s, r, g, b);

      for (y = 0; y < zb->h; y++) {
        memset_s(zb->pbuf + y * zb->step, color_s, zb->w);
      }

      break;

    case 24:
      for (y = 0; y < zb->h; y++) {
        memset_RGB24(zb->pbuf + y * zb->step, r >> 8, g >> 8, b >> 8, zb->w);
      }

      break;

    case 32:
      RGB_TO_PIXEL32(&color, r, g, b);

      for (y = 0; y < zb->h; y++) {
        memset_l(zb->pbuf + y * zb->step, color, zb->w);
      }

      break;

    default:
      ASSERT(0);
      break;
    }
  }
}
void glopClearColor(GLContext* c, GLParam* p)
{
  c->clear_color[0] = p[1].f;
  c->clear_color[1] = p[2].f;
  c->clear_color[2] = p[3].f;
  c->clear_color[3] = p[4].f;
}
void glopClearDepth(GLContext* c, GLParam* p)
{
  c->clear_depth = p[1].f;
}
void glopShadeModel(GLContext* c, GLParam* p)
{
  int code = p[1].i;
  c->current_shade_model = code;
}
void glopCullFace(GLContext* c, GLParam* p)
{
  int code = p[1].i;
  c->current_cull_face = code;
}
void glopFrontFace(GLContext* c, GLParam* p)
{
  int code = p[1].i;
  c->current_front_face = code;
}
void glopPolygonMode(GLContext* c, GLParam* p)
{
  int face = p[1].i;
  int mode = p[2].i;

  switch (face) {
  case GL_BACK:
    c->polygon_mode_back = mode;
    break;

  case GL_FRONT:
    c->polygon_mode_front = mode;
    break;

  case GL_FRONT_AND_BACK:
    c->polygon_mode_front = mode;
    c->polygon_mode_back = mode;
    break;

  default:
    assert(0);
  }
}
void glopHint(GLContext* c, GLParam* p)
{
#if 0
  int target = p[1].i;
  int mode = p[2].i;
  /* do nothing */
#endif
}
void glopArrayElement(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopEnableClientState(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopDisableClientState(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopVertexPointer(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopColorPointer(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopNormalPointer(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopTexCoordPointer(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopPolygonOffset(GLContext* c, GLParam* p)
{
  c->offset_factor = p[1].f;
  c->offset_units = p[2].f;
}
static GLTexture* find_texture(GLContext* c, int h)
{
  GLTexture* t;
  t = c->shared_state.texture_hash_table[h % TEXTURE_HASH_TABLE_SIZE];

  while (t != NULL) {
    if (t->handle == h) {
      return t;
    }

    t = t->next;
  }

  return NULL;
}
static void free_texture(GLContext* c, int h)
{
  GLTexture* t, **ht;
  GLImage* im;
  int i;
  t = find_texture(c, h);

  if (t->prev == NULL) {
    ht = &c->shared_state.texture_hash_table
        [t->handle % TEXTURE_HASH_TABLE_SIZE];
    *ht = t->next;
  }
  else {
    t->prev->next = t->next;
  }

  if (t->next != NULL) {
    t->next->prev = t->prev;
  }

  for (i = 0; i < MAX_TEXTURE_LEVELS; i++) {
    im = &t->images[i];

    if (im->pixmap != NULL) {
      gl_free(im->pixmap);
    }
  }

  gl_free(t);
}
GLTexture* alloc_texture(GLContext* c, int h)
{
  GLTexture* t, **ht;
  t = (GLTexture*)gl_malloc(sizeof(GLTexture));
  ht = &c->shared_state.texture_hash_table[h % TEXTURE_HASH_TABLE_SIZE];
  t->next = *ht;
  t->prev = NULL;

  if (t->next != NULL) {
    t->next->prev = t;
  }

  *ht = t;
  t->handle = h;
  return t;
}
void initSharedState(GLContext* c)
{
  GLSharedState* s = &c->shared_state;
  s->lists = (GLList**)gl_malloc(sizeof(GLList*) * MAX_DISPLAY_LISTS);
  s->texture_hash_table = (GLTexture**)gl_malloc(sizeof(GLTexture*) * TEXTURE_HASH_TABLE_SIZE);
  alloc_texture(c, 0);
}
void endSharedState(GLContext* c)
{
  GLSharedState* s = &c->shared_state;
  int i;

  for (i = 0; i < MAX_DISPLAY_LISTS; i++) {
    /* TODO */
  }

  gl_free(s->lists);
  gl_free(s->texture_hash_table);
}
void glInitTextures(GLContext* c)
{
  /* textures */
  c->texture_2d_enabled = 0;
  c->current_texture = find_texture(c, 0);
}
void glopGenTextures(GLContext* c, int n, unsigned int* textures)
{
  int max, i;
  GLTexture* t;
  max = 0;

  for (i = 0; i < TEXTURE_HASH_TABLE_SIZE; i++) {
    t = c->shared_state.texture_hash_table[i];

    while (t != NULL) {
      if (t->handle > max) {
        max = t->handle;
      }

      t = t->next;
    }
  }

  for (i = 0; i < n; i++) {
    textures[i] = max + i + 1;
  }
}
void glopBindTexture(GLContext* c, GLParam* p)
{
  int target = p[1].i;
  int texture = p[2].i;
  GLTexture* t;
  assert(target == GL_TEXTURE_2D && texture >= 0);
  t = find_texture(c, texture);

  if (t == NULL) {
    t = alloc_texture(c, texture);
  }

  c->current_texture = t;
}
/*
 * image conversion
 */
void gl_convertRGB_to_5R6G5B(unsigned short* pixmap, unsigned char* rgb,
    int w, int h)
{
  int i, n;
  unsigned char* p;
  p = rgb;
  n = w * h;

  for (i = 0; i < n; i++) {
    pixmap[i] = ((p[0] & 0xF8) << 8) | ((p[1] & 0xFC) << 3) | ((p[2] & 0xF8) >> 3);
    p += 3;
  }
}
void gl_convertRGB_to_8A8R8G8B(unsigned int* pixmap, unsigned char* rgb,
    int w, int h)
{
  int i, n;
  unsigned char* p;
  p = rgb;
  n = w * h;

  for (i = 0; i < n; i++) {
    pixmap[i] = (((unsigned int)p[0]) << 16) |
        (((unsigned int)p[1]) << 8) |
        (((unsigned int)p[2]));
    p += 3;
  }
}
/*
 * linear interpolation with xf,yf normalized to 2^16
 */
#define INTERP_NORM_BITS 16
#define INTERP_NORM  (1 << INTERP_NORM_BITS)
static CC_INLINE int interpolate1(int v00, int v01, int v10, int xf, int yf)
{
  return v00 + (((v01 - v00) * xf + (v10 - v00) * yf) >> INTERP_NORM_BITS);
}
/*
 * TODO: more accurate resampling
 */
void gl_resizeImage(unsigned char* dest, int xsize_dest, int ysize_dest,
    unsigned char* src, int xsize_src, int ysize_src)
{
  unsigned char* pix, *pix_src;
  GLfloat x1, y1, x1inc, y1inc;
  int xi, yi, j, xf, yf, x, y;
  pix = dest;
  pix_src = src;
  x1inc = (GLfloat)(xsize_src - 1) / (GLfloat)(xsize_dest - 1);
  y1inc = (GLfloat)(ysize_src - 1) / (GLfloat)(ysize_dest - 1);
  y1 = 0;

  for (y = 0; y < ysize_dest; y++) {
    x1 = 0;

    for (x = 0; x < xsize_dest; x++) {
      xi = (int) x1;
      yi = (int) y1;
      xf = (int)((x1 - floor(x1)) * INTERP_NORM);
      yf = (int)((y1 - floor(y1)) * INTERP_NORM);

      if ((xf + yf) <= INTERP_NORM) {
        for (j = 0; j < 3; j++) {
          pix[j] = interpolate1(pix_src[(yi * xsize_src + xi) * 3 + j],
              pix_src[(yi * xsize_src + xi + 1) * 3 + j],
              pix_src[((yi + 1) * xsize_src + xi) * 3 + j],
              xf, yf);
        }
      }
      else {
        xf = INTERP_NORM - xf;
        yf = INTERP_NORM - yf;

        for (j = 0; j < 3; j++) {
          pix[j] = interpolate1(pix_src[((yi + 1) * xsize_src + xi + 1) * 3 + j],
              pix_src[((yi + 1) * xsize_src + xi) * 3 + j],
              pix_src[(yi * xsize_src + xi + 1) * 3 + j],
              xf, yf);
        }
      }

      pix += 3;
      x1 += x1inc;
    }

    y1 += y1inc;
  }
}
#define FRAC_BITS 16
/* resizing with no interlating nor nearest pixel */
void gl_resizeImageNoInterpolate(unsigned char* dest, int xsize_dest, int ysize_dest,
    unsigned char* src, int xsize_src, int ysize_src)
{
  unsigned char* pix, *pix_src, *pix1;
  int x1, y1, x1inc, y1inc;
  int xi, yi, x, y;
  pix = dest;
  pix_src = src;
  x1inc = (int)((GLfloat)((xsize_src) << FRAC_BITS) / (GLfloat)(xsize_dest));
  y1inc = (int)((GLfloat)((ysize_src) << FRAC_BITS) / (GLfloat)(ysize_dest));
  y1 = 0;

  for (y = 0; y < ysize_dest; y++) {
    x1 = 0;

    for (x = 0; x < xsize_dest; x++) {
      xi = x1 >> FRAC_BITS;
      yi = y1 >> FRAC_BITS;
      pix1 = pix_src + (yi * xsize_src + xi) * 3;
      pix[0] = pix1[0];
      pix[1] = pix1[1];
      pix[2] = pix1[2];
      pix += 3;
      x1 += x1inc;
    }

    y1 += y1inc;
  }
}
void glopTexImage2D(GLContext* c, GLParam* p)
{
  int target = p[1].i;
  int level = p[2].i;
  int components = p[3].i;
  int width = p[4].i;
  int height = p[5].i;
  int border = p[6].i;
  int format = p[7].i;
  int type = p[8].i;
  unsigned char* pixels = (unsigned char*)p[9].p;
  GLImage* im;
  unsigned char* pixels1;
  int do_free;
  ZBuffer* zb = c->zb;

  if (!(target == GL_TEXTURE_2D && level == 0 && components == 3 &&
      border == 0 && format == GL_RGB &&
      type == GL_UNSIGNED_BYTE)) {
    gl_fatal_error("glTexImage2D: combinaison of parameters not handled");
  }

  do_free = 0;

  if (width != 256 || height != 256) {
    pixels1 = (unsigned char*)gl_malloc(256 * 256 * 3);
    /* no interpolation is done here to respect the original image aliasing ! */
    gl_resizeImageNoInterpolate(pixels1, 256, 256, pixels, width, height);
    do_free = 1;
    width = 256;
    height = 256;
  }
  else {
    pixels1 = pixels;
  }

  im = &c->current_texture->images[level];
  im->w = width;
  im->h = height;

  if (im->pixmap != NULL) {
    gl_free(im->pixmap);
  }

  switch (zb->bpp) {
  case 24:
    im->pixmap = gl_malloc(width * height * 3);

    if (im->pixmap) {
      memcpy(im->pixmap, pixels1, width * height * 3);
    }

    break;

  case 32:
    im->pixmap = gl_malloc(width * height * 4);

    if (im->pixmap) {
      gl_convertRGB_to_8A8R8G8B((uint*)im->pixmap, pixels1, width, height);
    }

    break;

  case 15:
  case 16:
    im->pixmap = gl_malloc(width * height * 2);

    if (im->pixmap) {
      gl_convertRGB_to_5R6G5B((unsigned short*)im->pixmap, pixels1, width, height);
    }

    break;
  }

  if (do_free) {
    gl_free(pixels1);
  }
}
/* TODO: not all tests are done */
void glopTexEnv(GLContext* c, GLParam* p)
{
  int target = p[1].i;
  int pname = p[2].i;
  int param = p[3].i;

  if (target != GL_TEXTURE_ENV) {
error:
    gl_fatal_error("glTexParameter: unsupported option");
  }

  if (pname != GL_TEXTURE_ENV_MODE) {
    goto error;
  }

  if (param != GL_DECAL) {
    goto error;
  }
}
/* TODO: not all tests are done */
void glopTexParameter(GLContext* c, GLParam* p)
{
  int target = p[1].i;
  int pname = p[2].i;
  int param = p[3].i;

  if (target != GL_TEXTURE_2D) {
error:
    gl_fatal_error("glTexParameter: unsupported option");
  }

  switch (pname) {
  case GL_TEXTURE_WRAP_S:
  case GL_TEXTURE_WRAP_T:
    if (param != GL_REPEAT) {
      goto error;
    }

    break;
  }
}
void glopPixelStore(GLContext* c, GLParam* p)
{
  int pname = p[1].i;
  int param = p[2].i;

  if (pname != GL_UNPACK_ALIGNMENT ||
      param != 1) {
    gl_fatal_error("glPixelStore: unsupported option");
  }
}
int ZB_Init(ZBuffer* zb, int h, int w, uchar* pbuf, int step, int bpp)
{
  if (h != zb->h || w != zb->w || zb->pbuf != pbuf || zb->step != step || zb->bpp != bpp) {
    zb->h = h;
    zb->w = w;
    zb->pbuf = pbuf;
    zb->step = step;
    zb->bpp = bpp;
    zb->cn = BPP2CN(bpp);
    zb->current_texture = NULL;
    zb->zbuf = (unsigned short*)realloc(zb->zbuf, h * w * sizeof(unsigned short));
  }

  return 0;
}
void glopPointSize(GLContext* c, GLParam* p)
{
  c->point_size = p[1].i;
}
static GLList* find_list(GLContext* c, unsigned int list)
{
  return c->shared_state.lists[list];
}
static void delete_list(GLContext* c, int list)
{
  GLParamBuffer* pb, *pb1;
  GLList* l;
  l = find_list(c, list);
  assert(l != NULL);
  /* free param buffer */
  pb = l->first_op_buffer;

  while (pb != NULL) {
    pb1 = pb->next;
    gl_free(pb);
    pb = pb1;
  }

  gl_free(l);
  c->shared_state.lists[list] = NULL;
}
static GLList* alloc_list(GLContext* c, int list)
{
  GLList* l;
  GLParamBuffer* ob;
  l = (GLList*)gl_malloc(sizeof(GLList));
  ob = (GLParamBuffer*)gl_malloc(sizeof(GLParamBuffer));
  ob->next = NULL;
  l->first_op_buffer = ob;
  ob->ops[0].op = OP_EndList;
  c->shared_state.lists[list] = l;
  return l;
}
/* this opcode is never called directly */
void glopEndList(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
/* this opcode is never called directly */
void glopNextBuffer(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopCallList(GLContext* c, GLParam* p);
static char* op_table_str[] = {
#define ADD_OP(a,b,c) "gl" #a " " #c,
#include "opinfo.txt"
};
static void (*op_table_func[])(GLContext*, GLParam*) = {
#define ADD_OP(a,b,c) glop ## a ,
#include "opinfo.txt"
};
static int op_table_size[] = {
#define ADD_OP(a,b,c) b + 1 ,
#include "opinfo.txt"
};
void glopCallList(GLContext* c, GLParam* p)
{
  GLList* l;
  int list, op;
  list = p[1].ui;
  l = find_list(c, list);

  if (l == NULL) {
    gl_fatal_error("list %d not defined", list);
  }

  p = l->first_op_buffer->ops;

  while (1) {
    op = p[0].op;

    if (op == OP_EndList) {
      break;
    }

    if (op == OP_NextBuffer) {
      p = (GLParam*)p[1].p;
    }
    else {
      op_table_func[op](c, p);
      p += op_table_size[op];
    }
  }
}
#if 0
void gl_compile_op(GLContext* c, GLParam* p)
{
  int op, op_size;
  GLParamBuffer* ob, *ob1;
  int index, i;
  op = p[0].op;
  op_size = op_table_size[op];
  index = c->current_op_buffer_index;
  ob = c->current_op_buffer;

  /* we should be able to add a NextBuffer opcode */
  if ((index + op_size) > (OP_BUFFER_MAX_SIZE - 2)) {
    ob1 = (GLParamBuffer*)gl_malloc(sizeof(GLParamBuffer));
    ob1->next = NULL;
    ob->next = ob1;
    ob->ops[index].op = OP_NextBuffer;
    ob->ops[index + 1].p = (void*)ob1;
    c->current_op_buffer = ob1;
    ob = ob1;
    index = 0;
  }

  for (i = 0; i < op_size; i++) {
    ob->ops[index] = p[i];
    index++;
  }

  c->current_op_buffer_index = index;
}
void glopNewList(GLContext* c, unsigned int list, int mode)
{
  GLList* l;
  assert(mode == GL_COMPILE || mode == GL_COMPILE_AND_EXECUTE);
  assert(c->compile_flag == 0);
  l = find_list(c, list);

  if (l != NULL) {
    delete_list(c, list);
  }

  l = alloc_list(c, list);
  c->current_op_buffer = l->first_op_buffer;
  c->current_op_buffer_index = 0;
  c->compile_flag = 1;
  c->exec_flag = (mode == GL_COMPILE_AND_EXECUTE);
}
void glEndList(GLContext* c)
{
  GLParam p[1];
  assert(c->compile_flag == 1);
  /* end of list */
  p[0].op = OP_EndList;
  gl_compile_op(c, p);
  c->compile_flag = 0;
  c->exec_flag = 1;
}
GLboolean glIsList(GLContext* c, GLuint list)
{
  GLList* l;
  l = find_list(c, list);
  return (l != NULL);
}
GLuint glGenLists(GLContext* c, GLsizei range)
{
  int count, i, list;
  GLList** lists;
  lists = c->shared_state.lists;
  count = 0;

  for (i = 0; i < MAX_DISPLAY_LISTS; i++) {
    if (lists[i] == NULL) {
      count++;

      if (count == range) {
        list = i - range + 1;

        for (i = 0; i < range; i++) {
          alloc_list(c, list + i);
        }

        return list;
      }
    }
    else {
      count = 0;
    }
  }

  return 0;
}
void gl_print_op(FILE* f, GLParam* p)
{
  int op;
  char* s;
  op = p[0].op;
  p++;
  s = op_table_str[op];

  while (*s != 0) {
    if (*s == '%') {
      s++;

      switch (*s++) {
      case 'f':
        fprintf(f, "%g", p[0].f);
        break;

      default:
        fprintf(f, "%d", p[0].i);
        break;
      }

      p++;
    }
    else {
      fputc(*s, f);
      s++;
    }
  }

  fprintf(f, "\n");
}
void gl_add_op(GLContext* c, GLParam* p)
{
  int op;
  op = p[0].op;

  if (c->exec_flag) {
    op_table_func[op](c, p);
  }

  if (c->compile_flag) {
    gl_compile_op(c, p);
  }

  if (c->print_flag) {
    gl_print_op(stderr, p);
  }
}
#endif
GLContext g_c[1] = {0};
void gl_add_op(GLParam* p)
{
  int op;
  GLContext* c = g_c;
  op = p[0].op;

  if (c->exec_flag) {
    op_table_func[op](c, p);
  }

  if (c->compile_flag) {
    //gl_compile_op(c, p);
  }

  if (c->print_flag) {
    //gl_print_op(stderr, p);
  }
}
/*
画虚线。
首先，使用glEnable(GL_LINE_STIPPLE);来启动虚线模式（使用glDisable(GL_LINE_STIPPLE)可以关闭之）。
然后，使用glLineStipple来设置虚线的样式。
void glLineStipple(GLint factor, GLushort pattern);
pattern是由1和0组成的长度为16的序列，从最低位开始看，如果为1，
则直线上接下来应该画的factor个点将被画为实的；如果为0，则直线上接下来应该画的factor个点将被画为虚的。
*/
void glopLineStipple(GLint factor, GLushort pattern)
{
}
void glopLineWidth(GLfloat width)
{
}

/* glVertex */
void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  // glopVertex
  GLParam p[5];
  p[0].op = OP_Vertex;
  p[1].f = x;
  p[2].f = y;
  p[3].f = z;
  p[4].f = w;
  gl_add_op(p);
}
void glVertex2f(GLfloat x, GLfloat y)
{
  glVertex4f(x, y, 0, 1);
}
void glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
  glVertex4f(x, y, z, 1);
}
// void glVertex2fv(const GLfloat *v);
#define DEF_GLVERTEXV(t, type) \
  void glVertex2 ## t ## v(const type *v) {glVertex4f((GLfloat)v[0], (GLfloat)v[1], 0, 1);}\
  void glVertex3 ## t ## v(const type *v) {glVertex4f((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2], 1);}\
  void glVertex4 ## t ## v(const type *v) {glVertex4f((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2], (GLfloat)v[3]);}

DEF_GLVERTEXV(d, GLdouble);
DEF_GLVERTEXV(f, GLfloat);
DEF_GLVERTEXV(i, GLint);
DEF_GLVERTEXV(s, GLshort);
#undef DEF_GLVERTEXV
/* glNormal */
void glNormal3f(GLfloat x, GLfloat y, GLfloat z)
{
  GLParam p[4];
  p[0].op = OP_Normal;
  p[1].f = x;
  p[2].f = y;
  p[3].f = z;
  gl_add_op(p);
}
void glNormal3fv(GLfloat* v)
{
  glNormal3f(v[0], v[1], v[2]);
}
/* glColor */
void glColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
  GLParam p[8];
  p[0].op = OP_Color;
  r = (GLfloat)BOUND(r, 0, 1);
  g = (GLfloat)BOUND(g, 0, 1);
  b = (GLfloat)BOUND(b, 0, 1);
  a = (GLfloat)BOUND(a, 0, 1);
  p[1].f = r;
  p[2].f = g;
  p[3].f = b;
  p[4].f = a;
  /* direct convertion to integer to go faster if no shading */
  p[5].ui = (unsigned int)(b * (ZB_POINT_RED_MAX - ZB_POINT_RED_MIN) + ZB_POINT_RED_MIN);
  p[6].ui = (unsigned int)(g * (ZB_POINT_GREEN_MAX - ZB_POINT_GREEN_MIN) + ZB_POINT_GREEN_MIN);
  p[7].ui = (unsigned int)(r * (ZB_POINT_BLUE_MAX - ZB_POINT_BLUE_MIN) + ZB_POINT_BLUE_MIN);
  gl_add_op(p);
}
void glColor4fv(GLfloat* v)
{
  glColor4f(v[0], v[1], v[2], v[3]);
}
void glColor3f(GLfloat x, GLfloat y, GLfloat z)
{
  glColor4f(x, y, z, 1);
}
void glColor3fv(const GLfloat* v)
{
  glColor4f(v[0], v[1], v[2], 1);
}
/* TexCoord */
void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
  GLParam p[5];
  p[0].op = OP_TexCoord;
  p[1].f = s;
  p[2].f = t;
  p[3].f = r;
  p[4].f = q;
  gl_add_op(p);
}
void glTexCoord2f(GLfloat s, GLfloat t)
{
  glTexCoord4f(s, t, 0, 1);
}
void glTexCoord2fv(GLfloat* v)
{
  glTexCoord4f(v[0], v[1], 0, 1);
}
void glEdgeFlag(int flag)
{
  GLParam p[2];
  p[0].op = OP_EdgeFlag;
  p[1].i = flag;
  gl_add_op(p);
}
/* misc */
void glShadeModel(int mode)
{
  GLParam p[2];
  assert(mode == GL_FLAT || mode == GL_SMOOTH);
  p[0].op = OP_ShadeModel;
  p[1].i = mode;
  gl_add_op(p);
}
void glCullFace(int mode)
{
  GLParam p[2];
  assert(mode == GL_BACK ||
      mode == GL_FRONT ||
      mode == GL_FRONT_AND_BACK);
  p[0].op = OP_CullFace;
  p[1].i = mode;
  gl_add_op(p);
}
void glFrontFace(int mode)
{
  GLParam p[2];
  assert(mode == GL_CCW || mode == GL_CW);
  mode = (mode != GL_CCW);
  p[0].op = OP_FrontFace;
  p[1].i = mode;
  gl_add_op(p);
}
void glPolygonMode(int face, int mode)
{
  // glopPolygonMode
  GLParam p[3];
  assert(face == GL_BACK ||
      face == GL_FRONT ||
      face == GL_FRONT_AND_BACK);
  assert(mode == GL_POINT || mode == GL_LINE || mode == GL_FILL);
  p[0].op = OP_PolygonMode;
  p[1].i = face;
  p[2].i = mode;
  gl_add_op(p);
}
/* glEnable / glDisable */
void glEnable(int cap)
{
  // glopEnableDisable
  GLParam p[3];
  p[0].op = OP_EnableDisable;
  p[1].i = cap;
  p[2].i = 1;
  gl_add_op(p);
}
void glDisable(int cap)
{
  GLParam p[3];
  p[0].op = OP_EnableDisable;
  p[1].i = cap;
  p[2].i = 0;
  gl_add_op(p);
}
/* glBegin / glEnd */
void glBegin(GLenum mode)
{
  GLParam p[2];
  p[0].op = OP_Begin;
  p[1].i = mode;
  gl_add_op(p);
}
void glEnd(void)
{
  GLParam p[1];
  p[0].op = OP_End;
  gl_add_op(p);
}
/* matrix */
void glMatrixMode(GLenum mode)
{
  // glopMatrixMode
  GLParam p[2];
  p[0].op = OP_MatrixMode;
  p[1].i = mode;
  gl_add_op(p);
}
void glLoadMatrixf(const GLfloat* m)
{
  int i;
  GLParam p[17];
  p[0].op = OP_LoadMatrix;

  for (i = 0; i < 16; i++) {
    p[i + 1].f = (GLfloat)m[i];
  }

  gl_add_op(p);
}
void glLoadMatrixd(const GLdouble* m)
{
  int i;
  GLParam p[17];
  p[0].op = OP_LoadMatrix;

  for (i = 0; i < 16; i++) {
    p[i + 1].f = (GLfloat)m[i];
  }

  gl_add_op(p);
}
void glLoadIdentity(void)
{
  GLParam p[1];
  p[0].op = OP_LoadIdentity;
  gl_add_op(p);
}
void glMultMatrixf(const GLfloat* m)
{
  GLParam p[17];
  int i;
  p[0].op = OP_MultMatrix;

  for (i = 0; i < 16; i++) {
    p[i + 1].f = m[i];
  }

  gl_add_op(p);
}
void glPushMatrix(void)
{
  GLParam p[1];
  p[0].op = OP_PushMatrix;
  gl_add_op(p);
}
void glPopMatrix(void)
{
  GLParam p[1];
  p[0].op = OP_PopMatrix;
  gl_add_op(p);
}
void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  GLParam p[5];
  p[0].op = OP_Rotate;
  p[1].f = angle;
  p[2].f = x;
  p[3].f = y;
  p[4].f = z;
  gl_add_op(p);
}
void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
  // glopTranslate
  GLParam p[4];
  p[0].op = OP_Translate;
  p[1].f = x;
  p[2].f = y;
  p[3].f = z;
  gl_add_op(p);
}
void glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
  GLParam p[4];
  p[0].op = OP_Translate;
  p[1].f = (float)x;
  p[2].f = (float)y;
  p[3].f = (float)z;
  gl_add_op(p);
  return ;
}
void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
  GLParam p[4];
  p[0].op = OP_Scale;
  p[1].f = x;
  p[2].f = y;
  p[3].f = z;
  gl_add_op(p);
}
void glViewport(int x, int y, int width, int height)
{
  GLParam p[5];
  p[0].op = OP_Viewport;
  p[1].i = x;
  p[2].i = y;
  p[3].i = width;
  p[4].i = height;
  gl_add_op(p);
}
void glFrustum(double left, double right, double bottom, double top, double nearv, double farv)
{
  // glopFrustum
  GLParam p[7];
  p[0].op = OP_Frustum;
  p[1].f = (GLfloat)left;
  p[2].f = (GLfloat)right;
  p[3].f = (GLfloat)bottom;
  p[4].f = (GLfloat)top;
  p[5].f = (GLfloat)nearv;
  p[6].f = (GLfloat)farv;
  gl_add_op(p);
}
void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearv, GLdouble farv)
{
  // glopOrtho
  GLParam p[7];
  p[0].op = OP_Ortho;
  p[1].f = (GLfloat)left;
  p[2].f = (GLfloat)right;
  p[3].f = (GLfloat)bottom;
  p[4].f = (GLfloat)top;
  p[5].f = (GLfloat)nearv;
  p[6].f = (GLfloat)farv;
  gl_add_op(p);
  return ;
}
/* lightening */
void glMaterialfv(GLenum mode, GLenum type, const GLfloat* v)
{
  GLParam p[7];
  int i, n;
  assert(mode == GL_FRONT || mode == GL_BACK || mode == GL_FRONT_AND_BACK);
  p[0].op = OP_Material;
  p[1].i = mode;
  p[2].i = type;
  n = 4;

  if (type == GL_SHININESS) {
    n = 1;
  }

  for (i = 0; i < 4; i++) {
    p[3 + i].f = v[i];
  }

  for (i = n; i < 4; i++) {
    p[3 + i].f = 0;
  }

  gl_add_op(p);
}
void glMaterialf(int mode, int type, GLfloat v)
{
  GLParam p[7];
  int i;
  p[0].op = OP_Material;
  p[1].i = mode;
  p[2].i = type;
  p[3].f = v;

  for (i = 0; i < 3; i++) {
    p[4 + i].f = 0;
  }

  gl_add_op(p);
}
void glColorMaterial(int mode, int type)
{
  GLParam p[3];
  p[0].op = OP_ColorMaterial;
  p[1].i = mode;
  p[2].i = type;
  gl_add_op(p);
}
void glLightfv(int light, int type, const GLfloat* v)
{
  // glopLight
  GLParam p[7];
  int i;
  p[0].op = OP_Light;
  p[1].i = light;
  p[2].i = type;

  /* TODO: 3 composants ? */
  for (i = 0; i < 4; i++) {
    p[3 + i].f = v[i];
  }

  gl_add_op(p);
}
void glLightf(int light, int type, GLfloat v)
{
  GLParam p[7];
  int i;
  p[0].op = OP_Light;
  p[1].i = light;
  p[2].i = type;
  p[3].f = v;

  for (i = 0; i < 3; i++) {
    p[4 + i].f = 0;
  }

  gl_add_op(p);
}
void glLightModeli(int pname, int param)
{
  GLParam p[7];
  int i;
  p[0].op = OP_LightModel;
  p[1].i = pname;
  p[2].f = (GLfloat)param;

  for (i = 0; i < 4; i++) {
    p[3 + i].f = 0;
  }

  gl_add_op(p);
}
void glLightModelfv(int pname, GLfloat* param)
{
  GLParam p[6];
  int i;
  p[0].op = OP_LightModel;
  p[1].i = pname;

  for (i = 0; i < 4; i++) {
    p[2 + i].f = param[i];
  }

  gl_add_op(p);
}
/* clear */
void glClear(int mask)
{
  // glopClear
  GLParam p[2];
  p[0].op = OP_Clear;
  p[1].i = mask;
  gl_add_op(p);
}
void glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
  GLParam p[5];
  p[0].op = OP_ClearColor;
  p[1].f = BOUND(r, 0, 1);
  p[2].f = BOUND(g, 0, 1);
  p[3].f = BOUND(b, 0, 1);
  p[4].f = a;
  gl_add_op(p);
}
void glClearDepth(double depth)
{
  GLParam p[2];
  p[0].op = OP_ClearDepth;
  p[1].f = (float)depth;
  gl_add_op(p);
}
/* textures */
void glTexImage2D(int target, int level, int components,
    int width, int height, int border,
    int format, int type, void* pixels)
{
  GLParam p[10];
  p[0].op = OP_TexImage2D;
  p[1].i = target;
  p[2].i = level;
  p[3].i = components;
  p[4].i = width;
  p[5].i = height;
  p[6].i = border;
  p[7].i = format;
  p[8].i = type;
  p[9].p = pixels;
  gl_add_op(p);
}
void glBindTexture(GLenum target, GLenum texture)
{
  GLParam p[3];
  p[0].op = OP_BindTexture;
  p[1].i = target;
  p[2].i = texture;
  gl_add_op(p);
}
void glTexEnvi(int target, int pname, int param)
{
  GLParam p[8];
  p[0].op = OP_TexEnv;
  p[1].i = target;
  p[2].i = pname;
  p[3].i = param;
  p[4].f = 0;
  p[5].f = 0;
  p[6].f = 0;
  p[7].f = 0;
  gl_add_op(p);
}
void glTexParameteri(int target, int pname, int param)
{
  GLParam p[8];
  p[0].op = OP_TexParameter;
  p[1].i = target;
  p[2].i = pname;
  p[3].i = param;
  p[4].f = 0;
  p[5].f = 0;
  p[6].f = 0;
  p[7].f = 0;
  gl_add_op(p);
}
void glPixelStorei(int pname, int param)
{
  GLParam p[3];
  p[0].op = OP_PixelStore;
  p[1].i = pname;
  p[2].i = param;
  gl_add_op(p);
}
/* selection */
void glInitNames(void)
{
  GLParam p[1];
  p[0].op = OP_InitNames;
  gl_add_op(p);
}
void glPushName(unsigned int name)
{
  GLParam p[2];
  p[0].op = OP_PushName;
  p[1].i = name;
  gl_add_op(p);
}
void glPopName(void)
{
  GLParam p[1];
  p[0].op = OP_PopName;
  gl_add_op(p);
}
void glLoadName(unsigned int name)
{
  GLParam p[2];
  p[0].op = OP_LoadName;
  p[1].i = name;
  gl_add_op(p);
}
void glPolygonOffset(GLfloat factor, GLfloat units)
{
  GLParam p[3];
  p[0].op = OP_PolygonOffset;
  p[1].f = factor;
  p[2].f = units;
}
/* Special Functions */
void glCallList(unsigned int list)
{
  GLParam p[2];
  p[0].op = OP_CallList;
  p[1].i = list;
  gl_add_op(p);
}
void glFlush(void)
{
  /* nothing to do */
}
void glHint(int target, int mode)
{
  GLParam p[3];
  p[0].op = OP_Hint;
  p[1].i = target;
  p[2].i = mode;
  gl_add_op(p);
}
/* Non standard functions */
void glDebug(int mode)
{
  GLContext* c = g_c;
  c->print_flag = mode;
}
void glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
  glBegin(GL_POLYGON);
  glVertex2f(x1, y1);
  glVertex2f(x1, y2);
  glVertex2f(x2, y2);
  glVertex2f(x2, y1);
  glEnd();
  glFlush();
}
void glClose(void)
{
  GLContext* c = g_c;
  endSharedState(c);

  if (c->zb->zbuf) {
    free(c->zb->zbuf);
  }
}
void glDeleteTextures(int n, const unsigned int* textures)
{
  GLContext* c = g_c;
  int i;
  GLTexture* t;

  for (i = 0; i < n; i++) {
    t = find_texture(c, textures[i]);

    if (t != NULL && t != 0) {
      if (t == c->current_texture) {
        glBindTexture(GL_TEXTURE_2D, 0);
      }

      free_texture(c, textures[i]);
    }
  }
}
void glInit()
{
  GLContext* c = g_c;
  GLViewport* v = &c->viewport;
  ZBuffer* zb = c->zb;
  int i;
  /* allocate GLVertex array */
  c->vertex_max = POLYGON_MAX_VERTEX;
  c->vertex = (GLVertex*)gl_malloc(POLYGON_MAX_VERTEX * sizeof(GLVertex));
  /* viewport */
  v->xmin = 0;
  v->ymin = 0;
  v->w = zb->w;
  v->h = zb->h;
  v->updated = 1;
  /* shared state */
  initSharedState(c);
  /* lists */
  c->exec_flag = 1;
  c->compile_flag = 0;
  c->print_flag = 0;
  c->in_begin = 0;

  /* lights */
  for (i = 0; i < MAX_LIGHTS; i++) {
    GLLight* l = &c->lights[i];
    gl_V4_New(l->ambient, 0, 0, 0, 1);
    gl_V4_New(l->diffuse, 1, 1, 1, 1);
    gl_V4_New(l->specular, 1, 1, 1, 1);
    gl_V4_New(l->position, 0, 0, 1, 0);
    gl_V3_New(l->norm_position, 0, 0, 1);
    gl_V3_New(l->spot_direction, 0, 0, -1);
    gl_V3_New(l->norm_spot_direction, 0, 0, -1);
    l->spot_exponent = 0;
    l->spot_cutoff = 180;
    l->attenuation[0] = 1;
    l->attenuation[1] = 0;
    l->attenuation[2] = 0;
    l->enabled = 0;
  }

  c->first_light = NULL;
  gl_V4_New(c->ambient_light_model, 0.2f, 0.2f, 0.2f, 1.f);
  c->local_light_model = 0;
  c->lighting_enabled = 0;
  c->light_model_two_side = 0;

  /* default materials */
  for (i = 0; i < 2; i++) {
    GLMaterial* m = &c->materials[i];
    gl_V4_New(m->emission, 0, 0, 0, 1);
    gl_V4_New(m->ambient, 0.2f, 0.2f, 0.2f, 1.f);
    gl_V4_New(m->diffuse, 0.8f, 0.8f, 0.8f, 1.f);
    gl_V4_New(m->specular, 0, 0, 0, 1);
    m->shininess = 0;
  }

  c->current_color_material_mode = GL_FRONT_AND_BACK;
  c->current_color_material_type = GL_AMBIENT_AND_DIFFUSE;
  c->color_material_enabled = 0;
  /* textures */
  glInitTextures(c);
  /* default state */
  gl_V4_New(c->current_color, 1.f, 1.f, 1.f, 1.f);
  c->longcurrent_color[0] = 65535;
  c->longcurrent_color[1] = 65535;
  c->longcurrent_color[2] = 65535;
  gl_V4_New(c->current_normal, 1.f, 0.f, 0.f, 0.f);
  c->current_edge_flag = 1;
  gl_V4_New(c->current_tex_coord, 0, 0, 0, 1);
  c->polygon_mode_front = GL_FILL;
  c->polygon_mode_back = GL_FILL;
  c->current_front_face = 0; /* 0 = GL_CCW 1 = GL_CW */
  c->current_cull_face = GL_BACK;
  c->current_shade_model = GL_SMOOTH;
  c->cull_face_enabled = 0;
  /* clear */
  gl_V4_New(c->clear_color, 0, 0, 0, 0);
  c->clear_depth = 0;
  /* selection */
  c->render_mode = GL_RENDER;
  c->select_buffer = NULL;
  c->name_stack_size = 0;
  /* matrix */
  c->matrix_mode = 0;
  c->matrix_stack_depth_max[0] = MAX_MODELVIEW_STACK_DEPTH;
  c->matrix_stack_depth_max[1] = MAX_PROJECTION_STACK_DEPTH;
  c->matrix_stack_depth_max[2] = MAX_TEXTURE_STACK_DEPTH;

  for (i = 0; i < 3; i++) {
    c->matrix_stack[i] = (M4*)gl_malloc(c->matrix_stack_depth_max[i] * sizeof(M4));
    c->matrix_stack_ptr[i] = c->matrix_stack[i];
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  c->matrix_model_projection_updated = 1;
  /* opengl 1.1 arrays */
  c->client_states = 0;
  /* opengl 1.1 polygon offset */
  c->offset_states = 0;
  /* specular buffer */
  c->specbuf_first = NULL;
  c->specbuf_used_counter = 0;
  c->specbuf_num_buffers = 0;
  /* depth test */
  c->depth_test = 0;
}
void glPointSize(GLfloat size)
{
  // glopPointSize
  GLParam p[2];
  p[0].op = OP_PointSize;
  p[1].i = (int)size;
  gl_add_op(p);
}
void glPolygonStipple(const GLubyte* mask)
{
  ASSERT(0);
}
