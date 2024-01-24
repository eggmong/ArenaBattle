// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이 위젯(Stat)이 초기화 될 때
	// FABCharacterStat 라는 구조체가 가지는 속성들의 값들을 모두 읽어와
	// 여기에 매칭되는 TextBlock의 포인터를 가져오도록 함

	// 구조체가 가지고 있는 속성들을 모두 뽑아주는 이터레이터 : TFieldIterator
	// 구조체의 정보는 StaticStruct 로 가져올 수 있음
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());		// 구조체 속성들의 이름 뽑기
 		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());	// WBP_CharacterStat 에서
																									// 항목별 TextBlock 의 이름들을 TxtMaxHpBase, TxtMaxHpModifier 이런식으로 지정했었음.

		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));		// TextBlock 에 대한 포인터를 가져옴
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UABCharacterStatWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);

		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

		/*
		* 실제로 데이터가 들어가있는 BaseStat 의 포인터를 넘겨주면, ((const void*)&BaseStat)
		* GetValue_InContainer 함수를 사용하여 해당 FNumericProperty(PropIt) 가 가지고 있는 값을 얻어올 수 있게 됨.
		* 얻어온 값을 BaseData 변수에 out 하여 저장.
		*/

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));

			// SanitizeFloat : float 값을 예쁘게... 후행 0이 제거된 부동 소수점 문자열을 변환. 예를 들어 1.234는 "1.234000"이 아닌 "1.234"로 변환
			// 변환한 FString을 다시 FText로 변환하여 SetText
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}
