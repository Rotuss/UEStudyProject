// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SViewCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/SInputConfigData.h"
#include "Item/SWeaponActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/SAnimInstance.h"

ASViewCharacter::ASViewCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASViewCharacter::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (true == IsValid(PlayerController))
    {
        // subsystem을 통해 싱글톤 개체를 가져온다
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (true == IsValid(Subsystem))
        {
            Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
        }
    }

    // 델리게이트 연결 설정
    USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    if (true == IsValid(AnimInstance))
    {
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnMeleeAttackMontageEnded);
        AnimInstance->OnCheckHit.AddDynamic(this, &ThisClass::OnCheckHit);
        AnimInstance->OnCheckAttackInput.AddDynamic(this, &ThisClass::OnCheckAttackInput);
    }
}

void ASViewCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // 뷰모드 설정
    //SetViewMode(EViewMode::BackView);
    //DestArmLength = 400.0f; // 초기화에서 한 번 지정해야 함
    //DestArmRotation = FRotator::ZeroRotator; // 백뷰에서는 의미가 없으므로
    SetViewMode(EViewMode::TPSView);
}

void ASViewCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    switch (CurrentViewMode)
    {
    case EViewMode::BackView:
        break;
    case EViewMode::QuarterView:
    {
        // DirectionToMove값이 변했다?
        // SizeSquared은 절대값으로 어떠한 길이를 나타냄
        if (KINDA_SMALL_NUMBER < DirectionToMove.SizeSquared())
        {
            // X축 벡터 만들기
            GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
            // 해당 방향으로 이동
            AddMovementInput(DirectionToMove);
            // 0으로 초기화
            DirectionToMove = FVector::ZeroVector;
        }
        break;
    }
    case EViewMode::End:
        break;
    default:
        break;
    }

    if (KINDA_SMALL_NUMBER < abs(DestArmLength - SpringArmComponent->TargetArmLength))
    {
        // 보간작업
        SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, DestArmLength, DeltaTime, ArmLengthChangeSpeed);
        SpringArmComponent->SetRelativeRotation(FMath::RInterpTo(SpringArmComponent->GetRelativeRotation(), DestArmRotation, DeltaTime, ArmRotationChangeSpeed));
    }
}

void ASViewCharacter::OnMeleeAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // 특정 AM_Rifle_Fire_Melee의 애니메이션이 끝났을 때만
    if (true == Montage->GetName().Equals(TEXT("AM_Rifle_Fire_Melee"), ESearchCase::IgnoreCase))
    {
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        bIsNowAttacking = false;
    }
}

void ASViewCharacter::OnCheckHit()
{
    UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("CheckHit() has been called.")));
}

void ASViewCharacter::OnCheckAttackInput()
{
    USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    checkf(true == IsValid(AnimInstance), TEXT("Invalid AnimInstance"));
    checkf(true == IsValid(WeaponInstance), TEXT("Invalid WeaponInstance"));

    if (true == bIsAttackKeyPressed)
    {
        // 콤보 카운트 계산
        CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, MaxComboCount);

        // 콤보 섹션 이동
        FName NextSectionName = *FString::Printf(TEXT("%s%d"), *AttackAnimMontageSectionName, CurrentComboCount);
        AnimInstance->Montage_JumpToSection(NextSectionName, WeaponInstance->GetMeleeAttackMontage());
        bIsAttackKeyPressed = false;
    }
}

void ASViewCharacter::EndCombo(UAnimMontage* InMontage, bool bInterruped)
{
    checkf(true == IsValid(WeaponInstance), TEXT("Invalid WeaponInstance"));
    ensureMsgf(0 != CurrentComboCount, TEXT("0 == CurrentComboCount"));

    // 초기화 작업
    CurrentComboCount = 0;
    bIsAttackKeyPressed = false;
    bIsNowAttacking = false;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    // 정리코드
    if (true == OnMeleeAttackMontageEndedDelegate.IsBound()) OnMeleeAttackMontageEndedDelegate.Unbind();
}

