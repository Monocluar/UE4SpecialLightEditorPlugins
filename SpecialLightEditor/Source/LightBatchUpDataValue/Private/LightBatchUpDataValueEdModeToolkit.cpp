// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LightBatchUpDataValueEdModeToolkit.h"
#include "LightBatchUpDataValueEdMode.h"
#include "Engine/Selection.h"

#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"

#include "CurveRouteCreate/Public/CurveLightRoute.h"

#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FLightBatchUpDataValueEdModeToolkit"


FLightBatchUpDataValueEdModeToolkit::FLightBatchUpDataValueEdModeToolkit()
{
	Options.Add(MakeShareable(new FString("PointLight")));
	Options.Add(MakeShareable(new FString("SpotLight")));
	Options.Add(MakeShareable(new FString("CurvePointLight")));


	PointLightParametersTitle.Add(TEXT("光照强度倍数"));
	PointLightParametersTitle.Add(TEXT("衰减半径倍数"));
	//ParametersTitle.Add(TEXT("灯光颜色"));

	CurveRouteCreateParametersTitle.Add(TEXT("最小光照强度倍数"));
	CurveRouteCreateParametersTitle.Add(TEXT("最小衰减半径倍数"));

	CurveRouteCreateParametersTitle.Add(TEXT("最大光照强度倍数"));
	CurveRouteCreateParametersTitle.Add(TEXT("最大衰减半径倍数"));

	CurrentOptionPtr = Options[0];
}

void FLightBatchUpDataValueEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{

	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		
	};


	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Fill)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.Padding(0)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("LightSelectTitle", "灯光类型:"))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SComboBox< TSharedPtr<FString> >)
					.OptionsSource(&Options)
					.InitiallySelectedItem(CurrentOptionPtr)
					.OnGenerateWidget(this, &FLightBatchUpDataValueEdModeToolkit::HandleGenerateWidget)
					.OnSelectionChanged(this, &FLightBatchUpDataValueEdModeToolkit::HandleSelectionChanged)
					[
						SNew(STextBlock)
						.Text(this, &FLightBatchUpDataValueEdModeToolkit::GetSelectedSubjectName)
					]
				]
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			.Padding(0.f,20.f)
			.AutoHeight()
			[
				//CreateParametersSet()
				SAssignNew(ParameterWidget, SVerticalBox)
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("ButtonLabel", "应用"))
				.OnClicked(this, &FLightBatchUpDataValueEdModeToolkit::OnButtonClick)
			]
		];

	CreateParametersSet();

	FModeToolkit::Init(InitToolkitHost);
}

FName FLightBatchUpDataValueEdModeToolkit::GetToolkitFName() const
{
	return FName("LightBatchUpDataValueEdMode");
}

FText FLightBatchUpDataValueEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("LightBatchUpDataValueEdModeToolkit", "DisplayName", "LightBatchUpDataValueEdMode Tool");
}

class FEdMode* FLightBatchUpDataValueEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FLightBatchUpDataValueEdMode::EM_LightBatchUpDataValueEdModeId);
}

TSharedRef<SWidget> FLightBatchUpDataValueEdModeToolkit::HandleGenerateWidget(TSharedPtr<FString> Item) const
{
	FString StringItem = Item.IsValid() ? *Item : FString();

	return SNew(STextBlock)
		.Text(FText::FromString(StringItem));
}

void FLightBatchUpDataValueEdModeToolkit::HandleSelectionChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectionType)
{
	CurrentOptionPtr = Item;
	CreateParametersSet();
}

FText FLightBatchUpDataValueEdModeToolkit::GetSelectedSubjectName() const
{
	return  FText::FromString(*CurrentOptionPtr);
}

TSharedRef<SWidget> FLightBatchUpDataValueEdModeToolkit::CreateSSpinBoxWidget(FString Item, int ID) const
{
	TSharedPtr<SSpinBox<float>> MySpinBox;
	TSharedRef<SWidget> SSpinBoxWidget = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.Text(FText::FromString(Item))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SSpinBox<float>)
			.Value(1.f)
			.OnValueCommitted_Raw(this,&FLightBatchUpDataValueEdModeToolkit::HandleOnValueCommitted, ID)
		];

	return SSpinBoxWidget;
}

void FLightBatchUpDataValueEdModeToolkit::HandleOnValueCommitted(float InValue, ETextCommit::Type CommitMethod, int ID)
{
	FString SelectTitle = *CurrentOptionPtr;
	if (SelectTitle == "PointLight" || SelectTitle == "SpotLight")
	{
		switch (ID)
		{
		case 0:
			PointBatchData.LightIntensityMultiple = InValue;
			break;
		case 1:
			PointBatchData.FalloffRadiusMultiple = InValue;
			break;
		default:
			break;
		}
	}
	if (SelectTitle == "CurvePointLight")
	{
		switch (ID)
		{
		case 0:
			MinCurvePointBatchData.LightIntensityMultiple = InValue;
			break;
		case 1:
			MinCurvePointBatchData.FalloffRadiusMultiple = InValue;
			break;
		case 2:
			MaxCurvePointBatchData.LightIntensityMultiple = InValue;
			break;
		case 3:
			MaxCurvePointBatchData.FalloffRadiusMultiple = InValue;
			break;
		default:
			break;
		}
	}
}

