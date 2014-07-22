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
				// 缺省情况下的策略
				// 三个机器人进行追逐对方的策略

				MoonFollowOpponent ( &env->home [1], &env->opponent [2] );
				MoonFollowOpponent ( &env->home [2], &env->opponent [3] );
				MoonFollowOpponent ( &env->home [3], &env->opponent [4] );
				
				// 第四个抢球
				MoonAttack ( &env->home [4], env );
				// 守门员守门
				Goalie1 ( &env->home [0], env );

			}				
			break;

		case FREE_BALL:
			{
				// 跟随对方机器人
				MoonFollowOpponent ( &env->home [1], &env->opponent [2] );
				MoonFollowOpponent ( &env->home [2], &env->opponent [3] );
				MoonFollowOpponent ( &env->home [3], &env->opponent [4] );

				// 抢球
				MoonAttack ( &env->home [4], env );

				// 守门员守门
				Goalie1 ( &env->home [0], env );
			}
			break;

		case PLACE_KICK:
			{
				// 发生坠球时的策略
				// 由二号小车发球
				MoonAttack ( &env->home [2], env );
			}			
			break;	
			
		case PENALTY_KICK:

			// 发生罚球时的策略
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
				// 保存对方机器人的相关坐标
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

				// 发生任意球时的策略
				MoonAttack ( &env->home [0], env );
			}
			break;

		case GOAL_KICK:
			{
				// 发生点球时的策略
				MoonAttack ( &env->home [0], env );
			}			
			break;
  }
}