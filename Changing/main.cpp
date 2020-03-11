#include "iostream"
#include "string"
#include "fstream"

using namespace std;

template <typename K, typename T>
struct List
{
	struct Item
	{
		K key;
		T value;
		Item* next;

		Item() { next = NULL; }
		Item(K key_arg, T value_arg) : key(key_arg), value(value_arg) { next = NULL; }

		~Item()
		{
			next = NULL;
		}
	};
	Item* head;
	Item* tail;

	List()
	{
		head = NULL;
		tail = NULL;
	}


	bool delete_item(K key)
	{
		Item* tmp;

		if (head != NULL)
		{
			tmp = head->next;
			//Если это начало, то говорим, что второй элемент становится первым
			if (head->key == key)
			{
				delete head;
				head = tmp;
				return 1;
			}
			//Иначе производим стандартное удаление из списка
			while (tmp != NULL && tmp->next->key != key)
			{
				tmp = tmp->next;
			}
			if (tmp != NULL)
			{
				Item* del = tmp->next;
				tmp->next = del->next;
				delete del;
				return 1;
			}
		}

		return 0;
	}
	void delete_head()
	{
		Item* tmp;
		tmp = head->next;
		delete head;
		head = tmp;
	}

	void clear_list()
	{
		while (head != NULL)
		{
			delete_head();
		}
	}

	void allocate(K key, T value)
	{
		if (head)
		{
			tail->next = new Item(key, value);
			tail = tail->next;
		}
		else
		{
			head = new Item(key, value);
			tail = head;
		}
	}

	Item* find(K key)
	{
		Item* tmp = head;
		while (tmp != NULL && tmp->key != key)
		{
			tmp = tmp->next;
		}
		return tmp;
	}

};

template <typename K, typename T>
class HashMap {
	List<K, T>* date;
	int length;
	const int step = 10;
	int count;
public:

	HashMap() {
		count = 0;
		length = step;
		date = new List<K, T>[length];
	}

	~HashMap()
	{
		for (int i = 0; i < length; i++)
		{
			date[i].clear_list();
		}
		delete date;
	}

	int get_hash(K key)
	{
		tr1::hash<K> simple_hash;
		return abs((int)simple_hash(key)) % length;
	}

	//Добавление элемента в хеш-таблицу
	void add(K key, T value);
	//Удаление элемента из хэш-таблицы
	void delete_element(K key);
	//Поиск элемента по ключу
	typename List<K, T>::Item* find(K key);

	void rehash();

	int is_rehash()
	{
		return (count >= (int)(length / 2));
	}

	int length_hash_map()
	{
		return count;
	}

	int get_length()
	{
		return length;
	}

	int count_unique_items();

	class Iterator {
		List <K, T>* map;
		typename List<K, T>::Item* item;
		int last_index;
		int index;

	public:
		Iterator(List<K, T>* map, typename List<K, T>::Item* item, int index, int last_index) : map(map),
			item(item), index(index), last_index(last_index) {}

		bool isNULL()
		{
			return (map == NULL);
		}

		void delete_item()
		{
			typename List<K, T>::Item* tmp = item->next;

			delete item;
			item->next = NULL;
			if (tmp != NULL)
			{
				item = tmp;
				map[index].head = tmp;
			}
			else
			{
				++(*this);
			}
		}

		Iterator& operator++()
		{
			if ((item->next != NULL || check_final_item(index)))
			{
				item = item->next;
				return *this;
			}
			else
			{
				for (++index; index < last_index; index++)
				{
					if (map[index].head != NULL)
					{
						item = map[index].head;
						return *this;
					}
				}
			}
		}

		bool operator!=(Iterator it)
		{
			if (item == it.item)
			{
				return 0;
			}
			return 1;
		}

		typename List<K, T>& operator*()
		{
			return map[index];
		}

		typename List<K, T>::Item* operator->()
		{
			return item;
		}


		bool check_final_item(int ind)
		{
			for (int i = last_index - 1; i >= 0; i--)
			{
				if (map[i].tail != NULL)
				{
					if (ind == i)
					{
						return 1;
					}
					return 0;
				}

			}
		}
	};

