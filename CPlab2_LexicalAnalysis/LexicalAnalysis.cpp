#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

#define ID 15
#define NUM 16


char tempstr[255] = {};

bool temp = false;

char word[255] = {};

string keyword[9] = {
	"Const", "Var", "if", "else", "then",
	"while", "do", "begin", "end"
};

int keyword_num[9] = { 
	1,2,3,4,5,
	6,7,8,9 
};

char symbol[9] = { '+', '-', '*', '/', ';', '(' ,')' ,'{' ,'}'};

int symbol_num[9] = { 21,22,23,24,25,26,27,28,29 };

bool err_flag = false;
 
bool IsLetter(char ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return true;
	return false;
}

 
bool IsDigit(char ch)
{
	if (ch >= '0'&&ch <= '9')
		return true;
	return false;
}


int IsSymbol(char ch)
{
	for (int i = 0; i<9; i++)
	{
		if (ch == symbol[i])
			return i;
	}
	return -1;
}

 
int IsKeyword(string str)
{
	for (int i = 0; i<26; i++)
	{
		if (str == keyword[i])
		{
			return i;
		}
	}
	
	return ID;
}


void HandleSpace(char a[])
{
	int j = 0;
	memset(word, 0, 255);
	temp = false;
	for (int i = 0; i < strlen(a); i++)
	{

		if (a[i] != ' ' && a[i] != '\t')
		{
			word[j++] = a[i];
			temp = false;
		}
		else
		{
			if (!temp && a[i] != '\t')
			{
				word[j++] = a[i];
				temp = true;
			}
		}
	}
}


void prePro()
{
	int j = 0;
	memset(tempstr, 0, 255);
	for (int i = 0; i < strlen(word); i++)
	{
		if (word[i] == '/' && word[i + 1] == '/')
		{
			while (i < strlen(word))
			{
				i++;
			}
		}

		else {
			tempstr[j++] = word[i];
		}
	}
}

void Scanner(char *str);

int main()
{
	char instr[255] = {};
	bool flag = false;  
	string Token; 
	char *str = NULL;
	char delims[] = " ";
	freopen("test.cpp", "r", stdin);
	freopen("result.txt", "w", stdout); 

	printf("词法分析结果如下：\n");
	printf("******************************\n");

	while ((gets(instr)) != NULL)
	{
		HandleSpace(instr);
		prePro();

		str = strtok(tempstr, delims);
			
		while (str != NULL)
		{
			
			if (*(str) == '#')
			{
				printf("#\n");
				break;
			}
			
			Scanner(str);
			if (err_flag) {
				break;
			}
			str = strtok(NULL, delims);
		}
		if(err_flag) break;
	}

	return 0;
}

void Scanner(char *str) {
	bool flag = false; 
	string Token; 
	int type = 0;

	for (int i = 0; i<strlen(str); i++)
	{
		if (*(str + i) == '/')
		{
			if (*(str + i + 1) == '*')
			{
				flag = true;
				break;
			}
		}
		
		if (*(str + i) == '*' && flag)
		{
			if (*(str + i + 1) == '/')
			{
				flag = false;
				i++;
				break;
			}
		}

		if (IsLetter(*(str + i)) && (!flag))
		{
			while (IsLetter(*(str + i)) || IsDigit(*(str + i)) || *(str + i) == '_')
			{
				Token += *(str + i);
				i++;
			}
			if (IsKeyword(Token) != ID)
			{
				int syb = keyword_num[IsKeyword(Token)];
				type = syb;
			}
			else
			{
				type = ID;
			}
			printf("< %s , %d >\n", Token.c_str(), type);
			Token = "";
		}

		if (IsDigit(*(str + i)) && (!flag))
		{

			while (IsDigit(*(str + i)))
			{
				Token += *(str + i);
				if(IsLetter(*(str + i + 1))) {
					int tempIndex = 1;
					while(IsLetter(*(str + i + tempIndex))) {
						Token += *(str + i + tempIndex);
						tempIndex += 1;
					}
					printf("<error: %s wrong identifier>", Token.c_str());
					Token = "";
					err_flag = true;
					break;
				}
				i++;
			}
			if (err_flag) break;
			type = NUM;
			printf("< %s , %d >\n", Token.c_str(), type);
			Token = "";
			
		}

		//<,<=,<>
		if (*(str + i) == '<' && (!flag))
		{
			if (*(str + i + 1) == '=')
			{
				type = 32;
				printf("< %s , %d >\n", "<=", type);
			}
			if (*(str + i + 1) == '>')
			{
				type = 31;
				printf("< %s , %d >\n", "<>", type);
			}
			else
			{
				type = 30;
				printf("< %s , %d >\n", "<", type);
			}
		}
		//>,>=
		else if (*(str + i) == '>' && (!flag))
		{
			if (*(str + i + 1) == '=')
			{
				type = 34;
				printf("< %s , %d >\n", ">=", type);
			}
			else
			{
				type = 33;
				printf("< %s , %d >\n", ">", type);
			}
		}
		//=,==
		else  if (*(str + i) == '=' && (!flag))
		{
			if (*(str + i + 1) == '=')
			{
				type = 36;
				printf("< %s , %d >\n", "==", type);
			}
			else
			{
				type = 35;
				printf("< %s , %d >\n", "=", type);
			}
		}

		//余下定界符等
		else if (IsSymbol(*(str + i)) != -1 && (!flag))
		{
			int syb = symbol_num[IsSymbol(*(str + i))];
			type = syb;
			printf("< %c , %d >\n", *(str + i), syb);
		}
	}
}

