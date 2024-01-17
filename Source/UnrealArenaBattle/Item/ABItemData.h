// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemData.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll
};


/**
 * 
 */
UCLASS()
class UNREALARENABATTLE_API UABItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// 부모클래스인 UPrimaryDataAsset 클래스 들어가보면 GetPrimaryAssetId 라는 함수가 있음
	// 생성될 asset의 아이디를 직접 지정해줄 수 있는 함수
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());
		// 태그 이름은 ABItemData 가 되고, 이름은 에셋의 이름으로 지정했음
		// 이렇게 2가지 정보를 사용하여 유일한 식별자 아이디 값을 만들어냈음
	}

	// 왜 override 하여 아이디를 지정해줬냐면,
	// Project Setting -> Game/Asset Manager 라고 싱글턴 클래스가 있음
	// 엔진이 초기화될 때 반드시 활성화되는, 단 하나만 존재하는 인스턴스
	// 여기에 PrimaryAssetType 에서 ABItemData 를 관리하도록 지정해서, Asset Manager로부터 모든 에셋의 목록을 불러오도록 설정했음
	// 그래서 asset 아이디들을 ABItemData 로 지정한것..
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	EItemType Type;


protected:

};
