/*
 *
 * Compile:
 * gcc -fPIC -c -o libfakeopen.o libfakeopen.c
 * gcc -shared -o libfakeopen.so libfakeopen.o -ldl
 *
 * Use:
 * LD_PRELOAD="./libfakeopen.so" command
 *
 * In this example an open of "/etc/resolve.conf.fp-tmp" is transparently faked to
 * "/etc/resolv.conf.fake".
 */

//#define _GNU_SOURCE
#ifndef RTLD_NEXT
# define _GNU_SOURCE
#endif
#include <stdio.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>
#define _FCNTL_H
#include <bits/fcntl.h>

extern int errorno;

int (*_open)(const char * pathname, int flags, ...);
int (*_open64)(const char * pathname, int flags, ...);

int open(const char * pathname, int flags, mode_t mode)
{
    _open = (int (*)(const char * pathname, int flags, ...)) dlsym(RTLD_NEXT, "open");
    printf("[i] FAKE open(%s) CALLED:", pathname);
    const char * finalpath = pathname;
    const char * resolvconfpath = "/etc/resolv.conf.fp-tmp";
    const char * resolvconffakepath = "/etc/resolv.conf.fake";
    if (strncmp(pathname, resolvconfpath, sizeof(resolvconfpath)) == 0) {
    	finalpath = resolvconffakepath;
	printf("(faking path to %s)", finalpath);
    }
    printf("\n");
    if(flags & O_CREAT)
        return _open(finalpath, flags | O_NOATIME, mode);
    else
        return _open(finalpath, flags | O_NOATIME, 0);
}

int open64(const char * pathname, int flags, mode_t mode)
{
    _open64 = (int (*)(const char * pathname, int flags, ...)) dlsym(RTLD_NEXT, "open64");
    if(flags & O_CREAT)
        return _open64(pathname, flags | O_NOATIME, mode);
    else
        return _open64(pathname, flags | O_NOATIME, 0);
}
