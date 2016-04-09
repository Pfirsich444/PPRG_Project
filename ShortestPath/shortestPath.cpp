/* 
 * File:   main.cpp
 * Author: Alex
 *
 * Created on 9. April 2016, 08:55
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <limits.h>
#include <algorithm>
#include <omp.h>
#include <ctime>
#include <chrono>

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
    long long value;
    vector<node> passedNodes;
};

bool bellmanFord(const vector<edge> edges, node* Nodes, int numberOfNodes)
{
    Nodes[0].value = 0;
    
    for(int i = 0; i < numberOfNodes-1; i++)
    {
        for(int j = 0; j < edges.size(); j++)
        {
            /*if(Nodes[edges.at(j).startingNode].value >= ULLONG_MAX - abs(edges.at(j).weight))
            {
                cout << "value reaches infinity: " << Nodes[edges.at(j).startingNode].value << endl;
                cout << "kljlkj: " << ULLONG_MAX - abs(edges.at(j).weight) << endl;
                
                bool someBool = (Nodes[edges.at(j).startingNode].value >= ULLONG_MAX - abs(edges.at(j).weight));
                cout << "bool: " << someBool << endl;
                return false;
            }*/
            if(Nodes[edges.at(j).startingNode].value + edges.at(j).weight < Nodes[edges.at(j).endingNode].value && edges.at(j).endingNode != 0)
            {
                Nodes[edges.at(j).endingNode].value = Nodes[edges.at(j).startingNode].value + edges.at(j).weight;
                
                // only for small graphs for demonstration
                //Nodes[edges.at(j).endingNode].passedNodes.clear();
                //Nodes[edges.at(j).endingNode].passedNodes = Nodes[edges.at(j).startingNode].passedNodes;
                //Nodes[edges.at(j).endingNode].passedNodes.push_back(Nodes[edges.at(j).startingNode]);
            }
            //cout << "Starting node: " << edges.at(j).startingNode << " i: " << i << endl;
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

