/*
 * @file Graph.cpp
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
 *
 * TODO:
 * set maxNum : INTEGER.MAX_VALUE
 * removeEdge check to ensure it works...
 * redo print recursion??
 * comment all
 */

#include <fstream>
#include "Graph.h"
#include <iostream>
using namespace std;


/**
* constructor
* this function initializes a Graph and initializes
* the vertices array
* Preconditions: none
* Postconditios: vertices array is initialized
*/
Graph::Graph() {
}

/**
 * copy constructor
 * this function initializes a new Graph with the copyGraph
 * values via the copyGraphHelper function
 * Preconditions: copyGraph must be initialized
 * Postconditios: a new Graph is initialized with the same values
 * as the copyGraph
 * @param copyGraph: Graph to be copied
 */
Graph::Graph(const Graph& cGraph) {
	copyGraph(cGraph);
}

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
void Graph::copyGraph(const Graph& copyGraph) {
	//copy adjacency list
	//copy vertices
	for (int i = 1; i <= copyGraph.size; i++) {
		vertices[i].data = new Vertex(*copyGraph.vertices[i].data);
		vertices[i].edgeHead = copyGraphHelper(copyGraph.vertices[i].edgeHead);
	}

	//copy size
	size = copyGraph.size;

	//copy table
	for (int row = 0; row < MAX_VERTICES; row++)
	{
		for (int col = 0; col < MAX_VERTICES; col++)
		{
			T[row][col] = copyGraph.T[row][col];
		}
	}
}

/**
 * copyGraphHelper
 * this function copies the vertices from copyGraph
 * along with the EdgeNodes to the current vertices array
 * this function is called by the copy constructor and operator=
 * Preconditions: copyGraph must be initialized
 * Postconditios: adjacency list from copyGraph is copied
 * to current graph
 * @param copyGraph: Graph to be copied
 */
Graph::EdgeNode* Graph::copyGraphHelper(EdgeNode* vertexCopy) {

	if (vertexCopy == nullptr) {
		return nullptr;
	}

	EdgeNode* newVertex = new EdgeNode();
	newVertex->adjVertex = vertexCopy->adjVertex;
	newVertex->weight = vertexCopy->weight;

	newVertex->nextEdge = copyGraphHelper(vertexCopy->nextEdge);

	return newVertex;
}

/**
 * desctructor
 * this function deletes the adjacency list
 * stored in the vertices array by calling deleteList
 * Preconditions: none
 * Postconditios: adjacency list is deleted
 */
Graph::~Graph() {
	deleteList();
}

/**
 * deleteList
 * this function deletes the adjacency list
 * stored in the vertices array and calls
 * deleteListHelper in order to traverse the
 * EdgeNodes stored in the vertices array
 * Preconditions: none
 * Postconditios: adjacency list is deleted
 */
void Graph::deleteList() {
	//need to delete edgenode and vertex node 
	for (int source = 1; source <= size; source++) {
		if (vertices[source].edgeHead != nullptr) {
			deleteListHelper(vertices[source].edgeHead);
			vertices[source].edgeHead = nullptr; //not sure if this is needed...
			delete vertices[source].data;
			vertices[source].data = nullptr;
		}
	}
}

/**
 * deleteListHelper
 * this function recursively deletes the EdgeNodes
 * passed in from the deleteList function
 * Preconditions: none
 * Postconditios: EdgeNode list is deleted
 */
void Graph::deleteListHelper(EdgeNode* node) {
	if (node == nullptr) {
		return;
	}
	deleteListHelper(node->nextEdge);
	delete node;
	node = nullptr;
}

/**
 * operator=
 * this function clears the current graph if it is not the same as copyGraph
 * and then assigns the copyGraph data to the current graph via copygraphHelper()
 * Preconditions: none
 * Postconditios: a new Graph is initialized with the same values
 * as the copyGraph
 * @param copyGraph: Graph being assigned to current Graph
 */
const Graph Graph::operator=(const Graph& cGraph) {
	if (this != &cGraph) {
		deleteList();
		copyGraph(cGraph);
	}
	return *this;

}

