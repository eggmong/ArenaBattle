// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ABGameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UABGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALARENABATTLE_API IABGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	// 스코어가 변경될 때 호출
	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) = 0;
	
	// 플레이어가 죽었을 때 호출
	virtual void OnPlayerDead() = 0;

	// 현재 게임이 클리어됐는지 확인하는 함수
	virtual bool IsGameCleared() = 0;
};
