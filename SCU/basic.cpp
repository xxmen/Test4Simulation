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
DllMain,Create,Destroy���������������Ҳ�����ᣬ
ֻ��������ڲ���֮�������������Ҫ����ֵ������Create �н���
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

	//MessageBox(NULL,"�Ĵ���ѧ�����","SCU_KS",MB_OK);
	//��ʼ������
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

		p->opp[i].pos.x = InlitializeOppPosition_X[i];			//�Է�����
		p->opp[i].pos.y = InlitializeOppPosition_Y[i];			//�Է�����
		p->opp[i].pos.z = InlitializeOppPosition_Z[i];			//�Է�����

		p->opoldpos[i].x = InlitializeOppPosition_X[i];			//�Է� ������
		p->opoldpos[i].y = InlitializeOppPosition_Y[i];			//�Է� ������
		p->opoldpos[i].z = InlitializeOppPosition_Z[i];			//�Է� ������

		p->opspeed[0].x = 0;			//�Է� ��Ա�ٶ�
		p->opspeed[0].y = 0;			//�Է� ��Ա�ٶ�
		p->opspeed[0].z = 0;			//�Է� ��Ա�ٶ�

	}

	p->locked=false;				// �ж��� ���� ??
	p->mygrand=true;				// �� �ƶ� ??

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
//////////////������  !!�ƶ�
{
	Mydata * p;
	p=(Mydata *)env->userData;

	if(!p->locked)		// �� �жϳ����� ??
	{//ȷ������,blue or yellow
		if( env->home[0].pos.x < 50.0 )
			p->mygrand=true; /// �� = �ƶ�??
		else
			p->mygrand=false;
		p->locked=true;
	}

	See(env);			// Ԥ����
	
	MyGame(env);		//����

	End ( env );		//���ڴ���

}



//
/* See Ԥ����
1����������任
2�����һЩ�����о����õ���������ÿ����Ա�����ʺͽ��ٶȵ�
*/
//
void See ( Environment *env )
{
	Mydata * p;
	p=(Mydata *)env->userData;

	int i=0;


	if(p->mygrand)
	{///���ر任��������任
		//�ҷ��ǻƶ�

		p->gameState = env->gameState ;
		switch(env->whoseBall)
		{
		case 0 ://������,�ڴ������ʱ�����ַ������ܲ�����
			//��ð�����������꣬���ж�˭�ĵ��򣬺���������
			p->whoseBall = 0;
		case 1 :
			p->whoseBall = 1;
		case 2:
			p->whoseBall =2 ;
		}


		p->curball.x = env->currentBall.pos.x;		//������仯
		p->curball.y = env->currentBall.pos.y;
		//p->curball.z = env->currentBall.pos.z;


		for(i=0;i<5;i++)
		{
			p->robot[i].pos.x = env->home[i].pos.x ;	//�ҷ���Ա����任
			p->robot[i].pos.y = env->home[i].pos.y ;
			//p->robot[i].pos.z = env->home[i].pos.z ;
			p->robot[i].rotation= env->home[i].rotation;
			
			p->opp[i].pos.x =env->opponent[i].pos.x;	//�Է�����任
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
		case 0 ://������,�ڴ������ʱ�����ַ������ܲ�����
			//��ð�����������꣬���ж�˭�ĵ��򣬺���������
			p->whoseBall = 0;
		case 1 :
			p->whoseBall = 2;
		case 2:
			p->whoseBall = 1;
		}


		p->curball.x =FLEFT+FRIGHT + CORRECTX - env->currentBall.pos.x;		//������仯
		p->curball.y =FBOT+FTOP + CORRECTY - env->currentBall.pos.y;
		//p->curball.z = env->currentBall.pos.z;

		for(i=0;i<5;i++)
		{
			p->robot[i].pos.x =FLEFT+FRIGHT + CORRECTX - env->home[i].pos.x ;	//�ҷ���Ա����任
			p->robot[i].pos.y =FBOT+FTOP + CORRECTY - env->home[i].pos.y ;
			//p->robot[i].pos.z = env->home[i].pos.z ;
			p->robot[i].rotation= 180.0 + env->home[i].rotation;
			ReAngle(p->robot[i].rotation);
			
			p->opp[i].pos.x = FLEFT+FRIGHT + CORRECTX- env->opponent[i].pos.x;	//�Է�����任
			p->opp[i].pos.y = FBOT+FTOP + CORRECTY - env->opponent[i].pos.y;
			//p->opp[i].pos.z = env->opponent[i].pos.z;
			p->opp[i].rotation =  180 + env->opponent[i].rotation;
			ReAngle(p->opp[i].rotation);
		}
	}
	

////��һ�δ����ٶ�  (�ϴ�)
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
			p->myspeed[i].x = 0;	//70Ϊ����ϵ�����д�����
			p->myspeed[i].y = 0;
			p->myspeed[i].z = 0;		//�������� �˶����� ��ת���ٶ�,������

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
			p->myspeed[i].x = ( p->robot[i].pos.x - p->myoldpos[i].x);	//70Ϊ����ϵ�����д�����
			p->myspeed[i].y = ( p->robot[i].pos.y - p->myoldpos[i].y);
			p->myspeed[i].z = Atan(p->myspeed[i].y,p->myspeed[i].x);		//�������� �˶����� ��ת���ٶ�,������

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
				///����ײǽ
				//���������

				////����ײǽ		
				p->myoldpos[i].x =a;
				p->myoldpos[i].y =b;
				p->myoldpos[i].z =c;

				p->myspeed[i].x = ( p->robot[i].pos.x - p->myoldpos[i].x );	//70Ϊ����ϵ�����д�����
				p->myspeed[i].y = ( p->robot[i].pos.y - p->myoldpos[i].y );
				p->myspeed[i].z = Atan( p->myspeed[i].y , p->myspeed[i].x );
			}
			

			/////////	 Ԥ���������

			double x,y;
			x = p->curball.x ;
			y = p->curball.y ;

			PredictBall(env);		//���������λ��
			p->curball = p->preball;

			
			p->oldball.x = x;
			p->oldball.y = y;

			PredictBall(env);		//Ԥ����һ�����λ��


			p->ballspeed.x = p->curball.x - p->oldball.x;
			p->ballspeed.y = p->curball.y - p->oldball.y;
			p->ballspeed.z = Atan( p->ballspeed.y , p->ballspeed.x );
		}
		

	}
	PredictBall2(env);	
	for(i=0;i<5;i++)		
	{//����ֵ
		p->robot[i].velocityLeft = 0;
		p->robot[i].velocityRight = 0;
	}

	
}

