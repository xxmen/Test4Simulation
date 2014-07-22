#include <string.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#include "basic.h"
#include "tools.h"
#include "action.h"


void Velocity(Environment *env, int robot,double vl,double vr)
{
    Mydata * p;
    p=(Mydata *)env->userData;

    //vl,vr都有取值范围的!!!
    if(vl>125)vl=125;
    if(vl<-125)vl=-125;
    if(vr>125)vr=125;
    if(vr<-125)vr=-125;

    if(true)
    {
        //速度的特别控制
        if(vl==0 && vr!=0)
            vl=0.00001;
        if(vr==0 && vl!=0)
            vr=0.00001;
    }

    p->robot[robot].velocityLeft = vl;
    p->robot[robot].velocityRight= vr;

}

//
/*PositionAndStop 让robot 跑到pos，并且停下来，
bestangle 是停下来之后的朝向，
limit	控制停在pos附近的距离

*/
//
void PositionAndStop(Environment *env,int  robot,Vector3D pos ,double bestangle,double limit)
{
    //考虑到可能的	急停和 急快速加速
    //特别作了优化
    //还有就是 被碰转后的转角过程 不能耽搁时间!!!
    //转角是最危险的过程

    Mydata * p;
    p=(Mydata *)env->userData;

    double anglespeedmax=0;		//控制转交速度的变量
    double vmax=125;			//默认的跑位加速度
    double Limitedangle=2;		//默认减速范围

    if( limit < 0.5 )
        limit =0.5;
    double Limiteddis=limit;	//减速范围有一个下限，保证不会来回跑动

    double  distance;			//robot和目标点的距离
    double turnangle,posangle,vangle;	//转动角度 ，目标点相对robot的角度，速度的绝对角度
    double dx,dy;				//pos  和robot的坐标差
    double a=SPEED_A;			//参数
    double b=SPEED_B;
    double v,v1;				//临时保存速度的大小!!!
    double f=vmax;				//加速度变量
    double s=0;					//预测的减速位移(路程)
    int n=0;					//跑位的步数
    bool face=true;			//判断小车是否是正面前进

    v= sqrt(p->myspeed[robot].x * p->myspeed[robot].x + p->myspeed[robot].y*p->myspeed[robot].y);
    //临时保存速度的大小!!!
    dx = pos.x - p->robot[robot].pos.x ;		//pos  和robot的坐标差
    dy = pos.y - p->robot[robot].pos.y ;

    distance = Distance(p->robot[robot].pos , pos);
    posangle = Atan(dy,dx);

    turnangle = p->robot[robot].rotation - posangle;		//转动角度
    ReAngle(turnangle);

    if(turnangle > 90)
    {
        //判断小车是否是正面前进
        face=false;
        turnangle-=180;
    }
    else if(turnangle < -90)
    {
        face=false;
        turnangle+=180;
    }
    else
    {
        face=true;
    }

    vangle = p->myspeed[robot].z - p->robot[robot].rotation;		//速度的方向和robot正面的夹角
    ReAngle(vangle);					//主要用于最后控制减速度的大小
    if( vangle <-90 || vangle > 90 )		//同时判断v的正负
        v=-v;

    if(face)
    {
        //forward	跑位，如果后退的话  就v=0
        //设vl,vr=0 还是vl,vr=125 有一个条件有一个临界条件那就是
        //v = SPEED_ZERO
        if(v < -SPEED_ZERO)
        {
            Velocity(env,robot,0,0);
            return ;
        }
    }
    else if(v > SPEED_ZERO)
    {
        //back		跑位，如果后退的话  就v=0
        Velocity(env,robot,0,0);
        return ;
    }

    v1=v;	//v1 is changing while program running
    //whlie, v is not

    if(distance > Limiteddis )
    {
        //it is too early to count the steps
        //but the Limiteddis should be tested!!	to do...
        if(turnangle > Limitedangle || turnangle < -Limitedangle)
        {
            //adjust angle
            /////////////////测试这一段
            //对于goalie这一段应该特别注意
            //发生变向	1.knock the robot,especially the opponent
            //	2.knock the wall
            // so the anglespeedmax is allowed ++ more!!
            if(turnangle > 20  || turnangle < -20)
                anglespeedmax = 0;
            else if(turnangle > 10  || turnangle < -10)
                anglespeedmax = 125;
            else if(turnangle > 5  || turnangle < -5)
                anglespeedmax = 180;
            else
                anglespeedmax = 200;
            ///////////////测试这一段
            PAngle(env,robot,posangle,anglespeedmax);
        }
        else
        {
            if(face)
                Velocity(env,robot,f,f);
            else
                Velocity(env,robot,-f,-f);
        }//it is time to rush
    }
    else
    {
        if(distance > 1)
        {
            //调整角度	return!!!!!!
            //radious of robot is about 1.5 ,so the distance is very short
            if(turnangle > Limitedangle || turnangle < -Limitedangle)
            {
                Angle(env,robot,posangle);
                return ;
            }
        }

        if(distance < 0.4)
        {
            //停止并转向		return!!!!!!
            //radious of robot is about 1.5 ,so the distance is very short
            if( v<0.1 && v>-0.1)
            {
                //the range of v shoud be tested
                Angle(env,robot,bestangle);
                return	;
            }
        }

        if(true)
        {
            vmax=125;
            if(face)
            {
                f=-vmax;		//减速度  为  0000000
                v1=VelocityOne(v1,-f,-f);		//加速一步
                s=v1;
                do //whether to reduce
                {
                    if(v1 > SPEED_ZERO)	//as i said,this is limited
                        v1=VelocityOne(v1,0,0);
                    else
                        v1=VelocityOne(v1,f,f);
                    s+=v1;
                }
                while( v1 > 0 );

                s-=v1;

                if(s < distance)
                {
                    //不满足减速条件加速
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v > SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//减速一步
                        if( v1 < 0 )
                        {
                            do //该降低功率了
                            {
                                f++;		//f=-vmax;
                                v1 = VelocityOne(v,f,f);
                            }
                            while( v1 < distance && f < vmax);
                        }
                        Velocity(env,robot,f,f);
                    }
                }
            }
            else
            {
                f=vmax;		//减速度!!!!!
                v1=VelocityOne(v1,-f,-f);
                s=v1;
                do //whether to reduce
                {
                    if(v1 < -SPEED_ZERO)	//as i said,this is limited
                        v1=VelocityOne(v1,0,0);
                    else
                        v1=VelocityOne(v1,f,f);
                    s+=v1;
                }
                while( v1 < -0.1 );

                s-=v1;

                if(s > -distance)
                {
                    //不满足减速条件加速
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v < -SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//减速一步
                        if( v1 > 0 )
                        {
                            do //该降低功率了
                            {
                                f--;		//f=-vmax;
                                v1 = VelocityOne(v,f,f);
                            }
                            while( v1 > -distance && f > -vmax);
                        }
                        Velocity(env,robot,f,f);
                    }
                }
            }
        }
    }
}

