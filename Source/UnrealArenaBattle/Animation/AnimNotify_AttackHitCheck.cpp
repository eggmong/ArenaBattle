// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/ABAnimationAttackInterface.h"


// USkeletalMeshComponent : �ִϸ��̼��� ����
// UAnimSequenceBase : �ִϸ��̼� ����
// FAnimNotifyEventReference : �߰����� ����
void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		//MeshComp->GetOwner();
		// �޽��� ���ʰ� �÷��̾� ĳ���Ͷ��, ������ �����϶�� ��� ������
		// �̷��� ĳ���͸� �ٷ� ����Ϸ��� #include "Character/ABCharacterBase.h" �� ����� �ϴµ�,
		// ����� include �Ѵٴ� �� �������� ����ٴ� ��
		// => �������̽��� ���� ����ض�
		// ABAnimationAttackInterface
		// �׷��� �� �ڵ� �����, �Ʒ����� �ٽ� ����
		
		IABAnimationAttackInterface* AttackPawn = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
