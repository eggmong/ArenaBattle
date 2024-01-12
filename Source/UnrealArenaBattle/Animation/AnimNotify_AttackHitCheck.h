// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AttackHitCheck.generated.h"

/**
 UAnimNotify 클래스의 함수들을 override 해서 구현해줘야 함.
 */
UCLASS()
class UNREALARENABATTLE_API UAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()
	

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
