/*大一下学期工程实践--五子棋*/
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<Windows.h>
#include<string.h>
#include<stdlib.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

/*全局变量*/
FILE *fp0;
int ii, jj;
int row[15] = { 71, 102, 132, 164, 194, 227, 258, 290, 320, 350, 383, 413, 445, 475, 507 };//纵向
int coll[15] = { 80, 110, 141, 172, 202, 235, 266, 296, 329, 360, 390, 421, 453, 484, 515 };//横向
char GamerID[40];
/*棋盘信息结构体*/
struct Chess
{
	int x1, y1;//记录棋子在 窗口上的位置
	bool point = false;//false表示此处没有棋子，true表示此处有棋子
	bool down = true;//默认可以落子
	int color = 0;//初始为0,1为黑子，2为白子
}a[15][15];
/*玩家信息结构体*/
struct ID
{
	char name[40];
	int count = 0;
	bool flag=true;//开始时是”开放的“，当已有数据存储时变为不可读入，为false
}Gamer[10];

/*导入背景图片*/
void background()
{
	IMAGE mag1, mag2;
	loadimage(&mag1, _T("背景.jpg"));
	loadimage(&mag2, _T("棋盘1.jpg"));
	putimage(0, 0, &mag1);
	putimage(50, 60, &mag2);
}

/*安置按钮*/
void button()
{
	char ret0[] = "开    始";
	char ret1[] = "英 雄 榜";
	char ret2[] = "暂存退出";
	char ret3[] = "退    出";
	settextcolor(WHITE);
	settextstyle(50, 0, _T("汉仪晴空体繁"), 0, 0, 0, false, false, false);
	outtextxy(750, 100, ret0);
	outtextxy(750, 200, ret1);
	outtextxy(750, 300, ret2);
	outtextxy(750, 400, ret3);
}

/*重新加载  把棋子结构体内的部分数据重新初始化*/
void load()
{
	cleardevice();
	background();
	button();
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			a[i][j].color = 0;
			a[i][j].down = true;
			a[i][j].point = false;
		}
	}
}

/*寻找距离n.x，n.y最近的棋盘线*/
void optimizex(int x, int y)
{
	int i, j;
	//printf("1 %d %d\n",x,y);
	for ( i = 0; i < 15; i++)
	{
		if (x >= row[i] && x <= row[i + 1])
		{
			if (x - row[i] < row[i + 1] - x)
			{
				x = row[i]; ii = i;
			}
			else
			{
				x = row[i + 1]; ii = i + 1;
			}
			break;
		}
	}
	//printf("i %d\n",ii);
	for ( j = 0; j < 15; j++)
	if (y >= coll[j] && y <= coll[j + 1])
	{
		if (y - coll[j] < coll[j + 1] - y)//计算两个距离来比较距离哪根线更近
		{
			y = coll[j]; jj = j;
		}
		else
		{
			y = coll[j + 1]; jj = j + 1;
		}
		break;
	}
	//printf("j %d\n",jj);
	if (a[ii][jj].point)
	{
		a[ii][jj].down = false;
		return;
	};
	a[ii][jj].x1 = x;
	a[ii][jj].y1 = y;
	a[ii][jj].point = true;
}

/*存储获胜者信息*/
void RankSave()
{
	FILE *fp;
	HWND hwnd = GetHWnd();
	int ret;
	
	ret = MessageBox(hwnd, TEXT("要存入获胜者ID吗？"), TEXT("天地一棋盘"), MB_YESNO | MB_ICONQUESTION);
	if (ret == IDYES)
	{
		InputBox(GamerID, 40, TEXT("请输入获胜者ID"), TEXT("天地一棋盘"));//!!GamerID是一个全局变量
		printf("%s\n", GamerID);
		fp = fopen("rank.txt", "wb+");
		for (int k = 0; k < 10; k++)
		{
			printf("Gamer[k].count %d\n", Gamer[k].count);
			fread(&Gamer[k], sizeof(struct ID), 1, fp);
			/*有，判断是否是输入的相同的ID*/
			if (Gamer[k].flag == false)
			{
				if (strcmp(GamerID, Gamer[k].name) == 0)
				{
					Gamer[k].count++;
					printf("Gamer[k].count %d\n", Gamer[k].count);
					break;
				}
			}
			/*没有，则重建,然后数据处理完成*/
			else
			{
				if (Gamer[k].name[40] == '\0')//这句可能有问题！
				{
					strcpy(Gamer[k].name, GamerID);
					(Gamer[k].count)++;
					printf("GamerID %s\n", GamerID);
					printf("Gamer[k].name %s\n", Gamer[k].name);
					printf("Gamer[k].count %d\n", Gamer[k].count);
					Gamer[k].flag = false;
				};
				break;
			}
		}
		/*将数据存入文件*/
		for (int j = 0; j < 10; j++)
		{
			fwrite(&Gamer[j], sizeof(struct ID), 1, fp);
		}
		fclose(fp);
	}
}

