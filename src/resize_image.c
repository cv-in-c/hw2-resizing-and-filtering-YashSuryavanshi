#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    int a=round(x);
    int b=round(y);
    int i=a + b*im.w + im.h*im.w*c;
    float p=*(im.data + i);
    return p;
}

image nn_resize(image im, int w, int h)
{
    image newim = make_image(w, h, im.c);
    float factorw,factorh;
    factorw=im.w/w;
    factorh=im.h/h;
    for(int i=0;i<w*h*im.c;i++){
        *(newim.data+i)=nn_interpolate(im, i%w*factorw, i%h*factorh, i/(h*w));
    }
    return newim;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    float x_l = floor(x);
    float y_l = floor(y);
    float x_h = ceil(x);
    float y_h = ceil(y);

    float x_weight = (x) - x_l;
    float y_weight = (y) - y_l;

    float a = im.data[(int)y_l * im.w + (int)x_l];
    float b = im.data[(int)y_l * im.w + (int)x_h];
    float c = im.data[(int)y_h * im.w + (int)x_l];
    float d = im.data[(int)y_h * im.w + (int)x_h];

    float p = a * (1.0 - x_weight) * (1.0 - y_weight) +
    b * x_weight * (1.0 - y_weight) +
    c * y_weight * (1.0 - x_weight) +
    d * x_weight * y_weight;

    return p;
}

image bilinear_resize(image im, int w, int h)
{
    image newim = make_image(w, h, im.c);
      float x_ratio, y_ratio;

    if (w > 1) {
        x_ratio = ((float)im.w - 1.0) / ((float)w - 1.0);
    } else {
        x_ratio = 0;
    }

    if (h > 1) {
        y_ratio = ((float)im.h - 1.0) / ((float)h - 1.0);
    } else {
        y_ratio = 0;
    }
    for(int k=0;k<im.c;k++){
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
           float x=x_ratio * (float)j;
            float y=y_ratio * (float)i;
            newim.data[i * w + j +k*w*h] = bilinear_interpolate(im, x, y, im.c);
        }
    }}
    return newim;
}

