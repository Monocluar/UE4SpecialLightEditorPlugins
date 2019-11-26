// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EditorViewValue.h"
#include "CurveLightRoute.generated.h"

class UCurveFloat;
class USplineComponent;
class UPointLightComponent;



USTRUCT(BlueprintType)
struct FLightParameterSetting
{
	GENERATED_BODY()

	// 光照强度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightParameterSetting)
	float LightIntensity = 8.f;

	// 光源半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightParameterSetting)
		float LightRadius = 0;

	// 光源长度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightParameterSetting)
		float SourceLength = 0;

	// 衰减半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightParameterSetting)
		float FalloffRadius = 1000.f;

	// 光照颜色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightParameterSetting)
	FLinearColor LightColor = FLinearColor(255.f, 255.0, 255.0, 0.f);

	// 软光源范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightParameterSetting)
		float SoftLightRange = 0;

	// 间接光源强度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightParameterSetting)
		float IndirectLightIntensity = 1.f;

	// 灯光角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightParameterSetting)
		FRotator LightRotation = FRotator::ZeroRotator;

};

UENUM()
enum ELightDataType
{

	/** 根据Spline线的点数生成灯光 */
	SplinePoint_One UMETA(DisplayName = "点-光对等"),
	/** 根据Spline路径数生成等距灯光 */
	SplinePoint_Two UMETA(DisplayName = "线-光对等"),
	/** 根据Spline线的点数生成灯光，并且根据相机和自身距离值改变参数 */
	SplinePoint_Three UMETA(DisplayName = "点-光对等(LOD)"),
	/** 根据Spline路径数生成等距灯光，并且根据相机和自身距离值改变参数 */
	SplinePoint_Four UMETA(DisplayName = "线-光对等(LOD)")
};

UCLASS()
class CURVEROUTECREATE_API ACurveLightRoute : public AEditorViewValue
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACurveLightRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnEditorViewPos(FVector Pos) override;

#if WITH_EDITOR
	// 当编辑器参数改变时调用
	virtual void PreEditChange(UProperty* PropertyThatWillChange) override;
	virtual bool CanEditChange(const UProperty* InProperty) const override;
#endif // WITH_EDITOR

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Utilities|Transformation")
	USplineComponent* SplineComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Pawn")
	APawn* MyPawn;

protected:

	// 设置是否开启阴影（默认关闭阴影）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseSetting")
	bool bOpenShadow;

	// 是否开启色温（默认关闭）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseSetting")
	bool bOpenColorTemperature;

	// 设置色温值
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseSetting", AdvancedDisplay, meta = (editcondition = "bOpenColorTemperature", UIMin = "1700", UIMax = "12000"))
	float ColorTemperatureValue;


public:

	// 灯光类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LightSetting")
	TEnumAsByte<enum ELightDataType> SplineLightType;

	// 过度曲线
	UPROPERTY(EditAnywhere, Category = "LightSetting")
	UCurveFloat* CameraPosValue;

	// 最小相机距离物体位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSetting")
	float MinCameraPos;

	// 最小灯光数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSetting", meta = (UIMin = "1"))
	int MinLightNumber;

	// 最小距离的灯光设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSetting")
	FLightParameterSetting MinCameraPosLightParameter;

	// 最大相机距离物体位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSetting")
	float MaxCameraPos;

	// 最大灯光数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSetting", meta = (UIMin = "1"))
	int MaxLightNumber;

	// 最大距离的灯光设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSetting")
	FLightParameterSetting MaxCameraPosLightParameter;

protected:

	// 所有灯光Component
	TArray<UPointLightComponent*> PointLights;

	// 是否开启灯光更新
	bool bIsEnableLightUpData = true;

public:

	virtual FTransform GetSplineLightComponentLocation(int Index ,int PointNum , FRotator LightRotation) ;

	// 获取相机与最大最小值得比例(游戏模式)
	virtual float GetGameCameraAndCurveValue();

	// 获取相机与最大最小值得比例(编辑器模式)
	virtual float GetEditorCameraAndCurveValue(FVector ViewPos);

protected:

	// 判定当前位置应生成的灯光数 (ViewPos : 编辑器视口位置)
	int GetAddPointLightIndex(FVector ViewPos, float& Alpha);

	// 创建点光源数
	void CreatePointLightComponent(int Index);

	// 设置灯光参数
	void SetPointLightParameter(float Alpha, UPointLightComponent* Component, int Index);

	// 更新灯光参数
	void UpDataPointLightComponent(UPointLightComponent* Component, int Index, FLightParameterSetting LightParameter);

};
