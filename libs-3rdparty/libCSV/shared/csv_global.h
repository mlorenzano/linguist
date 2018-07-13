#ifndef CSV_GLOBAL_H
#define CSV_GLOBAL_H

#if defined(CSV_LIBRARY)
#  define CSVSHARED_EXPORT __declspec(dllexport)
#else
#  define CSVSHARED_EXPORT __declspec(dllimport)
#endif

#endif // CSV_GLOBAL_H
