/*��һ��ѧ�ڹ���ʵ��--������*/
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<Windows.h>
#include<string.h>
#include<stdlib.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

/*ȫ�ֱ���*/
FILE *fp0;
int ii, jj;
int row[15] = { 71, 102, 132, 164, 194, 227, 258, 290, 320, 350, 383, 413, 445, 475, 507 };//����
int coll[15] = { 80, 110, 141, 172, 202, 235, 266, 296, 329, 360, 390, 421, 453, 484, 515 };//����
char GamerID[40];
/*������Ϣ�ṹ��*/
struct Chess
{
	int x1, y1;//��¼������ �����ϵ�λ��
	bool point = false;//false��ʾ�˴�û�����ӣ�true��ʾ�˴�������
	bool down = true;//Ĭ�Ͽ�������
	int color = 0;//��ʼΪ0,1Ϊ���ӣ�2Ϊ����
}a[15][15];
/*�����Ϣ�ṹ��*/
struct ID
{
	char name[40];
	int count = 0;
	bool flag=true;//��ʼʱ�ǡ����ŵġ������������ݴ洢ʱ��Ϊ���ɶ��룬Ϊfalse
}Gamer[10];

/*���뱳��ͼƬ*/
void background()
{
	IMAGE mag1, mag2;
	loadimage(&mag1, _T("����.jpg"));
	loadimage(&mag2, _T("����1.jpg"));
	putimage(0, 0, &mag1);
	putimage(50, 60, &mag2);
}

/*���ð�ť*/
void button()
{
	char ret0[] = "��    ʼ";
	char ret1[] = "Ӣ �� ��";
	char ret2[] = "�ݴ��˳�";
	char ret3[] = "��    ��";
	settextcolor(WHITE);
	settextstyle(50, 0, _T("��������己"), 0, 0, 0, false, false, false);
	outtextxy(750, 100, ret0);
	outtextxy(750, 200, ret1);
	outtextxy(750, 300, ret2);
	outtextxy(750, 400, ret3);
}

/*���¼���  �����ӽṹ���ڵĲ����������³�ʼ��*/
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

/*Ѱ�Ҿ���n.x��n.y�����������*/
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
		if (y - coll[j] < coll[j + 1] - y)//���������������ȽϾ����ĸ��߸���
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

