// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget.generated.h"

// npc가 플레이어가 있는 방향으로 공격하도록 회전시키기
UCLASS()
class UNREALARENABATTLE_API UBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
	
public:
	UBTTask_TurnToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
