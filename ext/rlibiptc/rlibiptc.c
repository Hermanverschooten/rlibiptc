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
//#include <libiptc/xtcshared.h>
#include <libiptc/libiptc.h>
#include <errno.h>
#include <stdbool.h>

VALUE cClass = Qnil;

struct xtc_handle *my_handle;
bool opened = false;
char *table;

void Init_rlibiptc() {
    cClass = rb_define_class("Iptables", rb_cObject);
    rb_define_module_function(cClass, "initialize", method_iptc_init, 1);
    rb_define_module_function(cClass, "close", method_iptc_close, 0);
    rb_define_module_function(cClass, "open?", method_iptc_opened, 0);
    rb_define_module_function(cClass, "table", method_iptc_table, 0);
    rb_define_module_function(cClass, "rules", method_iptc_rules, 1);
}

VALUE method_iptc_init(VALUE self, VALUE tablename) {
    table = StringValueCStr(tablename);
    if(opened)
        rb_raise(rb_eStandardError, "Iptables already opened!");

    my_handle = iptc_init(table);
    if(!my_handle)
        rb_raise(rb_eStandardError, "Unable to initialize with table %s, %s", table, iptc_strerror(errno));

    opened = true;

    return Qtrue;
}

VALUE method_iptc_close(VALUE self) {
    if(opened) {
        iptc_free(my_handle);
        opened = false;
    }
    return Qtrue;
}

VALUE method_iptc_table(VALUE self) {
    if(opened)
        return rb_str_new2(table);
    else
        return Qnil;
}

VALUE method_iptc_opened(VALUE self) {
    return opened ? Qtrue : Qfalse;
}

VALUE method_iptc_rules(VALUE self, VALUE chain_name) {
    if(!opened)
        rb_raise(rb_eStandardError, "You need to use open first");

    char *chain = StringValueCStr(chain_name);
    const struct ipt_entry *e;

    e = iptc_first_rule(chain, my_handle);


