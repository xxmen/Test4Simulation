#include <string.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#include "basic.h"
#include "tools.h"
#include "action.h"
#include "strategy.h"
#include "myfunction.h"

/*MyGoalie 让robot守门

*/
//

void MyGoalie ( Environment *env, int robot )
{
    //先校正姿态，再去拦球
    Mydata * p;
    p=(Mydata *)env->userData;

    Vector3D go;

    double x1=10;
    double x2=17;
    double x3=20;
    double x4=25;
    double x5=45;
    double x7=76.3;
    double x10=62.0;
    double x6=61.8;
    double x8=89.7;
    double x9=84;

    double y1=9.5;
    double y2=26;
    double y3=31;
    double y4=52;
    double y5=56;
    double y6=73.5;

    double OX=	FLEFT - (GTOP - GBOT);	// 该点为球门中心 向后移动半个球门
    double OY=	(FTOP + FBOT)/2;			//球门中心
    double ballx=p->curball.x	;
    double bally=p->curball.y	;
    double gx = p->robot[robot].pos.x ;
    double gx_outline = FLEFT + 2.2;		//对x坐标的限定，防止离球门线太远了
    double gx_inline = FLEFT + 1.8;
    double gy = p->robot[robot].pos.y;		//跑位点,初值为当前位置
    double goalline = FLEFT + 3;
    bool   notout= true;	//不出击
    bool   standby = true;	//限制x 坐标
    bool   XX=false;	//是否旋转



    if( XX && ballx < gx + 0.5 )
    {
        //旋转
        if(bally < gy && bally > gy-4 )
        {
            Velocity(env,robot,-125,125);
            return ;
        }
        else if(bally > gy && bally < gy+4)
        {
            Velocity(env,robot,125,-125);
            return ;
        }
    }


    gy = OY + ( goalline - OX ) * (bally - OY)/(ballx - OX);

    if(notout)
    {
        if(gy > GTOP+3)
            gy = GTOP+3;
        else if(gy < GBOT-3)
            gy = GBOT-3;
    }
    go.y = gy ;

    if(standby)
    {
        if(gx > gx_outline )
            gx = gx_outline;
        else if(gx < gx_inline)
            gx = gx_inline;
    }

    go.x = gx ;
    GoaliePosition(env,robot,go,90,1.5);

    if(p->curball.x <= x1 && p->curball.y >= y2 && p->curball.y <=y3
            && p->ballspeed.x <= 2 && p->ballspeed.y <= 2)
    {
        go.x = p->robot[0].pos.x;
        go.y = 29;

        PositionAndThrough(env, robot, go);


    }
    else if(p->curball.x <= x1 && p->curball.y >= y4 && p->curball.y <= y5
            && p->ballspeed.x <= 2 && p->ballspeed.y <= 2)
    {
        go.x = p->robot[0].pos.x;
        go.y = 53;

        PositionAndThrough(env, robot, go);


    }

}


void AreaKick(Environment *env,int robot,Vector3D pos,int radius)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    Vector3D Weakpoint(Environment *env);
    {
        PositionAndThrough(env,robot,pos,125);
        if ((Distance(p->curball,p->robot[robot].pos)<radius)&&(Distance(pos,p->robot[robot].pos)<radius))
            MyKick(env,robot,Weakpoint(env));
        else
        {
            PositionAndStopX(env,robot,pos,50,3);
        }
    }
}

void AreaDef(Environment *env,int robot,Vector3D pos,int radius)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    {
        if((Distance(p->curball,p->robot[robot].pos)<radius)&&(Distance(pos,p->robot[robot].pos)<radius))
            MyKick(env,robot,CONSTGATE);
        else PositionAndStopX(env,robot,pos,50,3);
    }
}

