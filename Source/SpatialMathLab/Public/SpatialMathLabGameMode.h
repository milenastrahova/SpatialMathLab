#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpatialMathLabGameMode.generated.h"

UCLASS()
class SPATIALMATHLAB_API ASpatialMathLabGameMode
    : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASpatialMathLabGameMode();

protected:
    virtual void BeginPlay() override;
};
