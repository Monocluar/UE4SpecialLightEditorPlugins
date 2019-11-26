// Fill out your copyright notice in the Description page of Project Settings.

#include "CurveLightRoute.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/SplineComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ACurveLightRoute::ACurveLightRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ColorTemperatureValue = 6500.f;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->SetupAttachment(RootComponent);

	MinLightNumber = 2;
	MaxLightNumber = 2;
}

// Called when the game starts or when spawned
void ACurveLightRoute::BeginPlay()
{
	Super::BeginPlay();
	MyPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (SplineLightType == SplinePoint_Three)
	{
		CreatePointLightComponent(SplineComponent->GetNumberOfSplinePoints());
	}

}

// Called every frame
void ACurveLightRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsGameEnable && SplineLightType == SplinePoint_Three || SplineLightType == SplinePoint_Four)
	{
		float Alpha = 0;
		if (SplineLightType == SplinePoint_Three)
		{
			GetAddPointLightIndex(FVector::ZeroVector, Alpha);
		}
		else
		{
			CreatePointLightComponent(GetAddPointLightIndex(FVector::ZeroVector, Alpha));
		}
		
		//float Alpha = GetEditorCameraAndCurveValue(Pos);


		if (bIsEnableLightUpData)
		{

			for (int i = 0; i < PointLights.Num(); i++)
			{
				SetPointLightParameter(Alpha, PointLights[i], i);
			}

		}
		bIsEnableLightUpData = !(Alpha == 1 || Alpha == 0);
	}
	
}

void ACurveLightRoute::OnEditorViewPos(FVector Pos)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Pos.ToString());
	float Alpha = 0;
	switch (SplineLightType)
	{
	case SplinePoint_One:
		CreatePointLightComponent(SplineComponent->GetNumberOfSplinePoints());
		break;
	case SplinePoint_Two:
		CreatePointLightComponent(MaxLightNumber);
		break;
	case SplinePoint_Three:
		CreatePointLightComponent(SplineComponent->GetNumberOfSplinePoints());
		GetAddPointLightIndex(Pos, Alpha);
		break;
	case SplinePoint_Four:
		CreatePointLightComponent(GetAddPointLightIndex(Pos, Alpha));
		break;

	}
	//float Alpha = GetEditorCameraAndCurveValue(Pos);
	if (SplineLightType == SplinePoint_One || SplineLightType == SplinePoint_Two)
	{
		for (int i = 0; i < PointLights.Num(); i++)
		{
			UpDataPointLightComponent(PointLights[i], i, MaxCameraPosLightParameter);
		}
		return;
	}
	if (SplineLightType == SplinePoint_Three || SplineLightType == SplinePoint_Four)
	{
		for (int i = 0; i < PointLights.Num(); i++)
		{
			SetPointLightParameter(Alpha, PointLights[i], i);
		}
	}
	
}

#if WITH_EDITOR
void ACurveLightRoute::PreEditChange(UProperty* PropertyThatWillChange)
{
	MaxCameraPos = FMath::Max(MaxCameraPos, MinCameraPos);

}
bool ACurveLightRoute::CanEditChange(const UProperty* InProperty) const
{
	if (InProperty)
	{
		FString PropertyName = InProperty->GetName();
		switch (SplineLightType)
		{
		case SplinePoint_One:
			if (FCString::Strcmp(*PropertyName, TEXT("CameraPosValue")) == 0
				|| FCString::Strcmp(*PropertyName, TEXT("MinCameraPos")) == 0
				|| FCString::Strcmp(*PropertyName, TEXT("MinLightNumber")) == 0
				|| FCString::Strcmp(*PropertyName, TEXT("MinCameraPosLightParameter")) == 0
				|| FCString::Strcmp(*PropertyName, TEXT("MaxCameraPos")) == 0
				|| FCString::Strcmp(*PropertyName, TEXT("MaxLightNumber")) == 0)
			{
				return false;
			}
			break;
		case SplinePoint_Two:
			if (FCString::Strcmp(*PropertyName, TEXT("MinCameraPos")) == 0
				|| FCString::Strcmp(*PropertyName, TEXT("MinLightNumber")) == 0
				|| FCString::Strcmp(*PropertyName, TEXT("MinCameraPosLightParameter")) == 0
				|| FCString::Strcmp(*PropertyName, TEXT("MaxCameraPos")) == 0)
			{
				return false;
			}
			break;
		case SplinePoint_Three:
			if (FCString::Strcmp(*PropertyName, TEXT("MinLightNumber")) == 0
				|| FCString::Strcmp(*PropertyName, TEXT("MaxLightNumber")) == 0)
			{
				return false;
			}
			break;
		case SplinePoint_Four:
			break;
		}
	}
	return Super::CanEditChange(InProperty);
}
#endif

FTransform ACurveLightRoute::GetSplineLightComponentLocation(int Index, int PointNum, FRotator LightRotation)
{
	float SplineLength = SplineComponent->GetSplineLength();
	float Remainder;
	float Distance = UKismetMathLibrary::FMod(SplineLength, PointNum, Remainder) * Index;
	FVector SplineLocation = SplineLightType == SplinePoint_One || SplineLightType == SplinePoint_Three
		? SplineComponent->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::Local) 
		: SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
	FRotator SplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
	FRotator NewLightRotation = UKismetMathLibrary::ComposeRotators(SplineRotation, LightRotation);
	FTransform NewTransform = FTransform();
	NewTransform.SetLocation(SplineLocation);
	NewTransform.SetRotation(NewLightRotation.Quaternion());

	return NewTransform;
}

