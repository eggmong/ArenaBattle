// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 여기서 HUD 위젯을 스탯 값으로 업데이트 해준다.
}
