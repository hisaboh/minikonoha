// written by okamoto
import("konoha.new");
import("konoha.array");

void f () {
	int[] ia = new int[0];
	ia.add(4);
	ia.add(3);
	assert(ia.getSize() == 2);
}

f();
