#include "object.h"

Object::Object(int x, int y,int priority)
{
    xPos = x;
    yPos = y;
    prio = priority;
}
Object::Object(){

}

int Object::getxPos() const{
    return xPos;
}
//current node y Position
int Object::getyPos() const{
    return yPos;
}

int Object::getPriority() const{
    return prio;
}