//
/*PositionAndStopX 让robot 跑到pos，并且停下来原地旋转，
Xangle	旋转的角速度
limit	控制停在pos附近的距离

*/
//
void PositionAndStopX(Environment *env,int  robot,Vector3D pos ,double Xangle,double limit)
{
    Mydata * p;
    p=(Mydata *)env->userData;

    double anglespeedmax=0;		//控制转交速度的变量
    double vmax=125;
    double Limitedangle=2;

    if( limit < 2 )
        limit =2;
    double Limiteddis=limit;

    double  distance;
    double turnangle,posangle,vangle;
    double dx,dy;
    double a=SPEED_A;
    double b=SPEED_B;
    double v,v1;
    double f=vmax;
    double s=0;
    int n=0;
    bool face=true;			//判断小车是否是正面前进

    v= sqrt(p->myspeed[robot].x * p->myspeed[robot].x + p->myspeed[robot].y*p->myspeed[robot].y);

    dx = pos.x - p->robot[robot].pos.x ;
    dy = pos.y - p->robot[robot].pos.y ;

    distance = Distance(p->robot[robot].pos , pos);
    posangle = Atan(dy,dx);

    turnangle = p->robot[robot].rotation - posangle;		//think more!!
    ReAngle(turnangle);

    if(turnangle > 90)
    {
        face=false;
        turnangle-=180;
    }
    else if(turnangle < -90)
    {
        face=false;
        turnangle+=180;
    }
    else
    {
        face=true;
    }

    vangle = p->myspeed[robot].z - p->robot[robot].rotation;
    ReAngle(vangle);
    if( vangle <-90 || vangle > 90 )
        v=-v;
    v1=v;

    if(distance > Limiteddis )
    {
        //it is too early to count the steps
        if(turnangle > Limitedangle || turnangle < -Limitedangle)
        {
            //adjust angle
            /////////////////测试这一段
            if(turnangle > 20  || turnangle < -20)
                anglespeedmax = 0;
            else if(turnangle > 10  || turnangle < -10)
                anglespeedmax = 125;
            else if(turnangle > 5  || turnangle < -5)
                anglespeedmax = 180;
            else
                anglespeedmax = 200;
            ///////////////测试这一段
            PAngle(env,robot,posangle,anglespeedmax);

        }
        else
        {
            if(face)
                Velocity(env,robot,f,f);
            else
                Velocity(env,robot,-f,-f);
        }//it is time to rush
    }
    else
    {
        if(distance > 1)
        {
            //调整角度	return!!!!!!
            if(turnangle > Limitedangle || turnangle < -Limitedangle)
            {
                Angle(env,robot,posangle);
                return ;
            }

        }

        if(distance < 1)
        {
            //停止并转向		return!!!!!!
            if( v<0.5 && v>-0.5)
            {
                Velocity(env,robot,-Xangle,Xangle);
                return	;
            }
        }

        if(true)
        {
            vmax=125;
            if(face)
            {
                f=-vmax;		//减速度  为  0000000
                v1=VelocityOne(v1,-f,-f);		//加速一步
                s=v1;
                do //whether to reduce
                {
                    if(v1 > SPEED_ZERO)	//as i said,this is limited
                        v1=VelocityOne(v1,0,0);
                    else
                        v1=VelocityOne(v1,f,f);
                    s+=v1;
                }
                while( v1 > 0 );

                s-=v1;

                if(s < distance)
                {
                    //不满足减速条件加速
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v > SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//减速一步
                        if( v1 < 0 )
                        {
                            do //该降低功率了
                            {
                                f++;		//f=-vmax;
                                v1 = VelocityOne(v,f,f);
                            }
                            while( v1 < distance && f < vmax);
                        }
                        Velocity(env,robot,f,f);
                    }
                }
            }
            else
            {
                f=vmax;		//减速度!!!!!
                v1=VelocityOne(v1,-f,-f);
                s=v1;
                do //whether to reduce
                {
                    if(v1 < -SPEED_ZERO)	//as i said,this is limited
                        v1=VelocityOne(v1,0,0);
                    else
                        v1=VelocityOne(v1,f,f);
                    s+=v1;
                }
                while( v1 < -0.1 );

                s-=v1;

                if(s > -distance)
                {
                    //不满足减速条件加速
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v < -SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//减速一步
                        if( v1 > 0 )
                        {
                            do //该降低功率了
                            {
                                f--;		//f=-vmax;
                                v1 = VelocityOne(v,f,f);
                            }
                            while( v1 > -distance && f > -vmax);
                        }
                        Velocity(env,robot,f,f);
                    }
                }
            }
        }
    }
}

