
#include <vector>
class epnp {
 public:
  epnp(const img_t* cameraMatrix, const img_t* opoints, const img_t* ipoints);
  ~epnp();

  void add_correspondence(const double X, const double Y, const double Z,
              const double u, const double v);

  void compute_pose(img_t* R, img_t* t);
 private:
  template <typename T>
  void init_camera_parameters(const T* cameraMatrix, int step)
  {
    uc = cameraMatrix[step*0+ 2];
    vc = cameraMatrix[step*1+ 2];
    fu = cameraMatrix[step*0+ 0];
    fv = cameraMatrix[step*1+ 1];
  }
  template <typename OpointType, typename IpointType>
  void init_points(const OpointType* opoints, const IpointType* ipoints)
  {
      for(int i = 0; i < number_of_correspondences; i++)
      {
          pws[3 * i    ] = opoints[i].x;
          pws[3 * i + 1] = opoints[i].y;
          pws[3 * i + 2] = opoints[i].z;

          us[2 * i    ] = ipoints[i].x*fu + uc;
          us[2 * i + 1] = ipoints[i].y*fv + vc;
      }
  }
  double reprojection_error(const double R[3][3], const double t[3]);
  void choose_control_points(void);
  void compute_barycentric_coordinates(void);
  void fill_M(img_t * M, const int row, const double * alphas, const double u, const double v);
  void compute_ccs(const double * betas, const double * ut);
  void compute_pcs(void);

  void solve_for_sign(void);

  void find_betas_approx_1(const img_t * L_6x10, const img_t * Rho, double * betas);
  void find_betas_approx_2(const img_t * L_6x10, const img_t * Rho, double * betas);
  void find_betas_approx_3(const img_t * L_6x10, const img_t * Rho, double * betas);
  void qr_solve(img_t * A, img_t * b, img_t * X);

  double dot(const double * v1, const double * v2);
  double dist2(const double * p1, const double * p2);

  void compute_rho(double * rho);
  void compute_L_6x10(const double * ut, double * l_6x10);

  void gauss_newton(const img_t * L_6x10, const img_t * Rho, double current_betas[4]);
  void compute_A_and_b_gauss_newton(const double * l_6x10, const double * rho,
                    const double cb[4], img_t * A, img_t * b);

  double compute_R_and_t(const double * ut, const double * betas,
             double R[3][3], double t[3]);

  void estimate_R_and_t(double R[3][3], double t[3]);

  void copy_R_and_t(const double R_dst[3][3], const double t_dst[3],
            double R_src[3][3], double t_src[3]);


  double uc, vc, fu, fv;

  std::vector<double> pws, us, alphas, pcs;
  int number_of_correspondences;

  double cws[4][3], ccs[4][3];
  int max_nr;
  double * A1, * A2;
};

epnp::epnp(const img_t* cameraMatrix, const img_t* opoints, const img_t* ipoints)
{
  if (CC_MAT_TYPE(cameraMatrix) == CC_32F)
      init_camera_parameters(cameraMatrix->tt.fl, cameraMatrix->step/sizeof(float));
  else
    init_camera_parameters(cameraMatrix->tt.fl, cameraMatrix->step/sizeof(double));

  number_of_correspondences = MAX(cvCheckVector(opoints, 3, CC_32F), cvCheckVector(opoints, 3, CC_64F));

  pws.resize(3 * number_of_correspondences);
  us.resize(2 * number_of_correspondences);

  if (CC_MAT_TYPE(opoints) == CC_MAT_TYPE(ipoints))
  {
    if (CC_MAT_TYPE(opoints) == CC_32F)
      init_points((CPoint3f*)opoints->tt.data, (CPoint2f*)ipoints->tt.data);
    else
      init_points((CPoint3d*)opoints->tt.data, (CPoint2d*)ipoints->tt.data);
  }
  else if (CC_MAT_TYPE(opoints) == CC_32F)
    init_points((CPoint3f*)opoints->tt.data, (CPoint2d*)ipoints->tt.data);
  else
    init_points((CPoint3d*)opoints->tt.data, (CPoint2f*)ipoints->tt.data);

  alphas.resize(4 * number_of_correspondences);
  pcs.resize(3 * number_of_correspondences);

  max_nr = 0;
  A1 = NULL;
  A2 = NULL;
}