void FLightBatchUpDataValueEdModeToolkit::CreateParametersSet()
{
	FString SelectTitle = *CurrentOptionPtr;
	ParameterWidget->ClearChildren();
	if (SelectTitle == "PointLight" || SelectTitle == "SpotLight")
	{
		for (int i = 0; i < PointLightParametersTitle.Num(); i++)
		{
			ParameterWidget->AddSlot()
				.AutoHeight()
				.Padding(0.f, 5.f)
				[
					CreateSSpinBoxWidget(PointLightParametersTitle[i], i)
				];
		}
	}
	if (SelectTitle == "CurvePointLight")
	{
		for (int i = 0; i < CurveRouteCreateParametersTitle.Num(); i++)
		{
			ParameterWidget->AddSlot()
				.AutoHeight()
				.Padding(0.f, 5.f)
				[
					CreateSSpinBoxWidget(CurveRouteCreateParametersTitle[i], i)
				];
		}
	}
}


FReply FLightBatchUpDataValueEdModeToolkit::OnButtonClick()
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	FString SelectTitle = *CurrentOptionPtr;
	if (SelectTitle == "PointLight")
	{
		UpDataPointLightParameters(SelectedActors);
	}
	if (SelectTitle == "SpotLight")
	{
		UpDataSpotLightParameters(SelectedActors);
	}
	if (SelectTitle == "CurvePointLight")
	{
		UpDataCurvePointLightParameters(SelectedActors);
	}

	return FReply::Handled();
}

void FLightBatchUpDataValueEdModeToolkit::UpDataPointLightParameters(USelection* Value)
{
	for (FSelectionIterator Iter(*Value); Iter; ++Iter)
	{

		if (AActor * LevelActor = Cast<AActor>(*Iter))
		{
			TArray<UActorComponent*> ActorComponents = LevelActor->GetComponentsByClass(UPointLightComponent::StaticClass());
			for (auto& Item : ActorComponents)
			{
				UPointLightComponent* PointLightComponent = Cast<UPointLightComponent>(Item);
				if (PointLightComponent)
				{
					float LightIntensity = PointLightComponent->Intensity;
					PointLightComponent->AttenuationRadius *= PointBatchData.FalloffRadiusMultiple;
					float AttenuationRadius = PointLightComponent->AttenuationRadius;
					PointLightComponent->SetIntensity(LightIntensity * PointBatchData.LightIntensityMultiple);
					PointLightComponent->SetAttenuationRadius(AttenuationRadius);
				}

			}

		}
	}
}

void FLightBatchUpDataValueEdModeToolkit::UpDataSpotLightParameters(USelection* Value)
{
	for (FSelectionIterator Iter(*Value); Iter; ++Iter)
	{

		if (AActor * LevelActor = Cast<AActor>(*Iter))
		{
			TArray<UActorComponent*> ActorComponents = LevelActor->GetComponentsByClass(USpotLightComponent::StaticClass());
			for (auto& Item : ActorComponents)
			{
				USpotLightComponent* SpotLightComponent = Cast<USpotLightComponent>(Item);
				if (SpotLightComponent)
				{
					float LightIntensity = SpotLightComponent->Intensity;
					SpotLightComponent->AttenuationRadius *= PointBatchData.FalloffRadiusMultiple;
					float AttenuationRadius = SpotLightComponent->AttenuationRadius;
					SpotLightComponent->SetIntensity(LightIntensity * PointBatchData.LightIntensityMultiple);
					SpotLightComponent->SetAttenuationRadius(AttenuationRadius);
				}

			}

		}
	}
}

void FLightBatchUpDataValueEdModeToolkit::UpDataCurvePointLightParameters(USelection* Value)
{
	for (FSelectionIterator Iter(*Value); Iter; ++Iter)
	{

		if (ACurveLightRoute * LevelActor = Cast<ACurveLightRoute>(*Iter))
		{
			LevelActor->MinCameraPosLightParameter.LightIntensity *= MinCurvePointBatchData.LightIntensityMultiple;
			LevelActor->MinCameraPosLightParameter.FalloffRadius *= MinCurvePointBatchData.FalloffRadiusMultiple;
			LevelActor->MaxCameraPosLightParameter.LightIntensity *= MaxCurvePointBatchData.LightIntensityMultiple;
			LevelActor->MaxCameraPosLightParameter.FalloffRadius *= MaxCurvePointBatchData.FalloffRadiusMultiple;
		}
	}
}



#undef LOCTEXT_NAMESPACE
