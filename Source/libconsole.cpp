#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
using namespace std;
const 
	int clblack=0;
	int clblue=1;
	int clgreen=2;
	int claqua=3;
	int clred=4;
	int clpurple=5;
	int clyellow=6;
	int clwhite=7;
	int clgray=8;
	int cllightblue=9;
	int cllightgreen=10;
	int cllightaqua=11;
	int cllightred=12;
	int cllightpurple=13;
	int cllightyellow=14;
	int cllightwhite=15;
	int maxx=80;
	int maxy=25;
    
// base function lower level

HANDLE handle;

void initlibrary()
{
	handle=GetStdHandle(STD_OUTPUT_HANDLE); 
}

void hidecursor() // an con tro man hinh 

{
	HANDLE hOut;
	CONSOLE_CURSOR_INFO ConCurInf;
 
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
 
	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = FALSE;
 
	SetConsoleCursorInfo(hOut, &ConCurInf);
}

void SetFontApp(int x,int y) //dat font cho chuong trinh
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = x;
    cfi.dwFontSize.Y = y;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Raster Fonts");
    SetCurrentConsoleFontEx(handle, FALSE, &cfi);
}


void clrscr(int color)//Xoa man hinh

{
    color=color*16+7;
    SetConsoleTextAttribute(handle, color);
	system("cls");
}

void MaximizeWindow() 

{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	SMALL_RECT rc;
	rc.Left = rc.Top = 0;
	rc.Right = (short)(min(info.dwMaximumWindowSize.X, info.dwSize.X) - 1);
	rc.Bottom = (short)(min(info.dwMaximumWindowSize.Y, info.dwSize.Y) - 1);
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &rc);
}

void SetConsoleColor(int textcolor,int backgroundcolor)
{
	int color;
	color=backgroundcolor*16+textcolor;
	SetConsoleTextAttribute(handle, color);
}

void gotoxy(int x, int y)

{
  COORD c = { x, y };  
  SetConsoleCursorPosition(handle,c);
}

void createwindow(int tx,int ty,int dx,int dy,string tittle,int bgcolor)
{
	int i,j;
	gotoxy(tx,ty);
	SetConsoleColor(cllightwhite,bgcolor);
	printf("%c",201);
	printf("%c",205);
	printf("%s",tittle.c_str());
	for(i=tx+1+strlen(tittle.c_str())+1;i<dx;i++)
		printf("%c",205);
	printf("%c",187);
	for (i=1+ty;i<dy;i++)
	{
		gotoxy(tx,i);
		printf("%c",186);
		for(j=1+tx;j<dx;j++)
			printf(" ");
		printf("%c",186);
	}
	gotoxy(tx,dy);
	printf("%c",200);
	for(i=1+tx;i<dx;i++)
	printf("%c",205);
	printf("%c",188);	
}

void showmessage(string noidung,int mauchu,int maunen)
{
	int msgw,msgh;
	msgw=4+strlen(noidung.c_str());
	msgh=6;
	int cx=maxx/2;
	int cy=maxy/2-3;
	createwindow(cx-msgw/2,cy-msgh/2,cx+msgw/2,cy+msgh/2,"Thong bao",maunen);
	createwindow(cx-msgw/2,cy-msgh/2,cx+msgw/2,cy+msgh/2,"Thong bao",maunen);
	gotoxy(cx-msgw/2+2,cy-msgh/2+2);
	SetConsoleColor(mauchu,maunen);
	printf("%s",noidung.c_str());
	gotoxy(cx-msgw/2+2,cy-msgh/2+3);
	SetConsoleColor(cllightred,maunen);
	printf("Enter");
	do
	{
	}
	while (getch()!=13);
	
	
}

int messagedlg(string noidung,int mauchu,int maunen)
{
	int msgw,msgh;
	char key,sel;
	noidung=noidung+"[C,K]";
	msgw=4+strlen(noidung.c_str());
	msgh=6;
	int cx=maxx/2;
	int cy=maxy/2-3;
	createwindow(cx-msgw/2,cy-msgh/2,cx+msgw/2,cy+msgh/2,"Thong bao",maunen);
	gotoxy(cx-msgw/2+2,cy-msgh/2+2);
	SetConsoleColor(mauchu,maunen);
	printf("%s",noidung.c_str());
	key=' ';
	sel=' ';
	
	SetConsoleColor(cllightred,maunen);
	do
	{
		
		gotoxy(cx-msgw/2+2,cy-msgh/2+3);
		key=getch();
		if(key==('c')||(key=='C')||(key=='k')||(key=='K'))
		{
		printf("%c",key);
		if ((key=='c')||(key=='C')) sel='C';
		else if ((key=='k')||(key=='K')) sel='K';
	}
	}
	while ((key!=13)&&(sel!=' '));
	return sel=='C';
}

void setfont()
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 20;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Raster Font");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}









