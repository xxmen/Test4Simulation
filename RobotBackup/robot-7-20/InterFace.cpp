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
extern "C" STRATEGY_API void Create ( Environment *env )//���ڽ�����ʼ���ĵ������򳡶�̬��������������
{
    //MessageBox(NULL,"�Ĵ���ѧ�����","SCU_KS",MB_OK);
    //��ʼ������
    env->userData=(void* ) new Mydata;//definition of Mydata in Interface.h
    Mydata* p;
    p=(Mydata*)env->userData;//p as an pointer
    p->a=fopen("c:\\output.txt","w");//using txt file to out put the instant data
    int i;
    for(i =0 ; i < 5; i++)//��ʼ��text�еļ�¼
    {


        p->robot[i].pos.x=InlitializeMyPosition_X[i];
        p->robot[i].pos.y=InlitializeMyPosition_Y[i];
        p->robot[i].pos.z=InlitializeMyPosition_Z[i];			//what is 'z'? zΪ����

        p->myoldpos[i].x=InlitializeMyPosition_X[i];
        p->myoldpos[i].y=InlitializeMyPosition_Y[i];
        p->myoldpos[i].z=InlitializeMyPosition_Z[i];			//this z is the direction that the robot faces

        p->myspeed[i].x=0;
        p->myspeed[i].y=0;
        p->myspeed[i].z=0;

        p->myoldvelocity[i].x=0;//�ϴε�������
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

    p->oldball.x = (FLEFTX + FRIGHTX) / 2.0;//���ϴ�λ��λ��������
    p->oldball.y = (FTOP + FBOT) / 2.0;
    p->oldball.z = 0;							//what is 'z' mean? the ball's speed ??

    p->curball.x = (FLEFTX + FRIGHTX) / 2.0;//��Ŀǰλ��λ��������
    p->curball.y = (FTOP + FBOT) / 2.0;
    p->curball.z = 0;

    p->preball.x = (FLEFTX + FRIGHTX) / 2.0;//����λ��λ��������
    p->preball.y = (FTOP + FBOT) / 2.0;
    p->preball.z = 0;

    p->ballspeed.x =0;
    p->ballspeed.y =0;
    p->ballspeed.z =0;
}
extern "C" STRATEGY_API void Strategy ( Environment *env )//����ÿ���ڵ���һ��
//////////////������  !!�ƶ�
{
   Mydata * p;
    p=(Mydata *)env->userData;

    if(!p->locked)		// �� �жϳ����� ??
    {
        //ȷ������,blue or yellow
        if( env->home[0].pos.x < 50.0 )
            p->mygrand=true; /// �� = �ƶ�??
        else
            p->mygrand=false;
        p->locked=true;
    }

    See(env);			// Ԥ����

    MyGame(env);		//����

    End ( env );		//���ڴ���

	int testInt = 100;
	int nIndex  = 0;

	switch (env->gameState)
	{
		case 0:
			{
				// ȱʡ����µĲ���


			}
			break;

		case FREE_BALL:
			{

			}
			break;

		case PLACE_KICK:
			{
				// ����׹��ʱ�Ĳ���
			}
			break;

		case PENALTY_KICK:

			// ��������ʱ�Ĳ���
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
				// ����Է������˵��������
			}
			break;

		case GOAL_KICK:
			{
				// ��������ʱ�Ĳ���
			}
			break;
  }
}

extern "C" STRATEGY_API void Destroy ( Environment *env )
{
}