/**buildGraph
* Builds a graph by reading data from an ifstream
* Preconditions:  infile has been successfully opened and the file contains
*                 properly formated data (according to the program specs)
* Postconditions: One graph is read from infile and stored in the object
*/
void Graph::buildGraph(ifstream& infile) {

	// data member stores array size in private member variable size
	infile >> size;
	if (infile.eof())
		return;

	// throw away '\n' to go to next line
	infile.ignore();

	// get descriptions of vertices and add to vertices array
	for (int v = 1; v <= size; v++) {
		vertices[v].data = new Vertex;
		//add description to vertices initialized
		infile >> *vertices[v].data;
		vertices[v].edgeHead = nullptr;
	}

	// fill cost edge array
	int src = 1, dest = 1, weight = 1;
	for (;;) {
		infile >> src >> dest >> weight;
		if (src == 0 || infile.eof())
			break;
		insertEdge(src, dest, weight);
	}

}

/**
* isValidVertex
* this function takes in an int vertex
* and returns if it is a valid vertex
* (1-size)
* Preconditons: none
* Postconditions: returns true if the vertex is
* between 1 and size
*/
bool Graph::isValidVertex(int vertex) const {
	if (vertex >= 1 && vertex <= size) {
		return true;
	}
	return false;
}

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
void Graph::insertEdge(int source, int dest, int weight) {
	if (!isValidVertex(source)) {
		cout << "Invalid source vertex entered." << endl;
		return;
	}
	if (!isValidVertex(dest)) {
		cout << "Invalid destination vertex entered." << endl;
		return;
	}
	if (weight < 0) {
		cout << "Weight can not be negative." << endl;
		return;
	}
	EdgeNode* edge = findEdge(source, dest);
	//if edge exists 
	if (edge != nullptr) {
		edge->weight = weight;
	}
	else {
		EdgeNode* newNode = new EdgeNode();
		newNode->adjVertex = dest;
		newNode->weight = weight;
		newNode->nextEdge = vertices[source].edgeHead;
		vertices[source].edgeHead = newNode;
	}
	edge = nullptr;

}

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
Graph::EdgeNode* Graph::findEdge(int source, int dest) const {
	if (!isValidVertex(source)) {
		cout << "Invalid source vertex entered." << endl;
		return nullptr;
	}
	if (!isValidVertex(dest)) {
		cout << "Invalid destination vertex entered." << endl;
		return nullptr;
	}
	if (vertices[source].edgeHead == nullptr) {
		return nullptr;
	}
	EdgeNode* curr = vertices[source].edgeHead;
	while (curr != nullptr) {
		if (curr->adjVertex == dest) {
			return curr;
		}
		curr = curr->nextEdge;
	}
	curr = nullptr;
	return curr;
}

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
bool Graph::removeEdge(int source, int dest) {
	if (!isValidVertex(source)) {
		cout << "Invalid source vertex entered." << endl;
		return false;
	}
	if (!isValidVertex(dest)) {
		cout << "Invalid destination vertex entered." << endl;
		return false;
	}
	EdgeNode* curr = vertices[source].edgeHead;
	//if edgeHead is destination
	if (curr->adjVertex == dest) {
		curr->nextEdge = curr->nextEdge->nextEdge;
		delete curr;
		curr = nullptr;
		return true;
	}
	if (curr != nullptr) {
		while (curr->nextEdge != nullptr) {
			if (curr->nextEdge->adjVertex == dest) {
				curr->nextEdge = curr->nextEdge->nextEdge;
				delete curr->nextEdge;
				curr = nullptr;
				return true;
			}
			curr = curr->nextEdge;
		}
	}
	curr = nullptr;
	return false;
}

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
void Graph::findShortestPath() {
	//reset table values
	resetTable();

	for (int i = 1; i <= size; i++) {
		//ensure source vertex is connected to another vertex
		if (vertices[i].edgeHead != nullptr) {
			dijkstrasAlgo(vertices[i], i);
		}
	}
}


