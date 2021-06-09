#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <process.h>
#include <string.h>
#define MaxSym	26	//	максимальное	число	нетерминалов
#define MaxTerm	256	//	максимальное	число	символов
#define MaxStr	256	//	максимальное	число	правил

char left[MaxStr];	// левая часть правила
char right[MaxStr][70];	// правая часть правила
int kol;	// число правил грамматики 
short int fw[MaxSym][MaxTerm]; // значение follow_1

void GetData(const char* FileName)
// ввод правил грамматики (нетерминал - большая латинская буква)
// пустая цепочка представима символом #
{
	char aa;
	FILE* in = fopen(FileName, "r");
	if (in == NULL) 
	{ 
		printf("File not found", "");
		exit(1);
	} 
	int i;
	kol = 0;
	while (!feof(in))
	{
		fscanf(in, "%c", &aa);
		if (!feof(in)) left[kol++] = aa;
		fscanf(in, "%c", &aa);	fscanf(in, "%c", &aa);
		// прочитан знак ->
		i = 0; // длина правой чaсти правила
		while(!feof(in))
		{
			fscanf(in, "%c", &aa);
			if (aa == '#')
			{
				printf("Грамматика должна быть неукорачивающей!\n");
				exit(0);
			}
			if (!feof(in) && (aa != '\n')) right[kol - 1][i++] = aa;
			else break;
		}
	}
	fclose(in);
}

void PrintPar()
// вывести множество всех пар символов
{
	int i, j;
	for (i = 0; i < MaxSym; i++)
	{
		for (j = 0; j < MaxTerm; j++) if (fw[i][j] != 0)
			printf(" %c%c ", i + 'A', j);
		printf("\n");
	}
}

void PrintFollow()
// вывести множество follow_1  для всех символов грамматики
{
	int i, j, flag;
	for (i = 0; i < MaxSym; i++)
	{
		flag = 0;
		for (j = 0; j < MaxTerm; j++)
			if ((fw[i][j] != 0) && ((j > 'Z') || (j < 'A')))
			{
				if (flag == 0) printf(" follow(%c)= ", i + 'A');
				flag = 1;
				printf(" %c, ", j);
			}
		if (flag) printf("\n");
	}
}

int SetNew(int iNet, int iSym)
// обновление множества пар N путем порождения из пары iNet-iSym
// iNet - нетерминал, iSym - второй символ
{
	int i, j, k, flag, a, b, c; 
	flag = 0;
	for (i = 0; i < kol; i++)	// по всем правилам грамматики 
		if (left[i]==iNet)
	{
		k = 0;
		do
		{
			a = right[i][k];
			if (right[i][k + 1] != '\0')	b = right[i][k + 1];
			else	b = iSym;
			if ((a <= 'Z') && (a >= 'A'))	// 1-ый символ - нетерминал
			{
				if (fw[a - (int)'A'][b] == 0)
				{
					flag = 1;
					fw[a - (int)'A'][b] = 1;
					printf("%c%c ", a, b);
				} 
				if ((b <= 'Z') && (b >= 'A')) // 2-oй символ - нетерминал
					for (j = 0; j < kol; j++)	// по всем правилам грамматики
						if (left[j]==b)
						{
							c = right[j][0];
							if (fw[a - (int)'A'][c] == 0)
							{
								flag = 1;
								fw[a - (int)'A'][c] = 1;
								printf("%c%c ", a, c);
							}
						}
			} 
			k++;
		} 
		while (right[i][k] != '\0');
	}
	return flag;
}

void Follow(void)
// построить множество follow_1
{
	int flag, i, j, k;
	// очищаем массив для формирования пар символов: 
	for ( i=0; i<MaxSym; i++)
		for (j = 0; j < MaxTerm; j++) fw[i][j] = 0;

	fw[left[0] - (int)'A'][(int)'#'] = 1;
	// аксиома - первый символ при вводе правил
	// # - символ-ограничитель
	SetNew(left[0], '#');	// начальная пара S# построена

	flag = 1;
	while (flag)
	{

		flag = 0;
		for (i = 0; i < MaxSym; i++)
			for (j = 0; j < MaxTerm; j++) 
				if (fw[i][j] != '\0')
		k = SetNew(i + 'A', j);
		flag = flag || k;
	}
}

int main(int argc, char* argv[])
{
	if (argc <= 1) GetData("input.txt");	// без параметра - файл по умолчанию
	else GetData(argv[1]);	// ввести данные
	Follow();
	PrintPar();	// для демонстрационных целей - вывод пар символов 
	PrintFollow();
	return 0;
}
