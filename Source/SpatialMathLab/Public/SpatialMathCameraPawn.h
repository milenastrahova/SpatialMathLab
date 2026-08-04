#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpatialMathCameraPawn.generated.h"

class UCameraComponent;
class USceneComponent;

UCLASS()
class SPATIALMATHLAB_API ASpatialMathCameraPawn
    : public APawn
{
    GENERATED_BODY()

public:
    ASpatialMathCameraPawn();

    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float MoveSpeed = 900.0f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float FastMoveMultiplier = 3.0f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float MouseSensitivity = 0.12f;

    void UpdateMovement(float DeltaSeconds);
    void UpdateRotation();
    void UpdateCameraPresets();
    void ApplyCameraPreset(int32 PresetIndex);
};