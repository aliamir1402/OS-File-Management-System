#include <iostream>
#include <string.h>
using namespace std;

void intialize_htable();
int hash_value(string file_name);


class node
{
public:
	string file_name;
	string data;
	string dir;
	node* nptr;
	node* pptr;
};

node* temp_node;//signifies null node
node* search_node = new node;//stores searched on in linked list


class linkedlist
{
private:
	node* last = new node;
	node* start = new node;
	int length = 0;
public:
	node* hnode = new node;
	linkedlist()
	{
		last = temp_node;
		last->nptr = temp_node;
	}
	void insert(string fname, int hash_val)
	{
		node* newnode = new node;
		newnode->file_name = fname;
		newnode->nptr = temp_node;

		if (last == temp_node)
		{
			last = newnode;
			start = last;
			htable[hash_val].hnode->nptr = start;
		}
		else
		{
			last->nptr = newnode;
			last = newnode;
		}
		length++;
	}
	void search(string fname)
	{
		node* t = new node;
		t = start;
		t->pptr = temp_node;
		while (t != temp_node)
		{
			if (t->file_name == fname)
			{
				search_node = t;
				search_node->pptr = t->pptr;
				cout << "Searched File Found." << endl;
				return;
			}
			else
			{
				t->pptr = t;
				t = t->nptr;
			}
		}
		cout << "Filename Does not exist." << endl;
	}
	void delete_node(string fname)
	{
		search_node = temp_node;
		search_node->nptr = temp_node;

		if (start != temp_node)
		{
			search(fname);
			search_node->pptr->nptr = search_node->nptr;
			search_node->nptr = temp_node;
			delete search_node;
			length--;
		}
	}
};

linkedlist* htable = new linkedlist[1024];

int main()
{
	intialize_htable();
	int hash_val = 0;
	string f_name = "Pakistan";//from user input
	//cout << "Enter Name of File: " << endl;
	//cin >> f_name;

	hash_val = hash_value(f_name);//tested returns hash number remainder of 1024
	//cout << "Hash_Value: " << hash_val << endl;
	
	return 0;
}

void delete_file(string f_name, int hash_val)
{
	if (htable[hash_val].hnode->file_name == f_name)
	{
		htable[hash_val].hnode->file_name = "*";
		htable[hash_val].hnode->data = "*";
		htable[hash_val].hnode->dir = "*";
	}
	else
	{
		htable[hash_val].delete_node(htable[hash_val].hnode->file_name);
	}
}


void insert_file(string f_name, int hash_val)
{
	if (htable[hash_val].hnode->file_name == "*")
	{
		htable[hash_val].hnode->file_name = f_name;
	}
	else
	{
		htable[hash_val].insert(f_name, hash_val);
	}
}

void move_to_dir(string file_name, int hash_val, string dir_name)
{
	if (htable[hash_val].hnode->file_name == file_name)
	{
		htable[hash_val].hnode->dir = dir_name;
	}
	else
	{
		htable[hash_val].search(file_name);
		search_node->dir = dir_name;
	}
}

void edit_to_file(string f_name, int hash_val, int option)//for reading, editing and trucating file
{
	int r_option;
	int w_option;
	int read_size;
	if (htable[hash_val].hnode->file_name == f_name)
	{
		if (option == 0)//for reading file
		{
			cout << "Enter Reading Option\n 1.Read Sequentially.\n 2.Reading Till Specific Size." << endl;
			if (r_option == 0)//for reading sequentially
			{
				cout << "File Contents:" << endl;
				if (htable[hash_val].hnode->data != "*")
					cout << htable[hash_val].hnode->data << endl;
				else
					cout << "Nothing In File." << endl;
				cout << endl;
			}
			else if (r_option == 1)//for reading till character point
			{
				cout << "Enter Size Till Read: " << endl;
				cin >> read_size;
				if (htable[hash_val].hnode->data != "*")
				{
					cout << "Enter Size Till Read: " << endl;
					cin >> read_size;
					cout << htable[hash_val].hnode->data.substr(0, read_size) << endl;
				}
				else
					cout << "Nothing In File." << endl;
				cout << endl;
			}
		}

		if (option == 1)//for writing to file
		{
			cout << "Enter:\n 1.Write \n 2.Append\n" << endl;
			cin >> w_option;

			if (w_option == 1)//for overwriting file
			{
				cout << "Enter Data to write to file" << endl;
				string str1 = "";
				cin >> str1;
				htable[hash_val].hnode->data = str1;
			}
			else if (w_option == 2)//for appending to file
			{
				cout << "Enter Data to append to file" << endl;
				string str2 = "";
				cin >> str2;
				htable[hash_val].hnode->data = htable[hash_val].hnode->data + str2;
			}
		}
	}
	else//check chained list
	{
		int r_option;
		int w_option;
		int read_size;

		search_node = temp_node;
		htable[hash_val].search(f_name);//assigns search_node with the address of searched file_name
		if (search_node != temp_node)
		{
			if (option == 0)//for reading file
			{
				cout << "Enter Reading Option\n 1.Read Sequentially.\n 2.Reading Till Specific Size." << endl;
				if (r_option == 0)//for reading sequentially
				{
					cout << "File Contents:" << endl;
					if (search_node->data != "*")
						cout << search_node->data << endl;
					else
						cout << "Nothing In File." << endl;
					cout << endl;
				}
				else if (r_option == 1)//for reading till character point
				{
					cout << "Enter Size Till Read: " << endl;
					cin >> read_size;
					if (search_node->data != "*")
					{
						cout << "Enter Size Till Read: " << endl;
						cin >> read_size;
						cout << search_node->data.substr(0, read_size) << endl;
					}
					else
						cout << "Nothing In File." << endl;
					cout << endl;
				}
			}

			if (option == 1)//for writing to file
			{
				cout << "Enter:\n 1.Write \n 2.Append\n" << endl;
				cin >> w_option;

				if (w_option == 1)//for overwriting file
				{
					cout << "Enter Data to write to file" << endl;
					string str1 = "";
					cin >> str1;
					search_node->data = str1;
				}
				else if (w_option == 2)//for appending to file
				{
					cout << "Enter Data to append to file" << endl;
					string str2 = "";
					cin >> str2;
					search_node->data = search_node->data + str2;
				}
			}
		}
	}
}
int hash_value(string file_name)
{
	int sum = 0;
	char x;
	for (int i = 0; i < file_name.length(); i++)
	{
		x = file_name.at(i);
		sum += int(x);
	}
	int avg = sum % 1024;
	return avg;
}
void intialize_htable()
{
	for (int i = 0; i < 1024; ++i) {
		htable[i].hnode->file_name = "*";
		htable[i].hnode->dir = "*";
		htable[i].hnode->data = "*";
		htable[i].hnode->nptr = temp_node;
		htable[i].hnode->pptr = temp_node;
	}
}
