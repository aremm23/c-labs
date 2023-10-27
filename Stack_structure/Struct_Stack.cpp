#include <iostream>
#include <ctime>

struct List {
	int number;
	List* next;
};

using namespace std;

void SortInf(List* top) {
	List* pv = new List;
	pv = top;
	List* pnext;

	while (pv != NULL) {
		pnext = pv->next;
		while (pnext != NULL) {
			if (pnext->number > pv->number) {
				int temp = pv->number;
				pv->number = pnext->number;
				pnext->number = temp;

			}
			pnext = pnext->next;
		}
		pv = pv->next;
	}
}

List* Push(int num, List* top) {
	List* pv = new List;

	pv->next = top;
	pv->number = num;


	return pv;
}

void Delete(List** pv) {
	while (*pv != NULL) {
		List* p;
		p = *pv;
		*pv = (*pv)->next;
		delete p;
	}
}

List* Output(List* p, int* out) {
	List* pv = p;
	*out = p->number;
	p = p->next;
	delete pv;
	return p;
}

void View(List* top) {
	List* pv = top;
	if (!top) {
		cout << "Empty!" << endl;
		return;
	}

	while (pv != NULL) {
		cout << " " << pv->number;
		pv = pv->next;
	}
}

void DeleteMax(List** top, int value) {
	List* prev = NULL;
	List* curr = *top;
	while (curr != NULL) {
		if (curr->number == value) {
			if (prev == NULL) {
				*top = curr->next;
			}
			else {
				prev->next = curr->next;
			}
			List* p = curr;
			curr = curr->next;
			delete p;
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}
}

int FindMax(List** top) {
	int value;
	List* pv = *top;
	value = pv->number;
	while (pv->next != NULL) {

		if ((pv->next)->number > value) {
			value = (pv->next)->number;
		}
		pv = pv->next;
	}
	DeleteMax(top, value);
	return value;
}

int main()
{
	int a = 0;
	int n;
	List* top = NULL;

	int choise1, choise2;
	bool stop = true, stop1, stop2;
	int size;

	while (stop) {
		cout << "Menu:\n1. To make new stack or add new element in stack.\n2. To delete stack.\n3. To show stack in console\n4. To delete maximum element in stack\n5. To sort stack\n-Random symbol to finish program\n";
		cin >> choise1;
		stop1 = true;
		stop2 = true;

		switch (choise1) {
		case 1: {
			cout << "Enter 1 to fill with random numbers, or other number to fill stack by yourself: ";
			while (stop1) {
				cin >> choise2;
				switch (choise2) {
				case 1: {
					cout << "Set stack size: ";
					cin >> size;
					srand(time(NULL));
					int minNum, maxNum;
					cout << "Enter the range of random numbers:\n";
					while (stop2) {
						cout << "Min value is: ";
						cin >> minNum;
						cout << "Max value is: ";
						cin >> maxNum;

						if (maxNum <= minNum) cout << "Make sure that max value bigger then min. Try again\n";
						else stop2 = false;
					}

					for (int i = 0; i < size; i++) {
						top = Push(rand() % (maxNum - minNum + 1) + minNum, top);
					}
					stop1 = false;
					stop2 = true;
					cout << "Stack has been succesfully created!\n";
					break;
				}

				default: {
					cout << "Fill stack with number. To cancel filling enter number 0\n";
					while (stop2) {
						cout << "Enter " << a + 1 << " element in stack: ";
						cin >> n;
						if (n != 0) top = Push(n, top);
						else stop2 = false;
						a++;
					}
					stop1 = false;
					cout << "Stack has been succesfully created!\n";
					break;
				}
				}
			}
			break;
		}

		case 2: {
			if (top != NULL) {
				Delete(&top);
				cout << "Stack has been succesfully deleted!\n";
			}
			else cout << "Stack is empty!\n";
			break;
		}

		case 3: {
			if (top != NULL) {
				cout << "Your stack is: |";
				View(top);
				cout << " |\n";
			}
			else cout << "Your stack is empty\n";
			break;

		}

		case 4: {
			if (top != NULL) {
				int MaxValue = FindMax(&top);
				cout << "The biggest number in stack (" << MaxValue << ") has been succesfuly deleted!" << endl;
			}
			else cout << "Your stack is empty\n";

			break;
		}

		case 5: {
			if (top != NULL) {
				SortInf(top);
				cout << "has been succesfuly sorted\n";
			}
			else cout << "Your stack is empty\n";
			break;
		}

		default: {
			stop = false;
			break;
		}

		}

	}
	return 0;
}