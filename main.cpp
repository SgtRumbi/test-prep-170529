//! gcc -o main main.cpp | ./main
/*
 * Preparation for a test in school.
 * (School-Code is in Java, but idk, I prefer C++, because it
 * is easier for me to write)
 *
 * This program implements a depth-first-search (DFS), a breadth-first-search (BFS)
 * and a Dijkstra-search non-recursive.
 *
 * (This project can be compiled (since it uses only one translation-unit) using the standard
 * gcc, g++, clang or other compiler; or using the provided CMakeLists.txt)
 */

#include <cstdlib>
#include <cstdio>
#include <stack>
#include <queue>
#include <climits>

// Definitions
typedef unsigned int uint32;
typedef signed int int32;

typedef unsigned long int uint64;
typedef signed long int int64;

typedef int32 bool32;
typedef bool32 b32;

#define internal static
#define local_persist static
#define global_variable static

#define Assert(Expression) if(Expression) {printf("Expression '%s' in line %d failed!\n", #Expression, __LINE__); exit(0);}
#define Print(...) printf(__VA_ARGS__)

// TODO(js): Adjust to something realistic
#define INFINITY INT_MAX

struct node {
    const char *Name;
    int32 OwnIndex;
};

struct graph {
    node *Nodes;
    int32 NodeCount;
    int32 **Matrix;
};

internal graph
CreateGraph(int32 NodeCount) {
    graph Result = {};

    Result.Nodes = new node[NodeCount];
    Result.Matrix = new int32 *[NodeCount];
    for(int32 NodeIndex = 0;
        NodeIndex < NodeCount;
        ++NodeIndex) {
        Result.Matrix[NodeIndex] = new int32[NodeCount];
    }
    Result.NodeCount = NodeCount;

    return(Result);
}

internal void
LogSquareMatrix(int32 **Matrix, int32 ElementsCount) {
    for(int32 Vertical = 0;
        Vertical < ElementsCount;
        ++Vertical) {
        for(int32 Horizontal = 0;
            Horizontal < ElementsCount;
            ++Horizontal) {
            Print("%d ", Matrix[Vertical][Horizontal]);
        }
        Print("\n");
    }
}

internal node *
InsertNode(graph *Graph, const char *Name, int32 Index) {
    node Node = {};
    Node.Name = Name;
    Node.OwnIndex = Index;

    Graph->Nodes[Index] = Node;

    // TODO(js): Is this ok? Should this really return a pointer to an array-element? (Unsafe)
    return(&Graph->Nodes[Index]);
}

internal void
InsertEdgeDirectional(graph *Graph, int32 SourceIndex, int32 DestinationIndex, int32 Weight) {
    Graph->Matrix[SourceIndex][DestinationIndex] = Weight;
}

internal void
InsertEdge(graph *Graph, node *A, node *B, int32 Weight) {
    int32 AIndex = A->OwnIndex;
    int32 BIndex = B->OwnIndex;

    InsertEdgeDirectional(Graph, AIndex, BIndex, Weight);
    InsertEdgeDirectional(Graph, BIndex, AIndex, Weight);
}

internal void
LogConnections(graph Graph) {
    for(int32 NodeAIndex = 0;
        NodeAIndex < Graph.NodeCount;
        ++NodeAIndex) {
        for(int32 NodeBIndex = 0;
            NodeBIndex < Graph.NodeCount;
            ++NodeBIndex) {
            if(Graph.Matrix[NodeAIndex][NodeBIndex] > 0) {
                node NodeA = Graph.Nodes[NodeAIndex];
                node NodeB = Graph.Nodes[NodeBIndex];
                Print("Connection between %s(%d) and %s(%d) weights %d.\n", NodeA.Name, NodeAIndex, NodeB.Name, NodeBIndex,
                      Graph.Matrix[NodeAIndex][NodeBIndex]);
            }
        }
    }
}

