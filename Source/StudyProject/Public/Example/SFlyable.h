// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SFlyable.generated.h"

// This class does not need to be modified.
// Ŭ���� ������ ��Ÿ�� �� ���
UINTERFACE(MinimalAPI)
class USFlyable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
// ���� ����ϰ� �Ǵ� �κ�
// ���Ǹ� ���ִ� �κ�
class STUDYPROJECT_API ISFlyable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// ���� ���� �Լ�
	virtual void Fly() = 0;
};
