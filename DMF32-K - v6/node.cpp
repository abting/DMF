#include "node.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * default constructor
 */
node::node(){
}

/*
 * constructor
 */
node::node(int x,int y,int d, int p){
    xPos = x;
    yPos = y;
    distance = d;
    priority = p;
}

/*
 * getters
 */
//current node x Position
int node::getxPos() const{
    return xPos;
}
//current node y Position
int node::getyPos() const{
    return yPos;
}
/*
 * g value
 * The g value, in our case, should always increment by 1 since there are no
 * diagonal movements
 */
int node::getDistance() const{
    return distance;
}
/*
 * f value
 * The f value determines the priority at which the code should check
 * a path for.
 *
 * The lower the f value, the higher the priority
 */
int node::getPriority() const{
    return priority;
}
/*
 * updates the f value
 * For evaluating priority
 */
void node::updatePriority(const int & xDest,const int & yDest){
    priority =(estimateDistance(xDest,yDest)*10) + distance;
}

//the g value always increments by 10 (vertical and horizontal movements only)
void node::updateDistance(){
    distance +=10;
}
/*
 * updates the h value, the estimate distance from the final destination
 */
const int & node::estimateDistance(const int & xDest, const int & yDest)const{
    static int xDistance, yDistance, totalDistance;
    xDistance = xDest - xPos;
    yDistance = yDest - yPos;
    totalDistance = abs(xDistance)+abs(yDistance);

    return(totalDistance);
}
