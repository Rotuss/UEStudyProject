// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"		// UGameInstance를 상속 받기 위함
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

	// 이벤트 함수, 에디터에서 플레이 버튼을 눌렀을 때 호출
	virtual void Init() override;

	// 이벤트 함수, 에디터에서 플레이를 종료할 때 호출
	virtual void Shutdown() override;

	// 다이나믹 델리게이트이기 때문에 블프에서도 사용할 수 있게 UFUNCTION() 매크로 사용
	UFUNCTION()
	void HandlePigeonFlying(const FString& InName, const int32 InID);

protected:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	TObjectPtr<USPigeon> SpawnedPigeon;
};
