/*
Вариант 3.
Вывести количество неявных преобразований во всем файле
*/

double sum(int a, float b) {
	return a + b;
}

int mul(float a, float b) {
	return a + sum(a, b);
}

/*
Примерный вывод:

Function `sum`
int -> float: 1
float -> double: 1

Function `mul`
float -> int: 1
float -> double: 1
double -> int: 1

*/