/*胜负判断*/
void Check()
{
	HWND hwnd = GetHWnd();
	int win = 0;
	int ret;
	
	/*横向判断*/
	for (int i = 0; i < 15&&win == 0; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (a[i][j].color == 1 && a[i][j + 1].color == 1 && a[i][j + 2].color == 1 && a[i][j + 3].color == 1 && a[i][j + 4].color == 1)
			{
				win = 1; break;
			}
			if (a[i][j].color == 2 && a[i][j + 1].color == 2 && a[i][j + 2].color == 2 && a[i][j + 3].color == 2 && a[i][j + 4].color == 2)
			{
				win = 2; break;
			}
		}
	}
	/*纵向判断*/
	for (int j = 0; j < 15 && win == 0; j++)
	{
		for (int i = 0; i < 10; i++)
		{
			if (a[i][j].color == 1 && a[i + 1][j].color == 1 && a[i + 2][j].color == 1 && a[i + 3][j].color == 1 && a[i + 4][j].color == 1)
			{
				win = 1; break;
			}
			if (a[i][j].color == 2 && a[i + 1][j].color == 2 && a[i + 2][j].color == 2 && a[i + 3][j].color == 2 && a[i + 4][j].color == 2)
			{
				win = 2; break;
			}
		}
	}
	/*右下方判断*/
	for (int i = 0; i < 12 && win == 0; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (a[i][j].color == 1 && a[i + 1][j + 1].color == 1 && a[i + 2][j + 2].color == 1 && a[i + 3][j + 3].color == 1 && a[i + 4][j + 4].color == 1)
			{
				win = 1; break;
			}
			if (a[i][j].color == 2 && a[i + 1][j + 1].color == 2 && a[i + 2][j + 2].color == 2 && a[i + 3][j + 3].color == 2 && a[i + 4][j + 4].color == 2)
			{
				win = 2; break;
			}
		}
		/*左下方判断*/
		for (int j = 4; j < 15&& win == 0; j++)
		{
			if (a[i][j].color == 1 && a[i + 1][j - 1].color == 1 && a[i + 2][j - 2].color == 1 && a[i + 3][j - 3].color == 1 && a[i + 4][j - 4].color == 1)
			{
				win = 1; break;
			}
			if (a[i][j].color == 2 && a[i + 1][j - 1].color == 2 && a[i + 2][j - 2].color == 2 && a[i + 3][j - 3].color == 2 && a[i + 4][j - 4].color == 2)
			{
				win = 2; break;
			}
		}
	}

	if (win == 1)
	{
		PlaySound(TEXT("Man_Win.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
		printf("黑棋获胜！\n");
		/*怎么在文件里面查找的？？忘了找到对应name的结构体则stu.count++；如果找不到，就重新生成一个结构体来存储name和count*/
		RankSave();
		ret = MessageBox(hwnd, TEXT("再来一局吗？"), TEXT("天地一棋盘"), MB_YESNO | MB_ICONQUESTION);
		if (ret == IDYES)
		{
			load();
		}
		else
		{
			remove("E:\\Projects13\\ConsoleApplication4\\ConsoleApplication4\\record.txt");
			exit(0);
		}
	}
	if (win == 2)
	{
		PlaySound(TEXT("Man_Win.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
		mciSendString(TEXT("open backmusic.mp3 alias GSBGM"), NULL, 0, NULL);
		mciSendString(_T("play GSBGM repeat"), NULL, 0, NULL);
		printf("白棋获胜！\n");
		RankSave();
		ret = MessageBox(hwnd, TEXT("再来一局吗？"), TEXT("天地一棋盘"), MB_YESNO | MB_ICONQUESTION);
		if (ret == IDYES)
		{
			load();
		}
		else
		{
			remove("E:\\Projects13\\ConsoleApplication4\\ConsoleApplication4\\record.txt");
			exit(0);
		}
	}
}

/*暂时存储残局，仅能存储一次，如果下一次不使用，在下次退出前就会被删除*/
void TmpSave()
{
	FILE *fp;	
	fp = fopen("record.txt", "wb");
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			fwrite(&a[i][j], sizeof(struct Chess), 1, fp);
		}
	}
	fclose(fp);
}

/*判断之前是否有保存的残局*/
void Judge()/*怎么解决存储文件里的过时信息？？remove解决*/
{
	HWND hwnd = GetHWnd();
	FILE *fp;
	int ret;
	if ((fp=fopen("record.txt","rb"))==NULL)
	{
		printf("OK!");
		return;
	}
	else
	{
		ret = MessageBox(hwnd, TEXT("是否继续上次的残局？"), TEXT("天地棋局"), MB_YESNO | MB_ICONQUESTION);
		if (ret == IDYES)
		{
			/*这个for里面有问题！！！问题已解决*/
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					fread(&a[i][j], sizeof(struct Chess), 1, fp);
					switch (a[i][j].color)
					{
					case 0: break;
					case 1:setfillcolor(BLACK); solidcircle(a[i][j].x1, a[i][j].y1, 13); break;
					case 2:setfillcolor(WHITE); solidcircle(a[i][j].x1, a[i][j].y1, 13);  break;
					}
				}
			}
			fclose(fp);
		}
		else
			fclose(fp);
	}
}

