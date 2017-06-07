#pragma once

#include "resource.h"
enum state { space, hardWall, frahWall, halfWall, glassWall, tankA, tankB, tankC, tankD };  //定义地图枚举类型都有哪些砖块,以及坦克类型
struct Place
{
	state thing;

};
struct Player
{
	int Player_X;
	int Player_Y;
	int ID;               //坐标和身份：ABCD之一
};