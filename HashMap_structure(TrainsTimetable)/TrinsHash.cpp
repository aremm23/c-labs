#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Trains {
	string Day;
	string City;
	string Time;
	string Places;
	Trains* next = nullptr;
};

void Add_to_list_with_searched_trains(Trains** pbeg, Trains* pv1);
void Case2(int size, Trains** trains);
int Print_from_list_with_searched_trains(Trains* pbeg);
Trains* Find_certain_train(Trains* pbeg, int trainNum);
void Change_num_of_free_places(Trains** trains, const int index, Trains* pvCurr);
int Search(const string city, const int size, Trains** table, Trains** pbeg);
void RezervToFile(Trains* pv, int places);

//>>>-------------------------------------------->>> Основные функции <<<-------------------------------------------->>>


int GetHash(const string key) {//находит хеш
	int hash = 0;
	for (char ch : key) {
		hash = hash * 31 + ch;
	}
	return abs(hash);
}

int GetIndex(const string key, int size) {//находит индекс по хешу
	int hash = GetHash(key);
	int index = hash % size;
	return index;
}

int NumOfTrains(string file_name) {//ищет сколько строк в файле, тем самым определяя сколько поездов в файле с данными
	int AmountOfTrains = 0;
	ifstream file(file_name);

	if (!file.is_open()) return 0;
	else {
		string str;
		while (!file.eof()) {
			getline(file, str);
			AmountOfTrains++;
		}
		file.close();
	}
	if (AmountOfTrains < 1) return 0;

	return AmountOfTrains;
}

int GetSize(string file_name, int* AmountOfTrains) {//выбирает размер массива
	*AmountOfTrains = NumOfTrains(file_name);

	int arr_sizes[] = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384 };
	int i = 0;
	int size;
	while (arr_sizes[i] < *AmountOfTrains || i != 11) {
		if (arr_sizes[i] < *AmountOfTrains) i++;
		else {
			if (arr_sizes[i] - arr_sizes[i] / 4 < *AmountOfTrains) i++;
			else {
				size = arr_sizes[i];
				break;
			}
		}
	}
	if (i == 11) return 0;
	return size;
}

bool CheckDigit(string numstr) {//проверка на числовой ввод

	for (int i = 0; i < numstr.length(); i++) {
		if(!isdigit(numstr[i])) return false;
	}
	return true;
}


//>>>-------------------------------------------->>> Поиск элемента <<<-------------------------------------------->>>

// Case2() основная функция для нахождения конкретного поезда, и дальнейшего резервирования места
// в Search() находиться индекс в хеш таблице для города, введенного пользователем
// далее создается список со всеми поездами с одинаковым индексом
// указатель pbeg на первый элемент находиться в Case2()
// в консоль выводятся все найденные поезда, юзер выбирает номер и Find_certain_train() возращает указатель pv на этот поезд
// далее мы апперируем с указателем pv на выбранный поезд
// пользователь вводит сколько мест зарезервировать, после это кол во мест вычитается и перезаписывается в список
// затем из списка кол во мест перезаписывается и  в саму хэш таблицу
// создается файл Rezerved.txt
// и уже в конце выполнения программы(после выхода из цикла в main()) все измененные поезда перезаписываются в файл


