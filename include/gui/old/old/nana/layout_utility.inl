
	//overlap test if overlaped between r1 and r2
	bool overlapped(const IRect& r1, const IRect& r2)
	{
		if (r1.y + (r1.h) <= r2.y) return false;
		if(r1.y >= r2.y + (r2.h)) return false;

		if(r1.x + (r1.w) <= r2.x) return false;
		if(r1.x >= r2.x + (r2.w)) return false;

		return true;
	}

	//overlap, compute the overlap area between r1 and r2. the rect is for root
	bool overlap(const IRect& r1, const IRect& r2, IRect& r)
	{
		if(overlapped(r1, r2))
		{
			int l1 = (r1.x) + r1.w;
			int l2 = (r2.x) + r2.w;

			int b1 = (r1.y) + r1.h;
			int b2 = (r2.y) + r2.h;

			r.x = r1.x < r2.x ? r2.x : r1.x;
			r.y = r1.y < r2.y ? r2.y : r1.y;

			r.w = (l1 < l2 ? l1 - r.x: l2 - r.x);
			r.h = (b1 < b2 ? b1 - r.y: b2 - r.y);

			return true;
		}
		return false;
	}

	bool intersection(const IRect & r, IPOINT pos_beg, IPOINT pos_end, IPOINT& good_pos_beg, IPOINT& good_pos_end)
	{
		const int right = r.x+r.w;
		const int bottom = r.y+r.h;

    if(pos_beg.x > pos_end.x) {
      IPOINT tpt;
			CV_SWAP(pos_beg, pos_end, tpt);
    }

		bool good_beg = (0 <= pos_beg.x && pos_beg.x < right && 0 <= pos_beg.y && pos_beg.y < bottom);
		bool good_end = (0 <= pos_end.x && pos_end.x < right && 0 <= pos_end.y && pos_end.y < bottom);


		if(good_beg && good_end)
		{
			good_pos_beg = pos_beg;
			good_pos_end = pos_end;
			return true;
		}
		else if(pos_beg.x == pos_end.x)
		{
			if(r.x <= pos_beg.x && pos_beg.x < right)
			{
				if(pos_beg.y < r.y)
				{
					if(pos_end.y < r.y)
						return false;
					good_pos_beg.y = r.y;
					good_pos_end.y = (pos_end.y < bottom ? pos_end.y  : bottom - 1);
				}
				else if(pos_beg.y >= bottom)
				{
					if(pos_end.y >= bottom)
						return false;
					good_pos_beg.y = bottom - 1;
					good_pos_end.y = (pos_end.y < r.y ? r.y : pos_end.y);
				}

				good_pos_beg.x = good_pos_end.x = r.x;
				return true;
			}
			return false;
		}
		else if(pos_beg.y == pos_end.y)
		{
			if(r.y <= pos_beg.y && pos_beg.y < bottom)
			{
				if(pos_beg.x < r.x)
				{
					if(pos_end.x < r.x)
						return false;
					good_pos_beg.x = r.x;
					good_pos_end.x = (pos_end.x < right ? pos_end.x : right - 1);
				}
				else if(pos_beg.x >= right)
				{
					if(pos_end.x >= right)
						return false;

					good_pos_beg.x = right - 1;
					good_pos_end.x = (pos_end.x < r.x ? r.x : pos_end.x);
				}
				good_pos_beg.y = good_pos_end.y = r.y;
				return true;
			}
			return false;
		}

		double m = (pos_end.y - pos_beg.y ) / double(pos_end.x - pos_beg.x);
		bool is_nw_to_se = (m >= 0.0);
		//The formulas for the line.
		//y = m * (x - pos_beg.x) + pos_beg.y
		//x = (y - pos_beg.y) / m + pos_beg.x
		if(!good_beg)
		{
			good_pos_beg.y = static_cast<int>(m * (r.x - pos_beg.x)) + pos_beg.y;
			if(r.y <= good_pos_beg.y && good_pos_beg.y < bottom)
			{
				good_pos_beg.x = r.x;
			}
			else
			{
				bool cond;
				int y;
				if(is_nw_to_se)
				{
					y = r.y;
					cond = good_pos_beg.y < y;
				}
				else
				{
					y = bottom - 1;
					cond = good_pos_beg.y > y;
				}

				if(cond)
				{
					good_pos_beg.x = static_cast<int>((y - pos_beg.y) / m) + pos_beg.x;
					if(r.x <= good_pos_beg.x && good_pos_beg.x < right)
						good_pos_beg.y = y;
					else
						return false;
				}
				else
					return false;
			}

			if(good_pos_beg.x < pos_beg.x)
				return false;
		}
		else
			good_pos_beg = pos_beg;

		if(!good_end)
		{
			good_pos_end.y = static_cast<int>(m * (right - 1 - pos_beg.x)) + pos_beg.y;
			if(r.y <= good_pos_end.y && good_pos_end.y < bottom)
			{
				good_pos_end.x = right - 1;
			}
			else
			{
				bool cond;
				int y;
				if(is_nw_to_se)
				{
					y = bottom - 1;
					cond = good_pos_end.y > y;
				}
				else
				{
					y = r.y;
					cond = good_pos_end.y < y;
				}

				if(cond)
				{
					good_pos_end.x = static_cast<int>((y - pos_beg.y) / m) + pos_beg.x;
					if(r.x <= good_pos_end.x && good_pos_end.x < right)
						good_pos_end.y = y;
					else
						return false;
				}
				else
					return false;
			}
			if(good_pos_end.x > pos_end.x)
				return false;
		}
		else
			good_pos_end = pos_end;

		return true;
	}

	void fit_zoom(const ISIZE& input_s, const ISIZE& ref_s, ISIZE& result_s)
	{
		double rate_input = double(input_s.w) / double(input_s.h);
		double rate_ref = double(ref_s.w) / double(ref_s.h);

		if(rate_input < rate_ref)
		{
			result_s.h = ref_s.h;
			result_s.w = (ref_s.h * rate_input);
		}
		else if(rate_input > rate_ref)
		{
			result_s.w = ref_s.w;
			result_s.h = (ref_s.w / rate_input);
		}
		else
			result_s = ref_s;
	}

	ISIZE fit_zoom(const ISIZE& input_s, ISIZE ref_s)
	{
		double rate_input = double(input_s.w) / double(input_s.h);
		double rate_ref = double(ref_s.w) / double(ref_s.h);

		if (rate_input < rate_ref)
			ref_s.w = (ref_s.h * rate_input);
		else if (rate_input > rate_ref)
			ref_s.h = (ref_s.w / rate_input);

		return ref_s;
	}

	void zoom(const IRect& ref, const IRect& scaled, const IRect& ref_dst, IRect& r)
	{
		double rate_x = (scaled.x - ref.x) / double(ref.w);
		double rate_y = (scaled.y - ref.y) / double(ref.h);

		r.x = static_cast<int>(rate_x * ref_dst.w) + ref_dst.x;
		r.y = static_cast<int>(rate_y * ref_dst.h) + ref_dst.y;

		r.w = (scaled.w / double(ref.w) * ref_dst.w);
		r.h = (scaled.h / double(ref.h) * ref_dst.h);
	}

	//covered test if r2 covers r1.
	bool covered(const IRect& r1, //Rectangle 1 is must under IRect 2
						const IRect& r2) //Rectangle 2
	{
		if(r1.x < r2.x || (r1.x+r1.w) > (r2.x+r2.w)) return false;
		if(r1.y < r2.y || (r1.x+r1.w) > (r2.y+r2.h)) return false;
		return true;
	}

  
  bool overlap(const IRect& ir, const ISIZE& valid_input_area, const IRect & dr, const ISIZE& valid_dst_area, IRect& op_ir, IRect& op_dr)
  {
    IRect good_dr;
    IRect valid_r = iRect(0,0,valid_input_area.w, valid_input_area.h);
    if (overlap(ir, valid_r, op_ir) == false)
      return false;
    
    valid_r.w = valid_dst_area.w;
    valid_r.h = valid_dst_area.h;
    if (overlap(dr, valid_r, good_dr) == false)
      return false;
    
    zoom(ir, op_ir, dr, op_dr);
    
    if (covered(op_dr, good_dr))
    {
      overlap(op_dr, good_dr, op_dr);
    }
    else
    {
      op_dr = good_dr;
      zoom(dr, good_dr, ir, op_ir);
    }
    return true;
  }
