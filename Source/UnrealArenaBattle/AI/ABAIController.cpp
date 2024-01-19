// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ABAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ABAI.h"


AABAIController::AABAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/ArenaBattle/AI/BB_ABCharacter.BB_ABCharacter'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/ArenaBattle/AI/BT_ABCharacter.BT_ABCharacter'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AABAIController::RunAI()
{
	// AI가 구동될 땐 블랙보드로부터 블랙보드 컴포넌트를 가져와야 함

	// 블랙보드 포인터 가져오기
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	// 블랙보드 초기화 진행해서 성공하면,
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// SetValueAsVector 함수를 사용하여 홈 포지션(HomePos) 값을 시작할 때 할당하도록 함
		// AI가 빙의한 Pawn의 위치에 홈 포지션 값 설정
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// 행동트리 구동
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);		// 구동 안됐으면 Assertion 발생할 것
	}
}

void AABAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 폰이 빙의를 했을 때, AI 를 구동하도록
	RunAI();

}
