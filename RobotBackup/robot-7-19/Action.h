#ifndef  __ACTION_H_
#define  __ACTION_H_
#include "InterFace.h"
//	-------------------------------基本运动函数声明----------------------------------------

// 设置小车轮数
void Velocity ( Robot *robot, int vl, int vr );				

// 小车转角控制
void Angle ( Robot *robot, int desired_angle);				

// 移动小车到指定位置
void Position ( Robot *robot, double x, double y );			

#endif // __ACTION_H_