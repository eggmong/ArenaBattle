// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class UNREALARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

	FORCEINLINE class UBoxComponent* GetTrigger() { return Trigger; }

protected:
	// 액터의 세팅이 마무리 되는 시점에서 호출되는 함수 오버라이드하여 사용
	// -> 아이템 박스가 초기화 되었을 때, 에셋 매니저가 제공하고 있는 목록들을 살펴서
	// 이 중 하나를 랜덤으로 설정하려고
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;		// 루트 컴포넌트

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> Effect;

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemData> Item;
	// 부모 클래스(UABItemData) 로 만들어서, Weapon이나 Scroll이나 Potion이나 다 대응 가능
	// (ABWeaponItemData 등 아이템들이 UABItemData를 상속 받아 구현되어 있음!)

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	// UBoxComponent 에서 제공하는 델리게이트가 있음, OnComponentBeginOverlap 라고...
	// 이 델리게이트에 이 함수를 붙일것

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);
};
