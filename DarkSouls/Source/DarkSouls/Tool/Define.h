#pragma once

#define COLTAG_NOMANAGE TEXT("NoManage") // Col 매니저에서 관리 x
#define COLTAG_NOBLOCK TEXT("NoBlock") // Block 함수 호출 시 변경 x
#define COLTAG_ONLYATTACK TEXT("OnlyAttack") //  

#define COL_ATTACK TEXT("AttackCreatureCol")
#define COL_BLOCK TEXT("BlockCreatureCol")
#define COL_NONBLOCK TEXT("NonBlockCreatureCol")

#define CCHANNEL_CREATURE ECC_GameTraceChannel1
#define CCHANNEL_LOCKTARGET ECC_GameTraceChannel2



#define PRINT_TEXT(X) 		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(X));

