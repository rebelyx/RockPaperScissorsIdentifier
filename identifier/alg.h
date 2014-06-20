#ifndef ALG
#define ALG
#define MAX_CONNECTED_GRAPH 255
#include "m_mem.h"
#include "imatrix.h"
typedef struct stack
{
	axis_t data;
	struct stack* next;
	struct stack* pre;
} stack_t;

int l0 = 0;
int l1 = 0;
int l2 = 0;
int h0 = 0;
int h1 = 0;
int n0 = 0;
int n1 = 0;
unsigned int stat[MAX_CONNECTED_GRAPH];
stack_t* table = NULL;
stack_t* current = NULL;
stack_t* previous = NULL;
unsigned int count = 0;
axis_t temp;

int seg_num = 0;
double ratio = 0.0;
double percent = 0.0;

void debug(imt_c* m);

void push(unsigned int row,unsigned int col)
{
	if(table == NULL)
	{
		table = (stack_t*)s_malloc(sizeof(stack_t));
		table->data.row = row;
		table->data.col = col;
		table->next = NULL;
		table->pre = NULL;
		current = table;
		count ++;
	}
	else
	{
		current->next = (stack_t*)s_malloc(sizeof(stack_t));
		current->next->data.row = row;
		current->next->data.col = col;
		current->next->next = NULL;
		current->next->pre = current;
		current = current->next;
		count ++;
	}
}

void pop()
{
	if(table == NULL)
	{
		return;
	}
	if(table == current)
	{
		temp.row = current->data.row;
		temp.col = current->data.col;
		s_free(table);
		table = NULL;
		count--;
	}
	else
	{
		temp.row = current->data.row;
		temp.col = current->data.col;
		current = current->pre;
		s_free(current->next);
		current->next = NULL;
		count --;
	}
}

void medfilt2(imt_c* m,imt_c* result)
{
	int i = 0;
	int j = 0;
	double temp = 0;
	temp = (get_pixel_c(m,1,2) + get_pixel_c(m,2,2) + get_pixel_c(m,2,1))/3;
	set_pixel_c(result,1,1,temp);
	temp = (get_pixel_c(m,1,m->col - 1) + get_pixel_c(m,2,m->col - 1) + get_pixel_c(m,2,m->col))/3;
	set_pixel_c(result,1,m->col,temp);
	temp = (get_pixel_c(m,m->row,2) + get_pixel_c(m,m->row - 1,2) + get_pixel_c(m,m->row - 1,1))/3;
	set_pixel_c(result,m->row,1,temp);
	temp = (get_pixel_c(m,m->row,m->col - 1) + get_pixel_c(m,m->row - 1,m->col) + get_pixel_c(m,m->row - 1,m->col - 1))/3;
	set_pixel_c(result,m->row,m->col,temp);
	
	for(i = 2;i < m->col;i ++)
	{
		temp = (get_pixel_c(m,1,i - 1) + get_pixel_c(m,1,i + 1) + get_pixel_c(m,2,i) + get_pixel_c(m,2,i + 1) + get_pixel_c(m,2,i - 1))/5;
		set_pixel_c(result,1,i,temp);
		temp = (get_pixel_c(m,m->row,i - 1) + get_pixel_c(m,m->row,i + 1) + get_pixel_c(m,m->row - 1,i - 1) + get_pixel_c(m,m->row - 1,i) + get_pixel_c(m,m->row - 1,i + 1))/5;
		set_pixel_c(result,m->row,i,temp);
	}
	
	for(i = 2;i < m->row;i ++)
	{
		temp = (get_pixel_c(m,i - 1,1) + get_pixel_c(m,i + 1,1) + get_pixel_c(m,i - 1,2) + get_pixel_c(m,i,2) + get_pixel_c(m,i + 1,2))/5;
		set_pixel_c(result,i,1,temp);
		temp = (get_pixel_c(m,i - 1,m->col) + get_pixel_c(m,i + 1,m->col) + get_pixel_c(m,i - 1,m->col - 1) + get_pixel_c(m,i,m->col - 1) + get_pixel_c(m,i + 1,m->col - 1))/5;
		set_pixel_c(result,i,m->col,temp);
	}
	
	for(i = 2;i < m->row;i ++)
		for(j = 2;j < m->col;j ++)
		{
			temp = get_pixel_c(m,i - 1,j - 1) + get_pixel_c(m,i,j - 1) + get_pixel_c(m,i + 1,j - 1) + get_pixel_c(m,i - 1,j) + get_pixel_c(m,i + 1,j) + get_pixel_c(m,i - 1,j + 1) + get_pixel_c(m,i,j + 1) + get_pixel_c(m,i + 1,j + 1); 
			temp = temp/8;
			set_pixel_c(result,i,j,temp);
		}
}



