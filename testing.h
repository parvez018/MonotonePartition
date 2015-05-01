#ifndef TESTING_H
#define TESTING_H

#include <set>

void drawChar(int x, int y, char *string)
{
    float r=0.8;
    float g=0.8;
    float b=0.8;
    //int font=GLUT_BITMAP_TIMES_ROMAN_10;
    //GLUT_BITMAP_TIMES_ROMAN_10
    glColor3f( r, g, b );
    glRasterPos2f(x, y);
    int len, i;
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        //printf("%d--",i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

void priority_queue_test()
{
    priority_queue<Vertex, vector<Vertex>, Vertex> pq;
    pq.push(Vertex(5,10));
    pq.push(Vertex(3,10));
    pq.push(Vertex(9,10));
    pq.push(Vertex(10,20));
    pq.push(Vertex(15,30));
    pq.push(Vertex(20,40));
    pq.push(Vertex(2,40));
    pq.push(Vertex(30,40));
    while(!pq.empty())
    {
        Vertex v=pq.top();
        printf("%lf,%lf\n",v.x,v.y);
        pq.pop();
    }
}

void stlSetTest()
{
    set<int> mySet;
    set<int>::iterator it;
    //printf("it->%d\n",*it);
    mySet.insert(10);
    mySet.insert(20);
    it=mySet.insert(30).first;
    printf("it->%d\n",*it);
    mySet.insert(40);
    printf("it->%d\n",*it);
    mySet.insert(50);
    printf("it->%d\n",*it);

    int d=31;
    int a = *mySet.lower_bound(d);
    printf("low=%d\n",a);
    a = *mySet.upper_bound(d);
    printf("high=%d\n",a);
}



#endif // TESTING_H
