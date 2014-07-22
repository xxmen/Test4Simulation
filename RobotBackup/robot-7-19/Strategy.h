#ifndef	 _STRATEGY_H_
#define  _STRATEGY_H_

#include "InterFace.h"

//	-------------------------------策略函数声明----------------------------------------


// 防守球门
void Goalie1 ( Robot *robot, Environment *env );

// 控制小车在球场边线的控制
void NearBound2 ( Robot *robot, double vl, double vr, Environment *env );

// 抢球
void Attack2 ( Robot *robot, Environment *env );

// 防守
void Defend ( Robot *robot, Environment *env, double low, double high );

// 机器人向小球的位置移动的方法
void MoonAttack (Robot *robot, Environment *env );

// 对敌方的跟踪
void MoonFollowOpponent (  Robot *robot, OpponentRobot *opponent );

// 预估小球的位置
void PredictBall ( Environment *env );

#endif // _STRATEGY_H_