imt_c* imrode(imt_c* m,imt_c* strel)   
{
	int i;
	int j;
	imt_c* temp;
	imt_c* result;
	result = create_zero_c(m->row,m->col);
	temp =  create_zero_c(strel->row,strel->col);
	for(i = 2;i <= m->row - 1;i ++)
		for(j = 2;j <= m->col - 1;j ++)
		{
			sub_mt_fast_c(m,temp,i - 1,i - 2 + strel->row,j - 1,j - 2 + strel->col);
			if(is_equal_c(temp,strel))
			{
				set_pixel_c(result,i,j,255);
			}
		}
	return result;
}

imt_c* imdilate(imt_c* m,imt_c* strel)   
{
	int i;
	int j;
	int k;
	int l;
	imt_c* result;
	result = create_zero_c(m->row,m->col);
	for(i = 2;i <= m->row - 1;i ++)
		for(j = 2;j <= m->col - 1;j ++)
		{
			if(get_pixel_c(m,i,j) == 255)
			{
				for(k = i - 1;k <= i - 2 + strel->row;k ++)
					for(l = j - 1;l <= j - 2 + strel->col;l ++)
						set_pixel_c(result,k,l,255);
			}
		}
	return result;
}


int mark(imt_c* data,imt_c* target,int row,int col,unsigned char flag)
{
	
	if(get_pixel_c(data,row,col) == 0 || get_pixel_c(target,row,col) != 0 || flag >= MAX_CONNECTED_GRAPH)
		return 0;
	push(row,col);
	while(count > 0)
	{
		pop();
		if(get_pixel_c(data,temp.row - 1,temp.col - 1) != 0 && get_pixel_c(target,temp.row - 1,temp.col - 1) == 0)
		{
			set_pixel_c(target,temp.row - 1,temp.col - 1,flag);
			push(temp.row - 1,temp.col - 1);
			stat[flag]++;
		}
		if(get_pixel_c(data,temp.row - 1,temp.col) != 0 && get_pixel_c(target,temp.row - 1,temp.col) == 0)
		{
			set_pixel_c(target,temp.row - 1,temp.col,flag);
			push(temp.row - 1,temp.col);
			stat[flag]++;
		}
		if(get_pixel_c(data,temp.row - 1,temp.col + 1) != 0 && get_pixel_c(target,temp.row - 1,temp.col - 1) == 0)
		{
			set_pixel_c(target,temp.row - 1,temp.col + 1,flag);
			push(temp.row - 1,temp.col + 1);
			stat[flag]++;
		}
		if(get_pixel_c(data,temp.row,temp.col - 1) != 0 && get_pixel_c(target,temp.row,temp.col - 1) == 0)
		{
			set_pixel_c(target,temp.row,temp.col - 1,flag);
			push(temp.row,temp.col - 1);
			stat[flag]++;
		}
		if(get_pixel_c(data,temp.row,temp.col + 1) != 0 && get_pixel_c(target,temp.row,temp.col + 1) == 0)
		{
			set_pixel_c(target,temp.row,temp.col + 1,flag);
			push(temp.row,temp.col + 1);
			stat[flag]++;
		}
		if(get_pixel_c(data,temp.row + 1,temp.col - 1) != 0 && get_pixel_c(target,temp.row + 1,temp.col - 1) == 0)
		{
			set_pixel_c(target,temp.row + 1,temp.col - 1,flag);
			push(temp.row + 1,temp.col - 1);
			stat[flag]++;
		}
		if(get_pixel_c(data,temp.row + 1,temp.col + 1) != 0 && get_pixel_c(target,temp.row + 1,temp.col + 1) == 0)
		{
			set_pixel_c(target,temp.row + 1,temp.col + 1,flag);
			push(temp.row + 1,temp.col + 1);
			stat[flag]++;
		}
	}
	return 1;
}

imt_c* imboundary(imt_c* m)
{
	int i;
	int j;
	int ret;
	int flag = 1;
	imt_c* result;
	result = create_zero_c(m->row,m->col);
	for(i = 1;i <= m->row;i ++)
	{
		for(j = 1;j <= m->col;j ++)
		{	
			ret = mark(m,result,i,j,flag);
			if(ret)
				flag++;
		}
	}	
	return 	result;
}

void bwareaopen(imt_c* m,unsigned long n)
{
	int i;
	int j;
	int flag;
	imt_c* result;
	for(i = 0;i < MAX_CONNECTED_GRAPH;i ++)
	{
		stat[i] = 0;
	}
	result = imboundary(m);
	for(i = 1;i <= m->row;i ++)
		for(j = 1;j <= m->col;j ++)
		{
			flag = get_pixel_c(result,i,j);
			if(flag == 0)
			{
				continue;
			}
			else
			{
				if(stat[flag] < n)
				{
					set_pixel_c(m,i,j,0);
				}
			}
		}
	m_free(result);
} 

