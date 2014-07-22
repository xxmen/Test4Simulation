#ifndef _ACTION_H_
#define _ACTION_H_

void PositionAndStop(Environment *env,int  robot,Vector3D pos ,double bestangle=90,double limit=1);
void PositionAndStopX(Environment *env,int  robot,Vector3D pos ,double Xangle=90,double limit=2); 
void PositionAndThrough(Environment *env,int robot,Vector3D pos ,double MAX=125);		//
void Kick(Environment *env , int  robot , Vector3D Topos=CONSTGATE );
void MyKick(Environment *env , int  robot , Vector3D Topos=CONSTGATE );
void Velocity(Environment *env, int robot,double vl,double vr);

void Angle( Environment *env, int robot,double angle);
void Angle( Environment *env, int robot,Vector3D pos);
void PAngle( Environment *env, int robot,double angle,double speed=0);
void GoaliePosition(Environment *env,int  robot,Vector3D pos ,double bestangle=90,double limit=1.5);

#endif //_ACTION_H_