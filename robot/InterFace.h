#ifndef _INTERFACE_H_
#define _INTERFACE_H_

//#ifdef  STRATEGY_EXPORTS
#define STRATEGY_API __declspec(dllexport)//__declspec(dllexport):��һ����������Ϊ��������

//#else
//#define STRATEGY_API __declspec(dllimport)
//#endif

#include <string.h>
#include <stdio.h>

extern "C" STRATEGY_API void Create ( Environment *env );	// Create    ����ĳ�ʼ��
extern "C" STRATEGY_API void Strategy ( Environment *env );	// Strategy  �������Ҫִ���߼�
extern "C" STRATEGY_API void Destroy ( Environment *env );	// Destroy   ���������

//	-------------------------------ȫ�ֲ���----------------------------------------
//λ����������
//##ModelId=440D001A0187
typedef struct
{
	double x, y, z;
} Vector3D;
typedef struct
{
	Vector3D pos;											//����������
	double rotation;										//�����˷����
	double velocityLeft, velocityRight;						//�������������ٶ�
} Robot;

//�Է������˵���Ϣ�Ķ���
//##ModelId=440D001A0204
typedef struct
{
	Vector3D pos;											//�����˵�����λ��
	double rotation;										//�����˵�ǰ��ת��
} OpponentRobot;

//С���λ�ö���
//##ModelId=440D001A0223
typedef struct
{
	Vector3D pos;											//С�������λ��
} Ball;

//����Χ����
//##ModelId=440D001A01A5
typedef struct
{
	long left, right, top, bottom;
} Bounds;

//�����˵���Ϣ����
//##ModelId=440D001A01D4		
typedef struct
{
	Robot home[PLAYERS_PER_SIDE];//�ҷ�����������
	OpponentRobot opponent[PLAYERS_PER_SIDE];//�з�����������
	Ball currentBall, lastBall, predictedBall;//��ǰС���λ��,��һ��С���λ��,Ԥ�Ƶ�С���λ��
	Bounds fieldBounds, goalBounds;//���ط�Χ,���ŵ�λ���뷶Χ
	long gameState;	//��ǰ��Ϸ��״̬0,1,2,3,4,5(��˵��������Ч,��Ҫ�Լ��ж�...)
	long whoseBall; //��˭������0,1,2
	void *userData;//�û��Զ�����Ϣ
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
// ������Ϣ
const double FTOP        = 77.2392;							// �����ϱ߽�
const double FBOT        = 6.3730;							// �����±߽�
const double FRIGHTX     = 93.4259;							// �����ұ߽�
const double FLEFTX      = 6.8118;							// ������߽�

const double CORRECTX = 0;//-2.942073;			//����  ����ת��ʱ��  ƫ��
const double CORRECTY = 0;//-6.283795;

const double GTOPY       = 49.6801;							// �����ϱ߽�
const double GBOTY       = 33.9320;							// �����±߽�
const double GRIGHT      = 97.3632;							// �����ұ߽�
const double GLEFT       = 2.8748;							// ������߽�
const double PI = 3.1415923;								// 

const long PLAYERS_PER_SIDE = 5;							// ÿ����Ա����

// ����״̬
const long FREE_BALL     = 1;								// ����������
const long PLACE_KICK    = 2;								// ����
const long PENALTY_KICK  = 3;								// ����
const long FREE_KICK     = 4;								// ������
const long GOAL_KICK     = 5;								// ����

// ����
const long ANYONES_BALL  = 0;								// û�з���
const long BLUE_BALL     = 1;								// ���ӷ���
const long YELLOW_BALL   = 2;								// �ƶӷ���


//��Ҫ�Ķ��壬������������ʱ�ĵ���Ϣ����ϵͳˢ��
//##ModelId=440D001A0251

#endif // _INTERFACE_H_