float ACurveLightRoute::GetGameCameraAndCurveValue()
{
	if (!MyPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map don't have Pawn"));
		return 0.f;
	}
	USpringArmComponent* Arm = Cast<USpringArmComponent>(MyPawn->GetComponentByClass(USpringArmComponent::StaticClass()));
	float ArmLength = Arm ? Arm->TargetArmLength : 0.f;
	float ActorLength = (MyPawn->GetActorLocation() - this->GetActorLocation()).Size();
	float SliderLength = FMath::Max(((ActorLength + ArmLength) - MinCameraPos), 0.f);
	return SliderLength / (MaxCameraPos - MinCameraPos);
}

float ACurveLightRoute::GetEditorCameraAndCurveValue(FVector ViewPos)
{
	float ActorLength = (ViewPos - this->GetActorLocation()).Size();
	float SliderLength = FMath::Max((ActorLength - MinCameraPos), 0.f);
	return SliderLength / (MaxCameraPos - MinCameraPos);
}

int ACurveLightRoute::GetAddPointLightIndex(FVector ViewPos, float& Alpha)
{
	int LightNumberDir = FMath::Abs(MaxLightNumber - MinLightNumber);
	//float SliderScale = 0;
	if (CameraPosValue)
	{
		Alpha = CameraPosValue->GetFloatValue(bIsGameEnable ? GetGameCameraAndCurveValue() : GetEditorCameraAndCurveValue(ViewPos));
	}

	int LightIndex = FMath::TruncToInt(FMath::Lerp(MaxLightNumber, MinLightNumber, Alpha));

	return LightIndex;
}

void ACurveLightRoute::CreatePointLightComponent(int Index)
{
	
	if (PointLights.Num() != Index)
	{
		//int LightsIndex = PointLights.IsValidIndex(0) ? PointLights.Num() : 0;
		int Dir = PointLights.Num() - Index;
		if (Dir > 0)
		{
			for (int i = 0; i < Dir; i++)
			{
				if(PointLights.Num() > i)
				{
					PointLights[i]->DestroyComponent();
					PointLights.RemoveAt(i);
				}
				
			}
		}
		else
		{
			for (int i = 0; i < -Dir; i++)
			{
				
				//UPointLightComponent* PointLight = CreateDefaultSubobject<UPointLightComponent>(FName(*PointName));RegisterComponent
				UPointLightComponent* PointLight = NewObject<UPointLightComponent>(this, UPointLightComponent::StaticClass());
			
				PointLight->SetupAttachment(RootComponent);
				PointLights.Add(PointLight);
				PointLight->SetVisibility(true);
				PointLight->RegisterComponent();
			}
		}
	}
	
}

void ACurveLightRoute::SetPointLightParameter(float Alpha, UPointLightComponent* Component, int Index)
{
	FLightParameterSetting Max = MaxCameraPosLightParameter;
	FLightParameterSetting Min = MinCameraPosLightParameter;
	FLightParameterSetting LightParameter;
	LightParameter.LightIntensity = FMath::Lerp(Max.LightIntensity, Min.LightIntensity,Alpha);
	LightParameter.LightColor = FMath::Lerp(Max.LightColor, Min.LightColor, Alpha);
	LightParameter.LightRadius = FMath::Lerp(Max.LightRadius, Min.LightRadius, Alpha);
	LightParameter.SourceLength = FMath::Lerp(Max.SourceLength, Min.SourceLength, Alpha);
	LightParameter.FalloffRadius = FMath::Lerp(Max.FalloffRadius, Min.FalloffRadius, Alpha);
	LightParameter.SoftLightRange = FMath::Lerp(Max.SoftLightRange, Min.SoftLightRange, Alpha);
	LightParameter.IndirectLightIntensity = FMath::Lerp(Max.IndirectLightIntensity, Min.IndirectLightIntensity, Alpha);
	LightParameter.LightRotation = FMath::Lerp(Max.LightRotation, Min.LightRotation, Alpha);
	UpDataPointLightComponent(Component, Index, LightParameter);
}

void ACurveLightRoute::UpDataPointLightComponent(UPointLightComponent* Component, int Index, FLightParameterSetting LightParameter)
{
	if (Component)
	{
		Component->SetRelativeTransform(this->GetSplineLightComponentLocation(Index, PointLights.Num() - 1, LightParameter.LightRotation));
		Component->SetIntensity(LightParameter.LightIntensity);
		Component->SetLightColor(LightParameter.LightColor);
		Component->SetSourceLength(LightParameter.SourceLength);
		Component->SetSourceRadius(LightParameter.LightRadius);
		Component->SetAttenuationRadius(LightParameter.FalloffRadius);
		Component->SetSoftSourceRadius(LightParameter.SoftLightRange);
		Component->SetIndirectLightingIntensity(LightParameter.IndirectLightIntensity);

		Component->SetCastShadows(bOpenShadow);
		if (bOpenColorTemperature)
		{
			Component->SetTemperature(ColorTemperatureValue);
		}
	}
}




