#ifndef  __ACTION_H_
#define  __ACTION_H_
#include "InterFace.h"
//	-------------------------------�����˶���������----------------------------------------

// ����С������
void Velocity ( Robot *robot, int vl, int vr );				

// С��ת�ǿ���
void Angle ( Robot *robot, int desired_angle);				

// �ƶ�С����ָ��λ��
void Position ( Robot *robot, double x, double y );			

#endif // __ACTION_H_