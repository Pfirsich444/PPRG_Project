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

bool bellmanFord(const vector<edge> edges, array<node, 4> &Nodes)
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
    
    // nach dem Beenden der obigen for Schleife sollte überall der kürzeste Pfad gespeichert sein.
    // wenn es bei der folgenden Überprüfung noch einen kürzeren Pfad gibt, liegen negative cycles vor.
    for (auto &edge : edges)
    {
        if(Nodes[edge.startingNode].value + edge.weight < Nodes[edge.endingNode].value)
        {
            return false;
        }
    }
    
    return true;
}

int main(int argc, char** argv) {

    vector<edge> Edges;
    int numberOfNodes;
    int numberOfEdges;
    int minWeight;
    int maxWeight;
    
    cout << "Enter number of nodes: " << endl;
    cin >> numberOfNodes;
    
    cout << "Enter number of edges: " << endl;
    cin >> numberOfEdges;
    
    cout << "Enter minimum weight: " << endl;
    cin >> minWeight;
    
    cout << "Enter maximum weight: " << endl;
    cin >> maxWeight;
    
    
    
    // simple graph example
    /*Edges.push_back(edge{0,1,10});
    Edges.push_back(edge{0,5,8});
    Edges.push_back(edge{1,3,2});
    Edges.push_back(edge{2,1,1});
    Edges.push_back(edge{3,2,-2});
    Edges.push_back(edge{4,3,-1});
    Edges.push_back(edge{4,1,-4});
    Edges.push_back(edge{5,4,1});*/
    
    // negative cycles example
    Edges.push_back(edge{0,1,-20});
    Edges.push_back(edge{1,2,10});
    Edges.push_back(edge{2,1,-15});
    Edges.push_back(edge{2,0,5});
    
    array<node, 4> Nodes;
    
    for(int i=0;i<Nodes.size(); i++)
    {
        Nodes[i].name = i;
        Nodes[i].value = 1000000;
    }
    
    bool isSuccess = bellmanFord(Edges, Nodes);
    
    if(!isSuccess)
    {
        cout << "There are negative cycles!" << endl;
        return 0;
    }    
    
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

