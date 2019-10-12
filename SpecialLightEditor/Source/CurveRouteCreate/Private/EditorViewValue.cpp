// Fill out your copyright notice in the Description page of Project Settings.
#include "EditorViewValue.h"




// Sets default values
AEditorViewValue::AEditorViewValue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEditorViewValue::BeginPlay()
{
	Super::BeginPlay();

	bIsGameEnable = true;
}

// Called every frame
void AEditorViewValue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if WITH_EDITOR
	if (!bIsGameEnable)
	{
		FLevelEditorViewportClient* Client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
		if (Client)
		{
			FVector ViewLoction = Client->GetViewLocation();
			FVector ActorLoction = GetActorLocation();
			float Lenght = (ViewLoction - ActorLoction).Size();
			OnEditorViewPos(ViewLoction);
		}
	}
	
#endif // WITH_EDITOR

}

#if WITH_EDITOR
bool AEditorViewValue::ShouldTickIfViewportsOnly() const
{
	return !bIsGameEnable;
}
#endif

