#include <string.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>


#include "basic.h"
#include "tools.h"

//
/* ReAngle �淶angle�Ĵ�С�ڣ�-180��+180��֮��
*/
//
void ReAngle(double &angle)
{
    while(angle >= 180.0 )angle-=360.0;
    while(angle < -180.0 )angle+=360.0;
}
//
/*Atan ��y/x����ֵ��Ӧ�ĽǶȣ�-180��+180��֮��
*/
//

double Atan(double y,double x)
{
    if(x != 0.0 || y != 0.0)
        return 180*atan2(y,x)/PI;
    else return 0.0;
}

//
/*Atan (end-begin)ʸ���ĽǶȣ���-180��+180��֮��
*/
//
double Atan(Vector3D begin,Vector3D end)
{
    double y,x;
    y=end.y - begin.y ;
    x=end.x - begin.x ;
    return Atan(y,x);
}

//
/*AngleOne �����ڵ�ǰ���ٶ�omiga�Ļ���������������vl,vr���ƣ���һ�����ڴﵽ�Ľ��ٶ�
����  ��һ�����ڴﵽ�Ľ��ٶ�
*/
//
double AngleOne(double omiga,double vl,double vr)
{
//		omiga = p->robot[i].rotation - p->myoldpos[i].z ;
//		ReAngle(omiga);
    if(vl>125)vl=125;
    if(vl<-125)vl=-125;
    if(vr>125)vr=125;
    if(vr<-125)vr=-125;
    double angle = (vr - vl)/2;

    ReAngle(omiga);		//Ӧ��û�д���180 �Ľ��ٶȰ�

    omiga += ANGLE_A * (ANGLE_B * angle-omiga);

    if( vr > vl )
    {
        if( vl >= 0 || vr <=0 )
        {
            omiga -= 4 * ANGLE_K * angle * angle;
        }
    }
    else if( vr < vl )
    {
        if( vr >= 0 || vl <=0 )
        {
            omiga += 4 * ANGLE_K * angle * angle;
        }
    }

    ReAngle(omiga);		//Ӧ��û�д���180 �Ľ��ٶȰ�

    return omiga;

}

//
/*Distance  ����֮�����
*/
//
double Distance (Vector3D pos1,Vector3D pos2)
{
    return sqrt( (pos1.x-pos2.x)*(pos1.x-pos2.x) + (pos1.y-pos2.y)*(pos1.y-pos2.y) );
}

//
/*VelocityOne �����ڵ�ǰ�ٶ�speed�Ļ���������������vl,vr���ƣ���һ�����ڴﵽ���ٶ�
����  ��һ�����ڴﵽ���ٶ�
*/
//
double VelocityOne(double speed,double vl,double vr)
{
    if(vl>125)vl=125;
    if(vl<-125)vl=-125;
    if(vr>125)vr=125;
    if(vr<-125)vr=-125;

    if(speed > 3 || speed < -3)
        speed =0;

    if( vl==0 && vr==0 )
        speed += -SPEED_ODD * speed;
    else
        speed += SPEED_A*( SPEED_B*(vl +vr)/2-speed);
    return speed;
}
//
/*Count ����robot�ܵ�posλ�õ�������

*/
//


