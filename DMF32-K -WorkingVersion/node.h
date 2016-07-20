#ifndef NODE_H
 #define NODE_H

 class node
 {
 public:
     int xPos;
     int yPos;
     int distance;
     int priority;

     node();
     node(int, int,int,int);
     int getxPos() const;
     int getyPos() const;
     int getDistance() const;
     int getPriority() const;
     void updatePriority(const int&, const int&);
     void updateDistance();
     const int& estimateDistance(const int&, const int&)const;
};

 #endif // NODE_H