imt_c* focus(imt_c* m)
{
	int i;
	int j;
	int* left;
	int* right;
	int* band;
	int rtn = 0;
	
	imt_c* result;
	
	left = (int*)m_malloc(m->row * sizeof(int));
	right = (int*)m_malloc(m->row * sizeof(int));
	band = (int*)m_malloc(m->row * sizeof(int));
	for(i = 0; i < m->row;i ++)
	{
		left[i] = 0;
		right[i] = 0;
		band[i] = 0;
		
	}
	for(i = 1;i <= m->row;i ++)
	{
		for(j = 1;j <= m->col;j ++)
		{
			if(get_pixel_c(m,i,j) == 255)
			{
				l0 = i;
				break;
			}
		}
		if(l0 != 0)
		{
			break;
		}
	}
	for(i = 1;i <= m->row;i ++)
	{
		for(j = 1;j <= m->col;j ++)
		{
			if(get_pixel_c(m,i,j) == 255)
			{
				left[i - 1] = j;
				break;
			}
		}
		for(j = 0;j <= m->col - 1;j ++)
		{
			if(get_pixel_c(m,i,m->col - j) == 255)
			{
				right[i - 1] = m->col - j;
				break;
			}
		}
		band[i - 1] = right[i - 1] - left[i - 1];
	}
	for(i = 1;i <= m->col;i ++)
	{
		for(j = 1;j <= m->row;j ++)
		{
			if(get_pixel_c(m,j,i) == 255)
			{
				n0 = i;
				break;
			}
		}
		if(n0 != 0)
		{
			break;
		}
	}
	for(i = 0;i <= m->col - 1;i ++)
	{
		for(j = 1;j < m->row;j ++)
		{
			if(get_pixel_c(m,j,m->col - i) == 255)
			{
				n1 = m->col - i;
				break;
			}
		}
		if(n1 != 0)
		{
			break;
		}
	}
	for(i = 1;i <= m->row;i ++)
	{
		if(band[i - 1] > rtn)
		{
			rtn = band[i - 1];
			h0 = left[i - 1];
			h1 = right[i - 1];
			l1 = i;
		}
	
	}
	rtn = m->col;
	for(i = l1;i <= m->row;i ++)
	{
		if(band[i - 1] > rtn)
		{
			rtn = band[i - 1];
			l2 = i;
		}
	}
	result = sub_mt_c(m,l0,l1,n0,n1);
	return result;
}

int identifier(imt_c* m)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int t = 0;
	int top = 0;
	int area = 0;
	int flag = 0;
	
    seg_num = 0;
	
	ratio = (double)m->row / (double)m->col;
	t = m->row * 0.65;
	for(j = 1;j <= m->col;j ++)
	{
		for(i = 1;i <= m->row;i ++)
		{
			if(get_pixel_c(m,i,j) == 255)
			{
				top = m->row - i;
				if(top > t)
				{
					for(k = 1; k <= m->row;k ++)
					{
						set_pixel_c(m,k,j,255);
					}
				}
				else
				{
					for(k = i; k <= m->row;k ++)
					{
						set_pixel_c(m,k,j,0);
					}
				}
				break;
			}
		}
	}
	for(i = 1;i <= m->row;i ++)
	{	
		for(j = 1;j <= m->col;j ++)
		{
			if(get_pixel_c(m,i,j) == 255)
				area ++;
		}
	}	
	
	for(i = 1;i <= m->col;i ++)
	{	
		if(get_pixel_c(m,1,i) == 255)
		{
			if(flag == 0)
			{
				flag = 1;
				seg_num ++;
			}
		}
		else
		{
			if(flag == 1)
				flag = 0;
		}
	}	
	percent = (double)area / (double)(m->row * m->col);
	
	
	if(seg_num == 0)
	{
		return 0;
	}
	else if(seg_num > 2)
	{
		return 3;
	}
	else if(seg_num == 2)
	{
		return 2;
	}
	else if(seg_num == 1)
	{
		if(ratio < 0.5)
		{
			return 1;
		}
		else if(ratio < 0.75)
		{	
			if(percent > 0.60)
				return 1;
			else
				return 3;
		}
		else if(ratio < 0.9)
		{
			return 3;
		}
		else
		{
			if(percent < 0.55)
				return 2;
			else
				return 3;
		}		
	}
}

void characteristic()
{
	printf("%f,%f %d\n",ratio,percent,seg_num);
}

void debug(imt_c* m)
{
	int i;
	int j;
	for(i = 1;i < m->row;i ++)
	{	
		for(j = 1;j < m->col;j ++)
		{
			printf("%d ",get_pixel_c(m,i,j));
		}
		printf("\n");
	}	
}
#endif


