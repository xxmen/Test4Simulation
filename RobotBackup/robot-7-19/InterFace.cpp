#include <windows.h>
#include "InterFace.h"
#include "Strategy.h"

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{    
    return TRUE;
}

extern "C" STRATEGY_API void Create ( Environment *env )
{
}

extern "C" STRATEGY_API void Destroy ( Environment *env )
{
}

extern "C" STRATEGY_API void Strategy ( Environment *env )
{
	int testInt = 100;
	int nIndex  = 0;

	switch (env->gameState)
	{
		case 0:
			{
				// ȱʡ����µĲ���
				// ���������˽���׷��Է��Ĳ���

				MoonFollowOpponent ( &env->home [1], &env->opponent [2] );
				MoonFollowOpponent ( &env->home [2], &env->opponent [3] );
				MoonFollowOpponent ( &env->home [3], &env->opponent [4] );
				
				// ���ĸ�����
				MoonAttack ( &env->home [4], env );
				// ����Ա����
				Goalie1 ( &env->home [0], env );

			}				
			break;

		case FREE_BALL:
			{
				// ����Է�������
				MoonFollowOpponent ( &env->home [1], &env->opponent [2] );
				MoonFollowOpponent ( &env->home [2], &env->opponent [3] );
				MoonFollowOpponent ( &env->home [3], &env->opponent [4] );

				// ����
				MoonAttack ( &env->home [4], env );

				// ����Ա����
				Goalie1 ( &env->home [0], env );
			}
			break;

		case PLACE_KICK:
			{
				// ����׹��ʱ�Ĳ���
				// �ɶ���С������
				MoonAttack ( &env->home [2], env );
			}			
			break;	
			
		case PENALTY_KICK:

			// ��������ʱ�Ĳ���
			switch (env->whosBall)
			{
				case ANYONES_BALL:
					MoonAttack ( &env->home [1], env );
					break;
				case BLUE_BALL:
					MoonAttack ( &env->home [4], env );
					break;
				case YELLOW_BALL:
					MoonAttack ( &env->home [0], env );
					break;
			}
			break;

		case FREE_KICK:
			{
				// ����Է������˵��������
				FILE * pDebugFile = NULL; 
				pDebugFile = fopen("debugfile.txt", "a"); 
				
				for( nIndex = 0; nIndex < PLAYERS_PER_SIDE; nIndex++ ) 
				{
					fprintf( pDebugFile, "robot: %d x: %f y: %f z: %f \n", 
						nIndex, env->opponent[nIndex].pos.x, 
						env->opponent[nIndex].pos.y, 
						env->opponent[nIndex].pos.z ); 
				}		
				
				fclose(pDebugFile); 

				// ����������ʱ�Ĳ���
				MoonAttack ( &env->home [0], env );
			}
			break;

		case GOAL_KICK:
			{
				// ��������ʱ�Ĳ���
				MoonAttack ( &env->home [0], env );
			}			
			break;
  }
}