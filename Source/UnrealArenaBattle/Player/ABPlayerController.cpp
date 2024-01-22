// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"

AABPlayerController::AABPlayerController()
{
	ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작하자마자 마우스 커서가 뷰포트 안으로 들어가도록
	// 플레이 중에 마우스 나타나게 하고 싶으면 Shift+F1
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// 게임이 시작되면 HUD 위젯 생성
	ABHUDWidget = CreateWidget<class UABHUDWidget>(this, ABHUDWidgetClass);
	if (ABHUDWidget)
	{
		// 뷰포트 화면에 출력하도록 호출
		ABHUDWidget->AddToViewport();
	}
}
