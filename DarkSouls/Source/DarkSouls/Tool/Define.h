#pragma once

#define COLTAG_NOMANAGE TEXT("NoManage")
#define COLTAG_NOBLOCK TEXT("NoBlock")
#define COLTAG_ONLYATTACK TEXT("OnlyAttack")

#define COL_ATTACK TEXT("AttackCreatureCol")
#define COL_BLOCK TEXT("BlockCreatureCol")
#define COL_NONBLOCK TEXT("NonBlockCreatureCol")

#define CCHANNEL_CREATURE ECC_GameTraceChannel1
#define CCHANNEL_LOCKTARGET ECC_GameTraceChannel2



#define PRINT_TEXT(X) 		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(X));

