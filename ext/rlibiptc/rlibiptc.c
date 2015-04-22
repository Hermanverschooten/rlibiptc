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


void open_iptables(VALUE self) {
    VALUE itable;

    itable = rb_iv_get(self, "@table");

    char *table = StringValueCStr(itable);
    if(opened)
        rb_raise(rb_eStandardError, "Iptables already opened!");

    my_handle = iptc_init(table);
    if(!my_handle)
        rb_raise(rb_eStandardError, "Unable to initialize with table %s, %s", table, iptc_strerror(errno));

    opened = true;
}

void close_iptables() {
    if(opened) {
        iptc_free(my_handle);
        opened = false;
    }
}

VALUE method_table(VALUE self) {
    return rb_iv_get(self, "@table");
}

VALUE method_chains(VALUE self) {
    open_iptables(self);

    VALUE arr;

    arr = rb_ary_new();

    const char *chain = NULL;

    for(chain = iptc_first_chain(my_handle); chain; chain = iptc_next_chain(my_handle)) {
        rb_ary_push(arr, rb_str_new2(chain));
    }

    close_iptables();

    return arr;
}

VALUE method_rules(VALUE self, VALUE chain_name) {
    char *chain = StringValueCStr(chain_name);

    if(strlen(chain) ==0)
        rb_raise(rb_eStandardError, "Chain is required!");

    const struct ipt_entry *e;

    open_iptables(self);

    VALUE arr;

    arr = rb_ary_new();

    int i = 0;

    for(e = iptc_first_rule(chain, my_handle); e; e = iptc_next_rule(e,my_handle)) {
        VALUE entry;
        entry = rb_hash_new();
        rb_hash_aset(entry, ID2SYM(rb_intern("line")), INT2NUM(i++));
        rb_hash_aset(entry, ID2SYM(rb_intern("bytes")), LONG2NUM(e->counters.bcnt));
        rb_hash_aset(entry, ID2SYM(rb_intern("packets")), LONG2NUM(e->counters.pcnt));

        rb_ary_push(arr, entry);
    }

    close_iptables();

    return arr;

}

VALUE method_init(VALUE self, VALUE tablename) {
    rb_iv_set(self, "@table", tablename);
    return self;
}

void Init_rlibiptc() {
    VALUE Module;
    Module = rb_define_module("Rlibiptc");
    cClass = rb_define_class_under(Module, "Iptables", rb_cObject);
    rb_define_method(cClass, "initialize", method_init, 1);
    rb_define_method(cClass, "table", method_table, 0);
    rb_define_method(cClass, "rules", method_rules, 1);
    rb_define_method(cClass, "chains", method_chains, 0);
}
