// C++ Code of Server Side:

#include<iostream>
#include<cstring>
#include<cstdio>
#include<string.h>    //libary for string handling functsionality
#include<fstream>  //libary for file handling
#include<cstring>  //libary for string handling functionality
#include<thread>   //library for creating threads
#include<time.h>   //for finding execution time of the program
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

using namespace std;

// Defining Port Number and Buffer Size:
#define PORT 5000
#define BUFFER_SIZE 100

#define MAX_THREAD 3

int thread_count = 0;
int flag[MAX_THREAD] = { 0 };
int mutex = 1;//initialized as reader semaphore
int wtr = 1;//initialized as reader semaphore
int ReaderCount = 0;

FILE* fp = fopen("result.txt", "w");
int client_socket;

// Function for Reading Command from Client:

void read_command(char msg[], int socket)
{
	cout << ">> Receiving Command...";
	read(socket, msg, BUFFER_SIZE);
}

// Function for Sending Result File to the Client:

void send_results(int socket)
{
	FILE* fp2 = fopen("result.txt", "r"); //Reading the file

	char result[BUFFER_SIZE] = "";
	cout << "\n>> Sending Result...";

	// Loop is continued till whole file result is sent:

	while (fgets(result, BUFFER_SIZE, fp2) != NULL)
	{
		write(socket, result, sizeof(result));
	}

	cout << "\n>> Results Sent!" << endl;
}

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

void wait(int* s)
{
	int x = 0;
	while (*s <= 0)
	{
		if (x == 0)
			cout << "\n\"In Writing Mode...Starts when in read mode.\" OR \"No Reader Is Present.\"" << endl << endl;
		x++;
	}
	*s -= 1;
}
void signal(int* s)
{
	*s += 1;
}
//Functions to Create, Delete, Open, Edit, Move, Truncate File.
void Delete_File(string f_name, int hash_val)
{
	if (htable[hash_val].hnode->file_name == f_name)
	{
		htable[hash_val].hnode->file_name = "*";
		htable[hash_val].hnode->data = "*";
		htable[hash_val].hnode->dir = temp_node;
		cout << "\nFile deleted Successfully." << endl;
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
			fputs("File Created Successfully.", fp);
			fclose(fp);
			send_results(client_socket);

		}
		else
		{
			cout << "\nFile Has A Repeatative Name." << endl << endl;
		}
	}
	else
	{
		if (htable[hash_val].hnode->file_name == f_name)
		{
			cout << "\nFile Has A Repeatative Name." << endl << endl;
			return;
		}
		search_node = temp_node;
		htable[hash_val].search(f_name);
		if (search_node == temp_node)
		{
			htable[hash_val].insert(f_name, hash_val);
			fputs("File Created Successfully.", fp);
			fclose(fp);
			send_results(client_socket);

			htable[hash_val].hnode->nptr = htable[hash_val].start;
		}
		else
		{
			cout << "\nFile Has A Repeatative Name." << endl << endl;
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
		fputs("Directory Created Successfully.", fp);
		fclose(fp);
		send_results(client_socket);
	}

	else
	{
		cout << "\nDirectory Has A Repeatative Name." << endl << endl;
	}
}

