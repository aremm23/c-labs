#include <iostream>
#include <ctime>
using namespace std;

struct List {
	double value;
	List* next;
	List* prev;
};

//<<<------------------------------<<< Функции с добалением элементов и созданием списка  >>>------------------------------>>>

void Add(List** pbeg, List** pend, double num, int mode) {
	List* pv = new List;

	if (*pbeg == NULL) {//Создание списка
		pv->value = num;
		pv->next = NULL;
		pv->prev = NULL;
		*pbeg = pv;
		*pend = pv;
	}
	else if (mode == 1) {//Добвление в конец
		pv->value = num;
		pv->next = NULL;
		pv->prev = *pend;
		(*pend)->next = pv;
		*pend = pv;
	}

	else if (mode == 2) {//Добавление в начало
		pv->value = num;
		pv->next = *pbeg;
		pv->prev = NULL;
		(*pbeg)->prev = pv;
		*pbeg = pv;
	}
}

void EnterNum(List** pbeg, List** pend, int mode) {
	cout << "Fill struct with numbers. To cancel filling enter number 0\n";
	bool stop = true;
	double number;
	while (stop) {
		cout << "Enter number in list: ";
		cin >> number;
		if (number != 0) {
			Add(pbeg, pend, number, mode);
		}
		else stop = false;
	}
}

void RandomNum(List** pbeg, List** pend, int mode) {
	int minNum, maxNum, size;
	bool stop = true;

	cout << "Please enter how many numbers do you want to enter: ";
	cin >> size;

	cout << "Enter the range of random numbers:\n";
	while (stop) {
		cout << "Min value is: ";
		cin >> minNum;
		cout << "Max value is: ";
		cin >> maxNum;

		if (maxNum <= minNum) cout << "Make sure that max value bigger then min. Try again\n";
		else stop = false;
	}

	for (int i = 0; i < size; i++) {
		Add(pbeg, pend, rand() % (maxNum - minNum + 1) + minNum, mode);
	}
}

void Case1_New(List** pbeg, List** pend) {
	cout << "You haven't had a list yet. Create new:\n";
	cout << "Choose how do you want to fill list with numbers?\n";
	cout << "1. Enter numbers\n";
	cout << "2. Fill with random number\n";

	int choose;
	cin >> choose;

	switch (choose)
	{
	case 1: {
		EnterNum(pbeg, pend, 1);
		break;
	}

	case 2: {
		RandomNum(pbeg, pend, 1);
		break;
	}
	default: {
		cout << "Invalid input.\n";
		break;
	}
	}
}

void Case1_Add(List** pbeg, List** pend) {
	cout << "Please where and how you want to add number:\n";
	cout << "1. To enter a number at the end of the list.\n";
	cout << "2. To add a random number at the end of the list.\n";
	cout << "3. To enter a number at the begining of the list.\n";
	cout << "4. To add a random number at the begining of the list.\n";

	int choose;
	cin >> choose;
	switch (choose)
	{
	case 1: {
		EnterNum(pbeg, pend, 1);
		break;
	}

	case 2: {
		RandomNum(pbeg, pend, 1);
		break;
	}

	case 3: {
		EnterNum(pbeg, pend, 2);
		break;
	}

	case 4: {
		RandomNum(pbeg, pend, 2);
		break;
	}

	default: {
		cout << "Invalid input.\n";
		break;
	}
	}
}

//<<<------------------------------<<< Функция вывода в консоль  >>>------------------------------>>>

void Output(List* pbeg, List* pend, int mode) {
	if (mode == 1) {
		List* pv = new List;
		pv = pbeg;

		cout << "Your list is: | ";
		while (pv != NULL) {
			cout << pv->value << " ";
			pv = pv->next;
		}
		cout << "|\n";
	}
	else {
		List* pv = new List;
		pv = pend;

		cout << "Your list is: | ";
		while (pv != NULL) {
			cout << pv->value << " ";
			pv = pv->prev;
		}
		cout << "|\n";
	}
}

