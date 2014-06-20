#include "bmp.h"
#include "alg.h"
#include "math.h"
int main(int argc, char** argv)
{
        int i;
		int j;
		int Ymax = 235;
		int Ymin = 16;
		int row,col;
		int sum = 0;
		int id = 0;
		double temp;
		double Cx = 109.38;
		double Cy = 152.02;
		double theta = 2.53;
		double ecx = 1.60;
		double ecy = 2.41;
		double a = 25.39;
		double b = 14.03;
		double t;
		double lea = 0;
		imt_l* H;
		imt_l* y;
		imt_c* resultr;
		imt_c* resultg;
		imt_c* resultb;
		imt_c* bi_graph;
		imt_c* result;
		imt_c* h;

		linux_mem_init();
		
		
		rgb_mt_t* image = imread(argv[1]);
		if(image == NULL)
		{
			exit(2);
		}
		row = image->r->row;
		col = image->r->col;
		
		
		
		H =  create_zero_l(3,3);
		resultr = create_zero_c(row,col);
		resultg = create_zero_c(row,col);
		resultb = create_zero_c(row,col);
		y =  create_zero_l(row,col);
		
		
		medfilt2(image->r,resultr);
		move_c(resultr,image->r);
		medfilt2(image->g,resultg);
		move_c(resultg,image->g);
		medfilt2(image->b,resultb);
		move_c(resultb,image->b);
		
		imshow("blur.bmp",image);
		
		
		set_pixel_l(H,1,1,65.4810/255);
		set_pixel_l(H,1,2,128.5530/255);
		set_pixel_l(H,1,3,24.9660/255);
		set_pixel_l(H,2,1,-37.7970/255);
		set_pixel_l(H,2,2,-74.2030/255);
		set_pixel_l(H,2,3,112.0000/255);
		set_pixel_l(H,3,1,112.0000/255);
		set_pixel_l(H,3,2,-93.7860/255);
		set_pixel_l(H,3,3,-18.2140/255);
		
		for(i = 1;i <= row;i ++)
			for(j = 1;j <= col;j ++)
			{
				temp = get_pixel_l(H,1,1) * (double)get_pixel_c(image->r,i,j) + get_pixel_l(H,1,2) * (double)get_pixel_c(image->g,i,j) + get_pixel_l(H,1,3) * (double)get_pixel_c(image->b,i,j) + 16;
				set_pixel_c(resultr,i,j,(unsigned char)temp);
				temp = get_pixel_l(H,2,1) * (double)get_pixel_c(image->r,i,j) + get_pixel_l(H,2,2) * (double)get_pixel_c(image->g,i,j) + get_pixel_l(H,2,3) * (double)get_pixel_c(image->b,i,j) + 128;
				set_pixel_c(resultg,i,j,(unsigned char)temp);
				temp = get_pixel_l(H,3,1) * (double)get_pixel_c(image->r,i,j) + get_pixel_l(H,3,2) * (double)get_pixel_c(image->g,i,j) + get_pixel_l(H,3,3) * (double)get_pixel_c(image->b,i,j) + 128;
				set_pixel_c(resultb,i,j,(unsigned char)temp);
			}
		clear(H);		
		bi_graph = create_zero_c(row,col);
			
		for(i = 1;i <= row;i ++)
			for(j = 1;j <= col;j ++)
			{
				temp = cos(theta) * ((double)get_pixel_c(resultg,i,j) - Cx) + sin(theta) * ((double)get_pixel_c(resultb,i,j) - Cy);
				set_pixel_l(y,i,j,-sin(theta)*((double)get_pixel_c(resultg,i,j) - Cx) + cos(theta)*((double)get_pixel_c(resultb,i,j) - Cy));
				lea = (temp - ecx) * (temp - ecx) / (a * a) + (get_pixel_l(y,i,j) - ecy) * (get_pixel_l(y,i,j) - ecy) / b / b;
				if(lea < 1.0)
				{
					set_pixel_c(bi_graph,i,j,255);
				}
				if(get_pixel_c(resultr,i,j) <= 80)
				{
					set_pixel_c(bi_graph,i,j,0);
				}
			}
			
		clear(resultr);
		clear(resultg);
		clear(resultb);
		
		image->r = bi_graph;
		image->g = bi_graph;
		image->b = bi_graph;
		imshow("bigraph.bmp",image);
		
		h = create_zero_c(3,3);
		set_pixel_c(h,1,1,255);
		set_pixel_c(h,1,2,255);
		set_pixel_c(h,1,3,255);
		set_pixel_c(h,2,1,255);
		set_pixel_c(h,2,2,255);
		set_pixel_c(h,2,3,255);
		set_pixel_c(h,3,1,255);
		set_pixel_c(h,3,2,255);
		set_pixel_c(h,3,3,255);
		result = imrode(bi_graph,h);
		move_c(result,bi_graph);
		clear(result);
		clear(h);
		
		h = create_zero_c(12,12);
		for(i = 1;i <= 12;i ++)
			for(j = 1;j <= 12;j ++)
				set_pixel_c(h,i,j,255);
		result = imdilate(bi_graph,h);
		move_c(result,bi_graph);
		clear(result);
		clear(h);
		
		bwareaopen(bi_graph,row * col / 10);
		for(i = 1;i <= row;i ++)
		{
			for(j = 1;j <= col;j ++)
			{
				if(get_pixel_c(bi_graph,i,j) == 0)
					set_pixel_c(bi_graph,i,j,255);
				else
					set_pixel_c(bi_graph,i,j,0);
			}
		}
		bwareaopen(bi_graph,row * col / 10);
		for(i = 1;i <= row;i ++)
		{
			for(j = 1;j <= col;j ++)
			{
				if(get_pixel_c(bi_graph,i,j) == 0)
					set_pixel_c(bi_graph,i,j,255);
				else
					set_pixel_c(bi_graph,i,j,0);
			}		
		}
		
		result = focus(bi_graph);
		id = identifier(result);
		
		image->r = bi_graph;
		image->g = bi_graph;
		image->b = bi_graph;
		imshow("result.bmp",image);
		clear(bi_graph);
		
		image = imread(argv[1]);
		for(i = 1;i <= row;i ++)
			for(j = 1;j < col;j ++)
			{
				if(i <= l1 && i >= l0 && j <= n1 && j >= n0)
				{
					if(get_pixel_c(result,i - l0,j - n0) == 255)
					{
						sum ++;
						set_pixel_c(image->r,i,j,0);
						set_pixel_c(image->g,i,j,255);
						set_pixel_c(image->b,i,j,0);
					}
				}
				if(((j > n0 - 5 && j < n0 + 5) || (j < n1 + 5 && j > n1 - 5)) && ((i < l1 + 5 && i > l1 - 5) || (i > l0 - 5&& i < l0 + 5)))
				{
					set_pixel_c(image->r,i,j,255);
					set_pixel_c(image->g,i,j,0);
					set_pixel_c(image->b,i,j,0);
				}
			}
		imshow("focus.bmp",image);
		m_stat(); 
		characteristic();
		
		if(id == 0)
		{
			printf("Unknown\n");
		}
		else if(id == 1)
		{
			printf("Rock!\n");
		}
		else if(id == 2)
		{
			printf("Scissors!\n");
		}
		else if(id == 3)
		{
			printf("Paper!\n");
		}
        return 0;
}



