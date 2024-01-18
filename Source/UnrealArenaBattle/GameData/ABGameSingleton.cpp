// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	// 테이블 에셋에 접근해서 TArray 로 변환하는 작업

	// UDataTable 형태로 테이블을 가져왔음
	// (Key, Value) 쌍으로 구성되어 있음
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable'"));
	
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);

		// Transform 함수로 Value 값만 가져다 저장하여 쓰기로 함
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FABCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UABGameSingleton& UABGameSingleton::Get()
{
	// 싱글톤 그대로 반환
	// 이 싱글톤 클래스의 경우, 엔진이 초기화 될 때 바로 활성화 되도록 설정을 했었음
	// (Project Setting -> General Setting -> Default Classes ... GameSingleton)
	// CastChecked 로 검사해준 뒤, GEngine->GameSingleton 로 접근할 수 있음
	// 설정해놨어서 엔진이 멤버 변수로 할당해줌
	UABGameSingleton* Singleton = CastChecked<UABGameSingleton>(GEngine->GameSingleton);

	if (Singleton)
	{
		return *Singleton;
	}
	
	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game SIngleton"));

	return *NewObject<UABGameSingleton>();
	// 그냥 함수 흐름을 위해 리턴할 뿐, 사용할 순 없음.
}