void Case2(List* pbeg, List* pend) {
	cout << "Please choose how do you want to look at your list\n";
	cout << "1. left->right\n";
	cout << "2. right->left\n";

	int choose;
	cin >> choose;
	switch (choose) {
	case 1: {
		Output(pbeg, pend, 1);
		break;
	}

	case 2: {
		Output(pbeg, pend, 2);
		break;
	}

	default: {
		cout << "Invalid input.\n";
		break;
	}
	}
}

//<<<------------------------------<<< Функции сортировки >>>------------------------------>>>

void BubbleSort(List* pbeg, List* pend) {
	List* pv = pbeg;

	while (pend != pbeg) {
		while (pv != pend) {
			if (pv->value > pv->next->value) {
				double temp = pv->value;
				pv->value = pv->next->value;
				pv->next->value = temp;
			}
			pv = pv->next;
		}
		pend = pend->prev;
		pv = pbeg;
	}
}

void SelectionSort(List* pbeg) {
	List* current = pbeg;
	while (current != nullptr) {
		List* min_list = current;
		int min_data = current->value;
		List* search_list = current->next;

		while (search_list != nullptr) {
			if (search_list->value < min_data) {
				min_list = search_list;
				min_data = search_list->value;
			}
			search_list = search_list->next;
		}

		if (min_list != current) {
			double temp;
			temp = current->value;
			current->value = min_list->value;
			min_list->value = temp;
		}
		current = current->next;
	}
}

void ListInArray(List* pbeg, int* ListArr, int size) {
	List* pv = pbeg;
	for (int i = 0; i < size; i++) {
		ListArr[i] = pv->value;
		pv = pv->next;
	}

	for (int i = 0; i < size; i++) {
		cout << ListArr[i] << " ";
	}
}

void ArrToList(List* pbeg, int* ListArr, int size) {
	List* pv = pbeg;
	for (int i = 0; pv != NULL; i++) {
		pv->value = ListArr[i];
		pv = pv->next;
	}
}

int QuickSort(int* ListArr, int left, int right, int size) {
	if (left == right) return 0;

	int pivot = ListArr[(left + right) / 2];
	int start = left;

	do
	{
		while (ListArr[left] < pivot) {
			left++;
		}
		while (ListArr[right] > pivot) {
			right--;
		}
		if (right > left) {
			int temp;
			temp = ListArr[left];
			ListArr[left] = ListArr[right];
			ListArr[right] = temp;
			right--;
			left++;
		}

	} while (right > left);


	if (right - 1 < 0) return 0;
	if (right == left == start) return 0;

	QuickSort(ListArr, 0, left - 1, left);
	QuickSort(ListArr, left, size - 1, size);
}

void Case3(List* pbeg, List* pend) {
	cout << "Please choose type of sort you want to use\n";
	cout << "1. Bubble sort\n";
	cout << "2. Selection sort\n";
	cout << "3. Quick sort\n";

	int choose;
	cin >> choose;
	switch (choose) {
	case 1: {
		BubbleSort(pbeg, pend);
		cout << "Your list have been succesfully sorted with bubble sort!\n";
		break;
	}
	case 2: {
		SelectionSort(pbeg);
		cout << "Your list have been succesfully sorted with selection sort!\n";
		break;
	}
	case 3: {
		List* pv = pbeg;
		int size = 0;
		while (pv != NULL) {
			size++;
			pv = pv->next;
		}

		int* ListArr = new int[size];

		ListInArray(pbeg, ListArr, size);

		QuickSort(ListArr, 0, size - 1, size);
		ArrToList(pbeg, ListArr, size);
		cout << "Your list have been succesfully sorted with quick sort!\n";
		break;
	}

	}
}

//<<<------------------------------<<< Функции с поиском и удалением элемента >>>------------------------------>>>

List* Search(List** pbeg, double key) {
	List* pv = *pbeg;

	while (pv != NULL) {
		if (pv->value == key) {
			return pv;
		}
		pv = pv->next;
	}
	return NULL;
}

