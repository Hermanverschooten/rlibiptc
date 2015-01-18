/*
 * rlibiptc.c
 *
 * Simple interface to some function of iptables through the use of libiptc.
 *
 * (c) 2015 Herman verschooten
 *
 */

#include <ruby.h>
#include <rlibiptc.h>
#include <libiptc/xtcshared.h>
#include <libiptc/libiptc.h>
#include <xtables.h>

VALUE Module = Qnil;

void Init_rlibiptc() {
    Module = rb_define_module("Rlibiptc");
    rb_define_module_function(Module, "init", method_iptc_init, 1);
}

VALUE method_iptc_init(VALUE self, VALUE tablename) {
    char *table = StringValueCStr(tablename);
    iptc_handle *h;
    h = iptc_init(table);
    if(!h)
        rb_raise(rb_eStandardError, "Unable to initialize with table %s", table);

    return INT2NUM(h);
}
