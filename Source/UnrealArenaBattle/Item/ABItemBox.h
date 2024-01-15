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


protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;		// 루트 컴포넌트

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> Effect;

	UPROPERTY(EditAnywhere, Category = "Item")
	TObjectPtr<class UABItemData> Item;
	// 부모 클래스(UABItemData) 로 만들어서, Weapon이나 Scroll이나 Potion이나 다 대응 가능

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	// UBoxComponent 에서 제공하는 델리게이트가 있음, OnComponentBeginOverlap 라고...
	// 이 델리게이트에 이 함수를 붙일것

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);
};
