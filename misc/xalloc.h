#ifndef XALLOC_H
#define XALLOC_H
#include <cassert>
#include <stdlib.h>

void* xmalloc(const size_t size);

void* xcalloc(const size_t nmemb, const size_t size);

void* xrealloc(void* ptr, const size_t size);

void xfree(void* ptr);

#endif
