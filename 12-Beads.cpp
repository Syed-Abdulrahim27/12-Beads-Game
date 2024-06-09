#include <iostream>
#include<fstream>
#include<windows.h>
#include<conio.h>
#include<iomanip>
using namespace std;
enum turn { Red, Green };
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
struct Position
{
	char x;
	int y;
};
void init(char**& B, string* &PNames, int& Nop, int& turn, int& Dim,ifstream &Rdr,int* &ScoreCount)
{
	Rdr >> Dim;
	B = new char* [Dim];
	for (int ri = 0; ri < Dim; ri++)
	{
		B[ri] = new char[Dim];
		for (int ci = 0; ci < Dim; ci++)
		{
			Rdr >> B[ri][ci];
		}
	}
	Nop = 2;
	PNames = new string[Nop]{};
	for (int i = 0; i < Nop; i++)
	{
		cout << "Enter Player " << i + 1 << "'s Name:" << endl;
		cin >> PNames[i];
	}
	ScoreCount = new int[2]{};
	turn = rand() % 2;
}
void Square(int y,int x,int Dim)
{
	for (int ci = 0; ci < Dim * 4; ci++)
	{
		gotoRowCol(0 + y, x + ci);
		SetClr(4,0);
		cout << char(-37);
	}
	for (int ci = 0; ci < Dim*4; ci++)
	{
		gotoRowCol(Dim * 4-1  + y, ci + x);
		SetClr(4, 0);
		cout << char(-37);
	}
	for (int ri = 0; ri < Dim * 4; ri++)
	{
		gotoRowCol(ri + y, 0 + x);
		SetClr(4, 0);
		cout << char(-37);
	}
	for (int ri = 0; ri < Dim * 4; ri++)
	{
		gotoRowCol(ri + y, Dim * 4 - 1 + x);
		SetClr(4, 0);
		cout << char(-37);
	}
}
void Cross(int y,int x,int Dim)
{
	for (int ri = 0; ri < Dim * 8; ri++)
	{
		gotoRowCol(y + ri, x + ri);
		SetClr(2,0);
		cout << char(-37);
	}
	for (int ci = Dim * 8 - 1, ri = 0; ci >= 0; ci--, ri++)
	{
		gotoRowCol(ri + y, ci + x);
		SetClr(2,0);
		cout << char(-37);
	}
}
void PrintBoard(char **B,int Dim)
{
	for (int ri = 0; ri <Dim * 16; ri = ri + Dim * 4)
	{
		for (int ci = 0; ci <Dim * 16; ci = ci + Dim * 4)
		{
			Square(ri, ci, Dim);
		}
	}
	Cross(0, 0, Dim);
	Cross(0, Dim * 8,Dim);
	Cross(Dim * 8, 0, Dim);
	Cross(Dim * 8, Dim * 8, Dim);
	for (int ri = 0,r=0; ri <= Dim * 16; ri=ri+Dim*4,r++)
	{
		for (int ci = 0,c=0; ci <= Dim * 16; ci = ci + Dim * 4,c++)
		{
			gotoRowCol(ri, ci);
			SetClr(15,0);
			cout << B[r][c];
		}
	}
}
void Turnmessage(int turn,string *PNames)
{
	cout << endl << PNames[turn] << "'s Turn:" << endl;
}
void Turnchange(int &turn)
{
	if (turn == Green)
	{
		turn = Red;
	}
	else
	{
		turn++;
	}
}
void SelectPosition(Position &SP, string message,int turn)
{
	if (message == "Select Source")
	{
		if (turn == Green)
		{
			cout << "(Select Green)"<< endl;
		}
		else
		{
			cout << "(Select Red)" << endl;
		}
	}
	else
	{
		cout << message<<endl;
	}
	char i = 0;
	do
	{
		cout << "Enter an Alphabet(A-E):" << endl;
		cin >> i;
		if (i - char('A') > 4 || i < 65)
		{
			cout << "Invalid input.....Enter Again" << endl;
		}
	} while (i-char('A')>4||i<65);
	SP.x = i - char('A');
	int j = 0;
	do
	{
		cout << "Enter a Number(1-5):" << endl;
		cin >> j;
		if (j > 5 || j < 1)
		{
			cout << "Invalid input.....Enter Again" << endl;
		}
	} while (j>5||j<1);
	j--;
	SP.y = j;
}
bool ismypiece(char **B,int turn,Position SP)
{
	if (turn == Green)
	{
		return(B[SP.y][SP.x] == 'G');
	}
	else
	{
		return(B[SP.y][SP.x] == 'R');
	}
}
bool isVertical(Position SP,Position DP)
{
	return(SP.x == DP.x);
}
bool isHorizontal(Position SP, Position DP)
{
	return(SP.y == DP.y);
}
bool isDiagonal(Position SP, Position DP)
{
	int y = abs(SP.y - DP.y);
	int x = abs(SP.x - DP.x);
	return(y == x);
}
bool Beaten(char **B,Position SP,Position DP)
{
	if (B[DP.y][DP.x] != '-')
	{
		if (isVertical(SP, DP))
		{
			if (SP.y < DP.y)
			{
				if (B[DP.y + 1][DP.x] != '-')
				{
					return false;
				}
			}
			else
			{
				if (B[DP.y - 1][DP.x] != '-')
				{
					return false;
				}
			}
		}
		else if (isHorizontal(SP, DP))
		{
			if (SP.x < DP.x)
			{
				if (B[DP.y][DP.x + 1] != '-')
				{
					return false;
				}
			}
			else
			{
				if (B[DP.y][DP.x-1] != '-')
				{
					return false;
				}
			}
		}
		else if (isDiagonal(SP, DP))
		{
			//R2L
			if (SP.y<DP.y && SP.x > DP.x)
			{
				if (B[DP.y + 1][DP.x-1] != '-')
				{
					return false;
				}
			}
			else if (SP.y > DP.y && SP.x < DP.x)
			{
				if (B[DP.y - 1][DP.x + 1] != '-')
				{
					return false;
				}
			}
			//L2R
			else if (SP.y < DP.y && SP.x < DP.x)
			{
				if (B[DP.y + 1][DP.x + 1] != '-')
				{
					return false;
				}
			}
			else if (SP.y > DP.y && SP.x > DP.x)
			{
				if (B[DP.y - 1][DP.x - 1] != '-')
				{
					return false;
				}
			}
		}
	}
	else
	{
		return true;
	}
}
bool Legality(char **B,Position SP,Position DP)
{
	if (SP.y % 2 == 0)
	{
		if (SP.x == 1 || SP.x == 3)
		{
			return(isVertical(SP, DP) && abs(SP.y - DP.y)==1 && Beaten(B, SP, DP) || isHorizontal(SP, DP) && Beaten(B, SP, DP) && abs(SP.x - DP.x)==1);
		}
		else
		{
			return(isVertical(SP, DP) && abs(SP.y - DP.y)==1 && Beaten(B, SP, DP) || isHorizontal(SP, DP) && Beaten(B, SP, DP) && abs(SP.x - DP.x)==1 || isDiagonal(SP, DP) && Beaten(B, SP, DP));
		}
	}
	else
	{
		if (SP.x == 0 || SP.x == 2 || SP.x == 4)
		{
			return(isVertical(SP, DP) && abs(SP.y - DP.y)==1 && Beaten(B, SP, DP) || isHorizontal(SP, DP) && Beaten(B, SP, DP) && abs(SP.x - DP.x)==1);
		}
		else
		{
			return(isVertical(SP, DP) && abs(SP.y - DP.y)==1 && Beaten(B, SP, DP) || isHorizontal(SP, DP) && Beaten(B, SP, DP) && abs(SP.x - DP.x)==1 || isDiagonal(SP, DP) && Beaten(B, SP, DP));
		}
	}
}
void Move(char** &B, Position SP, Position DP,int turn,int* &ScoreCounter)
{
	if (B[DP.y][DP.x] != '-')
	{
		if (isVertical(SP, DP))
		{
			if (SP.y < DP.y)
			{
				B[DP.y + 1][DP.x] = B[SP.y][SP.x];
				B[DP.y][DP.x] = '-';
				B[SP.y][SP.x] = '-';
				ScoreCounter[turn]++;
			}
			else
			{
				B[DP.y - 1][DP.x] = B[SP.y][SP.x];
				B[DP.y][DP.x] = '-';
				B[SP.y][SP.x] = '-';
				ScoreCounter[turn]++;
			}
		}
		else if (isHorizontal(SP, DP))
		{
			if (SP.x < DP.x)
			{
				B[DP.y][DP.x + 1] = B[SP.y][SP.x];
				B[DP.y][DP.x] = '-';
				B[SP.y][SP.x] = '-';
				ScoreCounter[turn]++;
			}
			else
			{
				B[DP.y][DP.x - 1] = B[SP.y][SP.x];
				B[DP.y][DP.x] = '-';
				B[SP.y][SP.x] = '-';
				ScoreCounter[turn]++;
			}
		}
		else if (isDiagonal(SP, DP))
		{
			//R2L
			if (SP.y<DP.y && SP.x > DP.x)
			{
				B[DP.y + 1][DP.x - 1] = B[SP.y][SP.x];
				B[DP.y][DP.x] = '-';
				B[SP.y][SP.x] = '-';
				ScoreCounter[turn]++;
			}
			else if (SP.y > DP.y && SP.x < DP.x)
			{
				B[DP.y - 1][DP.x + 1] = B[SP.y][SP.x];
				B[DP.y][DP.x] = '-';
				B[SP.y][SP.x] = '-';
				ScoreCounter[turn]++;
			}
			//L2R
			else if (SP.y < DP.y && SP.x < DP.x)
			{
				B[DP.y + 1][DP.x + 1] = B[SP.y][SP.x];
				B[DP.y][DP.x] = '-';
				B[SP.y][SP.x] = '-';
				ScoreCounter[turn]++;
			}
			else if (SP.y > DP.y && SP.x > DP.x)
			{
				B[DP.y - 1][DP.x - 1] = B[SP.y][SP.x];
				B[DP.y][DP.x] = '-';
				B[SP.y][SP.x] = '-';
				ScoreCounter[turn]++;
			}
		}
	}
	else
	{
		B[DP.y][DP.x] = B[SP.y][SP.x];
		B[SP.y][SP.x] = '-';
	}
}
int WinningCondition(char **B,int Dim)
{
	int count1 = 0, count2 = 0;
	for (int ri = 0; ri < Dim; ri++)
	{
		for (int ci = 0; ci < Dim; ci++)
		{
			if (B[ri][ci] == 'G')
			{
				count1++;
			}
			else if(B[ri][ci]=='R')
			{
				count2++;
			}
		}
	}
	if (count1 == 0 || count2 == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int main()
{
	ifstream Rdr("Text.txt");
	int* ScoreCount=nullptr;
	char** B=nullptr;
	string* PNames = nullptr;
	int Nop = 0, turn = 0, Dim = 0;
	Position SP{};
	Position DP{};
	init(B, PNames, Nop, turn, Dim, Rdr,ScoreCount);
	system("cls");
	PrintBoard(B,Dim);
	cout << endl << PNames[0] << "'s Score is: " << ScoreCount[0] << endl;
	cout << PNames[1] << "'s Score is: " << ScoreCount[1];
	while (true)
	{
		Turnmessage(turn, PNames);
		do
		{
			do
			{
				SelectPosition(SP, "Select Source", turn);
				if (!ismypiece(B, turn, SP))
				{
					cout << "Invalid Source...Enter Again!!!" << endl;
				}
			} while (!ismypiece(B, turn, SP));
			do
			{
				SelectPosition(DP, "Select Destination", turn);
				if (ismypiece(B, turn, DP))
				{
					cout << "Invalid Destination...Enter Again!!!" << endl;
				}
			} while (ismypiece(B, turn, DP));
			if (!Legality(B, SP, DP))
			{
				cout<<"Not Legal Move...Enter Again"<<endl;
			}
		} while (!Legality(B,SP,DP));
		Move(B, SP, DP, turn, ScoreCount);
		system("cls");
		PrintBoard(B, Dim);
		cout<<endl<<PNames[0]<<"'s Score is: " << ScoreCount[0]<<endl;
		cout << PNames[1] << "'s Score is: " << ScoreCount[1];
		if (WinningCondition(B,Dim)==true)
		{
			cout <<endl<< PNames[turn] << " WON THE GAME!!!" << endl;
			exit(1);
		}
		Turnchange(turn);
	}
	return _getch();
}

