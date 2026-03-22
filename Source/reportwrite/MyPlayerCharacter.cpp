// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h" // これが ETriggerEvent の正体
#include "GameFramework/CharacterMovementComponent.h" // これが必要
// 冒頭に弾丸クラスのヘッダーを追加
#include "MyBullet.h"
#include "MyUserWidget.h" // 作成したWidgetのヘッダー
#include "Blueprint/UserWidget.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// キャラクターの回転に合わせてカメラが回らないように設定
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 2. 移動方向にキャラクターを自動で向かせる設定
	GetCharacterMovement()->bOrientRotationToMovement = true; 
    
	// 3. 回転の速さを設定（数値が大きいほどクイックに振り向く）
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
	
	// アーム（CameraBoom）を作成
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;       // キャラとの距離
	CameraBoom->bUsePawnControlRotation = true; // マウスでカメラを回転させる

	//　衝突無効処理追加
	CameraBoom->bDoCollisionTest = false; // カメラの衝突判定を無効にする
	
	// カメラを作成し、アームの先端に取り付ける
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void AMyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// プレイヤーコントローラーを取得して、Enhanced Inputのサブシステムに今回の設定(MappingContext)を登録
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// DefaultMappingContext を優先度0で追加
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	// UIを生成して画面に表示（Viewportへ追加）
	if (HUDClass)
	{
		HUDInstance = CreateWidget<UMyUserWidget>(GetWorld(), HUDClass);
		if (HUDInstance)
		{
			HUDInstance->AddToViewport();
			HUDInstance->UpdateScoreDisplay(CurrentScore); // 初期値0を表示
		}
	}

}

// Called every frame
void AMyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerCharacter::Move);
	}
	// 弾丸発射周りの処理
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// FireAction（ボタン）が押された(Started)時に Fire() を呼ぶ
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMyPlayerCharacter::Fire);
	}

	// 弾丸切り替え周りの処理(弾丸発射処理の改変)
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// SwitchBulletAction（ボタン）が押された(Started)時に SwitchBullet() を呼ぶ
		EnhancedInputComponent->BindAction(SwitchBulletAction,
			ETriggerEvent::Started, this, &AMyPlayerCharacter::SwitchBullet);
	}
}
// 実際の移動処理（前述のワールド座標基準）
void AMyPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddMovementInput(FVector(1.f, 0.f, 0.f), MovementVector.Y); // 前後
		AddMovementInput(FVector(0.f, 1.f, 0.f), MovementVector.X); // 左右
	}
}
// 発射処理の実装
void AMyPlayerCharacter::Fire(const FInputActionValue& Value)
{
	//ここも変更
	if (BulletClasses.IsValidIndex(SelectedBulletIndex) && BulletClasses[SelectedBulletIndex])
	{
		// 弾を出す位置（プレイヤーの少し前）
		FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f);
		// 弾を出す向き（プレイヤーの向き）
		FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 配列から選ばれた弾を生成！
		GetWorld()->SpawnActor<AMyBullet>(BulletClasses[SelectedBulletIndex], SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AMyPlayerCharacter::AddScore(int32 Amount)
{
	CurrentScore += Amount;

	// UIの表示を更新
	if (HUDInstance)
	{
		HUDInstance->UpdateScoreDisplay(CurrentScore);
	}
	// ログに出力して確認
	UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), CurrentScore);
}
void AMyPlayerCharacter::SwitchBullet()
{
	// 例外処理
	if (SelectedBulletIndex < 0)
	{
		SelectedBulletIndex = 0;
	}
	//Indexを1進める
	SelectedBulletIndex++;
	// 現在は0か1しかないので、2以上になったら0に戻す
	if (SelectedBulletIndex >=2)
	{
		SelectedBulletIndex = 0;
	}
	// ログに出力して確認
	UE_LOG(LogTemp, Warning, TEXT("Bullet Index: %d"), SelectedBulletIndex);
}


