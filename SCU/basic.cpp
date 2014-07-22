#include <string.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#include "basic.h"
#include "tools.h"
#include "mygame.h"
#include "myfunction.h"
#include "action.h"
#include "strategy.h"


//////////////////////////////////////////////////////////
/*
DllMain,Create,Destroy这三个函数建议大家不用理会，
只是如果你在策略之间如果有数据需要赋初值可以在Create 中进行
*/

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


extern "C" STRATEGY_API void Create ( Environment *env )
{

	//MessageBox(NULL,"四川大学代表队","SCU_KS",MB_OK);
	//初始化数据
	env->userData=(void* ) new Mydata;
	Mydata* p;
	p=(Mydata*)env->userData;//???

	p->a=fopen("c:\\output.txt","w");//w
	int i;


	for(i =0 ;i < 5;i++)
	{
		
	
		p->robot[i].pos.x=InlitializeMyPosition_X[i];
		p->robot[i].pos.y=InlitializeMyPosition_Y[i];
		p->robot[i].pos.z=InlitializeMyPosition_Z[i];			//what is 'z'?

		p->myoldpos[i].x=InlitializeMyPosition_X[i];
		p->myoldpos[i].y=InlitializeMyPosition_Y[i];
		p->myoldpos[i].z=InlitializeMyPosition_Z[i];			//this z is the direction that the robot faces

		p->myspeed[i].x=0;
		p->myspeed[i].y=0;
		p->myspeed[i].z=0;

		p->myoldvelocity[i].x=0;
		p->myoldvelocity[i].y=0;
		p->myoldvelocity[i].z=0;

		p->opp[i].pos.x = InlitializeOppPosition_X[i];			//对方坐标
		p->opp[i].pos.y = InlitializeOppPosition_Y[i];			//对方坐标
		p->opp[i].pos.z = InlitializeOppPosition_Z[i];			//对方坐标

		p->opoldpos[i].x = InlitializeOppPosition_X[i];			//对方 旧坐标
		p->opoldpos[i].y = InlitializeOppPosition_Y[i];			//对方 旧坐标
		p->opoldpos[i].z = InlitializeOppPosition_Z[i];			//对方 旧坐标

		p->opspeed[0].x = 0;			//对方 队员速度
		p->opspeed[0].y = 0;			//对方 队员速度
		p->opspeed[0].z = 0;			//对方 队员速度

	}

	p->locked=false;				// 判断了 场地 ??
	p->mygrand=true;				// 是 黄队 ??

	p->oldball.x = (FLEFT + FRIGHT) / 2.0;
	p->oldball.y = (FTOP + FBOT) / 2.0;
	p->oldball.z = 0;							//what is 'z' mean? the ball's speed ??

	p->curball.x = (FLEFT + FRIGHT) / 2.0;
	p->curball.y = (FTOP + FBOT) / 2.0;
	p->curball.z = 0;

	p->preball.x = (FLEFT + FRIGHT) / 2.0;
	p->preball.y = (FTOP + FBOT) / 2.0;
	p->preball.z = 0;

	p->ballspeed.x =0;
	p->ballspeed.y =0;
	p->ballspeed.z =0;

}

extern "C" STRATEGY_API void Strategy ( Environment *env )
//////////////适用于  !!黄队
{
	Mydata * p;
	p=(Mydata *)env->userData;

	if(!p->locked)		// 是 判断场地了 ??
	{//确定区域,blue or yellow
		if( env->home[0].pos.x < 50.0 )
			p->mygrand=true; /// 是 = 黄队??
		else
			p->mygrand=false;
		p->locked=true;
	}

	See(env);			// 预处理
	
	MyGame(env);		//策略

	End ( env );		//后期处理

}



