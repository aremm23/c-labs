#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

struct Trains {
	string Day;
	string Destination;
	string Time;
	string FreePlaces;
};

struct SearchTrains {
	string Day;
	string Destination;
	string Time;
	string FreePlaces;
	int num;

	SearchTrains* next;
};

//---------------------------->>>>>>>>	Функции проверки на ввод	<<<<<<<<----------------------------

void HourToInt(string time, int* hour, int* min) {//строка time (xx:xx), из неё выделяется левая и правая части и преобразуются в int
	int counter = 0;
	////////////////////////////////////////////////////////Ниже ГОВНОКОД не смотреть!!!!!!!!!!!!!!!
	char aS{}, bS{}, cS{}, dS{};
	int a, b, c, d;

	for (int i = 0; time[i] != '\0'; i++)
	{
		counter++;
		if (counter == 1) aS = time[i];
		if (counter == 2) bS = time[i];
		if (counter == 4) cS = time[i];
		if (counter == 5) dS = time[i];
	}

	a = aS - '0';
	b = bS - '0';
	c = cS - '0';
	d = dS - '0';

	*hour = (a * 10) + b;
	*min = (c * 10) + d;
}

bool CheckCity(string city) {
	int counter = 0;
	for (int i = 55; i < 91; i++)
	{
		if (city[0] == char(i)) {
			counter++;
			break;
		}
	}
	for (int i = 1; city[i] != '\0'; i++)
	{
		for (int j = 97; j < 123; j++) {
			if (city[i] == char(j)) {
				counter++;
				j = 122;
			}
		}
	}
	if (counter != city.length()) return false;
	else return true;
}

bool CheckDay(string day) {
	if (day == "Monday" || day == "Tuesday" || day == "Wednesday" || day == "Thursday" ||
		day == "Friday" || day == "Saturday" || day == "Sunday") return true;
	else return false;
}

bool CheckTime(string time) {
	int twodots = 0, counter = 0;
	for (int i = 0; time[i] != '\0'; i++)
	{
		if (time[i] == ':') {
			twodots = i;
			counter++;
		}
		else {
			for (int j = 48; j < 58; j++)
			{
				if (time[i] == char(j)) { counter++; j = 57; }
			}
		}
	}
	if (counter != time.length() || counter != 5 || twodots != 2) return false;
	else {
		int hour, min;

		HourToInt(time, &hour, &min);


		if (hour >= 23 || hour < 0) return false;
		if (min >= 60 || min < 0) return false;


		return true;
	}
	///////////////////////////////////////////////////////////////
}

bool CheckPlace(string place) {
	int counter = 0;
	for (int i = 0; place[i] != '\0'; i++)
	{
		for (int j = 48; j < 58; j++)
		{
			if (place[i] == char(j)) { counter++; j = 57; }
		}
	}
	if (counter != place.length())  return false;
	else return true;
}



//---------------------------->>>>>>>>	Функции для записи и вывода информации в/из файла	<<<<<<<<----------------------------

void OutputAllTrains(Trains* array, int AmountOfTrains, string name) {
	cout << "<<<-----------------------------------Trains from file \"" << name << "\"----------------------------------->>> \n";
	for (int i = 0; i < AmountOfTrains; i++) {
		cout << "Train " << i + 1 << ":" << endl << "Train to " << array[i].Destination << " | On " <<
			array[i].Day << " | Departure at " << array[i].Time << " | Places available: " << array[i].FreePlaces << endl;
	}
}

