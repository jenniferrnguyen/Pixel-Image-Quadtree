/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {

  // build stats objects
  stats s(imIn);
  pair<int,int> ul(0,0);
  root = buildTree(s, ul, imIn.width(), imIn.height(), tol);
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {

  if (w == 0 || h == 0) return nullptr;

  // make current node
  Node* currNode = new Node(s, ul, w, h); 

  // stop if Node var is at most tol
  if (currNode->var <= tol) {
    // Node n(ul, w, h, s.getAvg(ul,w,h), s.getVar(ul,w,h)); 
    return currNode;
  }  

  double bestVar = currNode->var;
  int bestX = 0;
  int bestY = 0; 

  double NW_var, NE_var, SW_var, SE_var; 

  // check all possible parti
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      
      // only continue if partition is different from original block (at least one of x and y != 0)
      if (x !=0 || y != 0) {

        // x = 0 ; only horizontal partion, two children
        if (x == 0) {
            NW_var = 0;
            SW_var = 0;

            NE_var = s.getVar(ul, w, y);
            pair<int,int> ul_SE(ul.first, y + ul.second);
            SE_var = s.getVar(ul_SE, w, h-y);

        // y = 0 ; only vertical partion, two children 
        } else if (y == 0) {
            NW_var = 0;
            NE_var = 0;

            SW_var = s.getVar(ul, x, h);
            pair<int,int> ul_SE(x + ul.first, ul.second);
            SE_var = s.getVar(ul_SE, w-x, h);
        
        // both vertical and horizontal partion - 4 children 
        } else if (x != 0 && y != 0) {
            NW_var = s.getVar(ul, x, y);

            pair<int,int> ul_NE(x + ul.first, ul.second);
            NE_var = s.getVar(ul_NE, w-x, y);

            pair<int,int> ul_SW(ul.first, y + ul.second);
            SW_var = s.getVar(ul_SW, x, h-y);

            pair<int,int> ul_SE(x + ul.first, y + ul.second);
            SE_var = s.getVar(ul_SE, w-x, h-y);
        }

        double currVar = NW_var; 
        // find the max var
        currVar = max(currVar, NE_var);
        currVar = max(currVar, SW_var);
        currVar = max(currVar, SE_var);

        // update maxVar, maxX, and maxY if curr_max_var < maxVar (i.e., if the best partion so far)
        if (currVar <= bestVar) {
            bestVar = currVar;
            bestX = x;
            bestY = y;
        }
      }
    }
  }

  int eastX = bestX + ul.first;
  int eastW = w - bestX;

  int southY = bestY + ul.second; 
  int southH = h - bestY;

  // now assign children 
  currNode->NW =  buildTree(s, ul, bestX, bestY, tol);
  
  pair<int,int> ul_NE(eastX, ul.second);
  currNode->NE = buildTree(s,ul_NE, eastW, bestY, tol);
  
  pair<int,int> ul_SW(ul.first, southY);
  currNode->SW = buildTree(s, ul_SW, bestX, southH, tol);
  
  pair<int,int> ul_SE(eastX, southY);
  currNode->SE = buildTree(s, ul_SE, eastW, southH, tol);

 return currNode; 

}

void SQtree::renderNode(Node* curr, PNG & imIn) {
  if (!curr) return;

  // curr is a leaf 
  if(curr->NW == nullptr && curr->NE == nullptr && curr->SW == nullptr && curr->SE == nullptr) {
    for(int x = curr->upLeft.first; x < curr->upLeft.first + curr->width; x++) {
      for(int y = curr->upLeft.second; y < curr->upLeft.second + curr->height; y++) {
        RGBAPixel* px = imIn.getPixel(x,y);
        *px = curr->avg;
      }
    }
  }

  renderNode(curr->NW, imIn);
  renderNode(curr->NE, imIn);
  renderNode(curr->SW, imIn);
  renderNode(curr->SE, imIn);

}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  PNG img(root->width, root->height);
  renderNode(root, img);
  return img; 
}

// clear helper
void SQtree::clear(Node* curr) {
  if (!curr) return; 

  
    clear(curr->NW);
    clear(curr->NE);
    clear(curr->SW);
    clear(curr->SE);
  

  delete curr; 
  curr = nullptr;
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // Your code here.
  clear(root);
}

SQtree::Node * SQtree::copy(Node * other) {
  if (!other) return nullptr; 

  Node* curr = new Node(other->upLeft, other->width, other->height, other->avg, other->var);
  
  
    curr->NW = copy(other->NW);
    curr->NE = copy(other->NE);
    curr->SW = copy(other->SW);
    curr->SE = copy(other->SE);

  return curr; 
}

void SQtree::copy(const SQtree & other) {
  // Your code here.
  root = copy(other.root);
}


int SQtree::size(Node* curr) {
  if (!curr) return 0; 

  return 1 + size(curr->NW) + size(curr->NE) + size(curr->SW) + size(curr->SE);
}

int SQtree::size() {
  // Your code here.
  return size(root);
}