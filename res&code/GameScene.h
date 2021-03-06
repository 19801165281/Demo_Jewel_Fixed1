#pragma once
#include <easy2d/easy2d.h>
#include "GameTask.h"
#include <mutex>

using namespace easy2d;

class GameScene:
	public Scene
{
public:
	GameScene();
	GameScene(int l);
	void onUpdate() override;
	~GameScene();
	static int get_selected_jewels_numbers();
	static void add_selected_jewels_numbers();
	static void init_selected_jewels_numbers();
	static void minu_selected_jewels_numbers();
	static Jewel*map[8][8];
	static bool canResume;
	GameTask *gt;
protected:
	static int selected_jewels_numbers;

};

	