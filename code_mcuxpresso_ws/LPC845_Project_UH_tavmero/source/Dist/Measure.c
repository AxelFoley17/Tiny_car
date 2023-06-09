#include "board.h"
#include "Measure.h"
#include "Sys.h"
#include "SysTimer.h"
#include "UH_HCSR-04.h"


void Measure(uint16_t * ui16_value, uint8_t * b_valid, uint8_t * b_canWrite){
	*b_valid = FALSE;
	*b_canWrite = FALSE;
	if(UH_measure(FALSE).ready){
		if(TimeoutDone1()){
			*b_canWrite = TRUE;
			if(UH_measure(FALSE).valid){
				*ui16_value = UH_measure(FALSE).value;
				*b_valid = TRUE;
			}else{
				*ui16_value = 0;
			}
			UH_measure(TRUE);       //állapotgép alaphelyzetbe állítása
			StartTimeout1(TO);      // UH mérési ciklus idő újraindítása
		}
	}
}

void FirstMeasure(){
	UH_measure(TRUE);
	PIO0_17_OFF();
	StartTimeout1(TO);
}

