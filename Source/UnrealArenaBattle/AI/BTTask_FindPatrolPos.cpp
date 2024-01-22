// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "ABAI.h"
#include "AI/ABAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();				// 행동트리를 소유한 컴포넌트의 오너, 즉 AIController를 상속받은 오너의 인스턴스
																			// 이 인스턴스가 어떤 Pawn에 빙의하고 있다면 GetPawn 함수로 가져올 수 있음
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 버전업이 되면서 V1이라는 이름이 뒤에 붙게 된 것.
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 인터페이스 상속 받아서 구현 되어있는지 Cast로 확인
	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 블랙보드의 값 가져오기
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);

	FNavLocation NextPatrolPos;

	float PatrolRadius = AIPawn->GetAIPatrolRadius();

	// BBKEY_HOMEPOS 에 저장된 Origin 값과 반지름값을 넘겨서 얻은 값을 NextPatrolPos 에 받아오게 함.
	// 반경 내에 적당한 위치를 찾게 되면 true를 반환함.
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
