// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABUserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;
	// HpProgressBar 포인터를 가져와야 해서, UI 기능 초기화를 하는 이 함수에서 포인터를 넘겨주도록 구현

public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);


public:
	//FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
