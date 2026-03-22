// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyEnemy.h"

#include "MyEnemySpawner.generated.h"

UCLASS()
class REPORTWRITE_API AMyEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// スポーンさせる敵のクラス（BPで選択可能にする）
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AMyEnemy> EnemyClass;

	// スポーン間隔（秒）
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval = 1.0f;

	// プレイヤーからの距離（画面外に設定）
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnRadius = 1000.0f;

	// 実際にスポーンさせる関数
	void SpawnEnemy();

	// タイマーハンドル
	FTimerHandle SpawnTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
