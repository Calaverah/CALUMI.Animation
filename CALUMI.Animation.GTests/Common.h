#pragma once
#define _RED(X) std::format("\033[31m{}\033[0m",X) 
#define _GREEN(X) std::format("\033[32m{}\033[0m",X) 
#define _YELLOW(X) std::format("\033[33m{}\033[0m",X) 
#define _BLUE(X) std::format("\033[34m{}\033[0m",X) 
#define _MAGENTA(X) std::format("\033[35m{}\033[0m",X)
#define _CYAN(X) std::format("\033[36m{}\033[0m",X) 
#define _DEFAULTCOLOR(X) std::format("\033[39m{}\033[0m",X) 
#define _ENDDEFAULTCOLOR std::format("\033[0m")
#define _ENDMAGENTACOLOR std::format("\033[35m")

#define _BMAGENTA(X) std::format("\033[95m{}\033[0m",X)
#define _BRED(X) std::format("\033[91m{}\033[0m",X)