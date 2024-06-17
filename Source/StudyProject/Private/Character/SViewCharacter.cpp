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
        // subsystem�� ���� �̱��� ��ü�� �����´�
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (true == IsValid(Subsystem))
        {
            Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
        }
    }

    // ��������Ʈ ���� ����
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

    // ���� ����
    //SetViewMode(EViewMode::BackView);
    //DestArmLength = 400.0f; // �ʱ�ȭ���� �� �� �����ؾ� ��
    //DestArmRotation = FRotator::ZeroRotator; // ��信���� �ǹ̰� �����Ƿ�
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
        // DirectionToMove���� ���ߴ�?
        // SizeSquared�� ���밪���� ��� ���̸� ��Ÿ��
        if (KINDA_SMALL_NUMBER < DirectionToMove.SizeSquared())
        {
            // X�� ���� �����
            GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
            // �ش� �������� �̵�
            AddMovementInput(DirectionToMove);
            // 0���� �ʱ�ȭ
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
        // �����۾�
        SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, DestArmLength, DeltaTime, ArmLengthChangeSpeed);
        SpringArmComponent->SetRelativeRotation(FMath::RInterpTo(SpringArmComponent->GetRelativeRotation(), DestArmRotation, DeltaTime, ArmRotationChangeSpeed));
    }
}

void ASViewCharacter::OnMeleeAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // Ư�� AM_Rifle_Fire_Melee�� �ִϸ��̼��� ������ ����
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
        // �޺� ī��Ʈ ���
        CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, MaxComboCount);

        // �޺� ���� �̵�
        FName NextSectionName = *FString::Printf(TEXT("%s%d"), *AttackAnimMontageSectionName, CurrentComboCount);
        AnimInstance->Montage_JumpToSection(NextSectionName, WeaponInstance->GetMeleeAttackMontage());
        bIsAttackKeyPressed = false;
    }
}

void ASViewCharacter::EndCombo(UAnimMontage* InMontage, bool bInterruped)
{
    checkf(true == IsValid(WeaponInstance), TEXT("Invalid WeaponInstance"));
    ensureMsgf(0 != CurrentComboCount, TEXT("0 == CurrentComboCount"));

    // �ʱ�ȭ �۾�
    CurrentComboCount = 0;
    bIsAttackKeyPressed = false;
    bIsNowAttacking = false;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    // �����ڵ�
    if (true == OnMeleeAttackMontageEndedDelegate.IsBound()) OnMeleeAttackMontageEndedDelegate.Unbind();
}

void ASViewCharacter::SetViewMode(EViewMode InViewMode)
{
    // ���� �����̸� ����
    if (CurrentViewMode == InViewMode)
    {
        return;
    }

    // ���� ������Ʈ
    CurrentViewMode = InViewMode;

    switch (CurrentViewMode)
    {
    case EViewMode::BackView:
        //SpringArmComponent->TargetArmLength = 400.0f;
        //// ControlRotation�� Pawn�� ȸ���� ����ȭ -> Pawn�� ȸ���� SprintArm�� ȸ�� ����ȭ. �̷� ���� SetRotation()�� ���ǹ�
        //SpringArmComponent->SetRelativeRotation(FRotator::ZeroRotator);

        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw = false;
        bUseControllerRotationRoll = false;

        SpringArmComponent->bUsePawnControlRotation = true;
        SpringArmComponent->bDoCollisionTest = true;

        SpringArmComponent->bInheritPitch = true;
        SpringArmComponent->bInheritYaw = true;
        SpringArmComponent->bInheritRoll = false;

        // ȸ�����⿡ ���� �޽� ȸ��
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
        GetCharacterMovement()->bOrientRotationToMovement = true;   // ������ �ϴ� �������� ȸ���Ǵ°� ����ϰڴ�
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

        // 45���� ���ܼ� �����̴� ���� �ε巴�� �ٲٱ� ����
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

    // ���߱�
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    // ���� ������ Ȯ��
    bIsNowAttacking = true;
    // ��Ÿ�� �÷��� ����
    AnimInstance->PlayAnimMontage(WeaponInstance->GetMeleeAttackMontage());

    CurrentComboCount = 1;

    if (false == OnMeleeAttackMontageEndedDelegate.IsBound())
    {
        // Ư���� ��Ÿ�ְ� ������ ���� ������ �۾��� �ϱ� ����
        // ���� Montage_SetEndDelegate�� ����Ͽ� Ư�� ��Ÿ��(OnMeleeAttackMontageEndedDelegate)�� ������ ���� ȣ��ǰ�!
        // ȣ�� �۾� ���� �̸� ��������Ʈ�� Ư�� ȣ��� �Լ��� ���ε��Ͽ�
        // Ư�� WeaponInstance->GetMeleeAttackMontage()�� ������ ���� OnMeleeAttackMontageEndedDelegate ��ε�ĳ��Ʈ�� ȣ��Ǵ� ���(EndCombo�� ȣ��)
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
    // Switch-Case ���� ������ Scope�� �����ϸ� �ش� Scope ������ ���� ������ ����
    { 
        // ī�޶� �ٶ󺸴� �������� ȸ���ϱ� ���� �ڵ�

        const FRotator ControlRotation = GetController()->GetControlRotation();
        const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.0f);

        // �ش� �� ����� ���� ���͸� �̿��� ���� ����
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

    //    // �¿�
    //    AddControllerYawInput(LookVector.X);
    //    // ����
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
        /* Case 1. ������ BackView �����̾��ٸ�

          BackView�� ��Ʈ�� ȸ���� == �������� ȸ����.
          �׷��� QuarterView�� ĳ������ ȸ���� == ��Ʈ�� ȸ����.
          ���� ���� ���� ���� ĳ������ ���� ȸ������ ��Ʈ�� ȸ���� �����ص־� ��.
          �ȱ׷��� ��Ʈ�� ȸ���� �Ͼ�鼭 ���� ĳ������ ȸ������ �������� ȸ����(��Ʈ�� ȸ����)���� ����ȭ��.
        */
        GetController()->SetControlRotation(GetActorRotation());
        DestArmLength = 900.0f;
        DestArmRotation = FRotator(-45.0f, 0.0f, 0.0f);
        SetViewMode(EViewMode::QuarterView);
        break;
    case EViewMode::QuarterView:
        /* Case 2. ������ QuarterView �����̾��ٸ�

          ��Ʈ�� ȸ���� ĳ���� ȸ���� ������ ��������.
          //QuarterView�� ���� ���������� ȸ������ ��Ʈ�� ȸ���� �����ص� ���¿��� ���� �����ؾ� �ùٸ�.
          BackView������ ��Ʈ�� �����̼��� ���� ȸ���� ����ȭ�ǰ� ���� ȸ���� ������ ���� ȸ���� ����ȭ.
          ���� ������ ���� ȸ���� ���Ƿ� ������ �� ����. 0���� ����.
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

// ���� ���� �� ������ ã�Ƽ� ����
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

// ���� ���� �� ó��
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

    // ���� �� ����X
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

            // ù �޺�
            if (0 == CurrentComboCount) BeginCombo();
            // �޺�����
            else
            {
                ensure(FMath::IsWithinInclusive<int32>(CurrentComboCount, 1, MaxComboCount));
                bIsAttackKeyPressed = true;
            }
        }
    }
}