void Move_To_Dir(string file_name, int hash_val, string dir_name)
{
	if (htable[hash_val].hnode->file_name == file_name)
	{
		search_node = temp_node;
		dlist->search_dir(dir_name);
		if (search_node != temp_node)
		{
			htable[hash_val].hnode->dir = search_node;
			cout << "\nFile changed and moved to " << htable[hash_val].hnode->dir->data << endl;
			fputs("File Moved", fp);
			fclose(fp);
			send_results(client_socket);


			return;
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
				cout << "\nFile changed and moved to " << s_node->dir->data << endl;
				cout << endl;
				return;
			}
		}
	}
	cout << "\nFile or Directory Not Found." << endl << endl;
}
void Edit_To_File(string f_name, int hash_val, string text)//for reading, editing file
{
	int r_option = -1;
	int w_option = -1;
	int read_size = -1;
	int option = -1;
	fstream MyFile;
	MyFile.open("result.txt", ios::out);

	// << "Enter Option For 0.Read Or 1.Write: "; cin >> option;//option from user for read or write
	if (text == "r")
		option = 0;
	else if (text == "w")
		option = 1;
	else if (text != "r" && text != "w")
		option = 1;

	if (htable[hash_val].hnode->file_name == f_name)
	{
		//code for critical section

		if (option == 0)//for reading file
		{
			wait(&mutex);
			ReaderCount++;
			if (ReaderCount > 0)
				wait(&wtr);
			signal(&mutex);
			//cout << "Enter Reading Option\n 0.Read Sequentially.\n 1.Reading Till Specific Size." << endl;
			//cin >> r_option;
			r_option = 0;
			if (r_option == 0)//for reading sequentially
			{
				if (htable[hash_val].hnode->data != "*")
				{
					MyFile << htable[hash_val].hnode->data;
					MyFile.close();
					send_results(client_socket);
				}

				else
				{
					fprintf(fp, "Nothing in File");
					fclose(fp);
					send_results(client_socket);
					cout << endl;
				}
			}

			else if (r_option == 1)//for reading till character point
			{
				if (htable[hash_val].hnode->data != "*")
				{
					cout << "\nEnter Size Till Read: " << endl;
					cin >> read_size;
					cout << htable[hash_val].hnode->data.substr(0, read_size) << endl;
				}
				else
					cout << "\nNothing In File." << endl;
				cout << endl;
			}
			cout << endl;
			cout << "\nOnce done with reading..." << endl;
			system("pause");

			wait(&mutex);
			ReaderCount--;
			if (ReaderCount == 0)
				signal(&wtr);
			signal(&mutex);
		}
		else if (option == 1)//for writing to file
		{
			wait(&wtr);

			//code for critical section

			//cout << "Enter:\n 1.Write \n 2.Append\n" << endl;
			//cin >> w_option;
			if (text == "w")
				w_option = 2;
			else
				w_option = 1;

			if (w_option == 1)//for overwriting file
			{
				//cout << "Enter Data to write to file" << endl;
				//char str1[100];
				//fgets(str1, sizeof(str1), stdin);
				//fgets(str1, sizeof(str1), stdin);
				htable[hash_val].hnode->data = text;
			}
			else if (w_option == 2)//for appending to file
			{
				cout << "\nEnter Data to append to file" << endl;
				char str2[100];
				fgets(str2, sizeof(str2), stdin);
				fgets(str2, sizeof(str2), stdin);
				htable[hash_val].hnode->data = htable[hash_val].hnode->data + str2;
			}

			cout << endl;
			cout << "\nOnce done with Writing..." << endl;
			system("pause");

			signal(&wtr);
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
				//cout << "Enter Reading Option\n 0.Read Sequentially.\n 1.Reading Till Specific Size." << endl;
			//cin >> r_option;

				wait(&mutex);
				ReaderCount++;
				if (ReaderCount > 0)
				{
					wait(&wtr);
				}
				signal(&mutex);

				r_option = 0;
				if (r_option == 0)//for reading sequentially
				{
					cout << "\nFile Contents:" << endl;
					if (search_node->data != "*")
						cout << search_node->data << endl;
					else
						cout << "\nNothing In File." << endl;
					cout << endl;
				}
				else if (r_option == 1)//for reading till character point
				{
					cout << "\nEnter Size Till Read: " << endl;
					cin >> read_size;
					if (search_node->data != "*")
					{
						cout << "\nEnter Size Till Read: " << endl;
						cin >> read_size;
						cout << search_node->data.substr(0, read_size) << endl;
					}
					else
						cout << "\nNothing In File." << endl;
					cout << endl;
				}
				cout << endl;
				cout << "\nOnce done with reading..." << endl;
				system("pause");

				wait(&mutex);
				ReaderCount--;
				if (ReaderCount == 0)
					signal(&wtr);
				signal(&mutex);
			}
			else if (option == 1)//for writing to file
			{
				//cout << "Enter:\n 1.Write \n 2.Append\n" << endl;
				//cin >> w_option;
				if (text == "w")
					w_option = 2;
				else
					w_option = 1;

				wait(&wtr);
				if (w_option == 1)//for overwriting file
				{
					//cout << "Enter Data to write to file" << endl;
					//char str1[100];
					//fgets(str1, sizeof(str1), stdin);
					//fgets(str1, sizeof(str1), stdin);
					htable[hash_val].hnode->data = text;
				}
				else if (w_option == 2)//for appending to file
				{
					cout << "\nEnter Data to append to file" << endl;
					char str2[100];
					fgets(str2, sizeof(str2), stdin);
					fgets(str2, sizeof(str2), stdin);
					htable[hash_val].hnode->data = htable[hash_val].hnode->data + str2;
				}

				cout << endl;
				cout << "\nOnce done with Writing..." << endl;
				system("pause");

				signal(&wtr);
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
		cout << "\nFile Edited Successfully" << endl << endl;
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
			cout << "\nFile Edited Successfully" << endl << endl;
		}
	}
}
void TruncateFile(string f_name, int size, int hash_val)
{
	if (htable[hash_val].hnode->file_name == f_name)
	{
		htable[hash_val].hnode->data = htable[hash_val].hnode->data.substr(0, size);
		cout << "\nFile Truncated." << endl << endl;
	}
	else
	{
		search_node = temp_node;
		htable[hash_val].search(f_name);
		if (search_node != temp_node)
		{
			search_node->data = search_node->data.substr(0, size);
			cout << "\nFile Truncated." << endl << endl;
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

	for (int i = 0; i < 1024; ++i)
	{
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
	string temp_string = "";
	node* Dtemp = new node;
	node* Htemp = new node;
	Dtemp = temp_node;
	Dtemp = dlist->start;
	string memory;
	int i = 0;

	fstream MyFile;

	//if (thread_count == 1)
	MyFile.open("result.txt", ios::out);
	//else if (thread_count == 2)
		//MyFile.open("output2.txt", ios::out);
	//else if (thread_count == 3)
		//MyFile.open("output3.txt", ios::out);

	while (Dtemp != temp_node)
	{
		FileNames[i] = Dtemp->data + ": ";
		for (int j = 0; j < 1024; j++)
		{
			if (htable[j].hnode->file_name != "*")
			{
				Htemp = htable[j].hnode;
				while (Htemp != temp_node)
				{
					if (Htemp->dir->data == Dtemp->data)
					{
						memory = FileNames[i];
						FileNames[i] = memory + Htemp->file_name + "(Text: " + Htemp->data + " )   ";

						//cout << FileNames[i] << endl;
							//"[" + to_string(Htemp->data.size()) + "]   ";
					}
					Htemp = Htemp->nptr;
				}
			}
		}
		i++;
		Dtemp = Dtemp->nptr;
	}
	cout << endl << "Successfully Written to Dat File." << endl << endl;

	// Write to the file
	for (int x = 0; x < i; x++)
	{
		MyFile << FileNames[x];
		MyFile << endl;
		MyFile << "-----" << endl;
	}

	send_results(client_socket);
	// Close the file
	MyFile.close();
}


//Options/Services Available
void Menu()
{
	cout << "---------------Menu----------------\n 1.Create A File.\n 2.Create Directory.\n 3.Move File To Directory\n";
	cout << " 4.Open A File(For Read or Write).\n 5.Move Contents Within A File.\n 6.Delete A File.\n";
	cout << " 7.Trucate a File.\n 8.Write Memory Map To File.\n";
}

//Main Functions
int ConvertToOption(string command)
{
	if (command == "create")
		return 1;
	else if (command == "mkdir")
		return 2;
	else if (command == "move")
		return 3;
	else if (command == "open" || command == "write" || command == "read")
		return 4;
	else if (command == "mvcfile")
		return 5;
	else if (command == "delete")
		return 6;
	else if (command == "trunc")
		return 7;
	else if (command == "showmm")
		return 8;
	else if (command == "close")
		return 0;
	return 0;
}

void thread_function()
{
	int hash_val = 0;
	int M_Option = -1;
	string f_name = "";//from user input
	int conti = 1;
	string Dir_Name = "";
	int size = 0;
	string word = "";
	char* next_token1 = NULL;
	int count = 0;
	int from;
	int to;
	char line[BUFFER_SIZE] = "";

	// Receiving Command from the client:

	read_command(line, client_socket);
	cout << "\n>> From Client:" << line;

	//maintain counter for threads
	cout << endl;
	cout << "\nThread Currently Running: " << ++thread_count << endl << endl;

	//fstream  afile;
	//if (thread_count == 1)
		//afile.open("command1.txt", ios::in);
	//else if (thread_count == 2)
		//afile.open("command2.txt", ios::in);
	//else if (thread_count == 3)
		//afile.open("command3.txt", ios::in);

	cout << "\nHome directory created." << endl;
	Insert_Dir("home");


	//afile >> line;
	count++;
	cout << "\n------------------------Command: " << count << "---------------------------" << endl;
	cout << "Line: " << line << endl;
	word = strtok(line, ",");
	//cout << "Line: " << word << endl << endl;
	M_Option = ConvertToOption(word);

	cout << endl;
	//cout << "Select Any Option: "; cin >> M_Option;

	switch (M_Option)
	{
	case 1:
		//cout << "Enter File Name to Create: "; fgets(f_name, sizeof(f_name), stdin);
		//fgets(f_name, sizeof(f_name), stdin);
		f_name = strtok(NULL, ",");
		hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
		Insert_File(f_name, hash_val);
		Move_To_Dir(f_name, hash_val, "home");
		break;
	case 2:
		//cout << "Enter Directory Name: "; fgets(Dir_Name, sizeof(Dir_Name), stdin);
		//fgets(Dir_Name, sizeof(Dir_Name), stdin);
		Dir_Name = strtok(NULL, ",");
		Insert_Dir(Dir_Name);
		break;
	case 3:
		//cout << "Enter File Name to Move: ";  fgets(f_name, sizeof(f_name), stdin);
		//fgets(f_name, sizeof(f_name), stdin);
		f_name = strtok(NULL, ",");
		Dir_Name = strtok(NULL, ",");
		hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
		//cout << "Enter Directory Name To Move: "; fgets(Dir_Name, sizeof(Dir_Name), stdin);
		Move_To_Dir(f_name, hash_val, Dir_Name);
		break;
	case 4:
		//cout << "Enter File Name to Change: ";  fgets(f_name, sizeof(f_name), stdin);
		//fgets(f_name, sizeof(f_name), stdin);
		f_name = strtok(NULL, ",");
		hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
		Edit_To_File(f_name, hash_val, strtok(NULL, ","));
		break;
	case 5:
		from = 0;
		to = 0;
		size = 0;
		//cout << "Enter File Name to Change: ";  fgets(f_name, sizeof(f_name), stdin);
		//fgets(f_name, sizeof(f_name), stdin);
		f_name = strtok(NULL, ",");
		hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
		//cout << "Start Selection From(Position): "; cin >> from;
		from = std::stoi(strtok(NULL, ","));
		//cout << "From: " << from << endl;
		//cout << "Limit Value To Be Changed "; cin >> size;
		size = std::stoi(strtok(NULL, ","));
		//cout << "Size: " << size << endl;
		//cout << "New Postion To Be Placed: "; cin >> to;
		to = std::stoi(strtok(NULL, ","));
		//cout << "To: " << to << endl;
		MoveContentWithinFile(f_name, from, to, size, hash_val);
		break;
	case 6:
		//cout << "Enter File Name to Delete: ";  fgets(f_name, sizeof(f_name), stdin);
		//fgets(f_name, sizeof(f_name), stdin);
		f_name = strtok(NULL, ",");
		hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
		Delete_File(f_name, hash_val);
		break;
	case 7:
		//cout << "Enter Position To Truncate File: "; cin >> size;
		//cout << "Enter File Name to Create: ";  fgets(f_name, sizeof(f_name), stdin);
		//fgets(f_name, sizeof(f_name), stdin);
		f_name = strtok(NULL, ",");
		hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
		TruncateFile(f_name, stoi(strtok(NULL, ",")), hash_val);
		break;
	case 8:
		PrintMemoryMap();
		break;
	default:
		cout << "\nUnavailable Option Selected." << endl;
	}
	//system("pause");
	//cout << endl;

	//Menu();
	//cout << endl;
	//cout << "Select Any Option: "; cin >> M_Option;

	flag[thread_count] = 1;
	//afile.close();
}

int main()
{

	char result[BUFFER_SIZE] = "";

	// Creating a TCP Socket for the Server:

	int server = socket(AF_INET, SOCK_STREAM, 0);

	// IP Address and Port for the Socket:

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;

	// Binding to the IP and Port:

	int status = bind(server, (struct sockaddr*)&address, sizeof(address));

	// If binding is unsuccessfull:

	if (status == -1)
		cout << "Binding Error\n";

	// If binding is successfull:

	else
	{
		cout << ">> Binding Successfull\n";

		// Listening to upto max 4 Connections:
		do {
			int status2 = listen(server, 4);

			if (status2 == -1)
				cout << "Error in Listening\n";

			else

				cout << ">> Listening for Connection...\n";

			// Accepting client's request:

			client_socket = accept(server, NULL, NULL);

			int start, finish = 0;
			Intialize_Htable();

			std::thread myThreads;

			myThreads = std::thread(thread_function);
			//while (flag[i + 1] != 1);//waiting for each thread processing to be completed.

			myThreads.join();
		} while (1);

	}

	// Closing Connection:

	cout << "\n>> Connection Closed!\n";

	return 0;
}
