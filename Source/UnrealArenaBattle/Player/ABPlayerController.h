// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	AABPlayerController();
	
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
};
