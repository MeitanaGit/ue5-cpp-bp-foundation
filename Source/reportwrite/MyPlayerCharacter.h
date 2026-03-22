// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h" //Geminiが必要と言ったヘッダー

#include "MyPlayerCharacter.generated.h"

// 冒頭に前方宣言を追加
class UMyUserWidget;

UCLASS()
class REPORTWRITE_API AMyPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayerCharacter();

	// スコアを加算する関数（外部から呼べるように public にします）
	void AddScore(int32 Amount);
	
	//移動処理作成時に作ったやつ　MyInputは"MyInput"の方が良いよ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyInput")
	class UInputMappingContext* DefaultMappingContext;
	//移動処理作成時に作ったやつ　MyInputは"MyInput"の方が良いよ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyInput")
	class UInputAction* MoveAction;	

	// この下が今追加した発射系のコード
	// 発射ボタン用のアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyInput")
	class UInputAction* FireAction;

	//　弾丸切り替え用アクション、発射ボタンアクションの名前いじっただけ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyInput")
	class UInputAction* SwitchBulletAction;

	// どの弾を撃つか（さっき作った AMyBullet クラスを指定するため）
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
//	TSubclassOf<class AMyBullet> BulletClass;

	// 複数の弾丸クラスを登録できる配列に変更
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<TSubclassOf<class AMyBullet>> BulletClasses;

	// 現在選択中の弾の番号（0番目, 1番目...）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 SelectedBulletIndex = 0;

	// 弾を切り替える関数（必要であれば）
	void SwitchBullet();	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 移動用の関数　Geminiが…
	void Move(const FInputActionValue& Value);
	
	// カメラを支えるアーム（これがないとカメラがキャラに埋まってしまいます）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	// 実際のカメラ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	// 発射関数
	void Fire(const FInputActionValue& Value);

	
	// BP側で「どのWidgetを使うか」選択するための変数
	UPROPERTY(EditAnywhere, Category = "MyUI")
	TSubclassOf<UMyUserWidget> HUDClass;

	// 生成されたUIの実体（インスタンス）
	UPROPERTY()
	UMyUserWidget* HUDInstance;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 現在のスコアを保持する変数
	UPROPERTY(VisibleAnywhere, Category = "MyStats")
	int32 CurrentScore = 0;
};