epnp::~epnp()
{
    if (A1)
        delete[] A1;
    if (A2)
        delete[] A2;
}

void epnp::choose_control_points(void)
{
  int i, j;
  // Take C0 as the reference points centroid:
  cws[0][0] = cws[0][1] = cws[0][2] = 0;
  for(i = 0; i < number_of_correspondences; i++)
    for(j = 0; j < 3; j++)
      cws[0][j] += pws[3 * i + j];

  for(j = 0; j < 3; j++)
    cws[0][j] /= number_of_correspondences;


  // Take C1, C2, and C3 from PCA on the reference points:
  img_t PW0[1] = {0};
  cvSetMat(PW0, number_of_correspondences, 3, CC_64F);

  img_t tmp[3] = {0};
  double pw0tpw0[3 * 3], dc[3], uct[3 * 3];
  img_t* PW0tPW0 = cvMat(tmp+0, 3, 3, CC_64F, 1, pw0tpw0);
  img_t* DC      = cvMat(tmp+1, 3, 1, CC_64F, 1, dc);
  img_t* UCt     = cvMat(tmp+2, 3, 3, CC_64F, 1, uct);

  for(i = 0; i < number_of_correspondences; i++)
    for(j = 0; j < 3; j++)
      PW0->tt.db[3 * i + j] = pws[3 * i + j] - cws[0][j];

  cvMulTransposed(PW0, PW0tPW0, 1);
  cvSVD(PW0tPW0, DC, UCt, 0, CC_SVD_MODIFY_A | CC_SVD_U_T);

  imfree(PW0);

  for(i = 1; i < 4; i++) {
    double k = sqrt(dc[i - 1] / number_of_correspondences);
    for(j = 0; j < 3; j++)
      cws[i][j] = cws[0][j] + k * uct[3 * (i - 1) + j];
  }
}

void epnp::compute_barycentric_coordinates(void)
{
  double cc[3 * 3], cc_inv[3 * 3];
  img_t tmp[3] = {0};
  img_t* CC     = cvMat(tmp+0, 3, 3, CC_64F, 1, cc);
  img_t* CC_inv = cvMat(tmp+1, 3, 3, CC_64F, 1, cc_inv);
  int i, j;

  for(i = 0; i < 3; i++)
    for(j = 1; j < 4; j++)
      cc[3 * i + j - 1] = cws[j][i] - cws[0][i];

  cvInvert(CC, CC_inv, CC_SVD);
  double * ci = cc_inv;
  for(i = 0; i < number_of_correspondences; i++) {
    double * pi = &pws[0] + 3 * i;
    double * a = &alphas[0] + 4 * i;

    for(j = 0; j < 3; j++)
      a[1 + j] = ci[3 * j    ] * (pi[0] - cws[0][0]) +
      ci[3 * j + 1] * (pi[1] - cws[0][1]) +
      ci[3 * j + 2] * (pi[2] - cws[0][2]);
    a[0] = 1.0f - a[1] - a[2] - a[3];
  }
}

void epnp::fill_M(img_t * M,
      const int row, const double * as, const double u, const double v)
{
  double * M1 = M->tt.db + row * 12;
  double * M2 = M1 + 12;

  for(int i = 0; i < 4; i++) {
    M1[3 * i    ] = as[i] * fu;
    M1[3 * i + 1] = 0.0;
    M1[3 * i + 2] = as[i] * (uc - u);

    M2[3 * i    ] = 0.0;
    M2[3 * i + 1] = as[i] * fv;
    M2[3 * i + 2] = as[i] * (vc - v);
  }
}

