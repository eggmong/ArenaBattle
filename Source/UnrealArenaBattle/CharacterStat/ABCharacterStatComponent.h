// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
// Hp가 0일 때 너 죽었다고 신호 보내는 델리게이트

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
// 변경된 현재 Hp값을 구독한 객체들에게 보내고, 인자값은 하나(OneParam)

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FABCharacterStat& /*BaseStat*/, const FABCharacterStat& /*ModifierStat*/);
// 변경된 BaseStat 값과 ModifierStat 값을 구독한 객체들에게 보낸다

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;


public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InNewLevel);			// 레벨 설정
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }

	// 스크롤 습득하여 BaseStat 변경
	FORCEINLINE void AddBaseStat(const FABCharacterStat& InAddBaseStat)
	{
		BaseStat = BaseStat + InAddBaseStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}

	FORCEINLINE void SetBaseStat(const FABCharacterStat& InBaseStat)
	{
		BaseStat = InBaseStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}

	FORCEINLINE void SetModifierStat(const FABCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}

	FORCEINLINE const FABCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FABCharacterStat& GetModifierStat() const { return ModifierStat; }

	FORCEINLINE FABCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	//FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	// 포션 습득하여 HP 충전
	FORCEINLINE void HealHp(float InHealAmount)
	{
		CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp);
		OnHpChanged.Broadcast(CurrentHp);
	}

	float ApplyDamage(float InDamage);


protected:
	void SetHp(float NewHp);

	//UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	//float MaxHp;
	// 기본 스탯으로 바뀌기 때문에 주석 처리

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentHp;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	// 기본 스탯
	// 캐릭터 스탯은 현재 레벨을 기반으로 게임 싱글톤으로부터 스탯 데이터를 받아오게 됨
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	// 아이템을 먹어서 더해지거나 빠지는 스탯
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;
};
