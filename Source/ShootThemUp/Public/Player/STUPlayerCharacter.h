// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Camera/CameraComponent.h"
#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

public:
    ASTUPlayerCharacter(const FObjectInitializer& ObjInit);

protected:
    UPROPERTY(EditAnywhere, Category = "View Camera Mode")
    UPlayerViewCameraMode ActiveViewCameraMode;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | First Person Camera Component Settings")
    USpringArmComponent* FPCSpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | First Person Camera Component Settings")
    UCameraComponent* FirstPersonCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | First Person Camera Component Settings")
    USphereComponent* FPCCollisionComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName = "First Person Camera Collision Component Sphere Radius"), Category = "Components | First Person Camera Component Settings")
    float FPCCollisionComponentSphereRadius;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Third Person Camera Component Settings")
    USpringArmComponent* TPCSpringArmComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Third Person Camera Component Settings")
    UCameraComponent* ThirdPersonCamera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName = "Third Person Camera Collision Component Sphere Radius"), Category = "Components | Third Person Camera Component Settings")
    float TPCCollisionComponentSphereRadius;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Third Person Camera Component Settings")
    USphereComponent* TPCCollisionComponent;
    
    virtual void OnDeath() override;
    virtual void BeginPlay() override;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual bool IsRunning() const override;

private:
    bool WantsToRun = false;
    bool IsMovingForward = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void ChangeViewCameraMode();
    
    void OnStartRunning();
    void OnStopRunning();

    void OnStartFire();

    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    USphereComponent* GetActiveCCollisionComponent() const;
      
    UFUNCTION()
    void SetViewCameraMode(UPlayerViewCameraMode NewViewCameraMode);

    void UpdateCameraComponents(UCameraComponent* InCamera);

    void CheckCameraOverlap();
};
