// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/ABCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "ABComboActionData.h"

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Pawn
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

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
    GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

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
    const float AttackSpeedRate = 1.f;
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

    const float AttackSpeedRate = 1.f;

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
