// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterStat.h"
#include "ABGameSingleton.generated.h"

/*
게임 데이터를 관리할 싱글톤 클래스 생성 및 등록
(등록 : Project Setting -> General Setting -> Default classes -> Advanced -> GameSingleton class 에 등록했음)
GameMode 클래스의 경우 멀티플레이를 포함한 전체 게임 내에서 단 하나만 존재하는 싱글톤 클래스
class 생성에서 GameInstance로 생성해도 되지만, GameInstance는 컨텐츠보단 어플리케이션의 데이터를 관리하는 용도로 많이 쓰임
그래서 컨텐츠 데이터를 관리하기 위해 UOBject 클래스로 생성해줬음
 */

DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All);

UCLASS()
class UNREALARENABATTLE_API UABGameSingleton : public UObject
{
	GENERATED_BODY()
	
	
public:
	UABGameSingleton();

	// 모든 클래스에서 이 인스턴스를 가져올 수 있도록 static으로 생성
	static UABGameSingleton& Get();

public:
	// Character Stat Data Section
	
	FORCEINLINE FABCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FABCharacterStat(); }
	// 레벨 정보를 넣으면 레벨에 해당하는 구조체 정보를 리턴
	// (IsValidIndex 로 유효한 인덱스인지 확인 후에)
	// 유효하지 않으면 기본 생성자를 만들어 넘김

	UPROPERTY()
	int32 CharacterMaxLevel;		// 총 몇 개의 레벨이 있는지 확인하는 용도


private:
	// 캐릭터 스탯들에 대한 테이블들을 내부적으로 보관해서 필요한 게임 객체들에게 제공하는 기능 추가
	TArray<FABCharacterStat> CharacterStatTable;
};