double SearchMax(List** pbeg) {
	List* pv = new List;
	pv = *pbeg;
	double valueMax = pv->value;

	while (pv != NULL) {
		if (pv->value > valueMax) valueMax = pv->value;
		pv = pv->next;
	}
	return valueMax;
}

bool Delete(List** pbeg, List** pend, List* pv, List** pvnext) {
	if (pv == NULL) return false;//Если key не найден то ничего не удвляется

	else if (pv->next == NULL && pv->prev == NULL) { // если элемент, который требуется удалить является единственным в списке
		delete pv;
		*pbeg = *pend = *pvnext = NULL;
		return true;
	}

	else {
		if (pv == *pbeg) {//первый в списке
			*pvnext = pv->next;
			pv->next->prev = NULL;
			*pbeg = pv->next;
			delete pv;
			return true;
		}

		else if (pv == *pend) {//последний в списке
			*pvnext = NULL;
			pv->prev->next = NULL;
			*pend = pv->prev;
			delete pv;
			return true;
		}

		else {///в середине списка
			pv->prev->next = pv->next;
			pv->next->prev = pv->prev;
			*pvnext = pv->next;
			delete pv;
			return true;
		}
	}
}

void Case4(List** pbeg, List** pend) {
	cout << "Please, enter which number do you want to delete from the list: ";

	double key;
	cin >> key;

	List* pvnext = new List;
	pvnext = *pbeg;
	bool deletesucces = true;
	int counter = 0;
	while (Delete(pbeg, pend, Search(&pvnext, key), &pvnext)) {
		counter++;
	}
	if (counter != 0) cout << "Element " << key << " have been deleted " << counter << " times\n";
	else cout << "Number " << key << " have not founded in your list\n";
}

void Case5(List** pbeg, List** pend) {
	int counter = 0;
	List* pvnext = new List;
	pvnext = *pbeg;

	double key = SearchMax(pbeg);
	while (Delete(pbeg, pend, Search(&pvnext, key), &pvnext) == true) {
		counter++;
	}
	if (counter != 0) cout << "Maximum element is " << key << ". It have been deleted " << counter << " times\n";
}

//<<<------------------------------<<< Удаление списка  >>>------------------------------>>>

void DeleteList(List** pbeg) {
	List* pv = new List;
	pv = *pbeg;
	while (pv != NULL) {
		*pbeg = pv;
		pv = pv->next;
		delete* pbeg;
	}
}

//<<<------------------------------<<< MAIN >>>------------------------------>>>

int main() {
	srand(time(NULL));

	List* pbeg = NULL;
	List* pend = NULL;
	bool STOP = true;

	while (STOP) {
		cout << "Menu:\n";
		cout << "1. Create a new list or add numbers in list\n";
		cout << "2. View a list\n";
		cout << "3. Sort the list\n";
		cout << "4. Delete element/s from list\n";
		cout << "5. Delete maximum element from list\n";
		cout << "6. Delete list\n";
		cout << "Other symbol to finish program\n";

		int choose;
		cin >> choose;
		switch (choose)
		{
		case 1: {
			if (pbeg != NULL) Case1_Add(&pbeg, &pend);
			else Case1_New(&pbeg, &pend);
			break;
		}

		case 2: {
			if (pbeg == NULL) cout << "Your list is empty!\n";
			else Case2(pbeg, pend);
			break;
		}
		case 3: {
			if (pbeg == NULL) cout << "Your list is empty!\n";
			else Case3(pbeg, pend);
			break;
		}

		case 4: {
			if (pbeg == NULL) cout << "Your list is empty!\n";
			else Case4(&pbeg, &pend);
			break;
		}

		case 5: {
			if (pbeg == NULL) cout << "Your list is empty!\n";
			else Case5(&pbeg, &pend);
			break;
		}
		case 6: {
			if (pbeg == NULL) cout << "Your list is empty!\n";
			else {
				DeleteList(&pbeg);
				pbeg = NULL;
				pend = NULL;
				cout << "Your list have been succesfully deleted\n";
			};
			break;
		}

		default: {
			STOP = false;
			break;
		}
		}
	}
}