internal void
DepthFirstSearch(graph Graph, node *StartNode) {
    Print("Starting Depth-First-Search from %s(%d).\n", StartNode->Name, StartNode->OwnIndex);

    int32 StartIndex = StartNode->OwnIndex;

    std::stack<int32> Stack;
    bool32 Visited[Graph.NodeCount];
    for(int32 I = 0;
        I < Graph.NodeCount;
        ++I) {
        Visited[I] = false;
    }

    Stack.push(StartIndex);
    Visited[StartIndex] = true;

    while(!Stack.empty()) {
        int32 Current = Stack.top();
        Stack.pop();

        for(int32 NodeToVisit = 0;
            NodeToVisit < Graph.NodeCount;
            ++NodeToVisit) {
            if(!Visited[NodeToVisit] && Graph.Matrix[Current][NodeToVisit]) {
                Visited[NodeToVisit] = true;
                Stack.push(NodeToVisit);

                node Node = Graph.Nodes[NodeToVisit];

                Print("Visited node %s(%d)\n", Node.Name, Node.OwnIndex);
            }
        }
    }

    Print("Finished Depth-First-Search.\n");
}

internal void
BreadthFirstSearch(graph Graph, node *StartNode) {
    Print("Starting Breadth-First-Search from %s(%d).\n", StartNode->Name, StartNode->OwnIndex);

    int32 StartIndex = StartNode->OwnIndex;

    std::queue<int32> Queue;
    bool32 Visited[Graph.NodeCount];
    for(int32 I = 0;
        I < Graph.NodeCount;
        ++I) {
        Visited[I] = false;
    }

    Queue.push(StartIndex);
    Visited[StartIndex] = true;

    while(!Queue.empty()) {
        int32 Current = Queue.front();
        Queue.pop();

        for(int32 NodeToVisit = 0;
            NodeToVisit < Graph.NodeCount;
            ++NodeToVisit) {
            if(!Visited[NodeToVisit] && Graph.Matrix[Current][NodeToVisit]) {
                Visited[NodeToVisit] = true;
                Queue.push(NodeToVisit);

                node Node = Graph.Nodes[NodeToVisit];

                Print("Visited node %s(%d)\n", Node.Name, Node.OwnIndex);
            }
        }
    }

    Print("Finished Breadth-First-Search.\n");
}

