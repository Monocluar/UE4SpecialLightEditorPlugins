// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"


struct FPointBatchValue
{
	// 光照强度倍数
	float LightIntensityMultiple = 1.f;
	// 衰减半径倍数
	float FalloffRadiusMultiple = 1.f;
	// 光照颜色
	FLinearColor LightColor = FLinearColor(255.f, 255.f, 255.f, 0.f);
};


class FLightBatchUpDataValueEdModeToolkit : public FModeToolkit
{
public:

	FLightBatchUpDataValueEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

private:

	TSharedPtr<SWidget> ToolkitWidget;

	TSharedPtr<SVerticalBox> ParameterWidget;

	// 点光源数据
	FPointBatchValue PointBatchData;

	// 最小LOD灯光数据
	FPointBatchValue MinCurvePointBatchData;

	// 最大LOD灯光数据
	FPointBatchValue MaxCurvePointBatchData;

protected:
	/** The true objects bound to the Slate combobox. */
	TArray< TSharedPtr<FString> > Options;

	/** A shared pointer to the current selected string */
	TSharedPtr<FString> CurrentOptionPtr;

	// 更改的点光源参数集名称
	TArray<FString> PointLightParametersTitle;

	// 更改的LOD灯光参数集名称
	TArray<FString> CurveRouteCreateParametersTitle;

protected:
	/** Called by slate when it needs to generate a new item for the combobox */
	virtual TSharedRef<SWidget> HandleGenerateWidget(TSharedPtr<FString> Item) const;

	/** Called by slate when the underlying combobox selection changes */
	virtual void HandleSelectionChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectionType);

	FText GetSelectedSubjectName() const;

	virtual TSharedRef<SWidget> CreateSSpinBoxWidget(FString Item, int ID) const;

	void HandleOnValueCommitted(float InValue, ETextCommit::Type CommitMethod, int ID);

	void CreateParametersSet();

	FReply OnButtonClick();

	// 更改点光源参数集
	void UpDataPointLightParameters(USelection* Value);

	// 更改点光源参数集
	void UpDataSpotLightParameters(USelection* Value);

	// 更改LOD点光源参数集
	void UpDataCurvePointLightParameters(USelection* Value);
};
