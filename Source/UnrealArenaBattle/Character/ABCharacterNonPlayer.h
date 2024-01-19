// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/ABCharacterAIInterface.h"
#include "ABCharacterNonPlayer.generated.h"


// Config 폴더의 DefaultUnrealArenaBattle.ini 를 사용하겠다는 것
UCLASS(Config=UnrealArenaBattle)
class UNREALARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase, public IABCharacterAIInterface
{
	GENERATED_BODY()
	

public:
	AABCharacterNonPlayer();

protected:
	// 여기서 NPCMeshes 로딩 진행하는걸로.
	virtual void PostInitializeComponents() override;

protected:
	// 상속받은 SetDead 함수 재구현
	void SetDead() override;
	
	// 로딩이 완료되었을 때 호출될 함수
	void NPCMeshLoadCompleted();

	// 해당 Config 파일로부터 데이터를 불러오겠다는 의미
	// Config 폴더의 ini 파일과 같은 NPCMeshes 을 변수 이름으로 사용해야 함
	// 프로젝트가 로딩될 때 자동으로 값이 채워질 것
	UPROPERTY(Config)
	TArray<FSoftObjectPath> NPCMeshes;

	// 위의 오브젝트 로딩을 비동기로 진행하기 위해 Handle 구조체 선언
	TSharedPtr<FStreamableHandle> NPCMeshHandle;


protected:
	
	// AI Section
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
};
