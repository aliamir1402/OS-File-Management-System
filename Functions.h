#include "LinkedList.h"

//Functions to Create, Delete, Open, Edit, Move, Truncate File.
void Delete_File(string f_name, int hash_val)
{
	if (htable[hash_val].hnode->file_name == f_name)
	{
		htable[hash_val].hnode->file_name = "*";
		htable[hash_val].hnode->data = "*";
		htable[hash_val].hnode->dir = temp_node;
		cout << "File deleted Successfully." << endl;
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
		cout << "Directory Created Successfully." << endl << endl;
	}
	else
	{
		cout << "Directory Has A Repeatative Name." << endl << endl;
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
			cout << "File changed and moved to " << htable[hash_val].hnode->dir->data << endl;
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
				cout << "File changed and moved to " << s_node->dir->data << endl;
				cout << endl;
				return;
			}
		}
	}
	cout << "File or Directory Not Found." << endl << endl;
}
void Edit_To_File(string f_name, int hash_val, string text)//for reading, editing file
{
	int r_option = -1;
	int w_option = -1;
	int read_size = -1;
	int option = -1;
	// << "Enter Option For 0.Read Or 1.Write: "; cin >> option;//option from user for read or write
	if (text == "r")
		option = 0;
	else if (text == "w")
		option = 1;
	else if (text != "r" && text != "w")
		option = 1;

	if (htable[hash_val].hnode->file_name == f_name)
	{
		if (option == 0)//for reading file
		{
			//cout << "Enter Reading Option\n 0.Read Sequentially.\n 1.Reading Till Specific Size." << endl;
			//cin >> r_option;
			r_option = 0;
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
				cout << "Enter Data to append to file" << endl;
				char str2[100];
				fgets(str2, sizeof(str2), stdin);
				fgets(str2, sizeof(str2), stdin);
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
				//cout << "Enter Reading Option\n 0.Read Sequentially.\n 1.Reading Till Specific Size." << endl;
			//cin >> r_option;
				r_option = 0;
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
					cout << "Enter Data to append to file" << endl;
					char str2[100];
					fgets(str2, sizeof(str2), stdin);
					fgets(str2, sizeof(str2), stdin);
					htable[hash_val].hnode->data = htable[hash_val].hnode->data + str2;
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
	string temp_string = "";
	node* Dtemp = new node;
	node* Htemp = new node;
	Dtemp = temp_node;
	Dtemp = dlist->start;
	string memory;
	int i = 0;

	fstream MyFile;

	if (thread_count == 1)
		MyFile.open("output1.txt", ios::out);
	else if (thread_count == 2)
		MyFile.open("output2.txt", ios::out);
	else if (thread_count == 3)
		MyFile.open("output3.txt", ios::out);

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
}
void thread_function()
{
	int hash_val = 0;
	int M_Option = -1;
	string f_name = "";//from user input
	int conti = 1;
	string Dir_Name = "";
	int size = 0;
	char line[100];
	string word = "";
	char* next_token1 = NULL;
	int count = 0;
	int from;
	int to;

	//maintain counter for threads
	cout << endl;
	cout << "Thread Currently Running: " << ++thread_count << endl << endl;

	fstream  afile;
	if (thread_count == 1)
		afile.open("command1.txt", ios::in);
	else if (thread_count == 2)
		afile.open("command2.txt", ios::in);
	else if (thread_count == 3)
		afile.open("command3.txt", ios::in);

	cout << "Home directory created." << endl;
	Insert_Dir("home");

	do
	{
		afile >> line;
		count++;
		cout << "------------------------Command: " << count << "---------------------------" << endl;
		cout << "Line: " << line << endl;
		word = strtok_s(line, ",", &next_token1);
		//cout << "Line: " << word << endl << endl;
		M_Option = ConvertToOption(word);

		cout << endl;
		//cout << "Select Any Option: "; cin >> M_Option;

		switch (M_Option)
		{
		case 1:
			//cout << "Enter File Name to Create: "; fgets(f_name, sizeof(f_name), stdin);
			//fgets(f_name, sizeof(f_name), stdin);
			f_name = strtok_s(NULL, ",", &next_token1);
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			Insert_File(f_name, hash_val);
			Move_To_Dir(f_name, hash_val, "home");
			break;
		case 2:
			//cout << "Enter Directory Name: "; fgets(Dir_Name, sizeof(Dir_Name), stdin);
			//fgets(Dir_Name, sizeof(Dir_Name), stdin);
			Dir_Name = strtok_s(NULL, ",", &next_token1);
			Insert_Dir(Dir_Name);
			break;
		case 3:
			//cout << "Enter File Name to Move: ";  fgets(f_name, sizeof(f_name), stdin);
			//fgets(f_name, sizeof(f_name), stdin);
			f_name = strtok_s(NULL, ",", &next_token1);
			Dir_Name = strtok_s(NULL, ",", &next_token1);
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			//cout << "Enter Directory Name To Move: "; fgets(Dir_Name, sizeof(Dir_Name), stdin);
			Move_To_Dir(f_name, hash_val, Dir_Name);
			break;
		case 4:
			//cout << "Enter File Name to Change: ";  fgets(f_name, sizeof(f_name), stdin);
			//fgets(f_name, sizeof(f_name), stdin);
			f_name = strtok_s(NULL, ",", &next_token1);
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			Edit_To_File(f_name, hash_val, strtok_s(NULL, ",", &next_token1));
			break;
		case 5:
			from = 0;
			to = 0;
			size = 0;
			//cout << "Enter File Name to Change: ";  fgets(f_name, sizeof(f_name), stdin);
			//fgets(f_name, sizeof(f_name), stdin);
			f_name = strtok_s(NULL, ",", &next_token1);
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			//cout << "Start Selection From(Position): "; cin >> from;
			from = std::stoi(strtok_s(NULL, ",", &next_token1));
			//cout << "From: " << from << endl;
			//cout << "Limit Value To Be Changed "; cin >> size;
			size = std::stoi(strtok_s(NULL, ",", &next_token1));
			//cout << "Size: " << size << endl;
			//cout << "New Postion To Be Placed: "; cin >> to;
			to = std::stoi(strtok_s(NULL, ",", &next_token1));
			//cout << "To: " << to << endl;
			MoveContentWithinFile(f_name, from, to, size, hash_val);
			break;
		case 6:
			//cout << "Enter File Name to Delete: ";  fgets(f_name, sizeof(f_name), stdin);
			//fgets(f_name, sizeof(f_name), stdin);
			f_name = strtok_s(NULL, ",", &next_token1);
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			Delete_File(f_name, hash_val);
			break;
		case 7:
			//cout << "Enter Position To Truncate File: "; cin >> size;
			//cout << "Enter File Name to Create: ";  fgets(f_name, sizeof(f_name), stdin);
			//fgets(f_name, sizeof(f_name), stdin);
			f_name = strtok_s(NULL, ",", &next_token1);
			hash_val = Hash_Value(f_name);//tested returns hash number remainder of 1024
			TruncateFile(f_name, int(strtok_s(NULL, ",", &next_token1)), hash_val);
			break;
		case 8:
			PrintMemoryMap();
			break;
		default:
			cout << "Unavailable Option Selected." << endl;
		}
		//system("pause");
		//cout << endl;

		//Menu();
		//cout << endl;		
		//cout << "Select Any Option: "; cin >> M_Option;
	} while (!afile.eof());
	flag[thread_count] = 1;
	afile.close();
}