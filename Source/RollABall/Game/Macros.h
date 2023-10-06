#pragma once

#include "CoreMinimal.h"
#include "Macros.h"
#include "Engine/GameEngine.h"

#define D(x) if (GEngine){GEngine->AddOnScreenDebugMessage(-1,2.0f, FColor::Yellow, TEXT(x));}

