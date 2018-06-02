
Status  PathCreateIter(PathIterator *iterator, const Path* path)
{
    INT size;

    TRACE("(%p, %p)\n", iterator, path);

    if(!iterator)
        return InvalidParameter;

    if(path){
        size = path->pathdata.Count;

        PathData_SetSize(&iterator->pathdata, size);
        memcpy(iterator->pathdata.Types, path->pathdata.Types, size);
        memcpy(iterator->pathdata.Points, path->pathdata.Points,size * sizeof(PointF));
        iterator->pathdata.Count = size;
    }
    else{
      PathData_Free(&iterator->pathdata);
        iterator->pathdata.Count  = 0;
    }

    iterator->subpath_pos  = 0;
    iterator->marker_pos   = 0;
    iterator->pathtype_pos = 0;

    return Ok;
}

Status  PathDeleteIter(PathIterator *iter)
{
    TRACE("(%p)\n", iter);

    if(!iter)
        return InvalidParameter;
    
    PathData_Free(&iter->pathdata);

    return Ok;
}

Status  GdipPathIterCopyData(PathIterator* iterator,
    INT* resultCount, PointF* points, BYTE* types, INT startIndex, INT endIndex)
{
    TRACE("(%p, %p, %p, %p, %d, %d)\n", iterator, resultCount, points, types,
           startIndex, endIndex);

    if(!iterator || !types || !points)
        return InvalidParameter;

    if(endIndex > iterator->pathdata.Count - 1 || startIndex < 0 ||
        endIndex < startIndex){
        *resultCount = 0;
        return Ok;
    }

    *resultCount = endIndex - startIndex + 1;

    memcpy(types, &(iterator->pathdata.Types[startIndex]), *resultCount);
    memcpy(points, &(iterator->pathdata.Points[startIndex]),
        *resultCount * sizeof(PointF));

    return Ok;
}

Status  GdipPathIterHasCurve(PathIterator* iterator, BOOL* hasCurve)
{
    INT i;

    TRACE("(%p, %p)\n", iterator, hasCurve);

    if(!iterator)
        return InvalidParameter;

    *hasCurve = FALSE;

    for(i = 0; i < iterator->pathdata.Count; i++)
        if((iterator->pathdata.Types[i] & PathPointTypePathTypeMask) == PathPointTypeBezier){
            *hasCurve = TRUE;
            break;
        }

    return Ok;
}

Status  GdipPathIterGetSubpathCount(PathIterator* iterator, INT* count)
{
    INT i;

    TRACE("(%p, %p)\n", iterator, count);

    if(!iterator || !count)
        return InvalidParameter;

    *count = 0;
    for(i = 0; i < iterator->pathdata.Count; i++){
        if(iterator->pathdata.Types[i] == PathPointTypeStart)
            (*count)++;
    }

    return Ok;
}

Status  GdipPathIterNextMarker(PathIterator* iterator, INT *resultCount,
    INT* startIndex, INT* endIndex)
{
    INT i;

    TRACE("(%p, %p, %p, %p)\n", iterator, resultCount, startIndex, endIndex);

    if(!iterator || !startIndex || !endIndex)
        return InvalidParameter;

    *resultCount = 0;

    /* first call could start with second point as all subsequent, cause
       path couldn't contain only one */
    for(i = iterator->marker_pos + 1; i < iterator->pathdata.Count; i++){
        if((iterator->pathdata.Types[i] & PathPointTypePathMarker) ||
           (i == iterator->pathdata.Count - 1)){
            *startIndex = iterator->marker_pos;
            if(iterator->marker_pos > 0) (*startIndex)++;
            *endIndex   = iterator->marker_pos = i;
            *resultCount= *endIndex - *startIndex + 1;
            break;
        }
    }

    return Ok;
}