void Case2(int size, Trains** trains) {
	bool stop = true;
	string CheckBuffer;
	
	string City_to_find;
	cout << "Train to which city do you want to find: ";
	cin >> City_to_find;

	Trains* pbeg = nullptr;

	int index = Search(City_to_find, size, trains, &pbeg);
	cout << "Trains to " << City_to_find << ":\n";
	int NumOfTrains = Print_from_list_with_searched_trains(pbeg);
	if (NumOfTrains == 0) { cout << "There is no trains to city " << City_to_find << endl; }
	else {
		cout << "Enter which number of train which you want to rezerve: ";

		int TrainNum;
		while (stop) {
			cin >> CheckBuffer;
			if (CheckDigit(CheckBuffer)) {
				stop = false;
				TrainNum = stoi(CheckBuffer);
			}
			else cout << "Please enter numerical value\n";
		}
		stop = true;

		Trains* pv = Find_certain_train(pbeg, TrainNum);
		
		if (pv == nullptr) { cout << "There is no such train\n"; return; }
		
		int places;
		cout << "How many places do you want to rezerve: ";

		while (stop) {
			cin >> CheckBuffer;
			if (CheckDigit(CheckBuffer)) {
				stop = false;
				places = stoi(CheckBuffer);
			}
			else cout << "Please enter numerical value\n";
		}
		stop = true;

		if (pv->Places == "FINISHED") { cout << "There is no places in this train\n"; }
		else {
			int freePlaces = stoi(pv->Places);

			if (places > freePlaces) {
				cout << "There is only " << freePlaces << " left." << endl;
			}
			else {
				freePlaces -= places;
				if (freePlaces == 0) pv->Places = "FINISHED";
				else pv->Places = to_string(freePlaces);

				Change_num_of_free_places(trains, index, pv);
				RezervToFile(pv, places);

			}
		}
	}
}

void Add_to_list_with_searched_trains(Trains** pbeg, Trains* pv1) {
	Trains* pv2 = new Trains;
	pv2->Day = pv1->Day;
	pv2->City = pv1->City;
	pv2->Time = pv1->Time;
	pv2->Places = pv1->Places;
	pv2->next = nullptr;

	if (*pbeg == nullptr) {
		*pbeg = pv2;
	}
	else {
		Trains* pv_SearchLast = *pbeg;
		while (pv_SearchLast->next != nullptr) pv_SearchLast = pv_SearchLast->next;
		pv_SearchLast->next = pv2;
	}
}

int Print_from_list_with_searched_trains(Trains* pbeg) {
	int counter = 0;
	Trains* pv = pbeg;
	
	while (pv != nullptr) {
		cout << counter + 1 << ". Train to " << pv->City << " at " << pv->Day << " on " << pv->Time << ". Places available: " << pv->Places << endl;
		counter++;
		pv = pv->next;
	}
	return counter;
}

Trains* Find_certain_train(Trains* pbeg, int trainNum) {
	int counter = 0;
	Trains* pv = pbeg;
	while (pv != nullptr) {
		if (counter + 1 == trainNum) {
			cout << counter + 1 << ". Train to " << pv->City << " at " << pv->Day << " on " << pv->Time << ". Places available: " << pv->Places << endl;
			return pv;
		}
		counter++;
		pv = pv->next;
	}
	return nullptr;
}

void Change_num_of_free_places(Trains** trains, const int index, Trains* pvCurr) {
	Trains* pvTable = trains[index];
	while (pvTable != nullptr) {
		if (pvTable->Day == pvCurr->Day && pvTable->Time == pvCurr->Time && pvTable->City == pvCurr->City) {
			pvTable->Places = pvCurr->Places;
		}
		pvTable = pvTable->next;
	}
}

int Search(const string city, const int size, Trains** table, Trains** pbeg) {
	int index = GetIndex(city, size);

	Trains* pv = table[index];

	if (pv == nullptr) return -1;
	if (pv->next == nullptr) Add_to_list_with_searched_trains(pbeg, pv);
	else {
		while (pv != nullptr) {
			if (pv->City == city) {
				Add_to_list_with_searched_trains(pbeg, pv);
			}
			pv = pv->next;
		}
	}
	return index;
}

void RezervToFile(Trains* pv, int places) {//после бронирования создаёт файл rezerv и записывает туда определённый поезд
	ofstream file;

	file.open("Rezerved.txt", ofstream::app);
	if (!file) cout << "I'm really sorry, but file isn't open";

	file << "\nTo " << pv->City << ":\n" << "\t" << "On " << pv->Day << ", at " << pv->Time << ". You rezerved " << places << " places";
}

//>>>-------------------------------------------->>>  <<<-------------------------------------------->>>

void DeleteTrain(Trains** trains, int index, string city_delete, Trains* pvCurr) {
	Trains* pv = trains[index];
	if (pv->City == pvCurr->City && pv->Time == pvCurr->Time && pv->Day == pvCurr->Day) {
		trains[index] = pv->next;
		delete pv;
	}
	else {
		Trains* pvprev = pv;
		pv = pv->next;

		while (pv != nullptr) {
			if (pv->City == pvCurr->City && pv->Time == pvCurr->Time && pv->Day == pvCurr->Day) {
				pvprev->next = pv->next;
				delete pv;
				break;
			}
			pv = pv->next;
			pvprev = pvprev->next;
		}
	}
}

