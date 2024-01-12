// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	

public:
	UABComboActionData();

	// 몽타주 섹션 이름 지정
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	// 콤보 총 갯수
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	// 프레임 재생 속도
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	// 입력이 사전에 입력됐는지 감지하는 프레임을 각각 지정
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
