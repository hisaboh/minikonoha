// written by kimio

import("konoha.object");

void test() {
	int n = 123;
	String s = n as String;  // stupid cast
	assert(s == "");         // never reached
}

test();

