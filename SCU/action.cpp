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

    //vl,vr����ȡֵ��Χ��!!!
    if(vl>125)vl=125;
    if(vl<-125)vl=-125;
    if(vr>125)vr=125;
    if(vr<-125)vr=-125;

    if(true)
    {
        //�ٶȵ��ر����
        if(vl==0 && vr!=0)
            vl=0.00001;
        if(vr==0 && vl!=0)
            vr=0.00001;
    }

    p->robot[robot].velocityLeft = vl;
    p->robot[robot].velocityRight= vr;

}

//
/*PositionAndStop ��robot �ܵ�pos������ͣ������
bestangle ��ͣ����֮��ĳ���
limit	����ͣ��pos�����ľ���

*/
//
void PositionAndStop(Environment *env,int  robot,Vector3D pos ,double bestangle,double limit)
{
    //���ǵ����ܵ�	��ͣ�� �����ټ���
    //�ر������Ż�
    //���о��� ����ת���ת�ǹ��� ���ܵ���ʱ��!!!
    //ת������Σ�յĹ���

    Mydata * p;
    p=(Mydata *)env->userData;

    double anglespeedmax=0;		//����ת���ٶȵı���
    double vmax=125;			//Ĭ�ϵ���λ���ٶ�
    double Limitedangle=2;		//Ĭ�ϼ��ٷ�Χ

    if( limit < 0.5 )
        limit =0.5;
    double Limiteddis=limit;	//���ٷ�Χ��һ�����ޣ���֤���������ܶ�

    double  distance;			//robot��Ŀ���ľ���
    double turnangle,posangle,vangle;	//ת���Ƕ� ��Ŀ������robot�ĽǶȣ��ٶȵľ��ԽǶ�
    double dx,dy;				//pos  ��robot�������
    double a=SPEED_A;			//����
    double b=SPEED_B;
    double v,v1;				//��ʱ�����ٶȵĴ�С!!!
    double f=vmax;				//���ٶȱ���
    double s=0;					//Ԥ��ļ���λ��(·��)
    int n=0;					//��λ�Ĳ���
    bool face=true;			//�ж�С���Ƿ�������ǰ��

    v= sqrt(p->myspeed[robot].x * p->myspeed[robot].x + p->myspeed[robot].y*p->myspeed[robot].y);
    //��ʱ�����ٶȵĴ�С!!!
    dx = pos.x - p->robot[robot].pos.x ;		//pos  ��robot�������
    dy = pos.y - p->robot[robot].pos.y ;

    distance = Distance(p->robot[robot].pos , pos);
    posangle = Atan(dy,dx);

    turnangle = p->robot[robot].rotation - posangle;		//ת���Ƕ�
    ReAngle(turnangle);

    if(turnangle > 90)
    {
        //�ж�С���Ƿ�������ǰ��
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

    vangle = p->myspeed[robot].z - p->robot[robot].rotation;		//�ٶȵķ����robot����ļн�
    ReAngle(vangle);					//��Ҫ���������Ƽ��ٶȵĴ�С
    if( vangle <-90 || vangle > 90 )		//ͬʱ�ж�v������
        v=-v;

    if(face)
    {
        //forward	��λ��������˵Ļ�  ��v=0
        //��vl,vr=0 ����vl,vr=125 ��һ��������һ���ٽ������Ǿ���
        //v = SPEED_ZERO
        if(v < -SPEED_ZERO)
        {
            Velocity(env,robot,0,0);
            return ;
        }
    }
    else if(v > SPEED_ZERO)
    {
        //back		��λ��������˵Ļ�  ��v=0
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
            /////////////////������һ��
            //����goalie��һ��Ӧ���ر�ע��
            //��������	1.knock the robot,especially the opponent
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
            ///////////////������һ��
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
            //�����Ƕ�	return!!!!!!
            //radious of robot is about 1.5 ,so the distance is very short
            if(turnangle > Limitedangle || turnangle < -Limitedangle)
            {
                Angle(env,robot,posangle);
                return ;
            }
        }

        if(distance < 0.4)
        {
            //ֹͣ��ת��		return!!!!!!
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
                f=-vmax;		//���ٶ�  Ϊ  0000000
                v1=VelocityOne(v1,-f,-f);		//����һ��
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
                    //�����������������
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v > SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//����һ��
                        if( v1 < 0 )
                        {
                            do //�ý��͹�����
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
                f=vmax;		//���ٶ�!!!!!
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
                    //�����������������
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v < -SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//����һ��
                        if( v1 > 0 )
                        {
                            do //�ý��͹�����
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
/*PositionAndStopX ��robot �ܵ�pos������ͣ����ԭ����ת��
Xangle	��ת�Ľ��ٶ�
limit	����ͣ��pos�����ľ���

*/
//
void PositionAndStopX(Environment *env,int  robot,Vector3D pos ,double Xangle,double limit)
{
    Mydata * p;
    p=(Mydata *)env->userData;

    double anglespeedmax=0;		//����ת���ٶȵı���
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
    bool face=true;			//�ж�С���Ƿ�������ǰ��

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
            /////////////////������һ��
            if(turnangle > 20  || turnangle < -20)
                anglespeedmax = 0;
            else if(turnangle > 10  || turnangle < -10)
                anglespeedmax = 125;
            else if(turnangle > 5  || turnangle < -5)
                anglespeedmax = 180;
            else
                anglespeedmax = 200;
            ///////////////������һ��
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
            //�����Ƕ�	return!!!!!!
            if(turnangle > Limitedangle || turnangle < -Limitedangle)
            {
                Angle(env,robot,posangle);
                return ;
            }

        }

        if(distance < 1)
        {
            //ֹͣ��ת��		return!!!!!!
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
                f=-vmax;		//���ٶ�  Ϊ  0000000
                v1=VelocityOne(v1,-f,-f);		//����һ��
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
                    //�����������������
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v > SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//����һ��
                        if( v1 < 0 )
                        {
                            do //�ý��͹�����
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
                f=vmax;		//���ٶ�!!!!!
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
                    //�����������������
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v < -SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//����һ��
                        if( v1 > 0 )
                        {
                            do //�ý��͹�����
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
/*PositionAndThrough ��robot�����MAX ����pos���м�û�м��ٿ���


*/
//
void PositionAndThrough(Environment *env,int robot,Vector3D pos ,double MAX)
{
    Mydata * p;
    p=(Mydata *)env->userData;

    double anglespeedmax=0;		//����ת���ٶȵı���
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
    bool face=true;			//�ж�С���Ƿ�������ǰ��

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
            /////////////////������һ��
            if(turnangle > 20  || turnangle < -20)
                anglespeedmax = 0;
            else if(turnangle > 10  || turnangle < -10)
                anglespeedmax = 125;
            else if(turnangle > 5  || turnangle < -5)
                anglespeedmax = 180;
            else
                anglespeedmax = 200;
            ///////////////������һ��
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
/* Kick	��robot�����ߵ�ToPos��λ��

*/
////////////
void Kick(Environment *env , int  robot , Vector3D ToPos )
{
    Mydata * p;
    p=(Mydata *)env->userData;

    Vector3D ball = p->curball;		//������λ�ò�����!!!

    Vector3D RobotToBall;		//�˺�������λ��
    RobotToBall.x = ball.x - p->robot[robot].pos.x ;
    RobotToBall.y = ball.y - p->robot[robot].pos.y ;
    RobotToBall.z = Atan(p->robot[robot].pos , ball);

    Vector3D BallToGate ;		//������ŵ����λ��
    BallToGate.x = ToPos.x - ball.x ;
    BallToGate.y = ToPos.y - ball.y ;
    BallToGate.z = Atan(ball , ToPos);

    double RunAngle ;
    RunAngle = RobotToBall.z - BallToGate.z;
    ReAngle(RunAngle);

    RunAngle = RobotToBall.z + RunAngle / 2 ;	// ���Ե���  2
    ReAngle(RunAngle);

    PAngle(env,robot,RunAngle,125);

}

void MyKick(Environment * env, int robot, Vector3D Topos)
{
    //�����˱�������
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
        //����
        PositionAndThrough(env, robot, p->curball);
    }
    else if(((p->robot[robot].rotation >= - 5 && p->robot[robot].rotation <= 5)||(p->robot[robot].rotation >= 175) ||(p->robot[robot].rotation <= -175))
            &&((p->robot[robot].pos.y >= FBOT + 8 && p->robot[robot].pos.y <= y2)||(p->robot[robot].pos.y <= FTOP - 8 || p->robot[robot].pos.y >= y5))
            &&(p->robot[robot].pos.x <= FLEFT + 4 || p->robot[robot].pos.x >= FRIGHT - 4))
    {
        //���ҳ�������
        PositionAndThrough(env, robot, p->curball);
    }
    else if(((p->robot[robot].pos.x + p->robot[robot].pos.y) < FLEFT + 8)
            &&((p->robot[robot].rotation >= 40 && p->robot[robot].rotation <= 50)||(p->robot[robot].rotation >= -140 && p->robot[robot].rotation <= -130)))
    {
        //���½�
        PositionAndThrough(env, robot, p->curball);
    }
//	else if(())
//	{//���½�

//	}
//	else if()
//	{//���Ͻ�

//	}
//	else if()
//	{//���½�

//	}
    else
    {
        Kick(env, robot, Topos);
    }

}


//
/*PAngle ��robot��angle�ķ����ܣ�����speed���������������

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
    double FF=125;		//�����ٶ�
    double f=0;	//�൱�ڼ���ʱ,�����ٶȣ�
//	int n=0;
    bool turnleft=true;			//�ж�С���Ƿ��Ǹ�����ת
    double a=ANGLE_A;
    double b=ANGLE_B;

    bool face ;
    if(  turnangle < 90 && turnangle > -90 )	//����Ƿ�������λ
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
            //����!!
            v1 =AngleOne(v1,speed-f,speed+f);//+= a *( b *f-v1);		// v1
            nextangle+=v1;
        }
        while( v1 > 0  );

        nextangle-=v1;

        if(nextangle < turnangle)
        {
            //������������� ���� f ȡ�෴��
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce

            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 < 0 )
            {
                do //�ý��͹�����
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
            //������������� ���� f ȡ�෴��
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce
            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 > 0 )
            {
                do //�ý��͹�����
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
/*Angle ��robotת��angle�ķ���

*/
//

void Angle( Environment *env, int robot,double angle)
{

    Mydata * p;
    p=(Mydata *)env->userData;

    double speed = 0;		//��pangle�ӹ�
    double accuracy=1;
    double turnangle=0,nextangle=0;
    double FF=125;		//�����ٶ�

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
    double f=0;	//�൱�ڼ���ʱ,�����ٶȣ�
//	int n=0;
    bool turnleft=true;			//�ж�С���Ƿ��Ǹ�����ת
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
            //����!!
            v1 =AngleOne(v1,speed-f,speed+f);//+= a *( b *f-v1);		// v1
            nextangle+=v1;
        }
        while( v1 > 0  );

        nextangle-=v1;

        if(nextangle < turnangle)
        {
            //������������� ���� f ȡ�෴��
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce

            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 < 0 )
            {
                do //�ý��͹�����
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
            //������������� ���� f ȡ�෴��
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce
            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 > 0 )
            {
                do //�ý��͹�����
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
/*Angle ��robotת������pos�ķ���

*/
//
void Angle( Environment *env, int robot,Vector3D pos)
{

    Mydata * p;
    p=(Mydata *)env->userData;

    double speed = 0;		//��pangle�ӹ�
    double accuracy=1;
    double turnangle=0,nextangle=0;
    double FF=125;		//�����ٶ�
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
    double f=0;	//�൱�ڼ���ʱ,�����ٶȣ�
//	int n=0;
    bool turnleft=true;			//�ж�С���Ƿ��Ǹ�����ת
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
            //����!!
            v1 =AngleOne(v1,speed-f,speed+f);//+= a *( b *f-v1);		// v1
            nextangle+=v1;
        }
        while( v1 > 0  );

        nextangle-=v1;

        if(nextangle < turnangle)
        {
            //������������� ���� f ȡ�෴��
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce

            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 < 0 )
            {
                do //�ý��͹�����
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
            //������������� ���� f ȡ�෴��
            Velocity(env,robot,speed+f,speed-f);
        }
        else
        {
            //reduce
            v1 = AngleOne(v,speed-f,speed+f);  //v + a *( b *f-v);
            if( v1 > 0 )
            {
                do //�ý��͹�����
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
/*GoaliePosition ��robot �ܵ�pos������ͣ������
bestangle ��ͣ����֮��ĳ���
limit	����ͣ��pos�����ľ���
	ע������������������Աд�ģ�
*/
//
void GoaliePosition(Environment *env,int  robot,Vector3D pos ,double bestangle,double limit)
{
    //���ǵ����ܵ�	��ͣ�� �����ټ���
    //�ر������Ż�
    //���о��� ����ת���ת�ǹ��� ���ܵ���ʱ��!!!
    //ת������Σ�յĹ���

    Mydata * p;
    p=(Mydata *)env->userData;

    double anglespeedmax=0;		//����ת���ٶȵı���
    double vmax=125;			//Ĭ�ϵ���λ���ٶ�
    double Limitedangle=2;		//Ĭ�ϼ��ٷ�Χ

    if( limit < 0.5 )
        limit =0.5;
    double Limiteddis=limit;	//���ٷ�Χ��һ�����ޣ���֤���������ܶ�

    double  distance;			//robot��Ŀ���ľ���
    double turnangle,posangle,vangle;	//ת���Ƕ� ��Ŀ������robot�ĽǶȣ��ٶȵľ��ԽǶ�
    double dx,dy;				//pos  ��robot�������
    double a=SPEED_A;			//����
    double b=SPEED_B;
    double v,v1;				//��ʱ�����ٶȵĴ�С!!!
    double f=vmax;				//���ٶȱ���
    double s=0;					//Ԥ��ļ���λ��(·��)
    int n=0;					//��λ�Ĳ���
    bool face=true;			//�ж�С���Ƿ�������ǰ��

    v= sqrt(p->myspeed[robot].x * p->myspeed[robot].x + p->myspeed[robot].y*p->myspeed[robot].y);
    //��ʱ�����ٶȵĴ�С!!!
    dx = pos.x - p->robot[robot].pos.x ;		//pos  ��robot�������
    dy = pos.y - p->robot[robot].pos.y ;

    distance = Distance(p->robot[robot].pos , pos);
    posangle = Atan(dy,dx);

    turnangle = p->robot[robot].rotation - posangle;		//ת���Ƕ�
    ReAngle(turnangle);

    if(turnangle > 90)
    {
        //�ж�С���Ƿ�������ǰ��
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

    vangle = p->myspeed[robot].z - p->robot[robot].rotation;		//�ٶȵķ����robot����ļн�
    ReAngle(vangle);					//��Ҫ���������Ƽ��ٶȵĴ�С
    if( vangle <-90 || vangle > 90 )		//ͬʱ�ж�v������
        v=-v;

    if(face)
    {
        //forward	��λ��������˵Ļ�  ��v=0
        //��vl,vr=0 ����vl,vr=125 ��һ��������һ���ٽ������Ǿ���
        //v = SPEED_ZERO
        if(v < -SPEED_ZERO)
        {
            Velocity(env,robot,0,0);
            return ;
        }
    }
    else if(v > SPEED_ZERO)
    {
        //back		��λ��������˵Ļ�  ��v=0
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
            /////////////////������һ��
            //����goalie��һ��Ӧ���ر�ע��
            //��������	1.knock the robot,especially the opponent
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
            ///////////////������һ��

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
            //�����Ƕ�	return!!!!!!
            //radious of robot is about 1.5 ,so the distance is very short
            if(turnangle > Limitedangle || turnangle < -Limitedangle)
            {
                Angle(env,robot,posangle);
                return ;
            }
        }

        if(distance < 0.4)
        {
            //ֹͣ��ת��		return!!!!!!
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
                f=-vmax;		//���ٶ�  Ϊ  0000000
                v1=VelocityOne(v1,-f,-f);		//����һ��
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
                    //�����������������
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v > SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//����һ��
                        if( v1 < 0 )
                        {
                            do //�ý��͹�����
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
                f=vmax;		//���ٶ�!!!!!
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
                    //�����������������
                    Velocity(env,robot,-f,-f);
                }
                else
                {
                    if(v < -SPEED_ZERO)
                        Velocity(env,robot,0,0);
                    else
                    {
                        v1=VelocityOne(v,f,f);		//����һ��
                        if( v1 > 0 )
                        {
                            do //�ý��͹�����
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
