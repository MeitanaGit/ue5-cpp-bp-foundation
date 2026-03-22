// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" // 必須
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class REPORTWRITE_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// meta = (BindWidget) を付けることで、BP側の同名パーツと自動で紐付きます
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	// スコアの数字を更新する関数
	void UpdateScoreDisplay(int32 NewScore);
	
};

