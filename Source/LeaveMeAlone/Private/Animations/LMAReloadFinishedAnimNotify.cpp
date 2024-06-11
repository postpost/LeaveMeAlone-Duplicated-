// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Animations/LMAReloadFinishedAnimNotify.h"

void ULMAReloadFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
	OnNotifyReloadFinished.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}
