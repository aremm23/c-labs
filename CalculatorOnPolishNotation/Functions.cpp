#include <iostream>
#include <cstring>
#include <stack>
#include <sstream>
using namespace std;
struct Var {
    double num;
    char letter;
    Var* next;
    Var* prev;
    bool devide; //???? ????? ?????? ???? ??????? /, ?? ????? true ? ????? ???????????, ???? num ?? ????????? ????
}*pend, *pbeg;
//<<<-------------------------------------- Работа со списком -------------------------------------->>>

void Add(char letter, double num, bool devide) {
	Var* pv = new Var;
	if (pbeg == NULL) {
		pbeg = pv;
		pv->prev = NULL;
	}
	else {
		pend->next = pv;
		pv->prev = pend;
	}
	pv->next = NULL;
	pv->num = num;
	pv->letter = letter;
	pv->devide = devide;
	pend = pv;
}

void Show() {
	Var* pv = pbeg;
	while (pv != NULL) {
		cout << pv->letter << " ";
		pv = pv->next;
	}
}

Var* Delete(Var* pv) {
	Var* pvnext = pv->next;

	if (pv == pend) {//последний в списке
		pend = pend->prev;
		pend->next = NULL;
		delete pv;
		return NULL;
	}

	else {//в середине списка
		pv->prev->next = pv->next;
		pv->next->prev = pv->prev;
		delete pv;
		return pvnext;
	}
}

void DelRepeate() {//удаляем повторяющиеся буквы
	Var* pv1 = pbeg;
	Var* pv2 = pbeg;
	while (pv1 != NULL) {
		pv2 = pv1->next;
		while (pv2 != NULL) {
			if (pv1->letter == pv2->letter) {
				pv2 = Delete(pv2);
			}
			else pv2 = pv2->next;
		}
		pv1 = pv1->next;
	}
}

double Search(char key) {
	Var* pv = pbeg;

	while (pv != NULL) {
		if (pv->letter == key) {
			return pv->num;
		}
		pv = pv->next;
	}
	return NULL;
}


//<<<-------------------------------------- Работа с преобразованием выражения в ОПЗ -------------------------------------->>>

void ToPolishNotation(char* input, char* output) {
	stack <char> Stack;//для простоты я не писал код работы стека сам
	int j = 0;

	for (int i = 0; input[i] != '\0'; i++) {
		if (input[i] == '+' || input[i] == '-') {//если + или - то выгружаются все элементы пока не будет ( или стек не опустотится
			while (!Stack.empty()) {
				if (Stack.top() == '(') {
					Stack.push(input[i]);
					break;
				}
				else {
					output[j] = Stack.top();
					Stack.pop();
					j++;
					output[j] = ' ';
					j++;
				}
			}
			if (Stack.empty()) {
				Stack.push(input[i]);
			}
		}
		else if (input[i] == '*' || input[i] == '/') {
			while (!Stack.empty()) {
				if (Stack.top() == '(' || Stack.top() == '+' || Stack.top() == '-') {
					Stack.push(input[i]);
					break;
				}

				else {
					output[j] = Stack.top();
					Stack.pop();
					j++;
					output[j] = ' ';
					j++;
				}
			}
			if (Stack.empty()) {
				Stack.push(input[i]);
			}
		}
		else if (input[i] == '(') {
			Stack.push(input[i]);
		}
		else if (input[i] == ')') {
			while (Stack.top() != '(') {
				output[j] = Stack.top();
				Stack.pop();
				j++;
				output[j] = ' ';
				j++;
			}
			Stack.pop();
		}
		else {
			if (input[i - 1] == '/') Add(input[i], NULL, true);//добавляем букву переменной в список Var
			else Add(input[i], NULL, false);
			output[j] = input[i];
			j++;
			output[j] = ' ';
			j++;
		}
	}
	while (!Stack.empty()) {//выгружаем всё из стека в строку
		output[j] = Stack.top();
		Stack.pop();
		j++;
		output[j] = ' ';
		j++;
	}
	output[j] = '\0';
}

