
class ClipperOffset
{
public:
  ClipperOffset(double miterLimit = 2.0, double roundPrecision = 0.25);
  ~ClipperOffset();
  void AddPath(const Path& path, JoinType joinType, EndType endType);
  void AddPaths(const Paths& paths, JoinType joinType, EndType endType);
  void Execute(Paths& solution, double delta);
  void Execute(PolyTree& solution, double delta);
  void Clear();
  double MiterLimit;
  double ArcTolerance;
private:
  Paths m_destPolys;
  Path m_srcPoly;
  Path m_destPoly;
  std::vector<DoublePoint> m_normals;
  double m_delta, m_sinA, m_sin, m_cos;
  double m_miterLim, m_StepsPerRad;
  IntPoint m_lowest;
  PolyNode m_polyNodes;

  void FixOrientations();
  void DoOffset(double delta);
  void OffsetPoint(int j, int& k, JoinType jointype);
  void DoSquare(int j, int k);
  void DoMiter(int j, int k, double r);
  void DoRound(int j, int k);
};

// ClipperOffset class
ClipperOffset::ClipperOffset(double miterLimit, double arcTolerance)
{
  this->MiterLimit = miterLimit;
  this->ArcTolerance = arcTolerance;
  m_lowest.X = -1;
}


ClipperOffset::~ClipperOffset()
{
  Clear();
}


void ClipperOffset::Clear()
{
  for (int i = 0; i < m_polyNodes.ChildCount(); ++i) {
    delete m_polyNodes.Childs[i];
  }

  m_polyNodes.Childs.clear();
  m_lowest.X = -1;
}


void ClipperOffset::AddPath(const Path& path, JoinType joinType, EndType endType)
{
  int highI = (int)path.size() - 1;

  if (highI < 0) {
    return;
  }

  PolyNode* newNode = new PolyNode();
  newNode->m_jointype = joinType;
  newNode->m_endtype = endType;

  //strip duplicate points from path and also get index to the lowest point ...
  if (endType == etClosedLine || endType == etClosedPolygon)
    while (highI > 0 && path[0] == path[highI]) {
      highI--;
    }

  newNode->Contour.reserve(highI + 1);
  newNode->Contour.push_back(path[0]);
  int j = 0, k = 0;

  for (int i = 1; i <= highI; i++)
    if (newNode->Contour[j] != path[i]) {
      j++;
      newNode->Contour.push_back(path[i]);

      if (path[i].Y > newNode->Contour[k].Y ||
          (path[i].Y == newNode->Contour[k].Y &&
              path[i].X < newNode->Contour[k].X)) {
        k = j;
      }
    }

  if ((endType == etClosedPolygon && j < 2) ||
      (endType != etClosedPolygon && j < 0)) {
    delete newNode;
    return;
  }

  m_polyNodes.AddChild(*newNode);

  //if this path's lowest pt is lower than all the others then update m_lowest
  if (endType != etClosedPolygon) {
    return;
  }

  if (m_lowest.X < 0) {
    m_lowest = IntPoint(0, k);
  }
  else {
    IntPoint ip = m_polyNodes.Childs[(int)m_lowest.X]->Contour[(int)m_lowest.Y];

    if (newNode->Contour[k].Y > ip.Y ||
        (newNode->Contour[k].Y == ip.Y &&
            newNode->Contour[k].X < ip.X)) {
      m_lowest = IntPoint(m_polyNodes.ChildCount() - 1, k);
    }
  }
}


void ClipperOffset::AddPaths(const Paths& paths, JoinType joinType, EndType endType)
{
  for (Paths::size_type i = 0; i < paths.size(); ++i) {
    AddPath(paths[i], joinType, endType);
  }
}


void ClipperOffset::FixOrientations()
{
  //fixup orientations of all closed paths if the orientation of the
  //closed path with the lowermost vertex is wrong ...
  if (m_lowest.X >= 0 &&
      !Orientation(m_polyNodes.Childs[(int)m_lowest.X]->Contour)) {
    for (int i = 0; i < m_polyNodes.ChildCount(); ++i) {
      PolyNode& node = *m_polyNodes.Childs[i];

      if (node.m_endtype == etClosedPolygon ||
          (node.m_endtype == etClosedLine && Orientation(node.Contour))) {
        ReversePath(node.Contour);
      }
    }
  }
  else {
    for (int i = 0; i < m_polyNodes.ChildCount(); ++i) {
      PolyNode& node = *m_polyNodes.Childs[i];

      if (node.m_endtype == etClosedLine && !Orientation(node.Contour)) {
        ReversePath(node.Contour);
      }
    }
  }
}