internal void
Dijkstra(graph Graph, node *StartNode, node *EndNode) {
    Print("Starting Dijkstra-way-search from %s(%d) to %s(%d).\n",
          StartNode->Name, StartNode->OwnIndex, EndNode->Name, EndNode->OwnIndex);

    int32 StartNodeIndex = StartNode->OwnIndex;
    int32 EndNodeIndex = EndNode->OwnIndex;

    // Initialisation
    int32 Distances[Graph.NodeCount];
    for(int32 DistanceIndex = 0;
        DistanceIndex < Graph.NodeCount;
        ++DistanceIndex) {
        Distances[DistanceIndex] = INFINITY;
    }
    Distances[StartNodeIndex] = 0;

    int32 Previous[Graph.NodeCount];
    for(int32 IndexOfPrevious = 0;
        IndexOfPrevious < Graph.NodeCount;
        ++IndexOfPrevious) {
        Previous[IndexOfPrevious] = -1;
    }
    Previous[StartNodeIndex] = StartNodeIndex;

    bool32 Visited[Graph.NodeCount];
    for(int32 I = 0;
        I < Graph.NodeCount;
        ++I) {
        Visited[I] = false;
    }

    // Algorithm
    for(int32 NodeIndex = 0;
        NodeIndex < Graph.NodeCount;
        ++NodeIndex) {
        int32 CurrentMinDistance = INFINITY;
        int32 CurrentMinNodeIndex = -1;

        // Get nearest node
        for(int32 NodeToCheck = 0;
            NodeToCheck < Graph.NodeCount;
            ++NodeToCheck) {
            int32 CurrentDistance = Distances[NodeToCheck];

            Print("Current distance between %s(%d) and %s(%d) is %d.\n", Graph.Nodes[NodeIndex].Name, NodeIndex,
                  Graph.Nodes[NodeToCheck].Name, NodeToCheck, CurrentDistance);
            if(!Visited[NodeToCheck] && (CurrentDistance < CurrentMinDistance)) {
                CurrentMinDistance = CurrentDistance;
                CurrentMinNodeIndex = NodeToCheck;
            }
        }

        Visited[CurrentMinNodeIndex] = true;

        for(int32 NodeIterator = 0;
            NodeIterator < Graph.NodeCount;
            ++NodeIterator) {
            if(!Visited[NodeIterator] && Graph.Matrix[CurrentMinNodeIndex][NodeIterator]) {
                int32 NewDistance = Distances[CurrentMinNodeIndex] + Graph.Matrix[CurrentMinNodeIndex][NodeIterator];
                if(NewDistance < Distances[NodeIterator]) {
                    Distances[NodeIterator] = NewDistance;
                    Previous[NodeIterator] = CurrentMinNodeIndex;
                }
            }
        }
    }

    // Evaluation
    for(int NodeIterator = 0;
        NodeIterator < Graph.NodeCount;
        ++NodeIterator) {
        Print("Distance from %s(%d) to %s(%d) is %d.\n", StartNode->Name, StartNodeIndex,
              Graph.Nodes[NodeIterator].Name, NodeIterator, Distances[NodeIterator]);
        if(Previous[NodeIterator] != -1) {
            Print("%s(%d)'s predecessor is %s(%d).\n",
                  Graph.Nodes[NodeIterator].Name, NodeIterator, Graph.Nodes[Previous[NodeIterator]].Name,
                  Previous[NodeIterator]);
        } else {
            Print("%s(%d) does not have a predecessor.\n", Graph.Nodes[NodeIterator].Name, NodeIterator);
        }
    }
    Print("Starting at end node: %s(%d)", Graph.Nodes[EndNodeIndex].Name, EndNodeIndex);
    int32 CurrentNodeIndex = EndNodeIndex;
    int32 Sum = 0;
    while(CurrentNodeIndex != StartNodeIndex) {
        Sum += Graph.Matrix[CurrentNodeIndex][Previous[CurrentNodeIndex]];
        CurrentNodeIndex = Previous[CurrentNodeIndex];
        Print("<-%s(%d)", Graph.Nodes[CurrentNodeIndex].Name, CurrentNodeIndex);
    }
    Print("\nDistance to target: %d / %d\n", Distances[EndNodeIndex], Sum);
    Print("Finished Dijkstra-way-search.");
}

internal void
Line() {
    Print("\n");
}

int
main() {
    const int32 NodeCount = 6;
    graph Graph = CreateGraph(NodeCount);

    node *A = InsertNode(&Graph, "A", 0);
    node *B = InsertNode(&Graph, "B", 1);
    node *C = InsertNode(&Graph, "C", 2);
    node *D = InsertNode(&Graph, "D", 3);
    node *E = InsertNode(&Graph, "E", 4);
    node *F = InsertNode(&Graph, "F", 5);

    InsertEdge(&Graph, A, B, 3);
    InsertEdge(&Graph, A, C, 4);
    InsertEdge(&Graph, A, F, 9);

    InsertEdge(&Graph, B, C, 2);
    InsertEdge(&Graph, B, D, 1);
    InsertEdge(&Graph, B, E, 4);
    InsertEdge(&Graph, B, F, 4);

    InsertEdge(&Graph, C, F, 7);

    InsertEdge(&Graph, D, E, 5);
    InsertEdge(&Graph, D, F, 6);

    InsertEdge(&Graph, E, F, 8);

    LogSquareMatrix(Graph.Matrix, Graph.NodeCount);
    LogConnections(Graph);

    Line();
    BreadthFirstSearch(Graph, B);
    Line();
    DepthFirstSearch(Graph, B);

    Line();

    Dijkstra(Graph, C, E);

    return(0);
}
