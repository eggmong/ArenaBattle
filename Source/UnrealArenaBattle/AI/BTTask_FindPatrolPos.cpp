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

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();				// �ൿƮ���� ������ ������Ʈ�� ����, �� AIController�� ��ӹ��� ������ �ν��Ͻ�
																			// �� �ν��Ͻ��� � Pawn�� �����ϰ� �ִٸ� GetPawn �Լ��� ������ �� ����
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// �������� �Ǹ鼭 V1�̶�� �̸��� �ڿ� �ٰ� �� ��.
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// �������̽� ��� �޾Ƽ� ���� �Ǿ��ִ��� Cast�� Ȯ��
	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// �������� �� ��������
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);

	FNavLocation NextPatrolPos;

	float PatrolRadius = AIPawn->GetAIPatrolRadius();

	// BBKEY_HOMEPOS �� ����� Origin ���� ���������� �Ѱܼ� ���� ���� NextPatrolPos �� �޾ƿ��� ��.
	// �ݰ� ���� ������ ��ġ�� ã�� �Ǹ� true�� ��ȯ��.
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
