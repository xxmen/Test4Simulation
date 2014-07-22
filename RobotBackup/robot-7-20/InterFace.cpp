#include <windows.h>
#include "InterFace.h"
#include "Strategy.h"

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
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
extern "C" STRATEGY_API void Create ( Environment *env )//用于建立初始化文档储存球场动态数据以留作分析
{
    //MessageBox(NULL,"四川大学代表队","SCU_KS",MB_OK);
    //初始化数据
    env->userData=(void* ) new Mydata;//definition of Mydata in Interface.h
    Mydata* p;
    p=(Mydata*)env->userData;//p as an pointer
    p->a=fopen("c:\\output.txt","w");//using txt file to out put the instant data
    int i;
    for(i =0 ; i < 5; i++)//初始化text中的记录
    {


        p->robot[i].pos.x=InlitializeMyPosition_X[i];
        p->robot[i].pos.y=InlitializeMyPosition_Y[i];
        p->robot[i].pos.z=InlitializeMyPosition_Z[i];			//what is 'z'? z为方向

        p->myoldpos[i].x=InlitializeMyPosition_X[i];
        p->myoldpos[i].y=InlitializeMyPosition_Y[i];
        p->myoldpos[i].z=InlitializeMyPosition_Z[i];			//this z is the direction that the robot faces

        p->myspeed[i].x=0;
        p->myspeed[i].y=0;
        p->myspeed[i].z=0;

        p->myoldvelocity[i].x=0;//上次的驱动力
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

    p->oldball.x = (FLEFTX + FRIGHTX) / 2.0;//球上次位置位于球场中央
    p->oldball.y = (FTOP + FBOT) / 2.0;
    p->oldball.z = 0;							//what is 'z' mean? the ball's speed ??

    p->curball.x = (FLEFTX + FRIGHTX) / 2.0;//球目前位置位于球场中央
    p->curball.y = (FTOP + FBOT) / 2.0;
    p->curball.z = 0;

    p->preball.x = (FLEFTX + FRIGHTX) / 2.0;//球将来位置位于球场中央
    p->preball.y = (FTOP + FBOT) / 2.0;
    p->preball.z = 0;

    p->ballspeed.x =0;
    p->ballspeed.y =0;
    p->ballspeed.z =0;
}
extern "C" STRATEGY_API void Strategy ( Environment *env )//程序每周期调用一次
//////////////适用于  !!黄队
{
   Mydata * p;
    p=(Mydata *)env->userData;

    if(!p->locked)		// 是 判断场地了 ??
    {
        //确定区域,blue or yellow
        if( env->home[0].pos.x < 50.0 )
            p->mygrand=true; /// 是 = 黄队??
        else
            p->mygrand=false;
        p->locked=true;
    }

    See(env);			// 预处理

    MyGame(env);		//策略

    End ( env );		//后期处理

	int testInt = 100;
	int nIndex  = 0;

	switch (env->gameState)
	{
		case 0:
			{
				// 缺省情况下的策略


			}
			break;

		case FREE_BALL:
			{

			}
			break;

		case PLACE_KICK:
			{
				// 发生坠球时的策略
			}
			break;

		case PENALTY_KICK:

			// 发生罚球时的策略
			switch (env->whosBall)
			{
				case ANYONES_BALL:

					break;
				case BLUE_BALL:

					break;
				case YELLOW_BALL:

					break;
			}
			break;

		case FREE_KICK:
			{
				// 保存对方机器人的相关坐标
			}
			break;

		case GOAL_KICK:
			{
				// 发生点球时的策略
			}
			break;
  }
}

extern "C" STRATEGY_API void Destroy ( Environment *env )
{
}