////////////
/* PredictBall	Ԥ�⾭�� steps ������֮�����λ��
	�������ؽ�������� steps ������֮�����λ��

	����ײǽ�Ĺ���

*/
////////////
void PredictBall2(Environment *env)
{
    Mydata * p;
    p=(Mydata *)env->userData;

    Vector3D predictball;
    Vector3D ballspeed;
    int i=0;
//	p->preball.x = p->curball.x + steps*(p->curball.x - p->oldball.x);
//	p->preball.y = p->curball.y + steps*(p->curball.y - p->oldball.y);
    ///z ����������ķ���
//	p->preball.z = Atan( (p->curball.y - p->oldball.y) , (p->curball.x - p->oldball.x) );

    predictball.x = p->curball.x;			//����ֵ
    predictball.y = p->curball.y;
//	predictball.z = p->curball.z;

    ballspeed.x = p->ballspeed.x ;
    ballspeed.y = p->ballspeed.y ;
    ballspeed.z = p->ballspeed.z ;

    for(i=0; i<30; i++)
    {
        predictball.x += ballspeed.x ;
        predictball.y += ballspeed.y ;
//����ײǽ
        if( predictball.x > FRIGHT )
        {
            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            ballspeed.x *=-SPEED_NORMAL;	//loose
            ballspeed.y *= SPEED_TANGENT;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;

        }
        else if( predictball.x < FLEFT )
        {
            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            ballspeed.x *=-SPEED_NORMAL;	//loose
            ballspeed.y *= SPEED_TANGENT;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;

        }
        else if( predictball.y < FBOT )
        {
            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            ballspeed.x *= SPEED_TANGENT;	//loose
            ballspeed.y *=-SPEED_NORMAL;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;

        }
        else if( predictball.y > FTOP )
        {
            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            ballspeed.x *= SPEED_TANGENT;	//loose
            ballspeed.y *=-SPEED_NORMAL;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
        }

        if( predictball.x + predictball.y > FRIGHT +FTOP -CORNER)
        {
            //����
            double vx,vy;
            vy=0.7071*ballspeed.y + 0.7071*ballspeed.x;	//�任1
            vx=-0.7071*ballspeed.y + 0.7071*ballspeed.x;

            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            vx *= SPEED_TANGENT;	//loose
            vy *=-SPEED_NORMAL;

            ballspeed.y = 0.7071 * vy - 0.7071 * vx;	//�任2
            ballspeed.x = 0.7071 * vy + 0.7071 * vx;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
            /*
            y1 = y * cos() - x * sin();
            x1 = y * sin() + x * sin();
            */
        }
        else if( predictball.x + predictball.y < FLEFT +FBOT+CORNER)
        {
            //����
            double vx,vy;
            vy=0.7071*ballspeed.y + 0.7071*ballspeed.x;	//�任1
            vx=-0.7071*ballspeed.y + 0.7071*ballspeed.x;

            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            vx *= SPEED_TANGENT;	//loose
            vy *=-SPEED_NORMAL;

            ballspeed.y = 0.7071 * vy - 0.7071 * vx;	//�任2
            ballspeed.x = 0.7071 * vy + 0.7071 * vx;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
        }
        else if( predictball.x - predictball.y > FRIGHT -FBOT -CORNER)
        {
            //����
            double vx,vy;
            vy=0.7071*ballspeed.y - 0.7071*ballspeed.x;	//�任1
            vx=0.7071*ballspeed.y + 0.7071*ballspeed.x;

            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            vx *= SPEED_TANGENT;	//loose
            vy *=-SPEED_NORMAL;

            ballspeed.y = 0.7071 * vy + 0.7071 * vx;	//�任2
            ballspeed.x = -0.7071 * vy + 0.7071 * vx;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
        }
        else if( predictball.y - predictball.x > FTOP - FLEFT-CORNER)
        {
            //����
            double vx,vy;
            vy=0.7071*ballspeed.y - 0.7071*ballspeed.x;	//�任1
            vx=0.7071*ballspeed.y + 0.7071*ballspeed.x;

            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            vx *= SPEED_TANGENT;	//loose
            vy *=-SPEED_NORMAL;

            ballspeed.y = 0.7071 * vy + 0.7071 * vx;	//�任2
            ballspeed.x = -0.7071 * vy + 0.7071 * vx;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
        }

        p->ba[i].x=predictball.x ;
        p->ba[i].y=predictball.y ;
        p->ba[i].z=Atan( ballspeed.y ,ballspeed.x );
//�����Ľ�
    }


}





