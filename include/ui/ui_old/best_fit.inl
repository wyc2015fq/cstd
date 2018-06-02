
#include "math1\Matrix.inl"

#define HALF_PI 1.57079632679
#define TWO_PI  6.28318530718

int LowerTriangularModifyInversion(int n, const double* l, int ll, double* m, int ml)
{
  int i, j, k;
  double sum;

  for (i = 0; i < n; ++i) {
    m[i * ml + i] = 1.0 / l[i * ll + i];
  }

  for (j = 0; j < n; ++j) {
    for (i = j + 1; i < n; ++i) {
      sum = 0.0;

      for (k = j; k <= i - 1; ++k) {
        sum += l[i * ll + k] * m[k * ml + j];
      }

      m[i * ml + j] = -m[i * ml + i] * sum;
    }
  }

  return 0;
}

bool CholeskyInversion(int n, const double* input, int input_step, double* inverse, int inverse_step)
{
  int ret = false;
  double* l, *m;
  l = MALLOC(double, n * n);
  m = MALLOC(double, n * n);
  MEMCPY2D(n, n, input, input_step, l, n);
  FILL(n * n, m, 0);

  if (0 == cholesky_decompose(n, l, n)) {
    LowerTriangularModifyInversion(n, l, n, m, n);
    matrix_mutiply_ATB(n, n, n, m, n, m, n, inverse, inverse_step);
    ret = true;
  }

  return ret;
}


// Matrix inversion routine using LU decomposition
bool InvertMatrix(int n, const double* src, int srcstep, double* dst, int dststep)
{
  cvInvert_64f(n, n, src, srcstep, dst, dststep, 0);
  return true;
}

// Determine solution vector
int EvaluateUnknowns(const double* m_qweight, const double* m_design, const double* m_l, int m_numObs, int m_numUnknowns, double* m_solution)
{
  int ret = false;
  double* pa, *atpa, *inverse;

  pa = MALLOC(double, m_numObs * m_numUnknowns);
  atpa = MALLOC(double, m_numUnknowns * m_numUnknowns);
  inverse = MALLOC(double, m_numUnknowns * m_numUnknowns);
  matrix_mutiply_AB(m_numObs, m_numUnknowns, m_numObs, m_qweight, m_numObs, m_design, m_numUnknowns, pa, m_numUnknowns);
  matrix_mutiply_ATB(m_numUnknowns, m_numUnknowns, m_numObs, m_design, m_numUnknowns, pa, m_numUnknowns, atpa, m_numUnknowns);

  //if (CholeskyInversion(atpa, inverse))
  if (InvertMatrix(m_numUnknowns, atpa, m_numUnknowns, inverse, m_numUnknowns)) {
    double* pl, *atpl;
    pl = MALLOC(double, m_numObs);
    atpl = MALLOC(double, m_numUnknowns);

    matrix_mutiply_AB(m_numObs, 1, m_numObs, m_qweight, m_numObs, m_l, 1, pl, 1);
    matrix_mutiply_ATB(m_numUnknowns, 1, m_numObs, m_design, m_numUnknowns, pl, 1, atpl, 1);
    matrix_mutiply_AB(m_numUnknowns, 1, m_numUnknowns, inverse, m_numUnknowns, atpl, 1, m_solution, 1);
    FREE(pl);
    FREE(atpl);
    ret = true;
  }
  else {
    printf("No solution. Cannot invert matrix.\n");
  }

  FREE(pa);
  FREE(atpa);
  FREE(inverse);

  return ret;
}

#define CONVERGENCE_CRITERIA 0.000000001
#define MAX_ITERATIONS 50

// Predicate for seeing whether the solution has become sufficiently small
bool HasConverged(int m_numUnknowns, const double* m_solution)
{
  int i;

  for (i = 0; i < m_numUnknowns; ++i) {
    if (fabs(m_solution[i]) > CONVERGENCE_CRITERIA) {
      return false;
    }
  }

  return true;
}

enum { FitT_Line, FitT_Circle, FitT_Ellipse };