void AddToList(int mode, Trains* array, int AmountOfTrains, string name) {
	ofstream file;

	file.open(name, ofstream::app);
	if (!file) cout << "I'm really sorry, but file isn't open";

	string value;
	bool stop = true;
	bool stop1 = true;
	int counter = 0;
	int AmountOfTrainsPrev = AmountOfTrains;
	cout << "Please, enter trains destination:\n";
	while (stop1) {
		cin >> value;
		if (CheckCity(value)) stop1 = false;
		else {
			cout << "\nPlease, enter correct city. Make sure the city is capitalized and there are no spaces, numbers or other symbols. Try again:\n";
		}
	}
	stop1 = true;

	while (stop)
	{
		//////////////////////////////////////////////////////////////////пиздец
		AmountOfTrains++;
		Trains* array1 = new Trains[AmountOfTrains - 1];
		for (int i = 0; i < AmountOfTrains - 1; i++) {
			array1[i].Day = array[i].Day;
			array1[i].Destination = array[i].Destination;
			array1[i].Time = array[i].Time;
			array1[i].FreePlaces = array[i].FreePlaces;
		}

		array = new Trains[AmountOfTrains];
		for (int i = 0; i < AmountOfTrains - 1; i++) {
			array[i].Day = array1[i].Day;
			array[i].Destination = array1[i].Destination;
			array[i].Time = array1[i].Time;
			array[i].FreePlaces = array1[i].FreePlaces;
		}
		delete[] array1;

		//////////////////////////////////////////////////////////////////

		array[AmountOfTrains - 1].Destination = value;

		cout << "Please, enter day of the week when train departure: ";
		while (stop1)
		{
			cin >> value;
			if (CheckDay(value)) stop1 = false;
			else cout << "Unknown day of week. Make sure that you wrote it correctly. Try again:\n";
		}
		array[AmountOfTrains - 1].Day = value;
		stop1 = true;

		cout << "Please, enter departure time: ";
		while (stop1)
		{
			cin >> value;
			if (CheckTime(value)) stop1 = false;
			else cout << "Please, write time according to this pattern \"XX:XX\". Try again:\n";
		}
		array[AmountOfTrains - 1].Time = value;
		stop1 = true;

		cout << "Please, enter how many places are available: ";
		while (stop1)
		{
			cin >> value;
			if (CheckPlace(value)) {
				stop1 = false;
				if (stoi(value) > 10000) {
					stop1 = true;
					cout << "Wtf u doing man? Enter real number! ";
				}
			}
			else cout << "You can only enter an integer value. Try again:\n";
		}
		array[AmountOfTrains - 1].FreePlaces = value;
		stop1 = true;


		cout << "If you want to add one more train, enter train's distanation, else enter number \"1\"" << endl;
		cin >> value;
		if (value == "1") stop = false;
	}

	for (int i = AmountOfTrainsPrev; i < AmountOfTrains; i++)
	{
		file << "Train " << i + 1 << ":\n" << array[i].Day << " " << array[i].Destination << " " << array[i].Time << " " << array[i].FreePlaces << "\n";
	}

	OutputAllTrains(array, AmountOfTrains, name);
}

void InputToList(Trains* array, int AmountOfTrains, string name) {//записывает данные в файл
	ofstream file;

	file.open(name);
	if (!file) cout << "I'm really sorry, but file isn't open";

	for (int i = 0; i < AmountOfTrains; i++)
	{
		file << "Train " << i + 1 << ":\n" << array[i].Day << " " << array[i].Destination << " " << array[i].Time << " " << array[i].FreePlaces << "\n";
	}
}



//---------------------------->>>>>>>>	Функции, связанные с поиском и бронированием	<<<<<<<<----------------------------

//Для бронирования:
//
//Функция Search()
//Создается линейный односвязный список, в который записываются поезда в соответствии с городом для поиска
//(если искать пару букв, то результатом поиска будет сразу несколько городов, в названии которых есть буквы в таком порядке как в ключе)
//-----
// далее в консоль выводятся все найденные города, причём если поездов несколько до одного конкретного города, то его название выводится только один раз
//-----
// далее следует выбрать город из предложенных. После выбора, остальные города несовпадающие с выбранным удаляются из списка. И финально выводится в консоль все поезда в определенный город
//-----
// далее все просто. функция RezervTrain, в которой пользователь выбирает конкретный поезд и резервирует места
//-----
// и наконец зарезервированные места записываются в файл rezerved и перезаписываются в файл trains.txt или любой другой выбранный

void RezervToFile(Trains* array, int num, string key, int place) {//после бронирования создаёт файл rezerv и записывает туда определённый поезд
	ofstream file;

	file.open("Rezerved.txt", ofstream::app);
	if (!file) cout << "I'm really sorry, but file isn't open";

	file << "\nTo " << key << ":\n" << "\t" << "On " << array[num].Day << ", at " << array[num].Time << ". You rezerved " << place << " places";
}