void PredictBall(Environment *env,int steps)
{
    Mydata * p;
    p=(Mydata *)env->userData;

    Vector3D predictball;
    Vector3D ballspeed;
    int i=0;
//	p->preball.x = p->curball.x + steps*(p->curball.x - p->oldball.x);
//	p->preball.y = p->curball.y + steps*(p->curball.y - p->oldball.y);
    ///z ����������ķ���
//	p->preball.z = Atan( (p->curball.y - p->oldball.y) , (p->curball.x - p->oldball.x) );

    predictball.x = p->curball.x;			//����ֵ
    predictball.y = p->curball.y;
//	predictball.z = p->curball.z;

    ballspeed.x = p->ballspeed.x ;
    ballspeed.y = p->ballspeed.y ;
    ballspeed.z = p->ballspeed.z ;

    for(i=0; i<steps; i++)
    {
        predictball.x += ballspeed.x ;
        predictball.y += ballspeed.y ;
//����ײǽ
        if( predictball.x > FRIGHT-0.7)
        {
            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            ballspeed.x *=-SPEED_NORMAL;	//loose
            ballspeed.y *= SPEED_TANGENT;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;

        }
        else if( predictball.x < FLEFT+0.7 )
        {
            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            ballspeed.x *=-SPEED_NORMAL;	//loose
            ballspeed.y *= SPEED_TANGENT;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;

        }
        else if( predictball.y < FBOT+0.7 )
        {
            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            ballspeed.x *= SPEED_TANGENT;	//loose
            ballspeed.y *=-SPEED_NORMAL;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;

        }
        else if( predictball.y > FTOP -0.7)
        {
            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            ballspeed.x *= SPEED_TANGENT;	//loose
            ballspeed.y *=-SPEED_NORMAL;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
        }

        if( predictball.x + predictball.y > FRIGHT +FTOP -CORNER)
        {
            //����
            double vx,vy;
            vy=0.7071*ballspeed.y + 0.7071*ballspeed.x;	//�任1
            vx=-0.7071*ballspeed.y + 0.7071*ballspeed.x;

            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            vx *= SPEED_TANGENT;	//loose
            vy *=-SPEED_NORMAL;

            ballspeed.y = 0.7071 * vy - 0.7071 * vx;	//�任2
            ballspeed.x = 0.7071 * vy + 0.7071 * vx;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
            /*
            y1 = y * cos() - x * sin();
            x1 = y * sin() + x * sin();
            */
        }
        else if( predictball.x + predictball.y < FLEFT +FBOT+CORNER)
        {
            //����
            double vx,vy;
            vy=0.7071*ballspeed.y + 0.7071*ballspeed.x;	//�任1
            vx=-0.7071*ballspeed.y + 0.7071*ballspeed.x;

            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            vx *= SPEED_TANGENT;	//loose
            vy *=-SPEED_NORMAL;

            ballspeed.y = 0.7071 * vy - 0.7071 * vx;	//�任2
            ballspeed.x = 0.7071 * vy + 0.7071 * vx;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
        }
        else if( predictball.x - predictball.y > FRIGHT -FBOT -CORNER)
        {
            //����
            double vx,vy;
            vy=0.7071*ballspeed.y - 0.7071*ballspeed.x;	//�任1
            vx=0.7071*ballspeed.y + 0.7071*ballspeed.x;

            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            vx *= SPEED_TANGENT;	//loose
            vy *=-SPEED_NORMAL;

            ballspeed.y = 0.7071 * vy + 0.7071 * vx;	//�任2
            ballspeed.x = -0.7071 * vy + 0.7071 * vx;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
        }
        else if( predictball.y - predictball.x > FTOP - FLEFT-CORNER)
        {
            //����
            double vx,vy;
            vy=0.7071*ballspeed.y - 0.7071*ballspeed.x;	//�任1
            vx=0.7071*ballspeed.y + 0.7071*ballspeed.x;

            predictball.x -= ballspeed.x ;	//retern
            predictball.y -= ballspeed.y ;

            vx *= SPEED_TANGENT;	//loose
            vy *=-SPEED_NORMAL;

            ballspeed.y = 0.7071 * vy + 0.7071 * vx;	//�任2
            ballspeed.x = -0.7071 * vy + 0.7071 * vx;

            predictball.x += ballspeed.x ;	//go on
            predictball.y += ballspeed.y ;
        }
//�����Ľ�
    }
    p->preball.x = predictball.x ;
    p->preball.y = predictball.y ;
    p->preball.z = Atan( ballspeed.y ,ballspeed.x );

}





double	 Sin(double x)
{
    x=PI*x/180;
    return sin(x);
}
double	 Cos(double x)
{
    x=PI*x/180;
    return cos(x);
}
double	 Acos(double x)
{
    return(acos(x)*180/PI);
}
double	 Asin(double x)
{
    return(asin(x)*180/PI);
}
bool	 top(Vector3D point,double d)
{
    if(point.y>FTOP-d)return true;
    else return false;
}
bool	 bot(Vector3D point,double d)
{
    if(point.y<FBOT+d)return true;
    else return false;
}
bool	 left(Vector3D point,double d)
{
    if(point.x<FLEFT+d)return true;
    else return false;
}
bool	 right(Vector3D point,double d)
{
    if(point.x>FRIGHT-d)return true;
    else return false;
}

void	 bline(Vector3D p1,Vector3D p2,double&A,double &B,double &C)//��ֱ��
{
    A=-(p2.y-p1.y)/(p2.x-p1.x);
    B=1;
    C=-(A*p1.x+p1.y);
    if(A<0)
    {
        A=-A;
        B=-B;
        C=-C;
    }

}
void	 bline(Vector3D p1,double angle,double&A,double &B,double &C)
{
    A=-tan(angle*PI/180);
    C=-(A*p1.x+p1.y);
    B=1;
    if(A<0)
    {
        A=-A;
        B=-B;
        C=-C;
    }
}