bool BestFit(int numPoints, const DPOINT2* points, int fittype, double* outputFields, DPOINT2* outpoints, double* residuals)
{
  int i;
  bool successful = true;
  int iteration = 0;

  int m_numObs = 0, m_numUnknowns = 0;
  double m_minx = (DBL_MAX), m_maxx = (-DBL_MAX), m_miny = (DBL_MAX), m_maxy = (-DBL_MAX);

  enum {kLineUnknowns = 2, kCircleUnknowns = 3, kEllipseUnknowns = 5, MAXUnknowns = 1 + MAX3(kLineUnknowns, kCircleUnknowns, kEllipseUnknowns)};
  int unknowns[] = {kLineUnknowns, kCircleUnknowns, kEllipseUnknowns};
  double m_solution[MAXUnknowns] = {0};
  double m_provisionals[MAXUnknowns] = {0};
  //double* m_solution;
  //double* m_provisionals;
  double* m_residuals;
  double* m_design;
  double* m_l;
  double* m_qweight;
  double* m_observations;
  double* m_b;

  m_numUnknowns = unknowns[fittype];
  m_numObs = numPoints;

  if (numPoints < m_numUnknowns + 1) {
    printf("No solution. Too few input points, need %d or more.\n", m_numUnknowns + 1 - numPoints);
    return false;
  }

  //m_solution = MALLOC(double, m_numUnknowns);
  //m_provisionals = MALLOC(double, m_numUnknowns);
  m_residuals = MALLOC(double, m_numObs);
  m_design = MALLOC(double, m_numObs * m_numUnknowns);
  m_l = MALLOC(double, m_numObs);
  m_qweight = MALLOC(double, m_numObs * m_numObs);
  m_observations = MALLOC(double, m_numObs * 2);
  m_b = MALLOC(double, m_numObs * m_numObs * 2);


  FILL(m_numUnknowns, m_solution, 0);
  FILL(m_numUnknowns, m_provisionals, 0);
  FILL(m_numObs, m_residuals, 0);
  FILL(m_numObs * m_numUnknowns, m_design, 0);
  FILL(m_numObs, m_l, 0);
  FILL(m_numObs * m_numObs, m_qweight, 0);
  FILL(m_numObs * 2, m_observations, 0);
  FILL(m_numObs * m_numObs * 2, m_b, 0);


  // Add another observable (coordinate) to the computation object
  for (i = 0; i < m_numObs; ++i) {
    double x, y;
    m_observations[i * 2 + 0] = x = points[i].x;
    m_observations[i * 2 + 1] = y = points[i].y;
    m_minx = MIN(m_minx, x), m_maxx = MAX(m_maxx, x);
    m_miny = MIN(m_miny, y), m_maxy = MAX(m_maxy, y);
  }

  // Do the least-squares adjustment
  switch (fittype) {
  case FitT_Line: {
  }
  break;

  case FitT_Circle: {
  }
  break;

  case FitT_Ellipse: {
  }
  break;

  default: {
    ASSERT(0);
  }
  break;
  }

  //GenerateProvisionals();
  switch (fittype) {
  case FitT_Line: {
    double slope, intercept;
    assert(0.0 != (m_maxx - m_minx));

    if (0.0 == (m_maxx - m_minx)) {
      printf("division by zero");
      return 0;
    }

    slope = (m_maxy - m_miny) / (m_maxx - m_minx);
    intercept = m_miny - (slope * m_minx);

    m_provisionals[0] = slope;
    m_provisionals[1] = intercept;
  }
  break;

  case FitT_Circle: {
    double centrex = 0.5 * (m_maxx + m_minx);
    double centrey = 0.5 * (m_maxy + m_miny);
    double radius =  0.5 * MAX(m_maxx - m_minx, m_maxy - m_miny);

    m_provisionals[0] = centrex;
    m_provisionals[1] = centrey;
    m_provisionals[2] = radius;
  }
  break;

  case FitT_Ellipse: {
    double centrex = 0.5 * (m_maxx + m_minx);
    double centrey = 0.5 * (m_maxy + m_miny);
    double dx = m_maxx - m_minx;
    double dy = m_maxy - m_miny;
    double major = 0.5 * MAX(dx, dy);
    double minor = 0.5 * MIN(dx, dy);
    double rotation = atan(dy / dx);

    m_provisionals[0] = centrex;
    m_provisionals[1] = centrey;
    m_provisionals[2] = major;
    m_provisionals[3] = minor;
    m_provisionals[4] = rotation;
  }
  break;

  default: {
    ASSERT(0);
  }
  break;
  }

  while (true) {
    //FormulateMatrices();
    switch (fittype) {
    case FitT_Line: {
      double slope = m_provisionals[0];
      double intercept = m_provisionals[1];

      for (i = 0; i < m_numObs; i++) {
        double x = m_observations[i * 2 + 0];
        double y = m_observations[i * 2 + 1];

        m_design[i * 2 + 0] = x;
        m_design[i * 2 + 1] = 1.0;

        m_qweight[i * m_numObs + i] = 1.0; // parametric case, not quasi-parametric
        m_l[i] = y - ((x * slope) + intercept);
      }
    }
    break;

    case FitT_Circle: {
      double x0 = m_provisionals[0];
      double y0 = m_provisionals[1];
      double radius = m_provisionals[2];

      for (i = 0; i < m_numObs; i++) {
        double dx = m_observations[i * 2 + 0] - x0;
        double dy = m_observations[i * 2 + 1] - y0;
        double dxsqr = dx * dx;
        double dysqr = dy * dy;

        m_design[i * m_numUnknowns + 0] = -2.0 * dx;
        m_design[i * m_numUnknowns + 1] = -2.0 * dy;
        m_design[i * m_numUnknowns + 2] = -2.0 * radius;
        m_qweight[i * m_numObs + i] = 1.0 / (4.0 * (dxsqr + dysqr));
        m_l[i] = (radius * radius) - dxsqr - dysqr;
        m_b[i * (m_numObs * 2) + i * 2 + 0] = 2.0 * dx;
        m_b[i * (m_numObs * 2) + i * 2 + 1] = 2.0 * dy;
      }
    }
    break;

    case FitT_Ellipse: {
      double x0 = m_provisionals[0];
      double y0 = m_provisionals[1];
      double a = m_provisionals[2];
      double b = m_provisionals[3];

      double sinr = sin(m_provisionals[4]);
      double cosr = cos(m_provisionals[4]);
      double asqr = a * a;
      double bsqr = b * b;
      double x, y, d1, d2, p, q;

      for (i = 0; i < m_numObs; i++) {
        x = m_observations[i * 2 + 0];
        y = m_observations[i * 2 + 1];
        d1 = ((y - y0) * cosr - (x - x0) * sinr);
        d2 = ((x - x0) * cosr + (y - y0) * sinr);

        m_design[i * m_numUnknowns + 0] = 2.0 * ((asqr * d1 * sinr) - (bsqr * d2 * cosr));
        m_design[i * m_numUnknowns + 1] = -2.0 * ((bsqr * d2 * sinr) + (asqr * d1 * cosr));
        m_design[i * m_numUnknowns + 2] = 2.0 * a * ((d1 * d1) - bsqr);
        m_design[i * m_numUnknowns + 3] = 2.0 * b * ((d2 * d2) - asqr);
        m_design[i * m_numUnknowns + 4] = 2.0 * d1 * d2 * (bsqr - asqr);

        p = 2.0 * ((asqr * d1 * sinr) + (bsqr * d2 * cosr));
        q = -2.0 * ((bsqr * d2 * sinr) + (asqr * d1 * cosr));
        m_qweight[i * m_numObs + i] = 1.0 / (p * p + q * q);
        m_l[i] = -((asqr * d1 * d1) + (bsqr * d2 * d2) - (asqr * bsqr));
      }
    }
    break;

    default: {
      ASSERT(0);
    }
    break;
    }


    {
      printf("Provisionals:    %d", m_provisionals);
      printf("l-matrix:        %d", m_l);
    }

    // evaluate the unknowns - small corrections to be applied to the provisional unknowns
    if (EvaluateUnknowns(m_qweight, m_design, m_l, m_numObs, m_numUnknowns, m_solution)) {
      ++iteration;

      // add the solution to the provisional unknowns
      // Add solution to the provisionals
      {
        for (i = 0; i < m_numUnknowns; i++) {
          m_provisionals[i] += m_solution[i];
        }

        //printf("Iter adj unknowns %d", m_provisionals);
      }


      if (HasConverged(m_numUnknowns, m_solution)) {
        break;
      }

      // Predicate for seeing whether the solution is diverging?
      if (iteration >= MAX_ITERATIONS) {
        printf("No solution. Does not converge.\n");
        break;
      }
    }
    else {
      successful = false;
      break;
    }
  }

#define Accuracy  0.000000001
  successful = successful && (iteration > 0 && iteration < MAX_ITERATIONS);

  if (successful) {
    // Give ellipse chance to normalise major >= minor and 0 <= rotation angle < 2PI.
    //NormaliseAdjustedUnknowns()
    if (FitT_Ellipse == fittype) {
      double major, minor;
      double theta = fabs(m_provisionals[4]);
      bool negative = (m_provisionals[4]) < -Accuracy;

      while (theta >= TWO_PI) {
        theta -= TWO_PI;
      }

      if (negative) {
        theta = TWO_PI - theta;
      }

      major = m_provisionals[2];
      minor = m_provisionals[3];

      if (minor > major) {
        // swap axes and make angle the perpendicular
        m_provisionals[2] = minor;
        m_provisionals[3] = major;

        theta -= HALF_PI;

        if (theta < -Accuracy) {
          theta += TWO_PI;
        }
      }

      m_provisionals[4] = theta;
    }


    // evaluate the residuals
    // Residuals to the initial observations
    // EvaluateResiduals()

    // v = Ax-l, quasi-residuals in the case of circle and ellipse
    matrix_mutiply_AB(m_numObs, 1, m_numUnknowns, m_design, m_numUnknowns, m_solution, 1, m_residuals, m_numUnknowns);

    for (i = 0; i < m_numObs; ++i) {
      m_residuals[i] -= m_l[i];
    }

    // add the residuals to the provisional observations
    //EvaluateAdjustedObservations();

    for (i = 0; i < m_numObs; ++i) {
      double vxi = 0.0, vyi = 0.0;

      //EvaluateFinalResiduals(i, vxi, vyi); // overridden for normal (non-quasi-parametric) BestFitLine
      if (FitT_Line == fittype) {
        vxi = 0.0;
        vyi = m_residuals[i];
      }
      else {
        // In the case of the quasi-parametric case (circle and ellipse) the actual
        // residuals for both the x and y coordinate need to be extracted.
        vxi = m_design[i * m_numUnknowns + 0] * m_qweight[i * m_numObs + i] * m_residuals[i];
        vyi = m_design[i * m_numUnknowns + 1] * m_qweight[i * m_numObs + i] * m_residuals[i];
      }

      m_observations[i * 2 + 0] += vxi;
      m_observations[i * 2 + 1] += vyi;
    }


    // Check that the adjusted unknowns and adjusted observations satisfy
    // the original line/circle/ellipse equation.
    //GlobalCheck();
    {
      double* global = 0;
      bool pass = (m_numObs > 0);
      global = MALLOC(double, m_numObs);
      FILL(m_numObs, global, 0);

      for (i = 0; i < m_numObs; ++i) {
        double x = m_observations[i * 2 + 0];
        double y = m_observations[i * 2 + 1];

        //global[i] = SolveAt(x, y); // should be zero

        switch (fittype) {
        case FitT_Line: {
          double slope = m_provisionals[0];
          double intercept = m_provisionals[1];
          global[i] = y - (x * slope + intercept);
        }
        break;

        case FitT_Circle: {
          double dx = x - m_provisionals[0];
          double dy = y - m_provisionals[1];
          double rsqr = m_provisionals[2] * m_provisionals[2];
          global[i] = rsqr - dx * dx - dy * dy;
        }
        break;

        case FitT_Ellipse: {
          double x0 = m_provisionals[0];
          double y0 = m_provisionals[1];
          double asqr = m_provisionals[2] * m_provisionals[2];
          double bsqr = m_provisionals[3] * m_provisionals[3];
          double sinr = sin(m_provisionals[4]);
          double cosr = cos(m_provisionals[4]);
          double dx = x - x0;
          double dy = y - y0;
          double d1 = (dx * cosr + dy * sinr);
          double d2 = (-dx * sinr + dy * cosr);
          global[i] = ((d1 * d1) / asqr) + ((d2 * d2) / bsqr) - 1.0;
        }
        break;

        default: {
          ASSERT(0);
        }
        break;
        }

        pass = pass && fabs(global[i]) < 0.01; // TODO: Is this too lax?
      }

      FREE(global);

      {
        int j;
        // Secondary test is to check that aTPv is zero too, really only neccessary
        // if the above global check fails.
        double* atp, *atpv;
        atp = MALLOC(double, m_numUnknowns * m_numObs);
        atpv = MALLOC(double, m_numUnknowns);
        matrix_mutiply_ATB(m_numUnknowns, m_numObs, m_numObs, m_design, m_numUnknowns, m_qweight, m_numObs, atp, m_numObs);
        matrix_mutiply_AB(m_numUnknowns, 1, m_numObs, atp, m_numObs, m_residuals, 1, atpv, 1);

        pass = true;

        for (j = 0; j < m_numUnknowns; ++j) {
          pass = pass && (fabs(atpv[j]) <= Accuracy);
        }

        FREE(atp);
        FREE(atpv);
      }
    }



    // Output stats
    // TODO: Variance-covariance matrices
    // Subsequent error analysis and statistical output
    //ErrorAnalysis(iteration);
    {
      double* pv = 0, vtpv = 0, variance, stddev;
      int degreesFreedom;

      pv = MALLOC(double, m_numObs);

      matrix_mutiply_AB(m_numObs, 1, m_numObs, m_qweight, m_numObs, m_residuals, 1, pv, 1);
      vtpv = d_dot(m_numObs, m_residuals, pv);
      degreesFreedom = m_numObs - m_numUnknowns;
      variance = vtpv / degreesFreedom;
      stddev = sqrt(variance);
      FREE(pv);

      if (0) {
        printf("Number of observations             %d\n", m_numObs);
        printf("Number of unknowns                 %d\n", m_numUnknowns);
        printf("Degrees of freedom                 %d\n", degreesFreedom);
        printf("Iterations until convergence       %d\n", iteration);
        printf("Variance                           %d\n", variance);
        printf("Std. dev. observation unit weight  %d\n", stddev);

        //OutputAdjustedUnknowns(m_oStream);

        switch (fittype) {
        case FitT_Line: {
          printf("Adj. line gradient                 %8.4f", m_provisionals[0]);
          printf("Adj. line Y-intercept              %8.4f", m_provisionals[1]);
        }
        break;

        case FitT_Circle: {
          printf("Adj. circle centre X               %8.4f", m_provisionals[0]);
          printf("Adj. circle centre Y               %8.4f", m_provisionals[1]);
          printf("Adj. circle radius                 %8.4f", m_provisionals[2]);
        }
        break;

        case FitT_Ellipse: {
          printf("Adj. ellipse centre X              %8.4f", m_provisionals[0]);
          printf("Adj. ellipse centre Y              %8.4f", m_provisionals[1]);
          printf("Adj. ellipse semi-major axis       %8.4f", m_provisionals[2]);
          printf("Adj. ellipse semi-minor axis       %8.4f", m_provisionals[3]);
          printf("Adj. ellipse rotation              %8.4f", m_provisionals[4]);
        }
        break;

        default: {
          ASSERT(0);
        }
        break;
        }

      }
    }

    if (0) {
      // Output at it's very simplest
      //OutputSimpleSolution();
      for (i = 0; i < m_numUnknowns; ++i) {
        printf("%8.4f ", m_provisionals[i]);
      }
    }
  }

  // Return selected values back to caller
  //FillOutput(out);

  //memset(outputFields, 0, sizeof(outputFields));
  if (outputFields) {
    for (i = 0; i < m_numUnknowns; ++i) {
      outputFields[i] = m_provisionals[i];
    }
  }

  if (outpoints) {
    for (i = 0; i < m_numObs; ++i) {
      outpoints[i].x = m_observations[i * 2 + 0];
      outpoints[i].y = m_observations[i * 2 + 1];
    }
  }

  if (residuals) {
    for (i = 0; i < m_numObs; ++i) {
      residuals[i] = m_residuals[i]; // ²Ð²î
    }
  }

  //FREE(m_solution);
  //FREE(m_provisionals);
  FREE(m_residuals);
  FREE(m_design);
  FREE(m_l);
  FREE(m_qweight);
  FREE(m_observations);
  FREE(m_b);
  return successful;
}

