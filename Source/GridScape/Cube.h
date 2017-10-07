// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Cube.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCubeClickEvent, FVector, ClickedCubeCoordinates);

UCLASS()
class GRIDSCAPE_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACube();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = Setup)
		UStaticMeshComponent* Cube;

	FVector GetCoordinate();
	void SetCoordinate(FVector newCoordinate);

	UMaterial* GetMaterial(int32 Channel);
	void SetMaterial(UMaterialInstanceDynamic* Material, int32 Channel);

	// Click delegate
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FCubeClickEvent OnCubeClick;
	UFUNCTION()
		void OnCubeClicked(AActor* TouchedActor, FKey ButtonClicked);
	UFUNCTION()
		void OnCubeTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

private:
	// The coordinate of this cube
	FVector Coordinate;
	
};
