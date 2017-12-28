#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
using namespace std;
struct Node
{
	string key;
	int size;
	string type;
	Node* parent;
	vector <Node*> children;
};
enum type {Folder, File} folder;
struct Compare
{
	bool operator ()(Node* first, Node* second)
	{
		return first -> size < second -> size;
	}
};