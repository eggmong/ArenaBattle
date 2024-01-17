// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"
#include "Item/ABItemBox.h"

// Sets default values
AABStageGimmick::AABStageGimmick()
{
	// Stage Section
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);			// ABCollision 에서 지정했던... ABTrigger... 폰만 감지할 수 있는 프로필
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnStageTriggerBeginOverlap);	// 정의 타고가보면 다이나믹 델리게이트라는 걸 알 수 있음. 그래서 AddDynamic


	// Gate Section
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));
	for (FName GateSocket : GateSockets)
	{
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);

		Gate->SetStaticMesh(GateMeshRef.Object);						// 문 리소스 하나 참조해서 for문으로 Gate마다 다 돌려씀

		Gate->SetupAttachment(Stage, GateSocket);						// Stage의 GateSocket 에다가 부착시킨다는 것
		Gate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		Gates.Add(GateSocket, Gate);									// 이름, 객체포인터를 넣어 Gates에 넣기

		FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);
		GateTrigger->ComponentTags.Add(GateSocket);						// 플레이어가 어느 트리거로 들어갔는지 식별하도록 컴포넌트에 태그 지정해둠

		GateTriggers.Add(GateTrigger);
	}


	// State Section
	CurrentState = EStageState::READY;

	StateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetReady)));
	StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetFight)));
	StateChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseReward)));
	StateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseNext)));
	

	// Fight Section
	OpponentSpawnTime = 2.0f;
	OpponentClass = AABCharacterNonPlayer::StaticClass();
	// 생성시킬 npc 클래스 지정


	// Reward Section
	RewardBoxClass = AABItemBox::StaticClass();		// 클래스 지정

	for (FName GateSocket : GateSockets)			// 위에 있던 게이트 소켓에 있는 값들을 참고해서 상자 spawn
	{
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}
}

void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 스테이지 트리거와 충돌
	// (플레이어가 스테이지에 들어왔다는 걸 감지)

	SetState(EStageState::FIGHT);
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 보상 상자를 열고 다음 스테이지로 넘어갈 때
	// 게이트 트리거와 충돌
	// 플레이어가 어느 게이트로 갔는지 감지

	check(OverlappedComponent->ComponentTags.Num() == 1);
	// 위에 생성자에서 for문 돌면서 Gate마다 하나씩 태그 달아줬었음. 1개를 초과한 태그갯수는 문제가 있는거임. 그걸 체크해봄.

	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// 태그 정보 가져옴

	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	// 맵의 기반이 되는 SM_SQUARE 열어보면 Socket Manager 볼 수 있음. 거기에 -XGate, +XGate... 다 있음.
	// 왼쪽 2개만 남김. +X

	check(Stage->DoesSocketExist(SocketName));
	// 해당 소켓 이름이 있는지 확인 (+X)

	FVector NewLocation = Stage->GetSocketLocation(SocketName);
	// 소켓 이름을 통해 소켓의 위치를 가져옴

	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTrigger), false, this);	// this : 자신을 제외하고 검사 쿼리
	
	bool bResult = GetWorld()->OverlapMultiByObjectType(			// NewLocation 위치에 무언가가 배치되어 있는지 확인 (OverlapResults 변수에 리턴)
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllObjects,			// 모든 object 타입에 대해서
		FCollisionShape::MakeSphere(775.0f),						// 해당 위치에 원 생성
		CollisionQueryParam
	);

	if (!bResult)
	{
		// 해당 위치에 아무것도 없다면 기믹 액터 배치 (새로운 맵 생성)

		GetWorld()->SpawnActor<AABStageGimmick>(NewLocation, FRotator::ZeroRotator);
	}
}

void AABStageGimmick::OpenAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;

	if (StateChangeActions.Contains(InNewState))							// InNewState 라는 키가 TMap 에 존재한다면
	{
		StateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();	// 바운딩된 함수가 있다면 실행
	}
}

void AABStageGimmick::SetReady()
{
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);				// 플레이어가 진입했을 때 전투가 시작되어야 하니 활성화

	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));			// 문에 대한 충돌은 일어날 필요가 없음, 비활성화
	}

	OpenAllGates();
}

void AABStageGimmick::SetFight()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));				// 전투 중이니 비활성화

	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));			// 문에 대한 충돌은 일어날 필요가 없음, 비활성화
	}

	CloseAllGates();

	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false);
	// OpponentSpawnTime 이후에 npc Spawn
}

void AABStageGimmick::SetChooseReward()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGates();
	SpawnRewardBoxes();
}

void AABStageGimmick::SetChooseNext()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}

	OpenAllGates();
}

void AABStageGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	// npc 가 죽었으므로 다음 단계 이동 구현

	SetState(EStageState::REWARD);
}

void AABStageGimmick::OnOpponentSpawn()
{
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;

	AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);
	// OpponentClass 는 TSubclassOf 템플릿으로 한정해놨으므로
	// AABCharacterNonPlayer 를 상속받은 클래스들만 Spawn 하게 될 것

	AABCharacterNonPlayer* ABOpponentCharacter = Cast<AABCharacterNonPlayer>(OpponentActor);		// 그래도 Cast로 한번 더 확인
	if (ABOpponentCharacter)
	{
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnOpponentDestroyed);
		// npc 가 Destroy 될 때에 함수 바인딩
	}
}

void AABStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())						// 약참조이기 때문에 유효성 확신 불가, 그래서 IsValid로 확인
		{
			AABItemBox* ValidItemBox = RewardBox.Get();	// Get 함수로 포인터 얻어오기

			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			// 현재 이벤트가 발생한 component의 오너 -> GetOwner

			if (OverlappedBox != ValidItemBox)
			{
				// 이벤트가 발생한 오너가 RewardBox 와 같지 않다면
				// 나머지 박스 소멸
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT);
}

void AABStageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FVector WorldSpawnLocation = GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f, 0.0f, 30.0f);


		AActor* ItemActor = GetWorld()->SpawnActor(RewardBoxClass, &WorldSpawnLocation, &FRotator::ZeroRotator);

		AABItemBox* RewardBoxActor = Cast<AABItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnRewardTriggerBeginOverlap);
			RewardBoxes.Add(RewardBoxActor);
		}
	}
}
