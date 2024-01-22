// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	AABAIController();
	
	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;				// BlackBoard 에셋 담을 변수 (BB_ABCharacter)

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;				// 행동트리 에셋 (BT_ABCharacter)


};
