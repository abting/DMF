#ifndef OBJECT_H
#define OBJECT_H


class Object
{
public:
    int xPos;
    int yPos;
    int prio;

    int getxPos() const;
    int getyPos() const;
    int getPriority() const;
    Object(int,int,int);
    Object();
};

#endif // OBJECT_H
