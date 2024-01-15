// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"

// Sets default values
AABItemBox::AABItemBox()
{
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

    RootComponent = Trigger;
    Mesh->SetupAttachment(Trigger);
    Effect->SetupAttachment(Trigger);

    Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);		// ABCollision 에서 정의한거
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));		// 사이즈 지정
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
	// OnComponentBeginOverlap -> FComponentBeginOverlapSignature 들어가보면,
	// DECLARE_DYNAMIC~ 이렇게 다이나믹 델리게이트 확인할 수 있음. DYNAMIC : 블루프린트 지원한다는 거
	// 그래서 델리게이트에 등록하려는 함수는 UFUNCTION() 지정을 해줘야 한다.
	// 그리고 AddDynamic 으로 써야한다.

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));			// 상자는 보여주기만 하고 충돌체크 안하도록 NoCollision (Trigger 에서 구현할거니까...)

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;							// 바로 이펙트 발동되지 않도록 auto 끄기
	}
}

void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Effect->Activate(true);					// SetActive(true)
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);			// 또 충돌 발생하면 안되니까 false

	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
	// OnSystemFinished 이펙트 종료 되면 발생하는 델리게이트
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}