#include <math.h>
#include "Strategy.h"
#include "Action.h"

// ����С������
void Velocity ( Robot *robot, int vl, int vr )
{
	robot->velocityLeft  = vl;
	robot->velocityRight = vr;
}

// С��ת�ǿ���
void Angle ( Robot *robot, int desired_angle)
{
	int theta_e, vl, vr;
	theta_e = desired_angle - (int)robot->rotation;	
	
	while (theta_e > 180) theta_e -= 360;
	while (theta_e < -180) theta_e += 360;

	if (theta_e < -90) theta_e += 180;
	
	else if (theta_e > 90) theta_e -= 180;

	if (abs(theta_e) > 50) 
	{
		vl = (int)(-9./90.0 * (double)theta_e);
		vr = (int)(9./90.0 * (double)theta_e);
	}
	else if (abs(theta_e) > 20) 
	{
		vl = (int)(-11.0/90.0 * (double)theta_e);
		vr = (int)(11.0/90.0 * (double)theta_e);
	}
	Velocity (robot, vl, vr);
}

//�ƶ�С�����ƶ�λ��
void Position( Robot *robot, double x, double y )
{
	int desired_angle = 0, theta_e = 0, d_angle = 0, vl, vr, vc = 70;

	double dx, dy, d_e, Ka = 10.0/90.0;
	//���㵱ǰλ����Ŀ������λ��
	dx = x - robot->pos.x;
	dy = y - robot->pos.y;

	//���㵽Ŀ��λ�õ�ֱ�߾���
	d_e = sqrt(dx * dx + dy * dy);

	//���㵱ǰλ�õ�Ŀ���ĽǶ�
	if (dx == 0 && dy == 0)
		desired_angle = 90;
	else
		desired_angle = (int)(180. / PI * atan2((double)(dy), (double)(dx)));

	//��ǰ��������Ŀ��ļн�
	theta_e = desired_angle - (int)robot->rotation;
	
	while (theta_e > 180) theta_e -= 360;
	while (theta_e < -180) theta_e += 360;

	if (d_e > 100.) 
		Ka = 17. / 90.;
	else if (d_e > 50)
		Ka = 19. / 90.;
	else if (d_e > 30)
		Ka = 21. / 90.;
	else if (d_e > 20)
		Ka = 23. / 90.;
	else 
		Ka = 25. / 90.;
	
	if (theta_e > 95 || theta_e < -95)
	{
		theta_e += 180;
		
		if (theta_e > 180) 
			theta_e -= 360;
		if (theta_e > 80)
			theta_e = 80;
		if (theta_e < -80)
			theta_e = -80;
		if (d_e < 5.0 && abs(theta_e) < 40)
			Ka = 0.1;
		vr = (int)(-vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) + Ka * theta_e);
		vl = (int)(-vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) - Ka * theta_e);
	}
	
	else if (theta_e < 85 && theta_e > -85)
	{
		if (d_e < 5.0 && abs(theta_e) < 40)
			Ka = 0.1;
		vr = (int)( vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) + Ka * theta_e);
		vl = (int)( vc * (1.0 / (1.0 + exp(-3.0 * d_e)) - 0.3) - Ka * theta_e);
	}

	else
	{
		vr = (int)(+.17 * theta_e);
		vl = (int)(-.17 * theta_e);
	}

	Velocity(robot, vl, vr);
}
