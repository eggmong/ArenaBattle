// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/ABCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "ABComboActionData.h"
#include "Physics/ABCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "UI/ABWidgetComponent.h"
#include "UI/ABHpBarWidget.h"
#include "Item/ABWeaponItemData.h"


DEFINE_LOG_CATEGORY(LogABCharacter);

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Pawn
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
    GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

    // Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // Mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    // 생성자에서 캐릭터컨트롤 데이터 추가해줬음 (숄더뷰, 쿼터뷰 데이터 추가)
    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(TEXT("/Script/UnrealArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder'"));
    if (ShoulderDataRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(TEXT("/Script/UnrealArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater'"));
    if (QuaterDataRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
    }

    // 새로 생성한 NonPlayer를 위해 기본 액션을 세팅해줬음
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));
    if (ComboActionMontageRef.Object)
    {
        ComboActionMontage = ComboActionMontageRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Script/ArenaBattle.ABComboActionData'/Game/ArenaBattle/CharacterAction/ABA_ComboAttack.ABA_ComboAttack'"));
    if (ComboActionDataRef.Object)
    {
        ComboActionData = ComboActionDataRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_Dead.AM_Dead'"));
    if (DeadActionMontageRef.Object)
    {
        DeadMontage = DeadActionMontageRef.Object;
    }


    // Stat Component
    Stat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("Stat"));


    // Widget Component
    // 위젯의 경우 애니메이션 블루프린트와 유사하게, 클래스 정보를 등록해서
    // BeginPlay가 시작 되면 그 때 클래스 정보로부터 인스턴스가 생성되는 형태
    // 그래서 위젯 블루프린트의 레퍼런스를 가져와, ConstructorHelper 로 클래스 정보를 가져와야 함
    HpBar = CreateDefaultSubobject<UABWidgetComponent>(TEXT("Widget"));
    HpBar->SetupAttachment(GetMesh());
    HpBar->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
    static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C")); // 클래스 정보니까 _C 붙임

    if (HpBarWidgetRef.Class)
    {
        HpBar->SetWidgetClass(HpBarWidgetRef.Class);
        HpBar->SetWidgetSpace(EWidgetSpace::Screen);        // 위젯 위치를 2D 공간으로 지정. 캔버스.
        HpBar->SetDrawSize(FVector2D(150.f, 15.0f));        // 위젯의 크기. 캔버스에서 이 위젯의 작업 공간의 크기. 
        HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision); // UI 의 충돌체크 끔 (마치 유니티에서의 raycast...)
    }


    // Item Action
    // 멤버함수와 바인딩된 델리게이트를 TArray에 하나씩 넣음
    // enum class EItemType : uint8 에서 정의한 열거형값 순서와 동일함. Weapon, Potion, Scroll
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::EquipWeapon)));
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::DrinkPotion)));
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::ReadScroll)));


    // Weapon Component
    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
    Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
    // 무기 스켈레탈 메쉬 컴포넌트를 캐릭터 메쉬의 hand_rSocket 라고 명명된 곳에 부착 (캐릭터의 특정 본에 무기가 부착되어 돌아다닐 수 있도록)
}

void AABCharacterBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    Stat->OnHpZero.AddUObject(this, &AABCharacterBase::SetDead);
    // 이 인스턴스에, 인스턴스의 SetDead함수를 OnHpZero 델리게이트에 등록
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
    // Pawn
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;
    // 보통 캐릭터컨트롤에서 로테이션은 Yaw만 체크한다고 한다

    // CharacterMovement
    GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AABCharacterBase::ProcessComboCommand()
{
    if (CurrentCombo == 0)
    {
        ComboActionBegin();
        return;
    }

    // 만약 타이머가 설정 안되어 있을 때 입력이 들어왔다면
    if (!ComboTimerHandle.IsValid())
    {
        // 이미 타이머가 발동 되어서 시기를 놓쳤거나
        // 더이상 타이머 진행할 필요가 없다는 것
        HasNextComboCommand = false;
    }
    else
    {
        // 타이머가 유효하다면
        // 다음 섹션으로 이동시킬 커맨드가 발동했다는 뜻
        HasNextComboCommand = true;
    }
}

