#ifndef cpp_h
#define cpp_h

#include <stddef.h>
#include <stdlib.h>

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};

void *operator new(size_t s);
void *operator new[](size_t s);
void  operator delete(void *m);
void  operator delete[](void *m);

void *operator new(size_t s)     { return malloc(s); }
void *operator new[](size_t s)   { return malloc(s); }
void  operator delete(void *m)   { free(m); }
void  operator delete[](void *m) { free(m); }

extern "C" void __cxa_pure_virtual(void); 
void __cxa_pure_virtual(void) {};
#endif