//Разделяет строку по разделителю.
vector <string> Split(string arg, char separator)
{
	vector <string> v;
	string s;	
	arg.push_back(separator);
	for (unsigned int i = 0; i < arg.size(); i++)
	{
		if (arg[i] != separator)
		{
			s.push_back(arg[i]);
		}
		if (arg[i] == separator)
		{
			v.push_back(s);
			s.clear();
		}
	}
	return v;
}
//Объединяет элементы вектора в строку
string Merge(vector <string> way, char separator)
{
	string key;
	if (way.size() == 1)
	{
		key = way[0];
	}
	if (way.size() >= 2)
	{
		while (way.size() != 0)
		{
			if (way.size() == 1)
			{
				key = key + way[way.size() - 1];
				way.pop_back();
				continue;
			}
			key = key + way[way.size() - 1] + separator;
			way.pop_back();
		}
	}
	return key;
}
//Инвертирует вектор
vector <string> Reverse(vector <string> arg)
{
	if (arg.size() % 2 == 0)
	{
		for (unsigned int i = 0; i < arg.size() / 2; i++)
		{
			string temp = arg[i];
			arg[i] = arg[arg.size() - 1 - i];
			arg[arg.size() - 1 - i] = temp;
		}
	}
	else
	{
		for (unsigned int i = 0; i < (arg.size() - 1) / 2; i++)
		{
			string temp = arg[i];
			arg[i] = arg[arg.size() - 1 - i];
			arg[arg.size() - 1 - i] = temp;
		}
	}
	return arg;
}
//Проверка аргументов (первая - на пробелы)
bool FirstCheckingArguments(string argument)
{
	if (argument.empty() == true)
		return false;
	if (argument[0] == ' ')
		return false;
	int count = 0;
	int begin = argument.find(" ");
	int end = argument.rfind(" ");
	if (begin == -1 || end == -1)
		return false;
	for (int i = begin; i <= end; i++)
	{
		if (argument[i] == ' ')
			count++;
	}
	if (count > 2)
		return false;
	return true;
}
//Проверка ключа и данных отдельно
bool SecondCheckingArguments(string command, string way, string data)
{
	if (command == "print" && way == "alltree")
		return true;
	if (data == "")
		return false;
	if (data != "zero")
	{
		if (data.find(",") == -1)
		{
			for (unsigned int i = 0; i < data.size(); i++)//Если папка
			{
				if (data[i] < '0' && data[i] > '9')
					return false;
			}
			return true;
		}
		for (unsigned int i = data.find(",") + 1; i < data.size(); i++)//Размер
		{
			if (data[i] < '0' && data[i] > '9')
				return false;
		}
		for (unsigned int i = 0; i < data.find(",") - 1; i++)//Расширение проверяем
		{
			if ((data[i] < 'a' && data[i] > 'z') || (data[i] < 'A' && data[i] > 'Z'))
				return false;
		}
		return true;
	}
	int begin = way.find(":");
	int end = way.rfind(":");
	if (begin != end || begin == -1)//Если двоеточий больше 1 или нет вообще
		return false;
	if (way[way.size() - 1] != ':' && way.find("/") == -1)//Если слэша нету в строке и двоеточие не в конце
		return false;
	int pos = way.find("/");
	if (pos != -1 && pos < begin)//Если слэш есть и стоит перед двоеточием
		return false;
	if (way[0] == ':' || way[0] == '/' || way[way.size() - 1] == '/')
		return false;
	if (way.rfind("/") != -1)
	{
		for (unsigned int i = pos; i <= way.rfind("/"); i++)
		{
			if (way[i] == '/' && way[i + 1] == '/')
				return false;
		}
	}
	for (unsigned int i = 0; i < way.size(); i++)
	{
		if (way[i] == '*' || way[i] == '?' || way[i] == '<' || way[i] == '>' || way[i] == '|' || way[i] == '«' || way[i] == '»')
			return false;
	}
	return true;
}