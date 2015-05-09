#ifndef MONOTONE_H
#define MONOTONE_H

#include <cstdio>
#include <vector>
#include <queue>
#include <set>

#define TOTAL_POINT 15
#define START_VERTEX 0
#define END_VERTEX 1
#define SPLIT_VERTEX 2
#define MERGE_VERTEX 3
#define REGULAR_VERTEX 4

using namespace std;

#include "pcustomtypes.h"
#include "testing.h"

class MonotonePartition
{
private:
    priority_queue<Vertex, vector<Vertex>, Vertex> pq;
    set<BSTedges> bst;
    set<BSTedges>::iterator it;
    pair<std::set<BSTedges>::iterator,bool> ret;
    vector<Edges> diagonals;
    vector<Edges> edgeList;
    vector<set<BSTedges>::iterator> edgePointer;
    vector<double> edgeKeyVals;
    int totalPoint;
    vector<Vertex> vertex;
    vector<int> helper;
    void takeInput();
    void drawOriginalPolygon();
    void drawDiagonals();
    void makeMonotone();
    int getVertexType(int id);
    void vertexHandler(int vertexId, int vertexType, double y);
    void printBst();
public:
    MonotonePartition();
    void draw();
};

void MonotonePartition :: printBst()
{
    printf("bst now >>> ");
    set<BSTedges>::iterator itt;
    for(itt=bst.begin(); itt!=bst.end(); itt++)
    {
        printf("%d,%3.2lf ",(*itt).index,(*itt).keyVal);
    }
    printf("\n\n");
}
MonotonePartition :: MonotonePartition()
{
    takeInput();
    makeMonotone();
}
void MonotonePartition :: takeInput()
{
    FILE *ifp;
    ifp = fopen("I:\\Dropbox\\open gl\\Monotone Partition\\input.txt","r");//for Desktop

    fscanf(ifp,"%d",&totalPoint);
    double a,b;
    for(int i=0; i<totalPoint; i++)
    {
        fscanf(ifp,"%lf%lf",&a,&b);
        vertex.push_back(Vertex(i,a,b));
        helper.push_back(-1);
        edgeKeyVals.push_back(-1);
    }

/// list of edges -> 1,0 - 2,1 - ... - 0,last
    edgeList.push_back(Edges(vertex[0],vertex[totalPoint-1]));
    edgePointer.push_back(it);
    for(int i=0; i<totalPoint-1; i++)
    {
        edgeList.push_back(Edges(vertex[i+1],vertex[i]));
        edgePointer.push_back(it);
    }
    fclose(ifp);
}
void MonotonePartition :: drawDiagonals()
{
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    {
        for(int i=0; i<diagonals.size(); i++)
        {
            glVertex2f(diagonals[i].a.x,diagonals[i].a.y);
            glVertex2f(diagonals[i].b.x,diagonals[i].b.y);
        }
    }
    glEnd();
}
void MonotonePartition :: drawOriginalPolygon()
{
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINE_LOOP);
    {
        for(unsigned int i=0; i<vertex.size(); i++)
        {
            glVertex2f(vertex[i].x,vertex[i].y);
        }
    }
    glEnd();

    char num[4];

    for(int i=0; i<vertex.size(); i++)
    {
        itoa(i,num,10);
        drawChar(vertex[i].x,vertex[i].y,num);
    }
}

int MonotonePartition :: getVertexType(int id)
{
    int prev,next;
    if(id>0)
        prev = id-1;
    else
        prev = totalPoint-1;
    if(id<(totalPoint-1))
        next = id+1;
    else
        next = 0;

    int vertexType = REGULAR_VERTEX;
    if(vertex[id]>vertex[prev] && vertex[id]>vertex[next])
    {
        //printf("vertex[id]>vertex[prev] && vertex[id]>vertex[next])\n");
        Vector3 prevVector(vertex[prev].x-vertex[id].x,vertex[prev].y-vertex[id].y,0);
        Vector3 nextVector(vertex[next].x-vertex[id].x,vertex[next].y-vertex[id].y,0);
        //Vector3 nextVector(v[next].x-v[id].x,v[next].y-v[id].y,0);
        if(prevVector.zcross(nextVector)<0)//internal angle greater than 180
        {
            vertexType=SPLIT_VERTEX;
        }
        else
        {
            vertexType=START_VERTEX;
        }
    }
    else if(vertex[id]<vertex[prev] && vertex[id]<vertex[next])
    {
        //printf("vertex[id]<vertex[prev] && vertex[id]<vertex[next]\n");
        /*
        Vector3 prevVector(v[prev].x-v[id].x,v[prev].y-v[id].y,0);
        Vector3 nextVector(v[next].x-v[id].x,v[next].y-v[id].y,0);
        */
        Vector3 prevVector(vertex[prev].x-vertex[id].x,vertex[prev].y-vertex[id].y,0);
        Vector3 nextVector(vertex[next].x-vertex[id].x,vertex[next].y-vertex[id].y,0);

        if(prevVector.zcross(nextVector)<0)//internal angle greater than 180
        {
            vertexType=MERGE_VERTEX;
        }
        else
        {
            vertexType=END_VERTEX;
        }
    }
    return vertexType;
}
void MonotonePartition :: makeMonotone()
{
    char typeToName[][10]= {"start","end","split","merge","regular"};
    int vertexType;
    //init priority queue
    diagonals.clear();
    bst.clear();
    for(unsigned int i=0; i<vertex.size(); i++)
    {
        pq.push(vertex[i]);
        //testing vertex types
        //printf("index = %d, type = %s\n",i,arr[getVertexType(i)-1]);
    }

    while(!pq.empty())
    {
        Vertex v=pq.top();
        pq.pop();

        printf("from makeMonotone: i=%d,x=%3.2lf,y=%3.2lf\n",v.index,v.x,v.y);
        vertexType = getVertexType(v.index);
        //printf("from makeMonotone: i=%d,x=%3.2lf,y=%3.2lf\n",v.index,v.x,v.y);
        vertexHandler(v.index,vertexType,v.y);
        //printf("index = %d, type = %s\n",v.index,typeToName[vertexType]);
        printf("\n");
        printBst();
    }
}