void epnp::compute_ccs(const double * betas, const double * ut)
{
  int i, j, k;
  for(i = 0; i < 4; i++)
    ccs[i][0] = ccs[i][1] = ccs[i][2] = 0.0f;

  for(i = 0; i < 4; i++) {
    const double * v = ut + 12 * (11 - i);
    for(j = 0; j < 4; j++)
      for(k = 0; k < 3; k++)
  ccs[j][k] += betas[i] * v[3 * j + k];
  }
}

void epnp::compute_pcs(void)
{
  for(int i = 0; i < number_of_correspondences; i++) {
    double * a = &alphas[0] + 4 * i;
    double * pc = &pcs[0] + 3 * i;

    for(int j = 0; j < 3; j++)
      pc[j] = a[0] * ccs[0][j] + a[1] * ccs[1][j] + a[2] * ccs[2][j] + a[3] * ccs[3][j];
  }
}

void epnp::compute_pose(img_t* R, img_t* t)
{
  choose_control_points();
  compute_barycentric_coordinates();

  img_t M[1] = {0};
  cvSetMat(M, 2 * number_of_correspondences, 12, CC_64F);
  int i;

  for(i = 0; i < number_of_correspondences; i++)
    fill_M(M, 2 * i, &alphas[0] + 4 * i, us[2 * i], us[2 * i + 1]);

  double mtm[12 * 12], d[12], ut[12 * 12];
  img_t tmp[5] = {0};
  img_t *MtM = cvMat(tmp+0, 12, 12, CC_64F, 1, mtm);
  img_t *D   = cvMat(tmp+1, 12,  1, CC_64F, 1, d);
  img_t *Ut  = cvMat(tmp+2, 12, 12, CC_64F, 1, ut);

  cvMulTransposed(M, MtM, 1);
  cvSVD(MtM, D, Ut, 0, CC_SVD_MODIFY_A | CC_SVD_U_T);
  imfree(M);

  double l_6x10[6 * 10], rho[6];
  img_t *L_6x10 = cvMat(tmp+3, 6, 10, CC_64F, 1, l_6x10);
  img_t *Rho    = cvMat(tmp+4, 6,  1, CC_64F, 1, rho);

  compute_L_6x10(ut, l_6x10);
  compute_rho(rho);

  double Betas[4][4], rep_errors[4];
  double Rs[4][3][3], ts[4][3];

  find_betas_approx_1(L_6x10, Rho, Betas[1]);
  gauss_newton(L_6x10, Rho, Betas[1]);
  rep_errors[1] = compute_R_and_t(ut, Betas[1], Rs[1], ts[1]);

  find_betas_approx_2(L_6x10, Rho, Betas[2]);
  gauss_newton(L_6x10, Rho, Betas[2]);
  rep_errors[2] = compute_R_and_t(ut, Betas[2], Rs[2], ts[2]);

  find_betas_approx_3(L_6x10, Rho, Betas[3]);
  gauss_newton(L_6x10, Rho, Betas[3]);
  rep_errors[3] = compute_R_and_t(ut, Betas[3], Rs[3], ts[3]);

  int N = 1;
  if (rep_errors[2] < rep_errors[1]) N = 2;
  if (rep_errors[3] < rep_errors[N]) N = 3;

  cvMat(tmp, 3, 1, CC_64F, 1, ts[N]);
  cvCopy(tmp, t);
  cvMat(tmp, 3, 3, CC_64F, 1, Rs[N]);
  cvCopy(tmp, R);
}

void epnp::copy_R_and_t(const double R_src[3][3], const double t_src[3],
      double R_dst[3][3], double t_dst[3])
{
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++)
      R_dst[i][j] = R_src[i][j];
    t_dst[i] = t_src[i];
  }
}

double epnp::dist2(const double * p1, const double * p2)
{
  return
    (p1[0] - p2[0]) * (p1[0] - p2[0]) +
    (p1[1] - p2[1]) * (p1[1] - p2[1]) +
    (p1[2] - p2[2]) * (p1[2] - p2[2]);
}

