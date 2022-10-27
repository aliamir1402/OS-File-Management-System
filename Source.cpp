#include <iostream>
#include <string.h>
using namespace std;

void Intialize_Htable();
void Delete_File(string f_name, int hash_val);
void Insert_File(string f_name, int hash_val);
void Move_To_Dir(string file_name, int hash_val, string dir_name);
void Edit_To_File(string f_name, int hash_val);
void MoveContentWithinFile(string f_name, int from, int to, int size, int hash_val);
void TruncateFile(string f_name, int size, int hash_val);
int Hash_Value(string file_name);
void Insert_Dir(string Dir_Name);
void PrintMemoryMap();
void Menu();

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
		search_node->nptr = temp_node;

		if (start != temp_node)
		{
			search(fname);
			search_node->pptr->nptr = search_node->nptr;
			search_node->nptr = temp_node;
			delete search_node;
			length--;
			cout << "File Deleted." << endl << endl;
		}
	}
};

string FileNames[1024];
linkedlist* htable = new linkedlist[1024];
linkedlist* dlist = new linkedlist;

int main()
{
	Intialize_Htable();
	int hash_val = 0;
	int M_Option = -1;
	string f_name = "";//from user input
	int conti = 1;
	string Dir_Name;
	int size;

	Menu();
	cout << endl;
	cout << "Select Any Option: "; cin >> M_Option;
	while (conti == 1)
	{
		switch (M_Option)
		{
		case 1:
			cout << "Enter File Name to Create: "; cin >> f_name;
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			Insert_File(f_name, hash_val);
			break;
		case 2:
			cout << "Enter File Name to Delete: "; cin >> f_name;
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			Delete_File(f_name, hash_val);
			break;
		case 3:
			cout << "Enter File Name to Move: "; cin >> f_name;
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			cout << "Enter Directory Name To Move: "; cin >> Dir_Name;
			Move_To_Dir(f_name, hash_val, Dir_Name);
			break;
		case 4:
			cout << "Enter File Name to Change: "; cin >> f_name;
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			Edit_To_File(f_name, hash_val);
			break;
		case 5:
			int from, to;
			cout << "Enter File Name to Change: "; cin >> f_name;
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			cout << "Start Selection From(Position): "; cin >> from;
			cout << "Limit Value To Be Changed "; cin >> size;
			cout << "New Postion To Be Placed: "; cin >> to;
			MoveContentWithinFile(f_name, from, to, size, hash_val);
			break;
		case 6:
			cout << "Enter Directory Name: "; cin >> Dir_Name;
			Insert_Dir(Dir_Name);
			break;
		case 7:
			cout << "Enter Position To Truncate File: "; cin >> size;
			cout << "Enter File Name to Create: "; cin >> f_name;
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			TruncateFile(f_name, size, hash_val);
			break;
		case 8:
			PrintMemoryMap();
			break;
		default:
			cout << "Unavailable Option Selected." << endl;
		}
		system("pause");
		cout << endl;
		Menu();
		cout << endl;
		cout << "Select Any Option: "; cin >> M_Option;
	}
	return 0;
}

