#include <stdio.h>
#include <conio.h>
#include "libconsole.cpp"
const int wide=37;//khoang cach 2 mui ten tren va duoi
const int dis=100;//khoang cach giua hai mui ten
const int ResX=150;//chieu ngang man hinh
const int ResY=100;//chieu doc mang hinh
const int MuiTenCount=3;//so luong mui ten
const int numbw=6;//number width chieu rong cua diem so
const int numh=30;//number height chieu cao cua diem so
const int disbetnum=1 ; //distance between number khoang cach giua hai so
const int scoretop=10;	//score top vi tri cua diem tren man hinh (toa do y)

struct TPoint  //struct toa do 2 chieu
	{
		int x;
		int y;
	};

struct TMuiTen //struct mui ten
	{
		int x;
		int length;
	};

int bird[20][20];//mang luu hinh anh con chim
int birddied[20][20]; //mang luu hinh an con chim chet
int GameOverMsgImg[100][100];
int logo[100][100];
int BirdH,BirdW;//luu chieu dai va rong cua buc anh
int BirdX=30;//toa do ban dau cua chim
int BirdY=0;
int logow,logoh;//chieu dai va rong cua logo it faculty
int govmsgw,govmsgh; // game over message width/height chieu rong/cao cua message game over
int quitgame;//true neu nguoi dung thoat game
int raw;// bat cao
int died;// true neu game over
int score;//bien luu diem
int bestscore;//bien luu diem cao nhat
int pause;//true neu nguoi dung dung tro choi
int quitall;//true neu thoat chuong trinh
int number[15][15][15];//mang luu hinh anh so
struct TMuiTen MuiTen[MuiTenCount];
int dangqua;

void GenMuiTen()//Generate Mui Ten
{
	int i,j;
	if (MuiTen[0].x<=0)
		{
			for ( i=0 ; i <= MuiTenCount-2 ; i++ )
				{
					MuiTen[i].x=MuiTen[i+1].x;
					MuiTen[i].length=MuiTen[i+1].length;
				}
		}
	MuiTen[MuiTenCount-1].x = MuiTen[MuiTenCount-2].x+dis;
	MuiTen[MuiTenCount-1].length = rand() % (ResY-10-wide-10+1) + 10;
}

void WelcomScreen()
{
	int i,j,t;
	
	system("MODE CON COLS=80 LINES=25");
	SetFontApp(8,16);
	for(j=0;j<logoh;j++)
		{
			printf("\n");
			for(i=0;i<logow;i++)
			{
				t=logo[i][j];
				switch (t)
				{
					case 'D':
						{
							SetConsoleColor(clblack,clblack);
							printf("%c",219);
							break;
						}
					case ' ':
						{
							SetConsoleColor(cllightwhite,clblack);
							printf("%c",219);
							break;
						}
					case 'C':
						{
							SetConsoleColor(cllightaqua,clblack);
							printf("%c",219);
							break;
						}
					case 'B':
						{
							SetConsoleColor(clblue,clblack);
							printf("%c",219);
							break;
						}
					
				}
			}
		}
		
	gotoxy(46,1);
	printf(" Tro choi * Flappy Brid * ");
	SetConsoleColor(cllightyellow,clblack);
	gotoxy(46,2);
	printf("Ngon ngu lap trinh * C++ *");
	SetConsoleColor(cllightred,clblack);
	gotoxy(46,3);
	printf("* GVHD: Nguyen Hieu Cuong ");
	gotoxy(46,4);
	printf("* Sinh vien thuc hien: ");
	gotoxy(46,5);
	printf("* Nguyen Truong Tien  ");
	gotoxy(46,6);
	printf("*  Cao Trong Tung ");
	gotoxy(46,7);
	printf("*  Do Khanh Trung  ");
	gotoxy(46,8);
	printf("*  Pham The Kien ");
	gotoxy(46,10);
	printf("       ***** *****        ");
	gotoxy(46,11);
	printf("      *************       ");
	gotoxy(46,12);
	printf("      *************       ");
	gotoxy(46,13);
	printf("       ***********        ");
	gotoxy(46,14);
	printf("        *********         ");
	gotoxy(46,15);
	printf("         *******          ");
	gotoxy(46,16);
	printf("          *****           ");
	gotoxy(46,17);
	printf("           ***            ");
	gotoxy(46,18);
	printf("            *             ");
	getch();
}


