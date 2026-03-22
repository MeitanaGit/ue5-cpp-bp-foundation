// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class REPORTWRITE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;	
	// スポナーのクラスをBPで指定できるようにする
	UPROPERTY(EditAnywhere, Category = "Wave Management")
	TSubclassOf<AActor> SpawnerClass;

};
