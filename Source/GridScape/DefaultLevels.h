// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LevelStruct.h"

#include "GameFramework/SaveGame.h"
#include "DefaultLevels.generated.h"

/**
 * 
 */
UCLASS()
class GRIDSCAPE_API UDefaultLevels : public USaveGame
{
	GENERATED_BODY()
	
public:
	UDefaultLevels();

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		TArray<FLevelStruct> DefaultLevels;
};