void LoadITLogo(int *logow,int *logoh)
{
	FILE *f;
	
	int i,j,c,x,y,t;
	if(((f=fopen("data\\logoit.txt","r"))==NULL))
	{
		printf("khong the mo file");
		return ;
	}
	clrscr(clblack);
	i=-1;
	j=0;
	
	while ((c=fgetc(f))!=EOF)
	{
	
		i=i+1;
		logo[i][j]=c;
			if(c=='\n')
		{
			i=-1;
			j++;
		}				
	}
	j++;
	i=i+1;
	*logow=i;
	*logoh=j;
	fclose(f);
}

void ReadBestScore( int *best )//doc best score tu tep tin
{
	FILE *fs;
	
	int tmp;
	if((fs=fopen("data\\best.txt","rt"))==NULL)
	{
		printf("khong mo duoc tap tin best.txt");
		getch();
		return;
	}
	fscanf(fs,"%d",best);
	fclose(fs);	
}

void WriteBestScore(int best)//ghi best score ra tep tin
{
	FILE *f;
	remove("data\\best.txt");
	if((f=fopen("data\\best.txt","w"))==NULL)
	{
		printf("khong mo duoc tap tin best.txt");
		getch();
		fclose(f);
		return;
	}
	fprintf(f,"%d",best);
	fclose(f);	
}

int GameOverMsg(int score,int best)//hien message game over
{
	int i,j,c;
	int temp,scoretmp,num[100],index,scorew,scoreh,scoreleft,k,lastx;
	char keypr;
	
	SetConsoleTitle("Game Flappy Bird C++  Game Over");
	
	//ve message
	for(j=0;j<=govmsgh;j++)
		{
			gotoxy((ResX-70)/2,(ResY-16)/2+j);
			for(i=0;i<=govmsgw;i++)
			{		
				c=GameOverMsgImg[i][j];
				switch(c)
				{
					case 'B':
						{
							SetConsoleColor(clgreen,clgray);
							printf("%c",219);
							break;
						}
					case '#':
						{
							SetConsoleColor(clblue,clblack);
							printf("%c",219);
							break;
						}
					case 'X':
						{
							SetConsoleColor(cllightred,clblack);
							printf("%c",219);
							break;
						}
					default :
						{
							SetConsoleColor(cllightwhite,clgray);
							printf("%c",176);
							break;
						}
				}
			}	
		}
	//tach tung so cua best score vao mang num
	index=-1;	
	temp=best;
	do
	{
		index++;
		num[index]=temp%10;
		temp=temp/10;		
	}
	while (temp!=0);
	
	//khoi tao cac gia tri ve toa do ve diem trong game over message
	scoreh=numh;
	scorew=numbw*index+(index-1)*disbetnum;
	scoreleft=(ResX-scorew)/2;
	lastx=(ResX-70)/2+35;
	SetConsoleColor(cllightyellow,clblack);	
	
	//ve best score len message
	for(i=index;i>=0;i--)
		{
			gotoxy(lastx,(ResY-16)/2+3);
			for(j=0;j<=5;j++)
				{
					gotoxy(lastx,(ResY-16)/2+2+j);
					for(k=0;k<=2;k++)
						if(number[num[i]][k][j]==78)
							printf("%c",219);
						else printf(" ");					
				}
			lastx=lastx+numbw+disbetnum;			
		}	
	
	//tach score luu vao mang num	
	temp=0;
	scoretmp=-1;
	index=-1;
	do
	{
		scoretmp++;
		temp=scoretmp;
		index=-1;
		do
		{
			index++;
			num[index]=temp%10;
			temp=temp/10;		
		}
		while (temp!=0);
		
		//ve score len message voi hieu ung tang dan trong 2 giay
		scoreh=numh;
		scorew=numbw*index+(index-1)*disbetnum;
		scoreleft=(ResX-scorew)/2;
		lastx=(ResX-70)/2+35;
		SetConsoleColor(cllightwhite,clgray);
		
		//xoa vung ghi score
		for(j=(ResY-16)/2+9;j<=(ResY-16)/2+16-1;j++)
		{
			gotoxy(lastx,j);
			for(i=lastx;i<=(ResX-70)/2+70-3;i++)		
			{
				printf("%c",176);
			}
		}
		SetConsoleColor(cllightyellow,clblack);	
		
		//ve score
		for(i=index;i>=0;i--)
		{
			gotoxy(lastx,(ResY-16)/2+9);
			for(j=0;j<=5;j++)
				{
					gotoxy(lastx,(ResY-16)/2+9+j);
					for(k=0;k<=2;k++)
						if(number[num[i]][k][j]==78)
							printf("%c",219);
						else printf(" ");					
				}
			lastx=lastx+numbw+disbetnum;			
		}	
		Sleep(500/((score==0)?1:score));
	}	
	while (scoretmp!=score);
	
	
	SetConsoleTitle("Nhan ENTER");
		keypr='2';
		do
		{
			if (GetAsyncKeyState(VK_RETURN)) keypr=VK_RETURN;
		}
		while(keypr=='2');
	if (score>best) bestscore=score;
		return ((keypr==VK_RETURN)?1:0);
		
}

