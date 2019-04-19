# Python版本的谷歌S2空间索引算法-朝闻道-51CTO博客
参考https://en.wikipedia.org/wiki/Hilbert_curve
```
hilbert_map = {
    'a': {(0, 0): (0, 'd'), (0, 1): (1, 'a'), (1, 0): (3, 'b'), (1, 1): (2, 'a')},
    'b': {(0, 0): (2, 'b'), (0, 1): (1, 'b'), (1, 0): (3, 'a'), (1, 1): (0, 'c')},
    'c': {(0, 0): (2, 'c'), (0, 1): (3, 'd'), (1, 0): (1, 'c'), (1, 1): (0, 'b')},
    'd': {(0, 0): (0, 'a'), (0, 1): (3, 'c'), (1, 0): (1, 'd'), (1, 1): (2, 'd')},
}
un_hilbert_map = {
    'a': { 0: (0, 0,'d'), 1: (0, 1,'a'), 3: (1, 0,'b'),  2: (1, 1,'a')},
    'b': { 2: (0, 0,'b'), 1: (0, 1,'b'), 3: (1, 0,'a'),  0: (1, 1,'c')},
    'c': { 2: (0, 0,'c'), 3: (0, 1,'d'), 1: (1, 0,'c'),  0: (1, 1,'b')},
    'd': { 0: (0, 0,'a'), 3: (0, 1,'c'), 1: (1, 0,'d'),  2: (1, 1,'d')}
}
#编码
def point_to_hilbert(lng,lat, order=16):
    print ('hilbert')
    lng_range = [-180.0, 180.0]
    lat_range = [-90.0, 90.0]
    current_square = 'a'
    position = 0
    for i in range(order - 1, -1, -1):
        position <<= 2
        lng_mid = (lng_range[0]+lng_range[1])/2
        lat_mid = (lat_range[0]+lat_range[1])/2
        if lng >= lng_mid :
            quad_x = 1
            lng_range[0] = lng_mid
        else:
            quad_x = 0
            lng_range[1] = lng_mid
        if lat >= lat_mid :
            quad_y = 1
            lat_range[0] = lat_mid
        else:
            quad_y = 0
            lat_range[1] = lat_mid
        quad_position,current_square = hilbert_map[current_square][(quad_x, quad_y)]
        position |= quad_position
    return position
#解码
def hilbert_to_point( d , order=16):
    print ('hilbert')
    lng_range = [-180.0, 180.0]
    lat_range = [-90.0, 90.0]
    current_square = 'a'
    lng=lat=lng_mid=lat_mid=0
    for i in range(order - 1, -1, -1):
        lng_mid = ( lng_range[0] + lng_range[1] ) / 2
        lat_mid = ( lat_range[0] + lat_range[1] ) / 2
        mask = 3 << (2*i)
        quad_position = (d & mask) >> (2*i)
        quad_x, quad_y, current_square= un_hilbert_map[current_square][quad_position]
        if quad_x:
            lng_range[0] = lng_mid
        else:
            lng_range[1] = lng_mid
        if quad_y:
            lat_range[0] = lat_mid
        else:
            lat_range[1] = lat_mid
    lat = lat_range[0]
    lng = lng_range[0]
    return lng,lat
if __name__ == '__main__':
    d = point_to_hilbert(-50.555443,77.776655,36)
    print (d)
    lng,lat = hilbert_to_point(d,36)
    print (lng,lat)
```