	Iterator begin() const
	{
		for (int i = 0; i < length; i++)
		{
			if (date[i].head != NULL)
			{
				return Iterator(date, date[i].head, i, length);
			}
		}

		return Iterator(NULL, NULL, 0, length);
	}

	Iterator end() const {
		for (int i = length - 1; i >= 0; i--)
		{
			if (date[i].tail != NULL)
			{
				return Iterator(date, date[i].tail->next, i, length);
			}
		}
	}

};

template<typename K, typename T>
void HashMap<K, T>::add(K key, T value)
{
	int keys = get_hash(key);
	if (date[keys].find(key) != NULL)
	{
		date[keys].find(key)->value = value;
	}
	else
	{
		date[keys].allocate(key, value);
		count++;
		if (is_rehash())
		{
			rehash();
		}
	}
}

template<typename K, typename T>
typename List<K, T>::Item* HashMap<K, T>::find(K key)
{
	typename List<K, T>::Item* tmp = date[get_hash(key)].find(key);
	return date[get_hash(key)].find(key);
}

template<typename K, typename T>
typename void HashMap<K, T>::delete_element(K key)
{
	//Берем индекс, в котором будем искать удаляемый элемент
	int keys = get_hash(key);
	if (date[keys].delete_item(key))
	{
		count--;
	}
}

template<typename K, typename T>
typename void HashMap<K, T>::rehash()
{
	int n = length;
	length *= step;
	List<K, T>* tmp_list = new List<K, T>[length];

	int key = 0;

	for (int i = 0; i < n; i++)
	{
		while (date[i].head != NULL)
		{
			int keys = get_hash(date[i].head->key);
			tmp_list[keys].allocate(date[i].head->key, date[i].head->value);
			date[i].delete_head();
		}
	}
	delete date;
	date = tmp_list;


}

template<typename K, typename T>
typename int HashMap<K, T>::count_unique_items()
{
	int unique = 0;

	auto it = begin();
	if (it.isNULL())
	{
		return 0;
	}

	for (it; it != end(); ++it)
	{
		auto ij = it;
		++ij;
		bool check = 1;
		for (ij; ij != end(); ++ij)
		{
			if (it->value == ij->value)
			{
				check = 0;
			}
		}
		unique += check;
	}

	return unique;
}

template<typename K, typename T>
void work_function(K key, T value, ifstream& fin, int n)
{
	HashMap<K, T> a;

	for (int i = 0; i < n; i++)
	{
		string tmp;
		fin >> tmp;
		if (tmp[0] == 'A')
		{
			fin >> key;
			fin >> value;
			a.add(key, value);
		}
		if (tmp[0] == 'R')
		{
			fin >> key;
			a.delete_element(key);
		}
	}

	ofstream fout("output.txt");
	if (!fout)
	{
		cout << "Output file doesn't exist";
	}
	fout << a.length_hash_map() << " " << a.count_unique_items();

}


template<typename K>
void read_value(K key, char value, ifstream& fin, int n)
{
	switch (value)
	{
	case ('I'):
	{
		int tmp = 0;
		work_function(key, tmp, fin, n);
		break;
	}
	case('D'):
	{
		double tmp = 0;
		work_function(key, tmp, fin, n);
		break;
	}
	case('S'):
	{
		string tmp = "";
		work_function(key, tmp, fin, n);
		break;
	}
	}
}


int main()
{
	ifstream fin("input.txt");

	if (!fin)
	{
		cout << "Input file doesn't exist";
	}

	int n;
	string tmp;
	getline(fin, tmp);
	char type_dates[2] = { '\0' };
	type_dates[0] = tmp[0];
	type_dates[1] = tmp[2];
	fin >> n;
	getline(fin, tmp);
	switch (type_dates[0])
	{
		case ('I'):
		{
			int tmp_key = 0;
			read_value(tmp_key, type_dates[1], fin, n);
			break;
		}
		case('D'):
		{
			double tmp_key = 0;
			read_value(tmp_key, type_dates[1], fin, n);
			break;
		}
		case('S'):
		{
			string tmp_key = "";
			read_value(tmp_key, type_dates[1], fin, n);
			break;
		}
	}


	return 0;
}