double epnp::dot(const double * v1, const double * v2)
{
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void epnp::estimate_R_and_t(double R[3][3], double t[3])
{
  int i, j;
  double pc0[3], pw0[3];

  pc0[0] = pc0[1] = pc0[2] = 0.0;
  pw0[0] = pw0[1] = pw0[2] = 0.0;

  for(i = 0; i < number_of_correspondences; i++) {
    const double * pc = &pcs[3 * i];
    const double * pw = &pws[3 * i];

    for(j = 0; j < 3; j++) {
      pc0[j] += pc[j];
      pw0[j] += pw[j];
    }
  }
  for(j = 0; j < 3; j++) {
    pc0[j] /= number_of_correspondences;
    pw0[j] /= number_of_correspondences;
  }

  double abt[3 * 3], abt_d[3], abt_u[3 * 3], abt_v[3 * 3];
  img_t tmp[10];
  img_t *ABt   = cvMat(tmp+0, 3, 3, CC_64F, 1, abt);
  img_t *ABt_D = cvMat(tmp+1, 3, 1, CC_64F, 1, abt_d);
  img_t *ABt_U = cvMat(tmp+2, 3, 3, CC_64F, 1, abt_u);
  img_t *ABt_V = cvMat(tmp+3, 3, 3, CC_64F, 1, abt_v);

  cvZeroMat(ABt);
  for(i = 0; i < number_of_correspondences; i++) {
    double * pc = &pcs[3 * i];
    double * pw = &pws[3 * i];

    for(j = 0; j < 3; j++) {
      abt[3 * j    ] += (pc[j] - pc0[j]) * (pw[0] - pw0[0]);
      abt[3 * j + 1] += (pc[j] - pc0[j]) * (pw[1] - pw0[1]);
      abt[3 * j + 2] += (pc[j] - pc0[j]) * (pw[2] - pw0[2]);
    }
  }

  cvSVD(ABt, ABt_D, ABt_U, ABt_V, CC_SVD_MODIFY_A);

  for(i = 0; i < 3; i++)
    for(j = 0; j < 3; j++)
      R[i][j] = dot(abt_u + 3 * i, abt_v + 3 * j);

  const double det =
    R[0][0] * R[1][1] * R[2][2] + R[0][1] * R[1][2] * R[2][0] + R[0][2] * R[1][0] * R[2][1] -
    R[0][2] * R[1][1] * R[2][0] - R[0][1] * R[1][0] * R[2][2] - R[0][0] * R[1][2] * R[2][1];

  if (det < 0) {
    R[2][0] = -R[2][0];
    R[2][1] = -R[2][1];
    R[2][2] = -R[2][2];
  }

  t[0] = pc0[0] - dot(R[0], pw0);
  t[1] = pc0[1] - dot(R[1], pw0);
  t[2] = pc0[2] - dot(R[2], pw0);
}

void epnp::solve_for_sign(void)
{
	int i, j;
  if (pcs[2] < 0.0) {
    for(i = 0; i < 4; i++)
      for(j = 0; j < 3; j++)
  ccs[i][j] = -ccs[i][j];

    for(i = 0; i < number_of_correspondences; i++) {
      pcs[3 * i    ] = -pcs[3 * i];
      pcs[3 * i + 1] = -pcs[3 * i + 1];
      pcs[3 * i + 2] = -pcs[3 * i + 2];
    }
  }
}

double epnp::compute_R_and_t(const double * ut, const double * betas,
           double R[3][3], double t[3])
{
  compute_ccs(betas, ut);
  compute_pcs();

  solve_for_sign();

  estimate_R_and_t(R, t);

  return reprojection_error(R, t);
}

double epnp::reprojection_error(const double R[3][3], const double t[3])
{
  double sum2 = 0.0;

  for(int i = 0; i < number_of_correspondences; i++) {
    double * pw = &pws[3 * i];
    double Xc = dot(R[0], pw) + t[0];
    double Yc = dot(R[1], pw) + t[1];
    double inv_Zc = 1.0 / (dot(R[2], pw) + t[2]);
    double ue = uc + fu * Xc * inv_Zc;
    double ve = vc + fv * Yc * inv_Zc;
    double u = us[2 * i], v = us[2 * i + 1];

    sum2 += sqrt( (u - ue) * (u - ue) + (v - ve) * (v - ve) );
  }

  return sum2 / number_of_correspondences;
}

// betas10        = [B11 B12 B22 B13 B23 B33 B14 B24 B34 B44]
// betas_approx_1 = [B11 B12     B13         B14]

void epnp::find_betas_approx_1(const img_t * L_6x10, const img_t * Rho,
             double * betas)
{
  int i;
  double l_6x4[6 * 4], b4[4];
  img_t tmp[10] = {0};
  img_t *L_6x4 = cvMat(tmp+0, 6, 4, CC_64F, 1, l_6x4);
  img_t *B4    = cvMat(tmp+1, 4, 1, CC_64F, 1, b4);

  for(i = 0; i < 6; i++) {
    cvmSet(L_6x4, i, 0, cvmGet(L_6x10, i, 0));
    cvmSet(L_6x4, i, 1, cvmGet(L_6x10, i, 1));
    cvmSet(L_6x4, i, 2, cvmGet(L_6x10, i, 3));
    cvmSet(L_6x4, i, 3, cvmGet(L_6x10, i, 6));
  }

  cvSolve(L_6x4, Rho, B4, CC_SVD);

  if (b4[0] < 0) {
    betas[0] = sqrt(-b4[0]);
    betas[1] = -b4[1] / betas[0];
    betas[2] = -b4[2] / betas[0];
    betas[3] = -b4[3] / betas[0];
  } else {
    betas[0] = sqrt(b4[0]);
    betas[1] = b4[1] / betas[0];
    betas[2] = b4[2] / betas[0];
    betas[3] = b4[3] / betas[0];
  }
}

// betas10        = [B11 B12 B22 B13 B23 B33 B14 B24 B34 B44]
// betas_approx_2 = [B11 B12 B22                            ]

void epnp::find_betas_approx_2(const img_t * L_6x10, const img_t * Rho,
             double * betas)
{
  double l_6x3[6 * 3], b3[3];
  int i;
  img_t tmp[10] = {0};
  img_t *L_6x3  = cvMat(tmp+0, 6, 3, CC_64F, 1, l_6x3);
  img_t *B3     = cvMat(tmp+1, 3, 1, CC_64F, 1, b3);

  for(i = 0; i < 6; i++) {
    cvmSet(L_6x3, i, 0, cvmGet(L_6x10, i, 0));
    cvmSet(L_6x3, i, 1, cvmGet(L_6x10, i, 1));
    cvmSet(L_6x3, i, 2, cvmGet(L_6x10, i, 2));
  }

  cvSolve(L_6x3, Rho, B3, CC_SVD);

  if (b3[0] < 0) {
    betas[0] = sqrt(-b3[0]);
    betas[1] = (b3[2] < 0) ? sqrt(-b3[2]) : 0.0;
  } else {
    betas[0] = sqrt(b3[0]);
    betas[1] = (b3[2] > 0) ? sqrt(b3[2]) : 0.0;
  }

  if (b3[1] < 0) betas[0] = -betas[0];

  betas[2] = 0.0;
  betas[3] = 0.0;
}

// betas10        = [B11 B12 B22 B13 B23 B33 B14 B24 B34 B44]
// betas_approx_3 = [B11 B12 B22 B13 B23                    ]

void epnp::find_betas_approx_3(const img_t * L_6x10, const img_t * Rho,
             double * betas)
{
  double l_6x5[6 * 5], b5[5];
  int i;
  img_t tmp[10] = {0};
  img_t *L_6x5 = cvMat(tmp+0, 6, 5, CC_64F, 1, l_6x5);
  img_t *B5    = cvMat(tmp+1, 5, 1, CC_64F, 1, b5);

  for(i = 0; i < 6; i++) {
    cvmSet(L_6x5, i, 0, cvmGet(L_6x10, i, 0));
    cvmSet(L_6x5, i, 1, cvmGet(L_6x10, i, 1));
    cvmSet(L_6x5, i, 2, cvmGet(L_6x10, i, 2));
    cvmSet(L_6x5, i, 3, cvmGet(L_6x10, i, 3));
    cvmSet(L_6x5, i, 4, cvmGet(L_6x10, i, 4));
  }

  cvSolve(L_6x5, Rho, B5, CC_SVD);

  if (b5[0] < 0) {
    betas[0] = sqrt(-b5[0]);
    betas[1] = (b5[2] < 0) ? sqrt(-b5[2]) : 0.0;
  } else {
    betas[0] = sqrt(b5[0]);
    betas[1] = (b5[2] > 0) ? sqrt(b5[2]) : 0.0;
  }
  if (b5[1] < 0) betas[0] = -betas[0];
  betas[2] = b5[3] / betas[0];
  betas[3] = 0.0;
}

void epnp::compute_L_6x10(const double * ut, double * l_6x10)
{
  int i, j;
  const double * v[4];

  v[0] = ut + 12 * 11;
  v[1] = ut + 12 * 10;
  v[2] = ut + 12 *  9;
  v[3] = ut + 12 *  8;

  double dv[4][6][3];

  for(i = 0; i < 4; i++) {
    int a = 0, b = 1;
    for(j = 0; j < 6; j++) {
      dv[i][j][0] = v[i][3 * a    ] - v[i][3 * b];
      dv[i][j][1] = v[i][3 * a + 1] - v[i][3 * b + 1];
      dv[i][j][2] = v[i][3 * a + 2] - v[i][3 * b + 2];

      b++;
      if (b > 3) {
  a++;
  b = a + 1;
      }
    }
  }

  for(i = 0; i < 6; i++) {
    double * row = l_6x10 + 10 * i;

    row[0] =        dot(dv[0][i], dv[0][i]);
    row[1] = 2.0f * dot(dv[0][i], dv[1][i]);
    row[2] =        dot(dv[1][i], dv[1][i]);
    row[3] = 2.0f * dot(dv[0][i], dv[2][i]);
    row[4] = 2.0f * dot(dv[1][i], dv[2][i]);
    row[5] =        dot(dv[2][i], dv[2][i]);
    row[6] = 2.0f * dot(dv[0][i], dv[3][i]);
    row[7] = 2.0f * dot(dv[1][i], dv[3][i]);
    row[8] = 2.0f * dot(dv[2][i], dv[3][i]);
    row[9] =        dot(dv[3][i], dv[3][i]);
  }
}

void epnp::compute_rho(double * rho)
{
  rho[0] = dist2(cws[0], cws[1]);
  rho[1] = dist2(cws[0], cws[2]);
  rho[2] = dist2(cws[0], cws[3]);
  rho[3] = dist2(cws[1], cws[2]);
  rho[4] = dist2(cws[1], cws[3]);
  rho[5] = dist2(cws[2], cws[3]);
}

void epnp::compute_A_and_b_gauss_newton(const double * l_6x10, const double * rho,
          const double betas[4], img_t * A, img_t * b)
{
  for(int i = 0; i < 6; i++) {
    const double * rowL = l_6x10 + i * 10;
    double * rowA = A->tt.db + i * 4;

    rowA[0] = 2 * rowL[0] * betas[0] +     rowL[1] * betas[1] +     rowL[3] * betas[2] +     rowL[6] * betas[3];
    rowA[1] =     rowL[1] * betas[0] + 2 * rowL[2] * betas[1] +     rowL[4] * betas[2] +     rowL[7] * betas[3];
    rowA[2] =     rowL[3] * betas[0] +     rowL[4] * betas[1] + 2 * rowL[5] * betas[2] +     rowL[8] * betas[3];
    rowA[3] =     rowL[6] * betas[0] +     rowL[7] * betas[1] +     rowL[8] * betas[2] + 2 * rowL[9] * betas[3];

    cvmSet(b, i, 0, rho[i] -
     (
      rowL[0] * betas[0] * betas[0] +
      rowL[1] * betas[0] * betas[1] +
      rowL[2] * betas[1] * betas[1] +
      rowL[3] * betas[0] * betas[2] +
      rowL[4] * betas[1] * betas[2] +
      rowL[5] * betas[2] * betas[2] +
      rowL[6] * betas[0] * betas[3] +
      rowL[7] * betas[1] * betas[3] +
      rowL[8] * betas[2] * betas[3] +
      rowL[9] * betas[3] * betas[3]
      ));
  }
}

void epnp::gauss_newton(const img_t * L_6x10, const img_t * Rho, double betas[4])
{
  const int iterations_number = 5;

  double a[6*4], b[6], x[4];
  img_t tmp[4] = {0};
  img_t *A = cvMat(tmp+0, 6, 4, CC_64F, 1, a);
  img_t *B = cvMat(tmp+1, 6, 1, CC_64F, 1, b);
  img_t *X = cvMat(tmp+2, 4, 1, CC_64F, 1, x);

  for(int k = 0; k < iterations_number; k++)
  {
    compute_A_and_b_gauss_newton(L_6x10->tt.db, Rho->tt.db, betas, A, B);
    qr_solve(A, B, X);
    for(int i = 0; i < 4; i++)
    betas[i] += x[i];
  }
}

void epnp::qr_solve(img_t * A, img_t * b, img_t * X)
{
  const int nr = A->rows;
  const int nc = A->cols;
  int i, j, k;

  if (max_nr != 0 && max_nr < nr)
  {
    delete [] A1;
    delete [] A2;
  }
  if (max_nr < nr)
  {
    max_nr = nr;
    A1 = new double[nr];
    A2 = new double[nr];
  }

  double * pA = A->tt.db, * ppAkk = pA;
  for(k = 0; k < nc; k++)
  {
    double * ppAik1 = ppAkk, eta = fabs(*ppAik1);
    for(i = k + 1; i < nr; i++)
    {
      double elt = fabs(*ppAik1);
      if (eta < elt) eta = elt;
      ppAik1 += nc;
    }
    if (eta == 0)
    {
      A1[k] = A2[k] = 0.0;
      //cerr << "God damnit, A is singular, this shouldn't happen." << endl;
      return;
    }
    else
    {
      double * ppAik2 = ppAkk, sum2 = 0.0, inv_eta = 1. / eta;
      for(i = k; i < nr; i++)
      {
        *ppAik2 *= inv_eta;
        sum2 += *ppAik2 * *ppAik2;
        ppAik2 += nc;
      }
      double sigma = sqrt(sum2);
      if (*ppAkk < 0)
      sigma = -sigma;
      *ppAkk += sigma;
      A1[k] = sigma * *ppAkk;
      A2[k] = -eta * sigma;
      for(j = k + 1; j < nc; j++)
      {
        double * ppAik = ppAkk, sum = 0;
        for(i = k; i < nr; i++)
        {
          sum += *ppAik * ppAik[j - k];
          ppAik += nc;
        }
        double tau = sum / A1[k];
        ppAik = ppAkk;
        for(i = k; i < nr; i++)
        {
          ppAik[j - k] -= tau * *ppAik;
          ppAik += nc;
        }
      }
    }
    ppAkk += nc + 1;
  }

  // b <- Qt b
  double * ppAjj = pA, * pb = b->tt.db;
  for(j = 0; j < nc; j++)
  {
    double * ppAij = ppAjj, tau = 0;
    for(i = j; i < nr; i++)
    {
      tau += *ppAij * pb[i];
      ppAij += nc;
    }
    tau /= A1[j];
    ppAij = ppAjj;
    for(i = j; i < nr; i++)
    {
      pb[i] -= tau * *ppAij;
      ppAij += nc;
    }
    ppAjj += nc + 1;
  }

  // X = R-1 b
  double * pX = X->tt.db;
  pX[nc - 1] = pb[nc - 1] / A2[nc - 1];
  for(i = nc - 2; i >= 0; i--)
  {
    double * ppAij = pA + i * nc + (i + 1), sum = 0;

    for(j = i + 1; j < nc; j++)
    {
      sum += *ppAij * pX[j];
      ppAij++;
    }
    pX[i] = (pb[i] - sum) / A2[i];
  }
}



int RANSACUpdateNumIters( double p, double ep, int modelPoints, int maxIters );

struct Algorithm{};

class CC_EXPORTS LMSolver : public Algorithm
{
public:
    class CC_EXPORTS Callback
    {
    public:
        virtual ~Callback() {}
        virtual bool compute(const img_t* param, img_t* err, img_t* J) const = 0;
    };

    virtual void setCallback(const LMSolver::Callback* cb) = 0;
    virtual int run(const img_t* param0) const = 0;
};

CC_EXPORTS LMSolver* createLMSolver(const LMSolver::Callback* cb, int maxIters);

class CC_EXPORTS PointSetRegistrator : public Algorithm
{
public:
    class CC_EXPORTS Callback
    {
    public:
        virtual ~Callback() {}
        virtual int runKernel(const img_t* m1, const img_t* m2, img_t* model) const = 0;
        virtual void computeError(const img_t* m1, const img_t* m2, const img_t* model, img_t* err) const = 0;
        virtual bool checkSubset(const img_t*, const img_t*, int) const { return true; }
    };

    virtual void setCallback(const PointSetRegistrator::Callback* cb) = 0;
    virtual bool run(const img_t* m1, const img_t* m2, img_t* model, img_t* mask) const = 0;
};

CC_EXPORTS PointSetRegistrator* createRANSACPointSetRegistrator(const PointSetRegistrator::Callback* cb,
                                                                    int modelPoints, double threshold,
                                                                    double confidence=0.99, int maxIters=1000 );

CC_EXPORTS PointSetRegistrator* createLMeDSPointSetRegistrator(const PointSetRegistrator::Callback* cb,
                                                                   int modelPoints, double confidence=0.99, int maxIters=1000 );

template<typename T> inline int compressElems( T* ptr, const uchar* mask, int mstep, int count )
{
    int i, j;
    for( i = j = 0; i < count; i++ )
        if( mask[i*mstep] )
        {
            if( i > j )
                ptr[j] = ptr[i];
            j++;
        }
    return j;
}

static inline bool haveCollinearPoints( const img_t* m, int count )
{
    int j, k, i = count-1;
    const CPoint2f* ptr = (CPoint2f*)m->tt.data;
    ASSERT(CC_32F==m->tid);

    // check that the i-th selected point does not belong
    // to a line connecting some previously selected points
    // also checks that points are not too close to each other
    for( j = 0; j < i; j++ )
    {
        double dx1 = ptr[j].x - ptr[i].x;
        double dy1 = ptr[j].y - ptr[i].y;
        for( k = 0; k < j; k++ )
        {
            double dx2 = ptr[k].x - ptr[i].x;
            double dy2 = ptr[k].y - ptr[i].y;
            if( fabs(dx2*dy1 - dy2*dx1) <= FLT_EPSILON*(fabs(dx1) + fabs(dy1) + fabs(dx2) + fabs(dy2)))
                return true;
        }
    }
    return false;
}

int checkChessboard(const img_t* img, const CSize & size);
int checkChessboardBinary(const img_t* img, const CSize & size);

