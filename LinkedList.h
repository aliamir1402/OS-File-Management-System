#include <iostream> //standard libary for c++
#include<string>    //libary for string handling functionality
#include <fstream>  //libary for file handling 
#include <cstring>  //libary for string handling functionality
#include <thread>   //library for creating threads
#include<windows.h> //implementing general functions
#include <time.h>   //for finding execution time of the program
using namespace std;

#define MAX_THREAD 3

int thread_count = 0;
int flag[MAX_THREAD] = { 0 };
int mutex = 1;//initialized as reader semaphore
int wtr = 1;//initialized as reader semaphore
int ReaderCount = 0;

class node
{
public:
	string file_name;
	string data;
	node* dir;
	node* nptr;
	node* pptr;
};
node* temp_node;//signifies null node
node* search_node = new node;//stores searched on in linked list
node* s_node = new node;

class linkedlist
{
public:
	node* last = new node;
	node* start = new node;
	int length = 0;

	node* hnode = new node;
	linkedlist()
	{
		last = temp_node;
	}
	void insert(string fname, int hash_val)
	{
		node* newnode = new node;
		if (hash_val != -1)
		{
			newnode->file_name = fname;
			newnode->nptr = temp_node;
		}
		else
		{
			newnode->data = fname;
			newnode->nptr = temp_node;
		}
		if (last == temp_node)
		{
			last = newnode;
			start = last;
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
		cout << "Filename Not In List." << endl;
	}
	void search_dir(string dir_name)
	{
		node* t = new node;
		t = start;
		t->pptr = temp_node;
		while (t != temp_node)
		{
			if (t->data == dir_name)
			{
				search_node = t;
				search_node->pptr = t->pptr;
				cout << "Searched Dir Found." << endl;
				return;
			}
			else
			{
				t->pptr = t;
				t = t->nptr;
			}
		}
		cout << "Dirname Not In List." << endl;
	}
	void delete_node(string fname)
	{
		search_node = temp_node;

		if (start != temp_node)
		{
			search(fname);
			if (search_node != temp_node)
			{
				search_node->pptr->nptr = search_node->nptr;
				search_node->nptr = temp_node;
				delete search_node;
				length--;
				cout << "File deleted Successfully." << endl;
			}
		}
	}
};

string FileNames[1024];
linkedlist* htable = new linkedlist[1024];
linkedlist* dlist = new linkedlist;