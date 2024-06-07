#pragma once

#define COL_ATTACK TEXT("AttackCreatureCol")
#define COL_BLOCK TEXT("BlockCreatureCol")
#define CCHANNEL_LOCKTARGET ECC_GameTraceChannel2



#define PRINT_TEXT(X) 		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s"), X))