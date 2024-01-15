// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API UABUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;			// 현재 유저 위젯을 소유하고 있는 액터의 정보 보관
};