void ShowError(int mode) {
	switch (mode) {
	case 1: {cout << "Please, enter expression without spaces\n"; break; }
	case 2: {cout << "Please, check symbol before \')\'\n"; break; }
	case 3: {cout << "Please, check symbol after \'(\'\n"; break; }
	case 4: {cout << "Please, check symbols before/after operators \'+-*/\'\n"; break; }
	case 5: {cout << "Please, check your expression. It may contain only variables and operators \'+-*/\'\n"; break; }
	case 6: {cout << "Please, make sure you close bracket\n"; break; }
	case 7: {cout << "There is no operator in your expression\n"; break; }
	case 8: {cout << "Two letters cant stay side by side\n"; break; }
	case 9: {cout << "There is no letters in your expression\n"; break; }
	}
}

bool CheckStr(char* str) {//проверка на правильность ввода выражения
	int bracketOpen = 0;
	char letters[] = { "qwertyuiopasdfghjklzxcvbnm\0" };//возможные буквы для переменных
	//char numbers[] = { "1234567890\0" };//возможные числа
	char operators[] = { "+-*/\0" };//возможные операнды
	int counter = 0;
	int StrLength = strlen(str);
	bool operatorTrue = false;//если есть знак оператора то тру
	bool LeftLetter = false;
	bool letterTrue = false;

	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == '(') {								//проверяем:
			for (int k = 0; operators[k] != '\0'; k++) {	//после скобки не может стоять знак оператора
				if (str[i + 1] == operators[k]) {
					ShowError(3);
					return false;
				}
			}
			bracketOpen++;//скобка открылась - ++, закрылась - --
			counter++;
		}
		else if (str[i] == ')') {
			for (int k = 0; operators[k] != '\0'; k++) { //аналогично символу "("
				if (str[i - 1] == operators[k]) {
					ShowError(2);
					return false;
				}
			}
			bracketOpen--;
			counter++;
		}
		else if (str[i] == ' ') { //пробелов не должно быть в выражении
			ShowError(1);
			return false;
		}
		else {
			for (int j = 0; letters[j] != '\0'; j++) //проверяем является ли символ буквой
			{
				if (str[i] == letters[j]) {
					if (i > 0 && i < StrLength) {//слева от первого или справа от последнего элемента никакой знак не стоит
						for (int k = 0; operators[k] != '\0'; k++) //проверяем чтоб справа/слева от буквы стоял знак операнда либо скобка
						{
							if (str[i - 1] == operators[k] || str[i - 1] == '(' || str[i - 1] == ')') {
								LeftLetter = true;
							}
						}
					}
					else if (i == 0 || i == StrLength) {//первй элемент - все ок
						LeftLetter = true;
					}

					if (LeftLetter == false) {	//если сюда заходит, 
						ShowError(8);			//значит слева от какой то переменной стоит не +_/*()
						return false;
					}
					letterTrue = true;
					counter++;
					break;
				}
			}

			LeftLetter = false;//чтоб в некст цикле проверить все по новой

			/*for (int j = 0; numbers[j] != '\0'; j++)
			{
				if (str[i] == numbers[j]) {//если цифра
					counter++;
					break;
				}
			}*/

			for (int j = 0; operators[j] != '\0'; j++)//если знак +-*/
			{
				if (str[i] == operators[j]) {
					for (int k = 0; operators[k] != '\0'; k++) { //проверка чтоб не было подряд несколько знаков опереторов
						if (str[i - 1] == operators[k] || str[i + 1] == operators[k]) {
							ShowError(4);
							return false;
						}
					}
					counter++;
					operatorTrue = true;
				}
			}
		}
	}

	if (counter != StrLength) { ShowError(5); return false; }//counter подчитался не на каждом символе => invalid символ в строке
	if (bracketOpen != 0) { ShowError(6); return false; }//незакрытая скобка
	if (!operatorTrue) { ShowError(7); return false; }//нет оператора
	if (!letterTrue) { ShowError(9); return false; }//нет буквы

	return true;//если все ок
}