/**
* resetTable
* this function resets the Table T
* Preconditons: none
* Postconditions: the tables values are reset to
* their initial values
*/
void Graph::resetTable() {
	for (int source = 1; source <= size; source++) {
		for (int dest = 1; dest <= size; dest++) {
			T[source][dest].path = 0;
			T[source][dest].visited = false;
			T[source][dest].dist = numeric_limits<int>::max();
		}
	}
}

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
void Graph::dijkstrasAlgo(VertexNode sourceVertex, int source) {

	//set distance of sourceVertex to 0
	T[source][source].dist = 0;

	//have priority queue of adjacentVertices
	priority_queue <AdjacentVertex, vector<AdjacentVertex>, CompareAdjacentVertex > pq;
	AdjacentVertex adjV = AdjacentVertex();
	//set source vertex to 0
	adjV.dist = 0;
	//set vertexIndex to source
	adjV.vertexIndex = source;
	pq.push(adjV);

	for (int dest = 1; dest <= size; dest++) {
		//if destination is not source, add to pq
		if (dest != source) {
			AdjacentVertex adjV = AdjacentVertex();
			//set vertexIndex to dest
			adjV.vertexIndex = dest;
			pq.push(adjV);
		}
	}

	//repeat n-1 times
	for (int i = 1; i < size; i++) {
		AdjacentVertex minVertex = pq.top();
		pq.pop();
		if (T[source][minVertex.vertexIndex].visited == false) {
			//set vertex first visited to visited
			T[source][minVertex.vertexIndex].visited = true;

			//have v represent the minVertex in pq
			EdgeNode* v = vertices[minVertex.vertexIndex].edgeHead;
			//while v is not nullptr
			while (v != nullptr) {
				int w = v->adjVertex;
				if (T[source][w].visited == false &&
					T[source][w].dist > T[source][minVertex.vertexIndex].dist + v->weight) {
					T[source][w].dist = T[source][minVertex.vertexIndex].dist + v->weight;
					//set previous vertex in shortest path to path in table
					T[source][w].path = minVertex.vertexIndex;
					//add new pq with updated distance 
					AdjacentVertex updated = AdjacentVertex();
					updated.dist = T[source][w].dist;
					updated.vertexIndex = w;
					pq.push(updated);
				}
				v = v->nextEdge;
			}
		}
	}
}

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
void Graph::printPath(int source, int dest) const {
	if (!isValidVertex(source)) {
		cout << "Invalid source vertex entered." << endl;
		return;
	}
	if (!isValidVertex(dest)) {
		cout << "Invalid destination vertex entered." << endl;
		return;
	}
	if (vertices[source].edgeHead != nullptr) {
		if (dest == source) {
			return;
		}
		else {
			printPath(source, dest = T[source][dest].path);
			cout << dest << " ";
		}
	}
}

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
void Graph::printDestinations(int source, int dest) const {
	if (!isValidVertex(source)) {
		cout << "Invalid source vertex entered." << endl;
		return;
	}
	if (!isValidVertex(dest)) {
		cout << "Invalid destination vertex entered." << endl;
		return;
	}
	//if there is a path from source to dest
	if (vertices[source].edgeHead != nullptr) {
		if (dest == source) {
			return;
		}
		printDestinations(source, dest = T[source][dest].path);
		cout << *vertices[dest].data << endl;
	}
}


/*
* displayAll
* this function displays the entire graph by printing
* all the vertices by their name and then how each
* source vertex is connected to the other vertices (edges)
* along with their shortest path and the distance
* Preconditions: none
* Postconditions: prints all the graphs information
*/
void Graph::displayAll() const {
	if (size == 0) {
		cout << "No graph to print. Please enter graph." << endl;
		return;
	}
	cout << "Description";
	cout << "                  From  To    Dist  Path" << endl;
	for (int source = 1; source <= size; source++) {
		//print out string representation of vertices
		cout << *vertices[source].data << endl;
		for (int dest = 1; dest <= size; dest++) {
			if (dest != source) {
				cout << "                             " << source;
				cout << "     " << dest;
				//if there is a path, print distance and shortest path
				if (vertices[source].edgeHead != nullptr) {
					if (T[source][dest].dist != numeric_limits<int>::max()) {
						cout << "     " << T[source][dest].dist;
						if (T[source][dest].dist < 10) {
							cout << "     ";
						}
						else {
							cout << "    ";
						}
						printPath(source, dest);
						cout << dest << " " << endl;
					}
					else {
						cout << "     " << "--" << endl;
					}
				}
				//if there is not a path 
				else {
					cout << "     --" << endl;
				}
			}
		}
	}
}

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
void Graph::display(int source, int dest) const {
	if (!isValidVertex(source)) {
		cout << "Invalid source vertex entered." << endl;
		return;
	}
	if (!isValidVertex(dest)) {
		cout << "Invalid destination vertex entered." << endl;
		return;
	}
	cout << source << "  " << dest << "  " << T[source][dest].dist;
	cout << "    ";
	printPath(source, dest);
	cout << dest << endl;
	//now print order of destinations 
	printDestinations(source, dest);
	cout << *vertices[dest].data << endl;

}





