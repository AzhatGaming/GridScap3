// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Barrier.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBarrierClickEvent, int32, BarrierIndex);

UCLASS()
class GRIDSCAPE_API ABarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarrier();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = Setup)
		UStaticMeshComponent* Barrier;

	int32 GetIndex();
	void SetIndex(int32 Index);

	UMaterial* GetMaterial(int32 Channel);
	void SetMaterial(UMaterialInstanceDynamic* Material, int32 Channel);
	
	// Click delegate
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FBarrierClickEvent OnBarrierClick;
	UFUNCTION()
		void OnBarrierClicked(AActor* TouchedActor, FKey ButtonClicked);
	UFUNCTION()
		void OnBarrierTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

private:
	// Index of the barrier
	int32 Index;
};