double Calculate(char* expr) {
	stack <double> stack;

	for (int i = 0; expr[i] != '\0'; i++) {
		if (isdigit(expr[i])) {
			char NumBeforeDot[255];
			int j = 0;
			double numerical;

			while (expr[i] != '.') {
				NumBeforeDot[j] = expr[i];
				j++;
				i++;
			}

			if (expr[i] == '.') {
				NumBeforeDot[j] = expr[i];
				i++;
				j++;

				while (isdigit(expr[i])) {
					NumBeforeDot[j] = expr[i];
					j++;
					i++;
				}
			}
			else cout << "Something went wrong\n";

			numerical = stod(NumBeforeDot);

			stack.push(numerical);
		}
		else if (expr[i] == ' ') {}
		else {
			if (stack.empty()) {
				cout << "Something went wrong\n";
			}

			double Stack1 = stack.top();
			stack.pop();

			if (stack.empty()) {
				cout << "Something went wrong\n";
			}

			double Stack2 = stack.top();
			stack.pop();

			switch (expr[i])
			{
			case '+': {
				stack.push(Stack1 + Stack2);
				break;
			}
			case '-': {
				stack.push(Stack2 - Stack1);
				break;
			}
			case '*': {
				stack.push(Stack1 * Stack2);
				break;
			}
			case '/': {
				stack.push(Stack2 / Stack1);
				break;
			}

			default:
			{
				cout << "Something went wrong\n";
			}
			}
		}
	}

	if (stack.empty()) {
		cout << "Something went wrong\n";
	}

	return stack.top();
}


//<<<-------------------------------------- Основные функции -------------------------------------->>>

bool GiveNumericalToLetter() {//присваеваем кждой букве в списке числовое значение
	bool DevideByZero = false;

	cout << "You have such variables: ";
	Var* pv = pbeg;
	while (pv != NULL) {
		if (pv->next == NULL) cout << pv->letter << ".\n";
		else cout << pv->letter << ", ";
		pv = pv->next;
	}

	cout << "Now enter numerical value to each letter\n";

	pv = pbeg;
	double num;
	while (pv != NULL) {
		cout << pv->letter << " = ";
		cin >> num;

		if (num == 0 && pv->devide == true) {
			DevideByZero = true;
		}
		pv->num = num;
		pv = pv->next;
	}
	return DevideByZero;
}

void LettersToNumbers(char* withnum, char* withletter) {
	char letters[] = { "qwertyuiopasdfghjklzxcvbnm\0" };
	string number;//сюда конвертируем тип double в string
	string WithNum(withnum);
	string WithLetter(withletter);
	bool stop = true;
	int k = 0;
	bool IsSymbol = false;

	int AfterDot;
	cout << "How many decimal places do you want to leave: ";//Знаков после запятой
	cin >> AfterDot;

	for (int i = 0; WithLetter[i] != '\0'; i++)
	{
		for (int j = 0; letters[j] != '\0'; j++)
		{
			if (WithLetter[i] == letters[j]) {
				IsSymbol = true;

				ostringstream oss; //класс который позволяет форматировать значения и сохранять их в строковый буфер,
				//который можно преобразовать в строку

				oss.precision(AfterDot); //количество знаков после запятой
				double numberDouble = Search(WithLetter[i]);
				oss << fixed << numberDouble; //для фиксированной точности
				number = oss.str();//присваиваем в number
				WithNum.insert(k, number);//встаялем number на k-ую позицию в строке
				k += number.length();//увеличиваем k
				break;
			}
		}

		if (!IsSymbol) {//если не буква, то отавляем такой же символ
			WithNum[k] = WithLetter[i];
			k++;
		}
		IsSymbol = false;
	}
	WithNum[k] = '\0';

	int i;
	for (i = 0; WithNum[i] != '\0'; i++)//перезаписывем каждый элемент из string в массив char
	{
		withnum[i] = WithNum[i];
	}
	withnum[i] = '\0';
}

int main() {
    char input[255]; //массив который вводим изначально в обычной записи
    char output[255]; //тот же массив с символами, но уже в ОПЗ
    char outputNum[255]; //то же выражение, но все переменные заменены числами
    pbeg = NULL;
    pend = pbeg;
    bool DevideByZero = false;

    cout << "Enter your expression:\n";
    cin.getline(input, 255);

    if (CheckStr(input)) {
        ToPolishNotation(input, output);
        cout << "Your expression: " << input << endl;
        cout << "In polish notation: " << output << endl;
        DelRepeate();
        DevideByZero = GiveNumericalToLetter();

        LettersToNumbers(outputNum, output);
        cout << "Your expression: " << outputNum;
        if (!DevideByZero) cout << " = " << Calculate(outputNum);
        else cout << "\nYou cant divide by zero\n";
    }
    else cout << "Invalid input\n";
}