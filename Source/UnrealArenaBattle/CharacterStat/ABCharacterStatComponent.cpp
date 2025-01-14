﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	//MaxHp = 200.0f;
	//SetHp(MaxHp);

	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true;		// true 로 넣어줘야 InitializeComponent 함수 실행.
											// 모든 상속받은 클래스들이 InitializeComponent 를 실행해버리면 성능 문제가 있을 수 있으니까
}



void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);

	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	//BaseStat = UABGameSingleton::Get().GetCharacterStat(CurrentLevel);
	SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;

	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	// 만약 InDamage 값이 -값이 들어올 수도 있으니, 최소 데미지는 0으로 지정

	SetHp(PrevHp - ActualDamage);

	if (CurrentHp < KINDA_SMALL_NUMBER)
	{
		// 죽었다는 이벤트 활성화
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	// OnHpChanged 델리게이트에 등록한 개체들에게 메세지 날려주기
	OnHpChanged.Broadcast(CurrentHp);
}

