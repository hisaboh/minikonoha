// written by kimio kuramitsu

import("konoha.desugar");

Expr TypeCheckTrue(Stmt stmt, Expr expr, Gamma gma, int typeid) {
	System.p("do nothing");
	return expr; // nothing
}

addTypeCheck("true", TypeCheckTrue);

assert(true);