void ClipperOffset::Execute(Paths& solution, double delta)
{
  solution.clear();
  FixOrientations();
  DoOffset(delta);

  //now clean up 'corners' ...
  Clipper clpr;
  clpr.AddPaths(m_destPolys, ptSubject, true);

  if (delta > 0) {
    clpr.Execute(ctUnion, solution, pftPositive, pftPositive);
  }
  else {
    IntRect r = clpr.GetBounds();
    Path outer(4);
    outer[0] = IntPoint(r.left - 10, r.bottom + 10);
    outer[1] = IntPoint(r.right + 10, r.bottom + 10);
    outer[2] = IntPoint(r.right + 10, r.top - 10);
    outer[3] = IntPoint(r.left - 10, r.top - 10);

    clpr.AddPath(outer, ptSubject, true);
    clpr.ReverseSolution(true);
    clpr.Execute(ctUnion, solution, pftNegative, pftNegative);

    if (solution.size() > 0) {
      solution.erase(solution.begin());
    }
  }
}


void ClipperOffset::Execute(PolyTree& solution, double delta)
{
  solution.Clear();
  FixOrientations();
  DoOffset(delta);

  //now clean up 'corners' ...
  Clipper clpr;
  clpr.AddPaths(m_destPolys, ptSubject, true);

  if (delta > 0) {
    clpr.Execute(ctUnion, solution, pftPositive, pftPositive);
  }
  else {
    IntRect r = clpr.GetBounds();
    Path outer(4);
    outer[0] = IntPoint(r.left - 10, r.bottom + 10);
    outer[1] = IntPoint(r.right + 10, r.bottom + 10);
    outer[2] = IntPoint(r.right + 10, r.top - 10);
    outer[3] = IntPoint(r.left - 10, r.top - 10);

    clpr.AddPath(outer, ptSubject, true);
    clpr.ReverseSolution(true);
    clpr.Execute(ctUnion, solution, pftNegative, pftNegative);

    //remove the outer PolyNode rectangle ...
    if (solution.ChildCount() == 1 && solution.Childs[0]->ChildCount() > 0) {
      PolyNode* outerNode = solution.Childs[0];
      solution.Childs.reserve(outerNode->ChildCount());
      solution.Childs[0] = outerNode->Childs[0];

      for (int i = 1; i < outerNode->ChildCount(); ++i) {
        solution.AddChild(*outerNode->Childs[i]);
      }
    }
    else {
      solution.Clear();
    }
  }
}


