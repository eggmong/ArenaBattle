// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/ABAnimationAttackInterface.h"


// USkeletalMeshComponent : 애니메이션을 관리
// UAnimSequenceBase : 애니메이션 정보
// FAnimNotifyEventReference : 추가적인 정보
void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		//MeshComp->GetOwner();
		// 메쉬의 오너가 플레이어 캐릭터라면, 공격을 판정하라고 명령 내리기
		// 이렇게 캐릭터를 바로 사용하려면 #include "Character/ABCharacterBase.h" 를 해줘야 하는데,
		// 헤더를 include 한다는 건 의존성이 생긴다는 것
		// => 인터페이스를 만들어서 사용해라
		// ABAnimationAttackInterface
		// 그래서 위 코드 지우고, 아래에서 다시 구현
		
		IABAnimationAttackInterface* AttackPawn = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