void output(SearchTrains* pv, int mode, int number, string* key) {//вывод в списка в консоль
	if (mode == 1) {///вывод в консоль в первый раз
		int counter = 0;
		int counterOfDiffCities = 0;
		string ExactCity;
		string* arr = new string[number];//создаем массив в который записываются выведенные в консоль города
		bool equal;

		while (pv)
		{
			equal = false;
			if (counter == 0) {//первый город 100% выводится
				equal = false;//true - не выводим, false - выводим
				arr[counter] = pv->Destination;//записывается в массив
			}
			else {
				for (int i = 0; i < counter; i++) {//если не первый поезд, то его место пребытия сравнивается с уже выведенными городами в консоль
					//и если такой город уже выводился, то его незачем выводитиь еще раз
					if (pv->Destination == arr[i]) {
						equal = true;
						arr[counter] = pv->Destination;
						break;
					}
				}
				arr[counter] = pv->Destination;
			}
			if (!equal) {//выводим в консоль если надо
				counterOfDiffCities++;
				ExactCity = pv->Destination;
				cout << pv->Destination << " | ";
			}
			pv = pv->next;
			counter++;
		}
		cout << endl;
		if (counterOfDiffCities == 1) *key = ExactCity;
	}
	else if (mode == 2) {//финальный вывод всех поездов в консоль
		cout << "Trains to " << pv->Destination << ":\n";
		while (pv)
		{
			cout << pv->num << ". " << "On " << pv->Day << ", at " << pv->Time << ". Places available: " << pv->FreePlaces << endl;
			pv = pv->next;
		}
	}
}

SearchTrains* DeleteCities(SearchTrains* pbeg, SearchTrains* pend, string key) {//удаление элемента
	SearchTrains* pv = pbeg;
	SearchTrains* pvdel = pv;
	int counter = 0;
	//чтобы поменять указатель предыдущего удаляемому элемента списка, нужно два указателя(pv - предыдущий удаляемому. pvdel - удаляемый элемент
	while (pv->next != NULL) {
		if (pv == pbeg && pv->Destination != key) {
			pbeg = pv->next;
			pv = pv->next;
			delete pvdel;
		}
		else if ((pv->next)->Destination != key) {
			pvdel = pv->next;
			if (pvdel == pbeg) {
				pbeg = pbeg->next;
				delete pv;
			}
			else if (pvdel == pend) {
				pend = pv;
				pv->next = NULL;
				delete pvdel;
			}
			else {
				pv->next = pvdel->next;
				delete pvdel;
			}
		}
		else { pv = pv->next; counter++; }

		if (pv == pbeg && pv->Destination == key) counter++;
		pvdel = pv;
	}
	if (pv == NULL) return 0;

	if (counter == 0) return 0;

	return pbeg;//возращаем указатьлль на начало, чтоб потом можно было обращаться к списку
}

int RezervTrain(Trains* array, int AmountOfTrains, string key, string name) {
	int num;
	int place;
	int freeplaces;
	bool stop = true;

	cout << "\nPlease enter number of train which you want to rezerv: \n";
	cin >> num;


	if (array[num].FreePlaces != "FINISHED") freeplaces = stoi(array[num].FreePlaces);
	else { cout << "There is no available places in this train\n"; return 0; }

	cout << "How many places do you want to rezerv: ";

	stop = true;
	while (stop) {
		cin >> place;
		if (place > freeplaces) cout << "\nThere are no so many places in train. Enter other number of seats you want to rezerv: ";
		else if (place < 1) cout << "\nPlease, enter integer number: ";
		else stop = false;
	}

	freeplaces -= place;
	if (freeplaces >= 1) array[num].FreePlaces = to_string(freeplaces);
	else array[num].FreePlaces = "FINISHED";

	RezervToFile(array, num, key, place);
	InputToList(array, AmountOfTrains, name);

	cout << "Rezervation is complete!. You can find your tickets in file \"Rezerved.txt\"\n";


	return 1;
}

