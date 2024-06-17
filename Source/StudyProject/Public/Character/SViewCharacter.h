// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SCharacter.h"
#include "InputActionValue.h"
#include "SViewCharacter.generated.h"

// ���带 ���� ����ü
// uint8�� ũ�� ����
UENUM(BlueprintType)
enum class EViewMode : uint8
{
	None,
	BackView,
	QuarterView,
	TPSView,
	End,

};
/**
 * 
 */
UCLASS()
class STUDYPROJECT_API ASViewCharacter : public ASCharacter
{
	GENERATED_BODY()
	
public:
	ASViewCharacter();

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnMeleeAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnCheckHit();

	/*UFUNCTION()
	void OnCheckAttackInput();

	UFUNCTION()
	void EndCombo(UAnimMontage* InMontage, bool bInterruped);*/

	void SetViewMode(EViewMode InViewMode);

	//void BeginCombo();

	float GetForwardInputValue() const { return ForwardInputValue; }
	float GetRightInputValue() const { return RightInputValue; }

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void OnCheckAttackInput();

	UFUNCTION()
	void EndCombo(UAnimMontage* InMontage, bool bInterruped);

	void Move(const FInputActionValue& InValue);

	void Look(const FInputActionValue& InValue);

	void ChangeView(const FInputActionValue& InValue);

	void InputQuickSlot01(const FInputActionValue& InValue);

	void InputQuickSlot02(const FInputActionValue& InValue);

	void Attack(const FInputActionValue& InValue);

	void BeginCombo();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SViewCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class USInputConfigData> PlayerCharacterInputConfigData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SViewCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class UInputMappingContext> PlayerCharacterInputMappingContext;

	// TSubclassOf: ASWeaponActorŬ������ �������� �ڽ� Ŭ�������� ���� �����ϰ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<class ASWeaponActor> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<class ASWeaponActor> WeaponInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float ForwardInputValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float RightInputValue;

	FOnMontageEnded OnMeleeAttackMontageEndedDelegate;

	// UPROPERTY() ��ũ�θ� ������� �����Ƿ� �ʱ�ȭ�� �����ؾ���
	EViewMode CurrentViewMode = EViewMode::None;

	FVector DirectionToMove = FVector::ZeroVector;
	FRotator DestArmRotation = FRotator::ZeroRotator;

	float DestArmLength = 0.0f;
	float ArmLengthChangeSpeed = 3.0f;
	float ArmRotationChangeSpeed = 10.0f;

	// ���Ǻ� ���� �̸��� �����ϱ� ������ �̸��� �޴� ������ ���
	FString AttackAnimMontageSectionName = FString(TEXT("Attack"));
	int32 MaxComboCount = 3;
	int32 CurrentComboCount = 0;

	// �ø�������� �ǰų� ���ø����̼� �� �ʿ� �����Ƿ� �׳� bool �ڷ��� ���
	// �ش� Ű�� ���;� �������� �Ѿ �� ����
	bool bIsAttackKeyPressed = false;

	bool bIsNowAttacking = false;
};