void LoadNumber()//nap hinh anh so tu 0-9 vao mang
{
	int i,j,c;
	FILE *f;
	
	if ((f=fopen("data\\0.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[0][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	
	if ((f=fopen("data\\1.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[1][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	
	if ((f=fopen("data\\2.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[2][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	
	if ((f=fopen("data\\3.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[3][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	
	if ((f=fopen("data\\4.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[4][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	if ((f=fopen("data\\5.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[5][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	if ((f=fopen("data\\6.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[6][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	if ((f=fopen("data\\7.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[7][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	if ((f=fopen("data\\8.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[8][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	if ((f=fopen("data\\9.txt","r"))!=NULL)
	{
		i=-1;
		j=0;
		do
		{
			c=getc(f);
			if(c=='\n') 
			{
				i=-1;
				j++;
				
			}
			else
			{
				i++;
				number[9][i][j]=c;
				
			}
		}
		while (c!=EOF);
	}
	else 
	{
		printf("Khong doc duoc du lieu tu file, loi roi!!");
		return;
	}
	fclose(f);
	
}

void step()//thay doi vi tri cua cac mui ten 
{
	int i;
	for(i=0;i<=MuiTenCount-1;i++)
		MuiTen[i].x=MuiTen[i].x-3;
}


void DrawMuiTen(int x,int l)//ve mui ten tai vi tri x
{
	int i;
	
	if(x<ResX-1)
	{		
		//ve ong tren
		for(i=l;i>=0;i--)
		{
			SetConsoleColor(((i%2==0)?cllightwhite:cllightred),clblack);
			gotoxy(x,i);
			printf("%c%c%c",177,219,177);
		}
	
		//ve ong duoi
		for(i=0;i<=ResY-l-wide;i++)
		{
			SetConsoleColor(((i%2==0)?cllightwhite:cllightred),clblack);
			gotoxy(x,ResY-1-i);
			printf("%c%c%c",177,219,177);
		}
	}	
}

void InScore(int score)//ve diem hien thoi len man hinh
{
	int temp,num[100],index,scorew,scoreh,i,scoreleft,j,k,lastx;
	temp=score;
	index=-1;
	
	//tach score luu vao mang num
	do
	{
		index++;
		num[index]=temp%10;
		temp=temp/10;		
	}
	while (temp!=0);
	
	//ve score len man hinh
	scoreh=numh;
	scorew=numbw*index+(index-1)*disbetnum;
	scoreleft=(ResX-scorew)/2;
	lastx=scoreleft;
	SetConsoleColor(cllightyellow,clblack);
			
	for(i=index;i>=0;i--)
		{
			for(j=0;j<=5;j++)
				{
					gotoxy(lastx,scoretop+j);
					for(k=0;k<=2;k++)
						if(number[num[i]][k][j]==78)
							printf("%c",219);
						else printf(" ");					
				}
			lastx=lastx+numbw+disbetnum;			
		}		
}

void DrawMuiTens()//ve tat ca mui ten ra man hinh
{
	int i;
	for(i=0;i<MuiTenCount;i++)
		DrawMuiTen(MuiTen[i].x,MuiTen[i].length);
}



void DrawBird(int x,int y)//ve hinh con chim
{
	int i,j,c,t;
	
	gotoxy(x,y);
	for(j=0;j<=BirdH;j++)
	{
		for(i=0;i<=BirdW;i++)
			{
				c=bird[i][j];
				switch (c)
					{
						case 'B':
							{
								SetConsoleColor(clblack,clwhite);
								t=219 ; break;
							}
						case 'R':
							{
								SetConsoleColor(cllightred,clwhite);
								t=219; break;
							}
						case 'Y':
							{
								SetConsoleColor(cllightyellow,cllightblue);
								t=219; break;
							}
						case 'W':
							{
								SetConsoleColor(cllightwhite,clwhite);
								t=219; break;
							}
						case 'O':
							{
								SetConsoleColor(cllightgreen,clblue);
								t=219; break;
							}
						default: 
							{
								t=c;
								SetConsoleColor(cllightblue,cllightblue);
							}
					}
				printf("%c",t);
			}
				//if (c=='\n') gotoxy(x,y+j+1);
				//else
		gotoxy(x,y+j+1);
				 
	}
}

void DrawBirdDown(int x,int y)//ve chim cui dau xuong
{
	int i,j,c,t;
	
	gotoxy(x,y);
	for(j=0;j<=BirdW;j++)
	{

		for(i=BirdH;i>=0;i--)
			{
				c=bird[j][i];
				switch (c)
					{
						case 'B':
							{
								SetConsoleColor(clblack,clwhite);
								t=219 ; break;
							}
						case 'R':
							{
								SetConsoleColor(clred,clwhite);
								t=219; break;
							}
						case 'Y':
							{
								SetConsoleColor(cllightyellow,cllightblue);
								t=219; break;
							}
						case 'W':
							{
								SetConsoleColor(cllightwhite,clwhite);
								t=219; break;
							}
						case 'O':
							{
								SetConsoleColor(cllightgreen,clblue);
								t=219; break;
							}
						default: 
							{
								t=c;
								SetConsoleColor(cllightblue,cllightblue);
							}
					}
				 printf("%c",t);
				}
				 gotoxy(x,y+j+1);
			}

}



void loadgameoverimg(int *maxx,int *maxy)//doc hinh anh game over message
{
	FILE *fs;
	int i,j,c;
	// nap message game over
	if((fs=fopen("data\\GameOverImg.txt","r"))==NULL)
		{
			printf("khong the mo tep ");
			getch();
			return;
		}
	i=-1;j=0;		
	do
	{
		c=getc(fs);
		if (c=='\n') 
		{
			j++;
			i=-1;
			continue;
		}
		
		if (c!=EOF)
		{
			i++;
			GameOverMsgImg[i][j]=c;
		}	
	}
	while (c!=EOF);
	*maxx=i;
	*maxy=j;
	//printf("da get xong msg %d %d",i,j);
	//getch();
	fclose(fs);
}

int LoadBirdImage(int *maxx,int *maxy)//lay hinh anh tu file flappybird.txt luu vao mang bird
{
	FILE *fs;
	int c,t,i,j;
	if((fs=fopen("data\\flappybird.txt","r"))==NULL)
		{
			return 0;
		}
	i=-1;j=0;		
	do
	{
		c=getc(fs);
		if (c=='\n') 
		{
			j++;
			i=-1;
			continue;
		}
		
		if (c!=EOF)
		{
			i++;
			bird[i][j]=c;
		}	
	}
	while (c!=EOF);
	*maxx=i;
	*maxy=j;
	fclose(fs);
}


void Render()//xoa man hinh va ve moi thong tin ra man hinh
{
	clrscr(cllightblue);
	if (died) 
		DrawBirdDown(BirdX,BirdY);
	else
		DrawBird(BirdX,BirdY);
	InScore(score);	
	//DrawCo();
	DrawMuiTens();	
}

void loadgamedata()//nap du lieu game
{
	initlibrary();
	LoadBirdImage(&BirdW,&BirdH);
	loadgameoverimg(&govmsgw,&govmsgh);
	LoadNumber();
	printf("load data successed");
	LoadITLogo(&logow,&logoh);
	//getch();	
}

void setdefaultsetting()//cai dat cac thong so man hinh
{
	system("MODE CON COLS=150 LINES=100");
	SetFontApp(4,6);
	MaximizeWindow();
}

void init()//khoi tao cac gia tri khi bat dau choi
{
	int i;
	
	dangqua=FALSE;
	raw=0;
	score=0;
	quitgame=false;
	quitall=false;
	pause=false;
	died=FALSE;
	BirdY=0;
	SetConsoleTitle("Game Flappy Bird C++  Never Over");
	
	for(i=0;i<3;i++)
	{
		MuiTen[i].x=149+i*dis;
		MuiTen[i].length=rand() % (ResY-10-wide-10+1) + 10;;
	}
}



int checkdied()//kiem tra game over
{
	int i,died;
	struct TPoint TopOfTop,DownOfTop,TopOfDown,DownOfDown;
	struct TPoint Bird1,Bird2;
	died=false;
	Bird1.x=BirdX;
	Bird1.y=BirdY;
	Bird2.x=BirdX+BirdW;
	Bird2.y=BirdY+BirdH;
	//kiem tra xem chim dang di qua hai cot hay khong
	
	
	if((BirdY+BirdH)>=ResY) return 1;
	
	for(i=0;i<MuiTenCount;i++)
		{
				//toa do hai dau mut cua mui ten tren
				TopOfTop.x=MuiTen[i].x;
				TopOfTop.y=0;
				DownOfTop.x=MuiTen[i].x;
				DownOfTop.y=0+MuiTen[i].length;
				
				//toa do hai dau duoi cua mui ten duoi
				TopOfDown.x=MuiTen[i].x;
				TopOfDown.y=0+MuiTen[i].length+wide;
				DownOfDown.x=MuiTen[i].x;
				DownOfDown.y=ResY-1;
				
				//kiem tra chim co bi ten dam hay ko
				if((Bird2.x>=TopOfTop.x )&& (Bird1.x<TopOfTop.x))
					dangqua=i;
				if ((i==dangqua)&&(TopOfTop.x<Bird1.x))
					
					{
						score++;
						dangqua=-1;	
					}
				died=(Bird1.x<DownOfTop.x)&&(DownOfTop.x<Bird2.x)&&((DownOfTop.y>Bird1.y)||(TopOfDown.y<Bird2.y));
				if (died==TRUE) break;		
		}
		return died;
}

void GameOver()
{
	int i;
	for(i=600;i<=6000;i=i+50)
		Beep(i,60);
}

int gamemenu()//menu lua chon truoc khi vao game
{
	int i,play;
	system("MODE CON COLS=80 LINES=25");
	SetFontApp(8,16);
	clrscr(cllightblue);
	DrawBird(2,4);
	SetConsoleColor(cllightyellow, cllightblue);	
	i=6;		
		gotoxy(20, i); printf(" _____ _                           ____  _         _ \n");
		gotoxy(20, i+1);printf("|  ___| | __ _ _ __  _ __  _   _  | __ )(_)_ __ __| |\n");
		gotoxy(20, i+2);printf("| |_  | |/ _` | '_ || '_ || | | | |  _ || | '__/ _` |\n");
		gotoxy(20, i+3);printf("|  _| | | (_| | |_)|| |_)|| |_| | | |_)|| | |  | (_)|\n");
		gotoxy(20, i+4);printf("|_|   |_||__._| .__/| .__/|___  | |____||_|_|  |____|\n");
		gotoxy(20, i+5);printf("              |_|   |_|    |___/                     \n");	
	gotoxy(30,13);
	SetConsoleColor(cllightyellow,cllightblue);
	printf("Diem cao nhat : %d",bestscore);
	gotoxy(36,23);
	play=-1;
	do
	{
		gotoxy(20,14);
		printf("nhan SPACE de bat dau choi, ESC de thoat");
		Sleep(200);
		gotoxy(20,14);
		printf("                                        ");
		Sleep(200);
		if(GetAsyncKeyState(VK_SPACE))
			{
				play=1;
			}
		if(GetAsyncKeyState(VK_ESCAPE))
			{
				play=0;
			}
		
	}
	while(play==-1);
	return play;
}

main()//than chuong trinh
{	
	int giatocroi=0;
	int menuselection;
	
	hidecursor();
	loadgamedata();
	ReadBestScore(&bestscore);
	WelcomScreen();
	do
	{
		init();	
		menuselection=gamemenu();
		if(menuselection==0) 
		{
			quitall=true; 
		}
		setdefaultsetting();
		clrscr(cllightblue);
		SetConsoleTitle("Nhan P de tam dung ");
		do
			{	
				
				Render();
				died=checkdied();
				if (died)
					{
						//quitgame=true;
						giatocroi=0;
						do
							{
								BirdY=BirdY+1.5*giatocroi;
								if(BirdY+BirdW>ResY) 
									BirdY=ResY-BirdW-1;
								Render();
								giatocroi=giatocroi+1;
								//Sleep(100/giatocroi);
							}
						while (BirdY!=ResY-BirdW-1);
						if(GameOverMsg(score,bestscore)!=1) 
						{
							quitall=true;
						}
					}
				else
				{
					step();
				GenMuiTen();
				if(raw>0)
					BirdY= BirdY-raw;
				else
					BirdY=BirdY+4;
				if (raw>0) 
					raw=raw-1;
				if(raw<5) raw=0;
				if (BirdY<0) 
					BirdY=0;
				if (GetAsyncKeyState(VK_SPACE))
					raw=7;		
				if ((GetAsyncKeyState('P')||GetAsyncKeyState('p'))&&(!quitgame))
					pause=true;
				Sleep(10);
				if(pause)
					{
						SetConsoleTitle("Dang dung, nhan SPACE de tiep tuc, ESC de thoat");
						do
							{
								pause=!GetAsyncKeyState(VK_SPACE);
								if(GetAsyncKeyState(VK_ESCAPE))
								{
									died=1;
									pause=0;
								}
								
							}
						while(pause==true);
						SetConsoleTitle("Nhan P de tam dung");
					}
				}		
			}
		while (((!died)&&(!quitall)));
		SetConsoleColor(cllightwhite,clblack);
	}
	while (!quitall);
	WriteBestScore(bestscore);
}
