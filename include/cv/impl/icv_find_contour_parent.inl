
{
  int lval = img0[lnbd.y * step + lnbd.x] & 0x7f;
  _CvContourInfo* cur = scanner->cinfo_table[lval - 2];

  assert(lval >= 2);

  /* find the first bounding contour */
  while (cur) {
    if ((unsigned)(lnbd.x - cur->rect.x) < (unsigned) cur->rect.w && (unsigned)(lnbd.y - cur->rect.y) < (unsigned) cur->rect.h) {
      if (par_info) {
        if (icvTraceContour(scanner->img0 + par_info->origin.y * step + par_info->origin.x, step, img + lnbd.x, par_info->is_hole) > 0) {
          break;
        }
      }

      par_info = cur;
    }

    cur = cur->next;
  }

  assert(par_info != 0);

  // if current contour is a hole and previous contour is a hole or
  // current contour is external and previous contour is external then
  // the parent of the contour is the parent of the previous contour else
  // the parent is the previous contour itself.
  if (par_info->is_hole == is_hole) {
    par_info = par_info->parent;

    /* every contour must have a parent
       (at least, the frame of the image) */
    if (!par_info) {
      par_info = &(scanner->frame_info);
    }
  }

  /* hole flag of the parent must differ from the flag of the contour */
  assert(par_info->is_hole != is_hole);
}