// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작하자마자 마우스 커서가 뷰포트 안으로 들어가도록
	// 플레이 중에 마우스 나타나게 하고 싶으면 Shift+F1
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
