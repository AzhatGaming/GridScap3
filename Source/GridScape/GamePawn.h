// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridScapeGameController.h"

#include "GameFramework/Pawn.h"
#include "GamePawn.generated.h"

UCLASS()
class GRIDSCAPE_API AGamePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGamePawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Called when we have a touch/click event
	void SetSideLocked();

	// Grabs the spring arm component so we can zoom in and out
	USpringArmComponent* GetSpringArmComponent();

protected:
	UPROPERTY(Category = Setup, EditAnywhere)
		USpringArmComponent* SpringArm;
	UPROPERTY(Category = Setup, EditAnywhere)
		UCameraComponent* Camera;
	UPROPERTY(Category = Game, EditAnywhere)
		AGridScapeGameController* GameController;
};
