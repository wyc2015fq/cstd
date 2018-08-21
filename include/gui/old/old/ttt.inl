
CC_INLINE float DegToRad(float r)
{
  return DegreeToRadian(r);
}

CC_INLINE float RadianToDegree(float r)
{
  float degree = r * (180.0f / PI);
  return degree;
}

CC_INLINE float RadToDeg(float r)
{
  return RadianToDegree(r);
}

struct M4_T {
  float m[4][4];
};
typedef M4_T M4[1];

CC_INLINE double V3Length(const float* v)
{
  float sum = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
  double vectorlength = sqrt(sum);
  return vectorlength;
}

inline double V3Dotf(const float* a, const float* b)
{
  double result = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
  return result;
}

CC_INLINE void V4Normalize(float* t, const float4 v)
{
		float len = V3Length(v);
    t[0]=v[0]/len;
    t[1]=v[1]/len;
    t[2]=v[2]/len;
    t[3]=1.f;
}

// z = x + y
CC_INLINE void V4Add(float* z, const float* x, const float* y)
{
  z[0] = x[0] + y[0];
  z[1] = x[1] + y[1];
  z[2] = x[2] + y[2];
  z[3] = 1.0;
}

// z = x - y
CC_INLINE void V4Sub(float* z, const float* x, const float* y)
{
  z[0] = x[0] - y[0];
  z[1] = x[1] - y[1];
  z[2] = x[2] - y[2];
  z[3] = 1.0;
}

// z = x - y
CC_INLINE void V4Scale(float* z, const float* x, float t)
{
  z[0] = x[0] * t;
  z[1] = x[1] * t;
  z[2] = x[2] * t;
  z[3] = 1.0;
}

CC_INLINE void V3CrossProduct(float* c, const float* a, const float* b)
{
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];
  c[3] = 0.0f;
  return ;
}

CC_INLINE void M4Identity(M4 m) {
  M4 g_MatrixCPU_Identity = {
    1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };
  *m = *g_MatrixCPU_Identity;
}

CC_INLINE void M4SetColumn(M4 m, int column, const float4 vec)
{
  float *p = &m->m[0][0] + column;
  assert(column>=0 && column<4);
  p[0] = vec[0];
  p[4] = vec[1];
  p[8] = vec[2];
  p[12] = vec[3];
}

CC_INLINE void M4Mul(M4 c, const M4 a, const M4 b)
{
  M4 z;
  int i, j;
  
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      z->m[j][i] = (a->m[j][0] * b->m[0][i]) + (a->m[j][1] * b->m[1][i]) + (a->m[j][2] * b->m[2][i]) + (a->m[j][3] * b->m[3][i]);
    }
  }
  
  c[0] = *z;
}
CC_INLINE void M4Apply3(float* a, const float* v, const M4 m)
{
  float x = v[0], y = v[1], z = v[2];
  a[0] = x * m->m[0][0] + y * m->m[1][0] + z * m->m[2][0] + m->m[3][0];
  a[1] = x * m->m[0][1] + y * m->m[1][1] + z * m->m[2][1] + m->m[3][1];
  a[2] = x * m->m[0][2] + y * m->m[1][2] + z * m->m[2][2] + m->m[3][2];
  a[3] = x * m->m[0][3] + y * m->m[1][3] + z * m->m[2][3] + m->m[3][3];
}
CC_INLINE void M4Apply4(float* a, const float* v, const M4 m)
{
  float x = v[0], y = v[1], z = v[2], w = v[3];
  a[0] = x * m->m[0][0] + y * m->m[1][0] + z * m->m[2][0] + w * m->m[3][0];
  a[1] = x * m->m[0][1] + y * m->m[1][1] + z * m->m[2][1] + w * m->m[3][1];
  a[2] = x * m->m[0][2] + y * m->m[1][2] + z * m->m[2][2] + w * m->m[3][2];
  a[3] = x * m->m[0][3] + y * m->m[1][3] + z * m->m[2][3] + w * m->m[3][3];
}
// left hand coord system
CC_INLINE void GutMatrixLookAtLH(M4 matrix, const float4 eye, const float4 lookat, const float4 up)
{
  float4 up_normalized, zaxis, xaxis, yaxis;
  V4Normalize(up_normalized, up);
	V4Sub(zaxis, lookat, eye);
  V4Normalize(zaxis, zaxis);
	V3CrossProduct(xaxis, up_normalized, zaxis);
  V3CrossProduct(yaxis, zaxis, xaxis);

	M4Identity(matrix);

	M4SetColumn(matrix, 0, xaxis);
	M4SetColumn(matrix, 1, yaxis);
	M4SetColumn(matrix, 2, zaxis);
	matrix->m[3][0] = -V3Dotf(xaxis, eye);
	matrix->m[3][1] = -V3Dotf(yaxis, eye);
	matrix->m[3][2] = -V3Dotf(zaxis, eye);
	return ;
}

