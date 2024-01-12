// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
		FTimerDelegate::CreateLambda(	// () => {} ���� C++ ����
			[&]()						// ������ ĸ���ؼ� (&)
			{
				Destroy();
			}),
		DeadEventDelayTime,				// ��ӹ����� 5��
		false);							// Loop �� �ʿ� �����ϱ�
}