int main()
{
	initgraph(960, 600, SHOWCONSOLE | NOCLOSE);
	MOUSEMSG n;
	bool flag = false;
	int ret;
	int k,count=0;
	struct ID temp;
	char out[200];
	FILE *fp;
	HWND hwnd = GetHWnd();
	background();
	button();
	mciSendString(TEXT("open backmusic.mp3 alias GSBGM"), NULL, 0, NULL);
	mciSendString(TEXT("play GSBGM"), NULL, 0, NULL);
	/*判断开始*/
	while (true)
	{
		n = GetMouseMsg();
		if (n.uMsg == WM_LBUTTONDOWN&&n.x > 750 && n.x<950 && n.y>100 && n.y < 150)
		{
			Judge();
			break;
		}
		if (n.uMsg == WM_LBUTTONDOWN&&n.x>750 && n.x<950 && n.y>200 & n.y < 250)
		{
			//如果英雄榜为空，否则显示英雄榜；这里只做从文件读出数据并且显示到屏幕上
			fp = fopen("rank.txt", "rb");
			for (int i = 0; i < 10; i++)
			{
				fread(&Gamer[i], sizeof(struct ID), 1, fp);
				if (Gamer[i].count != 0)
					count++;
				if (i==0&&Gamer[i].count==0)
				{
					ret = MessageBox(hwnd, TEXT("还没有记录，快点来玩吧！"), TEXT("积分排行"), MB_OK);
					break;
				}
				/*printf("%c\n", Gamer[i].name[0]);
				printf("%s %d\n", Gamer[i].name, Gamer[i].count); */				
			}//数据全部读完	
			fclose(fp);
			for (int m = 0; m < 9; m++)
			{
				k = m;
				for (int n = m + 1; n < 10;n++)
				{
					if (Gamer[k].count<Gamer[n].count)k=n;
				}
				if (k!=m)
				{
					temp = Gamer[k]; Gamer[k] = Gamer[m]; Gamer[m] = temp;
				}
			}
			sprintf(out, "第一名：%s    +%d\n第二名：%s    +%d\n第三名：%s    +%d\n", Gamer[0].name, Gamer[0].count, Gamer[1].name, Gamer[1].count, Gamer[2].name, Gamer[2].count);
			ret = MessageBox(hwnd, out, TEXT("积分排行"), MB_OK);
		}
		if (n.uMsg == WM_LBUTTONDOWN&&n.x > 750 && n.x<950 && n.y>300 & n.y < 350 || n.uMsg == WM_LBUTTONDOWN&&n.x>750 && n.x<950 && n.y>400 & n.y < 450)
		{
			return 0;
		}
	}

	/*处理鼠标信息*/
	while (true)
	{
		n = GetMouseMsg();
		/*重新开始*/
		if (n.uMsg == WM_LBUTTONDOWN&&n.x > 750 && n.x<950 && n.y>100 && n.y < 150)
		{
				ret = MessageBox(hwnd, TEXT("重新开始吗？"), TEXT("天地一棋盘"), MB_YESNO | MB_ICONQUESTION);
				if (ret == IDYES)
					load();
		}
		/*查看英雄榜*/
		else if (n.uMsg == WM_LBUTTONDOWN&&n.x>750 && n.x<950 && n.y>200 & n.y < 250)
		{
			fp = fopen("rank.txt", "rb");
			for (int i = 0; i < 10; i++)
			{
				fread(&Gamer[i], sizeof(struct ID), 1, fp);
				if (Gamer[i].count != 0)
					count++;
				if (i == 0 && Gamer[i].count == 0)
				{
					ret = MessageBox(hwnd, TEXT("还没有记录，快点来玩吧！"), TEXT("积分排行"), MB_OK);
					break;
				}
			}//数据全部读完	
			fclose(fp);
			for (int m = 0; m < 9; m++)
			{
				k = m;
				for (int n = m + 1; n < 10; n++)
				{
					if (Gamer[k].count<Gamer[n].count)k = n;
				}
				if (k != m)
				{
					temp = Gamer[k]; Gamer[k] = Gamer[m]; Gamer[m] = temp;
				}
			}
			sprintf(out, "第一名：%s    +%d\n第二名：%s    +%d\n第三名：%s    +%d\n", Gamer[0].name, Gamer[0].count, Gamer[1].name, Gamer[1].count, Gamer[2].name, Gamer[2].count);
			ret = MessageBox(hwnd, out, TEXT("积分排行"), MB_OK);
		}
		/*暂存并且退出*/
		else if (n.uMsg == WM_LBUTTONDOWN&&n.x>750 && n.x<950 && n.y>300 & n.y < 350)
		{
			remove("E:\\Projects13\\ConsoleApplication4\\ConsoleApplication4\\record.txt");
			TmpSave();
			return 0;
		}
		/*按退出按钮*/
		else if (n.uMsg == WM_LBUTTONDOWN&&n.x>750 && n.x<950 && n.y>400 & n.y < 450)
		{
			remove("E:\\Projects13\\ConsoleApplication4\\ConsoleApplication4\\record.txt");
			return 0;
		}
		 /*无效区域*/
		else if (n.x<71||n.x>507||n.y<80||n.y>515)
		 {
			 continue;
		 }
		 /*判断是否落子*/
		 else if (n.uMsg == WM_LBUTTONDOWN && flag == false){
			 setfillcolor(WHITE);
			 optimizex(n.x, n.y);			 
			 if (a[ii][jj].down){
				 solidcircle(a[ii][jj].x1, a[ii][jj].y1, 13);
				 PlaySound(TEXT("eat.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
				 flag = true;
				 a[ii][jj].color = 2;
				 Check();
			 }
		 }
		 else if (n.uMsg == WM_LBUTTONDOWN && flag == true){
			 setfillcolor(BLACK);
			 optimizex(n.x, n.y);
			 if (a[ii][jj].down){
				 solidcircle(a[ii][jj].x1, a[ii][jj].y1, 13);
				 PlaySound(TEXT("eat.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
				 flag = false;
				 a[ii][jj].color = 1;
				 Check();
			 }
		 }		 
	} 

}