//
/*PositionAndThrough 让robot以最快MAX 冲向pos，中间没有减速控制


*/
//
void PositionAndThrough(Environment *env,int robot,Vector3D pos ,double MAX)
{
    Mydata * p;
    p=(Mydata *)env->userData;

    double anglespeedmax=0;		//控制转交速度的变量
    double max=MAX;
    double Limitedangle=2;
    double Limiteddis=0;
    double  distance;
    double turnangle,posangle,vangle;
    double dx,dy;
    double a=SPEED_A;
    double b=SPEED_B;
    double v,v1;
    double f;
    double s=0;
    int n=0;
    bool face=true;			//判断小车是否是正面前进

    v= sqrt(p->myspeed[robot].x * p->myspeed[robot].x + p->myspeed[robot].y*p->myspeed[robot].y);

    dx = pos.x - p->robot[robot].pos.x ;
    dy = pos.y - p->robot[robot].pos.y ;

    distance = Distance(p->robot[robot].pos , pos);
    posangle = Atan(dy,dx);

    turnangle = posangle - p->robot[robot].rotation;		//think more!!
    ReAngle(turnangle);

    if(turnangle > 90)
    {
        face=false;
        turnangle-=180;
    }
    else if(turnangle < -90)
    {
        face=false;
        turnangle+=180;
    }
    else
    {
        face=true;
    }

    vangle = p->myspeed[robot].z - posangle;
    ReAngle(vangle);
    if( vangle <-90 || vangle > 90 )
        v=-v;
    v1=v;

    if(distance > Limiteddis)
    {
        //it is too early to count the steps
        if(turnangle > Limitedangle || turnangle < -Limitedangle)
        {
            //adjust angle
            /////////////////测试这一段
            if(turnangle > 20  || turnangle < -20)
                anglespeedmax = 0;
            else if(turnangle > 10  || turnangle < -10)
                anglespeedmax = 125;
            else if(turnangle > 5  || turnangle < -5)
                anglespeedmax = 180;
            else
                anglespeedmax = 200;
            ///////////////测试这一段
            PAngle(env,robot,posangle,anglespeedmax);

        }
        else
        {
            f=max;
            if(face)
                Velocity(env,robot,f,f);
            else
                Velocity(env,robot,-f,-f);

        }//it is time to rush

    }
    else
    {


    }//abserlutely count
}


