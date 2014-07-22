#ifndef _basic_H_
#define _basic_H_



#define STRATEGY_API __declspec(dllexport)

const int	 PLAYERS_PER_SIDE = 5;

//���ݵľ�ȷ��ֻ��	0.1
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

/////�����õ���


// global variables -- Useful field positions ... maybe???
const double FTOP = 77.2392;				//������������
const double FBOT = 6.3730;
const double FRIGHT = 93.4259;
const double FLEFT = 6.8118;
const double FMIDX=(FLEFT+FRIGHT)/2;
const double FMIDY=(FTOP+FBOT)/2;


const double CORRECTX = 0;//-2.942073;			//����  ����ת��ʱ��  ƫ��
const double CORRECTY = 0;//-6.283795;


const double GTOP = 49.6801;				//��������
const double GBOT = 33.9320;

const double GRIGHT = 97.3632;			//��������x����
const double GLEFT = 2.8748;		

const double LRIGHT = 79.3394;
const double LLEFT =21.2994;			//�������λ�ò���
const double LTOP =57.5541;
const double LBOT =26.0581;

const double SRIGHT = 85.3632;
const double SLEFT = 13.555;			//С������λ�ò���
const double STOP = 51.6486;
const double SBOT = 31.9636;

const double FREELEFT = 29.262907;			//������
const double FREERIGHT = 71.798508;
const double FREETOP = 64.454193;
const double FREEBOT = 18.184305;

const double PRIGHT = 78.329132;		//����
const double PLEFT = 22.216028;

const double CORNER=5;		///����Ϊ �߳� Ϊ5 �ĵ���������
const double CAR = 3.1;							//С���߳� 3.1




//////////////////////MANY CONST NUMBERS
//�ײ�ģ�͵Ĳ��� ���ڴ����õ�ģ��
const double SPEED_TANGENT = 0.81;
const double SPEED_NORMAL = 0.27;

const double SPEED_A=0.060;
const double SPEED_B=0.015222305;


const double ANGLE_A=0.273575;
const double ANGLE_B=0.534262;
const double ANGLE_K=0.000294678;

const double SPEED_ODD=0.662;	//0.338;��������Ϊ0ʱ�ļ��ٲ���
const double SPEED_ZERO = 0.1896;	// 0 ���ٶ� �� 125���ٶȵ��ٽ�ֵ

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
	Vector3D myoldpos[PLAYERS_PER_SIDE];	//��¼ �ҷ� ��Ա�ľ�����,����
	Vector3D myspeed[PLAYERS_PER_SIDE];		//��¼ �ҷ� ��Ա���ٶȣ���λ�ķ���
	Vector3D myoldvelocity[PLAYERS_PER_SIDE];		//��¼ �ҷ� ��Ա�ϴ�������

	Vector3D opoldpos[PLAYERS_PER_SIDE];	//��¼ �Է� ��Ա�ľ�����,����
	Vector3D opspeed[PLAYERS_PER_SIDE];		//��¼ �Է� ��Ա���ٶȣ���λ�ķ���

	Robot robot[PLAYERS_PER_SIDE];			// �ҷ� ��Ա
	OpponentRobot opp[PLAYERS_PER_SIDE];			// �Է� ��Ա

	Vector3D oldball;							//��¼�� ��ȥ ������
	Vector3D curball;							//��¼�� ���� ������
	Vector3D preball;							//��¼�� Ԥ�� ������
	Vector3D ballspeed;	//��¼�� Ԥ�� ������
	FILE*a;

	bool mygrand;							//�� �ƶ�  ��
    bool locked;
	long gameState;	//0,1,2,3,4,5
	long whoseBall; //0,1,2
 
	bool flag;//���¿����־
	
///////////////////�Զ���
	Vector3D ba[30];




}Mydata;


void MyCreate ( Environment *env ); 
void MyStrategy ( Environment *env );
void MyDestroy ( Environment *env ); 


//����
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