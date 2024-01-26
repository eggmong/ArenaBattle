// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogABPlayerController, Log, All);

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	AABPlayerController();

	// GameScoreChanged, Over, Clear 함수가 호출될 때 C++코드에서 UI 띄우는 건 비효율적.
	// 그래서 블루프린트로 호출되게 할거임.

	// UFUNCTION : 블루프린트와 호환되도록 하는 키워드. 함수를 코드에서 구현하지 않아도 엔진에서 이벤트라고 감지해서 본문을 만들어 줌
	// K2 : 블루프린트의 전신인 Kismet 이라는 기능을 나타내는 접두사. 관습적으로 쓰고 있다고 함.
	// BlueprintImplementableEvent : 블루프린트에서 이벤트를 발동시키는 기능을 수행하려면 이 지정자를 넣어줘야 함.
	// Meta = (DisplayName = "") : 블루프린트에서 보여질 함수명을 여기서 작성
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameRetryCountCpp"))
	void K2_OnGameRetryCount(int32 NewRetryCount);		// 재시작 하는 이벤트 생성

	void GameScoreChanged(int32 NewScore);
	void GameOver();
	void GameClear();
	
protected:
	virtual void BeginPlay() override;


protected:
	// HUD Section

	// 위젯을 생성할 때 클래스 정보를 넘겨야 하기 때문에 클래스 변수 생성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UABHUDWidget> ABHUDWidgetClass;

	// 위젯의 포인터를 담을 변수도 생성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UABHUDWidget> ABHUDWidget;

protected:
	// Save Game Section
	
	// 해당 객체가 메모리에서 관리되도록 변수 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UABSaveGame> SaveGameInstance;
};
