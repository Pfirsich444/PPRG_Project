/* 
 * File:   main.cpp
 * Author: Alexander Schmid, Angelika Kadnar
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

bool bellmanFord(const vector<edge> edges, node* Nodes, int numberOfNodes, bool isPathTracked)
{
    Nodes[0].value = 0;
    
    for(int i = 0; i < numberOfNodes-1; i++)
    {
        for(int j = 0; j < edges.size(); j++)
        {
            if(Nodes[edges.at(j).startingNode].value + edges.at(j).weight < Nodes[edges.at(j).endingNode].value && edges.at(j).endingNode != 0)
            {
                Nodes[edges.at(j).endingNode].value = Nodes[edges.at(j).startingNode].value + edges.at(j).weight;
                
                // only for small graphs for demonstration
                if(isPathTracked)
                {
                    Nodes[edges.at(j).endingNode].passedNodes.clear();
                    Nodes[edges.at(j).endingNode].passedNodes = Nodes[edges.at(j).startingNode].passedNodes;
                    Nodes[edges.at(j).endingNode].passedNodes.push_back(Nodes[edges.at(j).startingNode]);
                }
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

bool bellmanFordP(const vector<edge> edges, node* Nodes, int numberOfNodes, bool isPathTracked)
{
    Nodes[0].value = 0;
    
    for(int i = 0; i < numberOfNodes-1; i++)
    {
#pragma omp parallel for schedule(dynamic, 100)
        for(int j = 0; j < edges.size(); j++)
        {
            if(Nodes[edges.at(j).startingNode].value + edges.at(j).weight < Nodes[edges.at(j).endingNode].value && edges.at(j).endingNode != 0)
            {
                Nodes[edges.at(j).endingNode].value = Nodes[edges.at(j).startingNode].value + edges.at(j).weight;
                
                // only for small graphs for demonstration
                if(isPathTracked)
                {
                    Nodes[edges.at(j).endingNode].passedNodes.clear();
                    Nodes[edges.at(j).endingNode].passedNodes = Nodes[edges.at(j).startingNode].passedNodes;
                    Nodes[edges.at(j).endingNode].passedNodes.push_back(Nodes[edges.at(j).startingNode]);
                }
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
    srand(time(0));
    
    for(int i = 0; i < numberOfEdges; i++)
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
            i--;
            continue;
        }
        
        int weight = minWeight + (rand() % (maxWeight - minWeight +1));
        Edges.push_back(edge{startingNode, endingNode, weight});
    }
    
    return Edges;
}

vector<edge> generateSimpleSample()
{
    // simple graph example
    vector<edge> Edges;
    Edges.push_back(edge{0,1,10});
    Edges.push_back(edge{0,5,8});
    Edges.push_back(edge{1,3,2});
    Edges.push_back(edge{2,1,1});
    Edges.push_back(edge{3,2,-2});
    Edges.push_back(edge{4,3,-1});
    Edges.push_back(edge{4,1,-4});
    Edges.push_back(edge{5,4,1});
    
    return Edges;
}

vector<edge> generateNegativeCycle()
{
    // negative cycles example
    vector<edge> Edges;
    Edges.push_back(edge{0,1,-20});
    Edges.push_back(edge{1,2,10});
    Edges.push_back(edge{2,1,-15});
    Edges.push_back(edge{2,0,5});
    
    return Edges;
}

void InitializeNodes(node* Nodes, node* NodesP, int numberOfNodes, int maxWeight)
{
    for(int i=0;i<numberOfNodes; i++)
    {
        Nodes[i].name = i;
        Nodes[i].value = LLONG_MAX - maxWeight -1;
        NodesP[i].name = i;
        NodesP[i].value = LLONG_MAX - maxWeight -1;
    }
}

chrono::time_point<chrono::system_clock> StartTimer(string startText)
{
    chrono::time_point<chrono::system_clock> start;
    cout << "-----Start " << startText << "-----" << endl;
    return chrono::system_clock::now();
}

void EndTimer(chrono::time_point<chrono::system_clock> start)
{
    chrono::duration<double> elapsed_seconds = chrono::system_clock::now()-start;
    cout << "-----Time: " << elapsed_seconds.count() << "-----" << endl;
}

void SaveNodesToFile(node* Nodes, int numberOfNodes, string filename)
{
    ofstream myfile;
    myfile.open (filename);
    for(int i = 0; i < numberOfNodes; i++)
    {
        myfile << Nodes[i].name << " : " << Nodes[i].value << "   \n";
    }
    myfile.close();
}

void PrintNodes(node* Nodes, int numberOfNodes)
{
    for(int i = 0; i < numberOfNodes; i++)
    {
        cout << "Node: ";
        cout << Nodes[i].name;
        cout << ", Value: ";
        cout << Nodes[i].value;
        cout << "   ";
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
    }
    cout << endl;
}

int main(int argc, char** argv) {

    int numberOfNodes;
    int numberOfEdges;
    int minWeight;
    int maxWeight = 0;
    int numberOfThreads;
    int mode;
    vector<edge> Edges;
    
    cout << "Press 1 for random graph generation, 2 for small example graph, 3 for negative cycles example" << endl;
    cin >> mode;
    
    if(mode !=2 && mode != 3)
    {
        cout << "Enter number of nodes: " << endl;
        cin >> numberOfNodes;

        cout << "Enter number of edges: " << endl;
        cin >> numberOfEdges;

        cout << "Enter minimum weight: " << endl;
        cin >> minWeight;

        cout << "Enter maximum weight: " << endl;
        cin >> maxWeight;
        
        Edges = generateGraph(numberOfNodes, numberOfEdges, minWeight, maxWeight);
    }
    
    cout << "Choose number of threads: " << endl;
    cin >> numberOfThreads;
    
    omp_set_num_threads(numberOfThreads);
    
    if(mode == 2)
    {
        Edges = generateSimpleSample();
        numberOfNodes = 6;
    }
    if(mode == 3)
    {
        Edges = generateNegativeCycle();
        numberOfNodes = 3;
    }
    
    node* Nodes = new node[numberOfNodes];
    node* NodesP = new node[numberOfNodes];
    
    InitializeNodes(Nodes, NodesP, numberOfNodes, maxWeight);

    // Seriell:
    auto start = StartTimer("seriell");
    bool isSuccess = bellmanFord(Edges, Nodes, numberOfNodes, mode != 1);
    
    EndTimer(start);
    SaveNodesToFile(Nodes, numberOfNodes, "seriell.txt");
    
    if(!isSuccess)
    {
        cout << "There are negative cycles!" << endl;
        return 0;
    }    
    
    // Parallel:
    auto startParallel = StartTimer("parallel");
    bellmanFordP(Edges, NodesP, numberOfNodes, mode != 1);
    
    EndTimer(startParallel);
    SaveNodesToFile(NodesP, numberOfNodes, "parallel.txt");
    
    if(mode != 1)
    {
        PrintNodes(Nodes, numberOfNodes);
        PrintNodes(NodesP, numberOfNodes);
    }
    
    delete[] Nodes;
    delete[] NodesP;
    return 0;
}

