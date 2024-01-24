// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API UABCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);

private:
	// Base TextBlock들을 저장해둘 TMap
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	// Modifier TextBlock들을 저장해둘 TMap
	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;
};