void ClipperOffset::DoOffset(double delta)
{
  m_destPolys.clear();
  m_delta = delta;

  //if Zero offset, just copy any CLOSED polygons to m_p and return ...
  if (NEAR_ZERO(delta)) {
    m_destPolys.reserve(m_polyNodes.ChildCount());

    for (int i = 0; i < m_polyNodes.ChildCount(); i++) {
      PolyNode& node = *m_polyNodes.Childs[i];

      if (node.m_endtype == etClosedPolygon) {
        m_destPolys.push_back(node.Contour);
      }
    }

    return;
  }

  //see offset_triginometry3.svg in the documentation folder ...
  if (MiterLimit > 2) {
    m_miterLim = 2 / (MiterLimit * MiterLimit);
  }
  else {
    m_miterLim = 0.5;
  }

  double y;

  if (ArcTolerance <= 0.0) {
    y = def_arc_tolerance;
  }
  else if (ArcTolerance > fabs(delta) * def_arc_tolerance) {
    y = fabs(delta) * def_arc_tolerance;
  }
  else {
    y = ArcTolerance;
  }

  //see offset_triginometry2.svg in the documentation folder ...
  double steps = pi / acos(1 - y / fabs(delta));

  if (steps > fabs(delta) * pi) {
    steps = fabs(delta) * pi;  //ie excessive precision check
  }

  m_sin = sin(two_pi / steps);
  m_cos = cos(two_pi / steps);
  m_StepsPerRad = steps / two_pi;

  if (delta < 0.0) {
    m_sin = -m_sin;
  }

  m_destPolys.reserve(m_polyNodes.ChildCount() * 2);

  for (int i = 0; i < m_polyNodes.ChildCount(); i++) {
    PolyNode& node = *m_polyNodes.Childs[i];
    m_srcPoly = node.Contour;

    int len = (int)m_srcPoly.size();

    if (len == 0 || (delta <= 0 && (len < 3 || node.m_endtype != etClosedPolygon))) {
      continue;
    }

    m_destPoly.clear();

    if (len == 1) {
      if (node.m_jointype == jtRound) {
        double X = 1.0, Y = 0.0;

        for (cInt j = 1; j <= steps; j++) {
          m_destPoly.push_back(IntPoint(
              Round(m_srcPoly[0].X + X * delta),
              Round(m_srcPoly[0].Y + Y * delta)));
          double X2 = X;
          X = X * m_cos - m_sin * Y;
          Y = X2 * m_sin + Y * m_cos;
        }
      }
      else {
        double X = -1.0, Y = -1.0;

        for (int j = 0; j < 4; ++j) {
          m_destPoly.push_back(IntPoint(
              Round(m_srcPoly[0].X + X * delta),
              Round(m_srcPoly[0].Y + Y * delta)));

          if (X < 0) {
            X = 1;
          }
          else if (Y < 0) {
            Y = 1;
          }
          else {
            X = -1;
          }
        }
      }

      m_destPolys.push_back(m_destPoly);
      continue;
    }

    //build m_normals ...
    m_normals.clear();
    m_normals.reserve(len);

    for (int j = 0; j < len - 1; ++j) {
      m_normals.push_back(GetUnitNormal(m_srcPoly[j], m_srcPoly[j + 1]));
    }

    if (node.m_endtype == etClosedLine || node.m_endtype == etClosedPolygon) {
      m_normals.push_back(GetUnitNormal(m_srcPoly[len - 1], m_srcPoly[0]));
    }
    else {
      m_normals.push_back(DoublePoint(m_normals[len - 2]));
    }

    if (node.m_endtype == etClosedPolygon) {
      int k = len - 1;

      for (int j = 0; j < len; ++j) {
        OffsetPoint(j, k, node.m_jointype);
      }

      m_destPolys.push_back(m_destPoly);
    }
    else if (node.m_endtype == etClosedLine) {
      int j, k = len - 1;

      for (j = 0; j < len; ++j) {
        OffsetPoint(j, k, node.m_jointype);
      }

      m_destPolys.push_back(m_destPoly);
      m_destPoly.clear();
      //re-build m_normals ...
      DoublePoint n = m_normals[len - 1];

      for (j = len - 1; j > 0; j--) {
        m_normals[j] = DoublePoint(-m_normals[j - 1].X, -m_normals[j - 1].Y);
      }

      m_normals[0] = DoublePoint(-n.X, -n.Y);
      k = 0;

      for (j = len - 1; j >= 0; j--) {
        OffsetPoint(j, k, node.m_jointype);
      }

      m_destPolys.push_back(m_destPoly);
    }
    else {
      int j, k = 0;

      for (j = 1; j < len - 1; ++j) {
        OffsetPoint(j, k, node.m_jointype);
      }

      IntPoint pt1;

      if (node.m_endtype == etOpenButt) {
        int j = len - 1;
        pt1 = IntPoint((cInt)Round(m_srcPoly[j].X + m_normals[j].X *
            delta), (cInt)Round(m_srcPoly[j].Y + m_normals[j].Y * delta));
        m_destPoly.push_back(pt1);
        pt1 = IntPoint((cInt)Round(m_srcPoly[j].X - m_normals[j].X *
            delta), (cInt)Round(m_srcPoly[j].Y - m_normals[j].Y * delta));
        m_destPoly.push_back(pt1);
      }
      else {
        int j = len - 1;
        k = len - 2;
        m_sinA = 0;
        m_normals[j] = DoublePoint(-m_normals[j].X, -m_normals[j].Y);

        if (node.m_endtype == etOpenSquare) {
          DoSquare(j, k);
        }
        else {
          DoRound(j, k);
        }
      }

      //re-build m_normals ...
      for (j = len - 1; j > 0; j--) {
        m_normals[j] = DoublePoint(-m_normals[j - 1].X, -m_normals[j - 1].Y);
      }

      m_normals[0] = DoublePoint(-m_normals[1].X, -m_normals[1].Y);

      k = len - 1;

      for (j = k - 1; j > 0; --j) {
        OffsetPoint(j, k, node.m_jointype);
      }

      if (node.m_endtype == etOpenButt) {
        pt1 = IntPoint((cInt)Round(m_srcPoly[0].X - m_normals[0].X * delta),
            (cInt)Round(m_srcPoly[0].Y - m_normals[0].Y * delta));
        m_destPoly.push_back(pt1);
        pt1 = IntPoint((cInt)Round(m_srcPoly[0].X + m_normals[0].X * delta),
            (cInt)Round(m_srcPoly[0].Y + m_normals[0].Y * delta));
        m_destPoly.push_back(pt1);
      }
      else {
        k = 1;
        m_sinA = 0;

        if (node.m_endtype == etOpenSquare) {
          DoSquare(0, 1);
        }
        else {
          DoRound(0, 1);
        }
      }

      m_destPolys.push_back(m_destPoly);
    }
  }
}


