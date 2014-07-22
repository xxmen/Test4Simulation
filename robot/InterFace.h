#ifndef _INTERFACE_H_
#define _INTERFACE_H_

//#ifdef  STRATEGY_EXPORTS
#define STRATEGY_API __declspec(dllexport)//__declspec(dllexport):将一个函数声名为导出函数

//#else
//#define STRATEGY_API __declspec(dllimport)
//#endif

#include <string.h>
#include <stdio.h>

extern "C" STRATEGY_API void Create ( Environment *env );	// Create    程序的初始化
extern "C" STRATEGY_API void Strategy ( Environment *env );	// Strategy  程序的主要执行逻辑
extern "C" STRATEGY_API void Destroy ( Environment *env );	// Destroy   程序的销毁

//	-------------------------------全局参数----------------------------------------
//位置向量定义
//##ModelId=440D001A0187
typedef struct
{
	double x, y, z;
} Vector3D;
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
	Robot home[PLAYERS_PER_SIDE];//我方机器人数组
	OpponentRobot opponent[PLAYERS_PER_SIDE];//敌方机器人数组
	Ball currentBall, lastBall, predictedBall;//当前小球的位置,上一次小球的位置,预计的小球的位置
	Bounds fieldBounds, goalBounds;//场地范围,球门的位置与范围
	long gameState;	//当前游戏的状态0,1,2,3,4,5(据说这两个无效,需要自己判断...)
	long whoseBall; //由谁控制球0,1,2
	void *userData;//用户自定义信息
} Environment;
typedef struct
{
	Vector3D myoldpos[PLAYERS_PER_SIDE];	//记录 我方 队员的旧坐标,方向
	Vector3D myspeed[PLAYERS_PER_SIDE];		//纪录 我方 队员的速度，跑位的方向
	Vector3D myoldvelocity[PLAYERS_PER_SIDE];		//纪录 我方 队员上次驱动力

	Vector3D opoldpos[PLAYERS_PER_SIDE];	//记录 对方 队员的旧坐标,方向
	Vector3D opspeed[PLAYERS_PER_SIDE];		//纪录 对方 队员的速度，跑位的方向

	Robot robot[PLAYERS_PER_SIDE];			// 我方 球员
	OpponentRobot opp[PLAYERS_PER_SIDE];			// 对方 球员

	Vector3D oldball;							//纪录球 过去 的坐标
	Vector3D curball;							//纪录球 现在 的坐标
	Vector3D preball;							//纪录球 预测 的坐标
	Vector3D ballspeed;	//纪录球 预测 的坐标
	FILE*a;

	bool mygrand;							//是 黄队  ？
  bool locked;
	long gameState;	//0,1,2,3,4,5
	long whoseBall; //0,1,2
 
	bool flag;//重新开球标志
///////////////////自定义
	Vector3D ba[30];
}Mydata;
//##ModelId=440D001A0271
typedef void (*MyStrategyProc)(Environment*);
const	double InlitializeMyPosition_X[5]={
		10.6663,
		19.9199,
		19.7433,
		39.4618,
		39.8876
	};
const	double InlitializeMyPosition_Y[5]={
		42.3077,
		60.3647,
		22.9943,
		60.3031,
		23.1065
	};
const	double InlitializeMyPosition_Z[5]={
		90,
		0,
		0,
		0,
		0
	};
const	double InlitializeOppPosition_X[5]={
		90.4616,
		82.0921,
		81.2890,
		61.8525,
		61.4469
	};
const	double InlitializeOppPosition_Y[5]={
		42.2002,
		22.9046,
		60.4876,
		23.1527,
		60.3599
	};
const	double InlitializeOppPosition_Z[5]={
		-90,
		180,
		180,
		180,
		180
	};
// 场地信息
const double FTOP        = 77.2392;							// 场地上边界
const double FBOT        = 6.3730;							// 场地下边界
const double FRIGHTX     = 93.4259;							// 场地右边界
const double FLEFTX      = 6.8118;							// 场地左边界

const double CORRECTX = 0;//-2.942073;			//修正  坐标转换时的  偏差
const double CORRECTY = 0;//-6.283795;

const double GTOPY       = 49.6801;							// 球门上边界
const double GBOTY       = 33.9320;							// 球门下边界
const double GRIGHT      = 97.3632;							// 球门右边界
const double GLEFT       = 2.8748;							// 球门左边界
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


//主要的定义，包含所有运行时的的信息，由系统刷新
//##ModelId=440D001A0251

#endif // _INTERFACE_H_
