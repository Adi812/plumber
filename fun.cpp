#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

int LINE = 5; //行
int COL = 5;  //列

vector<vector<string>> screen(LINE);
vector<vector<int>> num(LINE); //存放管道图案的数组
string part[] = { "┗", "┏", "┓", "┛", "━", "┃" };
int book[20][20]; //存放已经走过的路径 
int step; //所走的步数
int flag; //是否有通路
int now_ok=0; //经过变换之后此时是否有通路
int a[10] = { 0 }; //共10关，记录闯关者此时的关数
int index;//闯关数数组的索引
int pass_num;//此时所在关数
struct Path //记录走过的坐标
{
	int x;
	int y;
};
vector<Path> v;
Path pat;

void start()
{
	system("cls");
	cout << "----------------------------" << endl;
	cout << " 会上天入地，不如会修水管   " << endl;
	cout << " 你若掌握这项修水管技能     " << endl;
	cout << " 妹子看了想嫁人，汉子看了觉得技不如人" << endl;
	cout << "好了，我也只能帮你到这里了，哈哈" << endl;
	cout << "----------------------------" << endl;
	cout << "    欢迎来到修水管游戏      " << endl;
	cout << "  1.进入游戏(既然都来了，客官留步)   " << endl;
	cout << "  2.帮助(了解游戏玩法，胜者为王)" << endl;
	cout << "  0.退出游戏(你执意要走，小主不再挽留)   " << endl;
	cout << "----------------------------" << endl;

}

void explain()
{
	system("cls");
	cout << "南水北调游戏是一款益智类的小游戏，地图有两个端口，左上角的入口，用来引入长江等地的水源，" << endl;
	cout<<"右下角的出口，用来将水源送到京津地区，地图上的管道代表一个又一个细小的河流，只有将河流"<<endl;
	cout<<"接通，水源才能到达！玩家可以通过旋转河流的走向来接通水管，旋转哪一个，旋转几次玩家均可"<<endl;
	cout<<"自行选择。本游戏采取闯关模式，根据关数的增加，地图上的水流会越来越多，加油！"<<endl;
	cout << "返回上一级，请输入q"<<endl;
}

void init_vector()
{
	LINE = pass_num + 4;
	COL = pass_num + 4;
	num.resize(LINE);
	screen.resize(LINE);
	for (int i = 0; i < LINE; i++)
	{
		num[i].resize(COL);
		screen[i].resize(COL);
	}
}

void empty_date()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			book[i][j] = 0;
		}
	}

	v.erase(v.begin(), v.end());
	step = 0;
	now_ok = 0;
}

void print()
{
	for (int i = 0; i < LINE + 2; i++)
	{
		cout << "▼ ";
	}
	cout << endl;
	for (int i = 0; i < LINE; i++)
	{
		cout << "   ";
		for (int j = 0; j < COL; j++)
		{
			cout << screen[i][j].c_str() << "  ";
		}
		cout << endl;
		cout << endl;

	}
	for (int i = 0; i < LINE + 2; i++)
	{
		cout << "▲ ";
	}
	cout << endl;
}

void revlove(int i, int j, int step) //旋转
{
	if (num[i][j] < 4)
	{
		num[i][j] = (num[i][j] + step) % 4;
		screen[i][j] = part[num[i][j]];
	}
	else
	{
		if (num[i][j] == 4)
		{
			if (step % 2 == 1)
			{
				num[i][j] = num[i][j] + 1;
				screen[i][j] = part[num[i][j]];
			}
		}
		else
		{
			if (step % 2 == 1)
			{
				num[i][j] = 4;
				screen[i][j] = part[num[i][j]];
			}
		}
	}
	print();
}


void exit()
{
	cout << "请等待3秒   3";
	Sleep(1000);
	cout << "2";
	Sleep(1000);
	cout << "1";
	Sleep(1000);
	system("cls");
	cout << "----------------------------" << endl;
	cout << "是不是觉得已经离开了" << endl;
	cout << "----------------------------" << endl;
	cout << "小主怎么会轻易放过你（阴险脸.jpg)" << endl;
	cout << "----------------------------" << endl;
	cout << "你真的要走吗？？（可怜脸.jpg)" << endl;
	cout << "  1.返回上一层   " << endl;
	cout << "  0.退出游戏   " << endl;
}

//初始化水管
void initScreen()
{
	system("cls");
	srand(time(0));
	init_vector();
	empty_date();
	for (int i = 0; i < LINE+2; i++)
	{
		cout << "▼ ";
	}
	cout << endl;
	for (int i = 0; i < LINE; i++)
	{
		if (i == 0)
			cout << "进 " ;
		else
			cout << "   ";
		for (int j = 0; j < COL; j++)
		{
			int tmp = rand() % 6;
			if (i == 0 && j == 0) //a[0][0]有3个位置直通，便于观察，直接设置
			{
				while (tmp>4||tmp<2)
					tmp = rand() % 6;
			}
			screen[i][j] = part[tmp];
			cout << screen[i][j].c_str() << "  ";
			num[i][j] = tmp;
		}
		if (i == LINE-1)
		{
			cout << "出" << endl;
		}
		else
		{
			cout << endl;
		}
		cout << endl;
	}
	for (int i = 0; i < LINE+2; i++)
	{
		cout << "▲ ";
	}
	cout << endl;
}

