/*data base command
*/
#pragma once
#include<map>
#include"skip_list.hpp"

using std::make_pair;
using std::map;
using std::cin;

class command
{
	//commands

	//level 1
	//@create,display,destory,in
	const string CREATE1 = "create";
	const string CREATE2 = "CREATE";
	const string DISPLAY1 = "display";
	const string DISPLAY2 = "DISPLAY";
	const string DESTORY1 = "destory";
	const string DESTORY2 = "DESTORY";

	//level 2
	//@insert,erase,fix,clear
	const string INSERT1 = "insert";
	const string INSERT2 = "INSERT";
	const string ERASE1 = "erase";
	const string ERASE2 = "ERASE";
	const string FIX1 = "fix";
	const string FIX2 = "FIX";
	const string CLEAR1 = "clear";
	const string CLEAR2 = "CLEAR";

	//help tips
	const string HELP1 = "help";
	const string HELP2 = "HELP";
	
	//list name path
	const string PATH = "D:/@list_name.txt";

	private:
		using list = skip_list<string , string>;
		using name = string;

	private:
		map<name , list> list_map;
		list* in_ptr = nullptr;

		//file operation
		ofstream writer;
		ifstream reader;

	public:
		command()
		{
			cout << "data base initializing" << endl;
			load();
		}

		void get_command(string& str)
		{
			string s_command;
			string name;

			get_command_from_string(str , s_command , name);
			int command_check = command_call(s_command , name);

			if (command_check == 1)
				cout << "operation done" << endl;
			else if (command_check == 2)
				cout << "input error , check your input please" << endl;
			else
				cout << "command error , use 'help' to check command and input again" << endl;

			system("pause");
		}

	private:
		//return value:
		//1 : operation effective
		//2 : command correct but list name or subsequent input wrong
		//-1 : command wrong
		int command_call(const string& s_command,const string& name)
		{
			if (s_command == CREATE1 || s_command == CREATE2)
			{
				if (create(name) == true)
					return 1;

				return 2;
			}
			else if (s_command == DISPLAY1 || s_command == DISPLAY2)
			{
				if (display(name) == true)
					return 1;

				return 2;
			}
			else if (s_command == DESTORY1 || s_command == DESTORY2)
			{
				if (destory(name) == true)
					return 1;

				return 2;
			}
			else if (s_command == CLEAR1 || s_command == CLEAR2)
			{
				if (clear(name) == true)
					return 1;

				return 2;
			}
			else if (s_command == INSERT1 || s_command == INSERT2)
			{
				if (insert(name) == true)
					return 1;

				return 2;
			}
			else if (s_command == ERASE1 || s_command == ERASE2)
			{
				if (erase(name) == true)
					return 1;

				return 2;
			}
			else if (s_command == FIX1 || s_command == FIX2)
			{
				if (fix(name) == true)
					return 1;

				return 2;
			}
			else if (s_command == HELP1 || s_command == HELP2)
			{
				help();
				return 1;
			}

			return -1;
		}

		void get_command_from_string(const string& str , string& s_command , string& name)
		{
			if (str.empty())
			{
				return;
			}

			s_command = str.substr(0 , str.find(" "));
			name = str.substr(str.find(" ") + 1 , str.length());
		}

		//lv1

		bool create(const string& name)
		{
			if (list_map.find(name) != list_map.end())
				return false;

			skip_list<string , string> obj(5,name);
			list_map.insert(make_pair(name , obj));

			save();
			return true;
		}

		bool display(const string& name)
		{
			auto pos = list_map.find(name);
			if (pos != list_map.end())
			{
				(*pos).second.display();

				return true;
			}

			return false;
		}

		bool destory(const string& name)
		{
			auto pos = list_map.find(name);
			if (pos != list_map.end())
			{
				(*pos).second.destory();
				list_map.erase(pos);

				save();
				return true;
			}

			return false;
		}


		//lv 2

		bool insert(const string& name)
		{
			auto pos = list_map.find(name);
			if (pos != list_map.end())
			{
				string key;
				string value;

				cout << "please input key and value:" << endl;
				cin >> key;
				cin >> value;

				cin.clear();
				cin.ignore(1024 , '\n');

				(*pos).second.insert(key , value);

				return true;
			}

			return false;
		}

		bool erase(const string& name)
		{
			auto pos = list_map.find(name);
			if (pos != list_map.end())
			{
				string key;

				cout << "please input the key" << endl;
				cin >> key;

				cin.clear();
				cin.ignore(1024 , '\n');

				(*pos).second.erase(key);

				return true;
			}

			return false;
		}

		bool fix(const string& name)
		{
			auto pos = list_map.find(name);
			if (pos != list_map.end())
			{
				string key;
				string value;

				cout << "please input key and value:" << endl;
				cin >> key;
				cin >> value;

				cin.clear();
				cin.ignore(1024 , '\n');
				
				return (*pos).second.fix(key , value);
			}

			return false;
		}

		bool clear(const string& name)
		{
			auto pos = list_map.find(name);
			if (pos != list_map.end())
			{
				(*pos).second.clear();

				return true;
			}

			return false;
		}


		//help
		
		void help()
		{
			cout << "-------------commands--------------" << endl;
			cout << "create		(list name)" << endl;
			cout << "display		(list name)" << endl;
			cout << "destory		(list name)" << endl;
			cout << "clear		(list name)" << endl;
			cout << "insert		(list name)" << endl;
			cout << "fix		(list name)" << endl;
		}


		//load & save list name

		void load()
		{
			reader.open(PATH);
			string name;

			while (getline(reader , name))
			{
				if (name.empty())
					break;

				create(name);
			}

			reader.close();
		}

		void save()
		{
			writer.open(PATH);
			
			for (auto p : list_map)
			{
				writer << p.first << endl;
			}

			writer.flush();
			writer.close();
		}
};