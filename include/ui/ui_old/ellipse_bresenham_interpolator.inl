{
  bool flag;
  int  mx, my, mxy, min_m;
  int  fx, fy, fxy;

  mx = fx = m_cur_f + m_inc_x + m_ry2;

  if (mx < 0) {
    mx = -mx;
  }

  my = fy = m_cur_f + m_inc_y + m_rx2;

  if (my < 0) {
    my = -my;
  }

  mxy = fxy = m_cur_f + m_inc_x + m_ry2 + m_inc_y + m_rx2;

  if (mxy < 0) {
    mxy = -mxy;
  }

  min_m = mx;
  flag = true;

  if (min_m > my) {
    min_m = my;
    flag = false;
  }

  m_dx = m_dy = 0;

  if (min_m > mxy) {
    m_inc_x += m_two_ry2;
    m_inc_y += m_two_rx2;
    m_cur_f = fxy;
    m_dx = 1;
    m_dy = 1;
  }
  else if (flag) {
    m_inc_x += m_two_ry2;
    m_cur_f = fx;
    m_dx = 1;
  }
  else {
    m_inc_y += m_two_rx2;
    m_cur_f = fy;
    m_dy = 1;
  }
}