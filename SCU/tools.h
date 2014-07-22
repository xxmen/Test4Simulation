#ifndef _FUNCTION_H_
#define _FUNCTION_H_


void ReAngle(double &angle);
void PredictBall(Environment *env,int steps=1);
void PredictBall2(Environment *env);

double Atan(double y,double x);
double Atan(Vector3D begin,Vector3D end);
double AngleOne(double omiga,double vl,double vr);
double VelocityOne(double speed,double vl,double vr);		/// speed 以和robot同向为正
double Distance (Vector3D pos1,Vector3D pos2); //求两点之间的距离
double	Acos(double x);
double	Asin(double x);

void	bline(Vector3D p1,Vector3D p2,double&A,double &B,double &C);
void	bline(Vector3D p1,double angle,double&A,double &B,double &C);
double	Cos(double x);
void	Chkarea(Environment *env,int *no,int &m,int ctrl);
bool	bot(Vector3D point,double d);

bool	isbetween(double angle ,double angle1,double angle2);
bool	iscontinue(int*a,int n,int i);

Vector3D projection(Vector3D pos,Vector3D go,double angle);
bool	left(Vector3D point,double d);
bool	right(Vector3D point,double d);
double	Sin(double x);
Vector3D transfpos(Vector3D pos,double angle,double dis);
int		timeneed(Environment*env,int i,Vector3D go);
int		timeneed(Environment*env,int i,double dis ,double angle);
bool	top(Vector3D point,double d);


#endif //_FUNCTION_H_
