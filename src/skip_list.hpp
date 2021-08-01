/*skip_list
*/

#pragma once
#include<iostream>
#include<time.h>
#include<random>
#include<vector>
#include<mutex>
#include<fstream>
#include<string>
#include<sstream>

using std::istringstream;
using std::string;
using std::ifstream;
using std::ofstream;
using std::unique_lock;
using std::mutex;
using std::default_random_engine;
using std::uniform_int_distribution;
using std::cout;
using std::endl;
using std::vector;


template<typename kt,typename vt>
class skip_list
{
	struct node
	{
		kt key;
		vt value;
		int node_level;

		vector<node*> next;

		node() = delete;

		node(int level):node_level(level)
		{
			for (int n = 0; n < level; ++n)
			{
				next.push_back(nullptr);
			}
		}

		node(kt k , vt v , int level) :key(k) , value(v) , node_level(level)
		{
			for (int n = 0; n < level; ++n)
			{
				next.push_back(nullptr);
			}
		}
	};

	public:
		using key_type = kt;
		using value_type = vt;

	private:
		node* head = nullptr;

		mutex mute;
		ofstream writer;
		ifstream reader;

		int elem_count = 0;
		int max_level = 0;

		string PATH = "D:/";

	public:
		skip_list() = delete;

		skip_list(int max_lv,const string& name) :max_level(max_lv)
		{
			head = new node(max_level);
			list_path(name);
			load();
		}

		skip_list(const skip_list<kt , vt>& obj)
		{
			this->elem_count = obj.elem_count;
			this->max_level = obj.max_level;
			this->head = new node(max_level);
			this->head->next = obj.head->next;
			this->PATH = obj.PATH;
		}

		~skip_list()
		{
			delete head;
			head = nullptr;
		}



		const node* find(kt key) const
		{
			return self_find(key);
		}

		int size() const
		{
			return elem_count;
		}


		bool fix(const kt key , const kt value)
		{
			node* fix_node = self_find(key);
			if (fix_node != nullptr)
			{
				fix_node->value = value;

				save();
				return true;
			}

			return false;
		}

		void insert(const kt key , const vt value)
		{
			mute.lock();

			node* ptr = head;
			node* new_node = self_find(key);

			//node exist
			if (new_node != nullptr)
			{
				new_node->value = value;
				mute.unlock();

				save();
				return;
			}

			//create and insert node node

			int level = get_random_level();
			new_node = new node(key , value , level);

			for (int n = level - 1; n >= 0; --n)
			{
				while (ptr->next[n] != nullptr && ptr->next[n]->key < key)
				{
					ptr = ptr->next[n];
				}

				new_node->next[n] = ptr->next[n];
				ptr->next[n] = new_node;
			}

			elem_count++;
			mute.unlock();

			save();
		}

		void erase(kt key)
		{
			unique_lock<mutex> lock(mute);
			node* del = self_find(key);

			if (del != nullptr)
			{
				node* ptr = head;
				int level = del->node_level;

				for (int n = level - 1; n >= 0; --n)
				{
					while (ptr->next[n] != nullptr && ptr->next[n]->key < key)
					{
						ptr = ptr->next[n];
					}

					ptr->next[n] = ptr->next[n]->next[n];
				}

				elem_count--;
				save();
			}
		}

		void clear()
		{
			head->next.clear();

			for (int n = max_level - 1; n >= 0; --n)
			{
				head->next.push_back(nullptr);
			}

			elem_count = 0;
			save();
		}

		void destory()
		{
			remove(PATH.c_str());
		}

		void display() const
		{
			for (int n = 0; n < max_level; ++n)
			{
				cout << "level: " << n + 1 << endl;

				node* ptr = head->next[n];
				while (ptr != nullptr)
				{
					cout << ptr->key << ":" << ptr->value << " ";
					ptr = ptr->next[n];
				}
				cout << endl;
			}cout << endl;
		}


	private:
		node* self_find(kt key)
		{
			node* ptr = head;

			for (int n = max_level - 1; n >= 0; --n)
			{
				while (ptr->next[n] != nullptr && ptr->next[n]->key < key)
				{
					ptr = ptr->next[n];
				}
			}

			ptr = ptr->next[0];

			if (ptr != nullptr && ptr->key == key)
				return ptr;

			return nullptr;
		}


		int get_random_level()
		{
			int level = 1;
			static default_random_engine e(time(NULL));
			static uniform_int_distribution<unsigned> u(0 , 1);

			while (u(e) && level < max_level)
			{
				level++;
			}

			return level;
		}

		void load()
		{
			reader.open(PATH);

			string line;
			string s_key;
			string s_value;
			key_type key;
			value_type value;

			while (std::getline(reader , line))
			{
				get_data_from_string(line , s_key , s_value);
				if (s_key.empty() || s_value.empty())
					continue;

				key = StringToType<key_type>(s_key);
				value = StringToType<value_type>(s_value);
				insert(key , value);
			}

			reader.close();
		}

		void save()
		{
			writer.open(PATH);

			node* ptr = head->next[0];

			while (ptr != nullptr)
			{
				writer << ptr->key << ":" << ptr->value << endl;
				ptr = ptr->next[0];
			}

			writer.flush();
			writer.close();
		}

		void get_data_from_string(const string& str , string& key , string& value)
		{
			if (str.empty() || str.find(":") == string::npos)
			{
				return;
			}

			key = str.substr(0 , str.find(":"));
			value = str.substr(str.find(":") + 1 , str.length());
		}

		template<typename ty>
		ty StringToType(const string& str)
		{
			istringstream iss(str);
			ty res;
			iss >> res;

			return res;
		}

		//create list path
		void list_path(const string& name)
		{
			PATH += name + ".txt";
		}
};