// right hand coord system
// eye = `镜头位置`
// lookat = `镜头对准的位置`
// up = `镜头正上方的方向`
CC_INLINE void GutMatrixLookAtRH(M4 matrix, const float4 eye, const float4 lookat, const float4 up)
{
	float4 up_normalized, zaxis, xaxis, yaxis;
  V4Normalize(up_normalized, up);
	V4Sub(zaxis, eye, lookat);
  V4Normalize(zaxis, zaxis);
	V3CrossProduct(xaxis, up_normalized, zaxis);
	V3CrossProduct(yaxis, zaxis, xaxis);

	M4Identity(matrix);

	M4SetColumn(matrix, 0, xaxis);
	M4SetColumn(matrix, 1, yaxis);
	M4SetColumn(matrix, 2, zaxis);
	matrix->m[3][0] = -V3Dotf(xaxis, eye);
	matrix->m[3][1] = -V3Dotf(yaxis, eye);
	matrix->m[3][2] = -V3Dotf(zaxis, eye);
	return ;
}

// w = `镜头水平方向可以看到的范围`
// h = `镜头垂直方向可以看到的范围`
// z_hear = `镜头可以看到的最近距离`
// z_far  = `镜头可以看到的最远距离`
CC_INLINE void GutMatrixOrthoRH_DirectX(M4 matrix, float w, float h, float z_near, float z_far)
{
  M4Identity(matrix);
  matrix->m[0][0] = 2.0f/w;
  matrix->m[1][1] = 2.0f/h;
  matrix->m[2][2] = 1.0f/(z_near - z_far);
  matrix->m[3][2] = z_near / (z_near - z_far);
}

// OpenGL native right hand system
// fovy = `垂直方向的视角`
// aspect = `水平方向视角对重直方向视角的比值`
// z_hear = `镜头可以看到的最近距离`
// z_far = `镜头可以看到的最远距离`
CC_INLINE void GutMatrixOrthoRH_OpenGL(M4 matrix, float w, float h, float z_near, float z_far)
{
  M4Identity(matrix);
  matrix->m[0][0] = 2.0f/w;
  matrix->m[1][1] = 2.0f/h;
  matrix->m[2][2] = 2.0f/(z_near - z_far);
  matrix->m[3][2] = (z_far + z_near)/(z_near - z_far);
  matrix->m[3][3] = 1.0f;
}

double cot(double x) {
  return (1./tan(x));
}

// Direct3D native left hand system
// fovy = 垂直方向的视角
// aspect = 水平方向视角对重直方向视角的比值
// z_hear = 镜头可以看到的最近距离
// z_far = 镜头可以看到的最远距离
void GutMatrixPerspective_DirectX(M4 matrix, float fovy, float aspect, float z_near, float z_far)
{
  M4Identity(matrix);
  
  float fovy_radian = DegreeToRadian(fovy);
  float yscale = cot(fovy_radian/2.0f);
  float xscale = yscale * aspect;
  
  matrix->m[0][0] = xscale;
  matrix->m[1][1] = yscale;
  matrix->m[2][2] = z_far / (z_far - z_near);
  matrix->m[2][3] = 1.0f;
  matrix->m[3][2] = -(z_near * z_far) /(z_far - z_near);
  matrix->m[3][3] = 0.0f;
}

