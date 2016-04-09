/* 
 * File:   main.cpp
 * Author: Alex
 *
 * Created on 9. April 2016, 08:55
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <functional>
#include <limits.h>

using namespace std;

struct edge 
{
    int startingNode;
    int endingNode;
    int weight;
};

struct node
{
    int name;
    int value;
    vector<node> passedNodes;
};

void bellmanFord(const vector<edge> edges, array<node, 6> &Nodes)
{
    Nodes[0].value = 0;
    
    for(int i = 0; i < Nodes.size()-1; i++)
    {
        for (auto &edge : edges)
        {
            if(Nodes[edge.startingNode].value + edge.weight < Nodes[edge.endingNode].value)
            {
                Nodes[edge.endingNode].value = Nodes[edge.startingNode].value + edge.weight;
                
                Nodes[edge.endingNode].passedNodes.clear();
                Nodes[edge.endingNode].passedNodes = Nodes[edge.startingNode].passedNodes;
                Nodes[edge.endingNode].passedNodes.push_back(Nodes[edge.startingNode]);
            }
        }
        
    }
    
}

int main(int argc, char** argv) {

    vector<edge> Edges;
    Edges.push_back(edge{0,1,10});
    Edges.push_back(edge{0,5,8});
    Edges.push_back(edge{1,3,2});
    Edges.push_back(edge{2,1,1});
    Edges.push_back(edge{3,2,-2});
    Edges.push_back(edge{4,3,-1});
    Edges.push_back(edge{4,1,-4});
    Edges.push_back(edge{5,4,1});
    
    array<node, 6> Nodes;
    
    for(int i=0;i<Nodes.size(); i++)
    {
        Nodes[i].name = i;
        Nodes[i].value = 1000000;
    }
    
    bellmanFord(Edges, Nodes);
    
    for(int i = 0; i < Nodes.size(); i++)
    {
        cout << Nodes[i].name << " : " << Nodes[i].value << "   ";
        
        Nodes[i].passedNodes.push_back(Nodes[i]);
        
        for (std::vector<node>::const_iterator j = Nodes[i].passedNodes.begin(); j != Nodes[i].passedNodes.end(); ++j)
        {
            std::cout << j->name;
            
            if((j != Nodes[i].passedNodes.end()) && (next(j) == Nodes[i].passedNodes.end()))
            {
                break;
            }
            std::cout << " -> ";
        }
        
        cout << endl;
    }
    
    
    return 0;
}