SearchTrains* FirstElement(Trains* array, int TrainNum) {

	SearchTrains* pv = new SearchTrains;

	pv->Day = array[TrainNum].Day;
	pv->Destination = array[TrainNum].Destination;
	pv->Time = array[TrainNum].Time;
	pv->FreePlaces = array[TrainNum].FreePlaces;
	pv->num = TrainNum;

	pv->next = 0;

	return pv;

}

SearchTrains* AddElement(SearchTrains* pend, Trains* array, int TrainNum) {

	SearchTrains* pv = new SearchTrains;

	pv->Day = array[TrainNum].Day;
	pv->Destination = array[TrainNum].Destination;
	pv->Time = array[TrainNum].Time;
	pv->FreePlaces = array[TrainNum].FreePlaces;
	pv->next = 0;
	pv->num = TrainNum;

	pend->next = pv;
	pend = pv;

	return pend;
}

int Search(Trains* array, int AmountOfTrains, string key, string name) {
	size_t const keyLength = key.length();//change int to size_t
	size_t keyLength1 = keyLength;
	int counter = 0;
	int AmountOfCites = 0;

	SearchTrains* pbeg = 0;//указатели на односвязный список городов
	SearchTrains* pend = 0;

	for (int i = 0; i < AmountOfTrains; i++) //i по массиву поездов
	{
		if (array[i].Destination == key) {//если сразу введен город, то сразу добавляем
			if (AmountOfCites == 0) {//для первого города создаем список
				pbeg = FirstElement(array, i);
				pend = pbeg;
			}
			else {//последующие города добавляем в конец списка
				pend = AddElement(pend, array, i);
			}
			AmountOfCites++;
		}
		else {//если же город сразу не введён полностью...
			while (array[i].Destination.length() < keyLength1) { keyLength1--; }//если ключ длиннее названия города, длинна ключа сокращается(чтоб не выйти за пределы массива)
			for (int j = 0; j < keyLength1; j++)//тут если символы в ключе и городе совпадают, то считается что это тот самый город, и можно добавлять его в структуру
			{
				if (array[i].Destination[j] == key[j]) {
					counter++;
				}
				else break;
			}
			if (counter == keyLength) {//добавление в структуру
				if (AmountOfCites == 0) {
					pbeg = FirstElement(array, i);
					pend = pbeg;
				}
				else {
					pend = AddElement(pend, array, i);
				}
				AmountOfCites++;
			}
			counter = 0;
		}
	}

	if (AmountOfCites == 0) { cout << "Train not found\n"; return 0; }

	string city = "\0";

	cout << "Cities according to your search: ";

	output(pbeg, 1, AmountOfCites, &city);//выводим в консоль все города

	if (city == "\0") {
		bool stop = true;
		cout << "\nChoose city: ";

		while (stop) {
			cin >> city;
			pbeg = DeleteCities(pbeg, pend, city);//удаляем ненужные города
			if (pbeg == 0) { cout << "Make sure that you wrote city corectly. "; return 0; }
			else stop = false;
		}
	}
	output(pbeg, 2, AmountOfCites, NULL);//выводим все поезда, следующие в один единственный город

	RezervTrain(array, AmountOfTrains, city, name);//функция для бронирования
}



//---------------------------->>>>>>>>	Функции, связанные с сортировкой	<<<<<<<<----------------------------\\..


void ChangeArrays(Trains* array, int j, int hour1, int hour2, int min1, int min2, int diff) {
	string hour1Str = to_string(hour2);
	string min1Str = to_string(min2);

	string hour2Str = to_string(hour1);
	string min2Str = to_string(min1);

	string element;
	if (hour1Str.length() != 2) {
		element = hour1Str[0];
		hour1Str = '0' + element;
	}
	if (hour2Str.length() != 2) {
		element = hour2Str[0];
		hour2Str = '0' + element;
	}
	if (min2Str.length() != 2) {
		element = min2Str[0];
		min2Str = '0' + element;
	}
	if (min1Str.length() != 2) {
		element = min1Str[0];
		min1Str = '0' + element;
	}

	array[j].Time = hour1Str + ":" + min1Str;
	array[j + diff].Time = hour2Str + ":" + min2Str;

	string value;

	value = array[j + diff].Day;
	array[j + diff].Day = array[j].Day;
	array[j].Day = value;

	value = array[j + diff].Destination;
	array[j + diff].Destination = array[j].Destination;
	array[j].Destination = value;

	value = array[j + diff].FreePlaces;
	array[j + diff].FreePlaces = array[j].FreePlaces;
	array[j].FreePlaces = value;
}

