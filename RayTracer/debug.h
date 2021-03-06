/**
*  @file debug.h
*  @poject RayTracer for online courses
*  @brief Debugging functons
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer3_debug_h
#define RayTracer3_debug_h

#include <stdarg.h>

/// this function used in case if target platform for some reason
/// missing 'sprintf' function.
/// this is safe and convenient but not exactly efficient
inline std::string format(const char* fmt, ...) {
    int size = 512;
    char* buffer = 0;
    buffer = new char[size];
    va_list vl;
    va_start(vl,fmt);
    int nsize = vsnprintf(buffer,size,fmt,vl);
    if(size<=nsize) { //fail delete buffer and try again
        delete buffer;
        buffer = 0;
        buffer = new char[nsize+1];//+1 for /0
        nsize = vsnprintf(buffer,size,fmt,vl);
    }
    std::string ret(buffer);
    va_end(vl);
    delete buffer;
    return ret;
}

#endif