Vector3D Weakpoint(Environment *env)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    {
        double distance0;
        Vector3D pos1= {FRIGHT,GTOP-3,0.0};
        Vector3D pos2= {FRIGHT,GBOT+3,0.0}; //上角坐标,下角坐标
        distance0 = (p->opp[0].pos.y - CONSTGATE.y);
        if (distance0>0)  return pos2;
        else return pos1;
    }
}
/*void AvoidOpp(Environment *env,int robot,Vector3D pos)
{
	Mydata * p;
	p=(Mydata *)env->userData;
	if (Distance(p->robot[robot].pos,p->opp[1].pos)<3 || Distance(p->robot[robot].pos,p->opp[2].pos)<3 || Distance(p->robot[robot].pos,p->opp[3].pos)<3 || Distance(p->robot[robot].pos,p->opp[4].pos)<3)
	{
*/
void Attack( Environment *env )
{
    Mydata * p;
    p=(Mydata *)env->userData;
    Vector3D Heart= {(FLEFT+FRIGHT)/2,(FTOP+FBOT)/2,0};
    Vector3D LargeTopDot= {LRIGHT,LTOP,0};
    Vector3D LargeBotDot= {LRIGHT,LBOT,0};
    Vector3D GateBotDot= {FRIGHT,GBOT+3,0};
    Vector3D GateTopDot= {FRIGHT,GTOP-3,0};
    Vector3D pos1,pos2;
    {
        if ((p->curball.x > Heart.x) && (p->curball.x < PRIGHT))//中前场推进策略
        {
            /*if((p->curball.y > LargeBotDot.y)&&(p->curball.y < LargeTopDot.y))//中区推进，3,4执行推进，1,2各在边路跑位
            {
            	MyKick(env,3,Weakpoint(env));
            	MyKick(env,4,Weakpoint(env));
            	pos1.x=(p->curball.x)-10;
            	pos1.y=LTOP;
            	pos2.x=(p->curball.x)-10;
            	pos2.y=LBOT;
            	PositionAndThrough(env,1,pos1,125);
            	PositionAndThrough(env,2,pos2,125);
            }*/
            if((p->curball.y < Heart.y))//下区推进，2,3,4执行推进，1在中路跑位
            {
                MyKick(env,3,GateTopDot);
                MyKick(env,4,GateBotDot);
                MyKick(env,2,GateBotDot);
                pos1.x=((p->curball.x)-10);
                pos1.y=Heart.y;
                AreaKick(env,1,pos1,25);
            }
            else//上区推进，1,3,4执行推进，2在中路跑位
            {
                MyKick(env,3,GateTopDot);
                MyKick(env,4,GateBotDot);
                MyKick(env,1,GateTopDot);
                pos2.x=((p->curball.x)-10);
                pos2.y=Heart.y;
                AreaKick(env,2,pos2,25);
            }
        }
        if (p->curball.x > PRIGHT)//前场进攻策略
        {
            if((p->curball.y > LTOP)&&(p->curball.y < LBOT))//中区进攻，34执行攻击，12各在两翼抢点
            {
                pos1.x=(LargeTopDot.x);
                pos1.y=(LargeTopDot.y);
                pos2.x=(LargeBotDot.x);
                pos2.y=(LargeBotDot.y);
                MyKick(env,3,GateTopDot);
                MyKick(env,4,GateBotDot);
                AreaKick(env,2,pos2,25);
                AreaKick(env,1,pos1,25);
            }

            if(p->curball.y < LBOT)//下边路进攻，134射门，2点球点附近抢点
            {
                MyKick(env,3,GateTopDot);
                MyKick(env,4,GateBotDot);
                MyKick(env,1,GateBotDot);
                pos2.x=PRIGHT;
                pos2.y=Heart.y;
                AreaKick(env,2,pos2,25);
            }
            else//上边路进攻，234射门，1点球点附近抢点
            {
                MyKick(env,3,GateTopDot);
                MyKick(env,4,GateBotDot);
                MyKick(env,2,GateTopDot);
                pos1.x=PRIGHT;
                pos1.y=Heart.y;
                AreaKick(env,1,pos1,25);
            }
        }
    }

}

void Exit(Environment *env,int robot)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    {
        if (p->robot[robot].pos.x < LLEFT) PAngle(env,robot,0,125);
        else PAngle(env,robot,180,125);
    }
}


void AttFoul( Environment *env )
{
    Mydata * p;
    p=(Mydata *)env->userData;
    {
        int i,robot1,robot2;
        bool Flag;
        for(i=1; i<5; i++) if((p->robot[i].pos.x)>FRIGHT) Exit(env,i);
        if (((p->curball.x)>LRIGHT)&&((p->curball.y)>LBOT)&&((p->curball.y)<LTOP))
        {
            for(i=1; i<5; i++)
            {
                if(((p->robot[i].pos.x)>SRIGHT)&&((p->robot[i].pos.y)>SBOT)&&((p->robot[i].pos.y)<STOP))
                {
                    robot1=i;
                    Flag=true;
                    break;
                }
                else
                {
                    robot1=0;
                    Flag=false;
                }
            }
            if (Flag)
            {
                for(i=1; i<5; i++)
                {
                    if((i != robot1)&&(((p->robot[i].pos.x)>SRIGHT)&&((p->robot[i].pos.y)>SBOT)&&((p->robot[i].pos.y)<STOP)))
                    {
                        robot2=i;
                        Exit(env,i);
                        Flag=false;
                    }
                    else robot2=0;
                }

            }
        }

    }
}

