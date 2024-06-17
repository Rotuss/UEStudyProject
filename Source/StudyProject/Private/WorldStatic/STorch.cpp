// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldStatic/STorch.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
ASTorch::ASTorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// ƽ �Լ��� ȣ���ϰ� �� ���̳�
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BodyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyStaticMeshComponent"));
	BodyStaticMeshComponent->SetupAttachment(GetRootComponent());
	BodyStaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
	// ��� ������ �÷��� �� ������� �����Ƿ� static Ű���� ���
	// ConstructorHelpers::FObjectFinder: ���ø� Ÿ�Կ� ���� �ش� ������Ʈ�� ã���ִ� ���α׷�
	// �ϵ��ڵ��� ��� ū ���۾����� ��Ʈ���� �����Ѵٸ� ��ΰ� �ٲ�� ��찡 ����, ���� �������� ���� �����ϰ� �۾��ϴ� ���� ����
	// �������� ������ �����ϰ� �Ǹ� �ش� ������Ʈ�� ��ΰ� �ٲ� 'Fix Up Redirectors'�� ���� �ڵ����� ����Ǽ� �����ϰ� ��
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyStaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Pillar_50x500.Pillar_50x500'"));
	if (true == BodyStaticMesh.Succeeded()) BodyStaticMeshComponent->SetStaticMesh(BodyStaticMesh.Object);*/

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->SetupAttachment(GetRootComponent());
	PointLightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(GetRootComponent());
	ParticleSystemComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
	/*static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleTemplate(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
	if (true == ParticleTemplate.Succeeded()) ParticleSystemComponent->SetTemplate(ParticleTemplate.Object);*/

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));

}

// Called when the game starts or when spawned
void ASTorch::BeginPlay()
{
	Super::BeginPlay();
	
	RotationSpeed = 300.0f;
	RotatingMovementComponent->RotationRate = FRotator(0.0f, RotationSpeed, 0.0f);

}

// Called every frame
void ASTorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ���� �����̼� ���� �߰������� ����
	//AddActorWorldRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

}