bool bellmanFordP(const vector<edge> edges, node* Nodes, int numberOfNodes)
{
    Nodes[0].value = 0;
    
    for(int i = 0; i < numberOfNodes-1; i++)
    {
#pragma omp parallel for
        for(int j = 0; j < edges.size(); j++)
        {
            /*if(Nodes[edges.at(j).startingNode].value >= ULLONG_MAX - abs(edges.at(j).weight))
            {
                cout << "value reaches infinity: " << Nodes[edges.at(j).startingNode].value << endl;
                cout << "kljlkj: " << ULLONG_MAX - abs(edges.at(j).weight) << endl;
                
                bool someBool = (Nodes[edges.at(j).startingNode].value >= ULLONG_MAX - abs(edges.at(j).weight));
                cout << "bool: " << someBool << endl;
                return false;
            }*/
            if(Nodes[edges.at(j).startingNode].value + edges.at(j).weight < Nodes[edges.at(j).endingNode].value && edges.at(j).endingNode != 0)
            {
                Nodes[edges.at(j).endingNode].value = Nodes[edges.at(j).startingNode].value + edges.at(j).weight;
                
                // only for small graphs for demonstration
                //Nodes[edges.at(j).endingNode].passedNodes.clear();
                //Nodes[edges.at(j).endingNode].passedNodes = Nodes[edges.at(j).startingNode].passedNodes;
                //Nodes[edges.at(j).endingNode].passedNodes.push_back(Nodes[edges.at(j).startingNode]);
            }
            //cout << "Starting node: " << edges.at(j).startingNode << " i: " << i << endl;
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

bool checkIfExisting(const vector<edge> edges, int startingNode, int endingNode)
{
    for (auto &edge : edges)
    {
        if(edge.startingNode == startingNode && edge.endingNode == endingNode)
            return true;
    }
    return false;
}

vector<edge> generateGraph(int numberOfNodes, int numberOfEdges, int minWeight, int maxWeight)
{
    vector<edge> Edges;
    
    int i = 0;
    srand(time(0));
    while(i < numberOfEdges)
    {
        int startingNode = rand() % numberOfNodes;
        int endingNode = startingNode;
        while(startingNode == endingNode)
        {
            endingNode  = rand() % numberOfNodes;
        }
        
        auto isExisting = checkIfExisting(Edges, startingNode, endingNode);
        
        if(isExisting)
        {
            continue;
        }
        
        int weight = minWeight + (rand() % (maxWeight - minWeight +1));
        Edges.push_back(edge{startingNode, endingNode, weight});
        cout << "Start: " << startingNode << ", End: " << endingNode << ", Weight: " << weight << endl;
        i++;
    }
    
    return Edges;
}

int main(int argc, char** argv) {

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
    
    
    
    auto Edges = generateGraph(numberOfNodes, numberOfEdges, minWeight, maxWeight);
    
    // simple graph example
    /*
    vector<edge> Edges;
    Edges.push_back(edge{0,1,10});
    Edges.push_back(edge{0,5,8});
    Edges.push_back(edge{1,3,2});
    Edges.push_back(edge{2,1,1});
    Edges.push_back(edge{3,2,-2});
    Edges.push_back(edge{4,3,-1});
    Edges.push_back(edge{4,1,-4});
    Edges.push_back(edge{5,4,1});*/
    
    // negative cycles example
    /*vector<edge> Edges;
    Edges.push_back(edge{0,1,-20});
    Edges.push_back(edge{1,2,10});
    Edges.push_back(edge{2,1,-15});
    Edges.push_back(edge{2,0,5});*/
    
    //array<node, 4> Nodes;
    
    node* Nodes = new node[numberOfNodes];
    node* NodesP = new node[numberOfNodes];
    
    for(int i=0;i<numberOfNodes; i++)
    {
        Nodes[i].name = i;
        Nodes[i].value = LLONG_MAX - maxWeight -1;
        NodesP[i].name = i;
        NodesP[i].value = LLONG_MAX - maxWeight -1;
    }
    
    chrono::time_point<chrono::system_clock> start;
    start = chrono::system_clock::now();
    cout << "-----Start-----" << endl;
    bool isSuccess = bellmanFord(Edges, Nodes, numberOfNodes);
    if(!isSuccess)
    {
        cout << "There are negative cycles!" << endl;
        return 0;
    }    
    
    /*for(int i = 0; i < numberOfNodes; i++)
    {
        cout << Nodes[i].name << " : " << Nodes[i].value << "   ";
        
        Nodes[i].passedNodes.push_back(Nodes[i]);
        
        for (std::vector<node>::const_iterator j = Nodes[i].passedNodes.begin(); j != Nodes[i].passedNodes.end(); ++j)
        {
            cout << j->name;
                        
            if((j != Nodes[i].passedNodes.end()) && (next(j) == Nodes[i].passedNodes.end()))
            {
                break;
            }
            cout << " -> ";
        }
        
        cout << endl;
    }*/
    chrono::duration<double> elapsed_seconds = chrono::system_clock::now()-start;
    cout << "-----Time: " << elapsed_seconds.count() << "-----" << endl;
    
    ofstream myfile;
    myfile.open ("seriell.txt");
    for(int i = 0; i < numberOfNodes; i++)
    {
        myfile << Nodes[i].name << " : " << Nodes[i].value << "   " << endl;
    }
    myfile.close();
    
    start = chrono::system_clock::now();
    cout << "-----Start-----" << endl;
    bool isSuccessP = bellmanFordP(Edges, NodesP, numberOfNodes);
    if(!isSuccessP)
    {
        cout << "There are negative cycles!" << endl;
        return 0;
    }    
    
    /*for(int i = 0; i < numberOfNodes; i++)
    {
        cout << NodesP[i].name << " : " << NodesP[i].value << "   ";
        
        NodesP[i].passedNodes.push_back(NodesP[i]);
        
        for (std::vector<node>::const_iterator j = NodesP[i].passedNodes.begin(); j != NodesP[i].passedNodes.end(); ++j)
        {
            cout << j->name;
                        
            if((j != NodesP[i].passedNodes.end()) && (next(j) == NodesP[i].passedNodes.end()))
            {
                break;
            }
            cout << " -> ";
        }
        
        cout << endl;
    }*/
    elapsed_seconds = chrono::system_clock::now()-start;
    cout << "-----Time: " << elapsed_seconds.count() << "-----" << endl;
    
    ofstream myfile2;
    myfile2.open ("parallel.txt");
    for(int i = 0; i < numberOfNodes; i++)
    {
        myfile2 << NodesP[i].name << " : " << NodesP[i].value << "   " << endl;
    }
    myfile2.close();
    
    /*if(!isSuccess)
    {
        cout << "There are negative cycles!" << endl;
        return 0;
    }    
    
    for(int i = 0; i < numberOfNodes; i++)
    {
        cout << Nodes[i].name << " : " << Nodes[i].value << "   ";
        
        Nodes[i].passedNodes.push_back(Nodes[i]);
        
        for (std::vector<node>::const_iterator j = Nodes[i].passedNodes.begin(); j != Nodes[i].passedNodes.end(); ++j)
        {
            cout << j->name;
                        
            if((j != Nodes[i].passedNodes.end()) && (next(j) == Nodes[i].passedNodes.end()))
            {
                break;
            }
            cout << " -> ";
        }
        
        cout << endl;
    }*/
    
    
    
    delete[] Nodes;
    delete[] NodesP;
    return 0;
}

