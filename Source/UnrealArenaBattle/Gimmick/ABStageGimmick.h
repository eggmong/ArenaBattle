// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"



DECLARE_DELEGATE(FOnStageChangedDelegate);
// 스테이지 상태 전환을 관리할 델리게이트
// 준비, 시작, 보상선택, 다음 총 4가지이므로 열거형으로 관리하겠다
// 그래서 USTRUCT로 델리게이트를 감싸겠다 (eg. FTakeItemDelegateWrapper)

USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()
	FStageChangedDelegateWrapper() {}
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InStageDelegate) : StageDelegate(InStageDelegate) {}

	FOnStageChangedDelegate StageDelegate;
};



UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT
};



UCLASS()
class UNREALARENABATTLE_API AABStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABStageGimmick();


protected:
	virtual void OnConstruction(const FTransform& Transform) override;			// 에디터에서 속성 값들을 변경할 때 연동된 것들이 같이 시뮬레이션 되도록 설정할 수 있는 함수



protected:
	// Stage Section

	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage;
	// 루트 컴포넌트에 해당, 스테이지를 담을 변수

	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;
	// 플레이어가 들어왔다는 걸 감지할 BoxComponent

	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 트리거 감지할 함수


protected:
	// Gate Section

	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;
	// Gate는 맵마다 4개가 필요하기 때문에, TMap 으로 타입 지정 (고유한 Key값을 전달하여 문 세팅할 것)

	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;
	// 각가의 문 마다 배치할 트리거를 배열로 관리

	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenAllGates();
	void CloseAllGates();


protected:
	// State Section

	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;			// 현재 스테이지의 상태 저장

	void SetState(EStageState InNewState);

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;
	// 델리게이트를 감싼 구조체를 상태별로 

	// 델리게이트에 바인딩할 함수들
	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();


protected:
	// Fight Section

	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABCharacterNonPlayer> OpponentClass;					// 싸울 상대 npc 담을 변수
	// TSubclassOf : 언리얼 엔진이 제공하는 템플릿
	// AABCharacterNonPlayer 클래스를 상속받은 클래스들만 변수에 담을 수 있도록!

	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;												// npc 등장 딜레이 타임 변수

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);						// npc가 죽으면 보상 단계로 진행되어야 하기 때문에, 그걸 위한 함수

	FTimerHandle OpponentTimerHandle;
	void OnOpponentSpawn();													
};