void QuickSort(int first, int last, Trains* array) {
	int mid;
	int f = first, l = last;

	int hour1, min1, hour2, min2;
	HourToInt(array[f].Time, &hour1, &min1);
	HourToInt(array[l].Time, &hour2, &min2);

	mid = (hour1 + hour2) / 2;

	do
	{
		HourToInt(array[f].Time, &hour1, &min1);
		HourToInt(array[l].Time, &hour2, &min2);

		while (hour1 < mid) {
			f++;
			HourToInt(array[f].Time, &hour1, &min1);
		}
		while (hour2 > mid) {
			l--;
			HourToInt(array[l].Time, &hour2, &min2);
		}
		if (f <= l)
		{
			ChangeArrays(array, f, hour1, hour2, min1, min2, l - f);
			f++;
			l--;
		}
	} while (f < l);

	if (first < l) QuickSort(first, l, array);
	if (f < last) QuickSort(f, last, array);
}

void BubbleSortTime(Trains* array, int AmountOfTrains) {
	for (int i = AmountOfTrains - 1; i >= 0; i--) {
		for (int j = 0; j < i; j++) {
			int hour1, min1, hour2, min2;
			HourToInt(array[j].Time, &hour1, &min1);
			HourToInt(array[j + 1].Time, &hour2, &min2);

			if (hour1 > hour2) ChangeArrays(array, j, hour1, hour2, min1, min2, 1);
			else if (hour1 == hour2 && min1 > min2) ChangeArrays(array, j, hour1, hour2, min1, min2, 1);
		}
	}
}

int BubbleSortLetters(Trains* array, int AmountOfTrains, int letter, int j) {
	for (int i = AmountOfTrains - 1; i >= 0; i--) {
		for (; j < i; j++) {
			char left, right;
			left = array[j].Destination[letter];
			right = array[j + 1].Destination[letter];
			if (letter > 3) return 0;
			if (int(left) > int(right)) {
				string value;
				value = array[j + 1].Day;
				array[j + 1].Day = array[j].Day;
				array[j].Day = value;

				value = array[j + 1].Destination;
				array[j + 1].Destination = array[j].Destination;
				array[j].Destination = value;

				value = array[j + 1].FreePlaces;
				array[j + 1].FreePlaces = array[j].FreePlaces;
				array[j].FreePlaces = value;

				value = array[j + 1].Time;
				array[j + 1].Time = array[j].Time;
				array[j].Time = value;
			}
			else if (int(left) == int(right)) {
				BubbleSortLetters(array, AmountOfTrains, letter + 1, j);
			}
			if (letter != 0) return 0;
			letter = 0;
		}
		j = 0;
	}
	return 1;
}



//---------------------------->>>>>>>>	Основные функции	<<<<<<<<----------------------------

int NumOfElem(string name) {//ищет сколько строк в файле, тем самым определяя сколько поездов в файле с данными
	int AmountOfTrains = 0;
	ifstream file(name);
	if (!file.is_open()) return false;
	else {
		string str;
		while (!file.eof())
		{
			getline(file, str);
			AmountOfTrains++;
		}
		file.close();
	}
	return AmountOfTrains / 2;//т.к. к каждому поезду в файле относятся 2 строки, то количество таких поездов n/2
}

