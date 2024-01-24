// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UABHUDWidget(const FObjectInitializer& ObjectInitializer);


public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);	// Stat UI에 기본스탯, 추가스탯 다 출력할거니까 둘 다 인자로 받음
	void UpdateHpBar(float NewCurrentHp);


protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UABHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UABCharacterStatWidget> CharacterStat;


};