////////////
/* Kick	让robot把球踢到ToPos的位置

*/
////////////
void Kick(Environment *env , int  robot , Vector3D ToPos )
{
    Mydata * p;
    p=(Mydata *)env->userData;

    Vector3D ball = p->curball;		//这个球的位置不保险!!!

    Vector3D RobotToBall;		//人和球的相对位置
    RobotToBall.x = ball.x - p->robot[robot].pos.x ;
    RobotToBall.y = ball.y - p->robot[robot].pos.y ;
    RobotToBall.z = Atan(p->robot[robot].pos , ball);

    Vector3D BallToGate ;		//球和球门的相对位置
    BallToGate.x = ToPos.x - ball.x ;
    BallToGate.y = ToPos.y - ball.y ;
    BallToGate.z = Atan(ball , ToPos);

    double RunAngle ;
    RunAngle = RobotToBall.z - BallToGate.z;
    ReAngle(RunAngle);

    RunAngle = RobotToBall.z + RunAngle / 2 ;	// 可以调整  2
    ReAngle(RunAngle);

    PAngle(env,robot,RunAngle,125);

}

void MyKick(Environment * env, int robot, Vector3D Topos)
{
    //处理了边线死球
    Mydata * p;
    p=(Mydata *)env->userData;

    double x1 = 10;
    double x2 = 17;
    double x3 = 20;
    double x4 = 25;
    double x5 = 45;
    double x7 = 76.3;
    double x10 = 62.0;
    double x6 = 61.8;
    double x8 = 89.7;
    double x9 = 84;

    double Y1 = 9.5;
    double y2 = 26;
    double y3 = 31;
    double y4 = 51;
    double y5 = 56;
    double y6 = 73;

    if(((p->robot[robot].rotation >= 85 && p->robot[robot].rotation <= 95)||(p->robot[robot].rotation>= - 95 && p->robot[robot].rotation <= -85))
            &&((p->robot[robot].pos.x >= FLEFT + 8)&&(p->robot[robot].pos.x <= FRIGHT - 8))
            &&(p->robot[robot].pos.y <= FBOT + 4 || p->robot[robot].pos.y >= FTOP - 4))
    {
        //上下
        PositionAndThrough(env, robot, p->curball);
    }
    else if(((p->robot[robot].rotation >= - 5 && p->robot[robot].rotation <= 5)||(p->robot[robot].rotation >= 175) ||(p->robot[robot].rotation <= -175))
            &&((p->robot[robot].pos.y >= FBOT + 8 && p->robot[robot].pos.y <= y2)||(p->robot[robot].pos.y <= FTOP - 8 || p->robot[robot].pos.y >= y5))
            &&(p->robot[robot].pos.x <= FLEFT + 4 || p->robot[robot].pos.x >= FRIGHT - 4))
    {
        //左右除开门区
        PositionAndThrough(env, robot, p->curball);
    }
    else if(((p->robot[robot].pos.x + p->robot[robot].pos.y) < FLEFT + 8)
            &&((p->robot[robot].rotation >= 40 && p->robot[robot].rotation <= 50)||(p->robot[robot].rotation >= -140 && p->robot[robot].rotation <= -130)))
    {
        //左下角
        PositionAndThrough(env, robot, p->curball);
    }
//	else if(())
//	{//右下角

//	}
//	else if()
//	{//左上角

//	}
//	else if()
//	{//右下角

//	}
    else
    {
        Kick(env, robot, Topos);
    }

}