void ReadTrains(int AmountOfTrains, int mode, string name) {
	ifstream file(name);
	if (!file.is_open())
	{
		cout << "I'm really sorry, but file isn't open\n";
	}
	else {
		Trains* array = new Trains[AmountOfTrains];

		string Train, num;
		for (int i = 0; i < AmountOfTrains; i++) {
			file >> Train >> num >> array[i].Day >> array[i].Destination >> array[i].Time >> array[i].FreePlaces;
		}
		file.close();

		if (mode == 1) OutputAllTrains(array, AmountOfTrains, name);
		else if (mode == 2) { AddToList(1, array, AmountOfTrains, name); }
		else if (mode == 3 || mode == 4 || mode == 5) {
			if (mode == 3) BubbleSortTime(array, AmountOfTrains);
			else if (mode == 4) { QuickSort(0, AmountOfTrains - 1, array); } //QuickSortMINUT(0, AmountOfTrains - 1, array); }
			else if (mode == 5) BubbleSortLetters(array, AmountOfTrains, 0, 0);

			OutputAllTrains(array, AmountOfTrains, name);
			string IfSort;
			cout << "\nAre you sure, that you want to sort it in file \"" << name << "\"? If yes, enter number 1, else enter random symbol: ";
			cin >> IfSort;
			if (IfSort == "1") InputToList(array, AmountOfTrains, name);
		}
		else if (mode == 6) {
			string key;
			cout << "Enter city: ";
			cin >> key;

			Search(array, AmountOfTrains, key, name);
		}
	}
}

int main() {
	int option;
	bool stop = true;
	int AmountOfTrains = 0;//самая важная переменная, отвечающаяя за количество поездов в файле
	while (stop) {
		cout << "Please choose number\n" << "1 to look at all available trains.\n2 to add " <<
			"a new train.\n3 to search certain train.\n4 to sort trains.\nother symbol to finish program.\n";
		cin >> option;

		AmountOfTrains = 0;
		if (option == 1) {
			////////////////////во всех if else есть такой же кусок кода, отвечающий за определение, с каким фалом работать

			string name;
			cout << "If you want to find trains in file \"Trains.txt\" enter 1, else enter name of file which you want to read:\n";
			cin.ignore();
			getline(cin, name);
			if (name == "1") name = "Trains.txt";
			else name += ".txt";
			AmountOfTrains = NumOfElem(name);

			/////////////////////////////////////

			ReadTrains(AmountOfTrains, 1, name);

			return main();
		}

		else if (option == 2) {
			//////////////////////////////////////////
			string name;
			cout << "If you want to add a new train to file \"Trains.txt\" enter 1, else enter name of" <<
				"file where you want to add a new train:\n";
			cin.ignore();
			getline(cin, name);
			if (name == "1") name = "Trains.txt";
			else name += ".txt";
			AmountOfTrains = NumOfElem(name);

			/////////////////////////////////////

			if (AmountOfTrains != 0) ReadTrains(AmountOfTrains, 2, name);//если файл не пустой, то вызываем функцию доьавление файла через readTrains
			else AddToList(2, NULL, AmountOfTrains, name);//если файл пустой, то вызываем напрямую, в качестве массива передаем NULL

			return main();
		}

		else if (option == 3) {
			string name;
			cout << "If you want to search the train to file \"Trains.txt\" enter 1, else enter" <<
				" name of file where you want to search train : \n";
			cin.ignore();
			getline(cin, name);
			if (name == "1") name = "Trains.txt";
			else name += ".txt";
			AmountOfTrains = NumOfElem(name);

			ReadTrains(AmountOfTrains, 6, name);

			cout << endl;

			return main();
		}

		else if (option == 4) {
			string choose;
			cout << "If you want a bubble sort - enter 1, if you want a quick sort enter 2. For alphabetical sorting enter 3:\n";
			cin >> choose;

			string name;
			cout << "If you want to sort trains in file \"Trains.txt\" enter 1, else enter name of file where you want to make sort:\n";
			cin.ignore();
			getline(cin, name);
			if (name == "1") name = "Trains.txt";
			else name += ".txt";
			AmountOfTrains = NumOfElem(name);

			if (choose == "1") ReadTrains(AmountOfTrains, 3, name);
			if (choose == "2") ReadTrains(AmountOfTrains, 4, name);
			if (choose == "3") ReadTrains(AmountOfTrains, 5, name);

			return main();
		}

		else stop = false;
	}
	return 0;
}