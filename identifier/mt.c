#include "m_mem.h"

int main()
{
	linux_mem_init();
	char* p1 = m_malloc(100);
	char* p2 = m_malloc(200);
	char* p3 = m_malloc(200);
	m_free(p1);
	p1 = m_malloc(50);
	p1 = m_malloc(20);
	p1 = m_malloc(30);
	return 0;
}