//
/*PAngle 让robot朝angle的方向跑，并且speed控制它的最大轮速

*/
//
void PAngle( Environment *env, int robot,double angle,double speed)
{
    Mydata * p;
    p=(Mydata *)env->userData;

    double accuracy=1;
    double turnangle=0,nextangle=0;
    turnangle = angle - p->robot[robot].rotation;
    ReAngle(turnangle);
    double v=p->robot[robot].rotation - p->myoldpos[robot].z ;
    ReAngle(v);
    double v1=v;
    double FF=125;		//最大减速度
    double f=0;	//相当于减速时,右轮速度，
//	int n=0;
    bool turnleft=true;			//判断小车是否是该向左转
    double a=ANGLE_A;
    double b=ANGLE_B;

    bool face ;
    if(  turnangle < 90 && turnangle > -90 )	//检查是否正面跑位
    {
        face = true;
        speed = speed;
    }
    else
    {
        face = false;
        speed = -speed;
    }



    if(turnangle > 90)
    {
        turnleft=false;
        turnangle-=180;
    }
    else if(turnangle >0)
    {
        turnleft=true;
    }
    else if(turnangle > -90)
    {
        turnleft=false;
    }
    else
    {
        turnleft=true;
        turnangle+=180;
    }




    if(turnleft)
    {
        //
        f=-FF;

        v1=AngleOne(v1,speed+f,speed-f);		//v1+=a *( -b *f-v1);
        nextangle+=v1;

        do //whether to reduce
        {
            //收敛!!
            v1 =AngleOne(v1,speed-f,speed+f);//+= a *( b *f-v1);		// v1
            nextangle+=v1;
        }
        while( v1 > 0  );

        nextangle-=v1;

        if(nextangle < turnangle)
        {
            //不满足减速条件 所以 f 取相反数
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce

            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 < 0 )
            {
                do //该降低功率了
                {
                    f++;
                    v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
                }
                while( v1 < turnangle && f <125);
            }
            Velocity(env,robot,speed-f,speed+f);
        }
    }
    else
    {
        //
        f=FF;
        v1=AngleOne(v1,speed+f,speed-f);		//v1+=a *( -b *f-v1);
        nextangle+=v1;

        do //whether to reduce
        {
            v1 = AngleOne(v1,speed-f,speed+f);//+= a *( b *f-v1);		// v1
            nextangle+=v1;
        }
        while( v1 < 0 );

        nextangle-=v1;

        if(nextangle > turnangle)
        {
            //不满足减速条件 所以 f 取相反数
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce
            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 > 0 )
            {
                do //该降低功率了
                {
                    f--;
                    v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
                }
                while( v1 > turnangle && f >-125);
            }
            Velocity(env,robot,speed-f,speed+f);
        }
    }
}

//
/*Angle 让robot转到angle的方向

*/
//

