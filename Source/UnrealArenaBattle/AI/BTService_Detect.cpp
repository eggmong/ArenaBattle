// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "ABAI.h"
#include "AI/ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");			// Service��� �̸��� Detect�� ��µǵ��� ��. 

	Interval = 1.0f;					// 1�� ������ �����ϵ��� ��.
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	// AIOwner�κ��� ���� �����ϰ� �ִ� Pawn�� ���� ������ �����´�.

	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	// Pawn �� ��ġ

	UWorld* World = ControllingPawn->GetWorld();
	// Pawn �� ���� ������ �� ��������

	if (nullptr == World)
	{
		return;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	// ������ ���� �ݰ� ��������
	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;			// ����Ƽ�� raycastResult ����.

	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	// FCollisionQueryParams ���� ������ ����,
	// InTraceTag : �±� ������ �м��� �� �ĺ��� ������ �Ǵ� ���� -> Detect
	// bInTraceComplex : ������ ������ �浹ü�� �������� ����
	// InIgnoreActor : ������ Actor��, �ڱ� �ڽŸ� �����ϸ� �Ǳ� ������ this�� �־���

	bool bResult = World->OverlapMultiByChannel(				// �÷��̾ �ټ� �ִٴ� �����Ͽ� Multi�� �������. �̷� ��� ����� TArray�� ����.
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			if (Pawn && Pawn->GetController()->IsPlayerController())		// Pawn�� �����ϰ� �ִ� ���� Player�� ���
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);	// BBKEY_TARGET, �� Ÿ���� Pawn(=Player)�� ����
				
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);	// Ž�� ������ ������� ����

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);	// �÷��̾�� npc�� ���̸� ���� ������ ǥ��

				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);		// ��ã�ų� Pawn ���� Player�� �ƴѰ�� Ÿ���� null
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);			// Ž�� ������ ���� ������ ����
}
