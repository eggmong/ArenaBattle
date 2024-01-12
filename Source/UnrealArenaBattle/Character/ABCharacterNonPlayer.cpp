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
		FTimerDelegate::CreateLambda(	// () => {} 같이 C++ 람다
			[&]()						// 본문을 캡쳐해서 (&)
			{
				Destroy();
			}),
		DeadEventDelayTime,				// 상속받은거 5초
		false);							// Loop 할 필요 없으니까
}