void Angle( Environment *env, int robot,double angle)
{

    Mydata * p;
    p=(Mydata *)env->userData;

    double speed = 0;		//和pangle接轨
    double accuracy=1;
    double turnangle=0,nextangle=0;
    double FF=125;		//最大减速度

    turnangle = angle -p->robot[robot].rotation;
    ReAngle(turnangle);
    if(turnangle < 1 && turnangle >-1)
    {
        Velocity(env,robot,0,0);
        return ;
    }
    else if(turnangle < 2 && turnangle >-2)
        FF=10;
    else if( turnangle >-3 && turnangle < 3)
        FF=15;
    else if( turnangle >-5 && turnangle < 5)
        FF=30;

    double v=p->robot[robot].rotation - p->myoldpos[robot].z ;
    ReAngle(v);
    double v1=v;
    double f=0;	//相当于减速时,右轮速度，
//	int n=0;
    bool turnleft=true;			//判断小车是否是该向左转
    double a=ANGLE_A;
    double b=ANGLE_B;

    if(turnangle > 90)
    {
        turnleft=false;
        turnangle-=180;
    }
    else if(turnangle >0)
    {
        turnleft=true;
    }
    else if(turnangle > -90)
    {
        turnleft=false;
    }
    else
    {
        turnleft=true;
        turnangle+=180;
    }

    if(turnleft)
    {
        //
        f=-FF;

        v1=AngleOne(v1,speed+f,speed-f);		//v1+=a *( -b *f-v1);
        nextangle+=v1;

        do //whether to reduce
        {
            //收敛!!
            v1 =AngleOne(v1,speed-f,speed+f);//+= a *( b *f-v1);		// v1
            nextangle+=v1;
        }
        while( v1 > 0  );

        nextangle-=v1;

        if(nextangle < turnangle)
        {
            //不满足减速条件 所以 f 取相反数
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce

            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 < 0 )
            {
                do //该降低功率了
                {
                    f++;
                    v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
                }
                while( v1 < turnangle && f <FF);
            }
            Velocity(env,robot,speed-f,speed+f);
        }
    }
    else
    {
        //
        f=FF;
        v1=AngleOne(v1,speed+f,speed-f);		//v1+=a *( -b *f-v1);
        nextangle+=v1;

        do //whether to reduce
        {
            v1 =AngleOne(v1,speed-f,speed+f);//+= a *( b *f-v1);		// v1
            nextangle+=v1;
        }
        while( v1 < 0 );

        nextangle-=v1;

        if(nextangle > turnangle)
        {
            //不满足减速条件 所以 f 取相反数
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce
            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 > 0 )
            {
                do //该降低功率了
                {
                    f--;
                    v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
                }
                while( v1 > turnangle && f >-FF);
            }
            Velocity(env,robot,speed-f,speed+f);
        }
    }

}