void DefFoul( Environment *env )
{
    Mydata * p;
    p=(Mydata *)env->userData;
    {
        int i;
        if (((p->curball.x)<LLEFT)&&((p->curball.y)>LBOT)&&((p->curball.y)<LTOP))
        {
            for(i=1; i<5; i++)
            {
                if(((p->robot[i].pos.x)<SLEFT+3)&&((p->robot[i].pos.y)>SBOT-3)&&((p->robot[i].pos.y)<STOP+3))
                {
                    Exit(env,i);
                }
            }
        }
        for(i=1; i<5; i++)
            if((p->robot[i].pos.x)<FLEFT)
                Exit(env,i);
    }
}

void Defence( Environment *env )
{
    Mydata * p;
    p=(Mydata *)env->userData;
    Vector3D Heart= {(FLEFT+FRIGHT)/2,(FTOP+FBOT)/2,0};
    Vector3D LargeTopDot= {LLEFT,LTOP,0};
    Vector3D LargeBotDot= {LLEFT,LBOT,0};
    Vector3D GateBotDot= {FLEFT,GBOT+3,0};
    Vector3D GateTopDot= {FLEFT,GTOP-3,0};
    {
        if ((p->curball.x)>(FLEFT+FRIGHT)/4)//分前后半区的防守策略
        {
            if((p->curball.y)>Heart.y)//上区防守策略，134围抢，2在点球点掩护
            {
                Vector3D pos2= {PLEFT,Heart.y,0};
                Vector3D pos4= {(PLEFT+FLEFT)/2,(FTOP+LTOP)/2,0};
                MyKick(env,1,CONSTGATE);
                MyKick(env,3,CONSTGATE);
                AreaDef(env,4,pos4,20);
                AreaDef(env,2,pos2,20);
                /*{
                	PositionAndThrough(env,2,pos2,125);
                	Angle(env,2,p->curball);
                }*/

            }
            else//下区防守策略，234围抢，1在点球点掩护
            {
                Vector3D pos1= {PLEFT,Heart.y,0};
                Vector3D pos4= {(PLEFT+FLEFT)/2,(FBOT+LBOT)/2,0};
                MyKick(env,2,CONSTGATE);
                MyKick(env,3,CONSTGATE);
                AreaDef(env,4,pos4,20);
                AreaDef(env,1,pos1,20);
                /*{
                	PositionAndThrough(env,1,pos1,125);
                	Angle(env,1,p->curball);
                }*/
            }
        }
        else//危险区策略
            if((p->curball.y)>Heart.y)//上半区，3抢球往对方半场推，4抢球往边线推，2在底线防守，1在点球点防守
            {
                Vector3D pos1= {(PLEFT+FLEFT)/2,(FTOP+LTOP)/2,0};
                Vector3D pos2= {FLEFT,FTOP,0};
                Vector3D pos3= {PLEFT,Heart.y,0};
                Vector3D pos4= {(PLEFT+SLEFT)/2,(FTOP+LTOP)/2,0};
                if (p->curball.x < SLEFT) MyKick(env,3,pos2);
                else MyKick(env,3,CONSTGATE);
                AreaDef(env,4,pos4,20);
                AreaDef(env,2,pos3,20);
                /*{
                	PositionAndThrough(env,2,pos3,125);
                	Angle(env,2,p->curball);
                }*/
                AreaDef(env,1,pos1,20);
                /*{
                	PositionAndThrough(env,1,pos1,125);
                	Angle(env,1,p->curball);
                }*/
            }

            else//下半区，3抢球往对方半场推，4抢球往边线推，1在底线防守，2在点球点防守
            {
                Vector3D pos1= {(PLEFT+FLEFT)/2,(FBOT+LBOT)/2,0};
                Vector3D pos2= {FLEFT,FBOT,0};
                Vector3D pos3= {PLEFT,Heart.y,0};
                Vector3D pos4= {(PLEFT+SLEFT)/2,(FBOT+LBOT)/2,0};
                if (p->curball.x < SLEFT) MyKick(env,3,pos2);
                else MyKick(env,3,CONSTGATE);
                AreaDef(env,4,pos4,20);
                AreaDef(env,1,pos3,20);
                /*{
                	PositionAndThrough(env,1,pos3,125);
                	Angle(env,1,p->curball);
                }*/
                AreaDef(env,2,pos2,20);
                /*{
                	PositionAndThrough(env,2,pos1,125);
                	Angle(env,2,p->curball);
                }*/
            }
    }
}

