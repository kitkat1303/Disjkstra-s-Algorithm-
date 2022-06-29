/*
 * @file Graph.h
 * @author Katarina McGaughy
 * Graph class: The Graph class read in a graph from a file and
 * stores the graph's information in an adjacency list. The findShortestPath
 * function computes all shortes paths between all vertices via Dijkstras algorithm
 * and stores them in a 2D table. The class displays the graphs information along
 * with the shortest paths in varying ways.

 *
 * Features:
 * -stores a Graph read in from a file
 * -copy constructor
 * -assignment operator
 * -findShortesPath computes all shortest paths betweeen all
 * vertices and stores them
 * -displayAll prints out all of the graphs information
 * -display prints out path information between two vertices
 *
 * Assumptions:
 * -file is in correct format
 * -valid vertices are entered along with
 * valid weights (non negative)

 *
 * @version 0.1
 * @date 2022-2-3
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <iostream>
#include "Vertex.h"
#include <limits>
#include <queue>
#include <vector>

class Graph
{

private:

	//max number of vertices allowed in graph 
	static const int MAX_VERTICES = 101;

	//EdgeNode represents the edge between two vertices
	struct EdgeNode {
		int adjVertex = 0; // subscript of the adjacent vertex 
		int weight = 0; // weight of edge
		EdgeNode* nextEdge = nullptr;
	};

	//the source vertex that points to EdgeNodes
	struct VertexNode {
		EdgeNode* edgeHead = nullptr; // head of the list of edges
		Vertex* data = nullptr; // store vertex data here
	};

	// array of VertexNodes
	VertexNode vertices[MAX_VERTICES];

	// table of information for Dijkstra's algorithm
	struct Table {
		bool visited = false; // whether vertex has been visited
		int dist = numeric_limits<int>::max(); // shortest known distance from source
		int path = 0; // previous vertex in path of min dist
	};

	//stores information for Dijkstras Algorithm
	struct AdjacentVertex {
		int vertexIndex = -1;
		int dist = numeric_limits<int>::max();

	};

	//used to compare AdjacentVertices distances
	struct CompareAdjacentVertex {
		bool operator()(AdjacentVertex const& source, AdjacentVertex const& dest)
		{
			return source.dist > dest.dist;
		}
	};

	int size = 0; // number of vertices in the graph

	// stores visited, distance, path -
	// two dimensional in order to solve
	// for all sources
	Table T[MAX_VERTICES][MAX_VERTICES];

	/**
	* isValidVertex
	* this function takes in an int vertex
	* and returns if it is a valid vertex
	* (1-size)
	* Preconditons: none
	* Postconditions: returns true if the vertex is
	* between 1 and size
	*/
	bool isValidVertex(int vertex) const;

	/**
	* findEdge
	* this function takes in a source and dest vertex
	* and traverses the adjacency list of EdgeNodes in order
	* to find if the edge exists, if it does, the EdgeNode is
	* returned and if it does not, nullptr is returned
	* this function is called by insert
	* Precondtions: valid source must be entered
	* Postconditons: if edge is found, EdgeNode is returned, and if
	* it is not found, nullptr is returned
	* @param source: the source vertex
	* @param det: the destination vertex
	*/
	EdgeNode* findEdge(int source, int dest) const;

	/**
	 * deleteList
	 * this function deletes the adjacency list
	 * stored in the vertices array and calls
	 * deleteListHelper in order to traverse the
	 * EdgeNodes stored in the vertices array
	 * Preconditions: none
	 * Postconditios: adjacency list is deleted
	 */
	void deleteList();

	/**
	 * deleteListHelper
	 * this function recursively deletes the EdgeNodes
	 * passed in from the deleteList function
	 * Preconditions: none
	 * Postconditios: EdgeNode list is deleted
	 */
	void deleteListHelper(EdgeNode* node);

	/**
	 * copyGraph
	 * this function copies the vertices from copyGraph
	 * along with the EdgeNodes to the current vertices array
	 * via copyGraphHelper
	 * Preconditions: copyGraph must be initialized
	 * Postconditios: adjacency list from copyGraph is copied
	 * to current graph
	 * @param cGraph: Graph to be copied
	 */
	void copyGraph(const Graph& cGraph);

	/**
	 * copyGraphHelper
	 * this function copies the EdgeNodes from vertexCopy
	 * and returns the EdgeNode attached to adjacency list
	 * Preconditions: none
	 * Postconditios: returns the EdgeNode attached to adjacency list
	 * @param vertexCopy: EdgeNode to be copied
	 * @ return: returns the EdgeNode attached to adjacency list
	 */
	EdgeNode* copyGraphHelper(EdgeNode* vertexCopy);

	/**
	* resetTable
	* this function resets the Table T
	* Preconditons: none
	* Postconditions: the tables values are reset to
	* their initial values
	*/
	void resetTable();

	/**
	* dijkstrasAlgo
	* this function takes in a VertexNode along with the
	* vertexNum which represents the VertexNode's vertex position
	* and perform dijkstraAlgo on the sourceVertex in order to
	* find the shortest path from the sourceVertex to
	* all other vertices in the graph (which is stored in Table T)
	* Preconditons: sourceVertex must be valid vertex
	* Postconditions: the shortest path information
	* is stored in the Table T
	* @param sourceVertex: the source vertex for the algorithm
	* @param vertexNum: the number vertex of sourceVertex
	*/
	void dijkstrasAlgo(VertexNode sourceVertex, int vertexNum);

	/**
	* printPath
	* this function takes in a source and dest and prints
	* the shortest path starting from the
	* vertex before the dest vertex
	* Precondtion: source and dest must be valid vertices
	* Postcondition: the shortest path is printed, excluding the
	* dest vertex
	* @param source: the source vertex of path to be printed
	* @param dest: the destination vertex of path to be printed
	*/
	void printPath(int souce, int dest) const;

	/**
	* printDestination
	* this function takes in a source and dest and prints the
	* path from the source to the dest based on the names of the
	* vertices visited
	* Precondtion: source and dest must be valid vertices
	* Postcondition: information about the path from
	* source to dest is printed
	* @param source: the source vertex of path to be printed
	* @param dest: the destination vertex of path to be printed
	*/
	void printDestinations(int source, int dest) const;