//
/*Angle 让robot转到正对pos的方向

*/
//
void Angle( Environment *env, int robot,Vector3D pos)
{

    Mydata * p;
    p=(Mydata *)env->userData;

    double speed = 0;		//和pangle接轨
    double accuracy=1;
    double turnangle=0,nextangle=0;
    double FF=125;		//最大减速度
    double angle=0;
    angle = Atan(p->robot[robot].pos , pos);

    turnangle = angle -p->robot[robot].rotation;
    ReAngle(turnangle);
    if(turnangle < 1 && turnangle >-1)
    {
        Velocity(env,robot,0,0);
        return ;
    }
    else if(turnangle < 2 && turnangle >-2)
        FF=10;
    else if( turnangle >-3 && turnangle < 3)
        FF=15;
    else if( turnangle >-5 && turnangle < 5)
        FF=30;

    double v=p->robot[robot].rotation - p->myoldpos[robot].z ;
    ReAngle(v);
    double v1=v;
    double f=0;	//相当于减速时,右轮速度，
//	int n=0;
    bool turnleft=true;			//判断小车是否是该向左转
    double a=ANGLE_A;
    double b=ANGLE_B;

    if(turnangle > 90)
    {
        turnleft=false;
        turnangle-=180;
    }
    else if(turnangle >0)
    {
        turnleft=true;
    }
    else if(turnangle > -90)
    {
        turnleft=false;
    }
    else
    {
        turnleft=true;
        turnangle+=180;
    }

    if(turnleft)
    {
        //
        f=-FF;

        v1=AngleOne(v1,speed+f,speed-f);		//v1+=a *( -b *f-v1);
        nextangle+=v1;

        do //whether to reduce
        {
            //收敛!!
            v1 =AngleOne(v1,speed-f,speed+f);//+= a *( b *f-v1);		// v1
            nextangle+=v1;
        }
        while( v1 > 0  );

        nextangle-=v1;

        if(nextangle < turnangle)
        {
            //不满足减速条件 所以 f 取相反数
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce

            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 < 0 )
            {
                do //该降低功率了
                {
                    f++;
                    v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
                }
                while( v1 < turnangle && f <FF);
            }
            Velocity(env,robot,speed-f,speed+f);
        }
    }
    else
    {
        //
        f=FF;
        v1=AngleOne(v1,speed+f,speed-f);		//v1+=a *( -b *f-v1);
        nextangle+=v1;

        do //whether to reduce
        {
            v1 =AngleOne(v1,speed-f,speed+f);//+= a *( b *f-v1);		// v1
            nextangle+=v1;
        }
        while( v1 < 0 );

        nextangle-=v1;

        if(nextangle > turnangle)
        {
            //不满足减速条件 所以 f 取相反数
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce
            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 > 0 )
            {
                do //该降低功率了
                {
                    f--;
                    v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
                }
                while( v1 > turnangle && f >-FF);
            }
            Velocity(env,robot,speed-f,speed+f);
        }
    }

}

