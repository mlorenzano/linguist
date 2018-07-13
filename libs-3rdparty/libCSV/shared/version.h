#ifndef VERSION_H
#define VERSION_H

#define STRINGFY(x)                         #x


#define TOSTRING(x)                         STRINGFY(x)


#define LIBCSV_VERSION_MAJOR                1

#define LIBCSV_VERSION_MINOR                0

#define LIBCSV_VERSION_REVISION             0

#define LIBCSV_VERSION                      TOSTRING(LIBCSV_VERSION_MAJOR) "." \
                                            TOSTRING(LIBCSV_VERSION_MINOR) "." \
                                            TOSTRING(LIBCSV_VERSION_REVISION)

#define LIBCSV_VERSION_CODE                 ((LIBCSV_VERSION_MAJOR << 16)   | \
                                            (LIBCSV_VERSION_MINOR << 8)     | \
                                            LIBCSV_VERSION_REVISION)

#define LIBCSV_VERSION_CHECK_(maj,min,rev)  ((maj == LIBCSV_VERSION_MAJOR)  && \
                                            (min == LIBCSV_VERSION_MINOR)   && \
                                            (rev == LIBCSV_VERSION_REVISION))

#endif // VERSION_H
