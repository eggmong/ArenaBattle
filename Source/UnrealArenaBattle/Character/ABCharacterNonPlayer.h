// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase
{
	GENERATED_BODY()
	

public:
	AABCharacterNonPlayer();

protected:
	// 상속받은 SetDead 함수 재구현
	void SetDead() override;
};