//
/*GoaliePosition 让robot 跑到pos，并且停下来，
bestangle 是停下来之后的朝向，
limit	控制停在pos附近的距离
	注意这个函数是针对守门员写的，
*/
//
void GoaliePosition(Environment *env,int  robot,Vector3D pos ,double bestangle,double limit)
{
    //考虑到可能的	急停和 急快速加速
    //特别作了优化
    //还有就是 被碰转后的转角过程 不能耽搁时间!!!
    //转角是最危险的过程

    Mydata * p;
    p=(Mydata *)env->userData;

    double anglespeedmax=0;		//控制转交速度的变量
    double vmax=125;			//默认的跑位加速度
    double Limitedangle=2;		//默认减速范围

    if( limit < 0.5 )
        limit =0.5;
    double Limiteddis=limit;	//减速范围有一个下限，保证不会来回跑动

    double  distance;			//robot和目标点的距离
    double turnangle,posangle,vangle;	//转动角度 ，目标点相对robot的角度，速度的绝对角度
    double dx,dy;				//pos  和robot的坐标差
    double a=SPEED_A;			//参数
    double b=SPEED_B;
    double v,v1;				//临时保存速度的大小!!!
    double f=vmax;				//加速度变量
    double s=0;					//预测的减速位移(路程)
    int n=0;					//跑位的步数
    bool face=true;			//判断小车是否是正面前进

    v= sqrt(p->myspeed[robot].x * p->myspeed[robot].x + p->myspeed[robot].y*p->myspeed[robot].y);
    //临时保存速度的大小!!!
    dx = pos.x - p->robot[robot].pos.x ;		//pos  和robot的坐标差
    dy = pos.y - p->robot[robot].pos.y ;

    distance = Distance(p->robot[robot].pos , pos);
    posangle = Atan(dy,dx);

    turnangle = p->robot[robot].rotation - posangle;		//转动角度
    ReAngle(turnangle);

    if(turnangle > 90)
    {
        //判断小车是否是正面前进
        face=false;
        turnangle-=180;
    }
    else if(turnangle < -90)
    {
        face=false;
        turnangle+=180;
    }
    else
    {
        face=true;
    }

    vangle = p->myspeed[robot].z - p->robot[robot].rotation;		//速度的方向和robot正面的夹角
    ReAngle(vangle);					//主要用于最后控制减速度的大小
    if( vangle <-90 || vangle > 90 )		//同时判断v的正负
        v=-v;

    if(face)
    {
        //forward	跑位，如果后退的话  就v=0
        //设vl,vr=0 还是vl,vr=125 有一个条件有一个临界条件那就是
        //v = SPEED_ZERO
        if(v < -SPEED_ZERO)
        {
            Velocity(env,robot,0,0);
            return ;
        }
    }
    else if(v > SPEED_ZERO)
    {
        //back		跑位，如果后退的话  就v=0
        Velocity(env,robot,0,0);
        return ;
    }

    v1=v;	//v1 is changing while program running
    //whlie, v is not

    if(distance > Limiteddis )
    {
        //it is too early to count the steps
        //but the Limiteddis should be tested!!	to do...
        if(turnangle > Limitedangle || turnangle < -Limitedangle)
        {
            //adjust angle
            /////////////////测试这一段
            //对于goalie这一段应该特别注意
            //发生变向	1.knock the robot,especially the opponent
            //	2.knock the wall
            // so the anglespeedmax is allowed ++ more!!
            if(turnangle > 50  || turnangle < -50)
                anglespeedmax = 0;
            else if(turnangle > 30  || turnangle < -30)
                anglespeedmax = 80;
            else if(turnangle > 10  || turnangle < -10)
                anglespeedmax = 125;
            else if(turnangle > 5  || turnangle < -5)
                anglespeedmax = 180;
            else
                anglespeedmax = 200;
            ///////////////测试这一段

            PAngle(env,robot,posangle,anglespeedmax);
        }
        else
        {
            if(face)
                Velocity(env,robot,f,f);
            else
                Velocity(env,robot,-f,-f);
        }//it is time to rush
    }
    else
    {
        if(distance > 1)
        {
            //调整角度	return!!!!!!
            //radious of robot is about 1.5 ,so the distance is very short
            if(turnangle > Limitedangle || turnangle < -Limitedangle)
            {
                Angle(env,robot,posangle);
                return ;
            }
        }

        if(distance < 0.4)
        {
            //停止并转向		return!!!!!!
            //radious of robot is about 1.5 ,so the distance is very short

            if( v<0.1 && v>-0.1)
            {
                //the range of v shoud be tested
                Angle(env,robot,bestangle);
                return	;
            }
        }

        if(true)
        {
            vmax=125;
            if(face)
            {
                f=-vmax;		//减速度  为  0000000
                v1=VelocityOne(v1,-f,-f);		//加速一步
                s=v1;
                do //whether to reduce
                {
                    if(v1 > SPEED_ZERO)	//as i said,this is limited
                        v1=VelocityOne(v1,0,0);
                    else
                        v1=VelocityOne(v1,f,f);
                    s+=v1;
                }
                while( v1 > 0 );

                s-=v1;

                if(s < distance)
                {
                    //不满足减速条件加速
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v > SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//减速一步
                        if( v1 < 0 )
                        {
                            do //该降低功率了
                            {
                                f++;		//f=-vmax;
                                v1 = VelocityOne(v,f,f);
                            }
                            while( v1 < distance && f < vmax);
                        }
                        Velocity(env,robot,f,f);
                    }
                }
            }
            else
            {
                f=vmax;		//减速度!!!!!
                v1=VelocityOne(v1,-f,-f);
                s=v1;
                do //whether to reduce
                {
                    if(v1 < -SPEED_ZERO)	//as i said,this is limited
                        v1=VelocityOne(v1,0,0);
                    else
                        v1=VelocityOne(v1,f,f);
                    s+=v1;
                }
                while( v1 < -0.1 );

                s-=v1;

                if(s > -distance)
                {
                    //不满足减速条件加速
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v < -SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//减速一步
                        if( v1 > 0 )
                        {
                            do //该降低功率了
                            {
                                f--;		//f=-vmax;
                                v1 = VelocityOne(v,f,f);
                            }
                            while( v1 > -distance && f > -vmax);
                        }
                        Velocity(env,robot,f,f);
                    }
                }
            }
        }
    }
}
