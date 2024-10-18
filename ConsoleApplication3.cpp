#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>

using namespace std;

class Meter {

private:
	struct Pressure {
		string date;
		double height;
		int value;
	};

	vector <Pressure> pressure_v;

public:

	// Считывание файла
	vector<string> read_file(ifstream& in) {
		vector<string> str;
		string buf;
		while (!in.eof()) {
			getline(in, buf);;
			str.push_back(buf);
		}
		return str;
	}

	string save_date(string str) {
		regex datePattern(R"(([0-2]\d{3})\.([01]\d)\.([0-3]\d))"); // regex даты ГГГГ.ММ.ДД
		smatch match;
		if (regex_search(str, match, datePattern)) {
			return match.str();
		}
	}

	double save_height(string str) {
		regex doublePattern(R"(\d+,\d)"); // regex высоты 0.0
		smatch match;
		if (regex_search(str, match, doublePattern)) {
			return stod(match.str());
		}
		else return -1;
	}

	int save_value(string str) {
		regex intPattern(R"((?:\s|^)\d*(?:\s|$))"); // regex высоты 0.0
		smatch match;
		if (regex_search(str, match, intPattern)) {
			return stoi(match.str());
		}
	}

	// Фильтруем данные по регулярным выражениям
	void save_info(vector<string> str) {

		for (const auto &item: str) {

			try {

				pressure_v.push_back({ save_date(item), save_height(item), save_value(item) });

			} catch (...) {
				cout << "Не удалось считать данные ( " << item << " ), проверьте файл" << endl;
			}
		}
	}

	// Ввод структур данных в вектор
	void save_in_pressure_v(Pressure pressure_s) {
		pressure_v.push_back(pressure_s);
	}

	// Оператор для сортировки по дате
	struct comp {
		bool operator()(const Pressure& item1, const Pressure& item2) {
			return ((item1.height == item2.height) &&
				((stoi(item1.date.substr(5, 2)) == stoi(item2.date.substr(5, 2)) &&
					stoi(item1.date.substr(8, 2)) < stoi(item2.date.substr(8, 2))) ||
					stoi(item1.date.substr(5, 2)) < stoi(item2.date.substr(5, 2))));
		}
	};
	
	// Сортировка вектора pressure_v
	void sort_v() {

		sort(pressure_v.begin(), pressure_v.end(), 
			[](const Pressure& item1, const Pressure& item2){return item1.height < item2.height; });


		sort(pressure_v.begin(), pressure_v.end(), comp());
	}

	// Вывод данных в командную строку
	void cout_result() const {
		cout << "Данные с файла:" << endl;
		for (const auto& item : pressure_v) {
			printf("%s %.1f %d\n", item.date.c_str(), item.height, item.value);
		}
	}

	void PrintByHeight() {
		double h;
		cout << "Введите искомую глубину: ";
		cin >> h;
		bool flag = false;
		for (auto& Pressure : pressure_v) {
			if (Pressure.height == h) {
				cout << Pressure.date << " " << Pressure.height << " " << Pressure.value << endl;
				flag = true;
			}
		}
		if (!flag) {
			cout << "Ошибка: Введенная глубина не найдена!" << endl;
		}
		
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream in("info.txt");

	Meter result;
	// Вектор из всех считанных данных
	vector<string> str = result.read_file(in);
	// Разбиение данных по структурам и занесение их в единый вектор
	result.save_info(str);
	// Сортировка вектора структур
	result.sort_v();

	// Вывод данных
	result.cout_result();

	//Вывод по искомой глубине
	result.PrintByHeight();
}