void ASViewCharacter::SetViewMode(EViewMode InViewMode)
{
    // 같은 뷰모드이면 무시
    if (CurrentViewMode == InViewMode)
    {
        return;
    }

    // 뷰모드 업데이트
    CurrentViewMode = InViewMode;

    switch (CurrentViewMode)
    {
    case EViewMode::BackView:
        //SpringArmComponent->TargetArmLength = 400.0f;
        //// ControlRotation이 Pawn의 회전과 동기화 -> Pawn의 회전이 SprintArm의 회전 동기화. 이로 인해 SetRotation()이 무의미
        //SpringArmComponent->SetRelativeRotation(FRotator::ZeroRotator);

        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw = false;
        bUseControllerRotationRoll = false;

        SpringArmComponent->bUsePawnControlRotation = true;
        SpringArmComponent->bDoCollisionTest = true;

        SpringArmComponent->bInheritPitch = true;
        SpringArmComponent->bInheritYaw = true;
        SpringArmComponent->bInheritRoll = false;

        // 회전방향에 따른 메쉬 회전
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
        GetCharacterMovement()->bOrientRotationToMovement = true;   // 가고자 하는 방향으로 회전되는걸 허용하겠다
        GetCharacterMovement()->bUseControllerDesiredRotation = false;

        break;
    case EViewMode::QuarterView:
        /*SpringArmComponent->TargetArmLength = 900.0f;
        SpringArmComponent->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));*/

        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw = false;
        bUseControllerRotationRoll = false;

        SpringArmComponent->bUsePawnControlRotation = false;
        SpringArmComponent->bDoCollisionTest = false;

        SpringArmComponent->bInheritPitch = false;
        SpringArmComponent->bInheritYaw = false;
        SpringArmComponent->bInheritRoll = false;

        // 45도씩 끊겨서 움직이는 것을 부드럽게 바꾸기 위함
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
        GetCharacterMovement()->bOrientRotationToMovement = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;

        break;
    case EViewMode::TPSView:
    {
        SpringArmComponent->TargetArmLength = 400.0f;
        SpringArmComponent->SetRelativeLocation(FVector(0.0f, 50.0f, 50.0f));

        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw = true;
        bUseControllerRotationRoll = false;

        SpringArmComponent->bUsePawnControlRotation = true;
        SpringArmComponent->bDoCollisionTest = true;

        SpringArmComponent->bInheritPitch = true;
        SpringArmComponent->bInheritYaw = true;
        SpringArmComponent->bInheritRoll = false;

        GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
        GetCharacterMovement()->bOrientRotationToMovement = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = false;

        break;
    }
    case EViewMode::End:
        break;
    default:
        break;
    }
}

void ASViewCharacter::BeginCombo()
{
    USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    checkf(true == IsValid(AnimInstance), TEXT("Invalid AnimInstance"));
    checkf(true == IsValid(WeaponInstance), TEXT("Invalid WeaponInstance"));

    // 멈추기
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    // 공격 중인지 확인
    bIsNowAttacking = true;
    // 몽타주 플레이 시작
    AnimInstance->PlayAnimMontage(WeaponInstance->GetMeleeAttackMontage());

    CurrentComboCount = 1;

    if (false == OnMeleeAttackMontageEndedDelegate.IsBound())
    {
        // 특정한 몽타주가 끝났을 때만 별도의 작업을 하기 원함
        // 따라서 Montage_SetEndDelegate을 사용하여 특정 몽타주(OnMeleeAttackMontageEndedDelegate)가 끝났을 때만 호출되게!
        // 호출 작업 전에 미리 델리게이트로 특정 호출될 함수를 바인드하여
        // 특정 WeaponInstance->GetMeleeAttackMontage()가 끝났을 때만 OnMeleeAttackMontageEndedDelegate 브로드캐스트가 호출되는 방식(EndCombo가 호출)
        OnMeleeAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndCombo);
        AnimInstance->Montage_SetEndDelegate(OnMeleeAttackMontageEndedDelegate, WeaponInstance->GetMeleeAttackMontage());
    }
}

void ASViewCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (true == ::IsValid(EnhancedInputComponent))
    {
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Move, ETriggerEvent::Triggered, this, &ASViewCharacter::Move);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Look, ETriggerEvent::Triggered, this, &ASViewCharacter::Look);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->ChangeView, ETriggerEvent::Started, this, &ASViewCharacter::ChangeView);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->QuickSlot01, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot01);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->QuickSlot02, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot02);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Attack, ETriggerEvent::Started, this, &ThisClass::Attack);
    }
}

void ASViewCharacter::Move(const FInputActionValue& InValue)
{
    if (MOVE_None == GetCharacterMovement()->GetGroundMovementMode()) return;
    
    FVector2D MovementVector = InValue.Get<FVector2D>();

    /*AddMovementInput(GetActorForwardVector(), MovementVector.X);
    AddMovementInput(GetActorRightVector(), MovementVector.Y);*/

    switch (CurrentViewMode)
    {
    case EViewMode::None:
        break;
    case EViewMode::TPSView:
    case EViewMode::BackView:
    // Switch-Case 구문 내에서 Scope를 지정하면 해당 Scope 내에서 변수 선언이 가능
    { 
        // 카메라가 바라보는 방향으로 회전하기 위한 코드

        const FRotator ControlRotation = GetController()->GetControlRotation();
        const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.0f);

        // 해당 축 행렬의 단위 벡터를 이용해 방향 변경
        const FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
        const FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardVector, MovementVector.X);
        AddMovementInput(RightVector, MovementVector.Y);

        ForwardInputValue = MovementVector.X;
        RightInputValue = MovementVector.Y;

        break;
    }
    case EViewMode::QuarterView:
        DirectionToMove.X = MovementVector.X;
        DirectionToMove.Y = MovementVector.Y;
        break;
    case EViewMode::End:
        break;
    default:
        AddMovementInput(GetActorForwardVector(), MovementVector.X);
        AddMovementInput(GetActorRightVector(), MovementVector.Y);
        break;
    }
}

