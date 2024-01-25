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

void AABPlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AABPlayerController::GameOver()
{
	K2_OnGameOver();
}

void AABPlayerController::GameClear()
{
	K2_OnGameClear();
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

		// 엔진의 초기화 순서 상
		// ABCharacterStatComponent 의 InitializeComponent 함수가 먼저 호출되면서 (컴포넌트의 InitializeComponent 함수가 먼저 호출됨)
		// 플레이어 스탯이 초기화 되고 (확정 되고),
		// 이후 ABPlayerController인 이 액터의 BeginPlay 함수에서 이 AddToViewport 가 호출되면서 NativeConstruct 가 실행됨.
		// 그러면 ABHUDWidget 의 OnNativeConstruct 가 실행 된다.
	}
}
