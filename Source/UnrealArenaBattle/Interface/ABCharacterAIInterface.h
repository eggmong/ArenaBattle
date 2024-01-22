// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ABCharacterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UABCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};


DECLARE_DELEGATE(FAICharacterAttackFinished);

class UNREALARENABATTLE_API IABCharacterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 앞으로 npc가 의무적으로 가져야 할, 기본적인 데이터를 얻어오는 함수 4가지
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	// 공격의 경우엔 바로 끝나는 액션이 아님
	// 공격이 시작되고, 몽타주가 재생되고, 몽타주가 끝나야 공격이 끝났다고 볼 수 있음
	// 그래서 공격 명령을 내렸다고해서 바로 성공했다고 리턴 할 수 없음
	// 그래서 AttackByAI 호출해 준 다음에
	// 리턴값을 지정할 때, InProgress로 설정 해주고,
	// 공격이 끝난 이후에 Succeded 값으로 반환해주면 됨
	// 공격이 끝난 것을 캐치하기 위한 델리게이트 -> FAICharacterAttackFinished
	virtual void AttackByAI() = 0;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
};
