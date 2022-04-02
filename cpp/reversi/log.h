#ifndef _LOG_H
#define _LOG_H

#define dbglog   std::cerr << __FILE__ << ':' << __LINE__ << ": "
#define errlog   std::cerr << __FILE__ << ':' << __LINE__ << ": "
#define trc      std::cerr << __func__  << "\n"
#define var(v)   std::cerr << #v":" << (v) << "\n"

#endif
