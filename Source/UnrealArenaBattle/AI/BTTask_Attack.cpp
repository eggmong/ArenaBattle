// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	// AIController가 현재 조종하고 있는 Pawn 을 가져와서

	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			// FinishLatentTask : 밑에서 InProgress 를 리턴하고, 이것이 마무리 되었다고 알려주는 함수
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);

	// 공격의 경우엔 바로 끝나는 액션이 아님
	// 공격이 시작되고, 몽타주가 재생되고, 몽타주가 끝나야 공격이 끝났다고 볼 수 있음
	// 그래서 공격 명령을 내렸다고해서 바로 성공했다고 리턴 할 수 없음
	AIPawn->AttackByAI();

	// 그래서 AttackByAI 호출해 준 다음에
	// 리턴값을 지정할 때, InProgress로 설정 해주고,
	// 공격이 끝난 이후에 Succeded 값으로 반환해주면 됨
	return EBTNodeResult::InProgress;


}
