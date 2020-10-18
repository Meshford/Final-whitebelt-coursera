#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <stdexcept>
#include <cmath>
#include <exception>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include "stdlib.h"
using namespace std;

class Date {
public:
	Date() {
		Year = 0;
		Day = 0;
		Month= 0;
	}
	Date(int N, int M, int L) {
		Year = N;
		Month = M;
		Day = L;
	}
	int GetYear() const {
		return Year;
	}
	int GetMonth() const {
		return Month;
	}
	int GetDay() const {
		return Day;
	}
	friend istream& operator>>(istream &is, Date &op2)//организовать чтение из потока
	{

		string date;
		is >> date; 

		istringstream input(date);
		bool ok = true;

		int year;
		ok = ok && (input >> year);
		ok = ok && (input.peek() == '-');
		input.ignore(1);

		int month;
		ok = ok && (input >> month);
		ok = ok && (input.peek() == '-');
		input.ignore(1);

		int day;
		ok = ok && (input >> day);
		ok = ok && input.eof();

		if (!ok) {
			throw invalid_argument("Wrong date format: " + date);
		}
		if ((month > 12) || (month < 1)) {
			throw invalid_argument("Month value is invalid: " + to_string(month));
		}
		if ((day < 1) || (day > 31)) {
			throw invalid_argument("Day value is invalid: " + to_string(day));
		}
		
		op2=Date(year, month, day);
		return is;
	}
	friend ostream& operator<<(ostream &os, const Date &op2)//организовать вывод 
	{
		//cout<<setw(3);задать ширину поля
		//cout<<setprecision;точность вывода вещественных чисел
		//cout<<setwfill(' ');заполнитель для setw(3)
		os << setw(4) << setfill('0') << op2.Year << "-" << setw(2) << op2.Month << "-" << setw(2)  << op2.Day;
		return os;
	}
	bool operator<(const Date& rhs) const {
		bool res;
		if (Year == rhs.Year) {
			if (Month == rhs.Month) {
				if (Day < rhs.Day) {
					res = true;
				}
				else res = false;
			}
			else if (Month < rhs.Month) {
				res = true;
			}
			else res = false;
		}
		else if (Year < rhs.Year) {
			res = true;
		}
		else res = false;
		return res;
	}
private:
	int Year;
	int Month;
	int Day;
};

class Database {
public:

	void AddEvent(const Date& date, const string& event) {
		set<string> st=Da[date];
		st.insert(event);
		Da[date] = st;
	}
	bool DeleteEvent(const Date& date, const string& event) {
		bool res=false;
		set<string> st = Da[date];
		if (st.find(event) != st.end()) { 	//значит нашл элемент
			st.erase(st.find(event));//удаляем элемент который нашли
			res = true;
		}
		Da[date] = st;//присваиваем обновленное множество
		return res;
	}

	int DeleteDate(const Date& date) {
		int n;
		try {
			n = Da.at(date).size();
		}
		catch (out_of_range &) {
			return 0;
		}
		Da.erase(date);
		return n;
	}

	void Find(const Date& date) const {
		if (Da.count(date) > 0) {
			set<string> s = Da.at(date);
			for (auto i : s) {
				cout << i << endl;//выводим по одному на строке
			}
		}
	}

	void Print() const {
		for (auto i : Da) {
			cout << i.first<<" ";
			for (auto j : i.second) {
				cout << j <<" ";
			}
			cout << endl;
    	}
	}

private:
	map<Date, set<string>> Da;
};


int Num_of_space(const string &s) {
	int k = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ')k++;
	}
	return k;
}

void CheckExseption(const string&s) {
	if (!((s == "Del")||(s=="Add")||(s=="Print")||(s=="Find"))) {
		throw invalid_argument("Unknown command: "+s);
	}
}
int main() {
	Database db;
	string command;


	while (getline(cin, command)) {
		string event;
		string com;
		Date date;
		string dat;
		stringstream input(command);
		int k = Num_of_space(command);
		try {
			input >> com;
			if (com.empty()) {
				continue;
			}
			CheckExseption(com);
		}
		catch (invalid_argument&ex) {
			cout << ex.what();
		}
		
		if (com == "Add") {
			bool ka = false;
			try {
				input >> date;
			}
			catch (invalid_argument &ex) {
				cout << ex.what();
				ka = true;
			}
			if (!ka) {
				input >> event;
				db.AddEvent(date, event);
			}
		}
		else if (com == "Find") {
			bool ki = false;
			try {
				input >> date;
			}
			catch (invalid_argument &ex) {
				cout << ex.what();
				ki = true;
			}
			if (!ki) 
			db.Find(date);
		}
		else if (com == "Del") {
			bool ke=false;
			try {
				input >> date;
			}
			catch (invalid_argument&ex) {
				cout << ex.what();
				ke = true;
			}
			if (!ke) {
				if (k == 1) {

					int count = db.DeleteDate(date);
					cout << "Deleted " << count << " events" << endl;
				}
				if (k == 2) {
					input >> event;
					if (db.DeleteEvent(date, event)) {
						cout << "Deleted successfully" << endl;
					}
					else cout << "Event not found" << endl;
				}
			}
		}
		else if (com == "Print") {
			db.Print();
		}
	}
	return 0;
}

/*
Date EnsureNextSymbolAndSkip(const string &dat) {

	istringstream input(dat);
	bool ok=true;

	int year;
	ok = ok && (input >> year);
	ok = ok && (input.peek() == '-');
	input.ignore(1);

	int month;
	ok = ok && (input >> month);
	ok = ok && (input.peek() == '-');
	input.ignore(1);

	int day;
	ok = ok && (input >> day);
	ok = ok && input.eof();

	if (!ok) {
		throw logic_error("Wrong date format: " + dat);
	}
	if ((month > 12) || (month < 0)) {
		throw logic_error("Month value is invalid: " + to_string(month));
	}
	if ((day < 1) || (day > 31)) {
		throw logic_error("Day value is invalid: " + to_string(day));
	}

	 Date res(year, month, day);
	return res;
}*/