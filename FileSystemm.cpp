// FileSystemm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FSTree.h"
#include "HelpingFunctios.h"
//Sorting folder
void SortFolder(vector <Node*> & children)
{
	if (children.empty() == true)
		return;
	stable_sort(begin(children), end(children), Compare());
}
//Searching element according to key
Node* SearchNode(string key, Node* element)
{
	if (element == NULL)
		return NULL;
	if (element != NULL)
	{
		//If we seeking only root
		if (key.find("/") == -1 && key.find(":") != -1)
		{
			if (element -> key == key)
				return element;
			else
				return NULL;
		}
		//if seeking some key
		if (key.find("/") == -1 && key.find(":") == -1)
			return element;
		//� ���� ��� ������� 2 �����, � � �������� �������� ���
		if (key.find("/") != -1 && element -> children.empty() == true)
			return NULL;
		//� ���� ��� ������� 2 �����, ������� � �������� ����, � ��� ������
		if (key.find("/") != -1 && element -> children.empty() == false && key.find(":") != -1)
		{
			vector <string> way = Split(key, '/');
			way = Reverse(way);
			if (element -> key == way.back())
			{
				way.pop_back();
				for (unsigned int i = 0; i < element -> children.size(); i++)
				{
					if (element -> children[i] -> key == way.back())
					{
						element = element -> children[i];
						key = Merge(way, '/');
						return SearchNode(key, element);
					}
				}
				return NULL;
			}
			//���� ����� �� ���������
			if (element -> key != way.back())
				return NULL;
		}
		//���� ��������� ��� ������� 2, ������� ����, � ������� �� ������
		if (key.find("/") != -1 && element -> children.empty() == false && key.find(":") == -1)
		{
			vector <string> way = Split(key, '/');
			way = Reverse(way);
			way.pop_back();
			for (unsigned int i = 0; i < element -> children.size(); i++)
			{
				if (element -> children[i] -> key == way.back())
				{
					element = element -> children[i];
					key = Merge(way, '/');
					return SearchNode(key, element);
				}
			}
			return NULL;
		}
	}
	return NULL;
}
//removing whole tree (or subtree)
Node* DeleteAllTree(Node* root)
{
	Node* element = NULL;
	while (true)
	{
		//children are
		if (root -> children.empty() == false)
		{
			root = root -> children[root -> children.size() - 1];
			continue;
		}
		//����� �� �����
		if (root -> parent == NULL)
			break;
		//no children
		if (root -> children.empty() == true)
		{
			element = root;
			root = root -> parent;
			delete root -> children[root -> children.size() - 1];
			root -> children.pop_back();
			continue;
		}
	}
	delete root;
	root = NULL;
	return root;
}
//removing node (or nodes) according to key
void DeleteNode(string key, Node* root)
{
	if (root == NULL)
		return;
	if (root != NULL)
	{
		//���� ������ ������ �������
		if (key.find("/") == -1 && key.find(":") != -1 && root -> key != key)
			return;
		//���� ������� ���� ��� ������
		if (key.find("/") == -1 && key.find(":") != -1 && root -> key == key)
		{
			if (root -> children.empty() == true)
			{
				delete root;
				return;
			}
			if (root -> children.empty() == false)
			{
				DeleteAllTree(root);
				return;
			}
		}
		//���� ������� ���� ����� (����) - �� ������
		if (key.find("/") != -1)
		{
			Node* element = NULL;
			element = SearchNode(key, root);
			if (element == NULL)
				return;
			//���� ������� ����������, � �������� � ���� ���
			if (element != NULL && element -> children.empty() == true)
			{
				root = element -> parent;
				//���� ������� - ������������ �������
				if (root -> children.size() == 1)
				{
					delete root -> children[0];
					root -> children.pop_back();
					return;
				}
				//���� ������� - �� ������������ �������, �� �� � ������� ���������
				if (root -> children.size() > 1 && element == root -> children.at(root -> children.size() - 1))
				{
					 delete root -> children.at(root -> children.size() - 1);
					 root -> children.pop_back();
					 return;
				}
				//���� ������� �� ������������ �������, � �� �� ��������� � �������
				if (root -> children.size() > 1 && element != root -> children.at(root -> children.size() - 1))
				{
					Node* temp = NULL;
					for (unsigned int i = 0; i < root -> children.size(); i++)
					{
						if (element -> key == root -> children[i] -> key)
						{
							temp = root -> children[i];
							root -> children[i] = root -> children[root -> children.size() - 1];
							root -> children[root -> children.size() - 1] = temp;
							delete root -> children[root -> children.size() - 1];
							root  -> children.pop_back();
							return;
						}
					}
				}
			}
			//���� ������� ����������, � ������� � ���� ����
			if (element != NULL && element -> children.empty() == false)
			{
				root = element;
				element = root -> parent;
				//���� �� �� ������������ � ������� � ��������� - ��� ������������ � �������
				if (element -> children.size() >= 1 && root == element -> children.at(element -> children.size() - 1))
				{
					root -> parent = NULL;
					DeleteAllTree(root);
					element -> children.pop_back();
					return;
				}
				//���� �� �� ������������ � �� ��������� � �������
				if (element -> children.size() > 1 && root != element -> children.at(element -> children.size() - 1))
				{
					Node* temp = NULL;
					for (unsigned int i = 0; i < root -> children.size(); i++)
					{
						if (root -> key == element -> children[i] -> key)
						{
							temp = element -> children[i];
							element -> children[i] = element -> children[element -> children.size() - 1];
							element -> children[element -> children.size() - 1] = temp;
							root -> parent = NULL;
							DeleteAllTree(root);
							element -> children.pop_back();
							return;
						}
					}
				}
			}
		}
	}
}
//adding node according to key
Node* AddNode(string key1, string data1, Node* root, Node* element) 
{
	//������� ������ �� �������� � ���������� ����
	if (element == NULL && key1.find("/") == -1)
	{
		element = new(Node);
		if (!element)
		{
			cout << "������ ��������� ������" << endl;
			return NULL;
		}
		if (data1.find(",") == -1)//���� ��� �����
		{
			char folderr [][7] = {"Folder"};//����� ������������ ���������� �����
			folder = Folder;
			element -> key = key1;
			element -> type = folderr[folder];
			element -> size = stoi(data1);
			if (root == NULL)
			{
				element -> parent = NULL;
				return element;
			}
			if (root -> type != "Folder")
				return NULL;
			element -> parent = root;
			root -> children.push_back(element);
			return element;
		}
		if (data1.find(",") != -1)//���� ��� ����
		{
			if (root -> type != "Folder")
				return NULL;
			vector <string> v = Split(data1, ',');
			element -> key = key1;
			element -> type = v[0];
			element -> size = stoi(v[1]);
			element -> parent = root;
			root -> children.push_back(element);
			return element;
		}
	}
	//���� �� ���������� ����� �������� ��������� ������
	if (key1.find("/") != -1 && element == NULL)
		return NULL;
	if (element != NULL)
	{
		//���� ���� ������������, � ��� ������
		if (key1.find("/") == -1 && key1.find(":") != -1)
			return NULL;
		//���� ���� ������������, � ��� �� ������
		if (key1.find("/") == -1 && key1.find(":") == -1)
			return element;
		//���� ���� �� ������������ � �������� � �������� ����
		if (key1.find("/") != -1 && element -> children.empty() == true)
		{
			vector <string> way = Split(key1, '/');
			if (way.size() > 2)
				return NULL;
			way = Reverse(way);
			if (way.size() == 2 && way[1] == element -> key)
			{
				way.pop_back();
				return AddNode(way[0], data1, element, NULL);
			}
			if (way.size() == 2 && way[1] != element -> key)
				return NULL;
		}
		//���� ���� �� ������������ � � ���� ���� �������
		if (key1.find("/") != -1 && element -> children.empty() == false)
		{
			vector <string> way = Split(key1, '/');
			way = Reverse(way);
			//���� ������� ��������� �� ������ � ������� ����� ���������
			if (key1.find(":") != -1 && element -> key == way.back())
			{
				way.pop_back();
				for (unsigned int i = 0; i < element -> children.size(); i++)
				{
					//���� ����� ����������, � �� �� ���������, ���������� ��������
					if (way.back() == element -> children[i] -> key && way.size() > 1)
					{
						element = element -> children[i];
						key1 = Merge(way, '/');
						return AddNode(key1, data1, element, element);
					}
					//���� ����� ����������, � ������� ����� ������ ��� ����������
					if (way.back() == element -> children[i] -> key && way.size() == 1)
						return NULL;
				}
				//�� ���������, � �� ���������. ���������� ����������� �� ����� ����
				if (way.size() == 1)
				{
					key1 = Merge(way, '/');
					return AddNode(key1, data1, element, NULL);
				}
				if (way.size() > 1)
					return NULL;
			}
			//���� ������� ��������� �� ������ � ������� ����� �� ���������
			if (key1.find(":") != -1 && element -> key != way.back())
				return NULL;
			//���� ������������ �������
			if (key1.find(":") == -1)
			{
				way.pop_back();
				for (unsigned int i = 0; i < element -> children.size(); i++)
				{
					if (way.back() == element -> children[i] -> key && way.size() > 1)
					{
						element = element -> children[i];
						key1 = Merge(way, '/');
						return AddNode(key1, data1, element, element);
					}
					if (way.back() == element -> children[i] -> key && way.size() == 1)
						return NULL;
				}
				if (way.size() > 1)
					return NULL;
				if (way.size() == 1)
				{
					key1 = Merge(way, '/');
					return AddNode(key1, data1, element, NULL);
				}
			}
		}
	}
	return NULL;
}
//print some node
void Print(string key, Node* element, char* output)
{
	ofstream out_put(output, ios :: app);
	if (!out_put)
	{
		cout << "������ �������� �����" << endl;
		return;
	}
	if (element == NULL)
	{
		out_put << "������� �� ����������" << endl;
		out_put.close();
		return;
	}
	element = SearchNode(key, element);
	SortFolder(element -> children);
	if (element != NULL)
	{
		out_put << "�������:" << endl;
		out_put << "[����: " << element -> key << " " << "���: " << element -> type << " " << "������: " << element -> size << "]" << endl;
		if (element -> parent != NULL)
		{
			out_put << "��������: " << element -> parent -> key << endl;
		}
		if (element -> children.empty() == true)
		{
			out_put << "�������� ���" << endl;
			out_put.close();
			return;
		}
		out_put << "������� ��������: ";
		for (unsigned int i = 0; i < element -> children.size(); i++)
		{
			if (i == element -> children.size() - 1)
			{
				out_put << "[����: " << element -> children[i] -> key << " " << "���: " << element -> children[i] -> type << " " << "������: " << element -> children[i] ->size << "]" << endl;
				out_put.close();
				return;
			}
			else
			{
				out_put << "[����: " << element -> children[i] -> key << " " << "���: " << element -> children[i] -> type << " " << "������: " << element -> children[i] ->size << "]" << ", ";
			}
		}
		out_put.close();

	}
}
//print a whole tree
void PrintAllTree(Node* element, char* output)
{
	ofstream out_put(output, ios :: app);
	if (!output)
	{
		cout << "������ �������� �����" << endl;
		return;
	}
	if (element == NULL)//���� ������ �����
	{
		out_put << "������ ������" << endl;
		out_put << "\n";
		out_put.close();
		return;
	}
	if (element != NULL)//���� �� �����
	{
		out_put << "������� ";
		out_put << "[����: " << element -> key << " ���: " << element -> type << " ������: " << element -> size << "]" << endl;
		if (element -> parent != NULL)
		{
			out_put << "��������: " << element -> parent -> key << endl;
		}
		if (element -> children.empty() == true)
			out_put.close();
		if (element -> children.empty() == false)
		{
			SortFolder(element -> children);
			out_put << "�������:";
			for (unsigned int i = 0; i < element -> children.size(); i++)
			{
				if (i == element -> children.size() - 1)
				{
					out_put << "[����: " << element -> children[i] -> key << " " << "���: " << element -> children[i] -> type << " " << "������: " << element -> children[i] ->size << "]";
					break;
				}
				out_put << "[����: " << element -> children[i] -> key << " " << "���: " << element -> children[i] -> type << " " << "������: " << element -> children[i] ->size << "], ";
			}
			out_put << endl;
			out_put.close();
			for (unsigned int i = 0; i < element -> children.size(); i++)
			{
				PrintAllTree(element -> children[i], output);
			}
		}
	}

}
//print results of functions of adding and searching
void PrintEl(Node* element, char* out_put, string arg)
{
	ofstream output(out_put, ios :: app);
	if (!output)
	{
		cout << "������ �������� �����" << endl;
		return;
	}
	if (arg == "add")
	{
		if (element == NULL)
		{
			output << "������� ��� ���������� ��� �� ����� ���� ��������" << endl;
			output << "\n";
			output.close();
			return;
		}
		else
		{
			output << "������� ��������" << endl;
			output << "[����: " << element -> key << " " << "���: " << element -> type << " " << "������: " << element -> size << "]" << endl;
			output << "\n";
			output.close();
			return;
		}
	}
	if (arg == "search")
	{
		if (element == NULL)
		{
			output << "������� �� ������" << endl;
			output << "\n";
			output.close();
			return;
		}
		else
		{
			output << "������� ������" << endl;
			output << "[����: " << element -> key << " " << "���: " << element -> type << " " << "������: " << element -> size << "]" << endl;
			output << "\n";
			output.close();
			return;
		}
	}
}
//entry point
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	Node* root = NULL;
	bool indicator = true;
	string line;
	vector <string> v;
	ifstream input(argv[1]);
	if (!input)
	{
		cout << "������ �������� �����" << endl;
		return 0;
	}
	while (!input.eof())
	{
		getline(input, line);
		//�������� ����������
		if (FirstCheckingArguments(line) == false)
		{
			ofstream output(argv[2], ios :: app);
			output << "��������� �����������" << endl;
			output.close();
			continue;
		}
		v = Split(line, ' ');
		if (v.size() == 3)
		{
			if (SecondCheckingArguments(v[0], v[1], v[2]) == false)
			{
				ofstream output(argv[2], ios :: app);
				output << "��������� �����������" << endl;
				output.close();
				continue;
			}	
		}
		if (v.size() == 2)
		{
			if (SecondCheckingArguments(v[0], v[1], "zero") == false)
			{
				ofstream output(argv[2], ios :: app);
				output << "��������� �����������" << endl;
				output.close();
				continue;
			}	
		}
		//��� ���������, ������������
		if (v[0] == "add" && indicator == true)//���� ��������� ������
		{
			if (AddNode(v[1], v[2], root, root) == NULL)
			{
				PrintEl(NULL, argv[2], v[0]);
				continue;
			}
			root = AddNode(v[1], v[2], root, root);
			indicator = false;
			PrintEl(root, argv[2], v[0]);
			continue;
		}
		if (v[0] == "add" && indicator == false)//���� ������ ��� ����
		{
			PrintEl(AddNode(v[1], v[2], root, root), argv[2], v[0]);
			continue;
		}
		if (v[0] == "delete")
		{
			ofstream output(argv[2], ios :: app);
			if (root  == NULL)
			{
				output << "������ ������" << endl;
				output.close();
				continue;
			}
			if (v[1] == root -> key)
			{
				root = DeleteAllTree(root);
				output << "������ �������" << endl;
				output.close();
				continue;
			}
			DeleteNode(v[1], root);
			output << "������� ������" << endl;
			output.close();
			continue;
		}
		if (v[0] == "search")
		{
			PrintEl(SearchNode(v[1], root), argv[2], v[0]);
			continue;
		}
		if (v[0] == "print")
		{
			//�������� ��� ������
			if (v[1] == "alltree")
			{
				PrintAllTree(root, argv[2]);
				ofstream output(argv[2], ios :: app);
				output << "\n";
				output.close();
			}
			//�������� �������
			else
			{
				Print(v[1], root, argv[2]);
				ofstream output(argv[2], ios :: app);
				output << "\n";
				output.close();
			}
			continue;
		}
		else
		{
			ofstream output(argv[2], ios :: app);
			output << "������� ������ �������" << endl;
			output.close();
			continue;
		}
	}
	input.close();
	cout << "������� ENTER ��� �����������" << endl;
	getchar();
	return 0;
}