void ASViewCharacter::Look(const FInputActionValue& InValue)
{
    //if (true == IsValid(GetController()))
    //{
    //    FVector2D LookVector = InValue.Get<FVector2D>();

    //    // 좌우
    //    AddControllerYawInput(LookVector.X);
    //    // 상하
    //    AddControllerPitchInput(LookVector.Y);
    //}

    FVector2D LookVector = InValue.Get<FVector2D>();

    switch (CurrentViewMode)
    {
    case EViewMode::None:
        break;
    case EViewMode::TPSView:
    case EViewMode::BackView:
        AddControllerYawInput(LookVector.X);
        AddControllerPitchInput(LookVector.Y);
        break;
    case EViewMode::QuarterView:
        break;
    case EViewMode::End:
        break;
    default:
        break;
    }
}

void ASViewCharacter::ChangeView(const FInputActionValue& InValue)
{
    switch (CurrentViewMode)
    {
    case EViewMode::BackView:
        /* Case 1. 이전에 BackView 시점이었다면

          BackView는 컨트롤 회전값 == 스프링암 회전값.
          그러나 QuarterView는 캐릭터의 회전값 == 컨트롤 회전값.
          따라서 시점 변경 전에 캐릭터의 현재 회전값을 컨트롤 회전에 세팅해둬야 함.
          안그러면 컨트롤 회전이 일어나면서 현재 캐릭터의 회전값이 스프링암 회전값(컨트롤 회전값)으로 동기화됨.
        */
        GetController()->SetControlRotation(GetActorRotation());
        DestArmLength = 900.0f;
        DestArmRotation = FRotator(-45.0f, 0.0f, 0.0f);
        SetViewMode(EViewMode::QuarterView);
        break;
    case EViewMode::QuarterView:
        /* Case 2. 이전에 QuarterView 시점이었다면

          컨트롤 회전이 캐릭터 회전에 맞춰져 있을거임.
          //QuarterView는 현재 스프링암의 회전값을 컨트롤 회전에 세팅해둔 상태에서 시점 변경해야 올바름.
          BackView에서는 컨트롤 로테이션이 폰의 회전과 동기화되고 폰의 회전이 스프링 암의 회전과 동기화.
          따라서 스프링 암의 회전을 임의로 설정할 수 없음. 0으로 고정.
        */
        GetController()->SetControlRotation(FRotator::ZeroRotator);
        DestArmLength = 400.0f;
        DestArmRotation = FRotator::ZeroRotator;
        SetViewMode(EViewMode::BackView);
        break;
    case EViewMode::End:
        break;
    default:
        break;
    }
}

// 장착 했을 때 소켓을 찾아서 설정
void ASViewCharacter::InputQuickSlot01(const FInputActionValue& InValue)
{
    FName WeaponSocket(TEXT("WeaponSocket"));
    if (true == GetMesh()->DoesSocketExist(WeaponSocket) && false == IsValid(WeaponInstance))
    {
        WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (true == IsValid(WeaponInstance))
        {
            WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
        }
    }
}

// 해제 했을 때 처리
void ASViewCharacter::InputQuickSlot02(const FInputActionValue& InValue)
{
    if (true == IsValid(WeaponInstance))
    {
        WeaponInstance->Destroy();
        WeaponInstance = nullptr;
    }
}

void ASViewCharacter::Attack(const FInputActionValue& InValue)
{
    /*if (true == IsValid(WeaponInstance))
    {
        UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Attack() has been called.")));
    }*/

    // 점프 중 공격X
    if (true == GetCharacterMovement()->IsFalling()) return;

    USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    checkf(true == IsValid(AnimInstance), TEXT("Invalid AnimInstance"));

    if (true == IsValid(WeaponInstance))
    {
        if (true == IsValid(WeaponInstance->GetMeleeAttackMontage()))
        {
            /*AnimInstance->PlayAnimMontage(WeaponInstance->GetMeleeAttackMontage());

            GetCharacterMovement()->SetMovementMode(MOVE_None);
            bIsNowAttacking = true;*/

            // 첫 콤보
            if (0 == CurrentComboCount) BeginCombo();
            // 콤보공격
            else
            {
                ensure(FMath::IsWithinInclusive<int32>(CurrentComboCount, 1, MaxComboCount));
                bIsAttackKeyPressed = true;
            }
        }
    }
}