void AABCharacterBase::ComboActionBegin()
{
    // Combo Status
    CurrentCombo = 1;

    // Movement Setting
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    // Animation Setting
    const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

    // 몽타주가 실행되면, 몽타주가 종료될 때 ComboActionEnd 함수 호출이 바로 되도록 여기서 몽타주 세팅
    // 구조체처럼 EndDelegate 라고 선언하고, 정보를 넣을 수 있음
    FOnMontageEnded EndDelegate;
    // 바인딩할 오브젝트 정보 추가 (현재 인스턴스, ComboActionEnd 함수를 매핑)
    EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);

    AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

    ComboTimerHandle.Invalidate();      // 일단 무효하도록 초기화
    SetComboCheckTimer();               // 그리고 타이머 세팅
}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    // 진짜 CurrentCombo 0이 아닌지 체크 (콤보가 시작하면 무조건 1부터 시작하니까)
    ensure(CurrentCombo != 0);

    CurrentCombo = 0;

    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
    int32 ComboIndex = CurrentCombo - 1;

    // 각 프레임 정보가 ComboIndex와 유효한지 검사
    ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

    const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;

    // 각 액션마다 콤보가 발동할 시간 정보를 얻을 수 있음 
    float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

    // 값이 0보다 작으면 콤보가 발동하지 않음 (마지막 콤보일 수도 있고 뭐.. 마지막엔 ComboAttack -1로 넣어줬었다! UABComboActionData 참고)
    if (ComboEffectiveTime > 0.0f)
    {
        // 타이머 설정
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
    }
}

void AABCharacterBase::ComboCheck()
{
    // 초기화 (타이머 또 발동되면 안되니까)
    ComboTimerHandle.Invalidate();

    // 타이머 발동 전에 입력이 들어왔다면
    if (HasNextComboCommand)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

        // MaxComboCount 값을 벗어나지 않도록 제한하면서 콤보카운트 +1
        CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

        // ComboAttack1, 2, 3...
        FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);

        // 해당 몽타주 섹션으로 점프
        AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

        // 그리고 다시 콤보타이머 걸어주기
        SetComboCheckTimer();

        HasNextComboCommand = false;
    }
}

void AABCharacterBase::AttackHitCheck()
{
    // 트레이스를 활용하여 물체가 서로 충돌되는지 체크하는 함수
    // 투사해서 감지할거라 LineTrace/Sweep 중 하나인데, 구체를 이용해서 할 거라 Sweep
    // 하나만 감지할거고, Trace 채널을 사용할거다

    // 트레이스 채널을 사용할거면 헤더 파일을 만들어 간단한 매크로를 작성해두면 편함.
    // ABCollision.h (텍스트파일로 만든거고, 만든 후 uproject에 generate 하면 됨)

    FHitResult OutHitResult;    // 체크 결과값
    FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
    // FCollisionQueryParams 들어가서 생성자 보면,
    // InTraceTag : 태그 정보로 분석할 때 식별자 정보가 되는 인자
    // bInTraceComplex : 복잡한 형태의 충돌체도 감지할지 말지
    // InIgnoreActor : 무시할 Actor들, 이번 프로젝트에선 자기 자신만 무시하면 되기 때문에 this를 넣었음



    const float AttackRange = Stat->GetTotalStat().AttackRange;
    const float AttackRadius = 50.0f;
    const float AttackDamage = Stat->GetTotalStat().Attack;
    const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
    // GetActorLocation() : 현재 액터 위치
    // 더하기
    // GetActorForwardVector (액터의 시선 방향) * GetCapsuleComponent()->GetScaledCapsuleRadius (캡슐의 반지름 값)
    // = 액터의 위치에서 시작되는 게 아니라 정면에 있는 캡슐의 위치에서부터 시작하도록 함

    const FVector End = Start + GetActorForwardVector() * AttackRange;

    bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
    // 월드가 제공하는 서비스라 월드 포인터를 가져와야 함 (GetWorld)
    // Start : 충돌에 사용할 구체 투사 시작 지점, End : 끝 지점
    // FCollisionShape::MakeSphere(AttackRadius) 로 구체의 영역 지정 (반지름 50f)
    // 사용할 트레이스 채널은 Project Setting -> Collision 에서 만들었던 ABAction (CCHANNEL_ABACTION)
    
    if (HitDetected)
    {
        FDamageEvent DamageEvent;
        OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
    }

#if ENABLE_DRAW_DEBUG

    FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
    // 캡슐의 원점

    float CapsuleHalfHeight = AttackRange * 0.5f;
    FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

    DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
    // World 에서 제공
    // 캡슐을 시선 방향으로 눕혀지도록 회전시킴 (FRotationMatrix::MakeFromZ)

#endif
}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // return 값은 Actor가 받은 데미지 양

    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    // EventInstigator : 가해자
    // DamageCauser : 가해자가 사용한 무기 또는 빙의한 pawn

    //// 죽음 구현
    //SetDead();
    Stat->ApplyDamage(DamageAmount);

    return DamageAmount;
}

