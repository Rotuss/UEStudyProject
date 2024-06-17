// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ASPlayerPawn::ASPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    float CharacterHalfHeight = 90.0f;
    float CharacterRadius = 40.0f;

#pragma region InitializeCapsuleComponent
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    SetRootComponent(CapsuleComponent);
    CapsuleComponent->SetCapsuleHalfHeight(CharacterHalfHeight);
    CapsuleComponent->SetCapsuleRadius(CharacterRadius);
#pragma endregion

#pragma region InitializeSkeletalMesh
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(RootComponent);
    FVector PivotPosition(0.0f, 0.0f, -CharacterHalfHeight);
    FRotator PivotRotation(0.0f, -90.0f, 0.0f);
    SkeletalMeshComponent->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);
    //static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("������Ʈ �н�"));
    //if (true == SkeletalMeshAsset.Succeeded())
    //{
    //    SkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshAsset.Object);
    //}
    // �� ������Ʈ �н� ��� ���� ����ǥ ���� ���븸���ε� ������
    // �������Ʈ ������ ���� �� ���� �������� _C�� �ٿ���� �Ѵ�
    // ������ ��� ���� ������ �������Ʈ���� ����
    /*static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassInfo(TEXT("/Script/Engine.AnimBlueprint'/Game/StudyProject/Animation/AnimationBlueprint/ABP_PlayerPawn.ABP_PlayerPawn_C'"));
    if (true == AnimInstanceClassInfo.Succeeded())
    {
        SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        SkeletalMeshComponent->SetAnimClass(AnimInstanceClassInfo.Class);
    }*/
#pragma endregion

#pragma region InitializeCamera
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 400.0f;
    SpringArmComponent->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);
#pragma endregion

    FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovementComponent"));

}

#pragma region FlowchartCheck
//void ASPlayerPawn::PostInitializeComponents()
//{
//	UE_LOG(LogTemp, Log, TEXT("                Start ASPlayerPawn::      PostInitializeComponents()"));
//	Super::PostInitializeComponents();
//	UE_LOG(LogTemp, Log, TEXT("                End   ASPlayerPawn::      PostInitializeComponents()"));
//}
//
//void ASPlayerPawn::PossessedBy(AController* NewController)
//{
//	UE_LOG(LogTemp, Log, TEXT("                Start ASPlayerPawn::      PossessedBy(ASPlayerController)"));
//	Super::PossessedBy(NewController);
//	UE_LOG(LogTemp, Log, TEXT("                End   ASPlayerPawn::      PossessedBy(ASPlayerController)"));
//}
//
//void ASPlayerPawn::UnPossessed()
//{
//	UE_LOG(LogTemp, Log, TEXT("                Start ASPlayerPawn::      UnPossessed()"));
//	Super::UnPossessed();
//	UE_LOG(LogTemp, Log, TEXT("                End   ASPlayerPawn::      UnPossessed()"));
//}
//
//void ASPlayerPawn::EndPlay(EEndPlayReason::Type EndPlayReason)
//{
//	UE_LOG(LogTemp, Log, TEXT("                Start ASPlayerPawn::      EndPlay()"));
//	Super::EndPlay(EndPlayReason);
//	UE_LOG(LogTemp, Log, TEXT("                End   ASPlayerPawn::      EndPlay()"));
//}
#pragma endregion

// Called when the game starts or when spawned
void ASPlayerPawn::BeginPlay()
{
	//UE_LOG(LogTemp, Log, TEXT("                Start ASPlayerPawn::      BeginPlay()"));
	Super::BeginPlay();
	//UE_LOG(LogTemp, Log, TEXT("                End   ASPlayerPawn::      BeginPlay()"));
	
    /*checkf(true == IsValid(SkeletalMeshComponent), TEXT("Invalid SkeletalMeshComponent"));
    SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);

    UAnimationAsset* AnimationAsset = LoadObject<UAnimationAsset>(SkeletalMeshComponent, TEXT("/Script/Engine.AnimSequence'/Game/ParagonRevenant/Characters/Heroes/Revenant/Animations/Jog_Fwd.Jog_Fwd'"));
    if (nullptr != AnimationAsset)
    {
        SkeletalMeshComponent->PlayAnimation(AnimationAsset, true);
    }*/

}

// Called every frame
void ASPlayerPawn::Tick(float DeltaTime)
{
	/*static bool bOnce = false;
	if (false == bOnce)
	{
		UE_LOG(LogTemp, Log, TEXT("                Start ASPlayerPawn::      Tick()"));
	}*/
	Super::Tick(DeltaTime);
	/*if (false == bOnce)
	{
		UE_LOG(LogTemp, Log, TEXT("                End   ASPlayerPawn::      Tick()"));
		bOnce = true;
	}*/

}

// Called to bind functionality to input
// �÷��̾ ���Ǹ� �� �� input ���� ������Ʈ ���ε� ó�� �۾��ϴ� �Լ�
void ASPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//UE_LOG(LogTemp, Log, TEXT("                Start ASPlayerPawn::      SetupPlayerInputComponent(PlayerInputComponent)"));
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//UE_LOG(LogTemp, Log, TEXT("                End   ASPlayerPawn::      SetupPlayerInputComponent(PlayerInputComponent)"));

}

