#include "SpatialMathCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"

ASpatialMathCameraPawn::ASpatialMathCameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot =
        CreateDefaultSubobject<USceneComponent>(
            TEXT("SceneRoot")
        );

    SetRootComponent(SceneRoot);

    Camera =
        CreateDefaultSubobject<UCameraComponent>(
            TEXT("Camera")
        );

    Camera->SetupAttachment(SceneRoot);
    Camera->SetRelativeLocation(FVector::ZeroVector);
    Camera->SetRelativeRotation(FRotator::ZeroRotator);

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASpatialMathCameraPawn::BeginPlay()
{
    Super::BeginPlay();

    ApplyCameraPreset(0);
}

void ASpatialMathCameraPawn::Tick(
    const float DeltaSeconds
)
{
    Super::Tick(DeltaSeconds);

    UpdateMovement(DeltaSeconds);
    UpdateRotation();
    UpdateCameraPresets();
}

void ASpatialMathCameraPawn::UpdateMovement(
    const float DeltaSeconds
)
{
    APlayerController* PlayerController =
        Cast<APlayerController>(GetController());

    if (!PlayerController)
    {
        return;
    }

    FVector Movement = FVector::ZeroVector;

    if (PlayerController->IsInputKeyDown(EKeys::W))
    {
        Movement += GetActorForwardVector();
    }

    if (PlayerController->IsInputKeyDown(EKeys::S))
    {
        Movement -= GetActorForwardVector();
    }

    if (PlayerController->IsInputKeyDown(EKeys::D))
    {
        Movement += GetActorRightVector();
    }

    if (PlayerController->IsInputKeyDown(EKeys::A))
    {
        Movement -= GetActorRightVector();
    }

    if (PlayerController->IsInputKeyDown(EKeys::E))
    {
        Movement += FVector::UpVector;
    }

    if (PlayerController->IsInputKeyDown(EKeys::Q))
    {
        Movement -= FVector::UpVector;
    }

    if (Movement.IsNearlyZero())
    {
        return;
    }

    const bool bFastMovement =
        PlayerController->IsInputKeyDown(
            EKeys::LeftShift
        ) ||
        PlayerController->IsInputKeyDown(
            EKeys::RightShift
        );

    const float Speed =
        MoveSpeed *
        (
            bFastMovement
                ? FastMoveMultiplier
                : 1.0f
        );

    AddActorWorldOffset(
        Movement.GetSafeNormal() *
        Speed *
        DeltaSeconds,
        false
    );
}

void ASpatialMathCameraPawn::UpdateRotation()
{
    APlayerController* PlayerController =
        Cast<APlayerController>(GetController());

    if (!PlayerController ||
        !PlayerController->IsInputKeyDown(
            EKeys::RightMouseButton
        ))
    {
        return;
    }

    float MouseX = 0.0f;
    float MouseY = 0.0f;

    PlayerController->GetInputMouseDelta(
        MouseX,
        MouseY
    );

    FRotator Rotation = GetActorRotation();

    Rotation.Yaw += MouseX * MouseSensitivity;

    Rotation.Pitch = FMath::Clamp(
        Rotation.Pitch -
        MouseY *
        MouseSensitivity,
        -85.0f,
        85.0f
    );

    Rotation.Roll = 0.0f;

    SetActorRotation(Rotation);
}

void ASpatialMathCameraPawn::UpdateCameraPresets()
{
    APlayerController* PlayerController =
        Cast<APlayerController>(GetController());

    if (!PlayerController)
    {
        return;
    }

    if (PlayerController->WasInputKeyJustPressed(EKeys::Zero))
    {
        ApplyCameraPreset(0);
    }
    else if (PlayerController->WasInputKeyJustPressed(EKeys::One))
    {
        ApplyCameraPreset(1);
    }
    else if (PlayerController->WasInputKeyJustPressed(EKeys::Two))
    {
        ApplyCameraPreset(2);
    }
    else if (PlayerController->WasInputKeyJustPressed(EKeys::Three))
    {
        ApplyCameraPreset(3);
    }
    else if (PlayerController->WasInputKeyJustPressed(EKeys::Four))
    {
        ApplyCameraPreset(4);
    }
    else if (PlayerController->WasInputKeyJustPressed(EKeys::Five))
    {
        ApplyCameraPreset(5);
    }
}

void ASpatialMathCameraPawn::ApplyCameraPreset(
    const int32 PresetIndex
)
{
    FVector Location = FVector::ZeroVector;
    FRotator Rotation = FRotator::ZeroRotator;
    FString PresetName;

    switch (PresetIndex)
    {
        case 1:
        {
            Location =
                FVector(-2100.0f, -1050.0f, 650.0f);

            Rotation =
                FRotator(-18.0f, 90.0f, 0.0f);

            PresetName = TEXT("01 Vector Projection");
            break;
        }

        case 2:
        {
            Location =
                FVector(-1050.0f, -500.0f, 650.0f);

            Rotation =
                FRotator(-18.0f, 90.0f, 0.0f);

            PresetName = TEXT("02 Quaternion SLERP");
            break;
        }

        case 3:
        {
            Location =
                FVector(0.0f, -1050.0f, 650.0f);

            Rotation =
                FRotator(-18.0f, 90.0f, 0.0f);

            PresetName = TEXT("03 Ray Intersections");
            break;
        }

        case 4:
        {
            Location =
                FVector(1050.0f, -500.0f, 650.0f);

            Rotation =
                FRotator(-18.0f, 90.0f, 0.0f);

            PresetName = TEXT("04 Target Selection");
            break;
        }

        case 5:
        {
            Location =
                FVector(2200.0f, -1050.0f, 720.0f);

            Rotation =
                FRotator(-20.0f, 90.0f, 0.0f);

            PresetName = TEXT("05 Ballistic Solutions");
            break;
        }

        case 0:
        default:
        {
            Location =
                FVector(100.0f, -4300.0f, 1700.0f);

            Rotation =
                FRotator(-18.0f, 90.0f, 0.0f);

            PresetName = TEXT("Overview");
            break;
        }
    }

    SetActorLocation(Location);
    SetActorRotation(Rotation);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.5f,
            FColor::White,
            FString::Printf(
                TEXT(
                    "Camera: %s | 0 overview | 1-5 stations"
                ),
                *PresetName
            )
        );
    }
}