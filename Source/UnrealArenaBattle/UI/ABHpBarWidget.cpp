// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"			// 이 헤더는 UMG 모듈에서 제공하고 있음.
											// 근데 이 모듈은 기본적으로 포함이 안되어 있음.
											// 그래서 빌드설정에 추가해야 함. (UnrealArenaBattle.Build.cs)
											// "UMG"



UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pHpBar")));
	// WBP_HpBar 위젯 블루프린트에 ProgressBar 이름을 pHpBar 라고 지정해놨었음!

	ensure(HpProgressBar);
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
