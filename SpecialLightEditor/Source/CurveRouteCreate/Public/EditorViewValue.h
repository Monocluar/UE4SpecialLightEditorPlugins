// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR
#include "Editor.h"
#include "UnrealEd.h"
#include "LevelEditorViewport.h"
#endif // WITH_EDITOR

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EditorViewValue.generated.h"

UCLASS()
class CURVEROUTECREATE_API AEditorViewValue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEditorViewValue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual bool ShouldTickIfViewportsOnly() const override;
#endif

public:

	virtual void OnEditorViewPos(FVector Pos) {};

protected:

	bool bIsGameEnable;
};
