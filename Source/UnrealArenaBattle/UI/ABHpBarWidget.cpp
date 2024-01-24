// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"			// 이 헤더는 UMG 모듈에서 제공하고 있음.
											// 근데 이 모듈은 기본적으로 포함이 안되어 있음.
											// 그래서 빌드설정에 추가해야 함. (UnrealArenaBattle.Build.cs)
											// "UMG"
#include "Components/TextBlock.h"
#include "Interface/ABCharacterWidgetInterface.h"


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

	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	// UABUserWidget 를 상속 받았으므로 OwningActor 변수 사용 가능
	// 이 액터 정보에다가 UpdateHpBar 함수 정보를 전달해서
	// 델리게이트에 등록하면, 스탯이 업데이트 될 때 마다 함수가 호출되어 HpBar 변화
	// 
	// 그런데 ABCharacterBase 클래스를 직접적으로 참조하면, 의존성이 발생하게 됨.
	// 위젯 컴포넌트들은 캐릭터와 무관하게 다양하게 적용되도록 설계하는 게 좋음.
	// 그래서 interface 선언하여 전달할 것.

	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	// OwningActor 가 IABCharacterWidgetInterface 로 형변환이 되는지 확인
	// ABCharacterBase 가 인터페이스 상속 받았으니 형변환 가능

	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UABHpBarWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UABHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}