/*�洢��ʤ����Ϣ*/
void RankSave()
{
	FILE *fp;
	HWND hwnd = GetHWnd();
	int ret;
	
	ret = MessageBox(hwnd, TEXT("Ҫ�����ʤ��ID��"), TEXT("���һ����"), MB_YESNO | MB_ICONQUESTION);
	if (ret == IDYES)
	{
		InputBox(GamerID, 40, TEXT("�������ʤ��ID"), TEXT("���һ����"));//!!GamerID��һ��ȫ�ֱ���
		printf("%s\n", GamerID);
		fp = fopen("rank.txt", "wb+");
		for (int k = 0; k < 10; k++)
		{
			printf("Gamer[k].count %d\n", Gamer[k].count);
			fread(&Gamer[k], sizeof(struct ID), 1, fp);
			/*�У��ж��Ƿ����������ͬ��ID*/
			if (Gamer[k].flag == false)
			{
				if (strcmp(GamerID, Gamer[k].name) == 0)
				{
					Gamer[k].count++;
					printf("Gamer[k].count %d\n", Gamer[k].count);
					break;
				}
			}
			/*û�У����ؽ�,Ȼ�����ݴ������*/
			else
			{
				if (Gamer[k].name[40] == '\0')//�����������⣡
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
		/*�����ݴ����ļ�*/
		for (int j = 0; j < 10; j++)
		{
			fwrite(&Gamer[j], sizeof(struct ID), 1, fp);
		}
		fclose(fp);
	}
}

/*ʤ���ж�*/
void Check()
{
	HWND hwnd = GetHWnd();
	int win = 0;
	int ret;
	
	/*�����ж�*/
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
	/*�����ж�*/
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
	/*���·��ж�*/
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
		/*���·��ж�*/
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
		printf("�����ʤ��\n");
		/*��ô���ļ�������ҵģ��������ҵ���Ӧname�Ľṹ����stu.count++������Ҳ���������������һ���ṹ�����洢name��count*/
		RankSave();
		ret = MessageBox(hwnd, TEXT("����һ����"), TEXT("���һ����"), MB_YESNO | MB_ICONQUESTION);
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
		printf("�����ʤ��\n");
		RankSave();
		ret = MessageBox(hwnd, TEXT("����һ����"), TEXT("���һ����"), MB_YESNO | MB_ICONQUESTION);
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

/*��ʱ�洢�о֣����ܴ洢һ�Σ������һ�β�ʹ�ã����´��˳�ǰ�ͻᱻɾ��*/
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

/*�ж�֮ǰ�Ƿ��б���Ĳо�*/
void Judge()/*��ô����洢�ļ���Ĺ�ʱ��Ϣ����remove���*/
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
		ret = MessageBox(hwnd, TEXT("�Ƿ�����ϴεĲо֣�"), TEXT("������"), MB_YESNO | MB_ICONQUESTION);
		if (ret == IDYES)
		{
			/*���for���������⣡���������ѽ��*/
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
	/*�жϿ�ʼ*/
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
			//���Ӣ�۰�Ϊ�գ�������ʾӢ�۰�����ֻ�����ļ��������ݲ�����ʾ����Ļ��
			fp = fopen("rank.txt", "rb");
			for (int i = 0; i < 10; i++)
			{
				fread(&Gamer[i], sizeof(struct ID), 1, fp);
				if (Gamer[i].count != 0)
					count++;
				if (i==0&&Gamer[i].count==0)
				{
					ret = MessageBox(hwnd, TEXT("��û�м�¼���������ɣ�"), TEXT("��������"), MB_OK);
					break;
				}
				/*printf("%c\n", Gamer[i].name[0]);
				printf("%s %d\n", Gamer[i].name, Gamer[i].count); */				
			}//����ȫ������	
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
			sprintf(out, "��һ����%s    +%d\n�ڶ�����%s    +%d\n��������%s    +%d\n", Gamer[0].name, Gamer[0].count, Gamer[1].name, Gamer[1].count, Gamer[2].name, Gamer[2].count);
			ret = MessageBox(hwnd, out, TEXT("��������"), MB_OK);
		}
		if (n.uMsg == WM_LBUTTONDOWN&&n.x > 750 && n.x<950 && n.y>300 & n.y < 350 || n.uMsg == WM_LBUTTONDOWN&&n.x>750 && n.x<950 && n.y>400 & n.y < 450)
		{
			return 0;
		}
	}

	/*���������Ϣ*/
	while (true)
	{
		n = GetMouseMsg();
		/*���¿�ʼ*/
		if (n.uMsg == WM_LBUTTONDOWN&&n.x > 750 && n.x<950 && n.y>100 && n.y < 150)
		{
				ret = MessageBox(hwnd, TEXT("���¿�ʼ��"), TEXT("���һ����"), MB_YESNO | MB_ICONQUESTION);
				if (ret == IDYES)
					load();
		}
		/*�鿴Ӣ�۰�*/
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
					ret = MessageBox(hwnd, TEXT("��û�м�¼���������ɣ�"), TEXT("��������"), MB_OK);
					break;
				}
			}//����ȫ������	
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
			sprintf(out, "��һ����%s    +%d\n�ڶ�����%s    +%d\n��������%s    +%d\n", Gamer[0].name, Gamer[0].count, Gamer[1].name, Gamer[1].count, Gamer[2].name, Gamer[2].count);
			ret = MessageBox(hwnd, out, TEXT("��������"), MB_OK);
		}
		/*�ݴ沢���˳�*/
		else if (n.uMsg == WM_LBUTTONDOWN&&n.x>750 && n.x<950 && n.y>300 & n.y < 350)
		{
			remove("E:\\Projects13\\ConsoleApplication4\\ConsoleApplication4\\record.txt");
			TmpSave();
			return 0;
		}
		/*���˳���ť*/
		else if (n.uMsg == WM_LBUTTONDOWN&&n.x>750 && n.x<950 && n.y>400 & n.y < 450)
		{
			remove("E:\\Projects13\\ConsoleApplication4\\ConsoleApplication4\\record.txt");
			return 0;
		}
		 /*��Ч����*/
		else if (n.x<71||n.x>507||n.y<80||n.y>515)
		 {
			 continue;
		 }
		 /*�ж��Ƿ�����*/
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