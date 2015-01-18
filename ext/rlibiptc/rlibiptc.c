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

iptc_handle *my_handle;

void Init_rlibiptc() {
    Module = rb_define_module("Rlibiptc");
    rb_define_module_function(Module, "open", method_iptc_init, 1);
    rb_define_module_function(Module, "close", method_iptc_close, 0);
}

VALUE method_iptc_init(VALUE self, VALUE tablename) {
    char *table = StringValueCStr(tablename);
    if(my_handle)
        rb_raise(rb_eStandardError, "Iptables already opened!");

    my_handle = iptc_init(table);
    if(!my_handle)
        rb_raise(rb_eStandardError, "Unable to initialize with table %s", table);

    return Qtrue;
}

VALUE method_iptc_close(VALUE self) {
    if(my_handle)
        iptc_free(my_handle);
    return Qtrue;
}
