#include <math.h>
#include "Strategy.h"
#include "Action.h"


// 机器人向小球的位置移动的方法
void MoonAttack ( Robot *robot, Environment *env )
{
	//Velocity (robot, 127, 127);
	//Angle (robot, 45);

	//计算小球的位置
	PredictBall ( env );

	//移动机器人到小球的位置
	Position(robot, env->predictedBall.pos.x, env->predictedBall.pos.y);

	// Position(robot, 0.0, 0.0);
}

// 对敌方的跟踪
void MoonFollowOpponent ( Robot *robot, OpponentRobot *opponent )
{
	Position(robot, opponent->pos.x, opponent->pos.y);
}

// 预估小球的位置...
void PredictBall ( Environment *env )
{
	double dx = env->currentBall.pos.x - env->lastBall.pos.x;
	double dy = env->currentBall.pos.y - env->lastBall.pos.y;
	env->predictedBall.pos.x = env->currentBall.pos.x + dx;
	env->predictedBall.pos.y = env->currentBall.pos.y + dy;
}

// 防守球门
void Goalie1 ( Robot *robot, Environment *env )
{
	double velocityLeft = 0, velocityRight = 0;

	double Tx = env->goalBounds.right - env->currentBall.pos.x;
	double Ty = env->fieldBounds.top - env->currentBall.pos.y;

	double Ax = env->goalBounds.right - robot->pos.x;
	double Ay = env->fieldBounds.top - robot->pos.y;

	if ( Ay > Ty + 0.9 && Ay > 27 )
	{
		velocityLeft = -100;
		velocityRight = -100;
	}

	if ( Ay > Ty - 0.9 && Ay < 43 )
	{
		velocityLeft = 100;
		velocityRight = 100;
	}

	if ( Ay < 27 )
	{
		velocityLeft = 100;
		velocityRight = 100;
	}

	if ( Ay > 43 )
	{
		velocityLeft = -100;
		velocityRight = -100;
	}

	double Tr = robot->rotation;
	if ( Tr < 0.001 )
		Tr = Tr + 360;
	if ( Tr > 360.001 )
		Tr = Tr - 360;
	if ( Tr > 270.5 )
		velocityRight = velocityRight + fabs ( Tr - 270 );
	else if ( Tr < 269.5 )
		velocityLeft = velocityLeft + fabs ( Tr - 270 );

	robot->velocityLeft = velocityLeft;
	robot->velocityRight = velocityRight;
}


// 抢球
void Attack2 ( Robot *robot, Environment *env )
{
	Vector3D t = env->currentBall.pos;
	double r = robot->rotation;
	if ( r < 0 ) r += 360;
	if ( r > 360 ) r -= 360;
	double vl = 0, vr = 0;

	if ( t.y > env->fieldBounds.top - 2.5 ) t.y = env->fieldBounds.top - 2.5;
	if ( t.y < env->fieldBounds.bottom + 2.5 ) t.y = env->fieldBounds.bottom + 2.5;
	if ( t.x > env->fieldBounds.right - 3 ) t.x = env->fieldBounds.right - 3;
	if ( t.x < env->fieldBounds.left + 3 ) t.x = env->fieldBounds.left + 3;

	double dx = robot->pos.x - t.x;
	double dy = robot->pos.y - t.y;

	double dxAdjusted = dx;
	double angleToPoint = 0;

	if ( fabs ( robot->pos.y - t.y ) > 7 || t.x > robot->pos.x )
		dxAdjusted -= 5;

	if ( dxAdjusted == 0 )
	{
		if ( dy > 0 )
			angleToPoint = 270;
		else
			angleToPoint = 90;
	}
	else if ( dy == 0 )
	{
		if ( dxAdjusted > 0 )
			angleToPoint = 360;
		else
			angleToPoint = 180;

	}
	else
		angleToPoint = atan ( fabs ( dy / dx ) ) * 180.0 / PI;

	if ( dxAdjusted > 0 )
	{
		if ( dy > 0 )
			angleToPoint -= 180;
		else if ( dy < 0 )
			angleToPoint = 180 - angleToPoint;
	}
	if ( dxAdjusted < 0 )
	{
		if ( dy > 0 )
			angleToPoint = - angleToPoint;
		else if ( dy < 0 )
			angleToPoint = 90 - angleToPoint;
	}

	if ( angleToPoint < 0 ) angleToPoint = angleToPoint + 360;
	if ( angleToPoint > 360 ) angleToPoint = angleToPoint - 360;
	if ( angleToPoint > 360 ) angleToPoint = angleToPoint - 360;

	double c = r;

	double angleDiff = fabs ( r - angleToPoint );

	if ( angleDiff < 40 )
	{
		vl = 100;
		vr = 100;
		if ( c > angleToPoint )
			vl -= 10;
		if ( c < angleToPoint )
			vr -= 10;
	}
	else
	{
		if ( r > angleToPoint )
		{
			if ( angleDiff > 180 )
				vl += 360 - angleDiff;
			else
				vr += angleDiff;
		}
		if ( r < angleToPoint )
		{
			if ( angleDiff > 180 )
				vr += 360 - angleDiff;
			else
				vl += angleDiff;
		}
	}

	NearBound2 ( robot, vl, vr, env );
}

// 控制小车在球场边线的控制
void NearBound2 ( Robot *robot, double vl, double vr, Environment *env )
{
	//Vector3D t = env->currentBall.pos;

	Vector3D a = robot->pos;
	double r = robot->rotation;

	if ( a.y > env->fieldBounds.top - 15 && r > 45 && r < 130 )
	{
		if ( vl > 0 )
			vl /= 3;
		if ( vr > 0 )
			vr /= 3;
	}

	if ( a.y < env->fieldBounds.bottom + 15 && r < -45 && r > -130 )
	{
		if ( vl > 0 ) vl /= 3;
		if ( vr > 0 ) vr /= 3;
	}

	if ( a.x > env->fieldBounds.right - 10 )
	{
		if ( vl > 0 )
			vl /= 2;
		if ( vr > 0 )
			vr /= 2;
	}

	if ( a.x < env->fieldBounds.left + 10 )
	{
		if ( vl > 0 )
			vl /= 2;
		if ( vr > 0 )
			vr /= 2;
	}

	robot->velocityLeft = vl;
	robot->velocityRight = vr;
}

// 防守
void Defend ( Robot *robot, Environment *env, double low, double high )
{
	double vl = 0, vr = 0;
	Vector3D z = env->currentBall.pos;

	double Tx = env->goalBounds.right - z.x;
	double Ty = env->fieldBounds.top - z.y;
	Vector3D a = robot->pos;
	a.x = env->goalBounds.right - a.x;
	a.y = env->fieldBounds.top - a.y;

	if ( a.y > Ty + 0.9 && a.y > low )
	{
		vl = -100;
		vr = -100;
	}
	if ( a.y < Ty - 0.9 && a.y < high )
	{
		vl = 100;
		vr = 100;
	}
	if ( a.y < low )
	{
		vl = 100;
		vr = 100;
	}
	if ( a.y > high )
	{
		vl = -100;
		vr = -100;
	}

	double Tr = robot->rotation;

	if ( Tr < 0.001 )
		Tr += 360;
	if ( Tr > 360.001 )
		Tr -= 360;
	if ( Tr > 270.5 )
		vr += fabs ( Tr - 270 );
	else if ( Tr < 269.5 )
		vl += fabs ( Tr - 270 );

	NearBound2 ( robot, vl ,vr, env );
}


