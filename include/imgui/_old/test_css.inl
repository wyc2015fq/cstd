typedef struct css_font_t {
 int size;
 int style;
 int height; //行高
 int weight; //粗细
 int variant;
 int transform;
 int decoration;
 char family[32];
} css_font_t;
typedef struct css_border_side_t {
 int style;
 int width;
 COLOR color;
} css_border_side_t;
typedef struct css_border_t {
 css_border_side_t left, top, right, bottom;
} css_border_t;
typedef struct css_background_t {
 COLOR color;
 str_t image;
 int repeat;
 int attachment;
 int transparent;
 int position;
} css_background_t;
//#include "css.inl"
typedef struct css_style_t {
 css_border_t border;
 css_background_t background;
 css_font_t font;
} css_style_t;
typedef struct css_t {
 str_t name; // 选择子名字
 css_style_t node;
} css_t;
enum {
#define CSSDEF(a, b, c) css_##a,
#include "css.txt"
#undef CSSDEF
};
COLOR color_parse(str_t s)
{
 COLOR clr;
 char str[16] = {0};
 s = STRtrim(s, " \t\r\t");
 ASSERT('#' == s.s[0]);
 ++s.s, --s.l;
 memset(str, 'f', 8);
 switch (s.l) {
 case 8:
 memcpy(str, s.s, 8);
 break;
 case 6:
 memcpy(str + 2, s.s, 6);
 break;
 case 4:
 str[0] = str[1] = s.s[0], str[2] = str[3] = s.s[1], str[4] = str[5] = s.s[2], str[6] = str[7] = s.s[3];
 break;
 case 3:
 str[2] = str[3] = s.s[0], str[4] = str[5] = s.s[1], str[6] = str[7] = s.s[2];
 break;
 }
 clr = str_hex2int(str, 8);
 return clr;
}
int css_node_parse(str_t s, css_style_t* out)
{
 static const char key[] =
#define CSSDEF(a, b, c) "|"#a
#include "css.txt"
#undef CSSDEF
 "|"
 ;
 int lkey = strlen(key);
 int i, j, id = -1, n = 0, f = 0;
 str_t s1, s2;
 for (i = 0; i < s.l; ++i) {
 if (':' == s.s[i]) {
 s1 = STRtrim(STR2(s.s + f, i - f), " \t\r\n");
 id = STRsplitfind(key, lkey, s1.s, s1.l, 0);
 ASSERT(id >= 0);
 f = i + 1;
 }
 else if (id >= 0 && ';' == s.s[i]) {
 s2 = STRtrim(STR2(s.s + f, i - f), " \t\r\n");
 f = i + 1;
 switch (id) {
 case css_color:
 break;
 case css_font_size:
 case css_font:
 for (; 1;) {
 s1 = STRsplit_str(s2, " \t", " \t", &s2, 0, 0);
 if (s1.l <= 0) {
 break;
 }
 if ('0' <= s1.s[0] && '9' >= s1.s[0]) {
 out->font.size = str_toi(s1);
 continue;
 }
 j = STRsplitfind("|xx-small|x-small|small|medium|large|x-large|xx-large|larger|smaller|", -1, s1.s, s1.l, 0);
 if (j >= 0) {
 out->font.size = j + 1;
 continue;
 }
 j = STRsplitfind("|normal|italic|oblique|", -1, s1.s, s1.l, 0);
 if (j >= 0) {
 out->font.style = j + 1;
 continue;
 }
 j = STRsplitfind("|normal|bold|bolder|lighter|", -1, s1.s, s1.l, 0);
 if (j >= 0) {
 out->font.weight = j + 1;
 continue;
 }
 j = STRsplitfind("|none|underline|blink|overline|line-through|", -1, s1.s, s1.l, 0);
 if (j >= 0) {
 out->font.decoration = j + 1;
 continue;
 }
 j = STRsplitfind("|none|capitalize|uppercase|lowercase|", -1, s1.s, s1.l, 0);
 if (j >= 0) {
 out->font.transform = j + 1;
 continue;
 }
 }
 break;
 case css_background_color:
 case css_background_image:
 case css_background_repeat:
 case css_background_attachment:
 case css_background_transparent:
 case css_background_position:
 case css_background:
 for (; 1;) {
 s1 = STRsplit_str(s2, " \t", " \t", &s2, 0, 0);
 if (s1.l <= 0) {
 break;
 }
 if ('#' == s1.s[0]) {
 out->background.color = color_parse(s1);
 }
 else if (s1.l > 3 && 'u' == s1.s[0] && 'r' == s1.s[1] && 'l' == s1.s[2] && '(' == s1.s[3] && ')' == s1.s[s1.l - 1]) {
 out->background.image = STR2(s1.s + 4, s1.l - 5);
 }
 else {
 j = STRsplitfind("|repeat|no-repeat|repeat-x|repeat-y|", -1, s1.s, s1.l, 0);
 if (j >= 0) {
 out->background.repeat = j + 1;
 continue;
 }
 j = STRsplitfind("|transparent|", -1, s1.s, s1.l, 0);
 if (j >= 0) {
 out->background.transparent = j + 1;
 continue;
 }
 j = STRsplitfind("|fixed|scroll|", -1, s1.s, s1.l, 0);
 if (j >= 0) {
 out->background.attachment = j + 1;
 continue;
 }
 j = STRsplitfind("|left|top|right|buttom|center|", -1, s1.s, s1.l, 0);
 if (j >= 0) {
 out->background.position = j + 1;
 continue;
 }
 if ('%' == s1.s[s1.l - 1]) {
 ASSERT(0);
 }
 ASSERT(0);
 }
 }
 break;
 default:
 ASSERT(0);
 break;
 }
 id = -1;
 }
 }
 return 0;
}
int css_parse(str_t s, css_t* out, int outlen)
{
 int i, n = 0;
 char ch;
 str_t s1;
 if (NULL == out) {
 int cnt[2] = {0}, t = 0;
 for (i = 0; i < s.l; ++i) {
 ch = s.s[i];
 if ('{' == ch || '}' == ch) {
 cnt['}' == ch]++;
 t = cnt[0] - cnt[1];
 if (t < 0 || t > 1) {
 return 0;
 }
 }
 }
 n = (cnt[0] != cnt[1]) ? 0 : cnt[0];
 }
 else {
 int f = 0, n = 0, ind = 0;
 for (i = 0; i < s.l; ++i) {
 ch = s.s[i];
 switch (ch) {
 case '{':
 out[n].name = STRtrim(STR2(s.s + f, i - f), " \t");
 f = i + 1;
 ++ind;
 break;
 case '}':
 ASSERT(1 == ind && "括号不匹配");
 s1 = STRtrim(STR2(s.s + f, i - f), " \t\r\n");
 css_node_parse(s1, &out[n].node);
 f = i + 1;
 ++n;
 --ind;
 break;
 default:
 break;
 }
 }
 }
 return n;
}
int test_css()
{
 str_t s[1] = {0};
 int n;
 css_t out[50];
 str_load("test.css", s);
 n = css_parse(*s, 0, 0);
 n = css_parse(*s, out, countof(out));
 str_free(s);
 return 0;
}
#define CCSTOKDEFDEF(CCSTOKDEF) \
 CCSTOKDEF(absolute) \
 CCSTOKDEF(active) \
 CCSTOKDEF(adjoins) \
 CCSTOKDEF(after) \
 CCSTOKDEF(align) \
 CCSTOKDEF(alpha) \
 CCSTOKDEF(alternate) \
 CCSTOKDEF(always) \
 CCSTOKDEF(attachment) \
 CCSTOKDEF(auto) \
 CCSTOKDEF(background) \
 CCSTOKDEF(base) \
 CCSTOKDEF(before) \
 CCSTOKDEF(block) \
 CCSTOKDEF(bold) \
 CCSTOKDEF(border) \
 CCSTOKDEF(bottom) \
 CCSTOKDEF(break) \
 CCSTOKDEF(bright) \
 CCSTOKDEF(button) \
 CCSTOKDEF(caps) \
 CCSTOKDEF(center) \
 CCSTOKDEF(checked) \
 CCSTOKDEF(children) \
 CCSTOKDEF(circle) \
 CCSTOKDEF(clip) \
 CCSTOKDEF(closable) \
 CCSTOKDEF(closed) \
 CCSTOKDEF(color) \
 CCSTOKDEF(content) \
 CCSTOKDEF(dark) \
 CCSTOKDEF(dash) \
 CCSTOKDEF(dashed) \
 CCSTOKDEF(decimal) \
 CCSTOKDEF(decoration) \
 CCSTOKDEF(default) \
 CCSTOKDEF(disabled) \
 CCSTOKDEF(disc) \
 CCSTOKDEF(dot) \
 CCSTOKDEF(dotted) \
 CCSTOKDEF(double) \
 CCSTOKDEF(edit) \
 CCSTOKDEF(editable) \
 CCSTOKDEF(enabled) \
 CCSTOKDEF(exclusive) \
 CCSTOKDEF(family) \
 CCSTOKDEF(features) \
 CCSTOKDEF(first) \
 CCSTOKDEF(fixed) \
 CCSTOKDEF(flat) \
 CCSTOKDEF(float) \
 CCSTOKDEF(floatable) \
 CCSTOKDEF(focus) \
 CCSTOKDEF(font) \
 CCSTOKDEF(frame) \
 CCSTOKDEF(gradient) \
 CCSTOKDEF(groove) \
 CCSTOKDEF(has) \
 CCSTOKDEF(height) \
 CCSTOKDEF(highlight) \
 CCSTOKDEF(highlighted) \
 CCSTOKDEF(horizontal) \
 CCSTOKDEF(hover) \
 CCSTOKDEF(image) \
 CCSTOKDEF(indent) \
 CCSTOKDEF(indeterminate) \
 CCSTOKDEF(inset) \
 CCSTOKDEF(italic) \
 CCSTOKDEF(item) \
 CCSTOKDEF(large) \
 CCSTOKDEF(last) \
 CCSTOKDEF(left) \
 CCSTOKDEF(light) \
 CCSTOKDEF(line) \
 CCSTOKDEF(link) \
 CCSTOKDEF(list) \
 CCSTOKDEF(lower) \
 CCSTOKDEF(lowercase) \
 CCSTOKDEF(margin) \
 CCSTOKDEF(max) \
 CCSTOKDEF(maximized) \
 CCSTOKDEF(medium) \
 CCSTOKDEF(mid) \
 CCSTOKDEF(middle) \
 CCSTOKDEF(midlight) \
 CCSTOKDEF(min) \
 CCSTOKDEF(minimized) \
 CCSTOKDEF(movable) \
 CCSTOKDEF(native) \
 CCSTOKDEF(next) \
 CCSTOKDEF(no) \
 CCSTOKDEF(non) \
 CCSTOKDEF(none) \
 CCSTOKDEF(normal) \
 CCSTOKDEF(nowrap) \
 CCSTOKDEF(number) \
 CCSTOKDEF(oblique) \
 CCSTOKDEF(off) \
 CCSTOKDEF(offset) \
 CCSTOKDEF(on) \
 CCSTOKDEF(one) \
 CCSTOKDEF(only) \
 CCSTOKDEF(open) \
 CCSTOKDEF(origin) \
 CCSTOKDEF(outline) \
 CCSTOKDEF(outset) \
 CCSTOKDEF(overline) \
 CCSTOKDEF(padding) \
 CCSTOKDEF(page) \
 CCSTOKDEF(paragraph) \
 CCSTOKDEF(position) \
 CCSTOKDEF(pre) \
 CCSTOKDEF(prefix) \
 CCSTOKDEF(pressed) \
 CCSTOKDEF(previous) \
 CCSTOKDEF(qt) \
 CCSTOKDEF(radius) \
 CCSTOKDEF(read) \
 CCSTOKDEF(relative) \
 CCSTOKDEF(repeat) \
 CCSTOKDEF(ridge) \
 CCSTOKDEF(right) \
 CCSTOKDEF(role) \
 CCSTOKDEF(roman) \
 CCSTOKDEF(round) \
 CCSTOKDEF(scroll) \
 CCSTOKDEF(selected) \
 CCSTOKDEF(selection) \
 CCSTOKDEF(shadow) \
 CCSTOKDEF(siblings) \
 CCSTOKDEF(size) \
 CCSTOKDEF(small) \
 CCSTOKDEF(solid) \
 CCSTOKDEF(space) \
 CCSTOKDEF(spacing) \
 CCSTOKDEF(square) \
 CCSTOKDEF(state) \
 CCSTOKDEF(static) \
 CCSTOKDEF(stretch) \
 CCSTOKDEF(style) \
 CCSTOKDEF(sub) \
 CCSTOKDEF(subcontrol) \
 CCSTOKDEF(suffix) \
 CCSTOKDEF(super) \
 CCSTOKDEF(table) \
 CCSTOKDEF(text) \
 CCSTOKDEF(through) \
 CCSTOKDEF(top) \
 CCSTOKDEF(transform) \
 CCSTOKDEF(transparent) \
 CCSTOKDEF(type) \
 CCSTOKDEF(unchecked) \
 CCSTOKDEF(underline) \
 CCSTOKDEF(upper) \
 CCSTOKDEF(uppercase) \
 CCSTOKDEF(user) \
 CCSTOKDEF(variant) \
 CCSTOKDEF(vertical) \
 CCSTOKDEF(visited) \
 CCSTOKDEF(wave) \
 CCSTOKDEF(weight) \
 CCSTOKDEF(white) \
 CCSTOKDEF(width) \
 CCSTOKDEF(window) \
 CCSTOKDEF(wrap) \
 CCSTOKDEF(x) \
 CCSTOKDEF(xx) \
 CCSTOKDEF(xy) \
 CCSTOKDEF(y) \
 CCSTOKDEF(z) \
#if 0
direction:
rtl
flex - direction（适用于伸缩容器，也就是伸缩项目的父元素）
row(默认值)：在“ltr”排版方式下从左向右排列；在“rtl”排版方式下从右向左排列。
row - reverse：与row排列方向相反，在“ltr”排版方式下从右向左排列；在“rtl”排版方式下从左向右排列。
column：类似 于row，不过是从上到下排列
column - reverse：类似于row - reverse，不过是从下到上排列。
#endif