public:

	/**
	 * constructor
	 * this function initializes a Graph and initializes
	 * the vertices array
	 * Preconditions: none
	 * Postconditios: vertices array is initialized
	 */
	Graph();

	/**
	 * copy constructor
	 * this function initializes a new Graph with the cGraph
	 * values via the copyGraphHelper function
	 * Preconditions: cGraph must be initialized
	 * Postconditios: a new Graph is initialized with the same values
	 * as the cGraph
	 * @param cGraph: Graph to be copied
	 */
	Graph(const Graph& cGraph);

	/**
	 * desctructor
	 * this function deletes the adjacency list
	 * stored in the vertices array by calling deleteList
	 * Preconditions: none
	 * Postconditios: adjacency list is deleted
	 */
	~Graph();

	/**
	 * operator=
	 * this function clears the current graph if it is not the same as cGraph
	 * and then assigns the cGraph data to the current graph via copygraphHelper()
	 * Preconditions: none
	 * Postconditios: a new Graph is initialized with the same values
	 * as the cGraph
	 * @param cGraph: Graph being assigned to current Graph
	 */
	const Graph operator=(const Graph& cGraph);

	/**buildGraph
	* Builds a graph by reading data from an ifstream
	* Preconditions:  infile has been successfully opened and the file contains
	*                 properly formated data (according to the program specs)
	* Postconditions: One graph is read from infile and stored in the object
	*/
	void buildGraph(ifstream& infile);

	/**
	* insertEdge
	* this function takes in a source and dest
	* represented by integers along with a weight and inserts the
	* new edge into the adjacency list stored in the
	* vertices array, if the edge already exists, it updates
	* the weight
	* Precondtions: valid numbers should be used for all of the
	* parameters (greater than 0)
	* Postconditions: edge is inserted into correct place in
	* adjacency list and if edge already exists, the weight is
	* updated
	* @param source: the source vertex
	* @param det: the destination vertex
	* @param weight: the weight of the edge between source and
	* dest
	*/
	void insertEdge(int source, int dest, int weight);

	/**
	* removeEdge
	* this function takes in a source and dest vertex
	* and traverses the adjacency list of EdgeNodes in order
	* to find if the edge exists, if it does, the EdgeNode is
	* deleted and true is returned, if it is not found,
	* false is returned
	* Precondtions: valid source must be entered
	* Postconditons: if edge is found, EdgeNode is deleted and
	* true is returned and if not, false is returned
	* @param source: the source vertex
	* @param det: the destination vertex
	*/
	bool removeEdge(int source, int dest);

	/**
	* findShortestPath
	* this function calls dijkstrasAlgo on all
	* of the VertexNodes in the vertices array
	* in order to store the shortest path information
	* for the entire graph (from all source vertices to all
	* other vertices)
	* Preconditons: none
	* Postconditions: the shortest path information
	* is stored in the Table T
	*/
	void findShortestPath();

	/*
	* displayAll
	* this function displays the entire graph by printing
	* all the vertices by their name and then how each
	* source vertex is connected to the other vertices (edges)
	* along with their shortest path and the distance
	* Preconditions: none
	* Postconditions: prints all the graphs information
	*/
	void displayAll() const;

	/**
	* display
	* this function takes in a source vertex and a
	* destination vertex and prints out the source and
	* dest, the distance from the source to the destination,
	* the shortest path, and then the shortest path in terms
	* of the names of the destinations
	* Precondtion: source and dest must be valid vertices
	* Postcondition: information about the path from
	* source to dest is printed
	* @param source: the source vertex of path to be printed
	* @param dest: the destination vertex of path to be printed
	*/
	void display(int source, int dest) const;

};