void MonotonePartition :: vertexHandler(int vertexId, int vertexType,double curY)
{
    // edges have high index as first point
    int prev,next;
    if(vertexId>0)
    {
        prev = vertexId-1;
    }
    else
    {
        prev = totalPoint-1;
    }
    if(vertexId==(totalPoint-1))
    {
        next=0;
    }
    else
    {
        next = vertexId+1;
    }
    if(vertexType==START_VERTEX)
    {
        printf("start vertex\n");
        it=bst.insert(BSTedges(vertexId,edgeList[vertexId].a.x)).first;

        //bst.insert(BSTedges(vertexId,edgeList[vertexId].a.x)).first;
        edgeKeyVals[vertexId]=edgeList[vertexId].a.x;
        edgePointer[vertexId]=it;
        helper[vertexId]=vertexId;

        //printf("inserted %d\nkey=%3.2lf\nhelper[%d]=%d\n",vertexId,edgeKeyVals[vertexId],vertexId,vertexId);
    }
    else if(vertexType==END_VERTEX)
    {
        printf("end vertex\n");
        if(getVertexType(helper[next])== MERGE_VERTEX)
        {
            diagonals.push_back(Edges(vertex[vertexId],vertex[helper[next]]));
        }
        bst.erase(edgePointer[next]);
    }
    else if(vertexType==SPLIT_VERTEX)
    {
        printf("split vertex\n");
        Edges edg;
        it = bst.lower_bound(BSTedges(vertex[vertexId].x));
        if(it==bst.end())it--;
        printf("found lower bound %d\n",(*it).index);
        diagonals.push_back(Edges(vertex[vertexId],vertex[helper[(*it).index]]));
        helper[(*it).index]=vertexId;
        printf("helper[%d]=%d\n",(*it).index,vertexId);
        //it=bst.insert(BSTedges(edgeList[vertexId],vertexId,curY)).first;
        it=bst.insert(BSTedges(vertexId,edgeList[vertexId].getHorzDistnc(curY))).first;
        edgeKeyVals[vertexId]=(*it).keyVal;
        edgePointer[vertexId]=it;

        helper[vertexId]=vertexId;
        printf("y=%lf,id=%d\n",curY,(*it).index);
    }
    else if(vertexType==MERGE_VERTEX)
    {
        printf("merge vertex\n");
        if(getVertexType(helper[next])== MERGE_VERTEX)
        {
            diagonals.push_back(Edges(vertex[vertexId],vertex[helper[next]]));
        }
        //printf("deleting %d,size %d\n",next,bst.size());
        bst.erase(edgePointer[next]);
        printf("deleted %d\n",next);

        it = bst.lower_bound(BSTedges(vertex[vertexId].x));
        //printf("found lower bound %d\n",(*it).index);
        //it--;
        if(it==bst.begin())
        {
            //printf("bst begin\n");
            //it++;
            //printf("lower bound not found\n");
        }
        else if (it==bst.end())
        {
            //printf("bst end\n");
            it--;
        }
        printf("found lower bound %d\n",(*it).index);
        if(getVertexType(helper[(*it).index]) == MERGE_VERTEX)
        {
            printf("helper merge vertex\n");
            diagonals.push_back(Edges(vertex[vertexId],vertex[helper[(*it).index]]));
        }
        helper[(*it).index]=vertexId;
        printf("helper[%d]=%d\n",(*it).index,vertexId);
    }
    else if(vertexType==REGULAR_VERTEX)
    {
        printf("regular vertex\n");
        if(vertex[vertexId].y<vertex[next].y)//interior of the polygon to the right of vertex
        {
            printf("inside if\n");
            if(getVertexType(helper[next])== MERGE_VERTEX)
            {
                diagonals.push_back(Edges(vertex[vertexId],vertex[helper[next]]));
                printf("helper[%d] merger vertex\n",next);
                printf("added diagonal from %d to %d\n",vertexId,helper[next]);
            }
            printf("erasing edge%d\n",next);
            bst.erase(edgePointer[next]);
            //printf("after erase\n");

            //it=bst.insert(BSTedges(edgeList[vertexId],vertexId,curY)).first;
            it=bst.insert(BSTedges(vertexId,edgeList[vertexId].getHorzDistnc(curY))).first;
            edgePointer[vertexId]=it;
            helper[vertexId]=vertexId;
            printf("helper%d=%d\n",vertexId,vertexId);
        }
        else//interior to the left
        {
            printf("inside else\n");
            it = bst.lower_bound(BSTedges(vertex[vertexId].x));
            if(getVertexType(helper[(*it).index]) == MERGE_VERTEX)
            {
                diagonals.push_back(Edges(vertex[vertexId],vertex[helper[(*it).index]]));
            }
            helper[(*it).index]=vertexId;
        }
    }
}

void MonotonePartition :: draw()
{
    drawOriginalPolygon();
    drawDiagonals();
}
#endif // MONOTONE_H
