#include <string.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#include "basic.h"
#include "tools.h"
#include "action.h"
#include "strategy.h"
#include "mygame.h"
#include "myfunction.h"


void NormalGame ( Environment *env )
{
    Mydata * p;
    p=(Mydata *)env->userData;
    if(p->curball.x > (FLEFT+FRIGHT)/2)
    {
        Attack(env);
        AttFoul(env);

    }
    else
    {
        Defence(env);
        DefFoul(env);
    }

    MyGoalie(env,0);

}
void FreeBallGame(Environment *env)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    NormalGame(env);

}

void PlaceBallGame(Environment *env)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    NormalGame(env);
}

void PenaltyBallGame(Environment *env)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    /*bool flag1=true;
    while(flag1)
    {
    	Penalty (env);
    	if (p->curball.x < (FLEFT+FRIGHT)/2) flag1=false;
    }*/
    NormalGame(env);

}

void FreeKickGame(Environment *env)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    NormalGame(env);
}

void GoalKickGame(Environment *env)
{
    Mydata * p;
    p=(Mydata *)env->userData;
    NormalGame(env);
}


//////
/*MyGame  �Բ�ͬ��GameState���ò�ͬ�Ĳ��ԣ�
�����ڸ���״̬֮���л���ʱ�����˼���������Ϊ������״̬�ı䵽����״̬���жϱ�־
ÿ������״̬���м����� ע��ø�ʽ

*/
//