// fovy = 垂直方向的视角
// aspect = `水平方向视角对重直方向视角的比值`
// z_hear = `镜头可以看到的最近距离`
// z_far = `镜头可以看到的最远距离`
void GutMatrixPerspectiveRH_DirectX(M4 matrix, float fovy, float aspect, float z_near, float z_far)
{
  M4Identity(matrix);
  
  float fovy_radian = DegreeToRadian(fovy);
  float yscale = cot(fovy_radian/2.0f);
  float xscale = yscale * aspect;
  
  matrix->m[0][0] = xscale;
  matrix->m[1][1] = yscale;
  matrix->m[2][2] = z_far / (z_near - z_far);
  matrix->m[2][3] = -1.0f;
  matrix->m[3][2] = (z_near * z_far) /(z_near - z_far);
  matrix->m[3][3] = 0.0f;
}

// OpenGL native right hand system
// fovy = `垂直方向的视角`
// aspect = `水平方向视角对重直方向视角的比值`
// z_hear = `镜头可以看到的最近距离`
// z_far = `镜头可以看到的最远距离`
void GutMatrixPerspectiveRH_OpenGL(M4 matrix, float fovy, float aspect, float z_near, float z_far)
{
  M4Identity(matrix);
  
  float fovy_radian = DegreeToRadian(fovy);
  float yscale = cot(fovy_radian/2.0f);
  float xscale = yscale * aspect;
  
  matrix->m[0][0] = xscale;
  matrix->m[1][1] = yscale;
  matrix->m[2][2] = (z_far + z_near)/(z_near - z_far);
  matrix->m[2][3] = -1.0f;
  matrix->m[3][2] = 2.0f * z_far * z_near / (z_near - z_far);
  matrix->m[3][3] = 0.0f;
}

#if 0
M4 GutMatrixOrthoRH(float w, float h, float z_near, float z_far)
{
	switch(GutGetGraphicsDeviceType())
	{
	case GUT_OPENGL:
		return GutMatrixOrthoRH_OpenGL(w, h, z_near, z_far);
		break;
	case GUT_DX9:
	case GUT_DX10:
	default:
		return GutMatrixOrthoRH_DirectX(w, h, z_near, z_far);
		break;
	}
}

M4 GutMatrixPerspectiveRH(float fovy, float aspect, float z_near, float z_far)
{
	switch(GutGetGraphicsDeviceType())
	{
	case GUT_OPENGL:
		return GutMatrixPerspectiveRH_OpenGL(fovy, aspect, z_near, z_far);
		break;
	case GUT_DX9:
	case GUT_DX10:
	default:
		return GutMatrixPerspectiveRH_DirectX(fovy, aspect, z_near, z_far);
		break;
	}
}

M4 GutMatrixOrtho_OpenGL(float w, float h, float z_near, float z_far)
{
	return GutMatrixOrthoRH_OpenGL(w, h, z_near, z_far);
}

M4 GutMatrixPerspective_OpenGL(float fovy, float aspect, float z_near, float z_far)
{
	return GutMatrixPerspectiveRH_OpenGL(fovy, aspect, z_near, z_far);
}