void	 Chkarea(Environment *env,int *no,int &m,int ctrl)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    //ctrl�������õ���λabc���������Ϣ��Hλ��ʾcheck�Ļ����ˣ�0�ҷ���1�Է�
    // 									,Mλ��ʾ		���أ�0�ҷ���1�Է�
    //									,Lλ��ʾ��С������0С������1�����,�ܹ�8���������ֵ�ϴ�0�䵽7
    int i;
    m=0;
    switch(ctrl)
    {
    case 0:
        for(i=0; i<5; i++)
        {
            if(p->robot[i].pos.x<11.9571)
            {
                if(p->robot[i].pos.y>33.7197&&p->robot[i].pos.y<52.9284)
                {
                    no[m++]=i;
                }
            }
        }
        break;
    case 1:
        for(i=0; i<5; i++)
        {
            if(p->robot[i].pos.x<19.8003)
            {
                if(p->robot[i].pos.y>27.8966&&p->robot[i].pos.y<58.566)
                {
                    no[m++]=i;
                }
            }
        }
        break;
    case 2:
        for(i=0; i<5; i++)
        {
            if(p->robot[i].pos.x>88.2806)
            {
                if(p->robot[i].pos.y>33.7197&&p->robot[i].pos.y<52.9284)
                {
                    no[m++]=i;
                }
            }
        }
        break;
    case 3:
        for(i=0; i<5; i++)
        {
            if(p->robot[i].pos.x>80.4374)
            {
                if(p->robot[i].pos.y>27.8966&&p->robot[i].pos.y<58.566)
                {
                    no[m++]=i;
                }
            }

        }
        break;
    case 4:
        for(i=0; i<5; i++)
        {
            if(p->opp[i].pos.x<11.9571)
            {
                if(p->opp[i].pos.y>33.7197&&p->opp[i].pos.y<52.9284)
                {
                    no[m++]=i;
                }
            }

        }
        break;
    case 5:
        for(i=0; i<5; i++)
        {
            if(p->opp[i].pos.x<19.8003)
            {
                if(p->opp[i].pos.y>27.8966&&p->opp[i].pos.y<58.566)
                {
                    no[m++]=i;
                }
            }

        }
        break;
    case 6:
        for(i=0; i<5; i++)
        {
            if(p->opp[i].pos.x>88.2806)
            {
                if(p->opp[i].pos.y>33.7197&&p->opp[i].pos.y<52.9284)
                {
                    no[m++]=i;
                }
            }

        }
        break;
    case 7:
        for(i=0; i<5; i++)
        {
            if(p->opp[i].pos.x>80.4374)
            {
                if(p->opp[i].pos.y>27.8966&&p->opp[i].pos.y<58.566)
                {
                    no[m++]=i;
                }
            }
        }
        break;
    default:
        break;

    }
}


bool	 isbetween(double angle ,double angle1,double angle2)//�ж�angle�Ƿ���angle1 angle2֮��
{
    double a=angle1-angle2;
    double b=angle-angle1;
    double c=angle-angle2;
    ReAngle(a);
    ReAngle(b);
    ReAngle(c);
    if(fabs(b)<fabs(a)&&fabs(c)<fabs(a))return true;
    else return false;

}



bool	 iscontinue(int*a,int n,int i)//�ж��������Ƿ���Ԫ��i
{
    int j;
    int m=0;
    for(j=0; j<n; j++)
    {
        if(a[j]==i)
        {
            m++;
            break;
        }
    }
    if(m)return(true);
    else return(false);

}


Vector3D projection(Vector3D pos,Vector3D go,double angle)
{
    double A,B,C;
    double a,b,c;

    bline(go,angle,A,B,C);
    bline(pos,angle+90,a,b,c);
    go.x=(b*C-B*c)/(B*a-b*A);
    go.y=(a*C-A*c)/(A*b-a*B);

    return(go);
}


Vector3D transfpos(Vector3D pos,double angle,double dis)
{
    Vector3D go;
    go.x=pos.x+dis*Cos(angle);
    go.y=pos.y+dis*Sin(angle);
    go.z=pos.z;
    return(go);
}
int		 timeneed(Environment*env,int i,Vector3D go)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    double an;
    double s;
    double dis=0;
    int t=0;
    double vo;
    double to;


    an=Atan(p->robot[i].pos,go);
    s=Distance(p->robot[i].pos,go);
    t=int(s/1.9);

    vo=p->myspeed[i].x*Cos(an)+p->myspeed[i].y*Cos(an-90);
    to=	log(1-vo/1.88)/-0.066;

    while(1)
    {
        if(t>=35)
            break;
        if(s-dis<3)break;
        dis=1.88*t+1.88 /0.066*(exp(-0.066*(to+t))-exp(-0.066*to));
        t++;
    }
    return(t);

}

