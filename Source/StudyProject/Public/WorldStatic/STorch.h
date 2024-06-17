// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STorch.generated.h"

UCLASS()
class STUDYPROJECT_API ASTorch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTorch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// TObjectPtr: �𸮾󿡼� �����ϰ� �� �� �ְ� �����ϴ� ������, ������ Ÿ���̶� ���������� ���� ����
	// Visivle-: �����Ϳ��� ���� �Ұ�, Edit-: �����Ϳ��� ���� ����
	// �ݸ�����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTorch", meta = (AllowPrivateAccess))
	TObjectPtr<class UBoxComponent> BoxComponent;

	// ȶ�� �޽ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTorch", meta = (AllowPrivateAccess))
	TObjectPtr<class UStaticMeshComponent> BodyStaticMeshComponent;

	// ��������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTorch", meta = (AllowPrivateAccess))
	TObjectPtr<class UPointLightComponent> PointLightComponent;
	
	// �ҿ� ���� ��ƼŬ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTorch", meta = (AllowPrivateAccess))
	TObjectPtr<class UParticleSystemComponent> ParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASTorch", meta = (AllowPrivateAccess))
	TObjectPtr<class URotatingMovementComponent> RotatingMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTorch", meta = (AllowPrivateAccess))
	int32 ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ASTorch", meta = (AllowPrivateAccess))
	float RotationSpeed;

};
