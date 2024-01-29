// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"
#include "ABSaveGame.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogABPlayerController);

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

	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		// 저장 실패하면 Error 로그를 띄우도록, 카테고리는 헤더에서 추가함
		UE_LOG(LogABPlayerController, Error, TEXT("Save Game Error!"));
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
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

	// 주석처리 한 이유 : UI 작업은 코드 보단 블루프린트에서 하는게 편해서
	// BP_ABPlayerController 블루프린트에서 BeginPlay 흐름에서 띄우도록 변경.
	//// 게임이 시작되면 HUD 위젯 생성
	//ABHUDWidget = CreateWidget<class UABHUDWidget>(this, ABHUDWidgetClass);
	//if (ABHUDWidget)
	//{
	//	// 뷰포트 화면에 출력하도록 호출
	//	ABHUDWidget->AddToViewport();

	//	// 엔진의 초기화 순서 상
	//	// ABCharacterStatComponent 의 InitializeComponent 함수가 먼저 호출되면서 (컴포넌트의 InitializeComponent 함수가 먼저 호출됨)
	//	// 플레이어 스탯이 초기화 되고 (확정 되고),
	//	// 이후 ABPlayerController인 이 액터의 BeginPlay 함수에서 이 AddToViewport 가 호출되면서 NativeConstruct 가 실행됨.
	//	// 그러면 ABHUDWidget 의 OnNativeConstruct 가 실행 된다.
	//}

	// LoadGameFromSlot : 저장한 게임이 있는지 확인하고 로드, Player0 : 파일 이름, 싱글 게임의 경우 UserIndex는 항상 0.
	SaveGameInstance = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (!SaveGameInstance)
	{
		SaveGameInstance = NewObject<UABSaveGame>();
		SaveGameInstance->RetryCount = 0;
	}

	SaveGameInstance->RetryCount++;

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}
