#include "Jewel.h"
#include "GameScene.h"
#include <vector>
#include<windows.h>
#include <thread>
#include <mutex>
#include <windows.h>
using namespace std;


MatrixNode* p0;
MatrixNode* q0;
int GameScene::selected_jewels_numbers = 0;
Jewel* GameScene::map[8][8];
bool GameScene::canResume = true;

GameScene::GameScene()
{
	gt = new GameTask(1);//默认为第一关
}

GameScene::GameScene(int l)
{
	gt = new GameTask(l);
}

void GameScene::onUpdate(){

	
	//if (非暂停)
	if (GameScene::selected_jewels_numbers == 2)// 如果选中2个棋子
	{
		int a = 0, b = 0, flag = 0;
		int i, j;
		Jewel* jew1 = NULL;
		Jewel* jew2 = NULL;

		//加步数

		//step++


		//找到两个被选中的宝石
		for (i = 0; i < 8; i++) {//第一个宝石
			for (j = 0; j < 8; j++) {
				if (GameScene::map[i][j]->isSelected) {
					a = i;
					b = j;
					jew1 = GameScene::map[i][j];
					flag = 1;
					break;
				}
			}
			if (flag == 1) {
				break;
			}
		}

		flag = 0;

		for (i = 0; i < 8; i++) {//第二个宝石
			for (j = 0; j < 8; j++) {
				if (i == a && j == b) continue;
				if (GameScene::map[i][j]->isSelected) {
					jew2 = map[i][j];
					flag = 1;
					break;
				}
			}
			if (flag == 1) break;
		}

		//若两个棋子相邻
		if (((a == i) && ((b == j + 1) || (b == j - 1)) || ((b == j) && ((a == i + 1) || (a == i - 1))))) {

			int pos[4] = { a, b, i, j };
			MatrixNode* StatusSet = this->gt->breakTask(pos);
			int lenght = StatusSet->lenght();
			if (StatusSet)//如果交换可以产生消子
			{
				jew1->isSelected = false;
				jew2->isSelected = false;
				//动画上的交换
				Jewel::Exchange(jew1, jew2, 0, false);//交换动画
				GameScene::map[i][j] = jew1;
				GameScene::map[a][b] = jew2;
				//执行状态链表中的节点动画
				p0 = StatusSet;//捕获状态链表
			}
			else {
				//交换如果不能产生消子，播放动画
				// 执行顺序动画
				// 执行顺序动画
				Jewel::Exchange(jew1, jew2, 0, true);//交换动画
			}
			printf("The lenght of the set is %d\n", lenght);
		}
		else
		{
			jew1->Select();
			jew2->Select();
			jew1->isSelected = false;
			jew2->isSelected = false;
		}
		init_selected_jewels_numbers();
	}
	if (p0 && GameScene::canResume)
	{
		GameScene::canResume = false;
		//统计每一列下落情况
		int* empty_in_col;
		int* fall_start_from;
		empty_in_col = (int*)malloc(sizeof(int) * MAPCOLNUM);
		fall_start_from = (int*)malloc(sizeof(int) * MAPCOLNUM);
		if (!empty_in_col || !fall_start_from) exit(0);
		memset(empty_in_col, 0, sizeof(int) * MAPCOLNUM);
		memset(fall_start_from, 0, sizeof(int) * MAPCOLNUM);
		//先消子
		for (int j = 0; j < MAPCOLNUM; j++)
		{
			for (int i = MAPROWNUM - 1; i >= 0; i--)
			{
				if (p0->map[i][j] == 0)
				{
					GameScene::map[i][j]->Break();
					empty_in_col[j]++;
					fall_start_from[j] = i - 1;
				}
			}
		}
		//再下降
		p0 = p0->next;
		for (int j = 0; j < MAPCOLNUM; j++)
		{
			if (empty_in_col[j] == 0)
			{
				continue;
			}
			//将在界内的宝石下降到对应位置
			if (fall_start_from[j] >= 0)
			{
				for (int i = fall_start_from[j]; i >= 0; i--)
				{
					GameScene::map[i][j]->Fall(empty_in_col[j]);
					GameScene::map[i + empty_in_col[j]][j] = GameScene::map[i][j];
					GameScene::map[i][j] = nullptr;
				}
			}
			//新创建新生成的宝石，安排到对应列上方
			for (int k = 0; k < empty_in_col[j]; k++)
			{
				int jew_num = p0->map[empty_in_col[j] - k - 1][j];
				Jewel* new_jew = new Jewel(jew_num);
				new_jew->pos_row = 30.0f + 36.25f + 72.5f * (float)(-1);
				new_jew->pos_col = 414.0f + 36.25f + 72.5f * (float)(j);
				new_jew->setPosX(new_jew->pos_col);
				new_jew->setPosY(new_jew->pos_row);
				new_jew->setVisible(true);
				new_jew->setScale(0.8f);
				this->addChild(new_jew);
				new_jew->Fall(empty_in_col[j] - k);
				GameScene::map[empty_in_col[j] - k - 1][j] = new_jew;
			}
		}

		p0 = p0->next;
	}
}

GameScene::~GameScene()
{

}

int GameScene::get_selected_jewels_numbers()
{
	return selected_jewels_numbers;
}

void GameScene::add_selected_jewels_numbers()
{
	selected_jewels_numbers++;
}

void GameScene::init_selected_jewels_numbers()
{
	selected_jewels_numbers = 0;
}

void GameScene::minu_selected_jewels_numbers()
{
	selected_jewels_numbers--;
}

