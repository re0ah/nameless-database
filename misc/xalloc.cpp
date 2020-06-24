#include "xalloc.h"

void* xmalloc(const size_t size)
{
	void* ptr = malloc(size);
	assert(ptr);
	return ptr;
}

void* xcalloc(const size_t nmemb, const size_t size)
{
	void* ptr = calloc(nmemb, size);
	assert(ptr);
	return ptr;
}

void* xrealloc(void* ptr, const size_t size)
{
	ptr = realloc(ptr, size);
	assert(ptr);
	return ptr;
}

void xfree(void* ptr)
{
	assert(ptr);
	free(ptr);
}
