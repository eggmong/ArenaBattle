// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "Interface/ABCharacterItemInterface.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterBase.generated.h"


// 로그 카테고리 선언 (LogTemp 처럼)
DECLARE_LOG_CATEGORY_EXTERN(LogABCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};


DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemData* /*InItemData*/);
// 아이템을 처리할 델리게이트 선언
// 배열로 관리를 하려고 하는데, 쉬운 방법은 구조체를 만들어 감싸는 것? 이라고 한다

USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	
	FOnTakeItemDelegate ItemDelegate;
};


UCLASS()
class UNREALARENABATTLE_API AABCharacterBase : public ACharacter, public IABAnimationAttackInterface, public IABCharacterWidgetInterface, public IABCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

	virtual void PostInitializeComponents() override;		// 인스턴스 Setup을 종료하는 시점에서 BeginPlay가 실행되기 전인 PostInitializeComponents 에서
															// Stat의 델리게이트에, 죽었을 때 죽는 모션을 수행하도록 처리하는 함수 등록
															// (액터의 생명주기 문서 참고)
															// BeginPlay에서 구현할 수도 있고 생성자에서 바인딩 할 수도 있지만
															// 이런 함수가 있다는 거 알려고 여기서 구현


protected:
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

protected:
	// Combo Action Section (for 몽타주)

	// 몽타주 만든거 불러오려고 만든 변수 (AM_ComboAttack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// 데이터에셋 만든거 불러오려고 만든 변수 (ABA_ComboAttack)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	void ProcessComboCommand();

	// 몽타주가 시작될 때 호출하는 함수
	void ComboActionBegin();

	// 몽타주가 끝날 때 호출, 얘는 몽타주에 설정된 델리게이트를 통해 호출되도록 파라미터 줬음
	// UAnimMontage 클래스 들어가보면 DECLARE_DELEGATE_TwoParams( FOnMontageEnded, class UAnimMontage*, bool /*bInterrupted*/) 
	// 저거랑 똑같이 맞춘것
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// 공격이 끝나는 지점을 파악하기 위해 새로 생성 (ComboActionEnd 함수도 있긴 하지만, 이건 몽타주가 연계되어있어서..)
	virtual void NotifyComboActionEnd();

	// 타이머를 발동 시킬 함수
	void SetComboCheckTimer();

	// 타이머가 발동 되면 입력이 들어왔는지 체크하는 함수
	void ComboCheck();



	// 현재 콤보가 어디까지 진행됐는지 파악하려고
	int32 CurrentCombo = 0;
	
	// 언리얼 엔진에서 제공하는 타이머 함수
	FTimerHandle ComboTimerHandle;

	// 타이머 이전에 입력 커맨드가 들어왔는지 체크
	bool HasNextComboCommand = false;


	
protected:
	// Attack Hit Section
	virtual void AttackHitCheck() override;					// IABAnimationAttackInterface 에서 상속받은 순수가상함수

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// AActor에 있음 (ACharacter 의 부모, 부모, 부모)


protected:
	// Dead Section

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;


protected:
	// Stat Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> Stat;


protected:
	// UI Widget Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget) override;			// IABCharacterWidgetInterface 에서 상속받은 순수가상함수


protected:
	// Item Section

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;
	// 무기 먹었을 때 손에 쥐어지도록 하기 위해, 무기를 담을 스켈레탈 메쉬 컴포넌트 변수 생성

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;
	// 위에서 선언했던 FOnTakeItemDelegate 델리게이트를 감싸는 FTakeItemDelegateWrapper 라는 구조체를 배열로 만듦
	// Weapon, Potion, Scroll 아이템 효과 구현 할거라 3가지 함수 바인딩 할 것

	virtual void TakeItem(class UABItemData* InItemData) override;

	virtual void DrinkPotion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);


public:
	// Stat Section

	int32 GetLevel();
	void SetLevel(int32 InNewLevel);

	void ApplyStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
};
