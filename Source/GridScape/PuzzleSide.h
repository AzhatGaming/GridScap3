// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Side.h"

#include "GameFramework/Actor.h"
#include "PuzzleSide.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSideClickEvent);

UCLASS()
class GRIDSCAPE_API APuzzleSide : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APuzzleSide();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Setup)
		UBoxComponent* Box;

	// Set the side
	void SetSide(uint8 newSide);
	uint8 GetSide();

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FSideClickEvent OnSideClick;
	UFUNCTION()
		void OnSideClicked(AActor* TouchedActor, FKey ButtonClicked);
	UFUNCTION()
		void OnSideTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

private:
	uint8 thisSide;
};
