// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnerClass)
	{
		// ゲーム開始時にスポナーを1つ生成する
		GetWorld()->SpawnActor<AActor>(SpawnerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}
}