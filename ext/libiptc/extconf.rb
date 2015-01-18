require 'mkmf'
unless find_header('libiptc/libiptc.h')
  abort "Cannot find required header libiptc.h, maybe you need to apt-get install iptables-dev?"
end
unless find_header('libiptc/xtcshared.h')
  abort "Cannot find required header libiptc.h, maybe you need to apt-get install iptables-dev?"
end
unless find_library('ip4tc','iptc_init')
  abort "Cannot find required library ip4tc"
end

create_makefile('libiptc')

