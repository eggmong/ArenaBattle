// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UABAnimInstance();

protected:
	// AnimInstance가 처음 생성될 때 한 번 호출
	virtual void NativeInitializeAnimation() override;

	// 프레임마다 호출 (Update)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;		// AnimInstance를 소유하고 있는 캐릭터의 정보를 담는 객체타입 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;			// 캐릭터 무브먼트를 담는 객체타입 변수

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)	// 값 타입이라 EditAnywhere
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)	// 값 타입이라 EditAnywhere
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)	// 값 타입이라 EditAnywhere
	uint8 bIsIdle : 1;			// Idle인지 아닌지  (사이즈가 명확한 bool타입임. 언리얼은 이렇게 씀)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;		// 현재 상태를 계속 체크하기 위해, 움직이고 있는지 쉬고 있는지 체크하기 위한

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;		// 떨어지고 있는지 체크

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;		// 점프하고 있는지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;	// 현재 점프중인지...
};
