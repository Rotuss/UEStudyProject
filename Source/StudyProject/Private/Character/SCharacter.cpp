// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    float CharacterHalfHeight = 95.0f;
    float CharacterRadius = 20.0f;

    GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);

    FVector PivotPosition(0.0f, 0.0f, -CharacterHalfHeight);
    FRotator PivotRotation(0.0f, -90.0f, 0.0f);
    GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 400.0f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    // ���� �� �ִ� �ִ� �ӵ�
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    // �Ƴ��α� �Է��� �ּ� ��
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    // ������
    GetCharacterMovement()->JumpZVelocity = 700.0f;
    // ���߿� ���� �� ������ ���� ����
    GetCharacterMovement()->AirControl = 0.35f;
    // �ȴٰ� ������ �� �����Ÿ�, ���� Ŭ���� �� ������ ����
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

