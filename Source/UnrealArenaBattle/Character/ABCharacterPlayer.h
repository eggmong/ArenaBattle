// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "ABCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API AABCharacterPlayer : public AABCharacterBase
{
	GENERATED_BODY()
	

public:
	AABCharacterPlayer();

protected:
	// Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	// 카메라를 지탱해주는 지지대 역할을 하는 컴포넌트
	// Meta = (AllowPrivateAccess = "true") : private으로 선언된 언리얼 오브젝트의 객체를
	// 블루프린트에서도 접근할 수 있도록 만들어주는 지시어

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	// 실제 카메라 컴포넌트
};