void Case3(int size, Trains** trains) {
	string City_to_delete;
	string CheckBuffer;
	bool stop = true;

	cout << "Enter city to which you wnt to delete a train\n";
	Trains* pbeg = nullptr;
	cin >> City_to_delete;

	int index = Search(City_to_delete, size, trains, &pbeg);
	cout << "Trains to " << City_to_delete << ":\n";
	int NumOfTrains = Print_from_list_with_searched_trains(pbeg);
	if (NumOfTrains == 0) { cout << "There is no trains to city " << City_to_delete << endl; }
	else {
		cout << "Enter which number of train which you want to delete: ";

		int TrainNum;
		while (stop) {
			cin >> CheckBuffer;
			if (CheckDigit(CheckBuffer)) {
				stop = false;
				TrainNum = stoi(CheckBuffer);
			}
			else cout << "Please enter numerical value\n";
		}
		stop = true;

		Trains* pv = Find_certain_train(pbeg, TrainNum);
		DeleteTrain(trains, index, City_to_delete, pv);

		if (pv == nullptr) { cout << "There is no such train\n"; }

		cout << "Train has been succesfully deleted\n";
	}
}

//>>>-------------------------------------------->>>  <<<-------------------------------------------->>>

void Change_to_table(Trains** trains, const int index, Trains* pvCurr) {
	Trains* pvTable = trains[index];
	while (pvTable != nullptr) {
		if (pvTable->City == pvCurr->City && ((pvTable->Day == pvCurr->Day && pvTable->Time == pvCurr->Time) || (pvTable->Places == pvCurr->Places && pvTable->Time == pvCurr->Time) || (pvTable->Day == pvCurr->Day && pvTable->Places == pvCurr->Places))) {
			pvTable->Places = pvCurr->Places;
			pvTable->Day = pvCurr->Day;
			pvTable->Time = pvCurr->Time;

		}
		pvTable = pvTable->next;
	}
}

void ChangeElem(Trains** trains, const int index, Trains* pv) {
	cout << "1. Day\n2. Time\n3. Available places\n";
	int choise;
	cin >> choise;

	switch (choise) {
		case 1: {
			string day;
			cout << "Enter new day of week: ";
			cin >> day;
			pv->Day = day;
			break;
		}

		case 2: {
			string time;
			cout << "Enter new departure time: ";
			cin >> time;
			pv->Time = time;
			break;
		}

		case 3: {
			string places;
			cout << "Enter new amount of free places: ";
			cin >> places;
			pv->Places = places;
			break;
		}

		default: {
			cout << "Error\n";
			break;
		}
	}
	Change_to_table(trains, index, pv);
}

//>>>-------------------------------------------->>> Файл <-> Хеш таблица <<<-------------------------------------------->>>


void AddElem(const int index, Trains** trains, string Day, string City, string Time, string Places) {//в хэш таблицу
	if (trains[index] == nullptr) {
		Trains* pv = new Trains;
		trains[index] = pv;
		pv->Day = Day;
		pv->City = City;
		pv->Time = Time;
		pv->Places = Places;
		pv->next = nullptr;
	}

	else {
		Trains* pv = trains[index];
		while (pv->next != nullptr) {
			pv = pv->next;
		}
		Trains* pvNew = new Trains;
		pv->next = pvNew;
		pvNew->Day = Day;
		pvNew->City = City;
		pvNew->Time = Time;
		pvNew->Places = Places;
		pvNew->next = nullptr;
	}
}

int From_File_To_HashTable(const string file_name, Trains** trains, const int size, const int AmountOfTrains) {
	ifstream file(file_name);
	if (!file.is_open()) {
		cout << "I'm really sorry, but file isn't open\n";
		return 0;
	}
	string Day;
	string City;
	string Time;
	string Places;
	int index;

	for (int i = 0; i < AmountOfTrains; i++) {
		file >> Day >> City >> Time >> Places;

		index = GetIndex(City, size);
		AddElem(index, trains, Day, City, Time, Places);
	}
}

