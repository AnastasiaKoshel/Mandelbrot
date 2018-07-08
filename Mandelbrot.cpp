// Mandelbrot.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include "Windows.h"
#include <string>
#include <sstream>
#include <math.h>
#include<iostream>
#define _CRT_SECURE_NO_WARNINGS

struct complex {
	double x, y;
};
int shift_up=-1000, shift_right=-1000, zoom=7;
void scale(RGBTRIPLE *rgb_l, int x)
{
	(*rgb_l).rgbtBlue = (char)(x / 10 + 100);
	(*rgb_l).rgbtRed = (char)(200 - x*x);
	(*rgb_l).rgbtGreen = (char)x;
}
int when_end(complex c);
void write_to_file()
{
	RGBTRIPLE rgb_l;
	FILE *writer = fopen("file.bmp", "w+b");
	FILE* reader = fopen("Read.bmp", "r+b");
	BITMAPFILEHEADER bf;
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, reader);
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, writer);
	BITMAPINFOHEADER bi;
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, reader);
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, writer);
	fclose(reader);
	int n;
	complex c;
	for (int i = -shift_up; i<bi.biHeight - shift_up; i++)
		for (int j = -shift_right; j < bi.biWidth - shift_right; j++)
		{
			c.x = i / (zoom*512.);
			c.y = j / (zoom*512.);
			n = when_end(c);
			//std::cout <<c.x<<" "<<c.y<<" "<< n << std::endl;
			scale(&rgb_l, n);
			/*rgb_l.rgbtBlue = 0x00;
			rgb_l.rgbtGreen = 0x00;
			rgb_l.rgbtRed = 0xff;*/
			fwrite(&rgb_l, sizeof(rgb_l), 1, writer);
		}
	fclose(writer);
}


complex complex_sum(complex a, complex b)
{
	complex c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

complex complex_square(complex a)
{
	complex c;
	c.x = a.x*a.x - a.y*a.y;
	c.y = 2 * a.x*a.y;
	return c;
}
double modul(complex a)
{
	return sqrt(a.x*a.x + a.y*a.y);
}
int when_end(complex c)
{
	int n = 0;
	double res = modul(c);
	complex prev, cur;
	prev.x = c.x; prev.y = c.y;
	while (res < 2 && n<1000)
	{
		cur = complex_sum(complex_square(prev), c);
		res = modul(cur);
		prev = cur;
		n++;
	}
	return n;
}

int main()
{


	write_to_file();
	return 0;
}



