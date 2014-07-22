#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#ifdef  STRATEGY_EXPORTS
#define STRATEGY_API __declspec(dllexport)
#else
#define STRATEGY_API __declspec(dllimport)
#endif

#include <string.h>
#include <stdio.h>

//	-------------------------------ȫ�ֲ���----------------------------------------

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

// ������Ϣ
const double FTOP        = 77.2392;							// �����ϱ߽�
const double FBOT        = 6.3730;							// �����±߽�
const double FRIGHTX     = 93.4259;							// �����ұ߽�
const double FLEFTX      = 6.8118;							// ������߽�

const double GTOPY       = 49.6801;							// �����ϱ߽�
const double GBOTY       = 33.9320;							// �����±߽�
const double GRIGHT      = 97.3632;							// �����ұ߽�
const double GLEFT       = 2.8748;							// ������߽�


//λ����������
//##ModelId=440D001A0187
typedef struct
{
	double x, y, z;
} Vector3D;

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

//��Ҫ�Ķ��壬������������ʱ�ĵ���Ϣ����ϵͳˢ��
//##ModelId=440D001A0251
typedef struct
{
	Robot home[PLAYERS_PER_SIDE];							//�ҷ�����������
	OpponentRobot opponent[PLAYERS_PER_SIDE];				//�з�����������
	Ball currentBall,										//��ǰС���λ��
		 lastBall,											//��һ��С���λ��
		 predictedBall;										//Ԥ�Ƶ�С���λ��
	Bounds fieldBounds,										//���ط�Χ
		   goalBounds;										//���ŵ�λ���뷶Χ
	long gameState;											//��ǰ��Ϸ��״̬
	long whosBall;											//��˭������
	void *userData;											//�û��Զ�����Ϣ
} Environment;		

//##ModelId=440D001A0271
typedef void (*MyStrategyProc)(Environment*);

extern "C" STRATEGY_API void Create ( Environment *env );	// Create    ����ĳ�ʼ��
extern "C" STRATEGY_API void Strategy ( Environment *env );	// Strategy  �������Ҫִ���߼�
extern "C" STRATEGY_API void Destroy ( Environment *env );	// Destroy   ���������

#endif // _INTERFACE_H_