//
/* See 预处理
1。进行坐标变换
2。求出一些策略中经常用到的量，如每个队员的速率和角速度等
*/
//
void See ( Environment *env )
{
	Mydata * p;
	p=(Mydata *)env->userData;

	int i=0;


	if(p->mygrand)
	{///场地变换，球坐标变换
		//我方是黄队

		p->gameState = env->gameState ;
		switch(env->whoseBall)
		{
		case 0 ://自由球,在处理球的时候这种方法可能不保险
			//最好包括用球的坐标，来判断谁的点球，和其他的球
			p->whoseBall = 0;
		case 1 :
			p->whoseBall = 1;
		case 2:
			p->whoseBall =2 ;
		}


		p->curball.x = env->currentBall.pos.x;		//球坐标变化
		p->curball.y = env->currentBall.pos.y;
		//p->curball.z = env->currentBall.pos.z;


		for(i=0;i<5;i++)
		{
			p->robot[i].pos.x = env->home[i].pos.x ;	//我方队员坐标变换
			p->robot[i].pos.y = env->home[i].pos.y ;
			//p->robot[i].pos.z = env->home[i].pos.z ;
			p->robot[i].rotation= env->home[i].rotation;
			
			p->opp[i].pos.x =env->opponent[i].pos.x;	//对方坐标变换
			p->opp[i].pos.y =env->opponent[i].pos.y;
			//p->opp[i].pos.z = env->opponent[i].pos.z;
			p->opp[i].rotation =env->opponent[i].rotation;
			ReAngle(p->opp[i].rotation);

		}
	}
	else
	{//we are blue
		p->gameState = env->gameState ;
		switch(env->whoseBall)
		{
		case 0 ://自由球,在处理球的时候这种方法可能不保险
			//最好包括用球的坐标，来判断谁的点球，和其他的球
			p->whoseBall = 0;
		case 1 :
			p->whoseBall = 2;
		case 2:
			p->whoseBall = 1;
		}


		p->curball.x =FLEFT+FRIGHT + CORRECTX - env->currentBall.pos.x;		//球坐标变化
		p->curball.y =FBOT+FTOP + CORRECTY - env->currentBall.pos.y;
		//p->curball.z = env->currentBall.pos.z;

		for(i=0;i<5;i++)
		{
			p->robot[i].pos.x =FLEFT+FRIGHT + CORRECTX - env->home[i].pos.x ;	//我方队员坐标变换
			p->robot[i].pos.y =FBOT+FTOP + CORRECTY - env->home[i].pos.y ;
			//p->robot[i].pos.z = env->home[i].pos.z ;
			p->robot[i].rotation= 180.0 + env->home[i].rotation;
			ReAngle(p->robot[i].rotation);
			
			p->opp[i].pos.x = FLEFT+FRIGHT + CORRECTX- env->opponent[i].pos.x;	//对方坐标变换
			p->opp[i].pos.y = FBOT+FTOP + CORRECTY - env->opponent[i].pos.y;
			//p->opp[i].pos.z = env->opponent[i].pos.z;
			p->opp[i].rotation =  180 + env->opponent[i].rotation;
			ReAngle(p->opp[i].rotation);
		}
	}
	

////第一次处理速度  (上次)
	bool ReSet=false;
	if(p->curball.x -p->oldball.x > 10 || p->curball.x -p->oldball.x < -10 || p->curball.y -p->oldball.y > 10 || p->curball.y -p->oldball.y < -10 ){
		ReSet=true;
	}
	for(i=0;i<5;i++){
		if(p->robot[i].pos.x -p->myoldpos[i].x > 10 || p->robot[i].pos.x -p->myoldpos[i].x < -10 || p->robot[i].pos.y -p->myoldpos[i].y > 10 || p->robot[i].pos.y -p->myoldpos[i].y < -10 ){
			ReSet=true; 
		}

		if(p->opp[i].pos.x -p->opoldpos[i].x > 10 || p->opp[i].pos.x -p->opoldpos[i].x < -10 || p->opp[i].pos.y -p->opoldpos[i].y > 10 || p->opp[i].pos.y -p->opoldpos[i].y < -10 ){
			ReSet=true; 
		}
	}

	if(ReSet==true)
	{
		p->flag=true;
		for(i=0;i<5;i++)
		{///speed
			p->myspeed[i].x = 0;	//70为比例系数，有待调整
			p->myspeed[i].y = 0;
			p->myspeed[i].z = 0;		//关于他的 运动方向 和转角速度,考虑中

			p->opspeed[i].x = 0;
			p->opspeed[i].y = 0;
			p->opspeed[i].z = 0;
		}
		p->ballspeed.x = 0;
		p->ballspeed.y = 0;
		p->ballspeed.z = 0;
		
	}
	else
	{
		p->flag=false;
		for(i=0;i<5;i++)
		{///speed
			p->myspeed[i].x = ( p->robot[i].pos.x - p->myoldpos[i].x);	//70为比例系数，有待调整
			p->myspeed[i].y = ( p->robot[i].pos.y - p->myoldpos[i].y);
			p->myspeed[i].z = Atan(p->myspeed[i].y,p->myspeed[i].x);		//关于他的 运动方向 和转角速度,考虑中

			p->opspeed[i].x = ( p->opp[i].pos.x - p->opoldpos[i].x);
			p->opspeed[i].y = ( p->opp[i].pos.y - p->opoldpos[i].y);
			p->opspeed[i].z = Atan(p->opspeed[i].y,p->opspeed[i].x);
		}

		p->ballspeed.x = p->curball.x - p->oldball.x;
		p->ballspeed.y = p->curball.y - p->oldball.y;
		p->ballspeed.z = Atan( p->ballspeed.y , p->ballspeed.x );
	

		{
			double v,a,b,c,omiga,angle;
			for(i=0;i<5;i++)
			{

				omiga = p->robot[i].rotation - p->myoldpos[i].z ;
				ReAngle(omiga);
				omiga = AngleOne(omiga,p->myoldvelocity[i].x , p->myoldvelocity[i].y);
				c = p->robot[i].rotation;	
				p->robot[i].rotation+=omiga;
				ReAngle(p->robot[i].rotation);

				v = sqrt((p->myspeed[i].x * p->myspeed[i].x) + (p->myspeed[i].y * p->myspeed[i].y));
				angle = p->robot[i].rotation - p->myspeed[i].z;
				ReAngle(angle);
				if(angle >-90 && angle < 90 )		
					v=v;
				else
					v=-v;

				v=VelocityOne(v,p->myoldvelocity[i].x , p->myoldvelocity[i].y);
				a=p->robot[i].pos.x;
				b=p->robot[i].pos.y;

				p->robot[i].pos.x += v*cos( p->robot[i].rotation * PI / 180) ;
				p->robot[i].pos.y += v*sin( p->robot[i].rotation * PI / 180) ;
				///处理撞墙
				//不处理最好

				////处理撞墙		
				p->myoldpos[i].x =a;
				p->myoldpos[i].y =b;
				p->myoldpos[i].z =c;

				p->myspeed[i].x = ( p->robot[i].pos.x - p->myoldpos[i].x );	//70为比例系数，有待调整
				p->myspeed[i].y = ( p->robot[i].pos.y - p->myoldpos[i].y );
				p->myspeed[i].z = Atan( p->myspeed[i].y , p->myspeed[i].x );
			}
			

			/////////	 预测球的坐标

			double x,y;
			x = p->curball.x ;
			y = p->curball.y ;

			PredictBall(env);		//求到现在球的位置
			p->curball = p->preball;

			
			p->oldball.x = x;
			p->oldball.y = y;

			PredictBall(env);		//预测下一步球的位置


			p->ballspeed.x = p->curball.x - p->oldball.x;
			p->ballspeed.y = p->curball.y - p->oldball.y;
			p->ballspeed.z = Atan( p->ballspeed.y , p->ballspeed.x );
		}
		

	}
	PredictBall2(env);	
	for(i=0;i<5;i++)		
	{//赋初值
		p->robot[i].velocityLeft = 0;
		p->robot[i].velocityRight = 0;
	}

	
}

