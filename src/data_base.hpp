#pragma once

#include<iostream>
#include"command.hpp"

using std::getline;
using std::cout;
using std::cin;
using std::endl;

class data_base
{
	private:
		string s_command;
		command com;

	public:
		void run()
		{	
			cout << "data base running" << endl;
			while (1)
			{
				cout << "input command:" << endl;
				getline(cin , s_command);

				if (s_command == "exit" || s_command == "quit")
					break;

				com.get_command(s_command);
				s_command.clear();

				system("CLS");
			}
			cout << "data base exit" << endl;
		}
};