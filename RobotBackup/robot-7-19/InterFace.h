#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#ifdef  STRATEGY_EXPORTS
#define STRATEGY_API __declspec(dllexport)
#else
#define STRATEGY_API __declspec(dllimport)
#endif

#include <string.h>
#include <stdio.h>

//	-------------------------------全局参数----------------------------------------

const double PI = 3.1415923;								// 

const long PLAYERS_PER_SIDE = 5;							// 每队球员个数

// 比赛状态
const long FREE_BALL     = 1;								// 当是自由球
const long PLACE_KICK    = 2;								// 点球
const long PENALTY_KICK  = 3;								// 罚球
const long FREE_KICK     = 4;								// 任意球
const long GOAL_KICK     = 5;								// 门球

// 发球方
const long ANYONES_BALL  = 0;								// 没有发球方
const long BLUE_BALL     = 1;								// 蓝队发球
const long YELLOW_BALL   = 2;								// 黄队发球

// 场地信息
const double FTOP        = 77.2392;							// 场地上边界
const double FBOT        = 6.3730;							// 场地下边界
const double FRIGHTX     = 93.4259;							// 场地右边界
const double FLEFTX      = 6.8118;							// 场地左边界

const double GTOPY       = 49.6801;							// 球门上边界
const double GBOTY       = 33.9320;							// 球门下边界
const double GRIGHT      = 97.3632;							// 球门右边界
const double GLEFT       = 2.8748;							// 球门左边界


//位置向量定义
//##ModelId=440D001A0187
typedef struct
{
	double x, y, z;
} Vector3D;

//区域范围定义
//##ModelId=440D001A01A5
typedef struct
{
	long left, right, top, bottom;
} Bounds;

//机器人的信息定义
//##ModelId=440D001A01D4
typedef struct
{
	Vector3D pos;											//机器人坐标
	double rotation;										//机器人方向角
	double velocityLeft, velocityRight;						//机器人左右轮速度
} Robot;

//对方机器人的信息的定义
//##ModelId=440D001A0204
typedef struct
{
	Vector3D pos;											//机器人的坐标位置
	double rotation;										//机器人当前的转角
} OpponentRobot;

//小球的位置定义
//##ModelId=440D001A0223
typedef struct
{
	Vector3D pos;											//小球的坐标位置
} Ball;

//主要的定义，包含所有运行时的的信息，由系统刷新
//##ModelId=440D001A0251
typedef struct
{
	Robot home[PLAYERS_PER_SIDE];							//我方机器人数组
	OpponentRobot opponent[PLAYERS_PER_SIDE];				//敌方机器人数组
	Ball currentBall,										//当前小球的位置
		 lastBall,											//上一次小球的位置
		 predictedBall;										//预计的小球的位置
	Bounds fieldBounds,										//场地范围
		   goalBounds;										//球门的位置与范围
	long gameState;											//当前游戏的状态
	long whosBall;											//由谁控制球
	void *userData;											//用户自定义信息
} Environment;		

//##ModelId=440D001A0271
typedef void (*MyStrategyProc)(Environment*);

extern "C" STRATEGY_API void Create ( Environment *env );	// Create    程序的初始化
extern "C" STRATEGY_API void Strategy ( Environment *env );	// Strategy  程序的主要执行逻辑
extern "C" STRATEGY_API void Destroy ( Environment *env );	// Destroy   程序的销毁

#endif // _INTERFACE_H_
