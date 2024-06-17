// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SAnimInstance.h"
#include "Character/SViewCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

USAnimInstance::USAnimInstance()
{
}

// �ʱ�ȭ �۾�
void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CurrentSpeed = 0.0f;

	Velocity = FVector::ZeroVector;

	bIsFalling = false;
	bIsCrouching = false;
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// �� TryGetPawnOwner() �Լ��� Try-�� ������? 
	// TryGetPawnOwner() �Լ��� �ݴ��, ������ �ִ��ν��Ͻ��� ������ ���� GetAnimInstance() �Լ�
	// ��ǲ�� �޾ƾ� ���� ������ ���ư� �׸��� ���� �ִϸ��̼��� ���ư�
	// �������� �ִϸ��̼��� ���� ������ �ִϸ��̼� �ʿ��� ���� ���� ���� ����
	// ���� �� ���� �����̴ٰ� � ��쿡 ���� �׾��� �� �ִϸ��̼� �ʿ��� ���� ���°ų� ��������
	// �׷� ��� nullptr�� �������� ������ �̷� ������ �����ϰ��� �ִϸ��̼� �ʿ����� Try�� ����
	ASCharacter* OwnerCharacter = Cast<ASCharacter>(TryGetPawnOwner());
	if (IsValid(OwnerCharacter) == true)
	{
		UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
		if (IsValid(CharacterMovementComponent) == true)
		{
			//CurrentSpeed = CharacterMovementComponent->GetLastUpdateVelocity().Size();

			Velocity = CharacterMovementComponent->GetLastUpdateVelocity();
			CurrentSpeed = Velocity.Size();
			bIsFalling = CharacterMovementComponent->IsFalling();
			bIsCrouching = CharacterMovementComponent->IsCrouching();
		}
	}
}

void USAnimInstance::PlayAnimMontage(UAnimMontage* InAnimMontage)
{
	checkf(true == IsValid(InAnimMontage), TEXT("Invalid InAnimMontage"));

	// �ִϸ��̼��� �������� �ƴҶ���
	if (false == Montage_IsPlaying(InAnimMontage))
	{
		Montage_Play(InAnimMontage);
	}
}

void USAnimInstance::AnimNotify_CheckHit()
{
	// �浹 �˻�
	if (true == OnCheckHit.IsBound())
	{
		// �ش� ��������Ʈ�� 1���� �Լ��� ���ε� �Ǿ� �ִٸ� true�� ��ȯ�ϴ� IsBound() �Լ�
		OnCheckHit.Broadcast();
	}
}

void USAnimInstance::AnimNotify_CheckAttackInput()
{
	// �ƹ��͵� ���ε尡 �ȵǾ� �ִµ� ��ε�ĳ��Ʈ�ϴ� ���� ���ǹ��ϹǷ� �� �� üũ�Ͽ� �۾�
	if (true == OnCheckAttackInput.IsBound())
	{
		OnCheckAttackInput.Broadcast();
	}
}