void Penalty ( Environment *env )
{

    Mydata * p;
    p=(Mydata *)env->userData;

    Vector3D pos;
    pos.x=FRIGHT;
    pos.y=40;
    Kick(env,4,pos);
    PredictBall(env,4);
    PositionAndThrough(env,4,p->preball,125);

}

/*void AttackStrategy ( Environment *env )
{
	Mydata * p;
	p=(Mydata *)env->userData;


    const double BTop=58.5660;
	const double BBot=27.8966;
	const double PENALTYRIGHT=79.5556;
	const double PENALTYLEFT=43.0032;

	Vector3D Position1={FRIGHT,FTOP,0};
    Vector3D Position2={FRIGHT,(FTOP+BTop)/2,0};
    Vector3D Position3={FRIGHT,BTop,0};
	Vector3D Position4={FRIGHT,(BTop+BBot)/2,0};
	Vector3D Position5={FRIGHT,FBOT,0};
    Vector3D Position6={FRIGHT,(FBOT+BBot)/2,0};
    Vector3D Position7={FRIGHT,FBOT,0};
	Vector3D Heart={(FLEFT+FRIGHT)/2,(FTOP+FBOT)/2,0};

	Vector3D PositionA;
	Vector3D PositionB;
	Vector3D PositionC;
	Vector3D pos1,pos2;


//A区（顶部）机器人1
	int i;
	for(i=1;i<4;i++)
	{
		if ((p->robot[i].pos.y<LTOP)&&(p->robot[i].pos.y>LBOT)&&(p->robot[i].pos.x<SLEFT))
			PositionAndThrough (env, i,Heart,125);
	else
	{
	if (p->curball.y>(BTop+BBot)/2 && p->curball.y<FTOP)
	{
		/*if (Distance(p->curball,p->robot[1].pos)<3 || Distance(p->curball,p->robot[2].pos)<3 || Distance(p->curball,p->robot[3].pos)<3 || Distance(p->curball,p->robot[4].pos)<3)
		{
			PositionA.x=p->curball.x;
			PositionA.y=Position2.y;
			PositionAndThrough (env, 1,PositionA);
		}
		else
		{
			PositionAndThrough (env, 1,p->curball);

		}
		if (p->curball.x < 70 )
			MyKick (env,1,Position3);
		else
		{
			if ((p->robot[4].pos.x >70) && (Distance(p->robot[1].pos,p->robot[4].pos)<15)) MyKick(env,1,p->robot[4].pos);
			else MyKick(env,1,Weakpoint(env));
		}
		if ((Distance(p->robot[1].pos,p->opp[1].pos)<10) || (Distance(p->robot[1].pos,p->opp[2].pos)<10) || (Distance(p->robot[1].pos,p->opp[3].pos)<10) || (Distance(p->robot[1].pos,p->opp[4].pos)<10))
			MyKick (env,1,Position1);
		if ((Distance(p->robot[1].pos,p->opp[1].pos)<9) || (Distance(p->robot[1].pos,p->opp[2].pos)<9) || (Distance(p->robot[1].pos,p->opp[3].pos)<9) || (Distance(p->robot[1].pos,p->opp[4].pos)<9))
			MyKick (env,1,Position3);
		else MyKick(env,1,Position2);
		if (p->curball.x > 88 && p->robot[4].pos.x > 88)
		{
			PositionAndThrough (env, 1,Heart,150);
		}

	}
	else
	{
		PredictBall(env,10);
		pos1.x=p->preball.x;
		pos1.y=p->robot[1].pos.y;
		PositionAndThrough (env, 1,pos1);
	}




//C区（底部）机器人2

	if (p->curball.y>FBOT && p->curball.y<(BBot+BTop)/2)
	{
		if (Distance(p->curball,p->robot[1].pos)<3 || Distance(p->curball,p->robot[2].pos)<3 || Distance(p->curball,p->robot[3].pos)<3 || Distance(p->curball,p->robot[4].pos)<3)
		{
			PositionB.x=p->curball.x;
			PositionB.y=Position4.y;
			PositionAndThrough (env, 2,PositionB);
		}
		else
		{
			PositionAndThrough (env, 2,p->curball);

		}
		if (p->curball.x < 70 )
			MyKick (env,2,Position5);
		else
		{
			if ((p->robot[4].pos.x>70) && (Distance(p->robot[1].pos,p->robot[4].pos)<15)) MyKick(env,2,p->robot[4].pos);
			else MyKick(env,2,Weakpoint(env));
		}
		if ((Distance(p->robot[2].pos,p->opp[1].pos)<10) || (Distance(p->robot[2].pos,p->opp[2].pos)<10) || (Distance(p->robot[2].pos,p->opp[3].pos)<10) || (Distance(p->robot[2].pos,p->opp[4].pos)<10))
			MyKick (env,2,Position3);
		if ((Distance(p->robot[2].pos,p->opp[1].pos)<9) || (Distance(p->robot[2].pos,p->opp[2].pos)<9) || (Distance(p->robot[2].pos,p->opp[3].pos)<9) || (Distance(p->robot[2].pos,p->opp[4].pos)<9))
			MyKick (env,2,Weakpoint(env));
		else MyKick(env,2,Position4);
		if (p->curball.x > 88 && p->robot[4].pos.x > 88)
		{
			PositionAndThrough (env, 2,Heart,150);
		}
	}
	else
	{
		PredictBall(env,10);
		pos2.x=p->preball.x;
		pos2.y=p->robot[2].pos.y;
		PositionAndThrough (env, 2,pos2);
	}



//B区（中）机器人3
	MyKick(env,3,Weakpoint(env));

	if (p->curball.y>BBot&&p->curball.y<BTop)
		{
		if (Distance(p->curball,p->robot[1].pos)<3 || Distance(p->curball,p->robot[2].pos)<3 || Distance(p->curball,p->robot[3].pos)<3 || Distance(p->curball,p->robot[4].pos)<3)
		{
			PositionC.x=p->curball.x;
			PositionC.x=Position6.y;
			PositionAndThrough (env, 3,PositionA);
		}
		else
		{
			PositionAndThrough (env, 3,p->curball);

		}
		if (p->curball.x < 70 )
			MyKick (env,3,Position6);
		else
		{
			if ((p->robot[4].pos.x>70) && (Distance(p->robot[1].pos,p->robot[4].pos)<15)) MyKick(env,3,p->robot[4].pos);
			else MyKick(env,3,Weakpoint(env));
		}
		if ((Distance(p->robot[3].pos,p->opp[1].pos)<10) || (Distance(p->robot[3].pos,p->opp[2].pos)<10) || (Distance(p->robot[3].pos,p->opp[3].pos)<10) || (Distance(p->robot[3].pos,p->opp[4].pos)<10))
			MyKick (env,3,Position1);
		if ((Distance(p->robot[3].pos,p->opp[1].pos)<9) || (Distance(p->robot[3].pos,p->opp[2].pos)<9) || (Distance(p->robot[3].pos,p->opp[3].pos)<9) || (Distance(p->robot[3].pos,p->opp[4].pos)<9))
			MyKick (env,3,Position3);
		else MyKick(env,3,Position6);
		if (p->curball.x > 88 && p->robot[4].pos.x > 88)
		{
			PositionAndThrough (env, 3,Heart,50);
		}

	}
	else
	{
		pos3.x=p->curball.x;
		pos3.y=p->robot[3].pos.y;
		PositionAndThrough (env, 3,pos3);
	}
//自由机器人4号

	MyKick(env,4,Weakpoint(env));
	if ((p->robot[1].pos.x<FLEFT || p->robot[1].pos.x > FRIGHT)) PositionAndThrough (env, 1,Heart,125);
	if ((p->robot[2].pos.x<FLEFT || p->robot[2].pos.x > FRIGHT)) PositionAndThrough (env, 2,Heart,125);
	if ((p->robot[3].pos.x<FLEFT || p->robot[3].pos.x > FRIGHT)) PositionAndThrough (env, 3,Heart,125);
	if ((p->robot[4].pos.x<FLEFT || p->robot[4].pos.x > FRIGHT)) PositionAndThrough (env, 4,Heart,125);
	if ((p->curball.x<SLEFT)&&(p->curball.y<LBOT))
	{ MyKick(env,2,Weakpoint(env));
	 MyKick(env,3,Weakpoint(env));
	}
	if ((p->curball.x<SLEFT)&&(p->curball.y>LTOP))
	{ MyKick(env,2,Weakpoint(env));
	 MyKick(env,1,Weakpoint(env));
	}
	}
	}
}*/

