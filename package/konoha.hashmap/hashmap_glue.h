/****************************************************************************
 * Copyright (c) 2012, the Konoha project authors. All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

#ifndef HASHMAP_GLUE_H_
#define HASHMAP_GLUE_H_


typedef const struct _kHashMap kHashMap;
struct _kHashMap {
	KonohaObjectHeader h;
	KUtilsHashMap *map;
};

/* ------------------------------------------------------------------------ */

static void HashMap_init(KonohaContext *kctx, kObject *o, void *conf)
{
	struct _kHashMap *map = (struct _kHashMap *)o;
	map->map = KLIB Kmap_init(kctx, 4);
	map->map = kctx->klib->Kmap_init(kctx, 4);
}

static void HashMap_free(KonohaContext *kctx, kObject *o)
{
	struct _kHashMap *map = (struct _kHashMap *)o;
	if (map->map != NULL) {
		KLIB Kmap_free(kctx, map->map, NULL);
	}
}

static void HashMap_p(KonohaContext *kctx, KonohaStack *sfp, int pos, KUtilsWriteBuffer *wb, int level)
{
	// TODO
}

#include <minikonoha/klib.h>
/* ------------------------------------------------------------------------ */

static KMETHOD HashMap_get(KonohaContext *kctx, KonohaStack *sfp)
{
	struct _kHashMap *m = (struct _kHashMap *)sfp[0].toObject;
	KUtilsHashMap *map = m->map;
	kString *key = sfp[1].toString;
	KonohaClass *ct = m->h.ct;
	kParam *cparam = CT_cparam(ct);
	kparam_t p1 = cparam->p[0];
	uintptr_t hcode = strhash(S_text(key), S_size(key));
	KUtilsHashMapEntry *e = KLIB Kmap_get(kctx, map, hcode);

	if (p1.ty == TY_Int || p1.ty == TY_Boolean || p1.ty == TY_Float) {
		RETURNd_((uintptr_t)e->uvalue);
	} else {
		RETURN_(e->ovalue);
	}
}

static KMETHOD HashMap_set(KonohaContext *kctx, KonohaStack *sfp)
{
	struct _kHashMap *m = (struct _kHashMap *)sfp[0].toObject;
	KUtilsHashMap *map = m->map;
	kString *key = sfp[1].toString;

	// want to know p1
	KonohaClass *ct = m->h.ct;
	kParam *cparam = CT_cparam(ct);
	kparam_t p1 = cparam->p[0];
	uintptr_t hcode = strhash(S_text(key), S_size(key));
	KUtilsHashMapEntry *e = KLIB Kmap_newentry(kctx, map, hcode);
	if (p1.ty == TY_Int || p1.ty == TY_Boolean || p1.ty == TY_Float) {  // FIXME
		e->uvalue =(uintptr_t)sfp[2].ivalue;
	} else {
		// object;
		e->uvalue = (uintptr_t)sfp[2].o;
	}
	RETURNvoid_();
}

static KMETHOD HashMap_new(KonohaContext *kctx, KonohaStack *sfp)
{
	RETURN_(KLIB new_kObject(kctx, O_ct(sfp[K_RTNIDX].o), 0));
}
/* ------------------------------------------------------------------------ */

#define _Public   kMethod_Public
#define _Const    kMethod_Const
#define _Im       kMethod_Immutable
#define _Coercion kMethod_Coercion
#define _F(F)   (intptr_t)(F)

#define CT_HashMap cHashMap
#define TY_HashMap cHashMap->cid
static	kbool_t hashmap_initPackage(KonohaContext *kctx, kNameSpace *ns, int argc, const char**args, kfileline_t pline)
{
	KDEFINE_CLASS defHashMap = {
		STRUCTNAME(HashMap),
		.cflag = kClass_Final,
		.init = HashMap_init,
		.free = HashMap_free,
		.p     = HashMap_p,
	};
	KonohaClass *cHashMap = Konoha_addClassDef(ns->packageId, PN_konoha, NULL, &defHashMap, pline);
	KonohaClassVar *ct = (KonohaClassVar *)CT_HashMap;
	ct->p0 = TY_String; // default
	KDEFINE_METHOD MethodData[] = {
		_Public, _F(HashMap_get), TY_T0, TY_HashMap, MN_("get"), 1, TY_String, FN_("key"),
		_Public, _F(HashMap_set), TY_void, TY_HashMap, MN_("set"), 2, TY_String, FN_("key"), TY_T0, FN_("value"),
		_Public, _F(HashMap_new), TY_HashMap, TY_HashMap, MN_("new"), 0,
		DEND,
	};
	kNameSpace_loadMethodData(ns, MethodData);
	return true;
}

static kbool_t hashmap_setupPackage(KonohaContext *kctx, kNameSpace *ns, kfileline_t pline)
{
	return true;
}

//----------------------------------------------------------------------------

static kbool_t hashmap_initNameSpace(KonohaContext *kctx,  kNameSpace *ns, kfileline_t pline)
{
	// TODO: map literal
	USING_SUGAR;
	KDEFINE_SYNTAX SYNTAX[] = {

			{ .keyword = KW_END, },
	};
	SUGAR NameSpace_defineSyntax(kctx, ns, SYNTAX);
	return true;
}

static kbool_t hashmap_setupNameSpace(KonohaContext *kctx, kNameSpace *ns, kfileline_t pline)
{
	return true;
}

#endif /* HASHMAP_GLUE_H_ */
