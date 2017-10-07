// Fill out your copyright notice in the Description page of Project Settings.

#include "GridScape.h"
#include "GridScapeGameModeBase.h"

AGridScapeGameModeBase::AGridScapeGameModeBase()
{
	PlayerControllerClass = ADefaultPlayerController::StaticClass();
}