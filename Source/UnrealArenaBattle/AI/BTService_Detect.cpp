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
	NodeName = TEXT("Detect");			// Service노드 이름을 Detect로 출력되도록 함. 

	Interval = 1.0f;					// 1초 단위로 수행하도록 함.
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	// AIOwner로부터 지금 제어하고 있는 Pawn에 대한 정보를 가져온다.

	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	// Pawn 의 위치

	UWorld* World = ControllingPawn->GetWorld();
	// Pawn 이 속한 월드의 값 가져오기

	if (nullptr == World)
	{
		return;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	// 감지할 영역 반경 가져오기
	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;			// 유니티의 raycastResult 같은.

	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	// FCollisionQueryParams 들어가서 생성자 보면,
	// InTraceTag : 태그 정보로 분석할 때 식별자 정보가 되는 인자 -> Detect
	// bInTraceComplex : 복잡한 형태의 충돌체도 감지할지 말지
	// InIgnoreActor : 무시할 Actor들, 자기 자신만 무시하면 되기 때문에 this를 넣었음

	bool bResult = World->OverlapMultiByChannel(				// 플레이어가 다수 있다는 가정하에 Multi를 사용했음. 이럴 경우 결과가 TArray로 들어옴.
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

			if (Pawn && Pawn->GetController()->IsPlayerController())		// Pawn을 조종하고 있는 것이 Player일 경우
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);	// BBKEY_TARGET, 즉 타겟을 Pawn(=Player)로 설정
				
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);	// 탐색 영역을 녹색으로 설정

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);	// 플레이어와 npc의 사이를 선과 점으로 표시

				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);		// 못찾거나 Pawn 값이 Player가 아닌경우 타겟을 null
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);			// 탐색 영역을 붉은 색으로 설정
}
