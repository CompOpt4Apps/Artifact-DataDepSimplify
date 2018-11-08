#if defined(__APPLE__)
#  include "config_apple.h"
#else
#  if __LP64__
#    include "config_general_64.h"
#  else
#    include "config_general_32.h"
#  endif
#endif
