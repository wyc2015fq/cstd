
#ifndef _TEST_HPP_
#define _TEST_HPP_

typedef void(*testfun_t)();

struct test_item_t {
	const char* a;
	const char* b;
	const char* file;
	int line;
	testfun_t fun;
	test_item_t* next;
};

static test_item_t* test_init(test_item_t* x, testfun_t fun, const char* a, const char* b, const char* file, int line) {
	static test_item_t* root = NULL;
	if (x) {
		x->fun = fun;
		x->a = a;
		x->b = b;
		x->file = file;
		x->line = line;
		if (root == NULL) {
			root = x;
		}
		else {
			test_item_t* t = root;
			for (; t->next;t = t->next);
			t->next = x;
		}
	}
	else {
		x = root;
	}
	return x;
}

int testall() {
	test_item_t* t = test_init(NULL, NULL, NULL, NULL, NULL, 0);
	for (; t; t = t->next) {
		if (t->fun) {
		  t->fun();
		  printf("test OK - %s(%d):%s::%s\n", t->file, t->line, t->a, t->b);
		}
	}
	return 0;
}

#define EXPECT_TRUE(a)   assert(true==(a))
#define EXPECT_FALSE(a)  assert(false==(a))
#define EXPECT_EQ(a, b)  assert((a)==(b))

#define TEST(a, b)  test_item_t a ## b ## item = {0}; void a ## b ## fun (); test_item_t* a ## b ## itemp = test_init(&a ## b ## item, a ## b ## fun, #a, #b, __FILE__, __LINE__); void a ## b ## fun()

//TEST(aa, bb) {	int aaa = 0;}


#endif // _TEST_HPP_