void AABCharacterBase::SetDead()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    // 움직임 멈추기

    PlayDeadAnimation();
    // 죽은 몽타주 재생

    SetActorEnableCollision(false);

    // HpBar 사라지기
    HpBar->SetHiddenInGame(true);
}

void AABCharacterBase::PlayDeadAnimation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    AnimInstance->StopAllMontages(0.f);
    AnimInstance->Montage_Play(DeadMontage, 1.f);
}

void AABCharacterBase::SetupCharacterWidget(UABUserWidget* InUserWidget)
{
    // HpBar 등록이 필요하여 헤더 추가 및 구현
    UABHpBarWidget* HpBarWidget = Cast<UABHpBarWidget>(InUserWidget);

    if (HpBarWidget)
    {
        HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
        HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
        Stat->OnHpChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
        // Stat의 CurrentHp 값이 변경될 때 마다 UABHpBarWidget::UpdateHpBar 함수가 호출되도록
        // OnHpChanged 델리게이트에 위젯 인스턴스의 멤버함수 추가
        // -> 두 컴포넌트 간의 느슨한 결합
    }
}

void AABCharacterBase::TakeItem(UABItemData* InItemData)
{
    // 캐릭터가 아이템을 가졌을 때,
    // 아이템 데이터에 있는 열거형 값에 따라 서로 다른 액션을 취하도록 구현

    if (InItemData)
    {
        TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
        // ExecuteIfBound : 바인딩된 함수가 존재하면 호출

        // 위의 생성자에서 TakeItemActions 에 추가했던 델리게이트와 바인딩된 함수들은
        // EItemType 에 정의했던 열거형 값들과 동일한 순서로 추가되었음 Weapon = 0, Potion = 1...
        // 그래서 (uint8)InItemData->Type 으로 해도 괜찮음
    }
}

void AABCharacterBase::DrinkPotion(UABItemData* InItemData)
{
    UE_LOG(LogABCharacter, Log, TEXT("Drink Potion"));
}

void AABCharacterBase::EquipWeapon(UABItemData* InItemData)
{
    UE_LOG(LogABCharacter, Log, TEXT("Equip Weapon"));
    
    UABWeaponItemData* WeaponItemData = Cast<UABWeaponItemData>(InItemData);

    if (WeaponItemData)
    {
        if (WeaponItemData->WeaponMesh.IsPending())         // 아직 로딩이 안되어 있다면
        {
            WeaponItemData->WeaponMesh.LoadSynchronous();   // 동기적으로 로딩하도록 함
        }

        Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());  // TSoftObjectPtr 로 로드한 건 .Get() 으로 가져올 수 있음
    }
}

void AABCharacterBase::ReadScroll(UABItemData* InItemData)
{
    UE_LOG(LogABCharacter, Log, TEXT("Read Scroll"));
}
