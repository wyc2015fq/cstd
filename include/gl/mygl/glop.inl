
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
#define X v[0]
#define Y v[1]
#define Z v[2]
#define W v[3]
typedef struct {
  float v[3];
} V3;
typedef struct {
  float v[4];
} V4;
#define V3_Set(v3, x, y, z)  ((v3).v[0] = (x), (v3).v[1] = (y), (v3).v[2] = (z))
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
void gl_MoveV3(V3* a, V3* b)
{
  memcpy(a, b, sizeof(V3));
}
void gl_MulM4V3(V3* a, M4* b, V3* c)
{
  a->X = b->m[0][0] * c->X + b->m[0][1] * c->Y + b->m[0][2] * c->Z + b->m[0][3];
  a->Y = b->m[1][0] * c->X + b->m[1][1] * c->Y + b->m[1][2] * c->Z + b->m[1][3];
  a->Z = b->m[2][0] * c->X + b->m[2][1] * c->Y + b->m[2][2] * c->Z + b->m[2][3];
}
void gl_MulM3V3(V3* a, M4* b, V3* c)
{
  a->X = b->m[0][0] * c->X + b->m[0][1] * c->Y + b->m[0][2] * c->Z;
  a->Y = b->m[1][0] * c->X + b->m[1][1] * c->Y + b->m[1][2] * c->Z;
  a->Z = b->m[2][0] * c->X + b->m[2][1] * c->Y + b->m[2][2] * c->Z;
}
void gl_M4_MulV4(V4* a, M4* b, V4* c)
{
  a->X = b->m[0][0] * c->X + b->m[0][1] * c->Y + b->m[0][2] * c->Z + b->m[0][3] * c->W;
  a->Y = b->m[1][0] * c->X + b->m[1][1] * c->Y + b->m[1][2] * c->Z + b->m[1][3] * c->W;
  a->Z = b->m[2][0] * c->X + b->m[2][1] * c->Y + b->m[2][2] * c->Z + b->m[2][3] * c->W;
  a->W = b->m[3][0] * c->X + b->m[3][1] * c->Y + b->m[3][2] * c->Z + b->m[3][3] * c->W;
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
/* inversion of an orthogonal matrix of type Y=M.X+P */
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
  /* identitée dans r */
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
int gl_V3_Norm(V3* a)
{
  GLfloat n;
  n = (float)sqrt(a->X * a->X + a->Y * a->Y + a->Z * a->Z);
  if (n == 0) {
    return 1;
  }
  a->X /= n;
  a->Y /= n;
  a->Z /= n;
  return 0;
}
V3 gl_V3_New(GLfloat x, GLfloat y, GLfloat z)
{
  V3 a;
  a.X = x;
  a.Y = y;
  a.Z = z;
  return a;
}
V4 gl_V4_New(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  V4 a;
  a.X = x;
  a.Y = y;
  a.Z = z;
  a.W = w;
  return a;
}
V3 gl_V3_Cross(V3 a, V3 b)
{
  return gl_V3_New(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
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
#define MAX_MODELVIEW_STACK_DEPTH  32
#define MAX_PROJECTION_STACK_DEPTH 8
#define MAX_TEXTURE_STACK_DEPTH    8
#define MAX_NAME_STACK_DEPTH       64
#define MAX_TEXTURE_LEVELS         11
#define MAX_LIGHTS                 16
#define VERTEX_HASH_SIZE  1031
#define MAX_DISPLAY_LISTS  1024
#define OP_BUFFER_MAX_SIZE 512
#define TGL_OFFSET_FILL    0x1
#define TGL_OFFSET_LINE    0x2
#define TGL_OFFSET_POINT   0x4
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
#define ZB_MODE_5R6G5B  1  /* true color 16 bits */
#define ZB_MODE_INDEX   2  /* color index 8 bits */
#define ZB_MODE_RGBA    3  /* 32 bit rgba mode */
#define ZB_MODE_RGB24   4  /* 24 bit rgb mode */
#define ZB_NB_COLORS    225 /* number of colors for 8 bit display */
#if 0
#define RGB_TO_PIXEL15(pp, r,g,b)   (*(unsigned short*)pp = (((r) >> 1) & 0x7c00) | (((g) >> 6) & 0x03e0) | ((b) >> 11))
#define RGB_TO_PIXEL16(pp, r,g,b)   (*(unsigned short*)pp = ((r) & 0xF800) | (((g) >> 5) & 0x07E0) | ((b) >> 11))
#define RGB_TO_PIXEL24(pp, r,g,b)   ((pp)[0] = r, (pp)[1] = g, (pp)[2] = b)
#define RGB_TO_PIXEL32(pp, r,g,b)   (*(unsigned int*)pp = (((r) << 8) & 0xff0000) | ((g) & 0xff00) | ((b) >> 8))
#else
#define RGB_TO_PIXEL15(pp, r,g,b)   (*(unsigned short*)pp = (((b) >> 1) & 0x7c00) | (((g) >> 6) & 0x03e0) | ((r) >> 11))
#define RGB_TO_PIXEL16(pp, r,g,b)   (*(unsigned short*)pp = ((b) & 0xF800) | (((g) >> 5) & 0x07E0) | ((r) >> 11))
#define RGB_TO_PIXEL24(pp, r,g,b)   ((pp)[0] = b, (pp)[1] = g, (pp)[2] = r)
#define RGB_TO_PIXEL32(pp, r,g,b)   (*(unsigned int*)pp = (((b) << 8) & 0xff0000) | ((g) & 0xff00) | ((r) >> 8))
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
  int x, y, z;   /* integer coordinates in the zbuffer */
  int s, t;      /* coordinates for the mapping */
  int r, g, b;   /* color indexes */
  float sz, tz;  /* temporary coordinates for mapping */
} ZBufferPoint;
typedef struct GLVertex {
  int edge_flag;
  V3 normal;
  V4 coord;
  V4 tex_coord;
  V4 color;
  /* computed values */
  V4 ec;                /* eye coordinates */
  V4 pc;                /* coordinates in the normalized volume */
  int clip_code;        /* clip code */
  ZBufferPoint zp;      /* integer coordinates for the rasterization */
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
  /* opengl 1.1 arrays  */
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
static GLContext g_c[1] = {0};
CC_INLINE GLContext* gl_get_context()
{
  return g_c;
}
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
#include "glop_impl.inl"
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
void   glNewList(unsigned int list, int mode)
{
  GLList* l;
  GLContext* c = gl_get_context();
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
void   glEndList(void)
{
  GLContext* c = gl_get_context();
  GLParam p[1];
  assert(c->compile_flag == 1);
  /* end of list */
  p[0].op = OP_EndList;
  gl_compile_op(c, p);
  c->compile_flag = 0;
  c->exec_flag = 1;
}
GLboolean   glIsList(GLuint list)
{
  GLContext* c = gl_get_context();
  GLList* l;
  l = find_list(c, list);
  return (l != NULL);
}
GLuint   glGenLists(GLsizei range)
{
  GLContext* c = gl_get_context();
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
void gl_add_op(GLParam* p)
{
  GLContext* c = gl_get_context();
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

