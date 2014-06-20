#ifndef IMATRIX
#define IMATRIX
#include "m_mem.h"

struct axis
{
	int row;
	int col;
};
struct imatrix_l
{
	unsigned int row;
	unsigned int col;
	double* data;
};
struct imatrix_c
{
    unsigned int row;
    unsigned int col;
    unsigned char* data;
};
typedef struct imatrix_l imt_l;
typedef struct imatrix_c imt_c;
typedef struct axis axis_t;

/*****************************************************************************************************/
imt_c* create_zero_c(unsigned int row,unsigned int col);
imt_l* create_zero_l(unsigned int row,unsigned int col);
void clear(void* m);
unsigned char get_pixel_c(imt_c* m,int row,int col);
double get_pixel_l(imt_l* m,int row,int col);
void set_pixel_c(imt_c* m,int row,int col,char value);
void set_pixel_l(imt_l* m,int row,int col,double value);
imt_l* c_to_l(imt_c* m);
imt_c* l_to_c(imt_l* m);
void move_l(imt_l* src,imt_l* des);
void move_c(imt_c* src,imt_c* des);
imt_c* sub_mt_c(imt_c* m,unsigned int r1, unsigned int r2,unsigned int c1,unsigned int c2);
void sub_mt_fast_c(imt_c* m,imt_c* result,unsigned int r1,unsigned int r2,unsigned int c1,unsigned int c2);
imt_l* sub_mt_l(imt_l* m,unsigned int r1,int unsigned r2,unsigned int c1,unsigned int c2);
int is_equal_l(imt_l* m1,imt_l* m2);
int is_equal_c(imt_c* m1,imt_c* m2);

/*****************************************************************************************************/

imt_c* create_zero_c(unsigned int row,unsigned int col)
{
	imt_c* result = NULL;
	int i = 0;
	result = malloc(sizeof(imt_c));
	if(result == NULL)
	{
		printf("when creating matrix\n");
		return NULL;
	}
	result->row = row;
	result->col = col;
	result->data = (unsigned char*)m_malloc(row * col);
	if(result->data == NULL)
	{
		printf("when creating matrix\n");
		m_free(result);
		return NULL;
	}
	for(i = 0;i < row * col;i ++)
	{
		result->data[i] = 0;
	}
	return result;
}

imt_l* create_zero_l(unsigned int row,unsigned int col)
{
	imt_l* result = NULL;
	int i = 0;
	result = malloc(sizeof(imt_l));
	if(result == NULL)
	{
		printf("when creating matrix\n");
		return NULL;
	}
	result->row = row;
	result->col = col;
	result->data = (double*)m_malloc(row * col * sizeof(double));
	if(result->data == NULL)
	{
		printf("when creating matrix\n");
		m_free(result);
		return NULL;
	}
	for(i = 0;i < row * col;i ++)
	{
		result->data[i] = 0;
	}
	return result;
}

void clear(void* m)
{
	if(m == NULL)
		return;
	m_free(((imt_c*)m)->data);
	free(m);
}

unsigned char get_pixel_c(imt_c* m,int row,int col)
{
	if(row < 1 || row > m->row || col < 1 || col > m->col)
		return 0;
	return *(m->data + (row - 1) * m->col + col - 1);
}

double get_pixel_l(imt_l* m,int row,int col)
{
	if(row < 1 || row > m->row || col < 1 || col > m->col)
		return 0;
	return *(m->data + (row - 1) * m->col + col - 1);
}

void set_pixel_c(imt_c* m,int row,int col,char value)
{
	if(row < 1 || row > m->row || col < 1 || col > m->col)
		return;
	*(m->data + (row - 1) * m->col + col - 1) = value;
}

void set_pixel_l(imt_l* m,int row,int col,double value)
{
	if(row < 1 || row > m->row || col < 1 || col > m->col)
		return;
	*(m->data + (row - 1) * m->col + col - 1) = value;
}

imt_l* c_to_l(imt_c* m)
{
	int i;
	imt_l* result = create_zero_l(m->row , m->col);
	if(result == NULL)
		return NULL;
	for(i = 0;i < m->row * m->col;i ++)
	{
		result->data[i] = m->data[i];
	}
	return result;
}

imt_c* l_to_c(imt_l* m)
{
	int i;
	imt_c* result = create_zero_c(m->row , m->col);
	if(result == NULL)
		return NULL;
	for(i = 0;i < m->row * m->col;i ++)
	{
		if(m->data[i] >= 254.99)
			result->data[i] = 255;
		else if (m->data[i] < 0)
			result->data[i] = 0;
		else
		result->data[i] = (int)(m->data[i]);
	}
	return result;
}

void move_l(imt_l* src,imt_l* des)
{
	int i;
	for(i = 0;i < src->row * src->col;i ++)
	{
		des->data[i] = src->data[i];
	}
}

void move_c(imt_c* src,imt_c* des)
{
	int i;
	for(i = 0;i < src->row * src->col;i ++)
	{
		des->data[i] = src->data[i];
	}
}

imt_c* sub_mt_c(imt_c* m,unsigned int r1, unsigned int r2,unsigned int c1,unsigned int c2)
{
	int i;
	int j;
	unsigned char temp;
	imt_c* result;
	result = create_zero_c(r2 - r1 + 1,c2 - c1 + 1);
	for(i = r1;i <= r2;i ++)
		for(j = c1;j <= c2;j ++)
		{
			temp = get_pixel_c(m,i,j);
			set_pixel_c(result,i - r1 + 1,j - c1 + 1,temp);
		}
	return result;
}

void sub_mt_fast_c(imt_c* m,imt_c* result,unsigned int r1,unsigned int r2,unsigned int c1,unsigned int c2)
{
	int i;
	int j;
	unsigned char temp;
	for(i = r1;i <= r2;i ++)
		for(j = c1;j <= c2;j ++)
		{
			temp = get_pixel_c(m,i,j);
			set_pixel_c(result,i - r1 + 1,j - c1 + 1,temp);
		}
}

imt_l* sub_mt_l(imt_l* m,unsigned int r1,int unsigned r2,unsigned int c1,unsigned int c2)
{
	int i;
	int j;
	double temp;
	imt_l* result;
	result = create_zero_l(r2 - r1 + 1,c2 - c1 + 1);
	for(i = r1;i <= r2;i ++)
		for(j = c1;j <= c2;j ++)
		{
			temp = get_pixel_l(m,i,j);
			set_pixel_l(result,i - r1 + 1,j - c1 + 1,temp);
		}
	return result;
}

int is_equal_l(imt_l* m1,imt_l* m2)
{
	int i;
	int size;
	size = m1->row * m1->col;
	if((m1->row !=  m2->row) || (m1->col != m2->col))
		return 0;
	else 
	{
		for(i = 0;i < size;i++)
		{
			if(m1->data[i] != m2->data[i])
				return 0;
		}
	}
	return 1;
}

int is_equal_c(imt_c* m1,imt_c* m2)
{
	int i;
	int size;
	size = m1->row * m1->col;
	if(m1->row !=  m2->row || m1->col != m2->col)
		return 0;
	else 
	{
		for(i = 0;i < size;i++)
		{
			if(m1->data[i] != m2->data[i])
				return 0;
		}
	}
	return 1;
}
#endif