//深度优先搜索
//进水口的方向 左1 上2 右3 下4

//判断此图是否有通路，有通路才可继续游戏，没有的话更换此时图
void dfs(int x, int y, int front)//front表示进水口的方向
{

	if (x == LINE - 1 && y == COL)
	{
		flag = 1;
		// 注释内容为测试代码	
		//cout << "找到方案" << endl;
		//vector<Path>::iterator it = v.begin();
		//while (it != v.end())
		//{
		//	cout << "(" << (*it).x << "," << (*it).y << ")" <<"--> ";
		//	it++;
		//}
		//cout << endl;
		return;
	}
	if (x<0 || x>LINE - 1 || y<0 || y>COL - 1) //越界
		return;
	if (book[x][y] == 1)//已经走过就不用再走了
		return;

	book[x][y] = 1; //标记此点已经走过
	++step;
	pat.x = x;
	pat.y = y;
	v.push_back(pat);

	//当前位置的管道是直管
	if (num[x][y] <= 5 && num[x][y] >= 4)
	{
		if (front == 1) //如果进水口在左边，所有下一个管道对应的进水口就是右边
			dfs(x, y + 1, 1);			   //且只能向右走,下一点的进水口在左边
		else if (front == 2)
			dfs(x + 1, y, 2);
		else if (front == 3)
			dfs(x, y - 1, 3);
		else if (front == 4)
			dfs(x - 1, y, 4);
	}
	//当前位置的管道是弯管
	else
	{
		if (front == 1)
		{
			dfs(x - 1, y, 4);
			dfs(x + 1, y, 2);
		}
		else if (front == 2)
		{
			dfs(x, y - 1, 3);
			dfs(x, y + 1, 1);
		}
		else if (front == 3)
		{
			dfs(x - 1, y, 4);
			dfs(x + 1, y, 2);
		}
		else if (front == 4)
		{
			dfs(x, y - 1, 3);
			dfs(x, y + 1, 1);
		}
	}
	book[x][y] = 0; //此位置已经退回
	--step;
	v.pop_back();
	return;
}

void dfs_every(int x, int y,int front)//front表示进水口的方向
{

	if (x == LINE - 1 && y == COL)
	{
		now_ok = 1;
		cout << "水流接通!" << endl;
		vector<Path>::iterator it = v.begin();
		while (it != v.end())
		{
			cout << "(" << (*it).x << "," << (*it).y << ")" << "--> ";
			it++;
		}
		cout << endl;
		return;
	}
	if (x<0 || x>LINE - 1 || y<0 || y>COL - 1) //越界
		return;
	if (book[x][y] == 1)//已经走过就不用再走了
		return;

	book[x][y] = 1; //标记此点已经走过
	++step;
	pat.x = x;
	pat.y = y;
	v.push_back(pat);

	//string part[] = { "┗", "┏", "┓", "┛", "━", "┃" };
	switch (num[x][y]) //判断下一个要走的位置
	{
	case 0:
		if (front == 2)
			dfs_every(x, y + 1, 1);
		else if (front == 3)
			dfs_every(x - 1, y, 4);
		break;
	case 1:
		if (front == 4)
			dfs_every(x, y + 1, 1);
		else if (front == 3)
			dfs_every(x + 1, y, 2);
		break;
	case 2:
		if (front == 1)
			dfs_every(x + 1, y, 2);
		else if (front == 4)
			dfs_every(x, y - 1, 3);
		break;
	case 3:
		if (front == 1)
			dfs_every(x - 1, y, 4);
		else if (front == 2)
			dfs_every(x, y - 1, 3);
		break;
	case 4:
		if (front == 1)
			dfs_every(x, y + 1, 1);
		else if (front == 3)
			dfs_every(x, y - 1, 3);
		break;
	case 5:
		if (front == 2)
			dfs_every(x + 1, y, 2);
		else if (front == 4)
			dfs_every(x - 1, y, 4);
		break;
	default:
		break;
	}

	book[x][y] = 0; //此位置已经退回
	--step;
	v.pop_back();
	return;
}
void print_pass()
{
	if (a[0] == 0)
		cout << "暂时还没有破解关卡，胜利之星即将亮起" << endl;
	for (int i = 0; i < 10; i++)
	{
		if (a[i] == 1)
			cout << i+1 << "  ★  ";
		else if (a[i] == 0)
			break;
	}
	cout << endl;
	while (1)
	{
		cout << "输入你想闯的关数" << endl;
		cout << "想直接进入下一关，输入-1!" << endl;
		cin >> pass_num;
		pass_num--;
		if (pass_num == 0)
			break;
		if (pass_num == -2)
		{
			int i = 0;
			for (; i < 10; i++)
			{
				 if (a[i] == 0)
					break;
			}
			pass_num = i ;
			break;
		}
		if (a[pass_num]==0 && a[pass_num-1]==0 )
		{
			cout << "你想要的关卡马上就要解锁了，再努力一把" << endl;
			cout << "当前已开关卡：";
			for (int i = 0; i < 10; i++)
			{
				if (a[i] == 1)
					cout << i+1;
				else if (a[i] == 0)
					break;
			}
			cout << endl;
		}
		else 
		{
			break;
		}
	}
	Sleep(5000);
}

