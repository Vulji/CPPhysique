#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Ball.generated.h"

UCLASS()
class CPPHYSIQUE_API ABall : public APawn
{
	GENERATED_BODY()

public:
	ABall();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, 
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	void Move(const FInputActionValue& Value);
	void Jump();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveForce = 1000000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
	float JumpImpulse = 800000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
	int MaxJumps = 2; 

private:
	int JumpCount = 0;
	bool bIsGrounded = true;
};
