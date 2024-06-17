// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SPlayerPawn.generated.h"

UCLASS()
class STUDYPROJECT_API ASPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	// �𸮾��� Construction Script�� ����, CDO ���� ����. �츮�� �����ϴ� ��ü ���� ������ ���� �ٸ�
	ASPlayerPawn();

#pragma region FlowchartCheck
	// �츮�� �����ϴ� ��ü ���� ���� �Լ�
	/*virtual void PostInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;*/
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "ASPlayerPawn", meta = (AllowPrivateAccess))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = "ASPlayerPawn", meta = (AllowPrivateAccess))
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, Category = "ASPlayerPawn", meta = (AllowPrivateAccess))
	TObjectPtr<class UFloatingPawnMovement> FloatingPawnMovementComponent;

	UPROPERTY(EditAnywhere, Category = "ASPlayerPawn", meta = (AllowPrivateAccess))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "ASPlayerPawn", meta = (AllowPrivateAccess))
	TObjectPtr<class UCameraComponent> CameraComponent;

};
