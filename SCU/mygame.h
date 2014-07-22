#ifndef _MYGAME_H_
#define _MYGAME_H_


void NormalGame ( Environment *env );
void FreeBallGame(Environment *env);
void PlaceBallGame(Environment *env);
void PenaltyBallGame(Environment *env);
void FreeKickGame(Environment *env);
void GoalKickGame(Environment *env);
void Attack( Environment *env );
void Defence( Environment *env );
Vector3D Weakpoint(Environment *env);
void AttFoul( Environment *env );
void DefFoul( Environment *env );
void AreaKick( Environment *env,int robot,Vector3D pos,int radius);
void AreaDef( Environment *env,int robot,Vector3D pos,int radius);
void Penalty ( Environment *env );
void PositionAndThrough(Environment *env,int robot,Vector3D pos ,double MAX);

#endif //_MYGAME_H_