// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"		// UGameInstance�� ��� �ޱ� ����
#include "SGameInstance.generated.h"

class USPigeon;

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USGameInstance();

	// �̺�Ʈ �Լ�, �����Ϳ��� �÷��� ��ư�� ������ �� ȣ��
	virtual void Init() override;

	// �̺�Ʈ �Լ�, �����Ϳ��� �÷��̸� ������ �� ȣ��
	virtual void Shutdown() override;

	// ���̳��� ��������Ʈ�̱� ������ ���������� ����� �� �ְ� UFUNCTION() ��ũ�� ���
	UFUNCTION()
	void HandlePigeonFlying(const FString& InName, const int32 InID);

protected:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	TObjectPtr<USPigeon> SpawnedPigeon;
};
