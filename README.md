# Pixel-Image-Quadtree

Part 1: Inspiration and Background
The inspiration for this program came from an article about an artist whose work recreates classic portraits with a blocky effect. 
The exact algorithm he used is not given in the article, similar images can be created using a strategy for image representation that
underlies some lossy image compression algorithms. The two images below illustrate the result of this  program. Note that the second image 
sacrifices colour detail in rectangles that do not contain much colour variability in the first, original image, but maintains detail by using 
smaller rectangles in areas of the original image containing lots of variability. <br />
![Screen Shot 2023-01-06 at 10 22 28 PM](https://user-images.githubusercontent.com/119923836/211134743-dd2e5e3d-95ae-4971-a601-2946868a96eb.png)

In specifying the algorithm several conventional assumptions are made, illustrated below: <br />
![Screen Shot 2023-01-06 at 10 24 14 PM](https://user-images.githubusercontent.com/119923836/211134491-62ed3b8b-e001-487b-a3c7-3ac4e2e24193.png)

The original image is represented in memory as a 4-ary tree whose nodes contain information about rectangles. Every node contains:
- the upper left point and the width and height that describes a rectangle of pixels in the image.
- a pixel that represents the average colour over the rectangle in the original image.
- a number that represents the variability of the colour over the rectangle in the original image. This number is described below in detail.
The two or four children of a node represent disjoint non-empty rectangles that the parent rectangle can be split into using at most one horizontal 
and at most one vertical line. <br />
![Screen Shot 2023-01-06 at 10 25 46 PM](https://user-images.githubusercontent.com/119923836/211134544-745846ea-1b16-4aec-9583-a6d2c45bee3e.png)
![Screen Shot 2023-01-06 at 10 25 53 PM](https://user-images.githubusercontent.com/119923836/211134548-31656c5a-6912-4198-ae73-4f68d1636180.png)

Every node contains a field representing the colour variability across its rectangle. The variability measurement used is the sum, over all pixels, 
of the squared difference between the pixel's colour and the rectangle's average colour. These sums are computed independently for the red, green, and 
blue colour channels, and then added to get total variability. The formula for the sum of squared difference is given on the left, and a 
computationally more convenient, but algebraically equivalent version is given on the right. (The area of rectangle |R|.) <br />
![Screen Shot 2023-01-06 at 10 28 04 PM](https://user-images.githubusercontent.com/119923836/211134592-3cbb3da2-1f1e-4529-8195-255608c32ff5.png)

To build the tree, the image is recursively split each rectangle using the horizontal and vertical line that minimizes the maximum variability 
among the (two or four) children. Note that two children (sub-rectangles) result if either the horizontal or vertical line is at an image edge. 
The case that both horizontal and vertical lines are at an edge is not considered since the result of the split would be the original rectangle. 
In case of multiple partitions having the same maximum variability, break the tie by a) the lower split, and if still tied then by 
b) the righter split. The partitions created in this program may use any horizontal and vertical lines to divide the rectangle 
(except for the one that results in the original rectangle). <br />
![Screen Shot 2023-01-06 at 10 30 19 PM](https://user-images.githubusercontent.com/119923836/211134681-0c496cdf-fd33-4e27-97f7-e3ed8a069aaf.png)

To achieve the artistic effect, the program stops splitting a rectangle and colours it using its average pixel colour when it is close enough to the 
underlying image. By "close enough" we mean that the variability is at most a prescribed tolerance.
![Screen Shot 2023-01-06 at 10 32 18 PM](https://user-images.githubusercontent.com/119923836/211134735-f8ce7a4c-ac78-4f56-9ffb-174692bafd67.png)