void From_hashtable_to_file(const string file_name, Trains** trains, const int size) {
	ofstream file;

	file.open(file_name);
	if (!file) cout << "I'm really sorry, but file isn't open";

	for (int i = 0; i < size; i++) {
		Trains* pv = trains[i];
		if (pv != nullptr) {
			while (pv != nullptr) {

				file << pv->Day << " " << pv->City << " " << pv->Time << " " << pv->Places << "\n";
				pv = pv->next;
			}
		}
	}
}


//>>>-------------------------------------------->>>  <<<-------------------------------------------->>>

void Add_to_file(const string file_name, Trains** trains, const int size) {

	string Day, City, Time, Places;

	cout << "Enter city: ";
	cin >> City;
	cout << "Enter time: ";
	cin >> Time;
	cout << "Enter day: ";
	cin >> Day;
	cout << "Enter free places: ";
	cin >> Places;
	
	int index = GetIndex(City, size);
	
	AddElem(index, trains, Day, City, Time, Places);


}

void Print(const int size, Trains** trains) {
	for (int i = 0; i < size; i++) {
		Trains* pv = trains[i];
		if (pv != nullptr) {
			while (pv != nullptr) {
				cout << "Train to " << pv->City << " at " << pv->Day << " on " << pv->Time << ". Places available: " << pv->Places << endl;
				pv = pv->next;
			}
		}
	}
}

int main() {

	string file_name;
	int size;
	int AmountOfTrains;
	bool STOP = true;
	bool stop = true;
	string CheckBuffer;

	cout << "If you want to work with file 'Trains.txt' enter 1, else enter name of file\n";
	getline(cin, file_name);
	if (file_name == "1") file_name = "Trains.txt";
	else file_name += ".txt";


	size = GetSize(file_name, &AmountOfTrains); //инициализируем массив в нулл
	Trains** trains = new Trains * [size];
	for (int i = 0; i < size; i++) {
		trains[i] = nullptr;
	}

	From_File_To_HashTable(file_name, trains, size, AmountOfTrains);

	while (stop) {

		cout << "Menu:\n";
		cout << "1. To look at all trains\n";
		cout << "2. To buy a ticket\n";
		cout << "3. To delete certain train\n";
		cout << "4. To change certain train\n";
		cout << "5. To add train\n";

		int menu;
		cin >> menu;

		if (AmountOfTrains < 1 && (menu == 1 || menu == 2 || menu == 3)) {
			cout << "Your file is empty. You can only add a new train there\n";
		}

		switch (menu) {
		case 1: {
			cout << "------------------>>> Trains from " << file_name << "<<<------------------";
			Print(size, trains);
			break;
		}
		case 2: {
			Case2(size, trains);
			break;
		}

		case 3: {
			Case3(size, trains);
			break;
		}

		case 4: {
			string City_to_change;
			string CheckBuffer;
			bool stop = true;

			cout << "Enter city\n";
			Trains* pbeg = nullptr;
			cin >> City_to_change;


			int index = Search(City_to_change, size, trains, &pbeg);
			cout << "Trains to " << City_to_change << ":\n";
			int NumOfTrains = Print_from_list_with_searched_trains(pbeg);
			if (NumOfTrains == 0) { cout << "There is no trains to city " << City_to_change << endl; }
			else {
				cout << "Enter which number of train which you want to delete: ";

				int TrainNum;
				while (stop) {
					cin >> CheckBuffer;
					if (CheckDigit(CheckBuffer)) {
						stop = false;
						TrainNum = stoi(CheckBuffer);
					}
					else cout << "Please enter numerical value\n";
				}
				stop = true;

				Trains* pv = Find_certain_train(pbeg, TrainNum);

				ChangeElem(trains, index, pv);
			}
			break;
		}

		case 5: {
			Add_to_file(file_name, trains, size);
			break;
		}
		default: {
			stop = false;
			break;
		}
		}
	}
	From_hashtable_to_file(file_name, trains, size);
}
