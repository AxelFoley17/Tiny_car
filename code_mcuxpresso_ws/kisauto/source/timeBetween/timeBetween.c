#include "timeBetween.h"
#include "sys/sys.h"

//checks if val is between start and end
uint8_t between(uint32_t val, uint32_t start, uint32_t end){
	if(start < end && (val >= start && val <= end)
		|| start > end && (val >= start || val <= end)){
		return TRUE;
	}
	return FALSE;
}
