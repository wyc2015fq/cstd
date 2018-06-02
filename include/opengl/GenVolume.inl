//生成正方体数据
//********************************************************************//
//x,y,z:正方体左下角坐标
//side:正方体边长
//density:正方体对应的标量值
//Data:体积数据
//Dim:体数据大小
//********************************************************************//
void GenCube(int x, int y, int z, int side, int density, byte* Data, int* Dim)
{
  int max_x = x + side, max_y = y + side, max_z = z + side;
  int Dimxy = Dim[0] * Dim[1];
  for (int k = z; k < max_z; k++) {
    for (int j = y; j < max_y; j++) {
      for (int i = x; i < max_x; i++) {
        Data[k * Dimxy + j * Dim[0] + i] = density;
      }
    }
  }
}
//生成球体数据
//********************************************************************//
//x,y,z:球心坐标
//radius:球半径
//density:球体对应的标量值
//Data:体积数据
//Dim:体数据大小
//********************************************************************//
void GenSphere(int x, int y, int z, int radius, int density, byte* Data, int* Dim)
{
  int radius2 = radius * radius;
  int Dimxy = Dim[0] * Dim[1];
  for (int k = 0; k < Dim[2]; k++) {
    for (int j = 0; j < Dim[1]; j++) {
      for (int i = 0; i < Dim[0]; i++) {
        if ((i - x) * (i - x) + (j - y) * (j - y) + (k - z) * (k - z) <= radius2) {
          Data[k * Dimxy + j * Dim[0] + i] = density;
        }
      }
    }
  }
}
//生成体数据
//********************************************************************//
//生成一个大正方体，内部包含一个球体，球体中间又包含一个小正方体
//Data:体积数据
//Dim:体数据大小
//********************************************************************//
void GenVolume(byte* Data, int* Dim)
{
  GenCube(0, 0, 0, 200, 100, Data, Dim); //大正方体
  GenSphere(100, 100, 100, 80, 200, Data, Dim); //球体
  GenCube(70, 70, 70, 60, 300, Data, Dim); //小正方体
}

