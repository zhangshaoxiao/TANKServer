#pragma once

#include "resource.h"
enum state { space, hardWall, frahWall, halfWall, glassWall, tankA, tankB, tankC, tankD };  //�����ͼö�����Ͷ�����Щש��,�Լ�̹������
struct Place
{
	state thing;

};
struct Player
{
	int Player_X;
	int Player_Y;
	int ID;               //�������ݣ�ABCD֮һ
};