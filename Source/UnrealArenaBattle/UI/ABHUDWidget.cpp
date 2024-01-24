// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"
#include "Interface/ABCharacterHUDInterface.h"
#include "ABHpBarWidget.h"
#include "ABCharacterStatWidget.h"

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UABHUDWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	// 스탯이 변경될 땐 MaxHP 값이 바뀌기 때문에 (증가하기 때문에)
	// BaseStat 과 추가 스탯을 합친게 Max가 된다.
	//FABCharacterStat TotalStat = BaseStat + ModifierStat;
	//HpBar->SetMaxHp(TotalStat.MaxHp);

	HpBar->UpdateStat(BaseStat, ModifierStat);

	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UABHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 여기서 HUD 위젯을 스탯 값으로 업데이트 해준다.

	HpBar = Cast<UABHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));			// WBP_ABHUD 에서 지정한 UI 이름으로 찾기
	ensure(HpBar);

	CharacterStat = Cast<UABCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	// 그리고 인터페이스를 사용해서, 이 위젯을 사용하고 있는 캐릭터에 명령을 보낼 것

	IABCharacterHUDInterface* HUDPawn = Cast<IABCharacterHUDInterface>(GetOwningPlayerPawn());
	// GetOwningPlayerPawn 함수 : HUD를 소유하는 컨트롤러가 빙의하고 있는 pawn을 가져올 수 있음
	// GetOwningPlayer : HUD를 소유하는 컨트롤러 가져오기

	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