void End ( Environment *env )
{
	///��һЩ��ɨ�Ĺ���
	///��һЩ��¼������
	
	Mydata * p;
	p=(Mydata *)env->userData;
	
	int i=0;

	
	/////////////////////////////////////////////////////////////////////
	for(i=0;i<5;i++)
	{//�ٶ�
		env->home[i].velocityLeft = p->robot[i].velocityLeft;
		env->home[i].velocityRight = p->robot[i].velocityRight;
		
		p->myoldvelocity[i].x=p->robot[i].velocityLeft;
		p->myoldvelocity[i].y=p->robot[i].velocityRight;

	}

	if(p->mygrand)
	{///���ر任��������任
		p->oldball.x = env->currentBall.pos.x;		//������仯
		p->oldball.y = env->currentBall.pos.y;
		//p->curball.z = env->currentBall.pos.z;


		for(i=0;i<5;i++)
		{
			p->myoldpos[i].x = env->home[i].pos.x ;	//�ҷ���Ա����任
			p->myoldpos[i].y = env->home[i].pos.y ;
			//p->robot[i].pos.z = env->home[i].pos.z ;
			p->myoldpos[i].z= env->home[i].rotation;
			
			p->opoldpos[i].x =env->opponent[i].pos.x;	//�Է�����任
			p->opoldpos[i].y =env->opponent[i].pos.y;
			//p->opp[i].pos.z = env->opponent[i].pos.z;
			p->opoldpos[i].z =env->opponent[i].rotation;
			ReAngle(p->opoldpos[i].z);

		}
	}
	else
	{
		p->oldball.x =FLEFT+FRIGHT + CORRECTX - env->currentBall.pos.x;		//������仯
		p->oldball.y =FBOT+FTOP + CORRECTY - env->currentBall.pos.y;
		//p->curball.z = env->currentBall.pos.z;

		for(i=0;i<5;i++)
		{
			p->myoldpos[i].x =FLEFT+FRIGHT + CORRECTX - env->home[i].pos.x ;	//�ҷ���Ա����任
			p->myoldpos[i].y =FBOT+FTOP + CORRECTY - env->home[i].pos.y ;
			//p->robot[i].pos.z = env->home[i].pos.z ;
			p->myoldpos[i].z = 180.0 + env->home[i].rotation;
			ReAngle(p->myoldpos[i].z);
			
			p->opoldpos[i].x = FLEFT+FRIGHT + CORRECTX- env->opponent[i].pos.x;	//�Է�����任
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
/* CheckBall	���������ڵķ�����

*/
////////////
