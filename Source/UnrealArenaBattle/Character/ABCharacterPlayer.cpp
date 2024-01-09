// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);		// SpringArm 을 루트 컴포넌트에 부착
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	// 카메라 지지대인 SpringArm에 부착
	// 특정 포지션 값이 아니라
	// Socket 이라는 이름지시자를 지정해주면
	// SpringArm의 끝에 자동으로 달라붙음
	FollowCamera->bUsePawnControlRotation = false;
}
