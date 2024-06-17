// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameMode.h"
#include "Controller/SPlayerController.h"
#include "Character/SPlayerPawn.h"

ASGameMode::ASGameMode()
{
	PlayerControllerClass = ASPlayerController::StaticClass();
	DefaultPawnClass = ASPlayerPawn::StaticClass();
}

// ���� ���� �ʱ�ȭ �κ� ȣ��
void ASGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Error, TEXT("         Start ASGameMode::        InitGame()"));
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Error, TEXT("         End   ASGameMode::        InitGame()"));
}

// state�� ��������Ƽ�� ������ �� ��Ȯ����
// ����� �̷��� �����Ѵ� �������� �����ϸ� ��
// ���Ӹ���� ���, ���� ���� �� ������ ����, �ߴ�
// ���ӽ�����Ʈ�� ���� �÷��̾ �󸶳� ���Ҵ��� ��ü���� ���� ��Ȳ ��
void ASGameMode::InitGameState()
{
	UE_LOG(LogTemp, Error, TEXT("         Start ASGameMode::        InitGameState()"));
	Super::InitGameState();
	UE_LOG(LogTemp, Error, TEXT("         End   ASGameMode::        InitGameState()"));
}

// ������Ʈ���� ��� �ʱ�ȭ �� �� �۾�
void ASGameMode::PostInitializeComponents()
{
	UE_LOG(LogTemp, Error, TEXT("         Start ASGameMode::        PostInitializeComponents()"));
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Error, TEXT("         End   ASGameMode::        PostInitializeComponents()"));
}

// �α��� ��
void ASGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMassage)
{
	UE_LOG(LogTemp, Error, TEXT("         Start ASGameMode::        PreLogin()"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMassage);
	UE_LOG(LogTemp, Error, TEXT("         End   ASGameMode::        PreLogin()"));
}

// �α��� ��
// UPlayer* NewPlayer ���� �ο��� ����
APlayerController* ASGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Error, TEXT("         Start ASGameMode::        Login()"));
	APlayerController* PlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	UE_LOG(LogTemp, Error, TEXT("         End   ASGameMode::        Login()"));

	return PlayerController;
}

// �α��� ��
void ASGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Error, TEXT("         Start ASGameMode::        PostLogin(ASPlayerController)"));
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Error, TEXT("         End   ASGameMode::        PostLogin(ASPlayerController)"));
}
