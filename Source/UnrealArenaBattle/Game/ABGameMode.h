// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/ABGameInterface.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API AABGameMode : public AGameModeBase, public IABGameInterface
{
	GENERATED_BODY()
	
public:
	AABGameMode();

	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

	// 현재 진행중인 게임의 클리어 조건 세팅 = 클리어 스코어
	// EditDefaultOnly -> 처음에 기본값으로 지정하도록 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 ClearScore;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Game)
	int32 CurrentScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Game)
	uint8 bIsCleared : 1;
};