void End ( Environment *env )
{
	///做一些清扫的工作
	///做一些记录整理工作
	
	Mydata * p;
	p=(Mydata *)env->userData;
	
	int i=0;

	
	/////////////////////////////////////////////////////////////////////
	for(i=0;i<5;i++)
	{//速度
		env->home[i].velocityLeft = p->robot[i].velocityLeft;
		env->home[i].velocityRight = p->robot[i].velocityRight;
		
		p->myoldvelocity[i].x=p->robot[i].velocityLeft;
		p->myoldvelocity[i].y=p->robot[i].velocityRight;

	}

	if(p->mygrand)
	{///场地变换，球坐标变换
		p->oldball.x = env->currentBall.pos.x;		//球坐标变化
		p->oldball.y = env->currentBall.pos.y;
		//p->curball.z = env->currentBall.pos.z;


		for(i=0;i<5;i++)
		{
			p->myoldpos[i].x = env->home[i].pos.x ;	//我方队员坐标变换
			p->myoldpos[i].y = env->home[i].pos.y ;
			//p->robot[i].pos.z = env->home[i].pos.z ;
			p->myoldpos[i].z= env->home[i].rotation;
			
			p->opoldpos[i].x =env->opponent[i].pos.x;	//对方坐标变换
			p->opoldpos[i].y =env->opponent[i].pos.y;
			//p->opp[i].pos.z = env->opponent[i].pos.z;
			p->opoldpos[i].z =env->opponent[i].rotation;
			ReAngle(p->opoldpos[i].z);

		}
	}
	else
	{
		p->oldball.x =FLEFT+FRIGHT + CORRECTX - env->currentBall.pos.x;		//球坐标变化
		p->oldball.y =FBOT+FTOP + CORRECTY - env->currentBall.pos.y;
		//p->curball.z = env->currentBall.pos.z;

		for(i=0;i<5;i++)
		{
			p->myoldpos[i].x =FLEFT+FRIGHT + CORRECTX - env->home[i].pos.x ;	//我方队员坐标变换
			p->myoldpos[i].y =FBOT+FTOP + CORRECTY - env->home[i].pos.y ;
			//p->robot[i].pos.z = env->home[i].pos.z ;
			p->myoldpos[i].z = 180.0 + env->home[i].rotation;
			ReAngle(p->myoldpos[i].z);
			
			p->opoldpos[i].x = FLEFT+FRIGHT + CORRECTX- env->opponent[i].pos.x;	//对方坐标变换
			p->opoldpos[i].y = FBOT+FTOP + CORRECTY - env->opponent[i].pos.y;
			//p->opp[i].pos.z = env->opponent[i].pos.z;
			p->opoldpos[i].z =  180 + env->opponent[i].rotation;
			ReAngle(p->opoldpos[i].z);
		}
	}



}

void MyGame ( Environment *env )
{
	Mydata * p;
	p=(Mydata *)env->userData;
	switch(p->gameState)
	{
	case FREE_BALL:	//1:
		 FreeBallGame(env);
		 break;
	case PLACE_KICK:	//2:
   		 PlaceBallGame(env);	 
		 break;
	case PENALTY_KICK:	//3:
		 PenaltyBallGame(env);	
		 break;
	case FREE_KICK:	//4:
	     FreeKickGame(env);
		 break;
	case GOAL_KICK:	//5:
		 GoalKickGame(env);
		 break;
	default :
		 NormalGame(env);
		 break;
	}
	
}

extern "C" STRATEGY_API void Destroy ( Environment *env )
{
	Mydata * p;
	p=(Mydata *)env->userData;

	fclose(p->a);

	if ( env->userData != NULL )	delete ( Mydata * ) env->userData;
}

////////////
/* CheckBall	返回球所在的分区号

*/
////////////
