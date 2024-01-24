// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/ABCharacterHUDInterface.h"
#include "ABCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API AABCharacterPlayer : public AABCharacterBase, public IABCharacterHUDInterface
{
	GENERATED_BODY()
	

public:
	AABCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	// 입력 매핑 컨텍스트를 진행하는 역할 (DefaultMappingContext)

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 언리얼 엔진의 InputSystem에서 밑에 선언한 Move, Look 함수를 서로 매핑시켜주는 것을
	// 여기서 완료해주도록 구현할 것

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


	
protected:
	// Input Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> AttackAction;

	// 다른 에셋으로 변경할 수 있도록 설계하기 위해서
	// 지정자를 EditAnywhere 로 지정

	// FInputActionValue 를 사용하려면
	// InputActionValue.h include 하고
	// Build.cs에서 "EnhancedInput" 라는 모듈을 추가해줘야 함
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	void Attack();

	ECharacterControlType CurrentCharacterControlType;

protected:
	// Character Control Section
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData) override;

	// v키를 눌렀을 때 컨트롤타입 변경하기 위한 함수
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);


protected:
	// UI Section
	virtual void SetupHUDWidget(class UABHUDWidget* InHUDWidget);
};