// 生成x_grids * y_grids个格子的棋盘格模型
bool GutCreateGrids(int x_grids, int y_grids, 
					  Vertex_V3N3 **ppVertices, int &num_vertices, 
					  unsigned short **ppIndices, int &num_indices, int &num_triangles)
{
	const int triangles_per_row = x_grids * 2;
	const int indices_per_row = triangles_per_row + 2;
	
	num_vertices = (x_grids + 1) * (y_grids + 1);
	Vertex_V3N3 *pVertices = (Vertex_V3N3 *) GutAllocate16BytesAlignedMemory(sizeof(Vertex_V3N3)*num_vertices);
	*ppVertices = pVertices;
	if ( pVertices==NULL )
		return false;

	num_indices = indices_per_row * y_grids;
	unsigned short *pIndices = new unsigned short[num_indices];
	*ppIndices = pIndices;
	if ( pIndices==NULL )
	{
		GutRelease16BytesAlignedMemory(pVertices);
		return false;
	}
	// 使用triangle strip时, 三角形数目永远等于索引值数目减去2
	num_triangles = num_indices-2;

	float4 vCorner(-0.5f, 0.5f, 0.0f, 1.0f);
	float4 vStep(1.0f/float(x_grids), -1.0f/float(y_grids), 0.0f, 0.0f);
	float4 vPosition = vCorner;
	float4 vNormal(0.0f, 0.0f, 1.0f);

	int x,y;
	int vertex_index = 0;

	for ( y=0; y<=y_grids; y++)
	{
		vPosition[0] = vCorner[0];
		for ( x=0; x<=x_grids; x++, vertex_index++)
		{
			vPosition.StoreXYZ(pVertices[vertex_index].m_Position);
			vNormal.StoreXYZ(pVertices[vertex_index].m_Normal);
			vPosition[0] += vStep[0];
		}
		vPosition[1] += vStep[1];
	}

	const int vertices_per_row = x_grids + 1;
	bool from_left_to_right = true;
	int  index_index = 0;

	for ( y=0; y<y_grids; y++ )
	{
		if ( from_left_to_right )
		{
			// 在奇数列的时候, 三角形从左排到右
			pIndices[index_index++] = y * vertices_per_row;
			pIndices[index_index++] = y * vertices_per_row + vertices_per_row;

			for ( x=0; x<x_grids; x++ )
			{
				vertex_index = y * vertices_per_row + x;
				pIndices[index_index++] = vertex_index + 1;
				pIndices[index_index++] = vertex_index + 1 + vertices_per_row;
			}
		}
		else
		{
			// 在偶数列的时候, 三角形从右排到左
			pIndices[index_index++] = y * vertices_per_row + x_grids;
			pIndices[index_index++] = (y+1) * vertices_per_row + x_grids;

			for ( x=x_grids; x>0; x-- )
			{
				vertex_index = y * vertices_per_row + x;
				pIndices[index_index++] = vertex_index - 1;
				pIndices[index_index++] = vertex_index - 1 + vertices_per_row;
			}
		}
		from_left_to_right = !from_left_to_right;
	}

	return true;
}

bool GutReleaseGrids(Vertex_V3N3 **ppVertices, unsigned short **ppIndices)
{
	if ( *ppVertices )
	{
		GutRelease16BytesAlignedMemory(*ppVertices);
		*ppVertices = NULL;
	}

	if ( *ppIndices )
	{
		delete [] *ppIndices;
		*ppIndices = NULL;
	}

	return true;
}

void GutConvertColor(float4 &vColor, unsigned char rgba[4])
{
	float4 c = vColor * 255.0f;
	
	switch( GutGetGraphicsDeviceType() )
	{
	case GUT_DX9:
		rgba[0] = (unsigned char ) c[2];
		rgba[1] = (unsigned char ) c[1];
		rgba[2] = (unsigned char ) c[0];
		rgba[3] = (unsigned char ) c[3];
		break;
	default:
		rgba[0] = (unsigned char ) c[0];
		rgba[1] = (unsigned char ) c[1];
		rgba[2] = (unsigned char ) c[2];
		rgba[3] = (unsigned char ) c[3];
		break;
	}
}

static char g_szShaderPath[128] = "";

void GutSetShaderPath(const char *path)
{
	strcpy(g_szShaderPath, path);
}

const char *GutGetShaderPath(void)
{
	return g_szShaderPath;
}

static std::stack<std::string> g_MessagePool;

void GutPushMessage(const char *msg)
{
	if ( msg && msg[0] )
	{
		std::string str = msg;
		g_MessagePool.push(str);
	}
}

std::string GutPopMessage(void)
{
	std::string str;

	if ( g_MessagePool.size() )
	{
		str = g_MessagePool.top();
		g_MessagePool.pop();
	}

	return str;
}

#endif
