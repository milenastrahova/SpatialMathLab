#include "SpatialMathLabGameMode.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SpatialMathCameraPawn.h"
#include "SpatialMathLabShowcaseActor.h"

ASpatialMathLabGameMode::ASpatialMathLabGameMode()
{
    DefaultPawnClass = ASpatialMathCameraPawn::StaticClass();
}

void ASpatialMathLabGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (!GetWorld())
    {
        return;
    }

    TArray<AActor*> ExistingShowcases;

    UGameplayStatics::GetAllActorsOfClass(
        this,
        ASpatialMathLabShowcaseActor::StaticClass(),
        ExistingShowcases
    );

    if (ExistingShowcases.IsEmpty())
    {
        GetWorld()->SpawnActor<
            ASpatialMathLabShowcaseActor
        >(
            ASpatialMathLabShowcaseActor::StaticClass(),
            FVector::ZeroVector,
            FRotator::ZeroRotator
        );
    }
}