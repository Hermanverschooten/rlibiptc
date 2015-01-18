require 'mkmf'
unless find_header('libiptc/libiptc.h')
  abort "Cannot find required header libiptc.h, maybe you need to apt-get install iptables-dev?"
end
unless find_library('ip4tc','iptc_init')
  abort "Cannot find required library ip4tc"
end
have_library('iptc')
have_library('xtables')

create_makefile('rlibiptc')

