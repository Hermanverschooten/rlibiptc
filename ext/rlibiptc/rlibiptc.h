/*
 * rlibiptc.h
 *
 * Simple interface to some function of iptables through the use of libiptc.
 *
 * (c) 2015 Herman verschooten
 *
 */

#ifndef _RUBY_LIBIPTC_H
#define _RUBY_LIBIPTC_H

VALUE method_iptc_init(VALUE, VALUE);
VALUE method_iptc_close(VALUE);
VALUE method_iptc_table(VALUE);
VALUE method_iptc_opened(VALUE);

#endif
