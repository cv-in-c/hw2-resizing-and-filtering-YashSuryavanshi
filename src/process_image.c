#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    int a;
    a= x + y*im.w + c*im.w*im.h;
    float b=*(im.data + a);
    return b;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    int a;
    a= x + y*im.w + c*im.w*im.h;
    *(im.data + a)=v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for(int i=0;i<im.h*im.w*im.c;i++){
        *(copy.data + i)=*(im.data + i);
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(int i=0;i<im.h*im.w*1;i++){
        float p=*(im.data + i)*0.299 + *(im.data + i + im.h*im.w)*0.587 + *(im.data + i + im.h*im.w*2)*0.114;
        *(gray.data + i)=p;
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for(int i=0;i<im.h*im.w;i++){
        *(im.data + i + im.h*im.w*c)+=v;
    }
}

void clamp_image(image im)
{
    for(int i=0;i<im.h*im.w*im.c;i++){
        if(*(im.data + i)<0)
        *(im.data + i)=0;
        else if(*(im.data + i)>1)
        *(im.data + i)=1;
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for(int i=0;i<im.h*im.w;i++){
        float V,m,C,S,h,H;
        V=three_way_max(*(im.data + i),*(im.data + i + im.h*im.w),*(im.data + i + im.h*im.w*2));
        m=three_way_min(*(im.data + i),*(im.data + i + im.h*im.w),*(im.data + i + im.h*im.w*2));
        C=V-m;
        if(V==0)
        S=0;
        else
        S=C/V;
        if(C==0)
        h=0;
        else if(V==*(im.data + i))
        h=(*(im.data + i + im.h*im.w)-*(im.data + i + im.h*im.w*2))/C;
        else if(V==*(im.data + i + im.h*im.w))
        h=(*(im.data + i + im.h*im.w*2)-*(im.data + i))/C + 2;
        else
        h=(*(im.data + i)-*(im.data + i + im.h*im.w))/C + 4;
        H=h/6;
        if(H<0)
        H+=1;
        *(im.data + i)=H;
        *(im.data + i + im.h*im.w)=S;
        *(im.data + i + im.h*im.w*2)=V;

    }
}

void hsv_to_rgb(image im)
{
    for(int i=0;i<im.h*im.w;i++){
      float s = *(im.data + i + im.h*im.w);
    float v = *(im.data + i + im.h*im.w*2);
    float C = s*v;
    float H=*(im.data + i)*360;
    float X = C*(1-abs(remainder(*(im.data + i)/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    r+=m;
    g+=m;
    b+=m;

    *(im.data + i)=r;
    *(im.data + i + im.h*im.w)=g;
    *(im.data + i + im.h*im.w*2)=b;
    }
}