Status  GdipPathIterNextMarkerPath(PathIterator* iterator, INT* result,
    Path* path)
{
    INT start, end;

    TRACE("(%p, %p, %p)\n", iterator, result, path);

    if(!iterator || !result)
        return InvalidParameter;

    GdipPathIterNextMarker(iterator, result, &start, &end);
    /* return path */
    if(((*result) > 0) && path){
        PathReset(path);

        if(!lengthen_path(path, *result))
            return OutOfMemory;

        memcpy(path->pathdata.Points, &(iterator->pathdata.Points[start]), sizeof(PointF)*(*result));
        memcpy(path->pathdata.Types,  &(iterator->pathdata.Types[start]),  sizeof(BYTE)*(*result));
        path->pathdata.Count = *result;
    }

    return Ok;
}

Status  GdipPathIterNextSubpath(PathIterator* iterator,
    INT *resultCount, INT* startIndex, INT* endIndex, BOOL* isClosed)
{
    INT i, count;

    TRACE("(%p, %p, %p, %p, %p)\n", iterator, resultCount, startIndex,
           endIndex, isClosed);

    if(!iterator || !startIndex || !endIndex || !isClosed || !resultCount)
        return InvalidParameter;

    count = iterator->pathdata.Count;

    /* iterator created with NULL path */
    if(count == 0){
        *resultCount = 0;
        return Ok;
    }

    if(iterator->subpath_pos == count){
        *startIndex = *endIndex = *resultCount = 0;
        *isClosed = 1;
        return Ok;
    }

    *startIndex = iterator->subpath_pos;

    for(i = iterator->subpath_pos + 1; i < count &&
        !(iterator->pathdata.Types[i] == PathPointTypeStart); i++);

    *endIndex = i - 1;
    iterator->subpath_pos = i;

    *resultCount = *endIndex - *startIndex + 1;

    if(iterator->pathdata.Types[*endIndex] & PathPointTypeCloseSubpath)
        *isClosed = TRUE;
    else
        *isClosed = FALSE;

    return Ok;
}
Status  GdipPathIterRewind(PathIterator *iterator)
{
    TRACE("(%p)\n", iterator);

    if(!iterator)
        return InvalidParameter;

    iterator->subpath_pos = 0;
    iterator->marker_pos = 0;
    iterator->pathtype_pos = 0;

    return Ok;
}

Status  GdipPathIterGetCount(PathIterator* iterator, INT* count)
{
    TRACE("(%p, %p)\n", iterator, count);

    if(!iterator || !count)
        return InvalidParameter;

    *count = iterator->pathdata.Count;

    return Ok;
}

Status  GdipPathIterEnumerate(PathIterator* iterator, INT* resultCount,
    PointF *points, BYTE *types, INT count)
{
    TRACE("(%p, %p, %p, %p, %d)\n", iterator, resultCount, points, types, count);

    if((count < 0) || !resultCount)
        return InvalidParameter;

    if(count == 0){
        *resultCount = 0;
        return Ok;
    }

    return GdipPathIterCopyData(iterator, resultCount, points, types, 0, count-1);
}

Status  GdipPathIterIsValid(PathIterator* iterator, BOOL* valid)
{
    TRACE("(%p, %p)\n", iterator, valid);

    if(!iterator || !valid)
        return InvalidParameter;

    *valid = TRUE;

    return Ok;
}

Status  GdipPathIterNextPathType(PathIterator* iter, INT* result,
    BYTE* type, INT* start, INT* end)
{
    FIXME("(%p, %p, %p, %p, %p) stub\n", iter, result, type, start, end);

    if(!iter || !result || !type || !start || !end)
        return InvalidParameter;

    return NotImplemented;
}

Status  GdipPathIterNextSubpathPath(PathIterator* iter, INT* result,
    Path* path, BOOL* closed)
{
    INT start, end;

    TRACE("(%p, %p, %p, %p)\n", iter, result, path, closed);

    if(!iter || !result || !closed)
        return InvalidParameter;

    GdipPathIterNextSubpath(iter, result, &start, &end, closed);
    /* return path */
    if(((*result) > 0) && path){
        PathReset(path);

        if(!lengthen_path(path, *result))
            return OutOfMemory;

        memcpy(path->pathdata.Points, &(iter->pathdata.Points[start]), sizeof(PointF)*(*result));
        memcpy(path->pathdata.Types,  &(iter->pathdata.Types[start]),  sizeof(BYTE)*(*result));
        path->pathdata.Count = *result;
    }

    return Ok;
}
