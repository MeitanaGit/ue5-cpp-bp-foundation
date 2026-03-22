// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyExpOrb.generated.h"

UCLASS()
class REPORTWRITE_API AMyExpOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyExpOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 衝突判定（これに触れたら取得）
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* CollisionComponent;

	// 見た目（黄色い球体）
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* OrbMesh;

	// 吸引が開始される距離
	UPROPERTY(EditAnywhere, Category = "Orb Stats")
	float MagnetRange = 500.0f;

	// 吸引スピード
	UPROPERTY(EditAnywhere, Category = "Orb Stats")
	float MoveSpeed = 600.0f;

	// プレイヤーへの参照
	UPROPERTY()
	class APawn* TargetPlayer;

	// 衝突時のイベント関数
	UFUNCTION()
	void OnOrbOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