void dfs_screen()
{
	int x, y, time;
	flag = 0;
	dfs(0, 0, 1);
	while (flag == 0)
	{
		if (flag == 0)
		{
			cout << "此图无通路,等待一下，正在更换" << endl;
			Sleep(1000);
			system("cls");
		}
		initScreen();
		dfs(0, 0, 1);
	}
	empty_date();
	clock_t time_start = clock();
	int run_time = (pass_num+2) * 60*1000;
	if (run_time == 0)
		run_time = 60*1000;
	cout << "为您提供两种模式进行游戏 "<<endl;
	cout << "1.直接输入坐标（可设定旋转次数）" << endl;
	cout << "2.通过 W上 A左 S下 D右进行方向调整，但是每次只旋转一次" << endl;
	int choose = 0;
	while (1)
	{
		cout << "请选择:";
		cin >> choose;
		if (choose == 1 || choose == 2)
			break;
		else
			cout << "输入不合法" << endl;
	}
	Path now_coor; //当前所在的坐标位置
	now_coor.x = now_coor.y = 0;
	char ch ;
	int is_revo;//当前坐标是否要旋转
	while (now_ok == 0)
	{
		if (choose == 1)
		{
			cout << "输入要调整的坐标：";
			cin >> x >> y;
			--x;
			--y;
			if (x >= LINE || x < 0 || y >= COL || y < 0)
			{
				cout << "坐标不合法，请重新输入" << endl;
				continue;
			}
			cout << "输入要调整的次数:";
			cin >> time;
			revlove(x, y, time);
		}
		else if (choose == 2)
		{
			cout << "当前坐标： (" << now_coor.x+1 << "," << now_coor.y+1 << ")" << endl;
			cout << "请按规定的方向键 W上 A左 S下 D右 E当前位置进行旋转" << endl;
			cout << "当前下一个位置的坐标是否旋转？旋转按1，不旋转按0" << endl;
			cout << "输入格式例如： w 0" << endl;
			cin >> ch>>is_revo;
			if (ch == 'w')
				--now_coor.x ;
			else if (ch == 'a')
				--now_coor.y;
			else if (ch == 's')
				++now_coor.x;
			else if (ch == 'd')
				++now_coor.y;
			else if (ch == 'e')
			{}
			else
			{
				cout << "输入不合法，请重新输入" << endl;
				continue;
			}
			if (now_coor.x >= LINE || now_coor.x < 0 || now_coor.y >= COL || now_coor.y < 0)
			{
				if (ch == 'w') //如果位置不合法，恢复原来的位置
					++now_coor.x;
				else if (ch == 'a')
					++now_coor.y;
				else if (ch == 's')
					--now_coor.x;
				else if (ch == 'd')
					--now_coor.y;
				cout << "坐标不合法，请重新输入" << endl;
				continue;
			}
			if (is_revo == 1)
			{
				revlove(now_coor.x, now_coor.y, 1);
			}
			else if (is_revo==0)
			{   
				print();
			}
			else
			{
				cout << "输入错误" << endl;
				continue;
			}
		}
		dfs_every(0, 0, 1);
		clock_t time_end = clock();
		if (time_end - time_start>=run_time-10000)
		{
			cout << "还剩不到10秒了，再快一点!" << endl;
		}
		else if (time_end - time_start>run_time)
		{
			cout << "时间到，这一关暂未破解!" << endl;
			Sleep(1000);
			print_pass();
			return;
		}

	}
	cout << "恭喜，恭喜，我就知道这点难度难不了你！（崇拜脸.jpg）" << endl;
	if (pass_num == index)
	{
		a[index++] = 1;
	}
	print_pass();

}

int main()
{
	int i;
	while (1)
	{
		start();
		cout << "做出你的选择：";
		cin >> i;
		if (i == 0)
		{
			exit();
			cout << "做出你的选择：";
			cin >> i;
			if (i == 0)
				return 0;
			else
			{
				continue;
			}
		}
		else if (i == 2)
		{
			explain();
			char q;
			while (1)
			{
				cin >> q;
				if (q == 'q')
				{
					break;
				}
				else
					cout << "输入字符不合法请重新输入" << endl;
			}
			continue;

		}
		else if (i == 1)
		{
			while (1)
			{
				initScreen();
				dfs_screen();
			}
		}
	}
	return 0;
}