//Functions to Create, Delete, Open, Edit, Move, Truncate File.
void Delete_File(string f_name, int hash_val)
{
	if (htable[hash_val].hnode->file_name == f_name)
	{
		htable[hash_val].hnode->file_name = "*";
		htable[hash_val].hnode->data = "*";
		htable[hash_val].hnode->dir = temp_node;
	}
	else
	{
		htable[hash_val].delete_node(f_name);
	}
}
void Insert_File(string f_name, int hash_val)
{
	if (htable[hash_val].hnode->file_name == "*")
	{
		if (htable[hash_val].hnode->file_name != f_name)
		{
			htable[hash_val].hnode->file_name = f_name;
			cout << "File Created Successfully." << endl << endl;
		}
		else
		{
			cout << "File Has A Repeatative Name." << endl << endl;
		}
	}
	else
	{
		if (htable[hash_val].hnode->file_name == f_name)
		{
			cout << "File Has A Repeatative Name." << endl << endl;
			return;
		}
		search_node = temp_node;
		htable[hash_val].search(f_name);
		if (search_node == temp_node)
		{
			htable[hash_val].insert(f_name, hash_val);
			cout << "File Created Successfully." << endl << endl;
			htable[hash_val].hnode->nptr = htable[hash_val].start;
		}
		else
		{
			cout << "File Has A Repeatative Name." << endl << endl;
		}
	}
}
void Insert_Dir(string Dir_Name)
{
	search_node = temp_node;
	dlist->search_dir(Dir_Name);
	if (search_node == temp_node)
	{
		dlist->insert(Dir_Name, -1);
		cout << "File Created Successfully." << endl << endl;
	}
	else
	{
		cout << "File Has A Repeatative Name." << endl << endl;
	}
}
void Move_To_Dir(string file_name, int hash_val, string dir_name)
{
	if (htable[hash_val].hnode->file_name == file_name)
	{
		search_node = temp_node;
		cout << "A";
		dlist->search_dir(dir_name);
		cout << "A";
		if (search_node != temp_node)
		{
			cout << "A";
			htable[hash_val].hnode->dir = search_node;
			cout << "A";
			cout << "File changed and moved to " << htable[hash_val].hnode->dir->data << endl;
		}
	}
	else
	{
		search_node = temp_node;
		htable[hash_val].search(file_name);
		s_node = search_node;
		if (s_node != temp_node)
		{
			search_node = temp_node;
			dlist->search_dir(dir_name);
			if (search_node != temp_node)
			{
				s_node->dir = search_node;
				cout << "File changed and moved to " << s_node->dir->data << endl;
				cout << endl;
			}
		}
	}
}
void Edit_To_File(string f_name, int hash_val)//for reading, editing file
{
	int r_option = -1;
	int w_option = -1;
	int read_size = -1;
	int option = -1;
	cout << "Enter Option For 0.Read Or 1.Write: "; cin >> option;//option from user for read or write
	if (htable[hash_val].hnode->file_name == f_name)
	{
		if (option == 0)//for reading file
		{
			cout << "Enter Reading Option\n 0.Read Sequentially.\n 1.Reading Till Specific Size." << endl;
			cin >> r_option;
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
		else if (option == 1)//for writing to file
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
		if (search_node != temp_node)//if file is not empty
		{
			if (option == 0)//for reading file
			{
				cout << "Enter Reading Option\n 1.Read Sequentially.\n 2.Reading Till Specific Size." << endl;
				cin >> r_option;
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
			else if (option == 1)//for writing to file
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
void MoveContentWithinFile(string f_name, int from, int to, int size, int hash_val)
{
	string temp_str = "";
	if (htable[hash_val].hnode->file_name == f_name)
	{
		temp_str = htable[hash_val].hnode->data.substr(from, size);//cut out to content to be moved
		htable[hash_val].hnode->data = htable[hash_val].hnode->data.substr(0, from) +
			htable[hash_val].hnode->data.substr(size + 1, htable[hash_val].hnode->data.length());
		htable[hash_val].hnode->data = htable[hash_val].hnode->data.substr(0, to) + temp_str +
			htable[hash_val].hnode->data.substr(to + 1, htable[hash_val].hnode->data.length());
		cout << "File Edited Successfully" << endl << endl;
	}
	else
	{
		search_node = temp_node;
		htable[hash_val].search(f_name);
		if (search_node != temp_node)
		{
			temp_str = search_node->data.substr(from, size);
			search_node->data = search_node->data.substr(0, from) +
				search_node->data.substr(size + 1, search_node->data.length());
			search_node->data = search_node->data.substr(0, to) + temp_str +
				search_node->data.substr(to + 1, search_node->data.length());
			cout << "File Edited Successfully" << endl << endl;
		}
	}	
}
void TruncateFile(string f_name, int size, int hash_val)
{
	if (htable[hash_val].hnode->file_name == f_name)
	{
		htable[hash_val].hnode->data = htable[hash_val].hnode->data.substr(0, size);
		cout << "File Truncated." << endl << endl;
	}
	else
	{
		search_node = temp_node;
		htable[hash_val].search(f_name);
		if (search_node != temp_node)
		{
			search_node->data = search_node->data.substr(0, size);
			cout << "File Truncated." << endl << endl;
		}
	}
}
int Hash_Value(string file_name)
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
void Intialize_Htable()
{
	for (int i = 0; i < 1024; ++i) {
		htable[i].hnode->file_name = "*";
		htable[i].hnode->data = "*";
		htable[i].hnode->dir = temp_node;
		htable[i].hnode->nptr = temp_node;
		htable[i].hnode->pptr = temp_node;
	}
}
void PrintMemoryMap()
{
	string str = "";
	node* Dtemp = new node;
	node* Htemp = new node;
	Dtemp = dlist->start;
	int i = 0;
	while (Dtemp != temp_node)
	{
		for (int j = 0; j < 1024; j++)
		{
			if (htable[j].hnode->file_name != "*")
			{
				FileNames[i] = Dtemp->dir->data + " : ";
				Htemp = htable[j].hnode;
				while (Htemp != temp_node)
				{
					if (Htemp->dir->data == Dtemp->dir->data)
					{
						FileNames[i] = FileNames[i] + Htemp->file_name;
					}
					Htemp = Htemp->nptr;
					FileNames[i] = FileNames[i] + "  ";
				}
			}
		}
		Dtemp = Dtemp->nptr;
		i++;
	}
	for (int x = 0; x < i; x++)
		cout << FileNames[x] << endl;
}


//Options/Services Available
void Menu()
{
	cout << "---------------Menu----------------\n 1.Create A File.\n 2.Delete A File.\n 3.Move File To Directory\n";
	cout << " 4.Open A File(For Read or Write).\n 5.Move Contents Within A File.\n 6.Create Directory.\n";
	cout << " 7.Trucate a File.\n 8.Write Memory Map To File.\n";
}
