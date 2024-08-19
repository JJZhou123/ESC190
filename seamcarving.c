#include "seamcarving.h"
#include <stdio.h>
#include <math.h>

void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    int height = im->height;
    int width = im->width;
    int Rx,Gx,Bx,Ry,Gy,By;
    double value;
    int x_written = 0;
    int y_written = 0;
    /*
    *grad = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*grad)->height = height;
    (*grad)->width = width;
    (*grad)->raster = (uint8_t *)malloc(3 * height * width);
    */
    create_img(grad, height, width);
    for (int x = 0;x<width;x++){
        for(int y = 0;y<height;y++){
            x_written = 0;
            y_written = 0;
            if (x == 0){
                //printf("1. %d, 2. %d Width %d\n",get_pixel(im,y,1,1),get_pixel(im,y,width,1),width);
                Rx = get_pixel(im,y,1,0) - get_pixel(im,y,width-1,0);
                Gx = get_pixel(im,y,1,1) - get_pixel(im,y,width-1,1);
                Bx = get_pixel(im,y,1,2) - get_pixel(im,y,width-1,2);     
                x_written = 1;
            }
            else if (x == width-1){
                Rx = get_pixel(im,y,0,0) - get_pixel(im,y,width-2,0);
                Gx = get_pixel(im,y,0,1) - get_pixel(im,y,width-2,1);
                Bx = get_pixel(im,y,0,2) - get_pixel(im,y,width-2,2);  
                x_written = 1; 
            }
            
            if (x_written == 0){
                Rx = get_pixel(im,y,x+1,0) - get_pixel(im,y,x-1,0);
                Gx = get_pixel(im,y,x+1,1) - get_pixel(im,y,x-1,1);
                Bx = get_pixel(im,y,x+1,2) - get_pixel(im,y,x-1,2);
                x_written = 1; 
            }
            
            if (y == 0){
                Ry = get_pixel(im,1,x,0) - get_pixel(im,height-1,x,0);
                Gy = get_pixel(im,1,x,1) - get_pixel(im,height-1,x,1);
                By = get_pixel(im,1,x,2) - get_pixel(im,height-1,x,2);
                y_written = 1;
            }
            else if (y == height-1){
                Ry = get_pixel(im,0,x,0) - get_pixel(im,height-2,x,0);
                Gy = get_pixel(im,0,x,1) - get_pixel(im,height-2,x,1);
                By = get_pixel(im,0,x,2) - get_pixel(im,height-2,x,2);
                y_written = 1;
            }
            if (y_written == 0){
                Ry = get_pixel(im,y+1,x,0) - get_pixel(im,y-1,x,0);
                Gy = get_pixel(im,y+1,x,1) - get_pixel(im,y-1,x,1);
                By = get_pixel(im,y+1,x,2) - get_pixel(im,y-1,x,2);
                y_written = 1;
            }
            //printf("x %d, y %d, Rx %ld, Gx %ld, Bx %d, Ry %ld, Gy %ld, By %d\n",x,y,Rx,Gx,Bx,Ry,Gy,By);
            //printf("x %d, y %d, R %d, G %d, B %d\n",x,y,get_pixel(im,y,x,0),get_pixel(im,y,x,1),get_pixel(im,y,x,2));
            value = sqrt(pow(Rx,2)+pow(Gx,2)+pow(Bx,2)+pow(Ry,2)+pow(Gy,2)+pow(By,2))/10;
            value = (uint8_t) value;
            //printf("%d\n",value);
            //*grad[count]->raster = (uint8_t)sqrt(pow(Rx,2)+pow(Gx,2)+pow(Bx,2)+pow(Ry,2)+pow(Gy,2)+pow(By,2))/10;
            set_pixel((*grad),y, x, value, value, value);
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    int width = grad->width;
    *best_arr = (double*)malloc(sizeof(double)*width*(grad->height));
    double left,right,mid;
    for (int i=0;i<width;i++){
        (*best_arr)[i] = (double) get_pixel(grad,0,i,0);
    }
    for (int y = 1; y < grad->height; y++){
        for (int x = 0; x<width; x++){
            if (x == 0){
                left = (*best_arr)[(y-1)*width+x];
                right = (*best_arr)[(y-1)*width+x+1];
                if (left<=right){
                    (*best_arr)[y*width+x] = left + (double) get_pixel(grad,y,x,0);
                }
                else{
                    (*best_arr)[y*width+x] = right + (double) get_pixel(grad,y,x,0);
                }
            }
            else if (x == width-1){
                left = (*best_arr)[(y-1)*width+x-1];
                right = (*best_arr)[(y-1)*width+x];
                if (left<=right){
                    (*best_arr)[y*width+x] = left + (double) get_pixel(grad,y,x,0);
                }
                else{
                    (*best_arr)[y*width+x] = right + (double) get_pixel(grad,y,x,0);
                }
            }

            else{
                left = (*best_arr)[(y-1)*width+x-1];
                right = (*best_arr)[(y-1)*width+x+1];
                mid = (*best_arr)[(y-1)*width+x];
                if (left<=mid && left <= right){
                    (*best_arr)[y*width+x] = left + (double) get_pixel(grad,y,x,0);
                }
                else if (mid<left && mid <= right){
                    (*best_arr)[y*width+x] = mid + (double) get_pixel(grad,y,x,0);
                }
                else if (right<mid && right < left){
                    (*best_arr)[y*width+x] = right + (double) get_pixel(grad,y,x,0);
                }
                
                /*
                else if (right<left && right==mid){
                    (*best_arr)[y*width+x] = mid + (double) get_pixel(grad,y,x,0);
                }
                else if (mid<left && mid<=right){
                    (*best_arr)[y*width+x] = mid + get_pixel(grad,y,x,0);
                }
                else if (mid<left && left==right){
                    (*best_arr)[y*width+x] = mid + (double) get_pixel(grad,y,x,0);
                }
                else if (left<right && mid==right){
                    (*best_arr)[y*width+x] = left + (double) get_pixel(grad,y,x,0);
                }
                else if (right<left && mid==left){
                    (*best_arr)[y*width+x] = right + (double) get_pixel(grad,y,x,0);
                }
                else if (left==mid && left == right){
                    (*best_arr)[y*width+x] = left + (double) get_pixel(grad,y,x,0);
                }
                */
                /*
                else if (right<left && right<mid){
                    (*best_arr)[y*width+x] = right + (double) get_pixel(grad,y,x,0);
                }
                else if (mid<left && mid<right){
                    (*best_arr)[y*width+x] = mid + get_pixel(grad,y,x,0);
                }
                else if (mid<left && left==right){
                    (*best_arr)[y*width+x] = mid + (double) get_pixel(grad,y,x,0);
                }
                else if (left<right && mid==right){
                    (*best_arr)[y*width+x] = left + (double) get_pixel(grad,y,x,0);
                }
                else if (right<left && mid==left){
                    (*best_arr)[y*width+x] = right + (double) get_pixel(grad,y,x,0);
                }
                else if (left<mid || left < right){
                    (*best_arr)[y*width+x] = left + (double) get_pixel(grad,y,x,0);
                }
                else if (right<left || right<mid){
                    (*best_arr)[y*width+x] = right + (double) get_pixel(grad,y,x,0);
                }
                else if (mid<left || mid<right){
                    (*best_arr)[y*width+x] = mid + (double) get_pixel(grad,y,x,0);
                }
                else if (right==left && mid==left){
                    (*best_arr)[y*width+x] = right + (double) get_pixel(grad,y,x,0);
                }
                */
            }
        }
    }
}

void recover_path(double *best, int height, int width, int **path){
    *path = (int*)malloc(sizeof(int)*height);
    int min=0;
    double cmp = best[(height-1)*width];
    double left,right,mid;
    for (int i =0;i<width;i++){
        if (best[(height-1)*width+i]<cmp){
            min = i;
            cmp = best[(height-1)*width+i];
        }
    }
    (*path)[height-1] = min;

    for (int y = height-2;y>=0;y--){
        if ((*path)[y+1] == 0){
            left = best[y*width];
            right = best[y*width+1];
            if (left <= right){
                (*path)[y] = 0;
            }
            else{
                (*path)[y] = 1;
            }
        }  
        else if ((*path)[y+1] % (width-1) == 0){
            left = best[y*width+width-2];
            right = best[y*width+width-1];
            if (left <= right){
                (*path)[y] = width-2;
            }
            else{
                (*path)[y] = width-1;
            }
        }  
        else{
            left = best[y*width+(*path)[y+1]-1];
            right = best[y*width+(*path)[y+1]+1];
            mid = best[y*width+(*path)[y+1]];
            if (left<=mid && left <= right){
                (*path)[y] = (*path)[y+1]-1;
            }
            else if (mid<left && mid <= right){
                (*path)[y] = (*path)[y+1];
            }
            else if (right<left && right < mid){
                (*path)[y] = (*path)[y+1]+1;
            }
        }
            /*
            if (left<right && left <= mid){
                (*path)[y] = (*path)[y+1]-1;
            }
            else if (right<left && right<mid){
                (*path)[y] = (*path)[y+1]+1;
            }
            else if (right<left && right==mid){
                (*path)[y] = (*path)[y+1];
            }
            else if (mid<left && mid<right){ //If added <= it will be the same as last case
                (*path)[y] = (*path)[y+1];
            }
            else if (mid<left && left==right){
                (*path)[y] = (*path)[y+1];
            }
            else if (left<right && mid==right){
                (*path)[y] = (*path)[y+1]-1;
            }
            else if (right<left && mid==left){
                (*path)[y] = (*path)[y+1]+1;
            }
            else if (left<mid && left == right){
                (*path)[y] = (*path)[y+1]-1;
            }
        }
        */
        
    }
    /*
    for (int y = height-1; y>=0; y--){
        double cmp = best[y*width];
        for (int x = 0;x<width;x++){
            if (best[y*width+x]<cmp){
                min = x;
                cmp = best[y*width+x];
            }
        }
        (*path)[y] = min;
    }
    */

}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    int R,G,B;
    create_img(dest,src->height,src->width-1);
    for (int y = 0;y<src->height;y++){
        for (int x=0;x<src->width;x++){
            if(x < path[y]){
                R = get_pixel(src,y,x,0);
                G = get_pixel(src,y,x,1);
                B = get_pixel(src,y,x,2);
                set_pixel((*dest),y,x,R,G,B);
            }
            if (x > path[y]){
                R = get_pixel(src,y,x,0);
                G = get_pixel(src,y,x,1);
                B = get_pixel(src,y,x,2);
                set_pixel((*dest),y,x-1,R,G,B);
            }
        }
    }
}


void print_pixel(struct rgb_img *grad){
    int height = grad->height;
    int width = grad->width;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("R is %d ", get_pixel(grad, i, j, 0));
            printf("G is %d ", get_pixel(grad, i, j, 1));
            printf("B is %d ", get_pixel(grad, i, j, 2));
        }
        printf("\n");
    printf("\n");    
    }
}
