/*
 * @file Vertex.h
 * @author Katarina McGaughy
 * Vertex class: The Vertex class reads in a line from istream and sets the
 * vertex data member, vertexName to the line from istream.
 *
 * Features:
 * -stores a string from istream
 * -operator<< prints out data member vertextName
 * -operator>> stores line from istream in vertexName
 *
 * Assumptions:
 * -istream getline is valid

 *
 * @version 0.1
 * @date 2022-2-3
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <string>
#include <iostream>
using namespace std;


class Vertex
{

private:
	//description of vertex
	string vertexName = "";

public:

	/**
	 * Overloaded output operator for Vertex class
	 * Preconditions: Vertex object must be initialized
	 * Postconditions: the vertexName is sent to the output stream
	 * @param v: Vertex object
	 * @param os: output stream
	 * @return: output stream
	 */
	friend ostream& operator<<(ostream& os, const Vertex& v) {
		cout << v.vertexName;
		return os;
	}

	/**
	 * operator>>
	 * this function takes in an istream and Vertex object and sets
	 * the vertexName to the line read in from istream
	 * @param is: input stream
	 * @param v : Vertex object
	 * @return istream& : returns the istream
	 */
	friend istream& operator>>(istream& in, Vertex& v) {
		getline(in, v.vertexName);
		return in;
	}
};

