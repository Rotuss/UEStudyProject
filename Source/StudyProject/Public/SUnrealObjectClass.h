// Fill out your copyright notice in the Description page of Project Settings.

// ��ȯ ��� ������ ���� ����
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"				// UObject ������ ��� �ޱ� ����
#include "SUnrealObjectClass.generated.h"		// UHT, ��ũ�θ� ��ȯ �����ϱ� ����

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USUnrealObjectClass : public UObject
{
public:
	GENERATED_BODY()

public:
	USUnrealObjectClass();

	UFUNCTION()
	void HelloUnreal();

	const FString& GetName() const { return Name; }

protected:
	UPROPERTY()
	FString Name;
};
