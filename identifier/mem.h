#ifndef MMEM
#define MMEM
#include <stdlib.h> 
#define MAX_SEG 50
typedef struct seg
{
	void* left;
	void* right;
} seg_t;

seg_t pointers[MAX_SEG];


void* m_malloc(unsigned int size)
{
	void* result;
	result = malloc(size);
	if(result == NULL)
		perror("memory error\n");
	return result; 
}

void* s_malloc(unsigned int size)
{
	void* result;
	result = malloc(size);
	if(result == NULL)
		perror("memory error\n");
	return result; 
}

unsigned int s_free(void* p,unsigned int size)
{
	free(p);
	return size;
}

void m_free(void* p)
{
	free(p);
}
#endif