void ClipperOffset::OffsetPoint(int j, int& k, JoinType jointype)
{
  m_sinA = (m_normals[k].X * m_normals[j].Y - m_normals[j].X * m_normals[k].Y);

  if (m_sinA < 0.00005 && m_sinA > -0.00005) {
    return;
  }
  else if (m_sinA > 1.0) {
    m_sinA = 1.0;
  }
  else if (m_sinA < -1.0) {
    m_sinA = -1.0;
  }

  if (m_sinA * m_delta < 0) {
    m_destPoly.push_back(IntPoint(Round(m_srcPoly[j].X + m_normals[k].X * m_delta),
        Round(m_srcPoly[j].Y + m_normals[k].Y * m_delta)));
    m_destPoly.push_back(m_srcPoly[j]);
    m_destPoly.push_back(IntPoint(Round(m_srcPoly[j].X + m_normals[j].X * m_delta),
        Round(m_srcPoly[j].Y + m_normals[j].Y * m_delta)));
  }
  else
    switch (jointype) {
    case jtMiter: {
      double r = 1 + (m_normals[j].X * m_normals[k].X +
          m_normals[j].Y * m_normals[k].Y);

      if (r >= m_miterLim) {
        DoMiter(j, k, r);
      }
      else {
        DoSquare(j, k);
      }

      break;
    }

    case jtSquare:
      DoSquare(j, k);
      break;

    case jtRound:
      DoRound(j, k);
      break;
    }

  k = j;
}


void ClipperOffset::DoSquare(int j, int k)
{
  double dx = tan(atan2(m_sinA,
      m_normals[k].X * m_normals[j].X + m_normals[k].Y * m_normals[j].Y) / 4);
  m_destPoly.push_back(IntPoint(
      Round(m_srcPoly[j].X + m_delta * (m_normals[k].X - m_normals[k].Y * dx)),
      Round(m_srcPoly[j].Y + m_delta * (m_normals[k].Y + m_normals[k].X * dx))));
  m_destPoly.push_back(IntPoint(
      Round(m_srcPoly[j].X + m_delta * (m_normals[j].X + m_normals[j].Y * dx)),
      Round(m_srcPoly[j].Y + m_delta * (m_normals[j].Y - m_normals[j].X * dx))));
}


void ClipperOffset::DoMiter(int j, int k, double r)
{
  double q = m_delta / r;
  m_destPoly.push_back(IntPoint(Round(m_srcPoly[j].X + (m_normals[k].X + m_normals[j].X) * q),
      Round(m_srcPoly[j].Y + (m_normals[k].Y + m_normals[j].Y) * q)));
}


void ClipperOffset::DoRound(int j, int k)
{
  double a = atan2(m_sinA,
      m_normals[k].X * m_normals[j].X + m_normals[k].Y * m_normals[j].Y);
  int steps = (int)Round(m_StepsPerRad * fabs(a));

  double X = m_normals[k].X, Y = m_normals[k].Y, X2;

  for (int i = 0; i < steps; ++i) {
    m_destPoly.push_back(IntPoint(
        Round(m_srcPoly[j].X + X * m_delta),
        Round(m_srcPoly[j].Y + Y * m_delta)));
    X2 = X;
    X = X * m_cos - m_sin * Y;
    Y = X2 * m_sin + Y * m_cos;
  }

  m_destPoly.push_back(IntPoint(
      Round(m_srcPoly[j].X + m_normals[j].X * m_delta),
      Round(m_srcPoly[j].Y + m_normals[j].Y * m_delta)));
}

