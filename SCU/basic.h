#ifndef _basic_H_
#define _basic_H_



#define STRATEGY_API __declspec(dllexport)

const int	 PLAYERS_PER_SIDE = 5;

//数据的精确度只有	0.1
// gameState
const int FREE_BALL = 1;
const int PLACE_KICK = 2;
const int PENALTY_KICK = 3;
const int FREE_KICK = 4;
const int GOAL_KICK = 5;

// whosBall
const int ANYONES_BALL = 0;
const int BLUE_BALL = 1;
const int YELLOW_BALL = 2;

/////以上用得少


// global variables -- Useful field positions ... maybe???
const double FTOP = 77.2392;				//球场上下左右中
const double FBOT = 6.3730;
const double FRIGHT = 93.4259;
const double FLEFT = 6.8118;
const double FMIDX=(FLEFT+FRIGHT)/2;
const double FMIDY=(FTOP+FBOT)/2;


const double CORRECTX = 0;//-2.942073;			//修正  坐标转换时的  偏差
const double CORRECTY = 0;//-6.283795;


const double GTOP = 49.6801;				//球门上下
const double GBOT = 33.9320;

const double GRIGHT = 97.3632;			//左右球网x坐标
const double GLEFT = 2.8748;		

const double LRIGHT = 79.3394;
const double LLEFT =21.2994;			//大禁区的位置参数
const double LTOP =57.5541;
const double LBOT =26.0581;

const double SRIGHT = 85.3632;
const double SLEFT = 13.555;			//小禁区的位置参数
const double STOP = 51.6486;
const double SBOT = 31.9636;

const double FREELEFT = 29.262907;			//自由球
const double FREERIGHT = 71.798508;
const double FREETOP = 64.454193;
const double FREEBOT = 18.184305;

const double PRIGHT = 78.329132;		//点球
const double PLEFT = 22.216028;

const double CORNER=5;		///角落为 边长 为5 的等腰三角形
const double CAR = 3.1;							//小车边长 3.1




//////////////////////MANY CONST NUMBERS
//底层模型的参数 ，期待更好的模型
const double SPEED_TANGENT = 0.81;
const double SPEED_NORMAL = 0.27;

const double SPEED_A=0.060;
const double SPEED_B=0.015222305;


const double ANGLE_A=0.273575;
const double ANGLE_B=0.534262;
const double ANGLE_K=0.000294678;

const double SPEED_ODD=0.662;	//0.338;左右轮速为0时的减速参数
const double SPEED_ZERO = 0.1896;	// 0 减速度 和 125减速度的临界值

typedef struct
{
	double x, y, z;
} Vector3D;

typedef struct
{
	long left, right, top, bottom;
} Bounds;

typedef struct
{
	Vector3D pos;
	double rotation;
	double velocityLeft, velocityRight;
} Robot;

typedef struct
{
	Vector3D pos;
	double rotation;
} OpponentRobot;

typedef struct
{
	Vector3D pos;
} Ball;

typedef struct
{
	Robot home[PLAYERS_PER_SIDE];
	OpponentRobot opponent[PLAYERS_PER_SIDE];
	Ball currentBall, lastBall, predictedBall;
	Bounds fieldBounds, goalBounds;
	long gameState;	//0,1,2,3,4,5
	long whoseBall; //0,1,2
	void *userData;
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


void MyCreate ( Environment *env ); 
void MyStrategy ( Environment *env );
void MyDestroy ( Environment *env ); 


//以下
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
const double PI = 3.1415926;
const Vector3D CONSTGATE={FRIGHT,(FTOP+FBOT)/2,0};		///zhuyi
const Vector3D MYGATE={FLEFT,(FTOP+FBOT)/2,0};

const	Vector3D BGatetop	={FRIGHT,GTOP,0};
const	Vector3D YGatetop	={FLEFT,GTOP,0};
const	Vector3D BGatebot	={FRIGHT,GBOT,0};
const	Vector3D YGatebot	={FLEFT,GBOT,0};

void See ( Environment *env );
void MyGame(Environment *env );
void End ( Environment *env );

extern "C" STRATEGY_API void Strategy ( Environment *env );
extern "C" STRATEGY_API void Create ( Environment *env );
extern "C" STRATEGY_API void Destroy ( Environment *env );



#endif //_basic_H_