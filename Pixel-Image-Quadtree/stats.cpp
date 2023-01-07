#include "stats.h"


#include "stats.h"

stats::stats(PNG & im){

/* Your code here!! */
 // initialize member variables 

 sumRed = vector<vector<long>> (im.width(), vector<long>(im.height(), 0));
 sumGreen = vector<vector<long>> (im.width(), vector<long>(im.height(), 0));
 sumBlue = vector<vector<long>> (im.width(), vector<long>(im.height(), 0));

 sumsqRed = vector<vector<long>> (im.width(), vector<long>(im.height(), 0));
 sumsqGreen = vector<vector<long>> (im.width(), vector<long>(im.height(), 0));
 sumsqBlue = vector<vector<long>> (im.width(), vector<long>(im.height(), 0));

 for (int x = 0; x < im.width(); x++) {
    for (int y = 0; y < im.height(); y++) {
        RGBAPixel* px = im.getPixel(x,y);
        
        // top left corner 
        if (x == 0 && y == 0) {
            sumRed[x][y] =   px->r;
            sumGreen[x][y] = px->g;
            sumBlue[x][y] =  px->b;

            sumsqRed[x][y] =   pow(px->r, 2);
            sumsqGreen[x][y] = pow(px->g, 2);
            sumsqBlue[x][y] =  pow(px->b, 2);

        } else if (x == 0) { 
            sumRed[x][y] =    sumRed[x][y-1] +   px->r;
            sumGreen[x][y] =  sumGreen[x][y-1] + px->g;
            sumBlue[x][y] =   sumBlue[x][y-1] +  px->b;

            sumsqRed[x][y] =   sumsqRed[x][y-1] +   pow(px->r, 2);
            sumsqGreen[x][y] = sumsqGreen[x][y-1] + pow(px->g, 2);
            sumsqBlue[x][y] =  sumsqBlue[x][y-1] +  pow(px->b, 2);

        } else if (y == 0) {
            sumRed[x][y] =   sumRed[x-1][y] +   px->r;
            sumGreen[x][y] = sumGreen[x-1][y] + px->g;
            sumBlue[x][y] =  sumBlue[x-1][y] +  px->b;

            sumsqRed[x][y] =   sumsqRed[x-1][y] +   pow(px->r, 2);
            sumsqGreen[x][y] = sumsqGreen[x-1][y] + pow(px->g, 2);
            sumsqBlue[x][y] =  sumsqBlue[x-1][y] +  pow(px->b, 2);
        } else {
            // case where x > 0 and y > 0 
            sumRed[x][y] =   sumRed[x][y-1]   + sumRed[x-1][y]   - sumRed[x-1][y-1]   + px->r;
            sumGreen[x][y] = sumGreen[x][y-1] + sumGreen[x-1][y] - sumGreen[x-1][y-1] + px->g;
            sumBlue[x][y] =  sumBlue[x][y-1]  + sumBlue[x-1][y]  - sumBlue[x-1][y-1]  + px->b;

            sumsqRed[x][y] =   sumsqRed[x][y-1]   + sumsqRed[x-1][y]   - sumsqRed[x-1][y-1]   + pow(px->r, 2);
            sumsqGreen[x][y] = sumsqGreen[x][y-1] + sumsqGreen[x-1][y] - sumsqGreen[x-1][y-1] + pow(px->g, 2);
            sumsqBlue[x][y] =  sumsqBlue[x][y-1]  + sumsqBlue[x-1][y]  - sumsqBlue[x-1][y-1]+ pow(px->b, 2);

        }
    }
 }
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */

if (channel == 'r') {
    return sumChannel(sumRed, ul, w, h);

} else if (channel == 'g') {
    return sumChannel(sumGreen, ul, w, h);

} else if (channel == 'b') {
    return sumChannel(sumBlue, ul, w, h);
}
}

long stats::sumChannel(vector<vector<long>> &vec, pair<int,int> ul, int w, int h) {
    // Case 1: top left is origin (0,0)
    int x; 
    int y;
    int sub_x = ul.first - 1;
    int sub_y = ul.second - 1;

    if (w == 0 || h == 0) return 0;
    
    if (ul.first == 0 && ul.second == 0) {
        x = w-1;
        y = h-1;
        return vec[x][y];

    // Case 2: x == 0, block against left wall of im    
    } else if (ul.first == 0) {
        x = w-1;
        y = ul.second + (h-1);
        return vec[x][y] - vec[x][sub_y];

    // Case 3: y == 0, block against top wall of im    
    } else if (ul.second == 0) {
        x = ul.first + (w-1);
        y = h-1;
        return vec[x][y] - vec[sub_x][y];

    // Case 4: both cords in top left corner are non zero    
    } else {
        x = ul.first + (w-1);
        y = ul.second + (h-1); // ;(
        return vec[x][y] - vec[x][sub_y] - vec[sub_x][y] + vec[sub_x][sub_y];
    }
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */

if (channel == 'r') {
    return sumChannel(sumsqRed, ul, w, h);

} else if (channel == 'g') {
    return sumChannel(sumsqGreen, ul, w, h);

} else if (channel == 'b') {
    return sumChannel(sumsqBlue, ul, w, h);
} 
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
/* Your code here!! */

int area = w * h;
 // need sumsq over all channels 
 
 double sumSqR = getSumSq('r', ul, w, h); 
 double sumSqG = getSumSq('g', ul, w, h);
 double sumSqB = getSumSq('b', ul, w, h);
 
 // get the sum of each channel, then square the sum 
 // then divide by area 

 double sumR = pow(getSum('r', ul, w, h), 2) / (double) area;
 double sumG = pow(getSum('g', ul, w, h), 2) / (double) area;
 double sumB = pow(getSum('b', ul, w, h), 2) / (double) area;

 // subtract sumSq and sum 

 double varR = sumSqR - sumR;
 double varG = sumSqG - sumG;
 double varB = sumSqB - sumB;

 // total variability is the var from each channel added together 
 return varR + varG + varB;

}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
/* Your code here!! */
int size = w * h;

long avgR = (int) getSum('r', ul, w, h) / (double) (w * h);

long avgG = (int) getSum('g', ul, w, h) / (double) (w * h);

long avgB = (int) getSum('b', ul, w, h) / (double) (w * h);

return RGBAPixel(avgR, avgG, avgB);
}
